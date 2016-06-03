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
      class Seconds
      {
      public:
         typedef double scalar_type;
         // Seconds(Seconds const&) = default;
         // Seconds(Seconds &&) = default;
         // Seconds& operator=(Seconds const&) = default;
         // Seconds& operator=(Seconds &&) = default;
         double as_seconds() const { return m_seconds; }

         std::ostream & display(std::ostream & os) const { return os << m_seconds; }
         std::istream & read   (std::istream & is)       { return is >> m_seconds; }

      protected:
         /**@name Construction/destruction
         */
         //@{
         /** Initialisation constructor.
         */
         explicit Seconds() {} // = default;
         explicit Seconds(double seconds) : m_seconds(seconds) {}
         /** Protected destructor.
         */
         ~Seconds() {}// = default;
         //@}

         /**@name Operations
         */
         //@{
         void add(Seconds const& rhs) { m_seconds += rhs.m_seconds; }
         void sub(Seconds const& rhs) { m_seconds -= rhs.m_seconds; }
         void mult(scalar_type coeff) { m_seconds *= coeff; }
         void div(scalar_type coeff)  { assert(coeff && "Cannot divide by 0"); m_seconds /= coeff; }
         template <typename V> friend scalar_type ratio(V const& lhs, V const& rhs)
         { return lhs.as_seconds() / rhs.as_seconds(); }

         template <typename U, typename V> friend U& operator+=(U & u, V const& v) {
            u.add(v);
            return u;
         }
         template <typename U, typename V> friend U& operator-=(U & u, V const& v) {
            u.sub(v);
            return u;
         }

         template <typename U, typename V> static U diff(V const& lhs, V const& rhs) {
            U const res(lhs.as_seconds() - rhs.as_seconds());
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
            return lhs.as_seconds() < rhs.as_seconds();
         }
         template <typename T> friend bool operator==(T const& lhs, T const& rhs) {
            return lhs.as_seconds() == rhs.as_seconds();
         }
         //@}
      private:
         double m_seconds;
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
      : public details::Seconds
      , private addable<Duration>
      , private substractable<Duration>
      , private streamable<Duration>
      , private multipliable2<Duration, double>
      , private dividable<Duration, details::Seconds::scalar_type>
      , private equality_comparable<Duration>
      , private less_than_comparable<Duration>
      {
      public:
         typedef details::Seconds::scalar_type scalar_type;

         /**@name Construction/destruction
         */
         //@{
         /** Initialisation constructor.
         */
         Duration() {} // = default;
         explicit Duration(double seconds)
            : details::Seconds(seconds) {}
         //@}

         double total_microseconds() const {
            return as_seconds() * 1000 * 1000;
         }
         bool is_negative() const { return as_seconds() < 0.0; }
         Duration invert_sign() { return Duration(- as_seconds()); }
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
      : public details::Seconds
      , private addable<ModifiedJulianDate, Duration>
      , private substractable<ModifiedJulianDate, Duration>
      , private substractable_asym<Duration, ModifiedJulianDate>
      , private streamable<ModifiedJulianDate>
      , private equality_comparable<ModifiedJulianDate>
      , private less_than_comparable<ModifiedJulianDate>
      {
      public:
         typedef details::Seconds::scalar_type scalar_type;

         /**@name Construction/destruction
         */
         //@{
         /** Initialisation constructor.
         */
         ModifiedJulianDate() {} // = default;
         explicit ModifiedJulianDate(double seconds)
            : details::Seconds(seconds) {}
         //@}
         using details::Seconds::diff;
      };


   ModifiedJulianDate toModifiedJulianDate(string_view const& utcTimeString);
   inline Duration microseconds(double us) {
      return Duration(us / (1000 * 1000));
   }
   std::string to_simple_string(ModifiedJulianDate const& d);
   std::string to_simple_string(Duration const& d);

   namespace details {
      // strptime is not portable, hence this simplified emulation
      ossimDate strptime(string_view const& format, string_view const& date);
   } // details namespace

} } // ossimplugins namespace::time

#endif // ossimTimeUtilities_h
