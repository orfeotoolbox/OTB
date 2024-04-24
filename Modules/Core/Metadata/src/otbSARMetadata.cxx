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
#include "otbSpan.h"
#include "otbMacro.h"

#include <regex>
#include <iomanip>

namespace
{
using dict_t = std::unordered_map<std::string, std::string>;
constexpr int STRING_PRECISION = 20;

const std::string & Get(const dict_t & kwl, const std::string & key)
{
  auto const& wh = kwl.find(key);
  if (wh != kwl.end())
  {
    return wh->second;
  }
  else
  {
    otbGenericExceptionMacro(itk::ExceptionObject,
        << "Unable to find '" << key << "' in the input keywordlist");
  }
}

template <typename T> inline T value_or_throw(
    dict_t const& kwl,
    std::string const& key,
    otb::string_view context)
{
  auto const& wh = kwl.find(key);
  if (wh != kwl.end())
  {
    auto const& value = wh->second;
    return otb::to<T>(value, "converting metadata '"+key+"' "+context);
  }
  else
  {
    otbGenericExceptionMacro(itk::ExceptionObject,
        << "Unable to find '" << key << "' in the input keywordlist " << context);
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

std::string DoubleVectorToString(otb::Span<double const> input)
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
                         std::vector<T> const& input,
                         const std::string & prefix)
{
  int i = 0;
  for (const auto & elem: input)
  {
    elem.ToKeywordlist(kwl, prefix + "_" + to_string_with_precision(i) + ".");
    i++;
  }
  kwl.emplace(prefix + ".number" , to_string_with_precision(i));
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

std::string IntArrayToString(otb::Span<int const> input)
{
  std::ostringstream oss;
  for (auto elem: input)
  {
    oss << elem << " ";
  }

  return oss.str();
}

void StringToIntArray(const std::string & input, otb::Span<int> output)
{
  const auto parts = otb::split_on(input, ' ');
  auto arrayIt = output.begin();
  for (const auto & elem : parts)
  {
    if(arrayIt > output.end())
    {
      otbLogMacro(Warning, << "StringToIntArray: Exceeded size of array ("<<output.size()<<") with string " << input);
      break;
    }
    if (!elem.empty())
    {
      *arrayIt = otb::to<int>(elem, "Cannot cast");
      ++arrayIt;
    }
  }
}
} // anonymous namespace 

namespace otb
{
void SARParam::ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix) const
{
  std::ostringstream oss;
  oss << azimuthTimeInterval;
  kwl.emplace(prefix + "AzimuthTimeInterval",     oss.str());
  kwl.emplace(prefix + "NearRangeTime",           to_string_with_precision(nearRangeTime));
  kwl.emplace(prefix + "RangeSamplingRate",       to_string_with_precision(rangeSamplingRate));
  kwl.emplace(prefix + "RangeResolution",         to_string_with_precision(rangeResolution));
  kwl.emplace(prefix + "NumberOfLinesPerBurst",   to_string_with_precision(numberOfLinesPerBurst));
  kwl.emplace(prefix + "NumberOfSamplesPerBurst", to_string_with_precision(numberOfSamplesPerBurst));
  kwl.emplace(prefix + "AzimuthBandwidth",        to_string_with_precision(azimuthBandwidth));
  kwl.emplace(prefix + "RangeBandwidth",          to_string_with_precision(rangeBandwidth));
  kwl.emplace(prefix + "AzimuthSteeringRate",     to_string_with_precision(azimuthSteeringRate));
  kwl.emplace(prefix + "RightLookingFlag",        std::to_string(rightLookingFlag));

  centerSceneCoord.ToKeywordlist(kwl, prefix + "CenterSceneCoord");
  ulSceneCoord.ToKeywordlist(kwl, prefix + "ulSceneCoord");
  urSceneCoord.ToKeywordlist(kwl, prefix + "urSceneCoord");
  lrSceneCoord.ToKeywordlist(kwl, prefix + "lrSceneCoord");
  llSceneCoord.ToKeywordlist(kwl, prefix + "llSceneCoord");

  VectorToKeywordList(kwl, azimuthFmRates,                 prefix + "AzimuthFmRates");
  VectorToKeywordList(kwl, dopplerCentroids,               prefix + "DopplerCentroid");
  VectorToKeywordList(kwl, orbits,                         prefix + "Orbits");
  VectorToKeywordList(kwl, burstRecords,                   prefix + "BurstRecords");
  VectorToKeywordList(kwl, slantRangeToGroundRangeRecords, prefix + "SlantRangeToGroundRangeRecords");
  VectorToKeywordList(kwl, groundRangeToSlantRangeRecords, prefix + "GroundRangeToSlantRangeRecords");

  for (const auto & kv : gcpTimes)
  {
    kv.second.ToKeywordlist(kwl, prefix + "GCPTimes_" + kv.first);
  }
}

void SARParam::FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix)
{
#if 1
  azimuthTimeInterval = value_or_throw<MetaData::Duration>(kwl, prefix + "AzimuthTimeInterval", "(FromKeywordlist)");
#else
  std::istringstream iss(Get(kwl, prefix + "AzimuthTimeInterval"));

  if (!(iss >> azimuthTimeInterval))
  {
    otbGenericExceptionMacro(itk::ExceptionObject,
        << "Unable to decode " << Get(kwl, prefix + "AzimuthTimeInterval"));
  }
#endif


  // TODO: use to<>
  nearRangeTime           = value_or_throw<double>(kwl, prefix + "NearRangeTime",       "(FromKeywordlist)");
  rangeSamplingRate       = value_or_throw<double>(kwl, prefix + "RangeSamplingRate",   "(FromKeywordlist)");
  rangeResolution         = value_or_throw<double>(kwl, prefix + "RangeResolution",     "(FromKeywordlist)");
  rightLookingFlag        = value_or_throw<int>   (kwl, prefix + "RightLookingFlag",    "(FromKeywordlist)");

  azimuthBandwidth        = value_or_throw<double>(kwl, prefix + "AzimuthBandwidth",    "(FromKeywordlist)");
  rangeBandwidth          = value_or_throw<double>(kwl, prefix + "RangeBandwidth",      "(FromKeywordlist)");
  azimuthSteeringRate     = value_or_throw<double>(kwl, prefix + "AzimuthSteeringRate", "(FromKeywordlist)");

  numberOfLinesPerBurst   = value_or_throw<unsigned long>(kwl, prefix + "NumberOfLinesPerBurst", "(FromKeywordlist)");
  numberOfSamplesPerBurst = value_or_throw<unsigned long>(kwl, prefix + "NumberOfSamplesPerBurst", "(FromKeywordlist)");

  centerSceneCoord.FromKeywordlist(kwl, prefix + "CenterSceneCoord");
  ulSceneCoord.FromKeywordlist(kwl, prefix + "ulSceneCoord");
  urSceneCoord.FromKeywordlist(kwl, prefix + "urSceneCoord");
  lrSceneCoord.FromKeywordlist(kwl, prefix + "lrSceneCoord");
  llSceneCoord.FromKeywordlist(kwl, prefix + "llSceneCoord");

  KeywordlistToVector(azimuthFmRates,                 kwl, prefix + "AzimuthFmRates");
  KeywordlistToVector(dopplerCentroids,               kwl, prefix + "DopplerCentroid");
  KeywordlistToVector(orbits,                         kwl, prefix + "Orbits");
  KeywordlistToVector(burstRecords,                   kwl, prefix + "BurstRecords");
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
  kwl.emplace(prefix + "AzimuthTime", oss.str());
  kwl.emplace(prefix + "AzimuthFmRatePolynomial", DoubleVectorToString(azimuthFmRatePolynomial));
}

AzimuthFmRate AzimuthFmRate::FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix)
{
  AzimuthFmRate output;
#if 1
  output.azimuthTime = value_or_throw<MetaData::TimePoint>(kwl, prefix + "AzimuthTime", "(AzimuthFmRate::FromKeywordlist)");
#else
  std::istringstream iss(Get(kwl, prefix + "AzimuthTime"));

  if (!(iss >> output.azimuthTime))
  {
    otbGenericExceptionMacro(itk::ExceptionObject,
        << "Unable to decode " << Get(kwl, prefix + "AzimuthTime"));
  }
#endif

  output.azimuthFmRatePolynomial = StringToDoubleVector(Get(kwl, prefix + "AzimuthFmRatePolynomial"));

  return output;
}

