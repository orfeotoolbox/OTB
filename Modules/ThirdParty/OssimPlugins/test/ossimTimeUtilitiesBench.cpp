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
//
// Typical results are:
//     Run on (8 X 1998 MHz CPU s)
//     2016-05-19 18:55:03
//     ***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
//     Benchmark                                          Time           CPU Iterations
//     --------------------------------------------------------------------------------
//     BM_decode_string_view/697.936M                  1927 ns       1923 ns     372340
//     BM_decode_string_view/1.36316G                  1906 ns       1904 ns     357143
//     BM_decode_split/697.936M                        4421 ns       4415 ns     157658
//     BM_decode_split/1.36316G                        4404 ns       4400 ns     159091
//     BM_decode_iso/697.936M                         11417 ns      11456 ns      62500
//     BM_decode_iso/1.36316G                         11418 ns      11456 ns      62500
//     BM_decode_strptime_manual_julian/697.936M       1977 ns       1982 ns     357143
//     BM_decode_strptime_manual_julian/1.36316G       1938 ns       1942 ns     364583
//     BM_decode_strptime_ossim_julian/697.936M        2018 ns       2011 ns     350000
//     BM_decode_strptime_ossim_julian/1.36316G        1982 ns       1977 ns     350000
//----------------------------------------------------------------------------
//
#include "ossimTimeUtilities.h"
#include <benchmark/benchmark.h>
#include <vector>
#include <cstdio>
#include <ctime>
#include "ossimStringUtilities.h"
#include <ossim/base/ossimDate.h>
#include <ossim/base/ossimString.h>

using namespace ossimplugins;

static void clobber() {
    asm volatile("" : : : "memory");
}

double
getModifiedJulianDate(ossimString const& utcTimeString)
{
   std::vector<ossimString> date_time;
   std::vector<ossimString> output;

   utcTimeString.split(date_time, "T");

   date_time[0].split(output, "-");

   if (output.size () != 3)
   {
      throw std::runtime_error("Invalid date" + utcTimeString);
   }

   char* stop;
   int year = std::strtol( output[0].c_str(), &stop, 10);
   if ( *stop != 0 )
   {
      throw std::runtime_error("Invalid year" + utcTimeString);
   }

   stop  = 0;
   int month = std::strtol( output[1].c_str(), &stop, 10);
   if ( *stop != 0 )
   {
      throw std::runtime_error("Invalid month" + utcTimeString);
   }
   stop  = 0;
   int day = std::strtol( output[2].c_str(), &stop, 10);
   if ( *stop != 0 )
   {
      throw std::runtime_error("Invalid day" + utcTimeString);
   }
   /* Parse time */
   output.clear();
   date_time[1].split(output, ":");
   if (output.size () != 3)
   {
      throw std::runtime_error("Invalid time" + utcTimeString);
   }

   stop  = 0;
   int hours = std::strtol( output[0].c_str(), &stop, 10);
   if ( *stop != 0 )
   {
      throw std::runtime_error("Invalid hour" + utcTimeString);
   }

   stop  = 0;
   int minutes = std::strtol( output[1].c_str(), &stop, 10);
   if ( *stop != 0 )
   {
      throw std::runtime_error("Invalid minutes" + utcTimeString);
   }

   stop  = 0;
   double seconds = std::strtod( output[2].c_str(), &stop);
   if ( *stop != 0 )
   {
      throw std::runtime_error("Invalid seconds" + utcTimeString);
   }

   /* Below method could replace all above split and convert. But there is a lot of
      checks done in  ossimLocalTm::setIso8601( ) which slows us down. And I don't
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


static const char format[] = "%Y-%m-%dT%H:%M:%S";
static const std::time_t now = std::time(NULL);

template <std::size_t N>
void build_date_string(char (&date)[N], std::time_t t)
{
   std::tm tm;
   localtime_r(&t, &tm);
   std::strftime(date, N, format, &tm);
}

static void BM_decode_string_view(benchmark::State & state)
{
   char date[1024]; // more than enough
   build_date_string(date, state.range_x());

   while (state.KeepRunning())  {
      const time::ModifiedJulianDate d = time::toModifiedJulianDate(date);
      benchmark::DoNotOptimize(d);
      clobber();
   }
}
BENCHMARK(BM_decode_string_view)->Range(now/2, now);

static void BM_decode_split(benchmark::State & state)
{
   char date[1024]; // more than enough
   build_date_string(date, state.range_x());

   while (state.KeepRunning())  {
      const double d = getModifiedJulianDate(date);
      benchmark::DoNotOptimize(d);
      clobber();
   }
}
BENCHMARK(BM_decode_split)->Range(now/2, now);

static void BM_decode_iso(benchmark::State & state)
{
   char date[1024]; // more than enough
   build_date_string(date, state.range_x());

   while (state.KeepRunning())  {
      ossimLocalTm iso;
      iso.setIso8601(date);
      const double d = iso.getModifiedJulian();
      benchmark::DoNotOptimize(d);
      clobber();
   }
}
BENCHMARK(BM_decode_iso)->Range(now/2, now);

static void BM_decode_strptime_manual_julian(benchmark::State & state)
{
   char date[1024]; // more than enough
   build_date_string(date, state.range_x());

   while (state.KeepRunning())  {
      // std::tm tm;
      ossimDate tm;
      strptime(date, format, &tm);
      int J = tm.tm_mon + 1;
      int K = tm.tm_mday;
      int I = tm.tm_year + 1900;
      double d = (K-32075+1461*(I+4800+(J-14)/12)/4+367*(J-2-(J-14)/12*12)
            /12-3*((I+4900+(J-14)/12)/100)/4+
            (tm.tm_hour/24.0)+
            (tm.tm_min/1440.0)+
            ((tm.tm_sec+0.0)/86400.0)) - 2400000.5;
      benchmark::DoNotOptimize(d);
      clobber();
   }
}
BENCHMARK(BM_decode_strptime_manual_julian)->Range(now/2, now);

static void BM_decode_strptime_ossim_julian(benchmark::State & state)
{
   char date[1024]; // more than enough
   build_date_string(date, state.range_x());

   while (state.KeepRunning())  {
      ossimDate tm;
      strptime(date, format, &tm);
      const double d = tm.getModifiedJulian();
      benchmark::DoNotOptimize(d);
      clobber();
   }
}
BENCHMARK(BM_decode_strptime_ossim_julian)->Range(now/2, now);

BENCHMARK_MAIN();
