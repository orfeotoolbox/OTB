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

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include "gdal_priv.h"
#pragma GCC diagnostic pop
#else
#include "gdal_priv.h"
#endif

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

namespace raii
{
// This class is used in the next function, so as to prevent any
// ressource leak on char ** returned by dataset->GetFileList()
class CharPPCapsule
{
public:
  CharPPCapsule(char ** in)
  {
    m_P = in;
  }

  ~CharPPCapsule()
  {
    if(m_P)
      CSLDestroy(m_P);
  }

  char ** m_P;
};
}

std::vector<std::string> OGRVersionProxy::GetFileListAsStringVector(GDALDatasetType * dataset)
{
  std::vector<std::string> ret;
  
  raii::CharPPCapsule capsule(dataset->GetFileList());

  std::string files_str="";
      
  if(capsule.m_P)
    {
    unsigned int i = 0;
    while(capsule.m_P[i]!=NULL)
      {
      ret.push_back(std::string(capsule.m_P[i]));
      ++i;
      }
    }
  return ret;
}

bool OGRVersionProxy::SyncToDisk(GDALDatasetType * dataset)
{
  dataset->FlushCache();

  return true;
}

std::vector<std::string> OGRVersionProxy::GetAvailableDriversAsStringVector()
{
  std::vector<std::string> ret;
  
  int nbDrivers = GetGDALDriverManager()->GetDriverCount();
  
  for(int i = 0; i < nbDrivers;++i)
    {
    ret.push_back(GDALGetDriverShortName(GetGDALDriverManager()->GetDriver(i)));
    }

  return ret;
}

} // end namespace