void DopplerCentroid::ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix) const
{
  std::ostringstream oss;
  oss << azimuthTime;
  kwl.emplace(prefix + "AzimuthTime", oss.str());
  kwl.emplace(prefix + "t0",          to_string_with_precision(t0));
  kwl.emplace(prefix + "DopCoef",     DoubleVectorToString(dopCoef));
  kwl.emplace(prefix + "GeoDopCoef",  DoubleVectorToString(geoDopCoef));
}

DopplerCentroid DopplerCentroid::FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix)
{
  DopplerCentroid output;
#if 1
  output.azimuthTime = value_or_throw<MetaData::TimePoint>(kwl, prefix + "AzimuthTime", "(DopplerCentroid::FromKeywordlist)");
#else
  std::istringstream iss(Get(kwl, prefix + "AzimuthTime"));

  if (!(iss >> output.azimuthTime))
  {
    otbGenericExceptionMacro(itk::ExceptionObject,
        << "Unable to decode " << Get(kwl, prefix + "AzimuthTime"));
  }
#endif

  output.t0 = value_or_throw<double>(kwl, prefix + "t0", "(DopplerCentroid::FromKeywordlist)");

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
  kwl.emplace(prefix + "Time",     oss.str());
  kwl.emplace(prefix + "Position", pointToString(position));
  kwl.emplace(prefix + "Velocity", pointToString(velocity));
}


