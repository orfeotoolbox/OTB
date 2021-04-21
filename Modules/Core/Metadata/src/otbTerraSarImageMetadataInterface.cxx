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


#include "otbTerraSarImageMetadataInterface.h"

#include "otbStringUtils.h"
#include "otbMath.h"
#include "itkIndex.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"
#include "otbXMLMetadataSupplier.h"
#include "otbSARMetadata.h"


#include <boost/filesystem.hpp>
//TODO C++ 17 : use std::optional instead
#include <boost/optional.hpp>

#include "itksys/SystemTools.hxx"
#include "itksys/RegularExpression.hxx"




namespace otb
{

TerraSarImageMetadataInterface::TerraSarImageMetadataInterface()
{
}



namespace ExtractXMLFiles {

// Adapted from boost filesystem documentation : https://www.boost.org/doc/libs/1_53_0/libs/filesystem/doc/reference.html
std::vector<std::string> GetXMLFilesInDirectory(const std::string & directoryPath)
{
  std::vector<std::string> fileList;
  if ( !boost::filesystem::exists( directoryPath) )
  {
    return fileList;
  }
  else if (!boost::filesystem::is_directory(directoryPath))
  {
    fileList.push_back(directoryPath);
    return fileList;
  }
  // End iterator : default construction yields past-the-end
  for ( const auto & item : boost::make_iterator_range(boost::filesystem::directory_iterator(directoryPath), {}) )
  {
    if ( boost::filesystem::is_directory(item.status()) )
    {
      auto subDirList = GetXMLFilesInDirectory(item.path().string());
      fileList.insert(fileList.end(), subDirList.begin(), subDirList.end());
    }
    else
    {
      //Extract only XML files
      std::string extension = boost::filesystem::extension(item);
      if (extension ==".xml")
        fileList.push_back(item.path().string());
    }
  }

  return fileList;
}



std::string GetResourceFile(const std::string & directoryPath, std::string pattern)
  {
  auto xmlFiles = GetXMLFilesInDirectory(directoryPath);
  itksys::RegularExpression reg;
  reg.compile(pattern);
  for (const auto & file : xmlFiles)
    {
    if (reg.find(boost::filesystem::path(file).filename().string()))
      return file ;
    }
  return std::string("");
  }


}



bool TerraSarImageMetadataInterface::CanRead() const
{
  std::string sensorID = GetSensorID();
  if (sensorID.find("TSX") != std::string::npos)
    return true;
  else
    return false;
}

int TerraSarImageMetadataInterface::GetDay() const
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

  std::string              valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2)
    itkExceptionMacro(<< "Invalid Day");

  int value = atoi(outputValues[2].c_str());
  return value;
}

int TerraSarImageMetadataInterface::GetMonth() const
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

  std::string              valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2)
    itkExceptionMacro(<< "Invalid Month");

  int value = atoi(outputValues[1].c_str());
  return value;
}

int TerraSarImageMetadataInterface::GetYear() const
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

  std::string              valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2)
    itkExceptionMacro(<< "Invalid Year");

  int value = atoi(outputValues[0].c_str());
  return value;
}

int TerraSarImageMetadataInterface::GetHour() const
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

  std::string              valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 4)
    itkExceptionMacro(<< "Invalid Hour");

  int value = atoi(outputValues[3].c_str());
  return value;
}

int TerraSarImageMetadataInterface::GetMinute() const
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

  std::string              valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 4)
    itkExceptionMacro(<< "Invalid Minute");

  int value = atoi(outputValues[4].c_str());
  return value;
}

int TerraSarImageMetadataInterface::GetProductionDay() const
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

  int value = atoi(outputValues[2].c_str());
  return value;
}

int TerraSarImageMetadataInterface::GetProductionMonth() const
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

  int value = atoi(outputValues[1].c_str());
  return value;
}

int TerraSarImageMetadataInterface::GetProductionYear() const
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

  int value = atoi(outputValues[0].c_str());
  return value;
}

double TerraSarImageMetadataInterface::GetCalibrationFactor() const
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
    double      value       = atof(valueString.c_str());
    return value;
  }

  return std::numeric_limits<double>::quiet_NaN(); // Invalid value
}

