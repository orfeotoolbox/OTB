/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

OGRExtendedFilenameToOptions::OGRExtendedFilenameToOptions() : m_HasFileName(false)
{
}

OGRExtendedFilenameToOptions::OGRExtendedFilenameToOptions(const GDALOptionType& options) : m_LayerOptions(), m_HasFileName(false)
{
  this->SetGDALLayerOptions(options);
}

OGRExtendedFilenameToOptions::Pointer OGRExtendedFilenameToOptions::GetGDALLayerOptionsHelper(const GDALOptionType& options)
{
  Pointer res = new OGRExtendedFilenameToOptions(options);
  res->UnRegister();
  return res;
}

void OGRExtendedFilenameToOptions::SetExtendedFileName(const char* extFname)
{
  if (extFname == nullptr)
  {
    itkGenericExceptionMacro(<< "Filename is NULL");
  }

  this->SetExtendedFileName(std::string(extFname));
}

void OGRExtendedFilenameToOptions::SetExtendedFileName(const std::string& extFname)
{
  Superclass::SetExtendedFileName(extFname);
  m_HasFileName = true;

  const OptionMapType& map = GetOptionMap();

  ConstMapIteratorType it;
  for (it = map.begin(); it != map.end(); it++)
  {
    std::vector<std::string> tmp;
    boost::split(tmp, it->first, boost::is_any_of(":"), boost::token_compress_on);

    if (tmp.size() > 2 && (tmp[0] == "gdal"))
    {
      if (tmp[1] == "oo")
      {
        m_OpenOptions.gdalOptions.push_back(tmp[2] + "=" + it->second);
      }
      else if (tmp[1] == "co")
      {
        m_CreationOptions.gdalOptions.push_back(tmp[2] + "=" + it->second);
      }
      else if (tmp[1] == "lco")
      {
        m_LayerOptions.gdalOptions[tmp[2]] = it->second;
      }
      else
      {
        // log a warning
      }
    }
  }
}

OGRExtendedFilenameToOptions::GDALOptionType OGRExtendedFilenameToOptions::GetGDALOptions(const std::string& type) const
{
  if (type == "layer")
    return GetGDALLayerOptions();
  else if (type == "creation")
    return m_CreationOptions.gdalOptions;
  else if (type == "open")
    return m_OpenOptions.gdalOptions;
  else
  {
    // warn user : wrong option
    return GDALOptionType();
  }
}

void OGRExtendedFilenameToOptions::SetGDALLayerOptions(const OGRExtendedFilenameToOptions::GDALOptionType& options)
{
  std::vector<std::string> tmp;
  for (const auto& option : options)
  {
    boost::split(tmp, option, boost::is_any_of(":"), boost::token_compress_on);
    if (tmp.size() < 2)
      boost::split(tmp, option, boost::is_any_of("="), boost::token_compress_on);
    m_LayerOptions.gdalOptions[tmp[0]] = tmp[1];
  }
}

void OGRExtendedFilenameToOptions::AddGDALLayerOptions(const OGRExtendedFilenameToOptions::GDALOptionType& options)
{
  for (const auto& option : options)
  {
    std::vector<std::string> tmp;
    boost::split(tmp, option, boost::is_any_of(":"), boost::token_compress_on);
    if (tmp.size() < 2)
      boost::split(tmp, option, boost::is_any_of("="), boost::token_compress_on);
    m_LayerOptions.gdalOptions[tmp[0]] = tmp[1];
  }
}

bool OGRExtendedFilenameToOptions::SimpleFileNameIsSet() const
{
  return m_HasFileName;
}

bool OGRExtendedFilenameToOptions::HasGDALLayerOption() const
{
  return !m_LayerOptions.gdalOptions.empty();
}

OGRExtendedFilenameToOptions::GDALOptionType OGRExtendedFilenameToOptions::GetGDALLayerOptions() const
{
  GDALOptionType options;
  for (const auto& option : m_LayerOptions.gdalOptions)
  {
    options.push_back(option.first + "=" + option.second);
  }
  return options;
}

#define GetGDALOptionMacro(Type)                                                                            \
  OGRExtendedFilenameToOptions::GDALOptionType OGRExtendedFilenameToOptions::GetGDAL##Type##Options() const \
  {                                                                                                         \
    return m_##Type##Options.gdalOptions;                                                                   \
  }

GetGDALOptionMacro(Open) GetGDALOptionMacro(Creation)
// GetGDALOptionMacro( Layer )


} // end namespace otb
