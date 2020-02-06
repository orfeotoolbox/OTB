/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#include "otbGDALDriverManagerWrapper.h"
#include <vector>
#include "otb_boost_string_header.h"
#include "otbSystem.h"

namespace otb
{

// GDALDriverManagerWrapper method implementation

GDALDriverManagerWrapper::GDALDriverManagerWrapper()
{
  GDALAllRegister();

  GDALDriver* driver = nullptr;

  // Ignore incompatible Jpeg2000 drivers (Jasper)
  driver = GetGDALDriverManager()->GetDriverByName("JPEG2000");
  if (driver)
    GetGDALDriverManager()->DeregisterDriver(driver);

  // #ifndef CHECK_HDF4OPEN_SYMBOL
  //     // Get rid of the HDF4 driver when it is buggy
  //     driver = GetGDALDriverManager()->GetDriverByName( "hdf4" );
  //     if (driver)
  //       GetGDALDriverManager()->DeregisterDriver( driver );
  // #endif
}

GDALDriverManagerWrapper::~GDALDriverManagerWrapper()
{
  // GDALDestroyDriverManager();
  // Since gdal 2.4 we need to explicitely call GDALDestroy
  // GDALDestroyDriverManager is called inside
  // GDALDestroy(); // gdaldllmain.cpp line 74
}

// Open the file for reading and returns a smart dataset pointer
GDALDatasetWrapper::Pointer GDALDriverManagerWrapper::Open(std::string filename) const
{
  GDALDatasetWrapper::Pointer datasetWrapper;

  if (boost::algorithm::starts_with(filename, "http://") || boost::algorithm::starts_with(filename, "https://"))
  {
    // don't try to open it and exit
    return datasetWrapper;
  }

  // test if a driver can identify the dataset
  GDALDriverH identifyDriverH = GDALIdentifyDriver(filename.c_str(), nullptr);
  if (identifyDriverH == nullptr)
  {
    // don't try to open it and exit
    return datasetWrapper;
  }

  GDALDriver* identifyDriver = static_cast<GDALDriver*>(identifyDriverH);

  // check if Jasper will be used
  if (strcmp(identifyDriver->GetDescription(), "JPEG2000") == 0)
  {
    itkGenericExceptionMacro(<< "Error : tried to open the file " << filename << " with GDAL driver Jasper "
                                                                                 "(which fails on OTB). Try setting the environment variable GDAL_SKIP"
                                                                                 " in order to avoid this driver.");
  }

  GDALDatasetH dataset = GDALOpen(filename.c_str(), GA_ReadOnly);

  if (dataset != nullptr)
  {
    datasetWrapper            = GDALDatasetWrapper::New();
    datasetWrapper->m_Dataset = static_cast<GDALDataset*>(dataset);
  }
  return datasetWrapper;
}

// Open the new  file for writing and returns a smart dataset pointer
GDALDatasetWrapper::Pointer GDALDriverManagerWrapper::Create(std::string driverShortName, std::string filename, int nXSize, int nYSize, int nBands,
                                                             GDALDataType eType, char** papszOptions) const
{
  GDALDatasetWrapper::Pointer datasetWrapper;

  GDALDriver* driver = GetDriverByName(driverShortName);
  if (driver != nullptr)
  {
    GDALDataset* dataset = driver->Create(filename.c_str(), nXSize, nYSize, nBands, eType, papszOptions);

    if (dataset != nullptr)
    {
      datasetWrapper            = GDALDatasetWrapper::New();
      datasetWrapper->m_Dataset = dataset;
    }
  }
  return datasetWrapper;
}

GDALDriver* GDALDriverManagerWrapper::GetDriverByName(std::string driverShortName) const
{
  return GetGDALDriverManager()->GetDriverByName(driverShortName.c_str());
}

} // end namespace otb
