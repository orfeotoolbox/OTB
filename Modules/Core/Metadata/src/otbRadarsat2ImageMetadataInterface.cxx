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


#include "otbSarImageMetadataInterface.h"
#include "otbRadarsat2ImageMetadataInterface.h"

#include "otbMacro.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"

// useful constants
#include <otbMath.h>

namespace otb
{

Radarsat2ImageMetadataInterface::Radarsat2ImageMetadataInterface()
{
}

bool Radarsat2ImageMetadataInterface::CanRead() const
{
  std::string sensorID = GetSensorID();

  if (sensorID.find("RADARSAT-2") != std::string::npos)
  {
    return true;
  }
  else
    return false;
}


void Radarsat2ImageMetadataInterface::CreateCalibrationLookupData(const short type)
{
  std::string lut = "SigmaNought";

  switch (type)
  {
  case SarCalibrationLookupData::BETA:
  {
    lut = "BetaNought";
  }
  break;

  case SarCalibrationLookupData::GAMMA:
  {
    lut = "GammaNought";
  }
  break;

  case SarCalibrationLookupData::DN:
  {
    lut = "DN";
  }
  break;

  case SarCalibrationLookupData::SIGMA:
  default:
  {
    lut = "SigmaNought";
  }
  break;
  }

  const ImageKeywordlistType imageKeywordlist = this->GetImageKeywordlist();
  const std::string          key              = "referenceNoiseLevel[" + lut + "].gain";

  Radarsat2CalibrationLookupData::GainListType glist;
  int                                          offset = 0;

  Utils::ConvertStringToVector(imageKeywordlist.GetMetadataByKey("referenceNoiseLevel[" + lut + "].gain"), glist, "referenceNoiseLevel[" + lut + "].gain");

  Utils::LexicalCast<int>(imageKeywordlist.GetMetadataByKey("referenceNoiseLevel[" + lut + "].offset"), "referenceNoiseLevel[" + lut + "].offset");

  Radarsat2CalibrationLookupData::Pointer sarLut;
  sarLut = Radarsat2CalibrationLookupData::New();
  sarLut->InitParameters(type, offset, glist);
  this->SetCalibrationLookupData(sarLut);
}

void Radarsat2ImageMetadataInterface::ParseDateTime(const char* key, std::vector<int>& dateFields) const
{
  if (dateFields.size() < 1)
  {
    // parse from keyword list
    if (!this->CanRead())
    {
      itkExceptionMacro(<< "Invalid Metadata, not a valid product");
    }

    const ImageKeywordlistType imageKeywordlist = this->GetImageKeywordlist();
    if (!imageKeywordlist.HasKey(key))
    {
      itkExceptionMacro(<< "no key named '" << key << "'");
    }

    std::string date_time_str = imageKeywordlist.GetMetadataByKey(key);
    date_time_str.resize(date_time_str.size() - 1);
    Utils::ConvertStringToVector(date_time_str, dateFields, key, "-T:.");
  }
}

int Radarsat2ImageMetadataInterface::GetYear() const
{
  int value = 0;
  ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  if (m_AcquisitionDateFields.size() > 0)
  {
    value = Utils::LexicalCast<int>(m_AcquisitionDateFields[0], "support_data.image_date(year)");
  }
  return value;
}

int Radarsat2ImageMetadataInterface::GetMonth() const
{
  int value = 0;
  ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  if (m_AcquisitionDateFields.size() > 1)
  {
    value = Utils::LexicalCast<int>(m_AcquisitionDateFields[1], "support_data.image_date(month)");
  }
  return value;
}

int Radarsat2ImageMetadataInterface::GetDay() const
{
  int value = 0;
  ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  if (m_AcquisitionDateFields.size() > 2)
  {
    value = Utils::LexicalCast<int>(m_AcquisitionDateFields[2], "support_data.image_date(day)");
  }
  return value;
}

int Radarsat2ImageMetadataInterface::GetHour() const
{
  int value = 0;
  ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  if (m_AcquisitionDateFields.size() > 3)
  {
    value = Utils::LexicalCast<int>(m_AcquisitionDateFields[3], "support_data.image_date(hour)");
  }
  return value;
}

int Radarsat2ImageMetadataInterface::GetMinute() const
{
  int value = 0;
  ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  if (m_AcquisitionDateFields.size() > 4)
  {
    value = Utils::LexicalCast<int>(m_AcquisitionDateFields[4], "support_data.image_date(minute)");
  }
  return value;
}

int Radarsat2ImageMetadataInterface::GetProductionYear() const
{
  int value = 0;
  ParseDateTime("support_data.date", m_ProductionDateFields);
  if (m_ProductionDateFields.size() > 0)
  {
    value = Utils::LexicalCast<int>(m_ProductionDateFields[0], "support_data.image_date(year)");
  }
  return value;
}

int Radarsat2ImageMetadataInterface::GetProductionMonth() const
{
  int value = 0;
  ParseDateTime("support_data.date", m_ProductionDateFields);
  if (m_ProductionDateFields.size() > 1)
  {
    value = Utils::LexicalCast<int>(m_ProductionDateFields[1], "support_data.image_date(production month)");
  }
  return value;
}

int Radarsat2ImageMetadataInterface::GetProductionDay() const
{
  int value = 0;
  ParseDateTime("support_data.date", m_ProductionDateFields);
  if (m_ProductionDateFields.size() > 2)
  {
    value = Utils::LexicalCast<int>(m_ProductionDateFields[2], "support_data.image_date(production day)");
  }
  return value;
}


double Radarsat2ImageMetadataInterface::GetPRF() const
{
  return 0;
}

double Radarsat2ImageMetadataInterface::GetRSF() const
{
  return 0;
}

double Radarsat2ImageMetadataInterface::GetRadarFrequency() const
{
  return 0;
}

double Radarsat2ImageMetadataInterface::GetCenterIncidenceAngle() const
{
  return 0;
}

Radarsat2ImageMetadataInterface::UIntVectorType Radarsat2ImageMetadataInterface::GetDefaultDisplay() const
{
  UIntVectorType rgb(3);
  rgb[0] = 0;
  rgb[1] = 0;
  rgb[2] = 0;
  return rgb;
}

} // end namespace otb
