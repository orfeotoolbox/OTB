/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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

#include "otbSARMetadata.h"
#include "otbSarCalibrationLookupData.h"
#include "otbSentinel1CalibrationLookupData.h"
#include "otbStringUtilities.h"
#include "otbMacro.h"

#include <regex>
#include <iomanip>

namespace
{
  constexpr int STRING_PRECISION = 20;

  const std::string & Get(const std::unordered_map<std::string, std::string> & kwl, const std::string & key)
  {
    try
    {
      return kwl.at(key);
    }
    catch (const std::out_of_range & e)
    {
      otbGenericExceptionMacro(itk::ExceptionObject,
             << "Unable to find '" << key << "' in the input keywordlist");
    }
  }

  // the precision of std::to_string is limited to 6 digits
  template <typename T>
  std::string to_string_with_precision(const T value)
  {
      std::ostringstream out;
      out.precision(STRING_PRECISION);
      out << std::fixed << value;
      return out.str();
  }

  std::string DoubleVectorToString(const std::vector<double> & input)
  {
    std::ostringstream oss;
    oss.precision(STRING_PRECISION);
    for (auto elem: input)
    {
      oss << elem << " ";
    }

    return oss.str();
  }

  std::vector<double> StringToDoubleVector(const std::string & input)
  {
    std::vector<double> output;

    const auto parts = otb::split_on(input, ' ');
    for (const auto & elem : parts)
    {
      if (!elem.empty())
        output.push_back(otb::to<double>(elem, "Cannot cast"));
    }

    return output;
  }

  template <class T>
  void VectorToKeywordList(otb::MetaData::Keywordlist & kwl,
                           const std::vector<T> & input,
                           const std::string & prefix)
  {
    int i = 0;
    for (const auto & elem: input)
    {
      elem.ToKeywordlist(kwl, prefix + "_" + to_string_with_precision(i) + ".");
      i++;
    }
    kwl.insert({prefix + ".number" , to_string_with_precision(i)});
  }

  template<class T>
  void KeywordlistToVector(std::vector<T> & vector,
                           const otb::MetaData::Keywordlist & kwl,
                           const std::string & prefix)
  {
    vector.clear();

    const auto size = std::stoi(Get(kwl, prefix + ".number"));
    for (int i = 0; i < size; i++)
    {
      auto t = T::FromKeywordlist(kwl, prefix + "_" + to_string_with_precision(i) + ".");
      vector.push_back(t);
    }
  }

  std::string IntArrayToString(const std::array<int, 2> & input)
  {
    std::ostringstream oss;
    for (auto elem: input)
    {
      oss << elem << " ";
    }

    return oss.str();
  }

  void StringToIntArray(const std::string & input, std::array<int, 2> output)
  {
    const auto parts = otb::split_on(input, ' ');
    auto arrayIt = output.begin();
    for (const auto & elem : parts)
    {
      if(arrayIt > output.end())
      {
        otbLogMacro(Warning, << "StringToIntArray: Exceeded size of array with string " << input);
        break;
      }
      if (!elem.empty())
      {
        *arrayIt = otb::to<int>(elem, "Cannot cast");
        ++arrayIt;
      }
    }
  }
}

