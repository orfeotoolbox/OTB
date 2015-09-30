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

#include "gdal_priv.h"

namespace otb
{


OGRVersionProxy::GDALDatasetType * OGRVersionProxy::Open(const char * filename, bool readOnly)
{
  return (GDALDatasetType *)GDALOpenEx(filename, (readOnly? GDAL_OF_READONLY : GDAL_OF_UPDATE) | GDAL_OF_VECTOR,NULL,NULL,NULL);
}

void OGRVersionProxy::Close(GDALDatasetType * dataset)
{
  GDALClose(dataset);
}
  
OGRVersionProxy::GDALDatasetType * OGRVersionProxy::Create(GDALDriverType * driver, const char * name)
{
  return driver->Create(name,0,0,0,GDT_Unknown,NULL);
}

bool OGRVersionProxy::Delete(GDALDriverType * driver, const char * name)
{
  OGRErr ret = driver->Delete(name);

  return (ret == OGRERR_NONE);
}

OGRVersionProxy::GDALDriverType *  OGRVersionProxy::GetDriverByName(const char * name)
{
  return GetGDALDriverManager()->GetDriverByName(name);
}

bool OGRVersionProxy::TestCapability(const GDALDriverType *, const GDALDatasetType * dataset, const char * capability)
{
  // Const correctness problem in GDALDataset class
  return const_cast<GDALDatasetType *>(dataset)->TestCapability(capability);
}

std::string OGRVersionProxy::GetDatasetClassName()
{
  return std::string("GDALDataset");
}

std::string OGRVersionProxy::GetDriverClassName()
{
  return std::string("GDALDriver");
}

std::string OGRVersionProxy::GetDatasetDescription(GDALDatasetType * dataset)
{
  char ** files = dataset->GetFileList();

  std::string files_str="";
      
  if(files)
    {
    unsigned int i = 0;
    while(files[i]!=NULL)
      {
      files_str+=std::string(files[i])+" ";
      ++i;
      }
    CSLDestroy(files);
    }
  return files_str;
}

bool OGRVersionProxy::SyncToDisk(GDALDatasetType * dataset)
{
  dataset->FlushCache();

  return true;
}

} // end namespace
