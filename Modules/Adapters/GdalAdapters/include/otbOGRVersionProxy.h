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
#ifndef otbOGRVersionProxy_h
#define otbOGRVersionProxy_h

#include <string>
#include <vector>
#include "otbConfigure.h"
#include "itkMacro.h"

#ifdef OTB_USE_GDAL_20
class GDALDataset;
class GDALDriver;
#else
class OGRDataSource;
class OGRSFDriver;
#endif

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
 * should use ogr::version_proxy types GDALDatasetType and
 * GDALDriverType.
 * 
 * See function documentation for details.
 */

  #ifdef OTB_USE_GDAL_20
  typedef GDALDataset GDALDatasetType;
  typedef GDALDriver GDALDriverType;
  #else
  typedef OGRDataSource GDALDatasetType;
  typedef OGRSFDriver   GDALDriverType;
#endif

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
  GDALDatasetType * Open(const char * filename, bool readOnly = true);

  /**
   * This function closes a dataset.
   *
   * Calls OGRDataSource::DestroyDataSource for gdal 1.x
   * implementation and GDALClose for gdal 2.x implementation.
   *
   * \param dataset Pointer to the dataset to close. Will not be
   * checked for null pointer.
   */
  void Close(GDALDatasetType * dataset);

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
  GDALDatasetType * Create(GDALDriverType * driver, const char * name);


  /**
   * This function physically deletes an existing dataset.
   * 
   * Calls OGRDataSource::DeleteDataSource for gdal 1.x implementation
   * and GDALDriver::Delete for gdal 2.x implementation.
   *
   * \param name Name of the dataset to destroy.
   */ 
  bool Delete(const char * name);

  /**
   * This function returns a pointer to the driver from its name.
   * 
   * Calls OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName() for
   * gdal 1.x implementation and
   * GetGDALDriverManager()->GetDriverByName() for gdal 2.x
   * implementation.
   * 
   * \param name Name of the driver to retrieve
   * 
   * \return NULL if no driver could be retrieved.
   */
  GDALDriverType *  GetDriverByName(const char * name);


  std::string GetDriverNameFromDataSource(const GDALDatasetType * ds);

  /**
   * Sync dataset to disk.
   *
   * Calls OGRDataSource::SyncToDisk() for gdal 1.x implementation and
   * GDALDataset::FlushCache() for gdal 2.x  implementation.
   *
   * \param dataset Pointer to the dataset to sync. Will not be
   * checked for null pointer.
   *
   * \return True if sync went on without any error.
   */
  bool SyncToDisk(GDALDatasetType * dataset);

  /**
   * \return The name of the dataset class behind the implementation
   * (OGRDataSource for gdal 1.x and GdalDataset for gdal 2.x)
   */
  std::string GetDatasetClassName();

  /**
   * \return The name of the driver class behind the implementation
   * (OGRSFDriver for gdal 1.x and GDALDriver for gdal 2.x)
   */
  std::string GetDriverClassName();

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
  std::vector<std::string> GetFileListAsStringVector(GDALDatasetType * dataset);

  /** 
   * Return the list of available drivers.
   *
   * Calls OGRSFDriverRegistrar::GetRegistrar() for gdal 1.x
   * implementation and GetGDALDriverManager() for gdal 2.x
   * implementation.
   *
   * \return A vector of string containing the list of available drivers.
   */  
  std::vector<std::string> GetAvailableDriversAsStringVector();

}
}
} // end namespace otb

#endif