namespace otb
{
void SARParam::ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix) const
{
  std::ostringstream oss;
  oss << azimuthTimeInterval;
  kwl.insert({prefix + "AzimuthTimeInterval", oss.str()});
  kwl.insert({prefix +"NearRangeTime", to_string_with_precision(nearRangeTime)});
  kwl.insert({prefix + "RangeSamplingRate", to_string_with_precision(rangeSamplingRate)});
  kwl.insert({prefix + "RangeResolution", to_string_with_precision(rangeResolution)});
  kwl.insert({prefix + "NumberOfLinesPerBurst", to_string_with_precision(numberOfLinesPerBurst)});
  kwl.insert({prefix + "NumberOfSamplesPerBurst", to_string_with_precision(numberOfSamplesPerBurst)});
  kwl.insert({prefix + "AzimuthBandwidth", to_string_with_precision(azimuthBandwidth)});
  kwl.insert({prefix + "RangeBandwidth", to_string_with_precision(rangeBandwidth)});
  kwl.insert({prefix + "AzimuthSteeringRate", to_string_with_precision(azimuthSteeringRate)});
  kwl.insert({prefix + "RightLookingFlag", std::to_string(rightLookingFlag)});

  centerSceneCoord.ToKeywordlist(kwl, prefix + "CenterSceneCoord");
  ulSceneCoord.ToKeywordlist(kwl, prefix + "ulSceneCoord");
  urSceneCoord.ToKeywordlist(kwl, prefix + "urSceneCoord");
  lrSceneCoord.ToKeywordlist(kwl, prefix + "lrSceneCoord");
  llSceneCoord.ToKeywordlist(kwl, prefix + "llSceneCoord");

  VectorToKeywordList(kwl, azimuthFmRates, prefix + "AzimuthFmRates");
  VectorToKeywordList(kwl, dopplerCentroids, prefix + "DopplerCentroid");
  VectorToKeywordList(kwl, orbits, prefix + "Orbits");
  VectorToKeywordList(kwl, burstRecords, prefix + "BurstRecords");
  VectorToKeywordList(kwl, slantRangeToGroundRangeRecords, prefix + "SlantRangeToGroundRangeRecords");
  VectorToKeywordList(kwl, groundRangeToSlantRangeRecords, prefix + "GroundRangeToSlantRangeRecords");

  for (const auto & kv : gcpTimes)
  {
    kv.second.ToKeywordlist(kwl, prefix + "GCPTimes_" + kv.first);
  }
}

void SARParam::FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix)
{
  std::istringstream iss(Get(kwl, prefix + "AzimuthTimeInterval"));
  
  if (!(iss >> azimuthTimeInterval))
  {
    otbGenericExceptionMacro(itk::ExceptionObject,
           << "Unable to decode " << Get(kwl, prefix + "AzimuthTimeInterval"));
  }

  nearRangeTime = std::stod(Get(kwl, prefix + "NearRangeTime"));
  rangeSamplingRate = std::stod(Get(kwl, prefix + "RangeSamplingRate"));
  rangeResolution = std::stod(Get(kwl, prefix + "RangeResolution"));
  rightLookingFlag = std::stoi(Get(kwl, prefix + "RightLookingFlag"));

  azimuthBandwidth = std::stod(Get(kwl, prefix + "AzimuthBandwidth"));
  rangeBandwidth = std::stod(Get(kwl, prefix + "RangeBandwidth"));
  azimuthSteeringRate = std::stod(Get(kwl, prefix + "AzimuthSteeringRate"));

  numberOfLinesPerBurst = std::stoul(Get(kwl, prefix + "NumberOfLinesPerBurst"));
  numberOfSamplesPerBurst = std::stoul(Get(kwl, prefix + "NumberOfSamplesPerBurst"));

  centerSceneCoord.FromKeywordlist(kwl, prefix + "CenterSceneCoord");
  ulSceneCoord.FromKeywordlist(kwl, prefix + "ulSceneCoord");
  urSceneCoord.FromKeywordlist(kwl, prefix + "urSceneCoord");
  lrSceneCoord.FromKeywordlist(kwl, prefix + "lrSceneCoord");
  llSceneCoord.FromKeywordlist(kwl, prefix + "llSceneCoord");

  KeywordlistToVector(azimuthFmRates, kwl, prefix + "AzimuthFmRates");
  KeywordlistToVector(dopplerCentroids, kwl, prefix + "DopplerCentroid");
  KeywordlistToVector(orbits, kwl, prefix + "Orbits");
  KeywordlistToVector(burstRecords, kwl, prefix + "BurstRecords");
  KeywordlistToVector(slantRangeToGroundRangeRecords, kwl, prefix + "SlantRangeToGroundRangeRecords");
  KeywordlistToVector(groundRangeToSlantRangeRecords, kwl, prefix + "GroundRangeToSlantRangeRecords");

  for (const auto & kv : kwl)
  {
    const std::string key = "GCPTimes_";
    auto pos = kv.first.find("GCPTimes_");
    if (pos != std::string::npos)
    {
      auto id = kv.first.substr(pos + key.size(), std::string::npos);
      if (id.empty())
      {
        continue;
      }

      std::istringstream iss(kv.second);
      if (!(iss >> gcpTimes[id].azimuthTime >> gcpTimes[id].slantRangeTime))
      {
        otbGenericExceptionMacro(itk::ExceptionObject,
           << "Unable to decode " << kv.second);
      }
    }
  }
}

