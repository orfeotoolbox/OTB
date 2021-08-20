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
  return (sensorID.find("TSX-1") != std::string::npos ||
	  sensorID.find("PAZ-1") != std::string::npos);
}


double TerraSarXSarImageMetadataInterface::GetStartTimeUTC() const
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

  std::string referenceUTC = imageKeywordlist.GetMetadataByKey("support_data.first_line_time");
  double      julianDay    = this->ConvertStringTimeUTCToJulianDay(referenceUTC);

  return julianDay;
}

double TerraSarXSarImageMetadataInterface::GetStopTimeUTC() const
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

  std::string referenceUTC = imageKeywordlist.GetMetadataByKey("support_data.last_line_time");
  double      julianDay    = this->ConvertStringTimeUTCToJulianDay(referenceUTC);

  return julianDay;
}

double TerraSarXSarImageMetadataInterface::GetRangeTimeFirstPixel() const
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

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.slant_range_to_first_pixel");
  double value = 0;
    try
      {
	value = std::stod(valueString);
      }
    catch( ... )
      {
	// Throw an exception
	throw std::runtime_error("Failed to convert first range pixel.");
      }

  return value;
}

double TerraSarXSarImageMetadataInterface::GetRangeTimeLastPixel() const
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

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.slant_range_to_last_pixel");
  double value = 0;
    try
      {
	value = std::stod(valueString);
      }
    catch( ... )
      {
	// Throw an exception
	throw std::runtime_error("Failed to convert last range pixel.");
      }
  return value;
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
      // Throw an exception
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
      // Throw an exception
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
      // Throw an exception
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
      // Throw an exception
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
      // Throw an exception
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
      // Throw an exception
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
      // Throw an exception
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
      // Throw an exception
      throw std::runtime_error("Failed to convert the year.");
    }

  return value;
}


double TerraSarXSarImageMetadataInterface::ConvertStringTimeUTCToJulianDay(const std::string& value) const
{
  std::vector<std::string> splitDate;
  boost::split(splitDate, value, boost::is_any_of("-T:Z"));

  int year, month, day, hour, minu;
  double sec;

  try
  {
    year  = std::stoi(splitDate[0].c_str());
    month = std::stoi(splitDate[1].c_str());
    day   = std::stoi(splitDate[2].c_str());
    hour  = std::stoi(splitDate[3].c_str());
    minu  = std::stoi(splitDate[4].c_str());
    sec   = std::stod(splitDate[5].c_str());
  }
  catch( ... )
  {
    throw std::runtime_error("Failed to parse input date ");
  }

  // Conversion to julian day
  // according to http://en.wikipedia.org/wiki/Julian_day
  // division are integer divisions:
  int a = (14 - month) / 12;
  int y = year + 4800 - a;
  int m = month + 12 * a - 3;

  double julianDay = day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;

  // now, the division are NOT integer
  julianDay += hour / 24. + minu / 1440. + sec / 86400.;

  return julianDay;
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
      // Throw an exception
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
      // Throw an exception
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
      // Throw an exception
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
      // Throw an exception
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
	// Throw an exception
	throw std::runtime_error("Failed to convert calibration factor.");
      }
    return value;
  }

  return std::numeric_limits<double>::quiet_NaN(); // Invalid value
}

double TerraSarXSarImageMetadataInterface::GetRadiometricCalibrationScale() const
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
  double value = 0;
  if (imageKeywordlist.HasKey(key))
  {
    std::string valueString = imageKeywordlist.GetMetadataByKey(key);

    try
      {
	value = std::stod(valueString);
      }
    catch( ... )
      {
	// Throw an exception
	throw std::runtime_error("Failed to convert calibration factor.");
      }
  }
  return value;
}


// Noise
double TerraSarXSarImageMetadataInterface::GetNoiseTimeUTC(unsigned int noiseRecord) const
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

  DoubleVectorType   timeList;
  std::ostringstream oss;

  oss.str("");
  oss << "noise[" << noiseRecord << "]imageNoise.timeUTC";
  std::string key = oss.str();
  if (imageKeywordlist.HasKey(key))
  {
    std::string valueString = imageKeywordlist.GetMetadataByKey(key);
    double      julianDay   = ConvertStringTimeUTCToJulianDay(valueString);
    return julianDay;
  }

  return std::numeric_limits<double>::quiet_NaN(); // Invalid value
}

