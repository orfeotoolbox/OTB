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

#include "otbTerraSarImageMetadataInterface.h"

#include <boost/algorithm/string.hpp>
#include "otbMacro.h"
#include "otbMath.h"
#include "itkIndex.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"

namespace otb
{

TerraSarImageMetadataInterface
::TerraSarImageMetadataInterface()
{
}

bool
TerraSarImageMetadataInterface::CanRead() const
{
  std::string sensorID = GetSensorID();
  if (sensorID.find("TSX") != std::string::npos) return true;
  else return false;
}

int
TerraSarImageMetadataInterface::GetDay() const
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

  std::string key("azimuth_start_time");
  if (!imageKeywordlist.HasKey(key))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Day");

  int value = atoi(outputValues[2].c_str());
  return value;
}

int
TerraSarImageMetadataInterface::GetMonth() const
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

  std::string key("azimuth_start_time");
  if (!imageKeywordlist.HasKey(key))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Month");

  int value = atoi(outputValues[1].c_str());
  return value;
}

int
TerraSarImageMetadataInterface::GetYear() const
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

  std::string key("azimuth_start_time");
  if (!imageKeywordlist.HasKey(key))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Year");

  int value = atoi(outputValues[0].c_str());
  return value;
}

int
TerraSarImageMetadataInterface::GetHour() const
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

  std::string key("azimuth_start_time");
  if (!imageKeywordlist.HasKey(key))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 4) itkExceptionMacro(<< "Invalid Hour");

  int value = atoi(outputValues[3].c_str());
  return value;
}

int
TerraSarImageMetadataInterface::GetMinute() const
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

  std::string key("azimuth_start_time");
  if (!imageKeywordlist.HasKey(key))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 4) itkExceptionMacro(<< "Invalid Minute");

  int value = atoi(outputValues[4].c_str());
  return value;
}

int
TerraSarImageMetadataInterface::GetProductionDay() const
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

  std::string valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Production Day");

  int value = atoi(outputValues[2].c_str());
  return value;
}

int
TerraSarImageMetadataInterface::GetProductionMonth() const
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

  std::string valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Production Month");

  int value = atoi(outputValues[1].c_str());
  return value;
}

int
TerraSarImageMetadataInterface::GetProductionYear() const
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

  std::string valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Production Year");

  int value = atoi(outputValues[0].c_str());
  return value;
}

double
TerraSarImageMetadataInterface::GetCalibrationFactor() const
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
    double value = atof(valueString.c_str());
    return value;
    }

  return std::numeric_limits<double>::quiet_NaN(); // Invalid value
}

unsigned int
TerraSarImageMetadataInterface::GetNumberOfNoiseRecords() const
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
    std::string valueString = imageKeywordlist.GetMetadataByKey(key);
    unsigned int value = atoi(valueString.c_str());
    return value;
    }

  return std::numeric_limits<unsigned int>::quiet_NaN(); // Invalid value
}

unsigned int
TerraSarImageMetadataInterface::GetNoisePolynomialDegrees(unsigned int noiseRecord) const
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
    std::string valueString = imageKeywordlist.GetMetadataByKey(key);
    unsigned int value = atoi(valueString.c_str());
    return value;
    }

  return std::numeric_limits<unsigned int>::quiet_NaN(); // Invalid value
}


TerraSarImageMetadataInterface::UIntVectorType
TerraSarImageMetadataInterface::GetNoisePolynomialDegrees() const
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
    std::string key = oss.str();
    std::string valueString = imageKeywordlist.GetMetadataByKey(key);
    unsigned int value = atoi(valueString.c_str());
    polDeg.push_back(value);
    }

  return polDeg;
}


TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface
::GetNoisePolynomialCoefficients(unsigned int noiseRecord) const
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

  unsigned int   polDegs = this->GetNoisePolynomialDegrees(noiseRecord);

  DoubleVectorType       polCoef;
  std::ostringstream     oss;

  polCoef.clear();
  // set <= condition because degree N means N+1 coeff
  for (unsigned int j = 0; j <= polDegs; ++j)
    {
    oss.str("");
    oss << "noise[" << noiseRecord << "]imageNoise.noiseEstimate.coefficient[" << j << "]";
    std::string key = oss.str();
    std::string valueString = imageKeywordlist.GetMetadataByKey(key);
    double value = atof(valueString.c_str());
    polCoef.push_back(value);
  }

  return polCoef;
}


