/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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
  // calling GDALDestroyDriverManager() (or GDALDestroy) from the destructor of a 
  // static C++ object is unsafe.
  // GDALDestroyDriverManager();
}

GDALDatasetWrapper::Pointer GDALDriverManagerWrapper::OpenFromMemory(
                                const void* mem_ptr,
                                const uint64_t& width,
                                const uint64_t& height,
                                const GDALDataType pix_type,
                                const uint32_t byte_per_pixel,
                                const uint16_t nb_bands,
                                const uint64_t& band_offset) const
{
  GDALDatasetWrapper::Pointer datasetWrapper = nullptr;

  GDALDriver* memDriver = GetDriverByName("MEM");

  if (memDriver != NULL) {
    // create a dataset in memory but with 0 band, bands are added later
    GDALDataset* dataset = memDriver->Create("", width, height, 0, pix_type, nullptr);
    // The shift between bands was previously used with BANDOFFSET parameter
    // set to the byte per pixel size.
    // On normal case if not provided, the bandoffset is nbLines * lineSize.
    // BUT in ou case data in memory are not agenced per band.
    // For each X,Y pos the memory contains all band info.
    for (int i = 0; i < nb_bands; ++i) {
      char data_ptr[64] = {'\0'};
      char pixel_offset[64] = {'\0'};
      char line_offset[64] = {'\0'};
      char band_start_in_mem[32] = {'\0'};
      // use CPLPrintPointer to avoid worrying between windows and linux adress
      int nRet = CPLPrintPointer(band_start_in_mem,
                static_cast<GByte*>(const_cast<void*>(mem_ptr)) + band_offset * i,
                                  sizeof(band_start_in_mem));
      band_start_in_mem[nRet] = 0;

      snprintf(data_ptr, sizeof(data_ptr), "DATAPOINTER=%s", band_start_in_mem);
      snprintf(pixel_offset, sizeof(pixel_offset), "PIXELOFFSET=%d", byte_per_pixel * nb_bands);
      snprintf(line_offset, sizeof(line_offset), "LINEOFFSET=%lu",
                byte_per_pixel * nb_bands * width);
      char *band_opt[4] = {data_ptr, pixel_offset, line_offset, nullptr};
      dataset->AddBand(pix_type, band_opt);
    }

    datasetWrapper = GDALDatasetWrapper::New();
    datasetWrapper->m_Dataset = dataset;
  }

  return datasetWrapper;
}

// Open the file for reading and returns a smart dataset pointer
GDALDatasetWrapper::Pointer GDALDriverManagerWrapper::Open(std::string filename) const
{
  GDALDatasetWrapper::Pointer datasetWrapper;

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
GDALDatasetWrapper::Pointer GDALDriverManagerWrapper::Create(std::string& driverShortName, std::string filename, int nXSize, int nYSize, int nBands,
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
