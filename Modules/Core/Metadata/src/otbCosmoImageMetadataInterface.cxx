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
#include "otbCosmoImageMetadataInterface.h"

#include "otbMacro.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"
#include "ossim/ossimTimeUtilities.h"

//useful constants
#include <otbMath.h>
#include <iomanip>

namespace otb
{


bool
CosmoImageMetadataInterface::CanRead() const
{
  const std::string sensorID = GetSensorID();  

  return sensorID.find("CSK") != std::string::npos;
}

void
CosmoImageMetadataInterface
::ParseDateTime(std::string key, std::vector<int>& dateFields) const
{
  if(dateFields.empty())
    {
    //parse from keyword list
    if (!this->CanRead())
      {
      itkExceptionMacro(<< "Invalid Metadata, not a valid product");
      }

    const ImageKeywordlistType imageKeywordlist  = this->GetImageKeywordlist();
    if (!imageKeywordlist.HasKey(key))
      {
      itkExceptionMacro( << "no key named " << key );
      }

    const std::string date_time_str = imageKeywordlist.GetMetadataByKey(key);
    Utils::ConvertStringToVector(date_time_str, dateFields, key, "T:-.");
    }
}

int
CosmoImageMetadataInterface::GetYear() const
{
  int value = 0;
  if (m_AcquisitionDateFields.empty())
    {
      ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
    }
      
  if(m_AcquisitionDateFields.size() > 0 )
    {
    value = Utils::LexicalCast<int>( m_AcquisitionDateFields[0], "support_data.image_date:year(int)" );
    }
  else
    {
    itkExceptionMacro( << "Invalid year" );
    }
  return value;
}

int
CosmoImageMetadataInterface::GetMonth() const
{
  int value = 0;
  if (m_AcquisitionDateFields.empty())
    {
      ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
    }

  if(m_AcquisitionDateFields.size() > 1 )
    {
    value = Utils::LexicalCast<int>( m_AcquisitionDateFields[1], "support_data.image_date:month(int)" );
    }
  else
    {
    itkExceptionMacro( << "Invalid month" );
    }
  return value;
}

int
CosmoImageMetadataInterface::GetDay() const
{
  int value = 0;
  if (m_AcquisitionDateFields.empty())
    {
      ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
    }

  if(m_AcquisitionDateFields.size() > 2 )
    {
    value = Utils::LexicalCast<int>( m_AcquisitionDateFields[2], "support_data.image_date:day(int)");
    }
  else
    {
    itkExceptionMacro( << "Invalid day" );
    }
  return value;
}

int
CosmoImageMetadataInterface::GetHour() const
{
  int value = 0;
  if (m_AcquisitionDateFields.empty())
    {
      ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
    }

  if(m_AcquisitionDateFields.size() > 3 )
    {
    value = Utils::LexicalCast<int>( m_AcquisitionDateFields[3], "support_data.image_date:hour(int)");
    }
  else
    {
    itkExceptionMacro( << "Invalid hour" );
    }
  return value;
}

int
CosmoImageMetadataInterface::GetMinute() const
{
  int value = 0;
  if (m_AcquisitionDateFields.empty())
    {
      ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
    }
  
  if(m_AcquisitionDateFields.size() > 4 )
    {
    value = Utils::LexicalCast<int>( m_AcquisitionDateFields[4], "support_data.image_date:minute(int)");
    }
  else
    {
    itkExceptionMacro( << "Invalid minute" );
    }
  return value;
}

int
CosmoImageMetadataInterface::GetProductionYear() const
{
  int value = 0;
  ParseDateTime("support_data.date", m_ProductionDateFields);
  if(m_ProductionDateFields.size() > 0 )
    {
    value = Utils::LexicalCast<int>( m_ProductionDateFields[0], "support_data.date:year(int)" );
    }
  else
    {
    itkExceptionMacro( << "Invalid production year" );
    }
  return value;
}

int
CosmoImageMetadataInterface::GetProductionMonth() const
{
  int value = 0;
  ParseDateTime("support_data.date", m_ProductionDateFields);
  if(m_ProductionDateFields.size() > 1 )
    {
    value = Utils::LexicalCast<int>( m_ProductionDateFields[1], "support_data.date:month(int)" );
    }
  else
    {
    itkExceptionMacro( << "Invalid production month" );
    }
  return value;
}

int
CosmoImageMetadataInterface::GetProductionDay() const
{
  int value = 0;
  ParseDateTime("support_data.date", m_ProductionDateFields);
  if(m_ProductionDateFields.size() > 2 )
    {
    value = Utils::LexicalCast<int>( m_ProductionDateFields[2], "support_data.date:day(int)" );
    }
  else
    {
    itkExceptionMacro( << "Invalid production day" );
    }
  return value;
}

double
CosmoImageMetadataInterface::GetPRF() const
{
  double value = 0;
  const ImageKeywordlistType imageKeywordlist  = this->GetImageKeywordlist();
  if (!imageKeywordlist.HasKey("support_data.pulse_repetition_frequency"))
    {
    return value;
    }

  value = Utils::LexicalCast<double>( imageKeywordlist.GetMetadataByKey("support_data.pulse_repetition_frequency"),
                                      "support_data.pulse_repetition_frequency(double)" );

  return value;
}


CosmoImageMetadataInterface::UIntVectorType
CosmoImageMetadataInterface::GetDefaultDisplay() const
{
  UIntVectorType rgb(3);
  rgb[0] = 0;
  rgb[1] = 0;
  rgb[2] = 0;
  return rgb;
}

double
CosmoImageMetadataInterface::GetRSF() const
{
  return 0;
}

double
CosmoImageMetadataInterface::GetRadarFrequency() const
{
  return 0;
}

double
CosmoImageMetadataInterface::GetCenterIncidenceAngle() const
{
  return 0;
}

} // end namespace otb
