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

#include "gdal_priv.h"

#include "ossim/base/ossimKeywordlist.h"
#include "ossim/base/ossimString.h"
#include "ossim/ossimPluginProjectionFactory.h"
#include "ossim/imaging/ossimImageHandlerRegistry.h"
#include "ossim/ossimTileMapModel.h"
#include "ossim/projection/ossimProjectionFactoryRegistry.h"
#include "ossim/projection/ossimRpcModel.h"

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

bool
ImageKeywordlist::
convertToGDALRPC(GDALRPCInfo &rpc) const
{
  ossimKeywordlist geom_kwl;
  this->convertToOSSIMKeywordlist(geom_kwl);
  
  ossimRefPtr<ossimRpcModel> rpcModel = new ossimRpcModel;
  if (rpcModel->loadState(geom_kwl))
    {
    ossimRpcModel::rpcModelStruct ossimRpcStruct;
    rpcModel->getRpcParameters(ossimRpcStruct);
    
    if (ossimRpcStruct.type == 'B')
      {
      rpc.dfSAMP_OFF = ossimRpcStruct.sampOffset;
      rpc.dfLINE_OFF = ossimRpcStruct.lineOffset;
      rpc.dfSAMP_SCALE = ossimRpcStruct.sampScale;
      rpc.dfLINE_SCALE = ossimRpcStruct.lineScale;
      rpc.dfLAT_OFF = ossimRpcStruct.latOffset;
      rpc.dfLONG_OFF = ossimRpcStruct.lonOffset;
      rpc.dfHEIGHT_OFF = ossimRpcStruct.hgtOffset;
      rpc.dfLAT_SCALE = ossimRpcStruct.latScale;
      rpc.dfLONG_SCALE = ossimRpcStruct.lonScale;
      rpc.dfHEIGHT_SCALE = ossimRpcStruct.hgtScale;
      
      memcpy(rpc.adfLINE_NUM_COEFF, ossimRpcStruct.lineNumCoef, sizeof(double) * 20);
      memcpy(rpc.adfLINE_DEN_COEFF, ossimRpcStruct.lineDenCoef, sizeof(double) * 20);
      memcpy(rpc.adfSAMP_NUM_COEFF, ossimRpcStruct.sampNumCoef, sizeof(double) * 20);
      memcpy(rpc.adfSAMP_DEN_COEFF, ossimRpcStruct.sampDenCoef, sizeof(double) * 20);
      
      return true;
      }
    }
  
  return false;
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

  /****************************************************/
  /* First try : test the OSSIM plugins factory       */
  /****************************************************/
  /** Before, the pluginfactory was tested if the ossim one returned false.
      But in the case TSX, the images tif were considered as ossimQuickbirdTiffTileSource
      thus a TSX tif image wasn't read with TSX Model. We don't use the ossimRegisteryFactory
      because the default include factory contains ossimQuickbirdTiffTileSource. */
  ossimProjection * projection = ossimplugins::ossimPluginProjectionFactory::instance()
                                 ->createProjection(ossimFilename(filename.c_str()), 0);
  if (projection)
    {
    otbMsgDevMacro(<< "OSSIM plugin projection instantiated ! ");

    hasMetaData = projection->saveState(geom_kwl);
    otb_kwl.SetKeywordlist(geom_kwl);

    // Free memory
    delete projection;
    projection = 0;
    }

  /***********************************************/
  /* Second try : the OSSIM projection factory   */
  /***********************************************/
  if (!hasMetaData)
    {
    ossimImageHandler* handler = ossimImageHandlerRegistry::instance()
                                 ->open(ossimFilename(filename.c_str()));
    if (handler)
      {
      otbMsgDevMacro(<< "OSSIM Open Image SUCCESS ! ");

      // Add ossimPlugins model
      ossimProjectionFactoryRegistry::instance()->registerFactory(ossimplugins::ossimPluginProjectionFactory::instance());

      ossimRefPtr<ossimImageGeometry> geom = handler->getImageGeometry();
      if (geom.valid())
        {
        projection = geom->getProjection();
        if (projection)
          {
          hasMetaData = projection->saveState(geom_kwl);
          }
        }
      
      // if the handler has found a sensor model, copy the tags found
      if (hasMetaData && dynamic_cast<ossimSensorModel*>(projection))
        {
        otbMsgDevMacro(<<"OSSIM sensor projection instantiated ! ");
        otb_kwl.SetKeywordlist(geom_kwl);
        }
      else
        {
        hasMetaData = false;
        }
      // Free memory
      delete handler;
      }
    }
  
  /**********************************************************/
  /* Third try : look for external geom file and RPC tags   */
  /**********************************************************/
  if (!hasMetaData)
    {
    // If still no metadata, try the ".geom" file
    ossimFilename ossimGeomFile = ossimFilename(filename).setExtension(".geom");
    otb_kwl = ReadGeometryFromGEOMFile(ossimGeomFile);
    
    // also check any RPC tags
    ImageKeywordlist rpc_kwl = ReadGeometryFromRPCTag(filename);
     
    if (otb_kwl.HasKey("type"))
      {
      // external geom has a "type" keyword
      std::string geomType(otb_kwl.GetMetadataByKey("type"));
      ossimProjection *testProj = ossimProjectionFactoryRegistry::instance()
        ->createProjection(ossimString(geomType));
      if (dynamic_cast<ossimSensorModel*>(testProj))
        {
        // "type" keyword corresponds to a sensor model : don't erase it
        if (rpc_kwl.GetSize() > 0)
          {
          rpc_kwl.ClearMetadataByKey("type");
          }
        
        ossimKeywordlist ossim_test_kwl;
        otb_kwl.convertToOSSIMKeywordlist(ossim_test_kwl);
        testProj = ossimProjectionFactoryRegistry::instance()
          ->createProjection(ossim_test_kwl);
        if (testProj)
          {
          // external geom contains a valid sensor geometry
          hasMetaData = true;
          }
        }
      }
      
    // copy keywords found in RPC tags if the external geom is not valid
    if (!hasMetaData && rpc_kwl.GetSize() > 0)
      {
      const ImageKeywordlist::KeywordlistMap &kwlMap = rpc_kwl.GetKeywordlist();
      for (ImageKeywordlist::KeywordlistMap::const_iterator it = kwlMap.begin();
          it != kwlMap.end();
          ++it)
        {
        if (it->second != "")
          {
          otb_kwl.AddKey(it->first , it->second);
          }
        }
      hasMetaData = true;
      }
    }

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
  
  if (!hasMetaData)
    {
    otbMsgDevMacro(<< "OSSIM MetaData not present ! ");
    }
  else
    {
    otbMsgDevMacro(<< "OSSIM MetaData present ! ");
    //otbMsgDevMacro(<< geom_kwl);
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

ImageKeywordlist
ReadGeometryFromRPCTag(const std::string& filename)
{
  ossimKeywordlist geom_kwl;
  ImageKeywordlist otb_kwl;
  
  //  try to use GeoTiff RPC tag if present.
  // Warning : RPC in subdatasets are not supported
  GDALDatasetH identifyDriverH = GDALIdentifyDriver(filename.c_str(), NULL);
  if(identifyDriverH == NULL)
    {
    // If no driver has identified the dataset, don't try to open it and exit
    return otb_kwl;
    }
  
  GDALDatasetH datasetH = GDALOpen(filename.c_str(), GA_ReadOnly);
  if (datasetH != NULL)
    {
    GDALDataset* dataset = static_cast<GDALDataset*>(datasetH);
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
      
      if (rpcModel->saveState(geom_kwl))
        {
        otb_kwl.SetKeywordlist(geom_kwl);
        }
      }
    GDALClose(datasetH);
    }
  
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