void AzimuthFmRate::ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix) const
{
  std::ostringstream oss;
  oss << azimuthTime;
  kwl.insert({prefix + "AzimuthTime", oss.str()});
  kwl.insert({prefix + "AzimuthFmRatePolynomial", DoubleVectorToString(azimuthFmRatePolynomial)});
}

AzimuthFmRate AzimuthFmRate::FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix)
{
  AzimuthFmRate output;
  std::istringstream iss(Get(kwl, prefix + "AzimuthTime"));

  if (!(iss >> output.azimuthTime))
  {
    otbGenericExceptionMacro(itk::ExceptionObject,
           << "Unable to decode " << Get(kwl, prefix + "AzimuthTime"));
  }

  output.azimuthFmRatePolynomial = StringToDoubleVector(Get(kwl, prefix + "AzimuthFmRatePolynomial"));

  return output;
}

void DopplerCentroid::ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix) const
{
  std::ostringstream oss;
  oss << azimuthTime;
  kwl.insert({prefix + "AzimuthTime", oss.str()});
  kwl.insert({prefix + "t0", to_string_with_precision(t0)});
  kwl.insert({prefix + "DopCoef", DoubleVectorToString(dopCoef)});
  kwl.insert({prefix + "GeoDopCoef", DoubleVectorToString(geoDopCoef)});
}

DopplerCentroid DopplerCentroid::FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix)
{
  DopplerCentroid output;

  std::istringstream iss(Get(kwl, prefix + "AzimuthTime"));

  if (!(iss >> output.azimuthTime))
  {
    otbGenericExceptionMacro(itk::ExceptionObject,
           << "Unable to decode " << Get(kwl, prefix + "AzimuthTime"));
  }

  output.t0 = std::stod(Get(kwl, prefix + "t0"));

  auto dopCoefIt = kwl.find(prefix + "DopCoef");
  if (dopCoefIt != kwl.end())
  {
    output.dopCoef = StringToDoubleVector(dopCoefIt->second);
  }

  auto geoDopCoefIt = kwl.find(prefix + "GeoDopCoef");
  if (geoDopCoefIt != kwl.end())
  {
    output.geoDopCoef = StringToDoubleVector(geoDopCoefIt->second);
  }
  return output;
}

void Orbit::ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix) const
{
  auto pointToString = [](const PointType & input)
  {
    std::ostringstream oss;
    oss << std::setprecision(STRING_PRECISION) << input[0] << " " << input[1] << " " << input[2];
    return oss.str();
  };

  std::ostringstream oss;
  oss << time;
  kwl.insert({prefix + "Time", oss.str()});
  kwl.insert({prefix + "Position", pointToString(position)});
  kwl.insert({prefix + "Velocity", pointToString(velocity)});
}


Orbit Orbit::FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix)
{
  Orbit output;

  std::istringstream iss(Get(kwl, prefix + "Time"));

  if (!(iss >> output.time))
  {
    otbGenericExceptionMacro(itk::ExceptionObject,
           << "Unable to decode " << Get(kwl, prefix + "Time"));
  }

  auto stringToPoint = [](const std::string & input)
  {
    std::istringstream iss(input);
    PointType output;
    iss >> output[0];
    iss >> output[1];
    iss >> output[2];
    return output;
  };

  output.position = stringToPoint(Get(kwl, prefix + "Position"));
  output.velocity = stringToPoint(Get(kwl, prefix + "Velocity"));

  return output;
}

void BurstRecord::ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix) const
{
  std::ostringstream oss;
  oss << azimuthStartTime;
  kwl.insert({prefix + "AzimuthStartTime", oss.str()});
  std::ostringstream oss2;
  oss2 << azimuthStopTime;
  kwl.insert({prefix + "AzimuthStopTime", oss2.str()});
  
  kwl.insert({prefix + "StartLine", to_string_with_precision(startLine)});
  kwl.insert({prefix + "EndLine", to_string_with_precision(endLine)});
  kwl.insert({prefix + "StartSample", to_string_with_precision(startSample)});
  kwl.insert({prefix + "EndSample", to_string_with_precision(endSample)});
  kwl.insert({prefix + "AzimuthAnxTime", to_string_with_precision(azimuthAnxTime)});
}