double TerraSarXSarImageMetadataInterface::GetNoiseReferencePoint(unsigned int noiseRecord) const
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

  std::ostringstream oss;

  oss.str("");
  oss << "noise[" << noiseRecord << "]imageNoise.noiseEstimate.referencePoint";
  std::string valueString = imageKeywordlist.GetMetadataByKey(oss.str());
  double refPoint = 0;
    try
      {
	refPoint = std::stod(valueString);
      }
    catch( ... )
      {
	// Throw an exception
	throw std::runtime_error("Failed to convert noise.");
      }

  return refPoint;
}



unsigned int TerraSarXSarImageMetadataInterface::GetNoisePolynomialDegrees(unsigned int noiseRecord) const
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

  std::ostringstream oss;
  oss.str("");
  oss << "noise[" << noiseRecord << "]imageNoise.noiseEstimate.polynomialDegree";
  std::string key = oss.str();
  if (imageKeywordlist.HasKey(key))
  {
    std::string  valueString = imageKeywordlist.GetMetadataByKey(key);

    unsigned int value = 0;
    try
      {
	value = std::stoi(valueString);
      }
    catch( ... )
      {
	// Throw an exception
	throw std::runtime_error("Failed to convert polynomial degree.");
      }

    return value;
  }

  return std::numeric_limits<unsigned int>::quiet_NaN(); // Invalid value
}



TerraSarXSarImageMetadataInterface::UIntVectorType TerraSarXSarImageMetadataInterface::GetNoisePolynomialDegrees() const
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

  UIntVectorType     polDeg;
  std::ostringstream oss;

  unsigned int nbRec = this->GetNumberOfNoiseRecords();
  for (unsigned int i = 0; i < nbRec; ++i)
  {
    oss.str("");
    oss << "noise[" << i << "]imageNoise.noiseEstimate.polynomialDegree";
    std::string  key         = oss.str();
    std::string  valueString = imageKeywordlist.GetMetadataByKey(key);
    unsigned int value = 0;
    try
      {
	value = std::stoi(valueString);
      }
    catch( ... )
      {
	// Throw an exception
	throw std::runtime_error("Failed to convert polynomial degree.");
      }

    polDeg.push_back(value);
  }

  return polDeg;
}


unsigned int TerraSarXSarImageMetadataInterface::GetNumberOfNoiseRecords() const
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

  std::string key("noise.numberOfNoiseRecords");
  if (imageKeywordlist.HasKey(key))
  {
    std::string  valueString = imageKeywordlist.GetMetadataByKey(key);
    unsigned int value = 0;
    try
      {
	value = std::stoi(valueString);
      }
    catch( ... )
      {
	// Throw an exception
	throw std::runtime_error("Failed to convert number of noise records.");
      }

    return value;
  }

  return std::numeric_limits<unsigned int>::quiet_NaN(); // Invalid value
}




double TerraSarXSarImageMetadataInterface::Horner(std::vector<double>& coefficients, const double tauMinusTauRef) const
{
  std::vector<double>::reverse_iterator coefIt = coefficients.rbegin();
  double                                res    = *(coefIt);
  ++coefIt;

  while (coefIt < coefficients.rend())
  {
    // Cumulate polynomial
    res = res * tauMinusTauRef + (*coefIt);
    ++coefIt;
  }
  return res;
}

TerraSarXSarImageMetadataInterface::DoubleVectorType TerraSarXSarImageMetadataInterface::GetNoisePolynomialCoefficients(unsigned int noiseRecord) const
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

  unsigned int polDegs = this->GetNoisePolynomialDegrees(noiseRecord);

  DoubleVectorType   polCoef;
  std::ostringstream oss;

  polCoef.clear();
  // set <= condition because degree N means N+1 coeff
  for (unsigned int j = 0; j <= polDegs; ++j)
  {
    oss.str("");
    oss << "noise[" << noiseRecord << "]imageNoise.noiseEstimate.coefficient[" << j << "]";
    std::string key         = oss.str();
    std::string valueString = imageKeywordlist.GetMetadataByKey(key);

    double value = 0;
    try
      {
	value = std::stod(valueString);
      }
    catch( ... )
      {
	// Throw an exception
	throw std::runtime_error("Failed to convert polynomial coefficients.");
      }

    polCoef.push_back(value);
  }

  return polCoef;
}

