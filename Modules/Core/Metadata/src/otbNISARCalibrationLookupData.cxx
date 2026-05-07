/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#include "otbNISARCalibrationLookupData.h"
#include "otbStringUtilities.h"
#include "otbMacro.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

namespace
{
constexpr int STRING_PRECISION = 20;

template <typename T>
std::string ToStringWithPrecision(const T value)
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
  for (const auto & elem : input)
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

struct InterpolationIndex
{
  std::size_t lower = 0;
  std::size_t upper = 0;
  double fraction = 0.0;
};

InterpolationIndex GetInterpolationIndex(const std::vector<double> & axis, double value)
{
  if (axis.empty())
  {
    otbGenericExceptionMacro(itk::ExceptionObject, << "NISAR calibration lookup axis is empty.");
  }

  if (axis.size() == 1)
  {
    return {0, 0, 0.0};
  }

  if (value <= axis.front())
  {
    return {0, 1, 0.0};
  }

  if (value >= axis.back())
  {
    return {axis.size() - 2, axis.size() - 1, 1.0};
  }

  const auto wh = std::upper_bound(axis.begin(), axis.end(), value);
  const auto upper = static_cast<std::size_t>(std::distance(axis.begin(), wh));
  const auto lower = upper - 1;
  const auto x0 = axis[lower];
  const auto x1 = axis[upper];
  const auto fraction = (x1 == x0) ? 0.0 : (value - x0) / (x1 - x0);
  return {lower, upper, fraction};
}
}

namespace otb
{

NISARCalibrationLookupData::NISARCalibrationLookupData()
  : m_ImageFirstAzimuthTime(0.0),
    m_ImageAzimuthTimeSpacing(0.0),
    m_ImageFirstSlantRange(0.0),
    m_ImageSlantRangeSpacing(0.0)
{
}

void NISARCalibrationLookupData::Initialize(short type,
                                            double imageFirstAzimuthTime,
                                            double imageAzimuthTimeSpacing,
                                            double imageFirstSlantRange,
                                            double imageSlantRangeSpacing,
                                            std::vector<double> lutAzimuthTimes,
                                            std::vector<double> lutSlantRanges,
                                            std::vector<float> lutValues)
{
  this->SetType(type);
  m_ImageFirstAzimuthTime = imageFirstAzimuthTime;
  m_ImageAzimuthTimeSpacing = imageAzimuthTimeSpacing;
  m_ImageFirstSlantRange = imageFirstSlantRange;
  m_ImageSlantRangeSpacing = imageSlantRangeSpacing;
  m_LUTAzimuthTimes = std::move(lutAzimuthTimes);
  m_LUTSlantRanges = std::move(lutSlantRanges);
  m_LUTValues = std::move(lutValues);

  const auto expectedSize = m_LUTAzimuthTimes.size() * m_LUTSlantRanges.size();
  if (expectedSize != m_LUTValues.size())
  {
    otbGenericExceptionMacro(itk::ExceptionObject,
                             << "Inconsistent NISAR LUT sizes: expected "
                             << expectedSize << " values, got " << m_LUTValues.size());
  }
}

std::size_t NISARCalibrationLookupData::GetValueIndex(std::size_t row, std::size_t col) const
{
  return row * m_LUTSlantRanges.size() + col;
}

double NISARCalibrationLookupData::GetValue(const IndexValueType x, const IndexValueType y) const
{
  if (m_LUTAzimuthTimes.empty() || m_LUTSlantRanges.empty() || m_LUTValues.empty())
  {
    return 1.0;
  }

  const auto azimuthTime = m_ImageFirstAzimuthTime + static_cast<double>(y) * m_ImageAzimuthTimeSpacing;
  const auto slantRange = m_ImageFirstSlantRange + static_cast<double>(x) * m_ImageSlantRangeSpacing;

  const auto az = GetInterpolationIndex(m_LUTAzimuthTimes, azimuthTime);
  const auto rg = GetInterpolationIndex(m_LUTSlantRanges, slantRange);

  const auto v00 = static_cast<double>(m_LUTValues[GetValueIndex(az.lower, rg.lower)]);
  const auto v01 = static_cast<double>(m_LUTValues[GetValueIndex(az.lower, rg.upper)]);
  const auto v10 = static_cast<double>(m_LUTValues[GetValueIndex(az.upper, rg.lower)]);
  const auto v11 = static_cast<double>(m_LUTValues[GetValueIndex(az.upper, rg.upper)]);

  const auto vx0 = (1.0 - rg.fraction) * v00 + rg.fraction * v01;
  const auto vx1 = (1.0 - rg.fraction) * v10 + rg.fraction * v11;
  return (1.0 - az.fraction) * vx0 + az.fraction * vx1;
}

void NISARCalibrationLookupData::ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix) const
{
  kwl.insert({prefix + "Sensor", "NISAR"});
  kwl.insert({prefix + "Type", boost::lexical_cast<std::string>(this->GetType())});
  kwl.insert({prefix + "ImageFirstAzimuthTime", ToStringWithPrecision(m_ImageFirstAzimuthTime)});
  kwl.insert({prefix + "ImageAzimuthTimeSpacing", ToStringWithPrecision(m_ImageAzimuthTimeSpacing)});
  kwl.insert({prefix + "ImageFirstSlantRange", ToStringWithPrecision(m_ImageFirstSlantRange)});
  kwl.insert({prefix + "ImageSlantRangeSpacing", ToStringWithPrecision(m_ImageSlantRangeSpacing)});
  kwl.insert({prefix + "LUTAzimuthTimes", ScalarVectorToString(m_LUTAzimuthTimes)});
  kwl.insert({prefix + "LUTSlantRanges", ScalarVectorToString(m_LUTSlantRanges)});
  kwl.insert({prefix + "LUTHeight", boost::lexical_cast<std::string>(m_LUTAzimuthTimes.size())});
  kwl.insert({prefix + "LUTWidth", boost::lexical_cast<std::string>(m_LUTSlantRanges.size())});
  kwl.insert({prefix + "LUTValues", ScalarVectorToString(m_LUTValues)});
}

