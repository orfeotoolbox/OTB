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
#include "otbStringUtilities.h"

#if OTB_USE_BOOST_TIME
namespace boost 
{ 
namespace posix_time 
{

time_duration abs(time_duration d)
{
  if(d.is_negative())
    d = d.invert_sign();
  return d;
}


}
}

namespace otb
{
namespace MetaData
{

TimeType ReadFormattedDate(const std::string & dateStr, const std::string & format)
{
  MetaData::TimeType outputDate;
  std::stringstream ss;
  auto facet = new boost::posix_time::time_input_facet(format);
  ss.imbue(std::locale(std::locale(), facet));
  ss << dateStr;
  ss >> outputDate;
  return outputDate;
}

DurationType seconds(double input)
{
  return boost::posix_time::precise_duration(input * 1e6);
}

}
}

#else //OTB_USE_BOOST_TIME

namespace otb
{
namespace MetaData
{

ModifiedJulianDate ReadFormattedDate(const std::string & dateStr, const std::string & format)
{
  otb::string_view fmt  = format;
  otb::string_view d    = dateStr;
  int year    = 0;
  int month   = 0;
  int day     = 0;
  int hours   = 0;
  int minutes = 0;
  int seconds = 0;
  double       frac_sec   = 0.0;

  for ( ; !fmt.empty() ; fmt.remove_prefix(1))
  {
    switch (fmt.front())
    {
      case '%':
        assert(!fmt.empty()); // invalid format => error
        fmt.remove_prefix(1);
        switch (fmt.front()) 
        {
          case '%':
            assert(!d.empty()); // invalid format => error
            if (fmt.front() != d.front()) 
            {
              throw std::runtime_error("Date "+ dateStr +" doesn't match format (" + format + ")");
            }
            d.remove_prefix(1);
            break;
          case 'Y': year    = otb::details::decode_uint(d); break;
          case 'm': month   = otb::details::decode_uint(d); break;
          case 'd': day     = otb::details::decode_uint(d); break;
          case 'H': hours   = otb::details::decode_uint(d); break;
          case 'M': minutes = otb::details::decode_uint(d); break;
          case 'S': seconds = otb::details::decode_uint(d); break;
          case 'F': // subseconds
            if (!d.empty() && d.front() == '.')
            {
              d.remove_prefix(1);
              std::size_t nsecs_nb_digits = d.size();
              frac_sec = otb::details::decode_uint(d);
              nsecs_nb_digits -= d.size();
              frac_sec /= std::pow((double)10.0, (int)nsecs_nb_digits);
            }
            break;
          default:
            throw std::logic_error("Unsupported date format specifier (in "+format+": "+fmt.front()+")");
         }
         break;
      default:
         if (fmt.front() != d.front()) {
          throw std::runtime_error("Date "+ dateStr +" doesn't match format (" + format + ")");
         }
         d.remove_prefix(1);
    }
  }

#if 1
  // Conversion to julian day
  // according to http://en.wikipedia.org/wiki/Julian_day
  // division are integer divisions:
  int a = (14 - month) / 12;
  int y = year + 4800 - a;
  int m = month + 12 * a - 3;
  int julianDayNumber = day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;

  // now, the division are NOT integer
  double julianDay = julianDayNumber +  hours / 24. +  minutes / 1440.+ (seconds + frac_sec) / 86400.;

#else
  double julianDay = day -32075+1461*(year+4800+(month-14)/12)/4+367*(month-2-(month-14)/12*12)
           /12-3*((year+4900+(month-14)/12)/100)/4 + (hours/24.0)
               + (minutes/1440.0)
               + ((seconds+frac_sec)/86400.0);
#endif

  // Convert Juian day to modified julian day
  return ModifiedJulianDate(julianDay - 2400000.5);
}

}
}

#endif