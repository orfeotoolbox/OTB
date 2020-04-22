/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2018-2020 CS Systemes d'Information (CS SI)
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

#include "otbExtendedFilenameToWriterOptions.h"
#include "otb_boost_string_header.h"
#include <itksys/RegularExpression.hxx>
#include "otb_boost_tokenizer_header.h"
#include "otbStringUtils.h"

namespace otb
{

ExtendedFilenameToWriterOptions::ExtendedFilenameToWriterOptions() : ExtendedFilenameHelper()
{
  m_Options.simpleFileName.first  = false;
  m_Options.simpleFileName.second = "";

  m_Options.writeGEOMFile.first  = false;
  m_Options.writeGEOMFile.second = true;

  m_Options.multiWrite.first  = false;
  m_Options.multiWrite.second = true;

  m_Options.writeRPCTags.first  = false;
  m_Options.writeRPCTags.second = false;

  has_noDataValue = false;

  m_Options.gdalCreationOptions.first = false;
  m_Options.streamingType.first       = false;
  m_Options.streamingSizeMode.first   = false;
  m_Options.streamingSizeValue.first  = false;

  m_Options.bandRange.first  = false;
  m_Options.bandRange.second = "";

  m_Options.srsValue.first = false;

  m_Options.optionList = {"writegeom", "writerpctags", "multiwrite", "streaming:type",
    "streaming:sizemode", "streaming:sizevalue", "nodata", "box", "bands", "epsg"};
}

void ExtendedFilenameToWriterOptions::SetExtendedFileName(const char* extFname)
{
  if (extFname == nullptr)
  {
    itkGenericExceptionMacro(<< "Filename is NULL");
  }

  this->SetExtendedFileName(std::string(extFname));
}

void ExtendedFilenameToWriterOptions::SetExtendedFileName(const std::string& extFname)
{
  this->Superclass::SetExtendedFileName(extFname);
  // TODO: Rename map to a less confusing (with std::map) name
  MapType map = this->GetOptionMap();

  m_Options.simpleFileName.first  = true;
  m_Options.simpleFileName.second = this->GetSimpleFileName();

  MapIteratorType it;
  for (it = map.begin(); it != map.end(); it++)
  {
    std::vector<std::string> tmp;
    boost::split(tmp, it->first, boost::is_any_of(":"), boost::token_compress_on);

    if (tmp.size() > 2)
      if ((tmp[0] == "gdal") && (tmp[1] == "co"))
      {
        m_Options.gdalCreationOptions.first = true;
        m_Options.gdalCreationOptions.second.push_back(tmp[2] + "=" + it->second);
      }
  }

  if (!map["writegeom"].empty())
  {
    m_Options.writeGEOMFile.first = true;
    if (map["writegeom"] == "Off" || map["writegeom"] == "off" || map["writegeom"] == "OFF" ||
        map["writegeom"] == "false" || map["writegeom"] == "False" || map["writegeom"] == "0")
    {
      m_Options.writeGEOMFile.second = false;
    }
  }
  /*
   check nodata value(s) for each band and make a list of
   band-nodata pairs
  */
  auto iter = map.find("nodata");
  if (iter != map.end())
  {
    auto const&              nodata_values = iter->second;
    std::vector<std::string> nodata_list;
    boost::split(nodata_list, nodata_values, boost::is_any_of(","), boost::token_compress_on);
    for (auto const& nodata_pair : nodata_list)
    {
      std::vector<std::string> per_band_no_data;
      boost::split(per_band_no_data, nodata_pair, boost::is_any_of(":"), boost::token_compress_on);

      if (per_band_no_data.size() == 1)
      {
        double val = Utils::LexicalCast<double>(per_band_no_data[0], "nodata value");
        m_NoDataList.push_back({1, val});
      }
      else
      {
        int    band = Utils::LexicalCast<int>(per_band_no_data[0], "nodata value");
        double val  = Utils::LexicalCast<double>(per_band_no_data[1], "nodata value");
        m_NoDataList.push_back({band, val});
      }
    }
    has_noDataValue = true;
  }

  if (!map["writerpctags"].empty())
  {
    m_Options.writeRPCTags.first = true;
    if (map["writerpctags"] == "On" || map["writerpctags"] == "on" || map["writerpctags"] == "ON" ||
        map["writerpctags"] == "true" || map["writerpctags"] == "True" || map["writerpctags"] == "1")
    {
      m_Options.writeRPCTags.second = true;
    }
  }

  if (!map["multiwrite"].empty())
  {
    m_Options.multiWrite.first = true;
    if (map["multiwrite"] == "Off" || map["multiwrite"] == "off" || map["multiwrite"] == "OFF" ||
        map["multiwrite"] == "false" || map["multiwrite"] == "False" || map["multiwrite"] == "0")
    {
      m_Options.multiWrite.second = false;
    }
  }

  if (!map["streaming:type"].empty())
  {
    if (map["streaming:type"] == "auto" || map["streaming:type"] == "tiled" ||
        map["streaming:type"] == "stripped" || map["streaming:type"] == "none")
    {
      m_Options.streamingType.first  = true;
      m_Options.streamingType.second = map["streaming:type"];
    }
    else
    {
      itkWarningMacro("Unkwown value " << map["streaming:type"] << " for streaming:type option. Available values are auto,tiled,stripped.");
    }
  }

  if (!map["streaming:sizemode"].empty())
  {
    if (map["streaming:sizemode"] == "auto" || map["streaming:sizemode"] == "nbsplits" || map["streaming:sizemode"] == "height")
    {
      m_Options.streamingSizeMode.first  = true;
      m_Options.streamingSizeMode.second = map["streaming:sizemode"];
    }
    else
    {
      itkWarningMacro("Unkwown value " << map["streaming:sizemode"] << " for streaming:sizemode option. Available values are auto,nbsplits,height.");
    }
  }

  if (!map["streaming:sizevalue"].empty())
  {
    m_Options.streamingSizeValue.first  = true;
    m_Options.streamingSizeValue.second = atof(map["streaming:sizevalue"].c_str());
  }

  // Manage region size to write in output image
  if (!map["box"].empty())
  {
    itksys::RegularExpression reg;
    reg.compile("([^0-9\\.]:[^0-9\\.]:[^0-9\\.]:[^0-9\\.])");

    if (!reg.find(map["box"]))
    {
      m_Options.box.first  = true;
      m_Options.box.second = map["box"];
    }
    else
    {
      itkWarningMacro("Unkwown value " << map["box"] << " for image region to write. Must be 'xmin ymin sizex sizey', with whitespace as separator");
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
      itkWarningMacro("Unkwown value "
                      << map["bands"]
                      << " for band range. Expect a list of tokens separated with comma (each token being a single band index or a range in the form x:y)");
    }
  }

  if (!map["epsg"].empty())
  {
	int code;
	try
	{
	  code = std::stoi(map["epsg"]);
	}
	catch(const std::invalid_argument& e)
	{
	  itkWarningMacro("Invalid value ("
                      << map["epsg"]
                      << ") for Geographic coordinate reference system. Must be integer.");
      code = 0;
	}
	if (code < 0)
	{
      itkWarningMacro("Invalid value ("
		               << map["epsg"]
		               << ") for Geographic coordinate reference system. Must be positive.");
      code = 0;
	}
    if (code > 0)
    {
        m_Options.srsValue.first  = true;
        m_Options.srsValue.second = (unsigned int) code;
    }
  }

  // Option Checking
  for (it = map.begin(); it != map.end(); it++)
  {
    std::vector<std::string> tmp;
    boost::split(tmp, it->first, boost::is_any_of(":"), boost::token_compress_on);
    if (tmp.size() > 2)
    {
      if (!(tmp[0] == "gdal") || !(tmp[1] == "co"))
      {
        itkWarningMacro("Unknown option detected: " << it->first << ".");
      }
    }
    else
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
}

bool ExtendedFilenameToWriterOptions::NoDataValueIsSet() const
{
  return has_noDataValue;
}

bool ExtendedFilenameToWriterOptions::SimpleFileNameIsSet() const
{
  return m_Options.simpleFileName.first;
}

bool ExtendedFilenameToWriterOptions::WriteGEOMFileIsSet() const
{
  return m_Options.writeGEOMFile.first;
}

bool ExtendedFilenameToWriterOptions::WriteRPCTagsIsSet() const
{
  return m_Options.writeRPCTags.first;
}

bool ExtendedFilenameToWriterOptions::GetWriteGEOMFile() const
{
  return m_Options.writeGEOMFile.second;
}

bool ExtendedFilenameToWriterOptions::GetMultiWrite() const
{
  return m_Options.multiWrite.second;
}

bool ExtendedFilenameToWriterOptions::GetWriteRPCTags() const
{
  return m_Options.writeRPCTags.second;
}

bool ExtendedFilenameToWriterOptions::gdalCreationOptionsIsSet() const
{
  return m_Options.gdalCreationOptions.first;
}
ExtendedFilenameToWriterOptions::GDALCOType ExtendedFilenameToWriterOptions::GetgdalCreationOptions() const
{
  return m_Options.gdalCreationOptions.second;
}

bool ExtendedFilenameToWriterOptions::StreamingTypeIsSet() const
{
  return m_Options.streamingType.first;
}

bool ExtendedFilenameToWriterOptions::StreamingSizeModeIsSet() const
{
  return m_Options.streamingSizeMode.first;
}

bool ExtendedFilenameToWriterOptions::StreamingSizeValueIsSet() const
{
  return m_Options.streamingSizeValue.first;
}

std::string ExtendedFilenameToWriterOptions::GetStreamingType() const
{
  return m_Options.streamingType.second;
}

std::string ExtendedFilenameToWriterOptions::GetStreamingSizeMode() const
{
  return m_Options.streamingSizeMode.second;
}

double ExtendedFilenameToWriterOptions::GetStreamingSizeValue() const
{
  return m_Options.streamingSizeValue.second;
}

bool ExtendedFilenameToWriterOptions::BoxIsSet() const
{
  return m_Options.box.first;
}

std::string ExtendedFilenameToWriterOptions::GetBox() const
{
  return m_Options.box.second;
}

bool ExtendedFilenameToWriterOptions::BandRangeIsSet() const
{
  return m_Options.bandRange.first;
}

std::string ExtendedFilenameToWriterOptions::GetBandRange() const
{
  return m_Options.bandRange.second;
}

bool ExtendedFilenameToWriterOptions::SrsValueIsSet() const
{
	return m_Options.srsValue.first;
}

unsigned int ExtendedFilenameToWriterOptions::GetSrsValue() const
{
  return m_Options.srsValue.second;
}

} // end namespace otb
