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

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include "gdal_priv.h"
#pragma GCC diagnostic pop
#else
#include "gdal_priv.h"
#endif


namespace otb
{
namespace ogr
{
namespace version_proxy
{

GDALDatasetType * Open(const char * filename, bool readOnly)
{
  return (GDALDatasetType *)GDALOpenEx(filename, (readOnly? GDAL_OF_READONLY : GDAL_OF_UPDATE) | GDAL_OF_VECTOR,NULL,NULL,NULL);
}

void Close(GDALDatasetType * dataset)
{
  GDALClose(dataset);
}

GDALDatasetType * Create(GDALDriverType * driver, const char * name)
{
  return driver->Create(name,0,0,0,GDT_Unknown,NULL);
}

bool Delete(const char * name)
{
  // Open dataset
  GDALDatasetType * poDS = otb::ogr::version_proxy::Open(name,false);
  GDALDriverType * poDriver = NULL;
  if(poDS)
    {
    poDriver = poDS->GetDriver();
    Close(poDS);
    }

  if(poDriver)
    {
    OGRErr ret = poDriver->Delete(name);

    return (ret == OGRERR_NONE);
    }
  return false;
}

GDALDriverType *  GetDriverByName(const char * name)
{
  return GetGDALDriverManager()->GetDriverByName(name);
}

std::string GetDatasetClassName()
{
  return std::string("GDALDataset");
}

std::string GetDriverClassName()
{
  return std::string("GDALDriver");
}

namespace raii
{
// This class is used in the next function, so as to prevent any
// resource leak on char ** returned by dataset->GetFileList()
class CharPPCapsule
{
public:
  CharPPCapsule(char ** in)
    : m_P(in)
  {}

  const char ** P() const
  {
    return const_cast<const char **>(m_P);
  }

  ~CharPPCapsule()
  {
    if(m_P)
      CSLDestroy(m_P);
  }

private:
  char ** m_P;
};
}

std::vector<std::string> GetFileListAsStringVector(GDALDatasetType * dataset)
{
  std::vector<std::string> ret;

  raii::CharPPCapsule capsule(dataset->GetFileList());

  std::string files_str="";

  if(capsule.P())
    {
    unsigned int i = 0;
    while(capsule.P()[i]!=NULL)
      {
      ret.push_back(std::string(capsule.P()[i]));
      ++i;
      }
    }
  return ret;
}

bool SyncToDisk(GDALDatasetType * dataset)
{
  dataset->FlushCache();

  return true;
}

std::vector<std::string> GetAvailableDriversAsStringVector()
{
  std::vector<std::string> ret;

  int nbDrivers = GetGDALDriverManager()->GetDriverCount();

  for(int i = 0; i < nbDrivers;++i)
    {
    ret.push_back(GDALGetDriverShortName(GetGDALDriverManager()->GetDriver(i)));
    }

  return ret;
}

std::string GetDriverNameFromDataSource(const GDALDatasetType * ds)
{
  return std::string(const_cast<GDALDatasetType *>(ds)->GetDriverName());
}

}
}
} // end namespace