TerraSarImageMetadataInterface::DoubleVectorVectorType
TerraSarImageMetadataInterface::GetNoisePolynomialCoefficientsList() const
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

  unsigned int   nbRec = this->GetNumberOfNoiseRecords();
  UIntVectorType polDegs = this->GetNoisePolynomialDegrees();

  DoubleVectorVectorType polCoefList;
  DoubleVectorType       polCoef;
  std::ostringstream     oss;

  for (unsigned int i = 0; i < nbRec; ++i)
    {
    polCoef.clear();
    // set <= condition because degree N means N+1 coeff
    for (unsigned int j = 0; j <= polDegs.size(); ++j)
      {
      oss.str("");
      oss << "noise[" << i << "]imageNoise.noiseEstimate.coefficient[" << j << "]";
      std::string key = oss.str();
      std::string valueString = imageKeywordlist.GetMetadataByKey(key);
      double value = atof(valueString.c_str());
      polCoef.push_back(value);
      }
    polCoefList.push_back(polCoef);
    }

  return polCoefList;
}

double
TerraSarImageMetadataInterface
::GetNoiseTimeUTC(unsigned int noiseRecord) const
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

  DoubleVectorType         timeList;
  std::ostringstream       oss;

  oss.str("");
  oss << "noise[" << noiseRecord << "]imageNoise.timeUTC";
  std::string key = oss.str();
  if (imageKeywordlist.HasKey(key))
    {
    std::string valueString = imageKeywordlist.GetMetadataByKey(key);
    double julianDay = ConvertStringTimeUTCToJulianDay(valueString);
    return julianDay;
    }

  return std::numeric_limits<double>::quiet_NaN(); // Invalid value
}

double
TerraSarImageMetadataInterface
::ConvertStringTimeUTCToJulianDay(const std::string& value) const
{
  std::vector<std::string> splittedDate;
  boost::split(splittedDate, value, boost::is_any_of("-T:Z"));

  int year = atoi(splittedDate[0].c_str());
  int month = atoi(splittedDate[1].c_str());
  int day = atoi(splittedDate[2].c_str());
  int hour = atoi(splittedDate[3].c_str());
  int minu = atoi(splittedDate[4].c_str());
  double sec = atof(splittedDate[5].c_str());

  // Conversion to julian day
  // according to http://en.wikipedia.org/wiki/Julian_day
  // division are integer divisions:
  int a = (14 - month) / 12;
  int y = year + 4800 - a;
  int m = month + 12*a - 3;

  double julianDay = day + (153*m+2)/5 + 365*y + y/4 -y/100 + y/400 - 32045;

  // now, the division are NOT integer
  julianDay += hour/24. + minu/1440. + sec/86400.;

  return julianDay;
}

TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface::GetNoiseTimeUTCList() const
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

  DoubleVectorType         timeList;
  std::ostringstream       oss;

  unsigned int nbRec = this->GetNumberOfNoiseRecords();
  for (unsigned int i = 0; i < nbRec; ++i)
    {
    oss.str("");
    oss << "noise[" << i << "]imageNoise.timeUTC";
    std::string key = oss.str();
    std::string valueString = imageKeywordlist.GetMetadataByKey(key);
    double julianDay = ConvertStringTimeUTCToJulianDay(valueString);
    timeList.push_back(julianDay);
    }

  return timeList;
}

TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface::GetNoiseValidityRangeMaxList() const
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

  DoubleVectorType   maxList;
  std::ostringstream oss;

  unsigned int nbRec = this->GetNumberOfNoiseRecords();
  for (unsigned int i = 0; i < nbRec; ++i)
    {
    oss.str("");
    oss << "noise[" << i << "]imageNoise.noiseEstimate.validityRangeMax";
    std::string valueString = imageKeywordlist.GetMetadataByKey(oss.str());
    maxList.push_back(atof(valueString.c_str()));
    }

  return maxList;
}

TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface::GetNoiseValidityRangeMinList() const
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

  DoubleVectorType   minList;
  std::ostringstream oss;

  unsigned int nbRec = this->GetNumberOfNoiseRecords();
  for (unsigned int i = 0; i < nbRec; ++i)
    {
    oss.str("");
    oss << "noise[" << i << "]imageNoise.noiseEstimate.validityRangeMin";
    std::string valueString = imageKeywordlist.GetMetadataByKey(oss.str());
    minList.push_back(atof(valueString.c_str()));
    }

  return minList;
}


double
TerraSarImageMetadataInterface
::GetNoiseReferencePoint(unsigned int noiseRecord) const
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

  double refPoint;
  std::ostringstream oss;

  oss.str("");
  oss << "noise[" << noiseRecord << "]imageNoise.noiseEstimate.referencePoint";
  std::string valueString = imageKeywordlist.GetMetadataByKey(oss.str());
  refPoint = atof(valueString.c_str());

  return refPoint;
}


TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface::GetNoiseReferencePointList() const
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

  DoubleVectorType   refPointList;
  std::ostringstream oss;

  unsigned int nbRec = this->GetNumberOfNoiseRecords();
  for (unsigned int i = 0; i < nbRec; ++i)
    {
    oss.str("");
    oss << "noise[" << i << "]imageNoise.noiseEstimate.referencePoint";
    std::string valueString = imageKeywordlist.GetMetadataByKey(oss.str());

    refPointList.push_back(atof(valueString.c_str()));
    }

  return refPointList;
}

double
TerraSarImageMetadataInterface::GetRadarFrequency() const
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
  double      freq = atof(valueString.c_str());

  return freq;
}

double
TerraSarImageMetadataInterface::GetPRF() const
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
  double      freq = atof(valueString.c_str());

  return freq;
}

double
TerraSarImageMetadataInterface::GetRSF() const
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
  double      freq = atof(valueString.c_str());

  return freq;
}

unsigned int
TerraSarImageMetadataInterface::GetNumberOfCornerIncidenceAngles() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("sceneCoord.numberOfSceneCornerCoord");
  int      value = atoi(valueString.c_str());
  return value;
}

double
TerraSarImageMetadataInterface::GetMeanIncidenceAngles() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  ImageKeywordlistType imageKeywordlist;

  double sum = 0.;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("sceneCoord.sceneCenterCoord.incidenceAngle");
  double      value = atof(valueString.c_str());

  sum += value;

  unsigned int nbAngles = this->GetNumberOfCornerIncidenceAngles();
  std::ostringstream oss;
  for (unsigned int i = 0; i < nbAngles; ++i)
    {
    oss.str("");
    oss << "sceneCoord.sceneCornerCoord[" << i << "].incidenceAngle";
    valueString = imageKeywordlist.GetMetadataByKey(oss.str());
    value = atof(valueString.c_str());
    sum += value;
    }

  return (sum / (nbAngles + 1));
}

double
TerraSarImageMetadataInterface::GetCenterIncidenceAngle() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("sceneCoord.sceneCenterCoord.incidenceAngle");
  double      value = atof(valueString.c_str());

  return value;
}

TerraSarImageMetadataInterface::IndexType
TerraSarImageMetadataInterface::GetCenterIncidenceAngleIndex() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  TerraSarImageMetadataInterface::IndexType it;

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("sceneCoord.sceneCenterCoord.refRow");
  it[1] = atoi(valueString.c_str());

  valueString = imageKeywordlist.GetMetadataByKey("sceneCoord.sceneCenterCoord.refColumn");
  it[0] = atoi(valueString.c_str());

  return it;
}

TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface::GetCornersIncidenceAngles() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  ImageKeywordlistType imageKeywordlist;

  TerraSarImageMetadataInterface::DoubleVectorType dv;

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

    dv.push_back(atof(valueString.c_str()));
    }

  return dv;
}

TerraSarImageMetadataInterface::IndexVectorType
TerraSarImageMetadataInterface::GetCornersIncidenceAnglesIndex() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  ImageKeywordlistType imageKeywordlist;

  TerraSarImageMetadataInterface::IndexVectorType iv;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }


  std::ostringstream oss;
  std::ostringstream oss2;

  unsigned int nbAngles = this->GetNumberOfCornerIncidenceAngles();
  for (unsigned int i = 0; i < nbAngles; ++i)
    {
    TerraSarImageMetadataInterface::IndexType it;

    oss.str("");
    oss << "sceneCoord.sceneCornerCoord[" << i << "].refRow";

    std::string valueString = imageKeywordlist.GetMetadataByKey(oss.str());
    it[1] = atoi(valueString.c_str());

    oss2.str("");
    oss2 << "sceneCoord.sceneCornerCoord[" << i << "].refColumn";

    valueString = imageKeywordlist.GetMetadataByKey(oss2.str());
    it[0] = atoi(valueString.c_str());

    iv.push_back(it);
    }

  return iv;
}


double
TerraSarImageMetadataInterface
::Horner(std::vector<double>& coefficients, const double tauMinusTauRef) const
{
  std::vector<double>::reverse_iterator coefIt = coefficients.rbegin();
  double res = *(coefIt);
  ++coefIt;

  while (coefIt < coefficients.rend())
    {
    // Cumulate polynomial
    res = res * tauMinusTauRef + (*coefIt);
    ++coefIt;
    }
  return res;
}


