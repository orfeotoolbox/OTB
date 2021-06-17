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
#include "otbSpatialReference.h"

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
std::vector<std::string> GetXMLFilesInDirectory(const std::string & directoryPath,
                                                bool getSubDirectories=true)
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
    if ( boost::filesystem::is_directory(item.status()) && getSubDirectories )
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

std::string GetResourceFile(const std::string & directoryPath, std::string pattern, bool getSubDirectories=true)
{
  auto xmlFiles = GetXMLFilesInDirectory(directoryPath, getSubDirectories);
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

unsigned int TerraSarImageMetadataInterface::GetNumberOfNoiseRecords(const MetadataSupplierInterface& mds, const unsigned int polLayer) const
{
  if(polLayer == 0)
  {
    if(mds.HasValue("level1Product.noise.numberOfNoiseRecords"))
      return mds.GetAs<unsigned int>(std::numeric_limits<unsigned int>::quiet_NaN(), "level1Product.noise.numberOfNoiseRecords");
    else
      return mds.GetAs<unsigned int>(std::numeric_limits<unsigned int>::quiet_NaN(), "noise.numberOfNoiseRecords");
  }
  else
  {
    std::stringstream oss;
    oss << "level1Product.noise_" << polLayer << ".numberOfNoiseRecords";
    return mds.GetAs<unsigned int>(std::numeric_limits<unsigned int>::quiet_NaN(), oss.str());
  }
}

unsigned int TerraSarImageMetadataInterface::GetNoisePolynomialDegrees(const unsigned int noiseRecord,
                                                                       const MetadataSupplierInterface& mds,
                                                                       const unsigned int polLayer) const
{
  std::ostringstream oss;
  if(polLayer == 0)
  {
    oss << "level1Product.noise.imageNoise_" << noiseRecord << ".noiseEstimate.polynomialDegree";
    if(mds.HasValue(oss.str()))
      return mds.GetAs<unsigned int>(oss.str());
    oss.str("");
    oss << "noise[" << (noiseRecord-1) << "]imageNoise.noiseEstimate.polynomialDegree";
    if(mds.HasValue(oss.str()))
      return mds.GetAs<unsigned int>(oss.str());
  }
  else
  {
    oss << "level1Product.noise_" << polLayer << ".imageNoise_" << noiseRecord << ".noiseEstimate.polynomialDegree";
    if(mds.HasValue(oss.str()))
      return mds.GetAs<unsigned int>(oss.str());
  }
  return std::numeric_limits<unsigned int>::quiet_NaN(); // Invalid value
}

TerraSarImageMetadataInterface::UIntVectorType
TerraSarImageMetadataInterface::GetNoisePolynomialDegrees(const MetadataSupplierInterface& mds,
                                                          const unsigned int polLayer) const
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

  unsigned int nbRec = this->GetNumberOfNoiseRecords(mds, polLayer);
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
                                                               const MetadataSupplierInterface& mds,
                                                               const unsigned int polLayer) const
{
  unsigned int polDegs = this->GetNoisePolynomialDegrees(noiseRecord, mds, polLayer);

  DoubleVectorType   polCoef;
  polCoef.clear();

  std::ostringstream oss, oss2 , oss3;
  int shift = 0;
  if(polLayer == 0)
  {
    if (mds.HasValue("level1Product.noise.polLayer"))
    {
      oss << "level1Product.noise.imageNoise_";
      oss2 << ".noiseEstimate.coefficient_";
    }
    else
    {
      oss << "noise[";
      oss2 << "]imageNoise.noiseEstimate.coefficient[";
      oss3 << "]";
      shift = 1;
    }
  }
  else
  {
    oss << "level1Product.noise_" << polLayer << ".imageNoise_";
    oss2 << ".noiseEstimate.coefficient_";
  }
  const std::string path_start = oss.str();
  const std::string path_middle = oss2.str();
  const std::string path_end = oss3.str();
  const unsigned int const_shift = shift;

  // set <= condition because degree N means N+1 coeff
  for (unsigned int j = 1; j <= polDegs+1; ++j)
  {
    oss.str("");
    oss << path_start << (noiseRecord - const_shift) << path_middle << (j - const_shift) << path_end;
    polCoef.push_back(mds.GetAs<double>(oss.str()));
  }

  return polCoef;
}

