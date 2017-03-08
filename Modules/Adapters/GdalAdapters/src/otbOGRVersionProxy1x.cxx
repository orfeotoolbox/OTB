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

#include "otbOGRVersionProxy.h"

#include "itkMacro.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include "ogrsf_frmts.h"
#pragma GCC diagnostic pop
#else
#include "ogrsf_frmts.h"
#endif

namespace otb
{
namespace ogr
{
namespace version_proxy
{

OTBGdalAdapters_EXPORT bool IsOFTInteger64(OGRFieldType itkNotUsed(type))
{
  return false;
}


GDALDatasetType * Open(const char * filename, bool readOnly)
{
  return OGRSFDriverRegistrar::Open(filename,!readOnly);
}

void Close(GDALDatasetType * dataset)
{
  OGRDataSource::DestroyDataSource(dataset);
}

GDALDatasetType * Create(GDALDriverType * driver, const char * name)
{
  GDALDatasetType * ds = driver->CreateDataSource(name);

  if(ds)
    ds->SetDriver(driver);

  return ds;
}

bool Delete(const char * name)
{
  // Open dataset
  GDALDatasetType * poDS = Open(name,false);
  GDALDriverType * poDriver = ITK_NULLPTR;
  if(poDS)
    {
    poDriver = poDS->GetDriver();
    Close(poDS);
    }

  if(poDriver && poDriver->TestCapability(ODrCDeleteDataSource))
    {

    OGRErr ret = poDriver->DeleteDataSource(name);
    return (ret == OGRERR_NONE);
    }

  return false;
}

GDALDriverType *  GetDriverByName(const char * name)
{
  return OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(name);
}

std::string GetDatasetClassName()
{
  return std::string("OGRDataSource");
}

std::string GetDriverClassName()
{
  return std::string("OGRSFDriver");
}

std::vector<std::string> GetFileListAsStringVector(GDALDatasetType * dataset)
{
  std::vector<std::string> ret;

  ret.push_back(std::string(dataset->GetName()));

  return ret;
}

bool SyncToDisk(GDALDatasetType * dataset)
{
  const OGRErr res= dataset->SyncToDisk();

  return (res == OGRERR_NONE);
}

std::vector<std::string> GetAvailableDriversAsStringVector()
{
  std::vector<std::string> ret;

  int nbDrivers = OGRSFDriverRegistrar::GetRegistrar()->GetDriverCount();

  for(int i = 0; i < nbDrivers;++i)
    {
    ret.push_back(OGRSFDriverRegistrar::GetRegistrar()->GetDriver(i)->GetName());
    }

  return ret;
}

std::string GetDriverNameFromDataSource(const GDALDatasetType * ds)
{
  return std::string(const_cast<GDALDatasetType *>(ds)->GetDriver()->GetName());
}

}
}
} // end namespace