BurstRecord BurstRecord::FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix)
{
  BurstRecord output;

  std::istringstream iss(Get(kwl, prefix + "AzimuthStartTime"));
  if (!(iss >> output.azimuthStartTime))
  {
    otbGenericExceptionMacro(itk::ExceptionObject,
           << "Unable to decode " << Get(kwl, prefix + "AzimuthStartTime"));
  }

  std::istringstream iss2(Get(kwl, prefix + "AzimuthStopTime"));
  if (!(iss2 >> output.azimuthStopTime))
  {
    otbGenericExceptionMacro(itk::ExceptionObject,
           << "Unable to decode " << Get(kwl, prefix + "AzimuthStopTime"));
  }


  output.startLine = std::stoi(Get(kwl, prefix + "StartLine"));
  output.endLine = std::stoi(Get(kwl, prefix + "EndLine"));
  output.startSample = std::stoi(Get(kwl, prefix + "StartSample"));
  output.endSample = std::stoi(Get(kwl, prefix + "EndSample"));
  output.azimuthAnxTime = std::stod(Get(kwl, prefix + "AzimuthAnxTime"));

  return output;
}

void GCPTime::ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix) const
{
  std::ostringstream oss;
  oss << azimuthTime << " " << to_string_with_precision(slantRangeTime);
  kwl.insert({prefix, oss.str()});
}

void CoordinateConversionRecord::ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix) const
{
  std::ostringstream oss;
  oss << azimuthTime;
  kwl.insert({prefix + "AzimuthTime", oss.str()});
  kwl.insert({prefix + "rg0", to_string_with_precision(rg0)});
  kwl.insert({prefix + "coeffs", DoubleVectorToString(coeffs)});
}

CoordinateConversionRecord CoordinateConversionRecord::FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix)
{
  CoordinateConversionRecord output;

  std::istringstream iss(Get(kwl, prefix + "AzimuthTime"));
  if (!(iss >> output.azimuthTime))
  {
    otbGenericExceptionMacro(itk::ExceptionObject,
           << "Unable to decode " << Get(kwl, prefix + "AzimuthTime"));
  }

  output.rg0 = std::stod(Get(kwl, prefix + "rg0"));
  output.coeffs = StringToDoubleVector(Get(kwl, prefix + "coeffs"));

  return output;
}

void InfoSceneCoord::ToKeywordlist(MetaData::Keywordlist &kwl, const std::string &prefix) const
{
  std::ostringstream oss;
  oss << azimuthTime;
  kwl.insert({prefix + "AzimuthTime", oss.str()});
  kwl.insert({prefix + "referenceRow", to_string_with_precision(referenceRow)});
  kwl.insert({prefix + "referenceColumn", to_string_with_precision(referenceColumn)});
  kwl.insert({prefix + "latitude", to_string_with_precision(latitude)});
  kwl.insert({prefix + "longitude", to_string_with_precision(longitude)});
  kwl.insert({prefix + "rangeTime", to_string_with_precision(rangeTime)});
  kwl.insert({prefix + "incidenceAngle", to_string_with_precision(incidenceAngle)});
}

InfoSceneCoord InfoSceneCoord::FromKeywordlist(const MetaData::Keywordlist &kwl, const std::string &prefix)
{
  InfoSceneCoord output;

  std::istringstream iss(Get(kwl, prefix + "AzimuthTime"));
  if (!(iss >> output.azimuthTime))
  {
    otbGenericExceptionMacro(itk::ExceptionObject,
           << "Unable to decode " << Get(kwl, prefix + "AzimuthTime"));
  }

  output.referenceRow = std::stoul(Get(kwl, prefix + "referenceRow"));
  output.referenceColumn = std::stoul(Get(kwl, prefix + "referenceColumn"));
  output.latitude = std::stod(Get(kwl, prefix + "latitude"));
  output.longitude = std::stod(Get(kwl, prefix + "longitude"));
  output.rangeTime = std::stod(Get(kwl, prefix + "rangeTime"));
  output.incidenceAngle = std::stod(Get(kwl, prefix + "incidenceAngle"));

  return output;
}

