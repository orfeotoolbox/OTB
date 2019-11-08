/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "ossim/ossimTimeUtilities.h"
#include <stdexcept>
// #include <sstream>
#include <typeinfo>
#include <cmath>
#include <cstdio>
#include <stdexcept>
#include "ossim/ossimStringUtilities.h"
#include <ossim/base/ossimDate.h>
// #include <iostream>

ossimDate ossimplugins::time::details::strptime(string_view const& format, string_view const& date)
{
   using namespace ossimplugins::details;

   //[[pre: format doesn't end with a odd number of '%']]
   string_view fmt  = format;
   string_view d    = date;
   unsigned int year    = 0;
   unsigned int month   = 0;
   unsigned int day     = 0;
   unsigned int hours   = 0;
   unsigned int minutes = 0;
   unsigned int seconds = 0;
   double       ssecs   = 0.0;

   for ( ; !fmt.empty() ; fmt.remove_prefix(1))
   {
      switch (fmt.front())
      {
         case '%':
            // std::cout << "found ("<<fmt.size()<<")%: " << fmt.front();
            assert(!fmt.empty()); // invalid format => error
            fmt.remove_prefix(1);
            switch (fmt.front()) {
               case '%':
                  // std::cout << "% -> just a %" << std::endl;
                  assert(!d.empty()); // invalid format => error
                  if (fmt.front() != d.front()) {
                     throw std::runtime_error("Date "+date+" doesn't match format (" + format + ")");
                  }
                  d.remove_prefix(1);
                  break;
               case 'Y': year    = decode_uint(d); break;
               case 'm': month   = decode_uint(d); break;
               case 'd': day     = decode_uint(d); break;
               case 'H': hours   = decode_uint(d); break;
               case 'M': minutes = decode_uint(d); break;
               case 'S': seconds = decode_uint(d); break;
               case '.': // subseconds
                         if (!d.empty() && d.front() == '.') {
                            d.remove_prefix(1);
                            std::size_t nsecs_nb_digits = d.size();
                            ssecs = decode_uint(d);
                            nsecs_nb_digits -= d.size();
                            ssecs /= std::pow((double)10.0, (int)nsecs_nb_digits);
                         } else {
                            // std::cout << "No subfraction in source date" << std::endl;
                         }
                         break;
               default:
                  throw std::logic_error("Unsupported date format speficier (in "+format+": "+fmt.front()+")");
            }
            break;
         default:
            // std::cout << "found char: `" << fmt.front() << "`. Does it match `" << d.front() << "`?" << std::endl;
            if (fmt.front() != d.front()) {
               throw std::runtime_error("Date "+date+" doesn't match format (" + format + ")");
            }
            d.remove_prefix(1);
      }
   } // while fmt
   // std::cout << std::endl;

   ossimDate res(month, day, year);
   res.setHour(hours);
   res.setMin(minutes);
   res.setSec(seconds);
   res.setFractionalSecond(ssecs);
   // fmt = format;
   return res;
}

ossimplugins::time::ModifiedJulianDate ossimplugins::time::toModifiedJulianDate(string_view const& utcTimeString)
{
   const ossimDate date = details::strptime("%Y-%m-%dT%H:%M:%S%.", utcTimeString);
   return ModifiedJulianDate(date.getModifiedJulian());
}


std::string ossimplugins::time::to_simple_string(ModifiedJulianDate const& d)
{
   ossimDate date ;
   date.setDateFromModifiedJulian(d.as_day_frac());
   char buffer[1024] = {0};
   size_t s = std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", &date);
   if (s == 0) {
      // TODO d.as_day_frac() -> string
      throw std::runtime_error("Cannot encode date into a string");
   }
   assert(s < 1000); // should be more than enough
   typedef unsigned long ulong;
   const double frac_sec = date.getFractionalSecond();
   // const double fs2 = fmod(d.as_day_frac() * 24ULL*60*60, 1);
   // assert(fs2==frac_sec);
   // std::cout << "From: " << d.as_day_frac() << " -> frac_sec: " << fs2
      // << " --> *10^6: " << fs2*1000ULL*1000
      // << " --> %10^6: " << ulong(fs2*1000ULL*1000)%(1000ULL*1000)
      // << "\n";
   s += s_printf(&buffer[s], sizeof(buffer)-s, ".%06ld",
         ulong(frac_sec * 1000ULL * 1000) % (1000ULL*1000));
   return std::string(buffer, s);
}

std::string ossimplugins::time::to_simple_string(Duration const& d)
{
   typedef unsigned long ulong;
   const double fs = d.as_day_frac() * 24 * 60 * 60; // in seconds
   const ulong  ls = std::abs(fs);
   const ulong  us = ulong(std::abs(fs * 1000ULL*1000)) % (1000ULL*1000);
   const ulong  s  = ls % 60;
   const ulong  m  = (ls/60) % 60;
   const ulong  h  = (ls/60/60);
   char buffer[1024];
   const std::size_t N = s_printf(buffer, "%s%02d:%02d:%02d.%06ld", fs < 0 ? "-" : "", h, m, s, us);
   assert(N);

   return std::string(buffer, N);
}

