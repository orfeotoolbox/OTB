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
#include "otbOGRHelpers.h"

#include "itkMacro.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#if GDAL_VERSION_NUM<2000000
#include "ogrsf_frmts.h"
#else
#include "gdal_priv.h"
#endif
#include "ogr_feature.h"
#pragma GCC diagnostic pop
#else
#if GDAL_VERSION_NUM<2000000
#include "ogrsf_frmts.h"
#else
#include "gdal_priv.h"
#endif
#include "ogr_feature.h"
#endif // __GNUC__ || __clang__

namespace otb
{
namespace ogr
{
namespace version_proxy
{

#if GDAL_VERSION_NUM>=2000000
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
#endif

std::vector<std::string> GetFileListAsStringVector(GDALDataset * dataset)
{
  std::vector<std::string> ret;
#if GDAL_VERSION_NUM<2000000
  ret.push_back(std::string(dataset->GetName()));
#else
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
#endif
  return ret;
}

std::vector<std::string> GetAvailableDriversAsStringVector()
{
  std::vector<std::string> ret;
#if GDAL_VERSION_NUM<2000000
  int nbDrivers = OGRSFDriverRegistrar::GetRegistrar()->GetDriverCount();

  for(int i = 0; i < nbDrivers;++i)
    {
    ret.push_back(OGRSFDriverRegistrar::GetRegistrar()->GetDriver(i)->GetName());
    }
#else
  int nbDrivers = GetGDALDriverManager()->GetDriverCount();

  for(int i = 0; i < nbDrivers;++i)
    {
    ret.push_back(GDALGetDriverShortName(GetGDALDriverManager()->GetDriver(i)));
    }
#endif
  return ret;
}

/*----------------------[GDAL 2.2 change on IsFieldSet()]---------------------*/
bool IsFieldSetAndNotNull(OGRFeature *feat, int index)
{
#if GDAL_VERSION_NUM<2020000
  return feat->IsFieldSet(index);
#else
  return feat->IsFieldSetAndNotNull(index);
#endif
}

}
}
} // end namespace