void SARCalib::ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix) const
{
  // Boolean
  kwl.insert({prefix +"CalibrationLookupFlag", boost::lexical_cast<std::string>(calibrationLookupFlag)});

  // Double
  kwl.insert({prefix + "RescalingFactor", to_string_with_precision(rescalingFactor)});

  // MetaData::Time
  std::ostringstream oss;
  oss << calibrationStartTime;
  kwl.insert({prefix + "CalibrationStartTime", oss.str()});
  oss.str("");
  oss << calibrationStopTime;
  kwl.insert({prefix + "CalibrationStopTime", oss.str()});

  // std::array<int>
  kwl.insert({prefix + "RadiometricCalibrationNoisePolynomialDegree",
              IntArrayToString(radiometricCalibrationNoisePolynomialDegree)});
  kwl.insert({prefix + "RadiometricCalibrationAntennaPatternNewGainPolynomialDegree",
              IntArrayToString(radiometricCalibrationAntennaPatternNewGainPolynomialDegree)});
  kwl.insert({prefix + "RadiometricCalibrationAntennaPatternOldGainPolynomialDegree",
              IntArrayToString(radiometricCalibrationAntennaPatternOldGainPolynomialDegree)});
  kwl.insert({prefix + "RadiometricCalibrationIncidenceAnglePolynomialDegree",
              IntArrayToString(radiometricCalibrationIncidenceAnglePolynomialDegree)});
  kwl.insert({prefix + "RadiometricCalibrationRangeSpreadLossPolynomialDegree",
              IntArrayToString(radiometricCalibrationRangeSpreadLossPolynomialDegree)});

  // itk::PointSet<double, 2>
  auto pointsetToString = [](const PointSetType::Pointer input)
  {
    PointSetType::PointType point;
    typename PointSetType::PixelType pointValue;
    std::ostringstream oss;
    oss << std::setprecision(STRING_PRECISION);
    for(unsigned int i = 0 ; i < input->GetNumberOfPoints() ; ++i)
    {
      input->GetPoint(i, &point);
      input->GetPointData(i, &pointValue);
      oss << point << " " << pointValue << ";";
    }
    return oss.str();
  };
  kwl.insert({prefix + "RadiometricCalibrationNoise",
              pointsetToString(radiometricCalibrationNoise)});
  kwl.insert({prefix + "RadiometricCalibrationAntennaPatternNewGain",
              pointsetToString(radiometricCalibrationAntennaPatternNewGain)});
  kwl.insert({prefix + "RadiometricCalibrationAntennaPatternOldGain",
              pointsetToString(radiometricCalibrationAntennaPatternOldGain)});
  kwl.insert({prefix + "RadiometricCalibrationIncidenceAngle",
              pointsetToString(radiometricCalibrationIncidenceAngle)});
  kwl.insert({prefix + "RadiometricCalibrationRangeSpreadLoss",
              pointsetToString(radiometricCalibrationRangeSpreadLoss)});

  // std::unordered_map<short, LookupDataType::Pointer>
  for (const auto & kv : calibrationLookupData)
  {
    kv.second->ToKeywordlist(kwl, prefix + "CalibrationLookupData_"
                             + boost::lexical_cast<std::string>(kv.first) + "_");
  }
}

