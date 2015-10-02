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
#ifndef __otbOGRVersionProxy_h
#define __otbOGRVersionProxy_h

#include <string>
#include <vector>
#include "otbConfigure.h"

#ifdef OTB_USE_GDAL_20
class GDALDataset;
class GDALDriver;
#else
class OGRDataSource;
class OGRSFDriver;
#endif

namespace otb
{
/** 
 * \brief This class serves as a proxy hiding interface changes in gdal 2.0 
 *
 * This static class serves as a proxy hiding interface changes in OGR
 * dataset between gdal 1.x (x>10) and gdal 2.x. It defines a common
 * interface that should be used in place of calling directly the
 * wrapped gdal methods.
 * 
 * Whenever GDALDataset and GDALDriver have to be used to open a
 * vector dataset (or OGRDataSource an OGRSFDriver for gdal 1.x), one
 * should use OGRVersionProxy type members GDALDatasetType and
 * GDALDriverType.
 * 
 * See static method documentation for details.
 */

class OGRVersionProxy
{
public:
  #ifdef OTB_USE_GDAL_20
  typedef GDALDataset GDALDatasetType;
  typedef GDALDriver GDALDriverType;
  #else
  typedef OGRDataSource GDALDatasetType;
  typedef OGRSFDriver   GDALDriverType;
#endif

  /** 
   * This method opens a file, possibly in read-only mode, and returns
   * a dataset.
   *
   * Calls OGRSFDriverRegistrar::Open for gdal 1.x implementation and GDALopenEx for
   * gdal 2.x implementation.

   * \param filename Filename of the file to open
   * \param readOnly: If true, dataset is open in read-only mode.
   * \return NULL if file could not be open.
   */ 
  static GDALDatasetType * Open(const char * filename, bool readOnly = true);

  /**
   * This method closes a dataset.
   *
   * Calls OGRDataSource::DestroyDataSource for gdal 1.x
   * implementation and GDALClose for gdal 2.x implementation.
   *
   * \param dataset Pointer to the dataset to close. Will not be
   * checked for null pointer.
   */
  static void Close(GDALDatasetType * dataset);

  /**
   * This method creates a new dataset.
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
  static GDALDatasetType * Create(GDALDriverType * driver, const char * name);


  /**
   * This method physically deletes an existing dataset.
   * 
   * Calls OGRDataSource::DeleteDataSource for gdal 1.x implementation
   * and GDALDriver::Delete for gdal 2.x implementation.
   *
   * \param driver Pointer to the driver used for creation. Will not
   * be checked for null pointer.
   *
   * \param name Name of the dataset to destroy.
   */ 
  static bool Delete(GDALDriverType * driver, const char * name);

  /**
   * This method returns a pointer to the driver from its name.
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
  static GDALDriverType *  GetDriverByName(const char * name);

  /**
   * Test driver and dataset for a given capability.
   *
   * Calls OGRSFDriver::TestCapability for gdal 1.x implementation and
   * GDALDataset::TestCapability for gdal 2.x implementation.
   *
   * \param driver Pointer to the driver to test. Will not
   * be checked for null pointer.
   *
   * \param dataset Pointer to the dataset to test. Will not be
   * checked for null pointer.
   *
   * \param capability Name of the capability to test.
   *
   * \return True if the capability is supported.
   */
  static bool TestCapability(const GDALDriverType * driver, const GDALDatasetType * dataset, const char * capability);

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
  static bool SyncToDisk(GDALDatasetType * dataset);

  /**
   * \return The name of the dataset class behind the implementation
   * (OGRDataSource for gdal 1.x and GdalDataset for gdal 2.x)
   */
  static std::string GetDatasetClassName();

  /**
   * \return The name of the driver class behind the implementation
   * (OGRSFDriver for gdal 1.x and GDALDriver for gdal 2.x)
   */
  static std::string GetDriverClassName();

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
  static std::vector<std::string> GetFileListAsStringVector(GDALDatasetType * dataset);

  /** 
   * Return the list of available drivers.
   *
   * Calls OGRSFDriverRegistrar::GetRegistrar() for gdal 1.x
   * implementation and GetGDALDriverManager() for gdal 2.x
   * implementation.
   *
   * \return A vector of string containing the list of available drivers.
   */  
  static std::vector<std::string> GetAvailableDriversAsStringVector();

private:
  OGRVersionProxy(); // purposely not implemented
  OGRVersionProxy(const OGRVersionProxy&); // purposely not implemented
  void operator=(const OGRVersionProxy&); // purposely not implemented
};

} // end namespace otb

#endif
