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

#ifndef otbDateTime_h
#define otbDateTime_h

#include "OTBMetadataExport.h"

#include <cmath>
#include <boost/operators.hpp>
#include <chrono>
#include <string>

namespace otb
{
namespace MetaData
{

namespace details
{
using InternalDurationType = std::chrono::nanoseconds;
using InternalTimePointType = std::chrono::time_point<std::chrono::system_clock, InternalDurationType>;

// the number of second's fractions per tick of InternalDurationType
constexpr double internalPeriod = static_cast<double>(InternalDurationType::period::num) / InternalDurationType::period::den;

// Standard format used when parsing dates
const std::string timePointFormat = "%Y-%m-%dT%H:%M:%S";

// Add some operators not defined in boost

// division by a scalar and ratio
template <typename T, typename R> struct dividable 
{
  typedef R scalar_type;
  friend T operator/(T lhs, scalar_type const& rhs)
  {
    lhs /= rhs;
    return lhs;
  }
  friend scalar_type operator/(T const& lhs, T const& rhs)
  {
    return Ratio(lhs, rhs);
  }
};

template <typename U, typename V> struct substractable_asym
{
  friend U operator-(V const& lhs, V const& rhs) 
  {
    return V::template diff<U,V>(lhs, rhs);
  }
};

template <typename T> struct streamable
{
  friend std::ostream & operator<<(std::ostream & os, const T & v)
  {
    return v.Display(os);
  }
  friend std::istream & operator>>(std::istream & is, T & v)
  {
    return v.Read(is);
  }
};
} // namespace details


class Duration;

/** \class TimePoint
 *
 * \brief Represents a point in Time
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT TimePoint : private details::streamable<TimePoint>,
                                     private details::substractable_asym<Duration, TimePoint>,
                                     private boost::equality_comparable<TimePoint>,
                                     private boost::less_than_comparable<TimePoint>
{
public:
  using InternalTimePointType = details::InternalTimePointType;

  TimePoint(): m_Time(InternalTimePointType::min()) {}
  TimePoint(InternalTimePointType const& t): m_Time(t) {}

  std::ostream & Display(std::ostream & os) const;
  std::istream & Read   (std::istream & is, const std::string & format = details::timePointFormat);

  template <typename U, typename V> static U diff(V const& lhs, V const& rhs) 
  {
    U const res(lhs.m_Time - rhs.m_Time);
    return res;
  }

  friend bool operator < (TimePoint const& lhs, TimePoint const& rhs)
  {
    return lhs.m_Time < rhs.m_Time;
  }
  
  friend bool operator == (TimePoint const& lhs, TimePoint const& rhs)
  {
    return lhs.m_Time == rhs.m_Time;
  }

  /** Return the julian day corresponding to the time point */
  double GetJulianDay() const;

  /** Return the modified julian day corresponding to the time point */
  double GetModifiedJulianDay() const;

  /** Return the year corresponding to the time point */
  int GetYear() const;

  /** Return the month of the year corresponding to the time point (from 1 to 12) */
  unsigned int GetMonth() const;

  /** Return the calendar day corresponding to the time point (from 1 to 31) */
  unsigned int GetDay() const;

  /** Return the hour of the day corresponding to the time point (from 0 to 23) */
  unsigned int GetHour() const;

  /** Return the minute of the hour of the day corresponding to the time point (from 0 to 59) */
  unsigned int GetMinute() const;

  /** Return the fractional second of the minute of the hour of the day corresponding to the time point (from 0 and inferior to 60) */
  double GetSecond() const;

  friend TimePoint& operator+=(TimePoint & u, Duration const& v);
  friend TimePoint& operator-=(TimePoint & u, Duration const& v);

private:
  InternalTimePointType m_Time;

};

/** \class Duration
 *
 * \brief Represents a duration
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT Duration : private details::streamable<Duration>,
                                    private boost::addable<Duration>,
                                    private boost::subtractable<Duration>,
                                    private boost::multipliable2<Duration, double>,
                                    private details::dividable<Duration, double>,
                                    private boost::equality_comparable<Duration>,
                                    private boost::less_than_comparable<Duration>,
                                    private boost::addable<TimePoint, Duration>,
                                    private boost::subtractable<TimePoint, Duration>
{
public:
  using InternalDurationType = details::InternalDurationType;

  Duration() = default;
  Duration(InternalDurationType const& d): m_Duration(d) {}

  static Duration Seconds(double s)
  {
    return Duration(InternalDurationType(static_cast<InternalDurationType::rep>(std::round(s / details::internalPeriod))));
  }

  static Duration Milliseconds(double ms)
  {
    return Duration(InternalDurationType(static_cast<InternalDurationType::rep>(std::round(ms / details::internalPeriod * 1e-3))));
  }

  static Duration Microseconds(double us)
  {
    return Duration(InternalDurationType(static_cast<InternalDurationType::rep>(std::round(us / details::internalPeriod * 1e-6))));
  }

  static Duration Nanoseconds(double ns)
  {
    return Duration(InternalDurationType(static_cast<InternalDurationType::rep>(std::round(ns / details::internalPeriod * 1e-9))));
  }

  double TotalSeconds() const;

  InternalDurationType::rep NumberOfTicks() const;

  friend Duration& operator+=(Duration & u, Duration const& v)
  {
    u.m_Duration += v.m_Duration;
    return u;
  }

  friend Duration& operator-=(Duration & u, Duration const& v)
  {
    u.m_Duration -= v.m_Duration;
    return u;
  }

  friend Duration& operator*=(Duration & u, double v)
  {
    // note: don't use std::chrono::duration::operator* cause it multiplies the input by an integer value.
    u.m_Duration = InternalDurationType(static_cast<InternalDurationType::rep>(std::round(u.m_Duration.count() * v)));
    return u;
  }

  friend Duration& operator/=(Duration & u, double v)
  {
    // note: don't use std::chrono::duration::operator/ cause it divides the input by an integer value.
    u.m_Duration = InternalDurationType(static_cast<InternalDurationType::rep>(std::round(u.m_Duration.count() / v)));
    return u;
  }

  friend bool operator < (Duration const& lhs, Duration const& rhs)
  {
    return lhs.m_Duration < rhs.m_Duration;
  }
  
  friend bool operator == (Duration const& lhs, Duration const& rhs)
  {
    return lhs.m_Duration == rhs.m_Duration;
  }
  
  friend TimePoint& operator+=(TimePoint & u, Duration const& v)
  {
    u.m_Time += v.m_Duration;
    return u;
  }

  friend TimePoint& operator-=(TimePoint & u, Duration const& v)
  {
    u.m_Time -= v.m_Duration;
    return u;
  }

  std::ostream & Display(std::ostream & os) const;
  std::istream & Read   (std::istream & is);

  friend OTBMetadata_EXPORT Duration Abs(Duration d);

private:
  InternalDurationType m_Duration;
};

OTBMetadata_EXPORT double Ratio(const Duration & lhs, const Duration & rhs);
OTBMetadata_EXPORT TimePoint ReadFormattedDate(const std::string & dateStr, const std::string & format = details::timePointFormat);

} // namespace otb
} // namespace MetaData

#endif // otbDateTime_h