TerraSarImageMetadataInterface::PointSetPointer
TerraSarImageMetadataInterface
::GetRadiometricCalibrationNoise() const
{
  PointSetPointer points = PointSetType::New();

  IndexVectorType cornerIndex = this->GetCornersIncidenceAnglesIndex();
  unsigned int numberOfRows = 0;
  unsigned int numberOfCols = 0;

  for (unsigned int i = 0; i < cornerIndex.size(); ++i)
    {
    IndexType index;
    index = cornerIndex[i];
    unsigned int noRow = index[0];
    unsigned int noCol = index[1];
    if (noRow > numberOfRows)
      {
      numberOfRows = noRow;
      }
    if (noCol > numberOfCols)
      {
      numberOfCols = noCol;
      }
    }

  double startTime = this->GetStartTimeUTC();
  double stopTime = this->GetStopTimeUTC();
  RealType firstRangeTime = this->GetRangeTimeFirstPixel();
  RealType lastRangeTime = this->GetRangeTimeLastPixel();

  points->Initialize();
  unsigned int noPoint = 0;

  PointType p0;

  unsigned int numberOfNoiseRecords = this->GetNumberOfNoiseRecords();

  for (unsigned int noiseRecord = 0; noiseRecord < numberOfNoiseRecords; ++noiseRecord)
    {
    double currentNoiseTime = this->GetNoiseTimeUTC(noiseRecord);
    RealType AzimutAcquisition = (currentNoiseTime - startTime) * numberOfRows / (stopTime - startTime);
    RealType referencePointTime = this->GetNoiseReferencePoint(noiseRecord);

    std::vector<RealType> polynomialCoefficient;
    polynomialCoefficient = this->GetNoisePolynomialCoefficients(noiseRecord);

    p0[0] = AzimutAcquisition;

    for (unsigned int col = 0; col < numberOfCols; ++col)
      {
      RealType rangeTime = col * (lastRangeTime - firstRangeTime) / (numberOfCols) + firstRangeTime;
      RealType tauMinusTauRef = rangeTime - referencePointTime;
      RealType value = this->Horner(polynomialCoefficient, tauMinusTauRef);

      p0[1] = col;
      points->SetPoint(noPoint, p0);
      points->SetPointData(noPoint, value);
      ++noPoint;

      }
    }
  return points;
}

TerraSarImageMetadataInterface::IndexType
TerraSarImageMetadataInterface
::GetRadiometricCalibrationNoisePolynomialDegree() const
{
  IndexType polynomSize;
  polynomSize[0] = 2;
  polynomSize[1] = 2;

  return polynomSize;
}

double
TerraSarImageMetadataInterface
::GetStartTimeUTC() const
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

  std::string referenceUTC = imageKeywordlist.GetMetadataByKey("azimuth_start_time");
  double julianDay = ConvertStringTimeUTCToJulianDay(referenceUTC);

  return julianDay;
}

double
TerraSarImageMetadataInterface
::GetStopTimeUTC() const
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

  std::string referenceUTC = imageKeywordlist.GetMetadataByKey("azimuth_stop_time");
  double julianDay = ConvertStringTimeUTCToJulianDay(referenceUTC);

  return julianDay;
}

TerraSarImageMetadataInterface::RealType
TerraSarImageMetadataInterface
::GetRangeTimeFirstPixel() const
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

  std::string valueString = imageKeywordlist.GetMetadataByKey("range_first_time");
  double value = atof(valueString.c_str());
  return value;
}

TerraSarImageMetadataInterface::RealType
TerraSarImageMetadataInterface
::GetRangeTimeLastPixel() const
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

  std::string valueString = imageKeywordlist.GetMetadataByKey("range_last_time");
  double value = atof(valueString.c_str());
  return value;
}

TerraSarImageMetadataInterface::RealType
TerraSarImageMetadataInterface
::GetRadiometricCalibrationScale() const
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

  std::string valueString = imageKeywordlist.GetMetadataByKey("calibration.calibrationConstant.calFactor");
  double value = atof(valueString.c_str());
  return value;
}

TerraSarImageMetadataInterface::PointSetPointer
TerraSarImageMetadataInterface
::GetRadiometricCalibrationIncidenceAngle() const
{
  PointSetPointer points = PointSetType::New();
  PointType p0;

  double centerIncidenceAngleValue = this->GetCenterIncidenceAngle();
  IndexType centerIncidenceAngleIndex = this->GetCenterIncidenceAngleIndex();

  DoubleVectorType cornerIncidenceAngleValue = this->GetCornersIncidenceAngles();
  IndexVectorType cornerIncidenceAngleIndex = this->GetCornersIncidenceAnglesIndex();
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

TerraSarImageMetadataInterface::IndexType
TerraSarImageMetadataInterface
::GetRadiometricCalibrationIncidenceAnglePolynomialDegree() const
{
  IndexType polynomSize;
  polynomSize[0] = 2;
  polynomSize[1] = 1;

  return polynomSize;
}


  /** Get the R, G, B channel */
std::vector<unsigned int>
TerraSarImageMetadataInterface
::GetDefaultDisplay() const
{
  std::vector<unsigned int> rgb(3);
  rgb[0] = 0;
  rgb[1] = 0;
  rgb[2] = 0;
  return rgb;
}

void
TerraSarImageMetadataInterface
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


} // end namespace otb
