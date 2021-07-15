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


#include "otbTerraSarXSarImageMetadataInterface.h"

#include "otbStringUtils.h"
#include "otbMath.h"
#include "itkIndex.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"

namespace otb
{

bool TerraSarXSarImageMetadataInterface::CanRead() const
{
  std::string sensorID = GetSensorID();

  // TerraSarXSar model works for PAZ-1 mission as well as TSX-1
  return (sensorID.find("TSX") != std::string::npos ||
	  sensorID.find("PAZ-1") != std::string::npos);
}

int TerraSarXSarImageMetadataInterface::GetDay() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string key("support_data.first_line_time");
  if (!imageKeywordlist.HasKey(key))
  {
    return -1;
  }

  std::string              valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2)
    itkExceptionMacro(<< "Invalid Day");

  int value = 0;
  try
    {
      value = std::stoi(outputValues[2]);
    }
  catch( ... )
    {
      // Throw an execption
      throw std::runtime_error("Failed to convert the day.");
    }

  return value;
}

int TerraSarXSarImageMetadataInterface::GetMonth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string key("support_data.first_line_time");
  if (!imageKeywordlist.HasKey(key))
  {
    return -1;
  }

  std::string              valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2)
    itkExceptionMacro(<< "Invalid Month");

  int value = 0;
  try
    {
      value = std::stoi(outputValues[1]);
    }
  catch( ... )
    {
      // Throw an execption
      throw std::runtime_error("Failed to convert the month.");
    }

  return value;
}

int TerraSarXSarImageMetadataInterface::GetYear() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string key("support_data.first_line_time");
  if (!imageKeywordlist.HasKey(key))
  {
    return -1;
  }

  std::string              valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2)
    itkExceptionMacro(<< "Invalid Year");

  int value = 0;
  try
    {
      value = std::stoi(outputValues[0]);
    }
  catch( ... )
    {
      // Throw an execption
      throw std::runtime_error("Failed to convert the year.");
    }

  return value;
}

int TerraSarXSarImageMetadataInterface::GetHour() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string key("support_data.first_line_time");
  if (!imageKeywordlist.HasKey(key))
  {
    return -1;
  }

  std::string              valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 4)
    itkExceptionMacro(<< "Invalid Hour");

  int value = 0;
  try
    {
      value = std::stoi(outputValues[3]);
    }
  catch( ... )
    {
      // Throw an execption
      throw std::runtime_error("Failed to convert hour.");
    }
  return value;
}

int TerraSarXSarImageMetadataInterface::GetMinute() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string key("support_data.first_line_time");
  if (!imageKeywordlist.HasKey(key))
  {
    return -1;
  }

  std::string              valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 4)
    itkExceptionMacro(<< "Invalid Minute");

  int value = 0;
  try
    {
      value = std::stoi(outputValues[4]);
    }
  catch( ... )
    {
      // Throw an execption
      throw std::runtime_error("Failed to convert minutes.");
    }

  return value;
}

int TerraSarXSarImageMetadataInterface::GetProductionDay() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }
  std::string key("generation_time");
  if (!imageKeywordlist.HasKey(key))
  {
    return -1;
  }

  std::string              valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2)
    itkExceptionMacro(<< "Invalid Production Day");

  int value = 0;
  try
    {
      value = std::stoi(outputValues[2]);
    }
  catch( ... )
    {
      // Throw an execption
      throw std::runtime_error("Failed to convert the day.");
    }

  return value;
}

int TerraSarXSarImageMetadataInterface::GetProductionMonth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string key("generation_time");
  if (!imageKeywordlist.HasKey(key))
  {
    return -1;
  }

  std::string              valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2)
    itkExceptionMacro(<< "Invalid Production Month");

  int value = 0;
  try
    {
      value = std::stoi(outputValues[1]);
    }
  catch( ... )
    {
      // Throw an execption
      throw std::runtime_error("Failed to convert the month.");
    }

  return value;
}

int TerraSarXSarImageMetadataInterface::GetProductionYear() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string key("generation_time");
  if (!imageKeywordlist.HasKey(key))
  {
    return -1;
  }

  std::string              valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2)
    itkExceptionMacro(<< "Invalid Production Year");

  int value = 0;
  try
    {
      value = std::stoi(outputValues[0]);
    }
  catch( ... )
    {
      // Throw an execption
      throw std::runtime_error("Failed to convert the year.");
    }

  return value;
}


double TerraSarXSarImageMetadataInterface::GetRadarFrequency() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string valueString = imageKeywordlist.GetMetadataByKey("radarFrequency");

  double freq = 0;
  try
    {
      freq = std::stod(valueString);
    }
  catch( ... )
    {
      // Throw an execption
      throw std::runtime_error("Failed to convert frequency.");
    }

  return freq;
}

double TerraSarXSarImageMetadataInterface::GetPRF() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }
  std::string valueString = imageKeywordlist.GetMetadataByKey("sensor_params.prf");

  double freq = 0;
  try
    {
      freq = std::stod(valueString);
    }
  catch( ... )
    {
      // Throw an execption
      throw std::runtime_error("Failed to convert PRF.");
    }

  return freq;
}

double TerraSarXSarImageMetadataInterface::GetRSF() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string valueString = imageKeywordlist.GetMetadataByKey("sensor_params.sampling_frequency");

  double freq = 0;
  try
    {
      freq = std::stod(valueString);
    }
  catch( ... )
    {
      // Throw an execption
      throw std::runtime_error("Failed to convert RSF.");
    }

  return freq;
}

double TerraSarXSarImageMetadataInterface::GetCenterIncidenceAngle() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string valueString = imageKeywordlist.GetMetadataByKey("sceneCoord.sceneCenterCoord.incidenceAngle");

  double value = 0;
  try
    {
      value = std::stod(valueString);
    }
  catch( ... )
    {
      // Throw an execption
      throw std::runtime_error("Failed to convert the incidence angle.");
    }

  return value;
}

TerraSarXSarImageMetadataInterface::UIntVectorType TerraSarXSarImageMetadataInterface::GetDefaultDisplay() const
{
  UIntVectorType rgb(3);
  rgb[0] = 0;
  rgb[1] = 0;
  rgb[2] = 0;
  return rgb;
}


double TerraSarXSarImageMetadataInterface::GetCalibrationFactor() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string key("calibration.calibrationConstant.calFactor");
  if (imageKeywordlist.HasKey(key))
  {
    std::string valueString = imageKeywordlist.GetMetadataByKey(key);

    double value = 0;
    try
      {
	value = std::stod(valueString);
      }
    catch( ... )
      {
	// Throw an execption
	throw std::runtime_error("Failed to convert calibration factor.");
      }
    return value;
  }

  return std::numeric_limits<double>::quiet_NaN(); // Invalid value
}


}
