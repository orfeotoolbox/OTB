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

#include "otbDateTime.h"
#include "date.h"

namespace otb
{
namespace MetaData
{

TimePoint ReadFormattedDate(const std::string & dateStr, const std::string & format)
{
  std::istringstream is(dateStr);
  TimePoint tp;
  tp.Read(is, format.c_str());
  return tp;
}

std::ostream & TimePoint::Display(std::ostream & os, const std::string& format) const 
{
  date::to_stream(os, format.c_str(), m_Time);
  return os;
}

std::istream & TimePoint::Read(std::istream & is, const std::string & format)
{
  date::from_stream(is, format.c_str(), m_Time);
  return is;
}

double Duration::TotalSeconds() const
{
  return m_Duration.count() * details::internalPeriod;
}

Duration::InternalDurationType::rep Duration::NumberOfTicks() const
{
  return m_Duration.count();
}

std::ostream & Duration::Display(std::ostream & os) const 
{
  return os << m_Duration.count() * details::internalPeriod;
}

std::istream & Duration::Read(std::istream & is)
{
  double s;
  is >> s;
  m_Duration = InternalDurationType(static_cast<long long>(std::round(s * 1e9)));
  return is;
}

Duration Abs(Duration d)
{
  if(d.m_Duration.count() < 0)
    d.m_Duration = -d.m_Duration;
  return d;
}

double Ratio(const Duration & lhs, const Duration & rhs)
{
  return static_cast<double>(lhs.NumberOfTicks()) / rhs.NumberOfTicks();
}

DurationType seconds(double input)
{
  return DurationType::Seconds(input);
}

} // namespace otb
} // namespace MetaData