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
#include "otbOGRVersionProxy.h"

#include "ogrsf_frmts.h"

namespace otb
{


OGRVersionProxy::GDALDatasetType * OGRVersionProxy::Open(const char * filename, bool readOnly)
{
  return OGRSFDriverRegistrar::Open(filename,!readOnly);
}

void OGRVersionProxy::Close(GDALDatasetType * dataset)
{
  OGRDataSource::DestroyDataSource(dataset);
}
  
OGRVersionProxy::GDALDatasetType * OGRVersionProxy::Create(GDALDriverType * driver, const char * name)
{
  return driver->CreateDataSource(name);
}

bool OGRVersionProxy::Delete(GDALDriverType * driver, const char * name)
{
  OGRErr ret = driver->DeleteDataSource(name);

  return (ret == OGRERR_NONE);
}

OGRVersionProxy::GDALDriverType *  OGRVersionProxy::GetDriverByName(const char * name)
{
  return OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(name);
}

bool OGRVersionProxy::TestCapability(const GDALDriverType * driver, const GDALDatasetType *, const char * capability)
{
  // Class OGRDataSource is not const correct
  return const_cast<GDALDriverType * >(driver)->TestCapability(capability);
}

std::string OGRVersionProxy::GetDatasetClassName()
{
  return std::string("OGRDataSource");
}

std::string OGRVersionProxy::GetDriverClassName()
{
  return std::string("OGRSFDriver");
}

std::string OGRVersionProxy::GetDatasetDescription(GDALDatasetType * dataset)
{
  return std::string(dataset->GetName());
}

bool OGRVersionProxy::SyncToDisk(GDALDatasetType * dataset)
{
  const OGRErr res= dataset->SyncToDisk();

  return (res == OGRERR_NONE);
}

} // end namespace
