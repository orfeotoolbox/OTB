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

#ifndef otbDateTime_h
#define otbDateTime_h

// Use nanosecond precision in boost dates and durations
#define BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG
#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/config.hpp>
#include <boost/operators.hpp>

namespace otb
{
namespace MetaData
{

namespace details
{
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
    return ratio_(lhs, rhs);
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

class TimePoint : private details::streamable<TimePoint>,
                  private details::substractable_asym<Duration, TimePoint>,
                  private boost::equality_comparable<TimePoint>,
                  private boost::less_than_comparable<TimePoint>
{
public:
  //friend class Duration;
  TimePoint() = default;

  std::ostream & Display(std::ostream & os) const { return os << m_Time; }
  std::istream & Read   (std::istream & is)       { return is >> m_Time; }

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


public:
  boost::posix_time::ptime m_Time;
};

using TimeType = TimePoint;

double ratio_(Duration const& lhs, Duration const& rhs);

class Duration : private details::streamable<Duration>,
                 private boost::addable<Duration>,
                 private boost::subtractable<Duration>,
                 private boost::multipliable2<Duration, double>,
                 private details::dividable<Duration, double>,
                 private boost::equality_comparable<Duration>,
                 private boost::less_than_comparable<Duration>,
                 private boost::addable<TimeType, Duration>,
                 private boost::subtractable<TimeType, Duration>
{
public:
  using InternalDurationType = boost::posix_time::time_duration;

  Duration() = default;
  Duration(InternalDurationType const& d): m_Duration(d) {}


  static Duration Seconds(double d) 
  {
    return Duration(boost::posix_time::nanoseconds(static_cast<long long>(std::round(d * 1e9))));
  }

  static Duration Nanoseconds(double d) 
  {
    return Duration(boost::posix_time::nanoseconds(static_cast<long long>(std::round(d))));
  }

  Duration(double d): m_Duration(boost::posix_time::nanoseconds(static_cast<long long>(std::round(d)))) {}

  double TotalNanoseconds() const
  {
    return m_Duration.total_nanoseconds();
  }

  double TotalSeconds() const
  {
    return m_Duration.total_nanoseconds() * 1e9;
  }

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
    u.m_Duration = boost::posix_time::nanoseconds(static_cast<long>(std::round(
                            u.m_Duration.total_nanoseconds() * v)));
    return u;
  }


  friend Duration& operator/=(Duration & u, double v)
  {
    u.m_Duration = boost::posix_time::nanoseconds(static_cast<long>(std::round(
                            u.m_Duration.total_nanoseconds() / v)));
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
  

  friend TimeType& operator+=(TimeType & u, Duration const& v)
  {
    u.m_Time += v.m_Duration;
    return u;
  }

  friend TimeType& operator-=(TimeType & u, Duration const& v)
  {
    u.m_Time -= v.m_Duration;
    return u;
  }

  std::ostream & Display(std::ostream & os) const { return os << m_Duration; }
  std::istream & Read   (std::istream & is)       { return is >> m_Duration; }

  friend Duration Abs(Duration d);

private:
  InternalDurationType m_Duration;
};

Duration Abs(Duration d);


using DurationType = Duration;

DurationType seconds(double);

TimeType ReadFormattedDate(const std::string & dateStr, const std::string & format = "%Y-%m-%dT%H:%M:%S%F");

}
}


#endif
