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

#include "otbDateTime.h"
#include "date.h"
#include "otbMissingMetadataException.h"

namespace otb
{
namespace MetaData
{

TimePoint ReadFormattedDate(const std::string & dateStr, const std::string & format)
{
  std::istringstream is(dateStr);
  is.exceptions(std::istringstream::failbit | std::istringstream::badbit);
  TimePoint tp;
  
  try
  {
    tp.Read(is, format);
  }
  catch (const std::istringstream::failure &)
  {
    otbGenericExceptionMacro(otb::MissingMetadataException,
            << "Cannot parse the input date "
            << dateStr
            << " with input format "
            << format);
  }
  return tp;
}

std::ostream & TimePoint::Display(std::ostream & os) const 
{
  auto dateStr = date::format( "%Y-%m-%dT%H:%M:%S", m_Time);

  // date::format %S returns the number of seconds with a fixed size format (ns precision)
  // Remove the trailing zeros in the output.
  dateStr.erase(dateStr.find_last_not_of('0') + 1, std::string::npos);

  // Remove the decimal part separator for time points with an integer number of seconds
  if (dateStr.back() == '.')
  {
    dateStr.pop_back();
  }

  // Add the UTC time identifier
  os << dateStr << "Z";
  return os;
}

std::istream & TimePoint::Read(std::istream & is, const std::string & format)
{
  date::from_stream(is, format.c_str(), m_Time);

  // Remove the UTC time identifier from the input istream, if it exists.
  if (!is.eof() && is.peek() == 'Z')
  {
    is.get();
  }

  return is;
}

double TimePoint::GetJulianDay() const
{
  // 86400 is the number of seconds in a day, 2440587.5 is the offset in days between the UnixTime origin and the Julian day origin
  return (m_Time.time_since_epoch().count() * details::internalPeriod / 86400.0  + 2440587.5);
}

double TimePoint::GetModifiedJulianDay() const
{
  // 86400 is the number of seconds in a day, 40587 is the offset in days between the UnixTime origin and the modified Julian day origin
  return (m_Time.time_since_epoch().count() * details::internalPeriod / 86400.0 + 40587);
}

date::year_month_day ToYMD(details::InternalTimePointType in)
{
  auto dp = date::floor<date::days>(in);
  return date::year_month_day(dp);
}

date::hh_mm_ss<details::InternalDurationType> ToHMS(details::InternalTimePointType in)
{
  auto dp = date::floor<date::days>(in);
  return date::make_time(std::chrono::duration_cast<details::InternalDurationType>(in-dp));
}

int TimePoint::GetYear() const
{
  return static_cast<int>(ToYMD(m_Time).year());
}

unsigned int TimePoint::GetMonth() const
{
  return static_cast<unsigned int>(ToYMD(m_Time).month());
}

unsigned int TimePoint::GetDay() const
{
  return static_cast<unsigned int>(ToYMD(m_Time).day());
}

unsigned int TimePoint::GetHour() const
{
  return ToHMS(m_Time).hours().count();
}

unsigned int TimePoint::GetMinute() const
{
  return ToHMS(m_Time).minutes().count();
}

double TimePoint::GetSecond() const
{
  return ToHMS(m_Time).seconds().count() + ToHMS(m_Time).subseconds().count() * details::internalPeriod;
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

} // namespace otb
} // namespace MetaData