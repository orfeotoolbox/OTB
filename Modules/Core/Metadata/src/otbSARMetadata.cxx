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

#include "otbSARMetadata.h"
#include "otbStringUtilities.h"

namespace
{
  const int STRING_PRECISION = 20;

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
    for (auto it = parts.begin() ; it != parts.end() ; ++it) 
    {
      if (!it->empty())
        output.push_back(otb::to<double>(*it, "Cannot cast"));
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
  void KeywordlistToVector(std::vector<T> vector,
                           const otb::MetaData::Keywordlist & kwl,
                           const std::string & prefix)
  {
    vector.clear();

    const auto size = std::stoi(kwl.at(prefix + ".number"));
    for (int i = 0; i < size; i++)
    {
      auto t = T::FromKeywordlist(kwl, prefix + "_" + to_string_with_precision(i) + ".");
      vector.push_back(t);
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
  std::istringstream iss(kwl.at(prefix + "AzimuthTimeInterval"));
  iss >> azimuthTimeInterval;

  nearRangeTime = std::stod(kwl.at(prefix + "NearRangeTime"));
  rangeSamplingRate = std::stod(kwl.at(prefix + "RangeSamplingRate"));
  rangeResolution = std::stod(kwl.at(prefix + "RangeResolution"));

  numberOfLinesPerBurst = std::stoi(kwl.at(prefix + "NumberOfLinesPerBurst"));
  numberOfSamplesPerBurst = std::stoi(kwl.at(prefix + "NumberOfSamplesPerBurst"));

  KeywordlistToVector(azimuthFmRates, kwl, prefix + "AzimuthFmRates");
  KeywordlistToVector(dopplerCentroids, kwl, prefix + "DopplerCentroid");
  KeywordlistToVector(orbits, kwl, prefix + "Orbits");
  KeywordlistToVector(burstRecords, kwl, prefix + "BurstRecords");  
  KeywordlistToVector(slantRangeToGroundRangeRecords, kwl, prefix + "SlantRangeToGroundRangeRecords");
  KeywordlistToVector(groundRangeToSlantRangeRecords, kwl, prefix + "GroundRangeToSlantRangeRecords");

  for (const auto kv : kwl)
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

      GCPTime time;

      std::istringstream iss(kv.second);
      iss >> time.azimuthTime;
      iss >> time.slantRangeTime;

      gcpTimes[id] = time;
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
  std::istringstream iss(kwl.at(prefix + "AzimuthTime"));
  iss >> output.azimuthTime;
  output.azimuthFmRatePolynomial = StringToDoubleVector(kwl.at(prefix + "AzimuthFmRatePolynomial"));

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

  std::istringstream iss(kwl.at(prefix + "AzimuthTime"));
  iss >> output.azimuthTime;

  output.t0 = std::stod(kwl.at(prefix + "t0"));
  output.dopCoef = StringToDoubleVector(kwl.at(prefix + "DopCoef"));
  output.geoDopCoef = StringToDoubleVector(kwl.at(prefix + "GeoDopCoef"));

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

  std::istringstream iss(kwl.at(prefix + "Time"));
  iss >> output.time;

  auto stringToPoint = [](const std::string & input)
  {
    std::istringstream iss(input);
    PointType output;
    iss >> output[0];
    iss >> output[1];
    iss >> output[2];
    return output;
  };

  output.position = stringToPoint(kwl.at(prefix + "Position"));
  output.velocity = stringToPoint(kwl.at(prefix + "Velocity"));

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

  std::istringstream iss(kwl.at(prefix + "AzimuthStartTime"));
  iss >> output.azimuthStartTime;
  std::istringstream iss2(kwl.at(prefix + "AzimuthStopTime"));
  iss >> output.azimuthStopTime;

  output.startLine = std::stoi(kwl.at(prefix + "StartLine"));
  output.endLine = std::stoi(kwl.at(prefix + "EndLine"));
  output.startSample = std::stoi(kwl.at(prefix + "StartSample"));
  output.endSample = std::stoi(kwl.at(prefix + "EndSample"));
  output.azimuthAnxTime = std::stod(kwl.at(prefix + "AzimuthAnxTime"));

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

  std::istringstream iss(kwl.at(prefix + "AzimuthTime"));
  iss >> output.azimuthTime;
  output.rg0 = std::stod(kwl.at(prefix + "rg0"));
  output.coeffs = StringToDoubleVector(kwl.at(prefix + "coeffs"));

  return output;
}


}