double TerraSarImageMetadataInterface::GetNoiseTimeUTC(const unsigned int noiseRecord,
                                                       const MetadataSupplierInterface& mds,
                                                       const unsigned int polLayer) const
{
  std::ostringstream oss;
  MetaData::Time time, defaultTime;
  std::istringstream("2000-01-01T00:0:00.000000Z") >> defaultTime;

  if(polLayer == 0)
  {
    oss << "level1Product.noise.imageNoise_" << noiseRecord << ".timeUTC";
    if (mds.HasValue(oss.str()))
      std::istringstream(mds.GetAs<std::string>(oss.str())) >> time;
    oss.str("");
    oss << "noise[" << (noiseRecord-1) << "]imageNoise.timeUTC";
    std::istringstream(mds.GetAs<std::string>("2000-01-01T00:0:00.000000Z", oss.str())) >> time;
  }
  else
  {
    oss << "level1Product.noise_" << polLayer << ".imageNoise_" << noiseRecord << ".timeUTC";
    std::istringstream(mds.GetAs<std::string>("2000-01-01T00:0:00.000000Z", oss.str())) >> time;
  }
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


double TerraSarImageMetadataInterface::GetNoiseReferencePoint(const unsigned int noiseRecord,
                                                              const MetadataSupplierInterface& mds,
                                                              const unsigned int polLayer) const
{
  std::ostringstream oss;
  if(polLayer == 0)
  {
    oss << "level1Product.noise.imageNoise_" << noiseRecord << ".noiseEstimate.referencePoint";
    if(mds.HasValue(oss.str()))
      return mds.GetAs<double>(std::numeric_limits<double>::min(), oss.str());
    oss.str();
    oss << "noise[" << (noiseRecord+1) << "]imageNoise.noiseEstimate.referencePoint";
    return mds.GetAs<double>(std::numeric_limits<double>::min(), oss.str());
  }
  oss << "level1Product.noise_" << polLayer << ".imageNoise_" << noiseRecord << ".noiseEstimate.referencePoint";
  return mds.GetAs<double>(std::numeric_limits<double>::min(), oss.str());
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
  if(mds.HasValue("level1Product.productInfo.sceneInfo.sceneCenterCoord.incidenceAngle"))
    return mds.GetAs<double>("level1Product.productInfo.sceneInfo.sceneCenterCoord.incidenceAngle");
  return mds.GetAs<double>("sceneCoord.sceneCenterCoord.incidenceAngle");
}

TerraSarImageMetadataInterface::IndexType
TerraSarImageMetadataInterface::GetCenterIncidenceAngleIndex(const MetadataSupplierInterface& mds) const
{
  TerraSarImageMetadataInterface::IndexType it;
  if(mds.HasValue("level1Product.productInfo.sceneInfo.sceneCenterCoord.refRow"))
    it[1] = mds.GetAs<int>("level1Product.productInfo.sceneInfo.sceneCenterCoord.refRow");
  else
    it[1] = mds.GetAs<int>("sceneCoord.sceneCenterCoord.refRow");
  if(mds.HasValue("level1Product.productInfo.sceneInfo.sceneCenterCoord.refColumn"))
    it[0] = mds.GetAs<int>(std::numeric_limits<int>::min(), "level1Product.productInfo.sceneInfo.sceneCenterCoord.refColumn");
  else
    it[0] = mds.GetAs<int>("sceneCoord.sceneCenterCoord.refColumn");
  return it;
}

TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface::GetCornersIncidenceAngles(const MetadataSupplierInterface& mds) const
{
  TerraSarImageMetadataInterface::DoubleVectorType dv;
  std::ostringstream oss;
  auto nbAngles = this->GetNumberOfCornerIncidenceAngles(mds);
  for (unsigned int i = 0; i < nbAngles; ++i)
  {
    oss.str("");
    oss << "level1Product.productInfo.sceneInfo.sceneCornerCoord_" << (i+1) << ".incidenceAngle";
    if(mds.HasValue(oss.str()))
      dv.push_back(mds.GetAs<double>(oss.str()));
    else
    {
      oss.str("");
      oss << "sceneCoord.sceneCornerCoord[" << i << "].incidenceAngle";
      dv.push_back(mds.GetAs<double>(oss.str()));
    }
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
    if(!imd.HasValue(oss.str()))
    {
      oss.str("");
      oss << "sceneCoord.sceneCornerCoord[" << i << "].refRow";
    }
    it[1] = imd.GetAs<int>(oss.str());

    oss.str("");
    oss << "level1Product.productInfo.sceneInfo.sceneCornerCoord_" << (i+1) << ".refColumn";
    if(!imd.HasValue(oss.str()))
    {
      oss.str("");
      oss << "sceneCoord.sceneCornerCoord[" << i << "].refColumn";
    }
    it[0] = imd.GetAs<int>(oss.str());

    iv.push_back(std::move(it));
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
TerraSarImageMetadataInterface::GetRadiometricCalibrationNoise(const MetadataSupplierInterface& mds,
                                                               const ImageMetadata& imd, const std::string& bandName) const
{
  // Retrive the polarisation layer
  unsigned int polLayer = 0;
  if(bandName != "")
  {
    std::stringstream oss;
    for(unsigned int band = 1 ; band <= imd.Bands.size() ; ++band)
    {
      oss.str("");
      oss << "level1Product.noise_" << band << ".polLayer";
      if(mds.GetAs<std::string>("", oss.str()) == bandName)
      {
        polLayer = band;
        break;
      }
    }
  }

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

  unsigned int numberOfNoiseRecords = this->GetNumberOfNoiseRecords(mds, polLayer);

  for (unsigned int noiseRecord = 1; noiseRecord <= numberOfNoiseRecords; ++noiseRecord)
  {
    double   currentNoiseTime   = this->GetNoiseTimeUTC(noiseRecord, mds, polLayer);
    RealType AzimutAcquisition  = (currentNoiseTime - startTime) * numberOfRows / (stopTime - startTime);
    RealType referencePointTime = this->GetNoiseReferencePoint(noiseRecord, mds, polLayer);

    std::vector<RealType> polynomialCoefficient;
    polynomialCoefficient = this->GetNoisePolynomialCoefficients(noiseRecord, mds, polLayer);

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

void ReadGeorefGCP(const XMLMetadataSupplier & xmlMS, const XMLMetadataSupplier & geoXmlMS, ImageMetadata & imd, SARParam & param)
{
  Projection::GCPParam gcp;
  std::stringstream ss;

  // Get the ellipsoid and semi-major, semi-minor axes
  if(geoXmlMS.HasValue("geoReference.referenceFrames.sphere.ellipsoidID"))
  {
    auto ellipsoidID = geoXmlMS.GetAs<std::string>("geoReference.referenceFrames.sphere.ellipsoidID");
    auto minor_axis = geoXmlMS.GetAs<double>(0, "geoReference.referenceFrames.sphere.semiMinorAxis");
    auto major_axis = geoXmlMS.GetAs<double>(0, "geoReference.referenceFrames.sphere.semiMajorAxis");
    if(ellipsoidID.empty() || minor_axis == 0 || major_axis == 0)
    {
      otbGenericExceptionMacro(MissingMetadataException, << "Cannot read GCP's spatial reference");
    }
    else if(ellipsoidID == "WGS84")
    {
      gcp.GCPProjection = SpatialReference::FromWGS84().ToWkt();
    }
    else
    {
      gcp.GCPProjection = SpatialReference::FromGeogCS("", "", ellipsoidID, major_axis,
                                                       major_axis/(major_axis - minor_axis)).ToWkt();
    }
  }

  auto GCPCount = geoXmlMS.GetAs<unsigned int>(0, "geoReference.geolocationGrid.numberOfGridPoints.total");
  // Count the gcps if the given count value is invalid
  if(GCPCount == 0)
    GCPCount = geoXmlMS.GetNumberOf("geoReference.geolocationGrid.gridPoint");
  // Put some reasonable limits of the number of gcps
  if(GCPCount > 5000)
    GCPCount = 5000;

  const auto azimuthTimeStart = MetaData::ReadFormattedDate(
      xmlMS.GetAs<std::string>("level1Product.productInfo.sceneInfo.start.timeUTC"));

  for(unsigned int i = 1 ; i <= GCPCount ; ++i)
  {
    ss.str("");
    ss << "geoReference.geolocationGrid.gridPoint_" << i << ".";
    const std::string id = std::to_string(i);
    gcp.GCPs.emplace_back(id,                                  // id
                     "",                                       // info
                     geoXmlMS.GetAs<double>(ss.str() + "col"), // col
                     geoXmlMS.GetAs<double>(ss.str() + "row"), // row
                     geoXmlMS.GetAs<double>(ss.str() + "lon"), // px
                     geoXmlMS.GetAs<double>(ss.str() + "lat"), // py
                     0);                                       // pz ("height" in the xml file, but GDAL doesn't read it, so we do the same)
    
    GCPTime time;
    auto deltaAz = MetaData::DurationType(geoXmlMS.GetAs<double>(ss.str() + "t"));

    time.azimuthTime = azimuthTimeStart + deltaAz;
    time.slantRangeTime = param.nearRangeTime + geoXmlMS.GetAs<double>(ss.str() + "tau"); 

    param.gcpTimes[id] = time;
  }
  imd.Add(MDGeom::GCP, gcp);
}

void ReadSARSensorModel(const XMLMetadataSupplier & xmlMS,
                        const std::string& polarization,
                        SARParam & param)
{
  // Number of entries in the vector
  int listCount = xmlMS.GetAs<int>("level1Product.platform.orbit.orbitHeader.numStateVectors");

  const std::string dateFormat = "%Y-%m-%dT%H:%M:%S%F";

  // This streams wild hold the iteration number
  std::ostringstream oss;
  for (int listId = 1 ; listId <= listCount ; ++listId)
  {
    oss.str("");
    oss << listId;
    // Base path to the data, that depends on the iteration number
    std::string path_root = "level1Product.platform.orbit.stateVec_" + oss.str();
    Orbit orbit;
    
    orbit.time = MetaData::ReadFormattedDate(xmlMS.GetAs<std::string>(path_root + ".timeUTC"), dateFormat);

    orbit.position[0] = xmlMS.GetAs<double>(path_root + ".posX");
    orbit.position[1] = xmlMS.GetAs<double>(path_root + ".posY");
    orbit.position[2] = xmlMS.GetAs<double>(path_root + ".posZ");
    orbit.velocity[0] = xmlMS.GetAs<double>(path_root + ".velX");
    orbit.velocity[1] = xmlMS.GetAs<double>(path_root + ".velY");
    orbit.velocity[2] = xmlMS.GetAs<double>(path_root + ".velZ");
    param.orbits.push_back(orbit);
  }

  param.nearRangeTime = xmlMS.GetAs<double>("level1Product.productInfo.sceneInfo.rangeTime.firstPixel");

  if(xmlMS.HasValue("level1Product.instrument.settings.RSF"))
    param.rangeSamplingRate = xmlMS.GetAs<double>("level1Product.instrument.settings.RSF");
  else
  {
    // Retrive the polarisation layer
    auto nbLayers = xmlMS.GetAs<unsigned int>("level1Product.productInfo.imageDataInfo.numberOfLayers");
    for(unsigned int band = 1 ; band <= nbLayers ; ++band)
    {
      oss.str("");
      oss << "level1Product.instrument.settings_" << band << ".polLayer";
      if(xmlMS.GetAs<std::string>(oss.str()) == polarization)
      {
        oss.str("");
        oss << "level1Product.instrument.settings_" << band << ".RSF";
        param.rangeSamplingRate = xmlMS.GetAs<double>(oss.str());
        break;
      }
    }
  }

  const auto azimuthTimeStart = MetaData::ReadFormattedDate(
      xmlMS.GetAs<std::string>("level1Product.productInfo.sceneInfo.start.timeUTC"), dateFormat);

  const auto azimuthTimeStop = MetaData::ReadFormattedDate(
      xmlMS.GetAs<std::string>("level1Product.productInfo.sceneInfo.stop.timeUTC"), dateFormat);

  const auto td = azimuthTimeStop - azimuthTimeStart;
  assert(td > MetaData::seconds(0));

  const auto numberOfRows = xmlMS.GetAs<double>("level1Product.productInfo.imageDataInfo.imageRaster.numberOfRows");

  param.azimuthTimeInterval = td / numberOfRows;

  //For Terrasar-X only 1 burst is supported for now
  BurstRecord burstRecord;

  burstRecord.azimuthStartTime = azimuthTimeStart;
  burstRecord.azimuthStopTime = azimuthTimeStop;

  burstRecord.startLine = 0;
  burstRecord.endLine = numberOfRows - 1;

  burstRecord.startSample = 0;
  burstRecord.endSample = xmlMS.GetAs<double>("level1Product.productInfo.imageDataInfo.imageRaster.numberOfColumns") - 1;
  burstRecord.azimuthAnxTime = 0.;

  param.burstRecords.push_back(burstRecord);
}


void TerraSarImageMetadataInterface::ParseGdal(ImageMetadata &imd)
{
  // Main XML file
  std::string MainDirectory = itksys::SystemTools::GetParentDirectory(
        itksys::SystemTools::GetParentDirectory(m_MetadataSupplierInterface->GetResourceFile("")));
  std::string MainFilePath =
      ExtractXMLFiles::GetResourceFile(MainDirectory, "T[S|D]X1_SAR__.*.xml", false);
  if (MainFilePath.empty())
  {
    otbGenericExceptionMacro(MissingMetadataException,
           << "Cannot find the TerraSar main XML file");
  }

  XMLMetadataSupplier MainXMLFileMetadataSupplier(MainFilePath);
  imd.Add(MDNum::NumberOfLines, MainXMLFileMetadataSupplier.GetAs<int>("level1Product.productInfo.imageDataInfo.imageRaster.numberOfRows"));
  imd.Add(MDNum::NumberOfColumns, MainXMLFileMetadataSupplier.GetAs<int>("level1Product.productInfo.imageDataInfo.imageRaster.numberOfColumns"));
  imd.Add(MDStr::OrbitDirection, MainXMLFileMetadataSupplier.GetAs<std::string>("level1Product.productInfo.missionInfo.orbitDirection"));
  imd.Add(MDNum::OrbitNumber, MainXMLFileMetadataSupplier.GetAs<double>("level1Product.productInfo.missionInfo.absOrbit"));
  imd.Add(MDNum::LineSpacing, MainXMLFileMetadataSupplier.GetAs<double>("level1Product.productSpecific.complexImageInfo.projectedSpacingAzimuth"));
  imd.Add(MDNum::PixelSpacing, MainXMLFileMetadataSupplier.GetAs<double>("level1Product.productSpecific.complexImageInfo.projectedSpacingRange.slantRange"));
  imd.Add(MDStr::Mission, MainXMLFileMetadataSupplier.GetAs<std::string>("level1Product.generalHeader.mission"));
  imd.Add(MDStr::ProductType, MainXMLFileMetadataSupplier.GetAs<std::string>("level1Product.productInfo.productVariantInfo.productType"));
  imd.Add(MDStr::Mode, MainXMLFileMetadataSupplier.GetAs<std::string>("level1Product.productInfo.acquisitionInfo.imagingMode"));
  imd.Add(MDStr::SensorID, MainXMLFileMetadataSupplier.GetAs<std::string>("level1Product.productInfo.acquisitionInfo.sensor"));
  imd.Add(MDNum::RadarFrequency, MainXMLFileMetadataSupplier.GetAs<double>("level1Product.instrument.radarParameters.centerFrequency"));
  imd.Add(MDTime::AcquisitionStartTime, MainXMLFileMetadataSupplier.GetFirstAs<MetaData::Time>("level1Product.productInfo.sceneInfo.start.timeUTC"));
  imd.Add(MDTime::AcquisitionStopTime, MainXMLFileMetadataSupplier.GetFirstAs<MetaData::Time>("level1Product.productInfo.sceneInfo.stop.timeUTC"));
  imd.Add(MDNum::RangeTimeFirstPixel, MainXMLFileMetadataSupplier.GetFirstAs<double>("level1Product.productInfo.sceneInfo.rangeTime.firstPixel"));
  imd.Add(MDNum::RangeTimeLastPixel, MainXMLFileMetadataSupplier.GetFirstAs<double>("level1Product.productInfo.sceneInfo.rangeTime.lastPixel"));
  imd.Add(MDNum::PRF, MainXMLFileMetadataSupplier.GetAs<double>("level1Product.productSpecific.complexImageInfo.commonPRF"));
  imd.Add(MDNum::RSF, MainXMLFileMetadataSupplier.GetAs<double>("level1Product.productSpecific.complexImageInfo.commonRSF"));

  // Polarisation
  auto polarization =
      itksys::SystemTools::GetFilenameName(m_MetadataSupplierInterface->GetResourceFile("")).substr(6, 2);
  imd.Add(MDStr::Polarization, polarization);

  auto numberOfCalFactor = MainXMLFileMetadataSupplier.GetNumberOf("level1Product.calibration.calibrationConstant");
  if(numberOfCalFactor == 1)
  {
    imd.Add(MDNum::CalFactor, MainXMLFileMetadataSupplier.GetAs<double>("level1Product.calibration.calibrationConstant.calFactor"));
    imd.Add(MDNum::CalScale, MainXMLFileMetadataSupplier.GetAs<double>("level1Product.calibration.calibrationConstant.calFactor"));
  }
  else
  {
    // Retrive the polarisation layer
    std::stringstream oss;
    for(unsigned int band = 1 ; band <= imd.Bands.size() ; ++band)
    {
      oss.str("");
      oss << "level1Product.calibration.calibrationConstant_" << band << ".polLayer";
      if(MainXMLFileMetadataSupplier.GetAs<std::string>(oss.str()) == polarization)
      {
        oss.str("");
        oss << "level1Product.calibration.calibrationConstant_" << band << ".calFactor";
        imd.Add(MDNum::CalFactor, MainXMLFileMetadataSupplier.GetAs<double>(oss.str()));
        imd.Add(MDNum::CalScale, MainXMLFileMetadataSupplier.GetAs<double>(oss.str()));
      }
    }
  }

  SARCalib sarCalib;
  LoadRadiometricCalibrationData(sarCalib, MainXMLFileMetadataSupplier, imd, polarization);
  sarCalib.calibrationLookupFlag = false;

  imd.Add(MDGeom::SARCalib, sarCalib);

  // Open the georef file containing GCPs
  XMLMetadataSupplier GCPXMLFileMS(MainDirectory + "/ANNOTATION/GEOREF.xml");

  SARParam sarParam;

  // Fetch the GCP
  ReadGeorefGCP(MainXMLFileMetadataSupplier, GCPXMLFileMS, imd, sarParam);

  ReadSARSensorModel(MainXMLFileMetadataSupplier, polarization, sarParam);
  imd.Add(MDGeom::SAR, sarParam);
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
  Fetch(MDNum::CalScale, imd, "calibration.calibrationConstant.calFactor");
  Fetch(MDStr::Polarization, imd, "acquisitionInfo.polarisationList[0]");
  Fetch(MDNum::NumberOfLines, imd, "number_lines");
  Fetch(MDNum::NumberOfColumns, imd, "number_samples");
    
  // Main XML file
  std::string MainFilePath = ExtractXMLFiles::GetResourceFile(itksys::SystemTools::GetFilenamePath(m_MetadataSupplierInterface->GetResourceFile("")),
                                                              "T[S|D]X1_SAR__.*.xml") ;
  if (!MainFilePath.empty())
  {
    XMLMetadataSupplier MainXMLFileMS(MainFilePath);
    imd.Add(MDNum::RadarFrequency, MainXMLFileMS.GetAs<double>("level1Product.instrument.radarParameters.centerFrequency"));
    imd.Add(MDStr::OrbitDirection, MainXMLFileMS.GetAs<std::string>("level1Product.productInfo.missionInfo.orbitDirection"));
    imd.Add(MDNum::OrbitNumber, MainXMLFileMS.GetAs<double>("level1Product.productInfo.missionInfo.absOrbit"));
    imd.Add(MDNum::RSF, MainXMLFileMS.GetAs<double>("level1Product.productSpecific.complexImageInfo.commonRSF"));

    // Open the georef file containing GCPs
    XMLMetadataSupplier GCPXMLFileMS(itksys::SystemTools::GetParentDirectory(MainFilePath) + "/ANNOTATION/GEOREF.xml");

    SARParam sarParam;

    // Fetch the GCP
    ReadGeorefGCP(MainXMLFileMS, GCPXMLFileMS, imd, sarParam);

    ReadSARSensorModel(MainXMLFileMS, imd[MDStr::Polarization], sarParam);
    imd.Add(MDGeom::SAR, sarParam);
  }

  SARCalib sarCalib;
  LoadRadiometricCalibrationData(sarCalib, *m_MetadataSupplierInterface, imd);
  sarCalib.calibrationLookupFlag = false;

  imd.Add(MDGeom::SARCalib, sarCalib);
}

void TerraSarImageMetadataInterface::Parse(ImageMetadata & imd)
{
  // Try to fetch the metadata from GEOM file
  if (m_MetadataSupplierInterface->GetAs<std::string>("", "sensor") == "TSX-1")
    this->ParseGeom(imd);
  // Try to fetch the metadata from GDAL Metadata Supplier
  else
    this->ParseGdal(imd);
}


} // end namespace otb
