/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#include "otbExtendedFilenameToReaderOptions.h"
#include "otb_boost_string_header.h"
#include "itksys/RegularExpression.hxx"

namespace otb
{

ExtendedFilenameToReaderOptions::ExtendedFilenameToReaderOptions() : ExtendedFilenameHelper()
{
  m_Options.simpleFileName.first  = false;
  m_Options.simpleFileName.second = "";

  m_Options.extGEOMFileName.first  = false;
  m_Options.extGEOMFileName.second = "";

  m_Options.subDatasetIndex.first  = false;
  m_Options.subDatasetIndex.second = 0;

  m_Options.resolutionFactor.first  = false;
  m_Options.resolutionFactor.second = 0;

  m_Options.skipCarto.first  = false;
  m_Options.skipCarto.second = false;

  m_Options.skipGeom.first  = false;
  m_Options.skipGeom.second = false;

  m_Options.skipRpcTag.first  = false;
  m_Options.skipRpcTag.second = false;

  m_Options.bandRange.first  = false;
  m_Options.bandRange.second = "";

  m_Options.optionList.push_back("geom");
  m_Options.optionList.push_back("sdataidx");
  m_Options.optionList.push_back("resol");
  m_Options.optionList.push_back("skipcarto");
  m_Options.optionList.push_back("skipgeom");
  m_Options.optionList.push_back("skiprpctag");
  m_Options.optionList.push_back("bands");
}

void ExtendedFilenameToReaderOptions::SetExtendedFileName(const char* extFname)
{
  if (extFname)
  {
    this->SetExtendedFileName(std::string(extFname));
  }
}

void ExtendedFilenameToReaderOptions::SetExtendedFileName(const std::string& extFname)
{
  Superclass::SetExtendedFileName(extFname);
  MapType map = GetOptionMap();

  m_Options.simpleFileName.first  = true;
  m_Options.simpleFileName.second = this->GetSimpleFileName();

  if (!map["geom"].empty())
  {
    m_Options.extGEOMFileName.first  = true;
    m_Options.extGEOMFileName.second = map["geom"];
  }
  if (!map["sdataidx"].empty())
  {
    m_Options.subDatasetIndex.first  = true;
    m_Options.subDatasetIndex.second = atoi(map["sdataidx"].c_str());
  }
  if (!map["resol"].empty())
  {
    m_Options.resolutionFactor.first  = true;
    m_Options.resolutionFactor.second = atoi(map["resol"].c_str());
  }

  if (!map["skipcarto"].empty())
  {
    m_Options.skipCarto.first = true;
    if (map["skipcarto"] == "On" || map["skipcarto"] == "on" || map["skipcarto"] == "ON" || map["skipcarto"] == "true" || map["skipcarto"] == "True" ||
        map["skipcarto"] == "1")
    {
      m_Options.skipCarto.second = true;
    }
  }

  if (!map["skipgeom"].empty())
  {
    m_Options.skipGeom.first = true;
    if (map["skipgeom"] == "On" || map["skipgeom"] == "on" || map["skipgeom"] == "ON" || map["skipgeom"] == "true" || map["skipgeom"] == "True" ||
        map["skipgeom"] == "1")
    {
      m_Options.skipGeom.second = true;
    }
  }

  if (!map["skiprpctag"].empty())
  {
    m_Options.skipRpcTag.first = true;
    if (map["skiprpctag"] == "On" || map["skiprpctag"] == "on" || map["skiprpctag"] == "ON" || map["skiprpctag"] == "true" || map["skiprpctag"] == "True" ||
        map["skiprpctag"] == "1")
    {
      m_Options.skipRpcTag.second = true;
    }
  }

  if (!map["bands"].empty())
  {
    // Basic check on bandRange (using regex)
    itksys::RegularExpression reg;
    reg.compile("^((\\-?[0-9]+)?(:(\\-?[0-9]+)?)?)(,(\\-?[0-9]+)?(:(\\-?[0-9]+)?)?)*$");
    if (reg.find(map["bands"]))
    {
      m_Options.bandRange.first  = true;
      m_Options.bandRange.second = map["bands"];
    }
    else
    {
      itkExceptionMacro("Unkwown value "
                        << map["bands"]
                        << " for band range. Expect a list of tokens separated with comma (each token being a single band index or a range in the form x:y)");
    }
  }

  // Option Checking
  MapIteratorType it;
  for (it = map.begin(); it != map.end(); it++)
  {
    bool isKnown = false;
    for (unsigned int i = 0; i < m_Options.optionList.size(); i++)
    {
      if (m_Options.optionList[i] == it->first)
        isKnown = true;
    }
    if (!isKnown)
      itkWarningMacro("Unknown option detected: " << it->first << ".");
  }
}

bool ExtendedFilenameToReaderOptions::SimpleFileNameIsSet() const
{
  return m_Options.simpleFileName.first;
}

bool ExtendedFilenameToReaderOptions::ExtGEOMFileNameIsSet() const
{
  return m_Options.extGEOMFileName.first;
}
const char* ExtendedFilenameToReaderOptions::GetExtGEOMFileName() const
{
  return m_Options.extGEOMFileName.second.c_str();
}

bool ExtendedFilenameToReaderOptions::SubDatasetIndexIsSet() const
{
  return m_Options.subDatasetIndex.first;
}
unsigned int ExtendedFilenameToReaderOptions::GetSubDatasetIndex() const
{
  return m_Options.subDatasetIndex.second;
}

bool ExtendedFilenameToReaderOptions::ResolutionFactorIsSet() const
{
  return m_Options.resolutionFactor.first;
}
unsigned int ExtendedFilenameToReaderOptions::GetResolutionFactor() const
{
  return m_Options.resolutionFactor.second;
}

bool ExtendedFilenameToReaderOptions::SkipCartoIsSet() const
{
  return m_Options.skipCarto.first;
}
bool ExtendedFilenameToReaderOptions::GetSkipCarto() const
{
  return m_Options.skipCarto.second;
}

bool ExtendedFilenameToReaderOptions::SkipGeomIsSet() const
{
  return m_Options.skipGeom.first;
}
bool ExtendedFilenameToReaderOptions::GetSkipGeom() const
{
  return m_Options.skipGeom.second;
}

bool ExtendedFilenameToReaderOptions::SkipRpcTagIsSet() const
{
  return m_Options.skipRpcTag.first;
}
bool ExtendedFilenameToReaderOptions::GetSkipRpcTag() const
{
  return m_Options.skipRpcTag.second;
}

bool ExtendedFilenameToReaderOptions::BandRangeIsSet() const
{
  return m_Options.bandRange.first;
}

std::string ExtendedFilenameToReaderOptions::GetBandRange() const
{
  return m_Options.bandRange.second;
}

} // end namespace otb