TerraSarXSarImageMetadataInterface::IndexType TerraSarXSarImageMetadataInterface::GetRadiometricCalibrationNoisePolynomialDegree() const
{
  IndexType polynomSize;
  polynomSize[0] = 2;
  polynomSize[1] = 2;

  return polynomSize;
}


TerraSarXSarImageMetadataInterface::PointSetPointer TerraSarXSarImageMetadataInterface::GetRadiometricCalibrationNoise() const
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

  PointSetPointer points = PointSetType::New();

  std::string keyNbRow("number_lines");
  std::string keyNbCol("number_samples");
  std::string  nbRowString = imageKeywordlist.GetMetadataByKey(keyNbRow);
  std::string  nbColString = imageKeywordlist.GetMetadataByKey(keyNbCol);
  unsigned int numberOfRows = 0;
  unsigned int numberOfCols = 0;
  try
    {
      numberOfRows = std::stoi(nbRowString);
      numberOfCols = std::stoi(nbColString);
    }
  catch( ... )
    {
      // Throw an exception
      throw std::runtime_error("Failed to number of lines or colunms.");
    }


  double   startTime      = this->GetStartTimeUTC();
  double   stopTime       = this->GetStopTimeUTC();
  RealType firstRangeTime = this->GetRangeTimeFirstPixel();
  RealType lastRangeTime  = this->GetRangeTimeLastPixel();

  points->Initialize();
  unsigned int noPoint = 0;

  PointType p0;

  unsigned int numberOfNoiseRecords = this->GetNumberOfNoiseRecords();

  for (unsigned int noiseRecord = 0; noiseRecord < numberOfNoiseRecords; ++noiseRecord)
  {
    double   currentNoiseTime   = this->GetNoiseTimeUTC(noiseRecord);
    RealType AzimutAcquisition  = (currentNoiseTime - startTime) * numberOfRows / (stopTime - startTime);
    RealType referencePointTime = this->GetNoiseReferencePoint(noiseRecord);

    std::vector<RealType> polynomialCoefficient;
    polynomialCoefficient = this->GetNoisePolynomialCoefficients(noiseRecord);

    p0[0] = AzimutAcquisition;

    for (unsigned int col = 0; col < numberOfCols; ++col)
    {
      RealType rangeTime      = col * (lastRangeTime - firstRangeTime) / (numberOfCols) + firstRangeTime;
      RealType tauMinusTauRef = rangeTime - referencePointTime;
      RealType value          = this->Horner(polynomialCoefficient, tauMinusTauRef);

      p0[1] = col;
      points->SetPoint(noPoint, p0);
      points->SetPointData(noPoint, value);
      ++noPoint;
    }
  }
  return points;
}


// CalibrationIncidenceAngle
unsigned int TerraSarXSarImageMetadataInterface::GetNumberOfCornerIncidenceAngles() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string valueString = imageKeywordlist.GetMetadataByKey("sceneCoord.numberOfSceneCornerCoord");
  int value = 0;
  try
    {
      value = std::stoi(valueString);
    }
  catch( ... )
    {
      // Throw an exception
      throw std::runtime_error("Failed to convert number of scene coords.");
    }

  return value;
}


TerraSarXSarImageMetadataInterface::IndexType TerraSarXSarImageMetadataInterface::GetCenterIncidenceAngleIndex() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  IndexType it;

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string valueString1 = imageKeywordlist.GetMetadataByKey("sceneCoord.sceneCenterCoord.refRow");
  std::string valueString0 = imageKeywordlist.GetMetadataByKey("sceneCoord.sceneCenterCoord.refColumn");
  it[1] = 0;
  it[0] = 0;

  try
    {
      it[1] = std::stoi(valueString1);
      it[0] = std::stoi(valueString0);
    }
  catch( ... )
    {
      // Throw an exception
      throw std::runtime_error("Failed to convert indexes.");
    }

  return it;
}