unsigned int TerraSarImageMetadataInterface::GetNumberOfNoiseRecords(const MetadataSupplierInterface& mds) const
{
  auto ret = mds.GetAs<unsigned int>(std::numeric_limits<unsigned int>::quiet_NaN(),
                                     "level1Product.noise.numberOfNoiseRecords");
  if(ret == std::numeric_limits<unsigned int>::quiet_NaN())
    ret = mds.GetAs<unsigned int>(std::numeric_limits<unsigned int>::quiet_NaN(),
                                  "noise.numberOfNoiseRecords");
  return ret;
}

unsigned int TerraSarImageMetadataInterface::GetNoisePolynomialDegrees(const unsigned int noiseRecord,
                                                                       const MetadataSupplierInterface& mds) const
{
  std::ostringstream oss;
  oss << "level1Product.noise.imageNoise_" << (noiseRecord+1) << ".noiseEstimate.polynomialDegree";
  auto ret = mds.GetAs<unsigned int>(std::numeric_limits<unsigned int>::quiet_NaN(), oss.str());
  if(ret != std::numeric_limits<unsigned int>::quiet_NaN())
    return ret;
  oss.str("");
  oss << "noise.imageNoise_" << noiseRecord << ".noiseEstimate.polynomialDegree";
  ret = mds.GetAs<unsigned int>(std::numeric_limits<unsigned int>::quiet_NaN(), oss.str());
  if(ret != std::numeric_limits<unsigned int>::quiet_NaN())
    return ret;

  return std::numeric_limits<unsigned int>::quiet_NaN(); // Invalid value
}


TerraSarImageMetadataInterface::UIntVectorType
TerraSarImageMetadataInterface::GetNoisePolynomialDegrees(const MetadataSupplierInterface& mds) const
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

  unsigned int nbRec = this->GetNumberOfNoiseRecords(mds);
  for (unsigned int i = 0; i < nbRec; ++i)
  {
    oss.str("");
    oss << "noise[" << i << "]imageNoise.noiseEstimate.polynomialDegree";
    std::string  key         = oss.str();
    std::string  valueString = imageKeywordlist.GetMetadataByKey(key);
    unsigned int value       = atoi(valueString.c_str());
    polDeg.push_back(value);
  }

  return polDeg;
}


TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface::GetNoisePolynomialCoefficients(const unsigned int noiseRecord,
                                                               const MetadataSupplierInterface& mds) const
{
  unsigned int polDegs = this->GetNoisePolynomialDegrees(noiseRecord, mds);

  DoubleVectorType   polCoef;
  std::ostringstream oss;

  polCoef.clear();
  // set <= condition because degree N means N+1 coeff
  for (unsigned int j = 0; j <= polDegs; ++j)
  {
    oss.str("");
    oss << "level1Product.noise.imageNoise_" << (noiseRecord+1) << ".noiseEstimate.coefficient_" << (j+1);
    auto value = mds.GetAs<double>(std::numeric_limits<double>::min(), oss.str());
    if(value == std::numeric_limits<double>::min())
    {
      oss.str("");
      oss << "noise.imageNoise_" << noiseRecord << ".noiseEstimate.coefficient_" << j;
      value = mds.GetAs<double>(oss.str());
    }
    polCoef.push_back(value);
  }

  return polCoef;
}


TerraSarImageMetadataInterface::DoubleVectorVectorType
TerraSarImageMetadataInterface::GetNoisePolynomialCoefficientsList(const MetadataSupplierInterface& mds) const
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

  unsigned int   nbRec   = this->GetNumberOfNoiseRecords(mds);
  UIntVectorType polDegs = this->GetNoisePolynomialDegrees(mds);

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
      std::string key         = oss.str();
      std::string valueString = imageKeywordlist.GetMetadataByKey(key);
      double      value       = atof(valueString.c_str());
      polCoef.push_back(value);
    }
    polCoefList.push_back(polCoef);
  }

  return polCoefList;
}

