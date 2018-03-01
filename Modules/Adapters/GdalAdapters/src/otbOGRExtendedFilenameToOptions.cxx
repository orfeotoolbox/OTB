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

#include "otbOGRExtendedFilenameToOptions.h"
#include "otb_boost_string_header.h"
#include "otb_boost_tokenizer_header.h"

namespace otb
{

OGRExtendedFilenameToOptions::
OGRExtendedFilenameToOptions()
{
  m_OpenOptions.simpleFileName.first = false;
  m_OpenOptions.simpleFileName.second = "";

  m_CreationOptions.simpleFileName.first = false;
  m_CreationOptions.simpleFileName.second = ""; 

}

void
OGRExtendedFilenameToOptions::
SetExtendedFileName(const char *extFname)
{
  Superclass::SetExtendedFileName(extFname);
  OptionMapType map = GetOptionMap();

  m_OpenOptions.simpleFileName.first  = true;
  m_OpenOptions.simpleFileName.second = this->GetSimpleFileName();

  m_CreationOptions.simpleFileName.first  = true;
  m_CreationOptions.simpleFileName.second = this->GetSimpleFileName();

  MapIteratorType it;
  for ( it=map.begin(); it != map.end(); it++ )
    {
    std::vector<std::string> tmp;
    boost::split(tmp, it->first, boost::is_any_of(":"), boost::token_compress_on);

    if (tmp.size()>2 && (tmp[0]=="gdal") )
      {
      if ( tmp[1]=="oo" )
        {
        m_OpenOptions.gdalOptions.first = true;
        m_OpenOptions.gdalOptions.second.push_back(tmp[2] + "=" +it->second);
        }
      else if ( tmp[1]=="co" )
        {
        m_CreationOptions.gdalOptions.first = true;
        m_CreationOptions.gdalOptions.second.push_back(tmp[2] + "=" +it->second);
        }
      else if ( tmp[1]=="lco" )
        {
        m_LayerOptions.gdalOptions.first = true;
        m_LayerOptions.gdalOptions.second.push_back(tmp[2] + "=" +it->second);
        }
      else
        {
        // log a warning
        }
      }

    }


}


} //end namespace otb
