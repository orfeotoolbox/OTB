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

#include "otbSentinel1CalibrationLookupData.h"

namespace
{
  constexpr int STRING_PRECISION = 20;

  // the precision of std::to_string is limited to 6 digits
  template <typename T>
  std::string to_string_with_precision(const T value)
  {
      std::ostringstream out;
      out.precision(STRING_PRECISION);
      out << std::fixed << value;
      return out.str();
  }

  template <class T>
  std::string ScalarVectorToString(const std::vector<T> & input)
  {
    std::ostringstream oss;
    oss.precision(STRING_PRECISION);
    for (auto elem: input)
    {
      oss << elem << " ";
    }

    return oss.str();
  }

  template <class T>
  std::vector<T> StringToScalarVector(const std::string & input)
  {
    std::vector<T> output;

    const auto parts = otb::split_on(input, ' ');
    for (const auto & elem : parts)
    {
      if (!elem.empty())
        output.push_back(otb::to<T>(elem, "Cannot cast"));
    }

    return output;
  }

}

namespace otb
{

void Sentinel1CalibrationStruct::ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix) const
{
  // Double
  kwl.insert({prefix + "TimeMJD", to_string_with_precision(timeMJD)});
  kwl.insert({prefix + "DeltaMJD", to_string_with_precision(deltaMJD)});

  // Int
  kwl.insert({prefix +"Line", boost::lexical_cast<std::string>(line)});

  // std::vector<T>
  kwl.insert({prefix + "Pixels", ScalarVectorToString(pixels)});
  kwl.insert({prefix + "DeltaPixels", ScalarVectorToString(deltaPixels)});
  kwl.insert({prefix + "Vect", ScalarVectorToString(vect)});
}

Sentinel1CalibrationStruct Sentinel1CalibrationStruct::FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix)
{
  Sentinel1CalibrationStruct output;

  // Double
  output.timeMJD = std::stod(kwl.at(prefix + "TimeMJD"));
  output.deltaMJD = std::stod(kwl.at(prefix + "DeltaMJD"));

  // Int
  output.line = std::stoi(kwl.at(prefix + "Line"));

  // std::vector<T>
  output.pixels = StringToScalarVector<int>(kwl.at(prefix + "Pixels"));
  output.deltaPixels = StringToScalarVector<double>(kwl.at(prefix + "DeltaPixels"));
  output.vect = StringToScalarVector<float>(kwl.at(prefix + "Vect"));

  return output;
}

Sentinel1CalibrationLookupData::Sentinel1CalibrationLookupData() : firstLineTime(0.), lastLineTime(0.), numOfLines(0), count(0), lineTimeInterval(0.)
{
}

void Sentinel1CalibrationLookupData::InitParameters(short type, double ft, double lt, int lines, int c, std::vector<Sentinel1CalibrationStruct> const& vlist)
{
  firstLineTime         = ft;
  lastLineTime          = lt;
  numOfLines            = lines;
  count                 = c;
  calibrationVectorList = vlist;
  this->SetType(type);
  lineTimeInterval = (lt - ft) / ((lines - 1) * 1.0);
}

double Sentinel1CalibrationLookupData::GetValue(const IndexValueType x, const IndexValueType y) const
{
  const int calVecIdx = GetVectorIndex(y);
  assert(calVecIdx >= 0 && calVecIdx < count - 1);
  const Sentinel1CalibrationStruct& vec0     = calibrationVectorList[calVecIdx];
  const Sentinel1CalibrationStruct& vec1     = calibrationVectorList[calVecIdx + 1];
  const double                      azTime   = firstLineTime + y * lineTimeInterval;
  const double                      muY      = (azTime - vec0.timeMJD) / vec1.deltaMJD;
  const int                         pixelIdx = GetPixelIndex(x, calibrationVectorList[calVecIdx]);
  const double                      muX      = (x - vec0.pixels[pixelIdx]) / vec0.deltaPixels[pixelIdx + 1];
  const double                      lutVal =
      (1 - muY) * ((1 - muX) * vec0.vect[pixelIdx] + muX * vec0.vect[pixelIdx + 1]) + muY * ((1 - muX) * vec1.vect[pixelIdx] + muX * vec1.vect[pixelIdx + 1]);
  return lutVal;
}

int Sentinel1CalibrationLookupData::GetVectorIndex(int y) const
{
  for (int i = 1; i < count; i++)
  {
    if (y < calibrationVectorList[i].line)
    {
      return i - 1;
    }
  }
  return -1;
}

int Sentinel1CalibrationLookupData::GetPixelIndex(int x, const Sentinel1CalibrationStruct& calVec) const
{
  const int                        size = calVec.pixels.size();
  std::vector<int>::const_iterator wh   = std::upper_bound(calVec.pixels.begin(), calVec.pixels.end(), x);
  return wh == calVec.pixels.end() ? size - 2 : std::distance(calVec.pixels.begin(), wh) - 1;
}

} // namespace otb