/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbImageKeywordlist.h"
#include "otbConfigurationManager.h"

#include <cassert>
#include <itksys/SystemTools.hxx>

#include "otbMacro.h"

#include "gdal_priv.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Waddress"
#if defined(__GNUC__) && (__GNUC__ > 5)
#pragma GCC diagnostic ignored "-Wnonnull-compare"
#endif
#include "ossim/base/ossimKeywordlist.h"
#include "ossim/base/ossimString.h"
#include "ossim/ossimPluginProjectionFactory.h"
#include "ossim/imaging/ossimImageHandlerRegistry.h"
#include "ossim/ossimTileMapModel.h"
#include "ossim/projection/ossimProjectionFactoryRegistry.h"
#include "ossim/projection/ossimRpcModel.h"
#include "ossim/base/ossimNotify.h"
#pragma GCC diagnostic pop
#else
#include "ossim/base/ossimKeywordlist.h"
#include "ossim/base/ossimString.h"
#include "ossim/ossimPluginProjectionFactory.h"
#include "ossim/imaging/ossimImageHandlerRegistry.h"
#include "ossim/ossimTileMapModel.h"
#include "ossim/projection/ossimProjectionFactoryRegistry.h"
#include "ossim/projection/ossimRpcModel.h"
#include "ossim/base/ossimNotify.h"
#endif

//#include "otbSensorModelAdapter.h"
#include <memory>
#include <boost/scoped_ptr.hpp>
#include <boost/algorithm/string/predicate.hpp>

namespace otb
{

ImageKeywordlist::ImageKeywordlist()
{
}

ImageKeywordlist::ImageKeywordlist(const Self& p) : m_Keywordlist(p.m_Keywordlist)
{
}

ImageKeywordlist::~ImageKeywordlist()
{
}

void ImageKeywordlist::operator=(const Self& p)
{
  m_Keywordlist = p.m_Keywordlist;
}

bool ImageKeywordlist::operator==(const Self& p) const
{
  return m_Keywordlist == p.m_Keywordlist;
}

void ImageKeywordlist::SetKeywordlist(const ossimKeywordlist& kwl)
{
  m_Keywordlist = kwl.getMap();
}

const std::string& ImageKeywordlist::GetMetadataByKey(const std::string& key) const
{
  // Search for the key in the output map
  KeywordlistMap::const_iterator it = m_Keywordlist.find(key);

  // If the key can not be found, throw an exception
  if (it == m_Keywordlist.end())
  {
    itkGenericExceptionMacro(<< "Keywordlist has no output with key " << key);
  }

  // Then if everything is ok, return the ossimString
  return it->second;
}

bool ImageKeywordlist::HasKey(const std::string& key) const
{
  KeywordlistMap::const_iterator it = m_Keywordlist.find(key);

  return (it != m_Keywordlist.end());
}

void ImageKeywordlist::ClearMetadataByKey(const std::string& key)
{
  m_Keywordlist[key] = "";
}

void ImageKeywordlist::AddKey(const std::string& key, const std::string& value)
{
  m_Keywordlist[key] = value;
}

void ImageKeywordlist::convertToOSSIMKeywordlist(ossimKeywordlist& kwl) const
{
  kwl.getMap() = m_Keywordlist;
}

bool ImageKeywordlist::convertToGDALRPC(GDALRPCInfo& rpc) const
{
  /* ossimRpcModel::loadState() actually expects certain keyword values to be
   * present in the keywordlist. So We check a single keyword value
   * (polynomial_format) first.  Even though it is not enough to ensure a valid
   * ossimRpcModel by checking for presence of one single key but atleast we
   * are sure about not to create an ossimRpcModel.
   *
   * The current mechanism creates ossimRpcModel instance, calls loadState()
   * and fails. The below check for 'polynomial_format' save us from creating
   * an ossimRpcModel which will be invalid if the 'polynomial_format' is not
   * present.
   */
  if (m_Keywordlist.find("polynomial_format") != m_Keywordlist.end())
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
        rpc.dfSAMP_OFF     = ossimRpcStruct.sampOffset;
        rpc.dfLINE_OFF     = ossimRpcStruct.lineOffset;
        rpc.dfSAMP_SCALE   = ossimRpcStruct.sampScale;
        rpc.dfLINE_SCALE   = ossimRpcStruct.lineScale;
        rpc.dfLAT_OFF      = ossimRpcStruct.latOffset;
        rpc.dfLONG_OFF     = ossimRpcStruct.lonOffset;
        rpc.dfHEIGHT_OFF   = ossimRpcStruct.hgtOffset;
        rpc.dfLAT_SCALE    = ossimRpcStruct.latScale;
        rpc.dfLONG_SCALE   = ossimRpcStruct.lonScale;
        rpc.dfHEIGHT_SCALE = ossimRpcStruct.hgtScale;

        memcpy(rpc.adfLINE_NUM_COEFF, ossimRpcStruct.lineNumCoef, sizeof(double) * 20);
        memcpy(rpc.adfLINE_DEN_COEFF, ossimRpcStruct.lineDenCoef, sizeof(double) * 20);
        memcpy(rpc.adfSAMP_NUM_COEFF, ossimRpcStruct.sampNumCoef, sizeof(double) * 20);
        memcpy(rpc.adfSAMP_DEN_COEFF, ossimRpcStruct.sampDenCoef, sizeof(double) * 20);

        return true;
      }
    }
  }
  return false;
}