double TerraSarImageMetadataInterface::GetNoiseTimeUTC(const unsigned int noiseRecord,
                                                       const MetadataSupplierInterface& mds) const
{
  MetaData::Time time, defaultTime;
  std::istringstream("2000-01-01T00:0:00.000000Z") >> defaultTime;
  std::ostringstream oss;
  oss << "level1Product.noise.imageNoise_" << noiseRecord << ".timeUTC";
  std::istringstream(mds.GetAs<std::string>("2000-01-01T00:0:00.000000Z", oss.str())) >> time;
  if(time != defaultTime)
    return time.GetJulianDay();
  oss.str("");
  oss << "noise.imageNoise_" << noiseRecord << ".timeUTC";
  std::istringstream(mds.GetAs<std::string>("2000-01-01T00:0:00.000000Z", oss.str())) >> time;
  if(time != defaultTime)
    return time.GetJulianDay();
  return std::numeric_limits<double>::quiet_NaN(); // Invalid value
}

double TerraSarImageMetadataInterface::ConvertStringTimeUTCToJulianDay(const std::string& value) const
{
  std::vector<std::string> splitDate;
  boost::split(splitDate, value, boost::is_any_of("-T:Z"));

  int    year  = atoi(splitDate[0].c_str());
  int    month = atoi(splitDate[1].c_str());
  int    day   = atoi(splitDate[2].c_str());
  int    hour  = atoi(splitDate[3].c_str());
  int    minu  = atoi(splitDate[4].c_str());
  double sec   = atof(splitDate[5].c_str());

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

TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface::GetNoiseTimeUTCList(const MetadataSupplierInterface& mds) const
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

  unsigned int nbRec = this->GetNumberOfNoiseRecords(mds);
  for (unsigned int i = 0; i < nbRec; ++i)
  {
    oss.str("");
    oss << "noise[" << i << "]imageNoise.timeUTC";
    std::string key         = oss.str();
    std::string valueString = imageKeywordlist.GetMetadataByKey(key);
    double      julianDay   = ConvertStringTimeUTCToJulianDay(valueString);
    timeList.push_back(julianDay);
  }

  return timeList;
}

TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface::GetNoiseValidityRangeMaxList(const MetadataSupplierInterface& mds) const
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

  unsigned int nbRec = this->GetNumberOfNoiseRecords(mds);
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
TerraSarImageMetadataInterface::GetNoiseValidityRangeMinList(const MetadataSupplierInterface& mds) const
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

  unsigned int nbRec = this->GetNumberOfNoiseRecords(mds);
  for (unsigned int i = 0; i < nbRec; ++i)
  {
    oss.str("");
    oss << "noise[" << i << "]imageNoise.noiseEstimate.validityRangeMin";
    std::string valueString = imageKeywordlist.GetMetadataByKey(oss.str());
    minList.push_back(atof(valueString.c_str()));
  }

  return minList;
}


double TerraSarImageMetadataInterface::GetNoiseReferencePoint(const unsigned int noiseRecord,
                                                              const MetadataSupplierInterface& mds) const
{
  std::ostringstream oss;
  oss << "level1Product.noise.imageNoise_" << noiseRecord << ".noiseEstimate.referencePoint";
  auto ret = mds.GetAs<double>(std::numeric_limits<double>::min(), oss.str());
  if(ret != std::numeric_limits<double>::min())
    return ret;
  oss.str("");
  oss << "noise.imageNoise_" << noiseRecord << ".noiseEstimate.referencePoint";
  ret = mds.GetAs<double>(std::numeric_limits<double>::min(), oss.str());
  if(ret != std::numeric_limits<double>::min())
    return ret;
  return std::numeric_limits<double>::quiet_NaN();
}


TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface::GetNoiseReferencePointList(const MetadataSupplierInterface& mds) const
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

  unsigned int nbRec = this->GetNumberOfNoiseRecords(mds);
  for (unsigned int i = 0; i < nbRec; ++i)
  {
    oss.str("");
    oss << "noise[" << i << "]imageNoise.noiseEstimate.referencePoint";
    std::string valueString = imageKeywordlist.GetMetadataByKey(oss.str());

    refPointList.push_back(atof(valueString.c_str()));
  }

  return refPointList;
}

