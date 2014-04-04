/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbImageKeywordlist.h"

#include <cassert>

#include "otbMacro.h"

#include "otbGDALDriverManagerWrapper.h"

#include "ossim/base/ossimKeywordlist.h"
#include "ossim/base/ossimString.h"
#include "ossim/ossimPluginProjectionFactory.h"
#include "ossim/imaging/ossimImageHandlerRegistry.h"
#include "ossim/ossimTileMapModel.h"
#include "ossim/projection/ossimProjectionFactoryRegistry.h"

#include "ossim/imaging/ossimTiffTileSource.h"
#include "ossim/projection/ossimRpcModel.h"
#include "tiffio.h"

#include "otbSensorModelAdapter.h"

namespace otb
{

ImageKeywordlist
::ImageKeywordlist()
{
}

ImageKeywordlist
::ImageKeywordlist(const Self& p) : m_Keywordlist(p.m_Keywordlist)
{
}

ImageKeywordlist
::~ImageKeywordlist()
{
}

void
ImageKeywordlist::
operator =(const Self& p)
{
  m_Keywordlist = p.m_Keywordlist;
}

void
ImageKeywordlist::
SetKeywordlist(const ossimKeywordlist& kwl)
{
  m_Keywordlist.clear();
  for (ossimKeywordlist::KeywordMap::const_iterator it = kwl.getMap().begin();
       it != kwl.getMap().end();
       ++it)
    {
    std::string first(it->first);
    std::string second(it->second);
    m_Keywordlist[first] = second;
    }
}

const std::string&
ImageKeywordlist::
GetMetadataByKey(const std::string& key) const
{
  // Search for the key in the output map
  KeywordlistMap::const_iterator it = m_Keywordlist.find(key);

  // If the key can not be found, throw an exception
  if (it == m_Keywordlist.end())
    {
    itkExceptionMacro(<< "Keywordlist has no output with key " << key);
    }

  // Then if everything is ok, return the ossinString
  return it->second;
}

bool
ImageKeywordlist::
HasKey(const std::string& key) const
{
  KeywordlistMap::const_iterator it = m_Keywordlist.find(key);
  return (it != m_Keywordlist.end());
}

void
ImageKeywordlist::
ClearMetadataByKey(const std::string& key)
{
  m_Keywordlist[key] = "";
}

void
ImageKeywordlist::
AddKey(const std::string& key, const std::string& value)
{
  m_Keywordlist[key] = value;
}

void
ImageKeywordlist::
convertToOSSIMKeywordlist(ossimKeywordlist& kwl) const
{
  ossimKeywordlist::KeywordMap ossimMap;
  for(KeywordlistMap::const_iterator it = m_Keywordlist.begin();
      it != m_Keywordlist.end();
      ++it)
    {
    ossimMap[it->first] = it->second;
    }
  kwl.getMap() = ossimMap;
}

void
ImageKeywordlist::
Print(std::ostream& os, itk::Indent indent) const
{
  this->PrintSelf(os, indent.GetNextIndent());
}

void
ImageKeywordlist::
PrintSelf(std::ostream& os, itk::Indent indent) const
{
  ossimKeywordlist kwl;
  convertToOSSIMKeywordlist(kwl);
  os << indent << " Ossim Keyword list:" << std::endl;
  os << indent << kwl;
}

std::ostream &
operator <<(std::ostream& os, const ImageKeywordlist& kwl)
{
  kwl.Print(os);
  return os;
}

ImageKeywordlist
ReadGeometry(const std::string& filename)
{
  return ReadGeometryFromImage(filename);
}

ImageKeywordlist
ReadGeometryFromImage(const std::string& filename)
{
  // Trying to read ossim MetaData
  bool             hasMetaData = false;
  ossimKeywordlist geom_kwl; // = new ossimKeywordlist();
  ImageKeywordlist otb_kwl;

  // Test the plugins factory
  /** Before, the pluginfactory was tested if the ossim one returned false.
      But in the case TSX, the images tif were considered as ossimQuickbirdTiffTileSource
      thus a TSX tif image wasn't read with TSX Model. We don't use the ossimRegisteryFactory
      because the default include factory contains ossimQuickbirdTiffTileSource. */
  ossimProjection * projection = ossimplugins::ossimPluginProjectionFactory::instance()
                                 ->createProjection(ossimFilename(filename.c_str()), 0);

  if (!projection)
    {
    otbMsgDevMacro(<< "OSSIM Instantiate projection FAILED ! ");
    }
  else
    {
    otbMsgDevMacro(<< "OSSIM Instantiate projection SUCCESS ! ");

    hasMetaData = projection->saveState(geom_kwl);
    otb_kwl.SetKeywordlist(geom_kwl);

    // Free memory
    delete projection;
    }

  if (!hasMetaData)
    {
    ossimImageHandler* handler = ossimImageHandlerRegistry::instance()
                                 ->open(ossimFilename(filename.c_str()));
    if (!handler)
      {
      otbMsgDevMacro(<< "OSSIM Open Image FAILED ! ");

      // In some corner cases, ossim is unable to create a ossimImageHandler
      // instance for the provided file.
      // This can happen for TIFF+geom files where the ossimTiffTileSource
      // fails when trying to open the TIFF file (for example
      // because ossim is compiled with a libtiff without BigTIFF support
      // and when the actual TIFF file exceeds the Large File limit).
      // In such cases, we don't actually need ossim to interpret the TIFF file,
      // and just want a valid ossimKeywordlist corresponding to a sensor model.

      // Here is implemented a shortcut used as a fallback scenario,
      // where we bypass the ossimImageHandlerRegistry
      // and directly create a ossimKeywordlist from the ".geom" file.

      // We then verify it is a valid sensor model by using otb::SensorModelAdapter
      // which uses ossimSensorModelFactory and ossimPluginProjectionFactory internally,
      // thus by-passing the need for a valid ossimImageHandler.

      // Try to find a ".geom" file next to 'filename'
      ossimFilename ossimGeomFile = ossimFilename(filename).setExtension(".geom");

      otb_kwl = ReadGeometryFromGEOMFile(ossimGeomFile);
      }
    else
      {
      otbMsgDevMacro(<< "OSSIM Open Image SUCCESS ! ");

      // Add ossimPlugins model
      ossimProjectionFactoryRegistry::instance()->registerFactory(ossimplugins::ossimPluginProjectionFactory::instance());

      //     hasMetaData = handler->getImageGeometry(geom_kwl);
      ossimRefPtr<ossimImageGeometry> geom = handler->getImageGeometry();

      if (geom.valid())
        {
        ossimProjection* projection = geom->getProjection();
        if (projection)
          {
          hasMetaData = projection->saveState(geom_kwl);
          otb_kwl.SetKeywordlist(geom_kwl);
          }
        }
      
      //  try to use GeoTiff RPC tag if present. They will override the
      // standard projection found before
      // Warning : RPC in subdatasets are not supported
      GDALDatasetWrapper::Pointer gdalDatasetWrapper =
        GDALDriverManagerWrapper::GetInstance().Open(handler->getFilename());
      
      if (gdalDatasetWrapper.IsNotNull())
        {
        GDALDataset* dataset = gdalDatasetWrapper->GetDataSet();
        GDALRPCInfo rpcStruct;
        if (GDALExtractRPCInfo(dataset->GetMetadata("RPC"),&rpcStruct))
          {
          otbMsgDevMacro(<<"RPC Coeff found");
          std::vector<double> lineNumCoefs;
          std::vector<double> lineDenCoefs;
          std::vector<double> sampNumCoefs;
          std::vector<double> sampDenCoefs;
          
          for (unsigned int k=0; k<20; ++k)
            {
            lineNumCoefs.push_back(rpcStruct.adfLINE_NUM_COEFF[k]);
            lineDenCoefs.push_back(rpcStruct.adfLINE_DEN_COEFF[k]);
            sampNumCoefs.push_back(rpcStruct.adfSAMP_NUM_COEFF[k]);
            sampDenCoefs.push_back(rpcStruct.adfSAMP_DEN_COEFF[k]);
            }
          
          ossimRefPtr<ossimRpcModel> rpcModel = new ossimRpcModel;
          rpcModel->setAttributes( rpcStruct.dfSAMP_OFF,
                                   rpcStruct.dfLINE_OFF,
                                   rpcStruct.dfSAMP_SCALE,
                                   rpcStruct.dfLINE_SCALE,
                                   rpcStruct.dfLAT_OFF,
                                   rpcStruct.dfLONG_OFF,
                                   rpcStruct.dfHEIGHT_OFF,
                                   rpcStruct.dfLAT_SCALE,
                                   rpcStruct.dfLONG_SCALE,
                                   rpcStruct.dfHEIGHT_SCALE,
                                   sampNumCoefs,
                                   sampDenCoefs,
                                   lineNumCoefs,
                                   lineDenCoefs);
          
          double errorBias = 0.0;
          double errorRand = 0.0;
          
          // setup other metadata
          rpcModel->setPositionError(errorBias,errorRand,true);
          ossimDrect rectangle(0.0,
                               0.0,
                               static_cast<double>(dataset->GetRasterXSize()-1),
                               static_cast<double>(dataset->GetRasterYSize()-1));
          rpcModel->setImageRect(rectangle);
          
          ossimDpt size;
          size.line = rectangle.height();
          size.samp = rectangle.width();
          rpcModel->setImageSize(size);
          
          // Compute 4 corners and reference point
          rpcModel->updateModel();
          double heightOffset = rpcStruct.dfHEIGHT_OFF;
          ossimGpt ulGpt, urGpt, lrGpt, llGpt;
          ossimGpt refGndPt;
          
          rpcModel->lineSampleHeightToWorld(rectangle.ul(), heightOffset, ulGpt);
          rpcModel->lineSampleHeightToWorld(rectangle.ur(), heightOffset, urGpt);
          rpcModel->lineSampleHeightToWorld(rectangle.lr(), heightOffset, lrGpt);
          rpcModel->lineSampleHeightToWorld(rectangle.ll(), heightOffset, llGpt);
          rpcModel->setGroundRect(ulGpt,urGpt,lrGpt,llGpt);
          
          rpcModel->lineSampleHeightToWorld(rectangle.midPoint(), heightOffset, refGndPt);
          rpcModel->setRefGndPt(refGndPt);
          
          // compute ground sampling distance
          try
            {
            // Method can throw ossimException.
            rpcModel->computeGsd();
            }
          catch (const ossimException& e)
            {
            otbMsgDevMacro(<< "OSSIM Compute ground sampling distance FAILED ! ");
            }
          
          hasMetaData = rpcModel->saveState(geom_kwl);
          otb_kwl.SetKeywordlist(geom_kwl);
          }
        }
      
      /*
      ossimRefPtr<ossimTiffTileSource> tiffHandler = new ossimTiffTileSource;
      tiffHandler->setOpenOverviewFlag(true);
      if ( tiffHandler->open(handler->getFilename()) )
        {
        TIFF* tiffPtr = tiffHandler->tiffPtr();
        unsigned short  count     = 0;
        void            *tagData  = 0;
        
        if (TIFFGetField( tiffPtr, 50844, &count, &tagData) && (count == 92) )
          {
          otbMsgDevMacro(<<"RPC Coeff tag found");
          // read RPC coefficients
          double *tagValues = static_cast<double*>(tagData);
          
          std::vector<double> lineNumCoefs;
          std::vector<double> lineDenCoefs;
          std::vector<double> sampNumCoefs;
          std::vector<double> sampDenCoefs;
          
          for (unsigned int k=0; k<20; ++k)
            {
            lineNumCoefs.push_back(*(tagValues + 12 + k));
            lineDenCoefs.push_back(*(tagValues + 32 + k));
            sampNumCoefs.push_back(*(tagValues + 52 + k));
            sampDenCoefs.push_back(*(tagValues + 72 + k));
            }
          
          ossimRefPtr<ossimRpcModel> rpcModel = new ossimRpcModel;
          rpcModel->setAttributes( *(tagValues + 3),
                                   *(tagValues + 2),
                                   *(tagValues + 8),
                                   *(tagValues + 7),
                                   *(tagValues + 4),
                                   *(tagValues + 5),
                                   *(tagValues + 6),
                                   *(tagValues + 9),
                                   *(tagValues + 10),
                                   *(tagValues + 11),
                                   sampNumCoefs,
                                   sampDenCoefs,
                                   lineNumCoefs,
                                   lineDenCoefs);
          
          double errorBias = 0.0;
          double errorRand = 0.0;
          if (*(tagValues) >= 0.0) errorBias = *(tagValues);
          if (*(tagValues+1) >= 0.0) errorRand = *(tagValues+1);
          
          // setup other metadata
          rpcModel->setPositionError(errorBias,errorRand,true);
          ossimDrect rectangle = tiffHandler->getImageRectangle();
          rpcModel->setImageRect(rectangle);
          
          ossimDpt size;
          size.line = rectangle.height();
          size.samp = rectangle.width();
          rpcModel->setImageSize(size);
          
          // Compute 4 corners and reference point
          rpcModel->updateModel();
          double heightOffset = *(tagValues + 6);
          ossimGpt ulGpt, urGpt, lrGpt, llGpt;
          ossimGpt refGndPt;
          
          rpcModel->lineSampleHeightToWorld(rectangle.ul(), heightOffset, ulGpt);
          rpcModel->lineSampleHeightToWorld(rectangle.ur(), heightOffset, urGpt);
          rpcModel->lineSampleHeightToWorld(rectangle.lr(), heightOffset, lrGpt);
          rpcModel->lineSampleHeightToWorld(rectangle.ll(), heightOffset, llGpt);
          rpcModel->setGroundRect(ulGpt,urGpt,lrGpt,llGpt);
          
          rpcModel->lineSampleHeightToWorld(rectangle.midPoint(), heightOffset, refGndPt);
          rpcModel->setRefGndPt(refGndPt);
          
          // compute ground sampling distance
          try
            {
            // Method can throw ossimException.
            rpcModel->computeGsd();
            }
          catch (const ossimException& e)
            {
            otbMsgDevMacro(<< "OSSIM Compute ground sampling distance FAILED ! ");
            }
          
          hasMetaData = rpcModel->saveState(geom_kwl);
          otb_kwl.SetKeywordlist(geom_kwl);
          }
        }
      */
      // If still no metadata, try the ".geom" file
      if (!hasMetaData && otb_kwl.GetSize() == 0)
        {
        ossimFilename ossimGeomFile = ossimFilename(filename).setExtension(".geom");
        otb_kwl = ReadGeometryFromGEOMFile(ossimGeomFile);
        }

      }
    // Free memory
    delete handler;
    }

  if (!hasMetaData)
    {
    otbMsgDevMacro(<< "OSSIM MetaData not present ! ");
    }
  else
    {
    otbMsgDevMacro(<< "OSSIM MetaData present ! ");
    otbMsgDevMacro(<< geom_kwl);
    }

  return otb_kwl;
}

ImageKeywordlist
ReadGeometryFromGEOMFile(const std::string& filename)
{
  ossimKeywordlist geom_kwl;
  ImageKeywordlist otb_kwl;
  ossimFilename ossimGeomFile = ossimFilename(filename);

  if (ossimGeomFile.exists() && ossimGeomFile.isFile())
    {
    // Interpret the geom file as a KWL
    ossimKeywordlist kwl(ossimGeomFile);

    // Check that the geom file results in a valid ossimKeywordlist
    if (kwl.getErrorStatus() == ossimErrorCodes::OSSIM_OK)
      {
//       // Be sure there is a corresponding instance of ossimSensorModel
//       // which understands this kwl
//       SensorModelAdapter::Pointer sensorModel = SensorModelAdapter::New();
//       ImageKeywordlist otbkwl;
//       otbkwl.SetKeywordlist(kwl);
//       sensorModel->CreateProjection(otbkwl);
//
//       if (sensorModel->IsValidSensorModel())
//         {
//         geom_kwl = kwl;
//         }
      geom_kwl = kwl;
      }
    }

  otb_kwl.SetKeywordlist(geom_kwl);

  return otb_kwl;
}

void
WriteGeometry(const ImageKeywordlist& otb_kwl, const std::string& filename)
{
  // Write the image keyword list if any
  ossimKeywordlist geom_kwl;
  otb_kwl.convertToOSSIMKeywordlist(geom_kwl);

  if (geom_kwl.getSize() > 0)
    {
    otbMsgDevMacro(<< "Exporting keywordlist ...");
    ossimFilename geomFileName(filename);
    geomFileName.setExtension(".geom");
    geom_kwl.write(geomFileName.chars());
    }
}


} //namespace otb