TerraSarXSarImageMetadataInterface::DoubleVectorType TerraSarXSarImageMetadataInterface::GetCornersIncidenceAngles() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  ImageKeywordlistType imageKeywordlist;

  DoubleVectorType dv;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::ostringstream oss;

  unsigned int nbAngles = this->GetNumberOfCornerIncidenceAngles();
  for (unsigned int i = 0; i < nbAngles; ++i)
  {
    oss.str("");
    oss << "sceneCoord.sceneCornerCoord[" << i << "].incidenceAngle";
    std::string valueString = imageKeywordlist.GetMetadataByKey(oss.str());

    double value = 0;
    try
      {
	value = std::stod(valueString);
      }
    catch( ... )
      {
	// Throw an exception
	throw std::runtime_error("Failed to convert angles.");
      }

    dv.push_back(value);
  }

  return dv;
}

TerraSarXSarImageMetadataInterface::IndexVectorType TerraSarXSarImageMetadataInterface::GetCornersIncidenceAnglesIndex() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  ImageKeywordlistType imageKeywordlist;

  IndexVectorType iv;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }


  std::ostringstream oss;
  std::ostringstream oss2;

  unsigned int nbAngles = this->GetNumberOfCornerIncidenceAngles();
  for (unsigned int i = 0; i < nbAngles; ++i)
  {
    IndexType it;

    oss.str("");
    oss << "sceneCoord.sceneCornerCoord[" << i << "].refRow";

    std::string valueString1 = imageKeywordlist.GetMetadataByKey(oss.str());

    oss2.str("");
    oss2 << "sceneCoord.sceneCornerCoord[" << i << "].refColumn";

    std::string valueString0 = imageKeywordlist.GetMetadataByKey(oss2.str());

    it[1] = 0;
    it[0] = 0;

    try
      {
	it[1] = std::stoi(valueString1);
	it[0] = std::stoi(valueString0);
      }
    catch( ... )
      {
	// Throw an exception
	throw std::runtime_error("Failed to convert indexes.");
      }

    iv.push_back(it);
  }

  return iv;
}



TerraSarXSarImageMetadataInterface::PointSetPointer TerraSarXSarImageMetadataInterface::GetRadiometricCalibrationIncidenceAngle() const
{
  PointSetPointer points = PointSetType::New();
  PointType       p0;

  double    centerIncidenceAngleValue = this->GetCenterIncidenceAngle();
  IndexType centerIncidenceAngleIndex = this->GetCenterIncidenceAngleIndex();

  DoubleVectorType cornerIncidenceAngleValue = this->GetCornersIncidenceAngles();
  IndexVectorType  cornerIncidenceAngleIndex = this->GetCornersIncidenceAnglesIndex();
  points->Initialize();
  unsigned int noPoint = 0;

  p0[0] = centerIncidenceAngleIndex[0];
  p0[1] = centerIncidenceAngleIndex[1];

  points->SetPoint(noPoint, p0);
  points->SetPointData(noPoint, centerIncidenceAngleValue * CONST_PI_180);
  ++noPoint;

  for (unsigned int i = 0; i < cornerIncidenceAngleIndex.size(); ++i)
  {

    p0[0] = cornerIncidenceAngleIndex.at(i)[0];
    p0[1] = cornerIncidenceAngleIndex.at(i)[1];

    points->SetPoint(noPoint, p0);
    points->SetPointData(noPoint, cornerIncidenceAngleValue[i] * CONST_PI_180);
    ++noPoint;
  }
  return points;
}

TerraSarXSarImageMetadataInterface::IndexType TerraSarXSarImageMetadataInterface::GetRadiometricCalibrationIncidenceAnglePolynomialDegree() const
{
  IndexType polynomSize;
  polynomSize[0] = 2;
  polynomSize[1] = 1;

  return polynomSize;
}



}