double TerraSarImageMetadataInterface::GetRadarFrequency() const
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
  double      freq        = atof(valueString.c_str());

  return freq;
}

double TerraSarImageMetadataInterface::GetPRF() const
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
  double      freq        = atof(valueString.c_str());

  return freq;
}

double TerraSarImageMetadataInterface::GetRSF() const
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
  double      freq        = atof(valueString.c_str());

  return freq;
}

unsigned int TerraSarImageMetadataInterface::GetNumberOfCornerIncidenceAngles(const MetadataSupplierInterface& mds) const
{
  auto ret = mds.GetAs<unsigned int>(0, "sceneCoord.numberOfSceneCornerCoord");
  if (ret > 0)
    return ret;
  return mds.GetNumberOf("level1Product.productInfo.sceneInfo.sceneCornerCoord");;
}

double TerraSarImageMetadataInterface::GetMeanIncidenceAngles(const MetadataSupplierInterface& mds) const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  ImageKeywordlistType imageKeywordlist;

  double sum = 0.;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string valueString = imageKeywordlist.GetMetadataByKey("sceneCoord.sceneCenterCoord.incidenceAngle");
  double      value       = atof(valueString.c_str());

  sum += value;

  unsigned int       nbAngles = this->GetNumberOfCornerIncidenceAngles(mds);
  std::ostringstream oss;
  for (unsigned int i = 0; i < nbAngles; ++i)
  {
    oss.str("");
    oss << "sceneCoord.sceneCornerCoord[" << i << "].incidenceAngle";
    valueString = imageKeywordlist.GetMetadataByKey(oss.str());
    value       = atof(valueString.c_str());
    sum += value;
  }

  return (sum / (nbAngles + 1));
}

double TerraSarImageMetadataInterface::GetCenterIncidenceAngle(const MetadataSupplierInterface& mds) const
{
  auto ret = mds.GetAs<double>(std::numeric_limits<double>::min(), "level1Product.productInfo.sceneInfo.sceneCenterCoord.incidenceAngle");
  if(ret != std::numeric_limits<double>::min())
    return ret;
  ret = mds.GetAs<double>(std::numeric_limits<double>::min(), "sceneCoord.sceneCenterCoord.incidenceAngle");
  return ret;
}

TerraSarImageMetadataInterface::IndexType
TerraSarImageMetadataInterface::GetCenterIncidenceAngleIndex(const MetadataSupplierInterface& mds) const
{
  TerraSarImageMetadataInterface::IndexType it;

  auto value= mds.GetAs<int>(std::numeric_limits<int>::min(), "level1Product.productInfo.sceneInfo.sceneCenterCoord.refRow");
  if(value == std::numeric_limits<int>::min())
    value= mds.GetAs<int>("sceneCoord.sceneCenterCoord.refRow");
  it[1] = value;

  value= mds.GetAs<int>(std::numeric_limits<int>::min(), "level1Product.productInfo.sceneInfo.sceneCenterCoord.refColumn");
  if(value == std::numeric_limits<int>::min())
    value= mds.GetAs<int>("sceneCoord.sceneCenterCoord.refColumn");
  it[0] = value;

  return it;
}

TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface::GetCornersIncidenceAngles(const MetadataSupplierInterface& mds) const
{
  TerraSarImageMetadataInterface::DoubleVectorType dv;
  std::ostringstream oss;
  unsigned int nbAngles = this->GetNumberOfCornerIncidenceAngles(mds);
  for (unsigned int i = 0; i < nbAngles; ++i)
  {
    oss.str("");
    oss << "level1Product.productInfo.sceneInfo.sceneCornerCoord_" << (i+1) << ".incidenceAngle";
    auto value = mds.GetAs<double>(std::numeric_limits<double>::min(), oss.str());
    if(value == std::numeric_limits<double>::min())
    {
      oss.str("");
      oss << "sceneCoord.sceneCornerCoord[" << i << "].incidenceAngle";
      value = mds.GetAs<double>(oss.str());
    }
    dv.push_back(value);
  }

  return dv;
}