Orbit Orbit::FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix)
{
  Orbit output;

#if 1
  output.time = value_or_throw<MetaData::TimePoint>(kwl, prefix + "Time", "(Orbit::FromKeywordlist)");

  output.position = value_or_throw<Orbit::PointType>(kwl, prefix + "Position", "(Orbit::FromKeywordlist)");
  output.velocity = value_or_throw<Orbit::PointType>(kwl, prefix + "Velocity", "(Orbit::FromKeywordlist)");
#else
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
#endif

  return output;
}

void BurstRecord::ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix) const
{
  std::ostringstream oss;
  oss << azimuthStartTime;
  kwl.emplace(prefix + "AzimuthStartTime", oss.str());
  oss.str("");
  oss << azimuthStopTime;
  kwl.emplace(prefix + "AzimuthStopTime", oss.str());

  kwl.emplace(prefix + "StartLine",       to_string_with_precision(startLine));
  kwl.emplace(prefix + "EndLine",         to_string_with_precision(endLine));
  kwl.emplace(prefix + "StartSample",     to_string_with_precision(startSample));
  kwl.emplace(prefix + "EndSample",       to_string_with_precision(endSample));
  kwl.emplace(prefix + "AzimuthAnxTime",  to_string_with_precision(azimuthAnxTime));
}

BurstRecord BurstRecord::FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix)
{
  BurstRecord output;

#if 1
  output.azimuthStartTime = value_or_throw<MetaData::TimePoint>(kwl, prefix + "AzimuthStartTime", "(BurstRecord::FromKeywordlist)");
  output.azimuthStopTime  = value_or_throw<MetaData::TimePoint>(kwl, prefix + "AzimuthStopTime" , "(BurstRecord::FromKeywordlist)");
#else
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
#endif

  output.startLine      = value_or_throw<int>   (kwl, prefix + "StartLine",      "(BurstRecord::FromKeywordlist)");
  output.endLine        = value_or_throw<int>   (kwl, prefix + "EndLine",        "(BurstRecord::FromKeywordlist)");
  output.startSample    = value_or_throw<int>   (kwl, prefix + "StartSample",    "(BurstRecord::FromKeywordlist)");
  output.endSample      = value_or_throw<int>   (kwl, prefix + "EndSample",      "(BurstRecord::FromKeywordlist)");
  output.azimuthAnxTime = value_or_throw<double>(kwl, prefix + "AzimuthAnxTime", "(BurstRecord::FromKeywordlist)");

  return output;
}

void GCPTime::ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix) const
{
  std::ostringstream oss;
  oss << azimuthTime << " " << to_string_with_precision(slantRangeTime);
  kwl.emplace(prefix, oss.str());
}

void CoordinateConversionRecord::ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix) const
{
  std::ostringstream oss;
  oss << azimuthTime;
  kwl.emplace(prefix + "AzimuthTime", oss.str());
  kwl.emplace(prefix + "rg0",         to_string_with_precision(rg0));
  kwl.emplace(prefix + "coeffs",      DoubleVectorToString(coeffs));
}

CoordinateConversionRecord CoordinateConversionRecord::FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix)
{
  CoordinateConversionRecord output;

#if 1
  output.azimuthTime = value_or_throw<MetaData::TimePoint>(kwl, prefix + "AzimuthTime", "(CoordinateConversionRecord::FromKeywordlist)");
#else
  std::istringstream iss(Get(kwl, prefix + "AzimuthTime"));
  if (!(iss >> output.azimuthTime))
  {
    otbGenericExceptionMacro(itk::ExceptionObject,
        << "Unable to decode " << Get(kwl, prefix + "AzimuthTime"));
  }
#endif

  output.rg0 = value_or_throw<double>(kwl, prefix + "rg0", "(CoordinateConversionRecord::FromKeywordlist)");
  output.coeffs = StringToDoubleVector(Get(kwl, prefix + "coeffs"));

  return output;
}

void InfoSceneCoord::ToKeywordlist(MetaData::Keywordlist &kwl, const std::string &prefix) const
{
  std::ostringstream oss;
  oss << azimuthTime;
  kwl.emplace(prefix + "AzimuthTime",     oss.str());
  kwl.emplace(prefix + "referenceRow",    to_string_with_precision(referenceRow));
  kwl.emplace(prefix + "referenceColumn", to_string_with_precision(referenceColumn));
  kwl.emplace(prefix + "latitude",        to_string_with_precision(latitude));
  kwl.emplace(prefix + "longitude",       to_string_with_precision(longitude));
  kwl.emplace(prefix + "rangeTime",       to_string_with_precision(rangeTime));
  kwl.emplace(prefix + "incidenceAngle",  to_string_with_precision(incidenceAngle));
}