void NISARCalibrationLookupData::FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix)
{
  Superclass::FromKeywordlist(kwl, prefix);

  m_ImageFirstAzimuthTime = std::stod(kwl.at(prefix + "ImageFirstAzimuthTime"));
  m_ImageAzimuthTimeSpacing = std::stod(kwl.at(prefix + "ImageAzimuthTimeSpacing"));
  m_ImageFirstSlantRange = std::stod(kwl.at(prefix + "ImageFirstSlantRange"));
  m_ImageSlantRangeSpacing = std::stod(kwl.at(prefix + "ImageSlantRangeSpacing"));
  m_LUTAzimuthTimes = StringToScalarVector<double>(kwl.at(prefix + "LUTAzimuthTimes"));
  m_LUTSlantRanges = StringToScalarVector<double>(kwl.at(prefix + "LUTSlantRanges"));
  m_LUTValues = StringToScalarVector<float>(kwl.at(prefix + "LUTValues"));

  const auto lutHeight = static_cast<std::size_t>(std::stoul(kwl.at(prefix + "LUTHeight")));
  const auto lutWidth = static_cast<std::size_t>(std::stoul(kwl.at(prefix + "LUTWidth")));
  if (lutHeight != m_LUTAzimuthTimes.size() || lutWidth != m_LUTSlantRanges.size())
  {
    otbGenericExceptionMacro(itk::ExceptionObject,
                             << "Inconsistent NISAR LUT metadata dimensions while reading keywordlist.");
  }

  const auto expectedSize = lutHeight * lutWidth;
  if (expectedSize != m_LUTValues.size())
  {
    otbGenericExceptionMacro(itk::ExceptionObject,
                             << "Inconsistent NISAR LUT value count while reading keywordlist.");
  }
}

} // namespace otb