TerraSarImageMetadataInterface::IndexVectorType
TerraSarImageMetadataInterface::GetCornersIncidenceAnglesIndex(const MetadataSupplierInterface & imd) const
{
  TerraSarImageMetadataInterface::IndexVectorType iv;
  std::ostringstream oss;

  unsigned int nbAngles = this->GetNumberOfCornerIncidenceAngles(imd);
  for (unsigned int i = 0; i < nbAngles; ++i)
  {
    TerraSarImageMetadataInterface::IndexType it;

    oss.str("");
    oss << "level1Product.productInfo.sceneInfo.sceneCornerCoord_" << (i+1) << ".refRow";
    it[1] = imd.GetAs<int>(std::numeric_limits<int>::min(), oss.str());
    if(it[1] == std::numeric_limits<int>::min())
    {
      oss.str("");
      oss << "sceneCoord.sceneCornerCoord[" << i << "].refRow";
      it[1] = imd.GetAs<int>(oss.str());
    }

    oss.str("");
    oss << "level1Product.productInfo.sceneInfo.sceneCornerCoord_" << (i+1) << ".refColumn";
    it[0] = imd.GetAs<int>(std::numeric_limits<int>::min(), oss.str());
    if(it[0] == std::numeric_limits<int>::min())
    {
      oss.str("");
      oss << "sceneCoord.sceneCornerCoord[" << i << "].refColumn";
      it[0] = imd.GetAs<int>(oss.str());
    }

    iv.push_back(it);
  }
  return iv;
}


double TerraSarImageMetadataInterface::Horner(std::vector<double>& coefficients, const double tauMinusTauRef) const
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


