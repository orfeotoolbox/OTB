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

#ifndef ossimDateTime_h
#define ossimDateTime_h

#define OTB_USE_BOOST_TIME 0

#if OTB_USE_BOOST_TIME

#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/config.hpp>

#include "otbOperatorUtilities.h"

// boost::posix_time::time_duration doesn't have a sufficient precision to
// store things such an azimuth time interval, and yet, this IS a duration.
// Hence this new class injected into boost namespace to emulate a duration
// with precision behind the microsecond.
namespace boost { namespace posix_time {
  class precise_duration;
  double ratio_(precise_duration const& lhs, precise_duration const& rhs);

  class precise_duration
      : private boostAdapter::addable<precise_duration>
      , private boostAdapter::substractable<precise_duration>
      , private boostAdapter::streamable<precise_duration>
      , private boostAdapter::multipliable2<precise_duration, double>
      , private boostAdapter::dividable<precise_duration, double>
      , private boostAdapter::equality_comparable<precise_duration>
      , private boostAdapter::less_than_comparable<precise_duration>
      , private boostAdapter::addable<ptime, precise_duration>
      , private boostAdapter::substractable<ptime, precise_duration>
      {
      public:
         typedef double scalar_type;

         /**@name Construction/destruction
         */
         //@{
         /** Initialization constructor.
         */
         precise_duration() {} // = default;
         explicit precise_duration(double usec_frac)
            : m_usec_frac(usec_frac) {}
         precise_duration(time_duration const& d)
            : m_usec_frac(d.total_microseconds()) {}
         //@}

         double total_seconds() const {
            return total_microseconds() / 1000000.;
         }
         double total_microseconds() const {
            return m_usec_frac;
         }
         bool is_negative() const { return total_microseconds() < 0.0; }
         precise_duration invert_sign() { return precise_duration(- total_seconds()); }
         std::ostream & display(std::ostream & os) const { return os << m_usec_frac; }
         std::istream & read   (std::istream & is)       { return is >> m_usec_frac; }

      protected:

         /**@name Operations
         */
         //@{
         void add(precise_duration const& rhs) { m_usec_frac += rhs.total_microseconds(); }
         void sub(precise_duration const& rhs) { m_usec_frac -= rhs.total_microseconds(); }
         void mult(scalar_type coeff)          { m_usec_frac *= coeff; }
         void div(scalar_type coeff)           { assert(coeff && "Cannot divide by 0"); m_usec_frac /= coeff; }
         friend precise_duration& operator+=(precise_duration & u, precise_duration const& v) {
            u.add(v);
            return u;
         }
         friend ptime& operator+=(ptime & u, precise_duration const& v) {
            const time_duration d = microseconds(static_cast<int> (floor(v.total_microseconds()+0.5)));
            u += d;
            return u;
         }
         friend precise_duration& operator-=(precise_duration & u, precise_duration const& v) {
            u.sub(v);
            return u;
         }
         friend ptime& operator-=(ptime & u, precise_duration const& v) {
            const time_duration d = microseconds(static_cast<int>( floor(v.total_microseconds()+0.5)));
            u -= d;
            return u;
         }

         template <typename U, typename V> static U diff(V const& lhs, V const& rhs) {
            U const res(lhs.total_microseconds() - rhs.total_microseconds());
            return res;
         }

         friend precise_duration& operator*=(precise_duration & u, scalar_type const& v) {
            u.mult(v);
            return u;
         }
         friend precise_duration& operator/=(precise_duration & u, scalar_type const& v) {
            u.div(v);
            return u;
         }

         friend bool operator<(precise_duration const& lhs, precise_duration const& rhs) {
            return lhs.total_microseconds() < rhs.total_microseconds();
         }
         friend bool operator==(precise_duration const& lhs, precise_duration const& rhs) {
            return lhs.total_microseconds() == rhs.total_microseconds();
         }

      public:
         friend scalar_type ratio_(precise_duration const& lhs, precise_duration const& rhs)
         { return lhs.total_microseconds() / rhs.total_microseconds(); }

         //@}
      private:
         double m_usec_frac;
      };

   time_duration abs(time_duration d);

} 
} // boost::time namespaces

namespace otb
{
namespace MetaData
{
using TimeType = boost::posix_time::ptime;
using DurationType = boost::posix_time::precise_duration;

DurationType seconds(double);

TimeType ReadFormattedDate(const std::string & dateStr, const std::string & format = "%Y-%m-%dT%H:%M:%S%F");

}
}

#else //OTB_USE_BOOST_TIME

#include "otbOperatorUtilities.h"
#include <cassert>
#include <iomanip>

namespace otb
{
namespace MetaData
{
namespace details
{
class DayFrac
{
  public:
    typedef double scalar_type;
    // DayFrac(DayFrac const&) = default;
    // DayFrac(DayFrac &&) = default;
         // DayFrac& operator=(DayFrac const&) = default;
         // DayFrac& operator=(DayFrac &&) = default;
    double as_day_frac() const { return m_day_frac; }