void SARCalib::FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix)
{
  // Boolean
  calibrationLookupFlag = boost::lexical_cast<bool>(Get(kwl, prefix + "CalibrationLookupFlag"));

  // Double
  rescalingFactor = std::stod(Get(kwl, prefix + "RescalingFactor"));

  // MetaData::Time
  std::istringstream iss(Get(kwl, prefix + "CalibrationStartTime"));
  if (!(iss >> calibrationStartTime))
  {
    otbGenericExceptionMacro(itk::ExceptionObject,
           << "Unable to decode " << Get(kwl, prefix + "CalibrationStartTime"));
  }
  std::istringstream iss2(Get(kwl, prefix + "CalibrationStopTime"));
  if (!(iss2 >> calibrationStopTime))
  {
    otbGenericExceptionMacro(itk::ExceptionObject,
           << "Unable to decode " << Get(kwl, prefix + "CalibrationStopTime"));
  }

  // std::array<int>
  StringToIntArray(Get(kwl, prefix + "RadiometricCalibrationNoisePolynomialDegree"),
                   radiometricCalibrationNoisePolynomialDegree);
  StringToIntArray(Get(kwl, prefix + "RadiometricCalibrationAntennaPatternNewGainPolynomialDegree"),
                   radiometricCalibrationAntennaPatternNewGainPolynomialDegree);
  StringToIntArray(Get(kwl, prefix + "RadiometricCalibrationAntennaPatternOldGainPolynomialDegree"),
                   radiometricCalibrationAntennaPatternOldGainPolynomialDegree);
  StringToIntArray(Get(kwl, prefix + "RadiometricCalibrationIncidenceAnglePolynomialDegree"),
                   radiometricCalibrationIncidenceAnglePolynomialDegree);
  StringToIntArray(Get(kwl, prefix + "RadiometricCalibrationRangeSpreadLossPolynomialDegree"),
                   radiometricCalibrationRangeSpreadLossPolynomialDegree);

  // itk::PointSet<double, 2>
  auto stringToPointSet = [](const std::string & input, PointSetType::Pointer output)
  {
    output->Initialize();

    unsigned int id = 0;
    const std::regex PointSet_regex("\\[([0-9]+), ([0-9]+)] ([0-9]+)");
    std::smatch PointSet_match;
    PointSetType::PointType point;
    typename PointSetType::PixelType pointValue;
    const auto pointsets = otb::split_on(input, ';');
    for (const auto & pointset : pointsets)
    {
      if (pointset.empty())
        continue;
      const auto pointset_str = otb::to<std::string>(pointset, "Cannot cast");
      if (std::regex_match(pointset_str, PointSet_match, PointSet_regex))
      {
        point[0] = std::stoi(PointSet_match[1]);
        point[1] = std::stoi(PointSet_match[2]);
        pointValue = boost::lexical_cast<typename PointSetType::PixelType>(PointSet_match[3]);
        output->SetPoint(id, point);
        output->SetPointData(id, pointValue);
        ++id;
      }
    }
  };
  stringToPointSet(Get(kwl, prefix + "RadiometricCalibrationNoise"),
                   radiometricCalibrationNoise);
  stringToPointSet(Get(kwl, prefix + "RadiometricCalibrationAntennaPatternNewGain"),
                   radiometricCalibrationAntennaPatternNewGain);
  stringToPointSet(Get(kwl, prefix + "RadiometricCalibrationAntennaPatternOldGain"),
                   radiometricCalibrationAntennaPatternOldGain);
  stringToPointSet(Get(kwl, prefix + "RadiometricCalibrationIncidenceAngle"),
                   radiometricCalibrationIncidenceAngle);
  stringToPointSet(Get(kwl, prefix + "RadiometricCalibrationRangeSpreadLoss"),
                   radiometricCalibrationRangeSpreadLoss);

  // std::unordered_map<short, LookupDataType::Pointer>
  for (const auto & kv : kwl)
  {
    const std::string key = "CalibrationLookupData_";
    auto pos = kv.first.find("CalibrationLookupData_");
    if (pos != std::string::npos)
    {
      auto id = kv.first.substr(pos + key.size(), std::string::npos);
      if (id.empty())
        continue;
      pos = id.find("_Sensor");
      if (pos == std::string::npos)
        continue;
      id = id.substr(0, pos);
      short id_short= boost::lexical_cast<short>(id);
      auto sensor = kv.second;

      if (sensor == "Sentinel1")
      {
        auto lut = Sentinel1CalibrationLookupData::New();
        lut->FromKeywordlist(kwl, prefix + "CalibrationLookupData_" + id + "_");
        calibrationLookupData[id_short] = lut;
      }
      else
      {
        auto lut = SarCalibrationLookupData::New();
        lut->FromKeywordlist(kwl, prefix + "CalibrationLookupData_" + id + "_");
        calibrationLookupData[id_short] = lut;
      }
    }
  }
}

}