void ImageKeywordlist::Print(std::ostream& os, itk::Indent indent) const
{
  this->PrintSelf(os, indent.GetNextIndent());
}

void ImageKeywordlist::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  ossimKeywordlist kwl;
  convertToOSSIMKeywordlist(kwl);
  os << indent << " Ossim Keyword list:" << std::endl;
  os << indent << kwl;
}

std::ostream& operator<<(std::ostream& os, const ImageKeywordlist& kwl)
{
  kwl.Print(os);
  return os;
}

ImageKeywordlist ReadGeometry(const std::string& filename)
{
  return ReadGeometryFromImage(filename);
}

ImageKeywordlist ReadGeometryFromImage(const std::string& filename, bool checkRpcTag)
{
  // Trying to read ossim MetaData
  bool             hasMetaData = false;
  ossimKeywordlist geom_kwl; // = new ossimKeywordlist();
  ImageKeywordlist otb_kwl;

  // Save Ossim notify flags
  ossimPushNotifyFlags();

  // Except for DEBUG logger level, only fatal errors of OSSIM should be seen
  if (otb::ConfigurationManager::GetLoggerLevel() < itk::LoggerBase::DEBUG)
  {
    ossimSetNotifyFlag(ossimNotifyFlags_FATAL);
  }

  /****************************************************/
  /* First try : test the OSSIM plugins factory       */
  /****************************************************/
  {
    /** Before, the pluginfactory was tested if the ossim one returned false.
      But in the case TSX, the images tif were considered as ossimQuickbirdTiffTileSource
      thus a TSX tif image wasn't read with TSX Model. We don't use the ossimRegisteryFactory
      because the default include factory contains ossimQuickbirdTiffTileSource. */
    boost::scoped_ptr<ossimProjection> projection(ossimplugins::ossimPluginProjectionFactory::instance()->createProjection(ossimFilename(filename.c_str()), 0));

    if (projection)
    {
      hasMetaData = projection->saveState(geom_kwl);
      otb_kwl.SetKeywordlist(geom_kwl);
    }
  }

  /***********************************************/
  /* Second try : the OSSIM projection factory   */
  /***********************************************/
  if (!hasMetaData)
  {
    boost::scoped_ptr<ossimImageHandler> handler(ossimImageHandlerRegistry::instance()->open(ossimFilename(filename.c_str())));
    if (handler)
    {

      // Add ossimPlugins model
      ossimProjectionFactoryRegistry::instance()->registerFactory(ossimplugins::ossimPluginProjectionFactory::instance());

      ossimRefPtr<ossimImageGeometry> geom = handler->getImageGeometry();
      if (geom.valid())
      {
        ossimProjection const* projection = geom->getProjection();
        if (projection)
        {
          hasMetaData = projection->saveState(geom_kwl);

          // if the handler has found a sensor model, copy the tags found
          if (hasMetaData && dynamic_cast<ossimSensorModel const*>(projection))
          {
            otb_kwl.SetKeywordlist(geom_kwl);
            // geom_kwl.print(std::cout);
          }
          else
          {
            hasMetaData = false;
          }
        } // projection
      }   // geom.valid
    }     // handler
  }

  /**********************************************************/
  /* Third try : look for RPC tags   */
  /**********************************************************/
  if (!hasMetaData && checkRpcTag)
  {
    // check any RPC tags
    otb_kwl = ReadGeometryFromRPCTag(filename);

    if (!otb_kwl.Empty())
    {
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

  // Restore Ossim notify flags
  ossimPopNotifyFlags();

  return otb_kwl;
}

ImageKeywordlist ReadGeometryFromGEOMFile(const std::string& filename)
{
  ossimKeywordlist geom_kwl;
  ImageKeywordlist otb_kwl;
  ossimFilename    ossimGeomFile = ossimFilename(filename);

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

ImageKeywordlist ReadGeometryFromRPCTag(const std::string& filename)
{
  ossimKeywordlist geom_kwl;
  ImageKeywordlist otb_kwl;

  // Don't call GDALIdentifyDriver on a hdr file because this makes the ENVI driver throw an error:
  // "ERROR 1: The selected file is an ENVI header file, but to open ENVI datasets, the data file 
  // should be selected instead of the .hdr file. Please try again selecting the data file corresponding 
  // to the header file"
  // No driver can open hdr file anyway.
  std::string extension = itksys::SystemTools::GetFilenameLastExtension(filename);
  if (boost::iequals(extension, ".hdr"))
  {
    return otb_kwl;
  }

  //  try to use GeoTiff RPC tag if present.
  // Warning : RPC in subdatasets are not supported
  GDALDriverH identifyDriverH = GDALIdentifyDriver(filename.c_str(), nullptr);
  if (identifyDriverH == nullptr)
  {
    // If no driver has identified the dataset, don't try to open it and exit
    return otb_kwl;
  }

  GDALDatasetH datasetH = GDALOpen(filename.c_str(), GA_ReadOnly);
  if (datasetH != nullptr)
  {
    GDALDataset* dataset = static_cast<GDALDataset*>(datasetH);
    GDALRPCInfo  rpcStruct;
    if (GDALExtractRPCInfo(dataset->GetMetadata("RPC"), &rpcStruct))
    {
      otbMsgDevMacro(<< "RPC Coeff found");
      std::vector<double> lineNumCoefs;
      std::vector<double> lineDenCoefs;
      std::vector<double> sampNumCoefs;
      std::vector<double> sampDenCoefs;

      for (unsigned int k = 0; k < 20; ++k)
      {
        lineNumCoefs.push_back(rpcStruct.adfLINE_NUM_COEFF[k]);
        lineDenCoefs.push_back(rpcStruct.adfLINE_DEN_COEFF[k]);
        sampNumCoefs.push_back(rpcStruct.adfSAMP_NUM_COEFF[k]);
        sampDenCoefs.push_back(rpcStruct.adfSAMP_DEN_COEFF[k]);
      }

      ossimRefPtr<ossimRpcModel> rpcModel = new ossimRpcModel;
      rpcModel->setAttributes(rpcStruct.dfSAMP_OFF, rpcStruct.dfLINE_OFF, rpcStruct.dfSAMP_SCALE, rpcStruct.dfLINE_SCALE, rpcStruct.dfLAT_OFF,
                              rpcStruct.dfLONG_OFF, rpcStruct.dfHEIGHT_OFF, rpcStruct.dfLAT_SCALE, rpcStruct.dfLONG_SCALE, rpcStruct.dfHEIGHT_SCALE,
                              sampNumCoefs, sampDenCoefs, lineNumCoefs, lineDenCoefs);

      double errorBias = 0.0;
      double errorRand = 0.0;

      // setup other metadata
      rpcModel->setPositionError(errorBias, errorRand, true);
      ossimDrect rectangle(0.0, 0.0, static_cast<double>(dataset->GetRasterXSize() - 1), static_cast<double>(dataset->GetRasterYSize() - 1));
      rpcModel->setImageRect(rectangle);

      ossimDpt size;
      size.line = rectangle.height();
      size.samp = rectangle.width();
      rpcModel->setImageSize(size);

      // Compute 4 corners and reference point
      rpcModel->updateModel();
      double   heightOffset = rpcStruct.dfHEIGHT_OFF;
      ossimGpt ulGpt, urGpt, lrGpt, llGpt;
      ossimGpt refGndPt;

      rpcModel->lineSampleHeightToWorld(rectangle.ul(), heightOffset, ulGpt);
      rpcModel->lineSampleHeightToWorld(rectangle.ur(), heightOffset, urGpt);
      rpcModel->lineSampleHeightToWorld(rectangle.lr(), heightOffset, lrGpt);
      rpcModel->lineSampleHeightToWorld(rectangle.ll(), heightOffset, llGpt);
      rpcModel->setGroundRect(ulGpt, urGpt, lrGpt, llGpt);

      rpcModel->lineSampleHeightToWorld(rectangle.midPoint(), heightOffset, refGndPt);
      rpcModel->setRefGndPt(refGndPt);

      // compute ground sampling distance
      try
      {
        // Method can throw ossimException.
        rpcModel->computeGsd();
      }
      catch (const ossimException& itkNotUsed(e))
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

void WriteGeometry(const ImageKeywordlist& otb_kwl, const std::string& filename)
{
  // Write the image keyword list if any
  ossimKeywordlist geom_kwl;
  otb_kwl.convertToOSSIMKeywordlist(geom_kwl);

  if (geom_kwl.getSize() > 0)
  {
    ossimFilename geomFileName(filename);
    geomFileName.setExtension(".geom");
    geom_kwl.write(geomFileName.chars());
  }
}

bool ImageKeywordlist::ReadGeometry(const std::string & itkNotUsed(path))
{
  // The geom files used in OTB are simpler than what OSSIM actually supports
  // It is basically a list of 'key: value' lines
  // if the value contains special character like \n, \r, or if it has whitespaces
  // at the beginning or at the end, the value is enclosed in triple double-quotes
  // ("""value""")
  // TODO
  return false;
}

void ImageKeywordlist::WriteGeometry(const std::string& itkNotUsed(filename)) const
{
  // Write the image keyword list if any (without using ossimKeywordlist)
  // values in the keywordlist matching the regex '\n|\r| $|^ ' must be
  // encapsulated with triple double-quotes (""")
  // TODO
}

} // namespace otb