InfoSceneCoord InfoSceneCoord::FromKeywordlist(const MetaData::Keywordlist &kwl, const std::string &prefix)
{
  InfoSceneCoord output;

#if 1
  output.azimuthTime = value_or_throw<MetaData::TimePoint>(kwl, prefix + "AzimuthTime", "(InfoSceneCoord::FromKeywordlist)");
#else
  std::istringstream iss(Get(kwl, prefix + "AzimuthTime"));
  if (!(iss >> output.azimuthTime))
  {
    otbGenericExceptionMacro(itk::ExceptionObject,
        << "Unable to decode " << Get(kwl, prefix + "AzimuthTime"));
  }
#endif

  output.referenceRow    = value_or_throw<unsigned long>(kwl, prefix + "referenceRow", "(InfoSceneCoord::FromKeywordlist)");
  output.referenceColumn = value_or_throw<unsigned long>(kwl, prefix + "referenceColumn", "(InfoSceneCoord::FromKeywordlist)");
  output.latitude        = value_or_throw<double>(kwl, prefix + "latitude", "(InfoSceneCoord::FromKeywordlist)");
  output.longitude       = value_or_throw<double>(kwl, prefix + "longitude", "(InfoSceneCoord::FromKeywordlist)");
  output.rangeTime       = value_or_throw<double>(kwl, prefix + "rangeTime", "(InfoSceneCoord::FromKeywordlist)");
  output.incidenceAngle  = value_or_throw<double>(kwl, prefix + "incidenceAngle", "(InfoSceneCoord::FromKeywordlist)");

  return output;
}

void SARCalib::ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix) const
{
  // Boolean
  kwl.emplace(prefix +"CalibrationLookupFlag", boost::lexical_cast<std::string>(calibrationLookupFlag));

  // Double
  kwl.emplace(prefix + "RescalingFactor", to_string_with_precision(rescalingFactor));

  // MetaData::Time
  std::ostringstream oss;
  oss << calibrationStartTime;
  kwl.emplace(prefix + "CalibrationStartTime", oss.str());
  oss.str("");
  oss << calibrationStopTime;
  kwl.emplace(prefix + "CalibrationStopTime", oss.str());

  // std::array<int>
  kwl.emplace(prefix + "RadiometricCalibrationNoisePolynomialDegree",
              IntArrayToString(radiometricCalibrationNoisePolynomialDegree));
  kwl.emplace(prefix + "RadiometricCalibrationAntennaPatternNewGainPolynomialDegree",
              IntArrayToString(radiometricCalibrationAntennaPatternNewGainPolynomialDegree));
  kwl.emplace(prefix + "RadiometricCalibrationAntennaPatternOldGainPolynomialDegree",
              IntArrayToString(radiometricCalibrationAntennaPatternOldGainPolynomialDegree));
  kwl.emplace(prefix + "RadiometricCalibrationIncidenceAnglePolynomialDegree",
              IntArrayToString(radiometricCalibrationIncidenceAnglePolynomialDegree));
  kwl.emplace(prefix + "RadiometricCalibrationRangeSpreadLossPolynomialDegree",
              IntArrayToString(radiometricCalibrationRangeSpreadLossPolynomialDegree));

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
  kwl.emplace(prefix + "RadiometricCalibrationNoise",
              pointsetToString(radiometricCalibrationNoise));
  kwl.emplace(prefix + "RadiometricCalibrationAntennaPatternNewGain",
              pointsetToString(radiometricCalibrationAntennaPatternNewGain));
  kwl.emplace(prefix + "RadiometricCalibrationAntennaPatternOldGain",
              pointsetToString(radiometricCalibrationAntennaPatternOldGain));
  kwl.emplace(prefix + "RadiometricCalibrationIncidenceAngle",
              pointsetToString(radiometricCalibrationIncidenceAngle));
  kwl.emplace(prefix + "RadiometricCalibrationRangeSpreadLoss",
              pointsetToString(radiometricCalibrationRangeSpreadLoss));

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
  rescalingFactor = value_or_throw<double>(kwl, prefix + "RescalingFactor", "(SARCalib::FromKeywordlist)");

  // MetaData::TimePoint
#if 1
  calibrationStartTime = value_or_throw<MetaData::TimePoint>(kwl, prefix + "CalibrationStartTime", "(SARCalib::FromKeywordlist)");
  calibrationStopTime  = value_or_throw<MetaData::TimePoint>(kwl, prefix + "CalibrationStopTime",  "(SARCalib::FromKeywordlist)");
#else
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
#endif


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
