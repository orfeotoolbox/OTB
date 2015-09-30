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
  
  static GDALDatasetType * Open(const char * filename, bool readOnly = true);
  
  static void Close(GDALDatasetType * dataset);
  
  static GDALDatasetType * Create(GDALDriverType * driver, const char * name);

  static bool Delete(GDALDriverType * driver, const char * name);

  static GDALDriverType *  GetDriverByName(const char * name);

  static bool TestCapability(const GDALDriverType * driver, const GDALDatasetType * dataset, const char * capability);

  static bool SyncToDisk(GDALDatasetType * dataset);

  static std::string GetDatasetClassName();

  static std::string GetDriverClassName();

  static std::string GetDatasetDescription(GDALDatasetType * dataset);

private:
  OGRVersionProxy(); // purposely not implemeted
  OGRVersionProxy(const OGRVersionProxy&); // purposely not implemented
  void operator=(const OGRVersionProxy&); // purposely not implemented
};

} // end namespace otb

#endif
