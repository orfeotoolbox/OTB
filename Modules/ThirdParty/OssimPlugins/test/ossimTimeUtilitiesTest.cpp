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
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
// #pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wshadow"
#endif

#define BOOST_TEST_MODULE "ossim Time Utilities unit testing"
#define BOOST_TEST_DYN_LINK
#include "ossimTimeUtilities.h"
#include "ossimStringUtilities.h"
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdio>
#include <ossim/base/ossimDate.h>
#include <ossim/base/ossimString.h>
#include <sstream>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

using namespace ossimplugins;

double
getModifiedJulianDate(ossimString const& utcTimeString)
{
   std::vector<ossimString> date_time;
   std::vector<ossimString> output;

   utcTimeString.split(date_time, "T");

   date_time[0].split(output, "-");

   if (output.size () != 3)
   {
      throw std::runtime_error(("Invalid date" + utcTimeString).string());
   }

   char* stop;
   int year = std::strtol( output[0].c_str(), &stop, 10);
   if ( *stop != 0 )
   {
      throw std::runtime_error(("Invalid year" + utcTimeString).string());
   }

   stop  = 0;
   int month = std::strtol( output[1].c_str(), &stop, 10);
   if ( *stop != 0 )
   {
      throw std::runtime_error(("Invalid month" + utcTimeString).string());
   }
   stop  = 0;
   int day = std::strtol( output[2].c_str(), &stop, 10);
   if ( *stop != 0 )
   {
      throw std::runtime_error(("Invalid day" + utcTimeString).string());
   }
   /* Parse time */
   output.clear();
   date_time[1].split(output, ":");
   if (output.size () != 3)
   {
      throw std::runtime_error(("Invalid time" + utcTimeString).string());
   }

   stop  = 0;
   int hours = std::strtol( output[0].c_str(), &stop, 10);
   if ( *stop != 0 )
   {
      throw std::runtime_error(("Invalid hour" + utcTimeString).string());
   }

   stop  = 0;
   int minutes = std::strtol( output[1].c_str(), &stop, 10);
   if ( *stop != 0 )
   {
      throw std::runtime_error(("Invalid minutes" + utcTimeString).string());
   }

   stop  = 0;
   double seconds = std::strtod( output[2].c_str(), &stop);
   if ( *stop != 0 )
   {
      throw std::runtime_error(("Invalid seconds" + utcTimeString).string());
   }

   /* Below method could replace all above split and convert. But there is a lot of
      checks done in  ossimLocalTm::setIso8601( ) which slows us down. And I dont
      want that. */
   /*
      ossimLocalTm otm;
      if ( !otm.setIso8601(utcString) )
      {
      }
      ossimDate ossimdate(otm);
    */

   ossimDate ossimdate(month, day, year);
   ossimdate.setHour(hours);
   ossimdate.setMin(minutes);
   ossimdate.setFloatSec(seconds);
   return ossimdate.getModifiedJulian();
}

typedef unsigned int uint;
void check_time(char const* format, char const* sDate,
      int year, int month, int day,
      int hour, int min, int sec, double fsec)
{
   // std::cout << "Test " << sDate << " against " << format << std::endl;
   const ossimDate d = time::details::strptime(format, sDate);
   BOOST_CHECK_EQUAL(d.getYear(), year);
   BOOST_CHECK_EQUAL(d.getMonth(), month);
   BOOST_CHECK_EQUAL(d.getDay(), day);
   BOOST_CHECK_EQUAL(d.getHour(), hour);
   BOOST_CHECK_EQUAL(d.getMin(), min);
   BOOST_CHECK_EQUAL(d.getSec(), sec);
   BOOST_CHECK_EQUAL(d.getFractionalSecond(), fsec);

   // Reference
   ossimLocalTm iso;
   iso.setIso8601(sDate);
   BOOST_CHECK_EQUAL(d.getModifiedJulian(), iso.getModifiedJulian());

   // Is ossimDate |--> MJD bijective ?
   const double dMJD = d.getModifiedJulian();
   ossimDate d2; d2.setDateFromModifiedJulian(dMJD);
   BOOST_CHECK_EQUAL(d2.getYear(), year);
   BOOST_CHECK_EQUAL(d2.getMonth(), month);
   BOOST_CHECK_EQUAL(d2.getDay(), day);
   BOOST_CHECK_EQUAL(d2.getHour(), hour);
   BOOST_CHECK_EQUAL(d2.getMin(), min);
   BOOST_CHECK_CLOSE_FRACTION(d2.getSec()+d2.getFractionalSecond(), sec+fsec, 1e-6);

   // Alternative implementation
   BOOST_CHECK_CLOSE_FRACTION(d.getModifiedJulian(), getModifiedJulianDate(sDate), 1e-12);

   // Check string conversion
   // Yes, this is likelly to fail du to double imprecisions
   // - official ossimDate string conversion
   time::ModifiedJulianDate mjd = time::toModifiedJulianDate(sDate);
   // std::cout << "MJD("<<sDate<<"): " << mjd.as_day_frac() << std::endl;
   BOOST_CHECK_CLOSE_FRACTION(d.getModifiedJulian(), mjd.as_day_frac(), 1e-12);
   std::ostringstream oss;
   oss << d.getYear() << '-' << std::setw(2) << std::setfill('0') << d.getMonth() << '-' << d.getDay() << 'T';
   d.printTime(oss);
   if (d.getFractionalSecond() > 0) {
      oss << '.' << std::setw(6) << (d.getFractionalSecond() * 1000*1000);
   }
   BOOST_CHECK_EQUAL(oss.str(), sDate);
   // - our string conversion
   // std::cout << std::setprecision(20) ;
   // std::cout << mjd.as_day_frac() << " -> " << to_simple_string(mjd) << " / " << d.getModifiedJulian() << "\n";
   // std::cout << oss.str() << "\n";

   // We know this test will fail because of float rounding => just display
   std::cout.precision(16);
   std::cout << mjd << " as a simple string (" << time::to_simple_string(mjd)
      << ") is expected to differ from " << sDate << "\n";
   // BOOST_CHECK_EQUAL(to_simple_string(mjd), sDate);
}


BOOST_AUTO_TEST_CASE(check_strptime)
{
   check_time(
         "%Y-%m-%dT%H:%M:%S", "2016-01-12T13:42:52",
         2016, 1, 12, 13, 42, 52, 0.0);

   check_time(
         "%Y-%m-%dT%H:%M:%S", "1970-12-31T00:42:52",
         1970, 12, 31, 0, 42, 52, 0.0);

   check_time(
         "%Y-%m-%dT%H:%M:%S%.", "1970-12-31T00:42:52",
         1970, 12, 31, 0, 42, 52, 0.0);

   check_time(
         "%Y-%m-%dT%H:%M:%S%.", "1970-12-31T00:42:52.156000",
         1970, 12, 31, 0, 42, 52, 0.156);

   check_time(
         "%Y-%m-%dT%H:%M:%S%.", "2015-06-19T19:50:44.223221",
         2015, 6, 19, 19, 50, 44, 0.223221);
}
