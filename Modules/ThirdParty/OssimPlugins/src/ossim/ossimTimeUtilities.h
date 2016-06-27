//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL-2
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimTimeUtilities_h
#define ossimTimeUtilities_h

#include "ossimStringUtilities.h"
#include "ossimOperatorUtilities.h"
#include <cassert>
#include <ostream>
class ossimDate;

namespace ossimplugins { namespace time {
   // class ModifiedJulianDate;
   // class Duration;
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
         /** Initialisation constructor.
         */
         explicit DayFrac() {} // = default;
         explicit DayFrac(double day_frac) : m_day_frac(day_frac) {}
         /** Protected destructor.
         */
         ~DayFrac() {}// = default;
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
      , private addable<Duration>
      , private substractable<Duration>
      , private streamable<Duration>
      , private multipliable2<Duration, double>
      , private dividable<Duration, details::DayFrac::scalar_type>
      , private equality_comparable<Duration>
      , private less_than_comparable<Duration>
      {
      public:
         typedef details::DayFrac::scalar_type scalar_type;

         /**@name Construction/destruction
         */
         //@{
         /** Initialisation constructor.
         */
         Duration() {} // = default;
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
      , private addable<ModifiedJulianDate, Duration>
      , private substractable<ModifiedJulianDate, Duration>
      , private substractable_asym<Duration, ModifiedJulianDate>
      , private streamable<ModifiedJulianDate>
      , private equality_comparable<ModifiedJulianDate>
      , private less_than_comparable<ModifiedJulianDate>
      {
      public:
         typedef details::DayFrac::scalar_type scalar_type;

         /**@name Construction/destruction
         */
         //@{
         /** Initialisation constructor.
         */
         ModifiedJulianDate() {} // = default;
         explicit ModifiedJulianDate(double day_frac)
            : details::DayFrac(day_frac) {}
         //@}
         using details::DayFrac::diff;
      };

   ModifiedJulianDate toModifiedJulianDate(string_view const& utcTimeString);
   inline Duration microseconds(double us) {
      return Duration(us / (24L * 60 * 60 * 1000 * 1000));
   }
   inline Duration seconds(double us) {
      return Duration(us / (24L * 60 * 60));
   }
   std::string to_simple_string(ModifiedJulianDate const& d);
   std::string to_simple_string(Duration const& d);

   namespace details {
      // strptime is not portable, hence this simplified emulation
      ossimDate strptime(string_view const& format, string_view const& date);
   } // details namespace

} } // ossimplugins namespace::time

#if defined(USE_BOOST_TIME)
#  include <boost/config.hpp>
#  include <boost/date_time/posix_time/posix_time.hpp>
// boost::posix_time::time_duration doesn't have a sufficient precision to
// store things such an azimuth time interval, and yet, this IS a duration.
// Hence this new class injected into boost namespace to emulate a duration
// with precision behind the microsecond.
// TODO:
// - check whether we could have used another boost date
// - move this elsewhere
// - move this into another namespace
namespace boost { namespace posix_time {
   class precise_duration;
   double ratio_(precise_duration const& lhs, precise_duration const& rhs);

   class precise_duration
      : private ossimplugins::addable<precise_duration>
      , private ossimplugins::substractable<precise_duration>
      , private ossimplugins::streamable<precise_duration>
      , private ossimplugins::multipliable2<precise_duration, double>
      , private ossimplugins::dividable<precise_duration, double>
      , private ossimplugins::equality_comparable<precise_duration>
      , private ossimplugins::less_than_comparable<precise_duration>
      , private ossimplugins::addable<ptime, precise_duration>
      , private ossimplugins::substractable<ptime, precise_duration>
      {
      public:
         typedef double scalar_type;

         /**@name Construction/destruction
         */
         //@{
         /** Initialisation constructor.
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
            const time_duration d = microseconds(floor(v.total_microseconds()+0.5));
            u += d;
            return u;
         }
         friend precise_duration& operator-=(precise_duration & u, precise_duration const& v) {
            u.sub(v);
            return u;
         }
         friend ptime& operator-=(ptime & u, precise_duration const& v) {
            const time_duration d = microseconds(floor(v.total_microseconds()+0.5));
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

} } // boost::time namespaces
#endif

#endif // ossimTimeUtilities_h
