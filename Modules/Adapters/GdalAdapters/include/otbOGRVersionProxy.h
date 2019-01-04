/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbOGRVersionProxy_h
#define otbOGRVersionProxy_h

#include <string>
#include <vector>
#include "otbConfigure.h"
#include "itkMacro.h"

#if defined(_MSC_VER)
#pragma warning ( push )
#pragma warning ( disable: 4251 )
#include "ogr_core.h" // OGR enums
#pragma warning ( pop )
#else
#include "ogr_core.h" // OGR enums
#endif

class GDALDataset;
class GDALDriver;
class OGRFeature;

#include "OTBGdalAdaptersExport.h"

namespace otb
{
namespace ogr
{
namespace version_proxy
{

/** 
 * This namespace holds proxy functions hiding interface changes in gdal 2.0 
 *
 * This namespace holds proxy functions hiding interface changes in OGR
 * dataset between gdal 1.x (x>10) and gdal 2.x. It defines a common
 * interface that should be used in place of calling directly the
 * wrapped gdal functions.
 * 
 * Whenever GDALDataset and GDALDriver have to be used to open a
 * vector dataset (or OGRDataSource an OGRSFDriver for gdal 1.x), one
 * should use ogr::version_proxy types GDALDataset and
 * GDALDriverType.
 * 
 * See function documentation for details.
 */

  typedef GDALDriver GDALDriverType;

  /** 
   * This function opens a file, possibly in read-only mode, and returns
   * a dataset.
   *
   * Calls OGRSFDriverRegistrar::Open for gdal 1.x implementation and GDALopenEx for
   * gdal 2.x implementation.

   * \param filename Filename of the file to open
   * \param readOnly: If true, dataset is open in read-only mode.
   * \return NULL if file could not be open.
   */
  OTBGdalAdapters_EXPORT   
  GDALDataset * Open(const char * filename, bool readOnly = true , std::vector< std::string > const & options = std::vector< std::string >() );

  /**
   * This function closes a dataset.
   *
   * Calls OGRDataSource::DestroyDataSource for gdal 1.x
   * implementation and GDALClose for gdal 2.x implementation.
   *
   * \param dataset Pointer to the dataset to close. Will not be
   * checked for null pointer.
   */
  OTBGdalAdapters_EXPORT
  void Close(GDALDataset * dataset);

  /**
   * This function creates a new dataset.
   *
   * Calls OGRSFDriver::CreateDataSource for gdal 1.x implementation
   * and GDALDriver::Create with (0,0) raster size for gdal 2.x
   * implementation
   * 
   * \param driver Pointer to the driver used for creation. Will not
   * be checked for null pointer.
   *
   * \param name Name of the dataset to create.
   * 
   * \return NULL if dataset could not be created.
   */
  OTBGdalAdapters_EXPORT
  GDALDataset * Create(GDALDriverType * driver, const char * name , std::vector< std::string > const & options = std::vector< std::string >() );


  /**
   * This function physically deletes an existing dataset.
   * 
   * Calls OGRDataSource::DeleteDataSource for gdal 1.x implementation
   * and GDALDriver::Delete for gdal 2.x implementation.
   *
   * \param name Name of the dataset to destroy.
   */
  OTBGdalAdapters_EXPORT   
  bool Delete(const char * name);

  /**
   * Return the list of files composing the dataset.
   * 
   * Calls OGRDataSource::GetName() and wrap in string vector for gdal
   * 1.x implementation, and GDALDataset::GetFileList and wrap in
   * string vector for gdal 2.x implementation.
   *  
   * \param dataset Pointer to the dataset to get the file list from. Will not be
   * checked for null pointer.
   * 
   * \return A vector of string containing the list of files.
   */
   
  OTBGdalAdapters_EXPORT 
  std::vector<std::string> GetFileListAsStringVector(GDALDataset * dataset);

  /** 
   * Return the list of available drivers.
   *
   * Calls OGRSFDriverRegistrar::GetRegistrar() for gdal 1.x
   * implementation and GetGDALDriverManager() for gdal 2.x
   * implementation.
   *
   * \return A vector of string containing the list of available drivers.
   */  
  OTBGdalAdapters_EXPORT
  std::vector<std::string> GetAvailableDriversAsStringVector();

  /**
   * Returns true if the field 'index' is set and not-null in the given feature
   *
   * Before gdal 2.2, it calls OGRFeature::IsFieldSet().
   * After gdal 2.2, it calls OGRFeature::IsFieldSetAndNotNull()
   */
   OTBGdalAdapters_EXPORT
   bool IsFieldSetAndNotNull(OGRFeature *feat, int index);

}
}
} // end namespace otb

#endif