TerraSarImageMetadataInterface::PointSetPointer
TerraSarImageMetadataInterface::GetRadiometricCalibrationNoise(const MetadataSupplierInterface& mds, const ImageMetadata& imd) const
{
  PointSetPointer points = PointSetType::New();

  IndexVectorType cornerIndex  = this->GetCornersIncidenceAnglesIndex(mds);
  unsigned int    numberOfRows = 0;
  unsigned int    numberOfCols = 0;

  for (unsigned int i = 0; i < cornerIndex.size(); ++i)
  {
    IndexType index;
    index              = cornerIndex[i];
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

  double   startTime      = imd[MDTime::AcquisitionStartTime].GetJulianDay();
  double   stopTime       = imd[MDTime::AcquisitionStopTime].GetJulianDay();
  RealType firstRangeTime = imd[MDNum::RangeTimeFirstPixel];
  RealType lastRangeTime  = imd[MDNum::RangeTimeLastPixel];

  points->Initialize();
  unsigned int noPoint = 0;

  PointType p0;

  unsigned int numberOfNoiseRecords = this->GetNumberOfNoiseRecords(mds);

  for (unsigned int noiseRecord = 0; noiseRecord < numberOfNoiseRecords; ++noiseRecord)
  {
    double   currentNoiseTime   = this->GetNoiseTimeUTC(noiseRecord, mds);
    RealType AzimutAcquisition  = (currentNoiseTime - startTime) * numberOfRows / (stopTime - startTime);
    RealType referencePointTime = this->GetNoiseReferencePoint(noiseRecord, mds);

    std::vector<RealType> polynomialCoefficient;
    polynomialCoefficient = this->GetNoisePolynomialCoefficients(noiseRecord, mds);

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

SarImageMetadataInterface::ArrayIndexType TerraSarImageMetadataInterface::GetRadiometricCalibrationNoisePolynomialDegree() const
{
  return {2, 2};
}

TerraSarImageMetadataInterface::RealType TerraSarImageMetadataInterface::GetRadiometricCalibrationScale() const
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
  double      value       = atof(valueString.c_str());
  return value;
}

TerraSarImageMetadataInterface::PointSetPointer
TerraSarImageMetadataInterface::GetRadiometricCalibrationIncidenceAngle(const MetadataSupplierInterface& imd) const
{
  PointSetPointer points = PointSetType::New();
  PointType       p0;

  double    centerIncidenceAngleValue = this->GetCenterIncidenceAngle(imd);
  IndexType centerIncidenceAngleIndex = this->GetCenterIncidenceAngleIndex(imd);

  DoubleVectorType cornerIncidenceAngleValue = this->GetCornersIncidenceAngles(imd);
  IndexVectorType  cornerIncidenceAngleIndex = this->GetCornersIncidenceAnglesIndex(imd);
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

TerraSarImageMetadataInterface::ArrayIndexType TerraSarImageMetadataInterface::GetRadiometricCalibrationIncidenceAnglePolynomialDegree() const
{
  return {2, 1};
}


/** Get the R, G, B channel */
std::vector<unsigned int> TerraSarImageMetadataInterface::GetDefaultDisplay() const
{
  std::vector<unsigned int> rgb(3);
  rgb[0] = 0;
  rgb[1] = 0;
  rgb[2] = 0;
  return rgb;
}

void TerraSarImageMetadataInterface::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

void TerraSarImageMetadataInterface::ParseGdal(ImageMetadata &imd)
{
  // Metadata read by GDAL
  Fetch(MDStr::Mode, imd, "IMAGING_MODE");
  Fetch(MDStr::OrbitDirection, imd, "ORBIT_DIRECTION");
  Fetch(MDNum::OrbitNumber, imd, "ABSOLUTE_ORBIT");

  // Main XML file
  std::string MainFilePath = ExtractXMLFiles::GetResourceFile(itksys::SystemTools::GetFilenamePath(m_MetadataSupplierInterface->GetResourceFile("")), "T[S|D]X1_SAR__.*.xml") ;
  if (!MainFilePath.empty())
  {
    XMLMetadataSupplier MainXMLFileMetadataSupplier(MainFilePath);
    imd.Add(MDNum::LineSpacing, MainXMLFileMetadataSupplier.GetAs<double>("level1Product.productSpecific.complexImageInfo.projectedSpacingAzimuth"));
    imd.Add(MDNum::PixelSpacing, MainXMLFileMetadataSupplier.GetAs<double>("level1Product.productSpecific.complexImageInfo.projectedSpacingRange.slantRange"));
    imd.Add(MDStr::Mission, MainXMLFileMetadataSupplier.GetAs<std::string>("level1Product.generalHeader.mission"));
    imd.Add(MDStr::ProductType, MainXMLFileMetadataSupplier.GetAs<std::string>("level1Product.productInfo.productVariantInfo.productType"));
    // imd.Add(MDStr::Mode, MainXMLFileMS.GetAs<std::string>("level1Product.productInfo.acquisitionInfo.imagingMode"));
    imd.Add(MDStr::SensorID, MainXMLFileMetadataSupplier.GetAs<std::string>("level1Product.productInfo.acquisitionInfo.sensor"));
    imd.Add(MDNum::RadarFrequency, MainXMLFileMetadataSupplier.GetAs<double>("level1Product.instrument.radarParameters.centerFrequency"));
    imd.Add(MDTime::AcquisitionStartTime, MainXMLFileMetadataSupplier.GetFirstAs<MetaData::Time>("level1Product.productInfo.sceneInfo.start.timeUTC"));
    imd.Add(MDTime::AcquisitionStopTime, MainXMLFileMetadataSupplier.GetFirstAs<MetaData::Time>("level1Product.productInfo.sceneInfo.stop.timeUTC"));
    imd.Add(MDNum::RangeTimeFirstPixel, MainXMLFileMetadataSupplier.GetFirstAs<double>("level1Product.productInfo.sceneInfo.rangeTime.firstPixel"));
    imd.Add(MDNum::RangeTimeLastPixel, MainXMLFileMetadataSupplier.GetFirstAs<double>("level1Product.productInfo.sceneInfo.rangeTime.lastPixel"));
    imd.Add(MDNum::PRF, MainXMLFileMetadataSupplier.GetAs<double>("level1Product.productSpecific.complexImageInfo.commonPRF"));
    imd.Add(MDNum::RSF, MainXMLFileMetadataSupplier.GetAs<double>("level1Product.productSpecific.complexImageInfo.commonRSF"));
    imd.Add(MDNum::CalFactor, MainXMLFileMetadataSupplier.GetAs<double>("level1Product.calibration.calibrationConstant.calFactor"));
    SARCalib sarCalib;
    LoadRadiometricCalibrationData(sarCalib, MainXMLFileMetadataSupplier, imd);
    imd.Add(MDGeom::SARCalib, sarCalib);
  }

  assert(m_MetadataSupplierInterface->GetNbBands() == imd.Bands.size());

  SARParam sarParam;
  for (int bandId = 0 ; bandId < m_MetadataSupplierInterface->GetNbBands() ; ++bandId)
  {
    Fetch(MDStr::Polarization, imd, "POLARIMETRIC_INTERP", bandId);
    imd.Bands[bandId].Add(MDGeom::SAR, sarParam);
  }
}

void TerraSarImageMetadataInterface::ParseGeom(ImageMetadata & imd)
{
  Fetch(MDNum::LineSpacing, imd, "meters_per_pixel_y");
  Fetch(MDNum::PixelSpacing, imd, "meters_per_pixel_x");
  Fetch(MDStr::Mode, imd, "acquisitionInfo.imagingMode");
  Fetch(MDStr::Mission, imd, "sensor");
  Fetch(MDStr::ProductType, imd, "product_type");
  Fetch(MDStr::SensorID, imd, "acquisitionInfo.sensor");
  Fetch(MDTime::AcquisitionStartTime, imd, "azimuth_start_time");
  Fetch(MDTime::AcquisitionStopTime, imd, "azimuth_stop_time");
  Fetch(MDNum::RangeTimeFirstPixel, imd, "range_first_time");
  Fetch(MDNum::RangeTimeLastPixel, imd, "range_last_time");
  Fetch(MDNum::PRF, imd, "sensor_params.prf");
  Fetch(MDNum::CalFactor, imd, "calibration.calibrationConstant.calFactor");
    
  // Main XML file
  std::string MainFilePath = ExtractXMLFiles::GetResourceFile(itksys::SystemTools::GetFilenamePath(m_MetadataSupplierInterface->GetResourceFile("")), "T[S|D]X1_SAR__.*.xml") ;
  if (!MainFilePath.empty())
  {
    XMLMetadataSupplier MainXMLFileMS(MainFilePath);
    imd.Add(MDNum::RadarFrequency, MainXMLFileMS.GetAs<double>("level1Product.instrument.radarParameters.centerFrequency"));
    imd.Add(MDStr::OrbitDirection, MainXMLFileMS.GetAs<std::string>("level1Product.productInfo.missionInfo.orbitDirection"));
    imd.Add(MDNum::OrbitNumber, MainXMLFileMS.GetAs<double>("level1Product.productInfo.missionInfo.absOrbit"));
    imd.Add(MDNum::RSF, MainXMLFileMS.GetAs<double>("level1Product.productSpecific.complexImageInfo.commonRSF"));
  }

  assert(m_MetadataSupplierInterface->GetNbBands() == imd.Bands.size());

  SARParam sarParam;
  for (int bandId = 0 ; bandId < m_MetadataSupplierInterface->GetNbBands() ; ++bandId)
  {
    Fetch(MDStr::Polarization, imd, ("acquisitionInfo.polarisationList[" + std::to_string(bandId) + "]").c_str(), bandId);
    imd.Bands[bandId].Add(MDGeom::SAR, sarParam);
  }
  SARCalib sarCalib;
  LoadRadiometricCalibrationData(sarCalib, *m_MetadataSupplierInterface, imd);
  imd.Add(MDGeom::SARCalib, sarCalib);
}

void TerraSarImageMetadataInterface::Parse(ImageMetadata & imd)
{
  // Try to fetch the metadata from GDAL Metadata Supplier
  if (boost::filesystem::path(m_MetadataSupplierInterface->GetResourceFile()).filename().string().rfind("TSX1_", 0) == 0)
    this->ParseGdal(imd);
  // Try to fetch the metadata from GEOM file
  else if (m_MetadataSupplierInterface->GetAs<std::string>("", "sensor") == "TSX-1")
    this->ParseGeom(imd);
  // Failed to fetch the metadata
  else
    otbGenericExceptionMacro(MissingMetadataException,
			     << "Not a Sentinel1 product");
}


} // end namespace otb