    std::ostream & display(std::ostream & os) const { return os << m_day_frac; }
    std::istream & read   (std::istream & is)       { return is >> m_day_frac; }

      protected:
         /**@name Construction/destruction
         */
         //@{
         /** Initialization constructor.
         */
         explicit DayFrac() = default;
         explicit DayFrac(double day_frac) : m_day_frac(day_frac) {}
         /** Protected destructor.
         */
         ~DayFrac() = default;
         //@}

         /**@name Operations
         */
         //@{
         void add(DayFrac const& rhs) { m_day_frac += rhs.m_day_frac; }
         void sub(DayFrac const& rhs) { m_day_frac -= rhs.m_day_frac; }
         void mult(scalar_type coeff) { m_day_frac *= coeff; }
         void div(scalar_type coeff)  { assert(coeff && "Cannot divide by 0"); m_day_frac /= coeff; }
         template <typename V> friend scalar_type ratio_(V const& lhs, V const& rhs)
         { return lhs.as_day_frac() / rhs.as_day_frac(); }

         template <typename U, typename V> friend U& operator+=(U & u, V const& v) {
            u.add(v);
            return u;
         }
         template <typename U, typename V> friend U& operator-=(U & u, V const& v) {
            u.sub(v);
            return u;
         }

         template <typename U, typename V> static U diff(V const& lhs, V const& rhs) {
            U const res(lhs.as_day_frac() - rhs.as_day_frac());
            return res;
         }

         template <typename U> friend U& operator*=(U & u, scalar_type const& v) {
            u.mult(v);
            return u;
         }
         template <typename U> friend U& operator/=(U & u, scalar_type const& v) {
            u.div(v);
            return u;
         }

         template <typename T> friend bool operator<(T const& lhs, T const& rhs) {
            return lhs.as_day_frac() < rhs.as_day_frac();
         }
         template <typename T> friend bool operator==(T const& lhs, T const& rhs) {
            return lhs.as_day_frac() == rhs.as_day_frac();
         }
         //@}
      private:
         double m_day_frac;
      };
   }

   /**
    * Duration abstraction.
    *
    * Values of this class represent time interval.
    *
    * <p><b>Semantics</b><br>
    * <li> Value, mathematical: it's relative position
    * <li> Time interval
    *
    * @see \c std::duration<>
    */
   class Duration
      : public details::DayFrac
      , private boostAdapter::addable<Duration>
      , private boostAdapter::substractable<Duration>
      , private boostAdapter::streamable<Duration>
      , private boostAdapter::multipliable2<Duration, double>
      , private boostAdapter::dividable<Duration, details::DayFrac::scalar_type>
      , private boostAdapter::equality_comparable<Duration>
      , private boostAdapter::less_than_comparable<Duration>
      {
      public:
         typedef details::DayFrac::scalar_type scalar_type;

         /**@name Construction/destruction
         */
         //@{
         /** Initialization constructor.
         */
         Duration() = default;
         explicit Duration(double day_frac)
            : details::DayFrac(day_frac) {}
         //@}

         double total_seconds() const {
            return as_day_frac() * 24 * 60 * 60;
         }
         double total_microseconds() const {
            return total_seconds() * 1000 * 1000;
         }
         bool is_negative() const { return as_day_frac() < 0.0; }
         Duration invert_sign() { return Duration(- as_day_frac()); }
         friend Duration abs(Duration const& d) { return Duration(std::abs(d.as_day_frac())); }
      };

   /**
    * Modified JulianDate abstraction.
    *
    * Objects of this class represent points in time.
    *
    * <p><b>Semantics</b><br>
    * <li> Value, mathematical: it's an absolute position
    * <li> Point in time
    * @see \c std::time_point<>
    */
   class ModifiedJulianDate
      : public details::DayFrac
      , private boostAdapter::addable<ModifiedJulianDate, Duration>
      , private boostAdapter::substractable<ModifiedJulianDate, Duration>
      , private boostAdapter::substractable_asym<Duration, ModifiedJulianDate>
      , private boostAdapter::streamable<ModifiedJulianDate>
      , private boostAdapter::equality_comparable<ModifiedJulianDate>
      , private boostAdapter::less_than_comparable<ModifiedJulianDate>
      {
      public:
         typedef details::DayFrac::scalar_type scalar_type;

         /**@name Construction/destruction
         */
         //@{
         /** Initialization constructor.
         */
         ModifiedJulianDate() = default;
         explicit ModifiedJulianDate(double day_frac)
            : details::DayFrac(day_frac) {}
         //@}
         using details::DayFrac::diff;
      };

   ModifiedJulianDate toModifiedJulianDate(std::string const& utcTimeString);
   inline Duration microseconds(double us) {
      return Duration(us / (24ULL * 60 * 60 * 1000 * 1000));
  }
  inline Duration seconds(double us) {
    return Duration(us / (24ULL * 60 * 60));
  }

  ModifiedJulianDate ReadFormattedDate(const std::string & dateStr, const std::string & format = "%Y-%m-%dT%H:%M:%S%F");

  using TimeType = ModifiedJulianDate;
  using DurationType = Duration;
}
}

#endif


#endif
