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

#include "otbDEMHandler.h"
#include "otbGDALDriverManagerWrapper.h"
#include "boost/filesystem.hpp"
#include <boost/range/iterator_range.hpp>
#include "gdal_utils.h"

//Warp utility
#include "gdalwarper.h"
#include "vrtdataset.h"

//TODO C++ 17 : use std::optional instead
#include <boost/optional.hpp>

// TODO : RemoveOSSIM
#include <otbOssimDEMHandler.h>

#include <mutex>

#include "ogr_spatialref.h"

namespace otb {

namespace DEMDetails {

// Adapted from boost filesystem documentation : https://www.boost.org/doc/libs/1_53_0/libs/filesystem/doc/reference.html
std::vector<std::string> GetFilesInDirectory(const std::string & directoryPath)
{
  std::vector<std::string> fileList;
  
  if ( !boost::filesystem::exists( directoryPath) )
  {
    return fileList;
  }
  else if (!boost::filesystem::is_directory(directoryPath))
  {
    fileList.push_back(directoryPath);
    return fileList;
  }
  
  // End iterator : default construction yields past-the-end
  for ( const auto & item : boost::make_iterator_range(boost::filesystem::directory_iterator(directoryPath), {}) )
  {
    if ( boost::filesystem::is_directory(item.status()) )
    {
      auto subDirList = GetFilesInDirectory(item.path().string());
      fileList.insert(fileList.end(), subDirList.begin(), subDirList.end());
    }
    else
    {
      fileList.push_back(item.path().string());
    }
  }

  return fileList;
}

std::mutex demMutex;

boost::optional<double> GetDEMValue(double lon, double lat, GDALDataset& ds)
{
  const std::lock_guard<std::mutex> lock(demMutex);

#if GDAL_VERSION_NUM >= 3000000
  auto srs = ds.GetSpatialRef();
#else
  auto projRef = ds.GetProjectionRef();
  
  std::unique_ptr<OGRSpatialReference> srsUniquePtr;
  OGRSpatialReference* srs = nullptr;
  // GetProjectionRef() returns an empty non null string if no projection is available
  if (strlen(projRef) != 0 )
  {
    srsUniquePtr = std::make_unique<OGRSpatialReference>(ds.GetProjectionRef());
    srs = srsUniquePtr.get();
  }
#endif

  auto wgs84Srs = OGRSpatialReference::GetWGS84SRS();

  // Convert input lon lat into the coordinates defined by the dataset if needed.
  if (srs && !srs->IsSame(wgs84Srs))
  {
    auto poCT = std::unique_ptr<OGRCoordinateTransformation>
                    (OGRCreateCoordinateTransformation(wgs84Srs, srs));

    if (poCT && !poCT->Transform( 1, &lon, &lat ) )
    {
      return boost::none;
    }
  }

  double geoTransform[6];
  
  ds.GetGeoTransform(geoTransform);

  auto x = (lon - geoTransform[0]) / geoTransform[1] - 0.5;
  auto y = (lat - geoTransform[3]) / geoTransform[5] - 0.5;

  if (x < 0 || y < 0 || x > ds.GetRasterXSize() || y > ds.GetRasterYSize())
  {
    return boost::none;
  }

  auto x_int = static_cast<int>(x);
  auto y_int = static_cast<int>(y);

  auto deltaX = x - x_int;
  auto deltaY = y - y_int;

  if (x < 0 || y < 0 || x+1 > ds.GetRasterXSize() || y+1 > ds.GetRasterYSize())
  {
    return boost::none;
  }
  
  // Bilinear interpolation.
  double elevData[4];
  
  auto err = ds.GetRasterBand(1)->RasterIO( GF_Read, x_int, y_int, 2, 2,
              elevData, 2, 2, GDT_Float64,
              0, 0, nullptr);

  if (err)
  {
    return boost::none;
  }

  // Test for no data. Don't return a value if one pixel 
  // of the interpolation is no data.
  for (int i =0; i<4; i++)
  {
    if (elevData[i] == ds.GetRasterBand(1)->GetNoDataValue())
    {
      return boost::none;
    }
  }

  auto xBil1 = elevData[0] * (1-deltaX) + elevData[1] * deltaX;
  auto xBil2 = elevData[2] * (1-deltaX) + elevData[3] * deltaX;

  auto yBil = xBil1 * (1.0 - deltaY) + xBil2 * deltaY;
  return yBil;
}

}  // namespace DEMDetails

// Meyer singleton design pattern
DEMHandler & DEMHandler::GetInstance() 
{
  static DEMHandler s_instance;
  return s_instance;
}

DEMHandler::DEMHandler() : m_Dataset(nullptr),
                           m_GeoidDS(nullptr),
                           m_DefaultHeightAboveEllipsoid(0.0)
{
  GDALAllRegister();
};

DEMHandler::~DEMHandler()
{
  if (m_GeoidDS)
  {
    GDALClose(m_GeoidDS);
  }

  ClearDEMs();

  VSIUnlink(DEM_DATASET_PATH.c_str());
  VSIUnlink(DEM_WARPED_DATASET_PATH.c_str());
  VSIUnlink(DEM_SHIFTED_DATASET_PATH.c_str());
}

void DEMHandler::OpenDEMFile(const std::string& path)
{
  m_DatasetList.push_back(otb::GDALDriverManagerWrapper::GetInstance().Open(path));
  std::vector<GDALDatasetH> vrtDatasetList(1);
  vrtDatasetList[0] = m_DatasetList[0]->GetDataSet();
  auto close_me = GDALBuildVRT(DEM_DATASET_PATH.c_str(), 1, vrtDatasetList.data(),
                               nullptr, nullptr, nullptr);
  // Need to close the dataset, so it is flushed into memory.
  GDALClose(close_me);
  m_Dataset = static_cast<GDALDataset*>(GDALOpen(DEM_DATASET_PATH.c_str(), GA_ReadOnly));
  m_DEMDirectories.push_back(path);

  if(m_GeoidDS)
  {
    CreateShiftedDataset();
  }

  Notify();
}

void DEMHandler::OpenDEMDirectory(const std::string& DEMDirectory)
{
  // TODO : RemoveOSSIM
  OssimDEMHandler::Instance()->OpenDEMDirectory(DEMDirectory);

  // Free the previous in-memory dataset (if any)
  if (!m_DatasetList.empty())
    VSIUnlink(DEM_DATASET_PATH.c_str());

  auto demFiles = DEMDetails::GetFilesInDirectory(DEMDirectory);
  for (const auto & file : demFiles)
  {
    auto ds = otb::GDALDriverManagerWrapper::GetInstance().Open(file);
    if (ds)
    {
      m_DatasetList.push_back(ds );
    }
  }
  
  int vrtSize = m_DatasetList.size();
 
  // Don't build a vrt if there is no dataset
  if (m_DatasetList.empty())
  {
    m_Dataset = nullptr;
  }
  else
  {
    std::vector<GDALDatasetH> vrtDatasetList(vrtSize);
    for (int i = 0; i < vrtSize; i++)
    {
      vrtDatasetList[i] = m_DatasetList[i]->GetDataSet();
    }

    auto close_me = GDALBuildVRT(DEM_DATASET_PATH.c_str(), vrtSize, vrtDatasetList.data(),
                                 nullptr, nullptr, nullptr);
    // Need to close the dataset, so it is flushed into memory.
    GDALClose(close_me);
    m_Dataset = static_cast<GDALDataset*>(GDALOpen(DEM_DATASET_PATH.c_str(), GA_ReadOnly));
    m_DEMDirectories.push_back(DEMDirectory);
  }

  if(m_GeoidDS)
  {
    CreateShiftedDataset();
  }
  Notify();
}

bool DEMHandler::OpenGeoidFile(const std::string& geoidFile)
{
  // TODO : RemoveOSSIM
  OssimDEMHandler::Instance()->OpenGeoidFile(geoidFile);


  int pbError;
  auto ds = GDALOpenVerticalShiftGrid(geoidFile.c_str(), &pbError);
  
  // pbError is set to TRUE if an error happens.
  if (pbError == 0)
  {
    if (m_GeoidDS)
    {
      GDALClose(m_GeoidDS);
    }
    m_GeoidDS = static_cast<GDALDataset*>(ds);
    m_GeoidFilename = geoidFile;
  }

  if(m_Dataset)
  {
    CreateShiftedDataset();
  }

  Notify();
  return pbError;
}


void DEMHandler::CreateShiftedDataset()
{

  // WIP : no data is not handed at the moment

  double geoTransform[6];
  m_Dataset->GetGeoTransform(geoTransform);

  // Warp geoid dataset
  auto warpOptions = GDALCreateWarpOptions();;
  warpOptions->hSrcDS = m_GeoidDS;
  //warpOptions->hDstDS = m_Dataset;
  warpOptions->eResampleAlg =  GRA_Bilinear;
  warpOptions->eWorkingDataType = GDT_Float64;
  warpOptions->nBandCount = 1;
    warpOptions->panSrcBands =
        (int *) CPLMalloc(sizeof(int) * warpOptions->nBandCount );
    warpOptions->panSrcBands[0] = 1;
    warpOptions->panDstBands =
        (int *) CPLMalloc(sizeof(int) * warpOptions->nBandCount );
    warpOptions->panDstBands[0] = 1;

  // Establish reprojection transformer.
  warpOptions->pTransformerArg =
        GDALCreateGenImgProjTransformer( m_GeoidDS,
                                        GDALGetProjectionRef(m_GeoidDS),
                                        m_Dataset,
                                        GDALGetProjectionRef(m_Dataset),
                                        FALSE, 0.0, 1 );
  warpOptions->pfnTransformer = GDALGenImgProjTransform;

  auto ds = static_cast<GDALDataset *> (GDALCreateWarpedVRT(m_GeoidDS, 
                      m_Dataset->GetRasterXSize(), 
                      m_Dataset->GetRasterYSize(), 
                      geoTransform, 
                      warpOptions));

/*
  auto warpedDataset = dynamic_cast<VRTDataset*>(ds);
  if(warpedDataset)
  {
    auto xmlWarpedDs= CPLSerializeXMLTree(warpedDataset->SerializeToXML(nullptr));
    std::cout << xmlWarpedDs << std::endl;
  }
*/

  ds->SetDescription(DEM_WARPED_DATASET_PATH.c_str());
  GDALClose(ds);

  GDALDriver *poDriver = (GDALDriver *) GDALGetDriverByName( "VRT" );
  GDALDataset *poVRTDS;

  poVRTDS = poDriver->Create( DEM_SHIFTED_DATASET_PATH.c_str(), m_Dataset->GetRasterXSize(), m_Dataset->GetRasterYSize(), 0, GDT_Float64, NULL );

  poVRTDS->SetGeoTransform(geoTransform);

  poVRTDS->SetProjection(m_Dataset->GetProjectionRef());

  char** derivedBandOptions = NULL;

  derivedBandOptions = CSLAddNameValue(derivedBandOptions, "subclass", "VRTDerivedRasterBand");
  derivedBandOptions = CSLAddNameValue(derivedBandOptions, "PixelFunctionType", "sum");
  poVRTDS->AddBand(GDT_Float64, derivedBandOptions);

  GDALRasterBand *poBand = poVRTDS->GetRasterBand( 1 );

// TODO use std string (and boost format ?) or stream
  char demVrtXml[10000];

  sprintf( demVrtXml,
          "<SimpleSource>"
          "  <SourceFilename>%s</SourceFilename>"
          "  <SourceBand>1</SourceBand>"
          "</SimpleSource>",
          DEM_DATASET_PATH.c_str());

  poBand->SetMetadataItem( "source_0", demVrtXml, "new_vrt_sources" );
  

  char geoidVrtXml[10000];

  sprintf( geoidVrtXml,
          "<SimpleSource>"
          "  <SourceFilename>%s</SourceFilename>"
          "  <SourceBand>1</SourceBand>"
          "</SimpleSource>",
          DEM_WARPED_DATASET_PATH.c_str());


  poBand->SetMetadataItem( "source_1", geoidVrtXml, "new_vrt_sources" );

  GDALClose(poVRTDS);
}


double DEMHandler::GetHeightAboveEllipsoid(double lon, double lat) const
{
  double result = 0.;
  boost::optional<double> DEMresult;
  boost::optional<double> geoidResult;

  if (m_Dataset)
  {
    DEMresult = DEMDetails::GetDEMValue(lon, lat, *m_Dataset);
    if (DEMresult)
    {
      result += *DEMresult;
    }
  }


  if (m_GeoidDS)
  {
    geoidResult = DEMDetails::GetDEMValue(lon, lat, *m_GeoidDS);
    if (geoidResult)
    {
      result += *geoidResult;
    }
  }

  if (DEMresult || geoidResult)
    return result;
  else
    return m_DefaultHeightAboveEllipsoid;
}

double DEMHandler::GetHeightAboveEllipsoid(const PointType& geoPoint) const
{
  return GetHeightAboveEllipsoid(geoPoint[0], geoPoint[1]);
}

double DEMHandler::GetHeightAboveMSL(double lon, double lat) const
{
  if (m_Dataset)
  { 
    auto result = DEMDetails::GetDEMValue(lon, lat, *m_Dataset);
    
    if (result)
    {
      return *result;
    }
  }

  return 0;
}

double DEMHandler::GetHeightAboveMSL(const PointType& geoPoint) const
{
  return GetHeightAboveMSL(geoPoint[0], geoPoint[1]);
}

unsigned int DEMHandler::GetDEMCount() const
{
  return m_DatasetList.size();
}

bool DEMHandler::IsValidDEMDirectory(const std::string& DEMDirectory) const
{
  for (const auto & filename : DEMDetails::GetFilesInDirectory(DEMDirectory))
  {
    // test if a driver can identify this dataset
    auto identifyDriverH = GDALIdentifyDriver(filename.c_str(), nullptr);
    if (identifyDriverH)
    {
      return true;
    }
  }

  return false;
}

std::string DEMHandler::GetDEMDirectory(unsigned int idx) const
{
  if (idx >= m_DEMDirectories.size())
  {
    std::ostringstream oss;
    oss << "Requested DEM diectory " << idx
        << ", but only "<< m_DEMDirectories.size() << "have been set.";
    throw std::out_of_range (oss.str());
  }
  
  return m_DEMDirectories[idx];
  
}

std::string DEMHandler::GetGeoidFile() const
{
  return m_GeoidFilename;
}

void DEMHandler::ClearDEMs()
{
  // TODO : RemoveOSSIM
  // This should be call, but this causes a segfault ... OssimDEMHandler will
  // be removed in a near future anyway
  // OssimDEMHandler::Instance()->ClearDEMs();
  m_DEMDirectories.clear();

  // This will call GDALClose on all datasets
  m_DatasetList.clear();
  Notify();
}

void DEMHandler::SetDefaultHeightAboveEllipsoid(double height)
{
  OssimDEMHandler::Instance()->SetDefaultHeightAboveEllipsoid(height);

  m_DefaultHeightAboveEllipsoid = height;
  Notify();
}

double DEMHandler::GetDefaultHeightAboveEllipsoid() const
{
  return m_DefaultHeightAboveEllipsoid;
}

void DEMHandler::Notify() const
{
  for (const auto & observer: m_ObserverList)
  {
    observer->Update();
  }
};

} // namespace otb
