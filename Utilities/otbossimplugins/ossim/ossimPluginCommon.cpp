//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Common code for this plugin.
// 
//----------------------------------------------------------------------------
// $Id$

#include <cstdlib>

#include <ossimPluginCommon.h>
#include <otb/CivilDateTime.h>

bool ossim::UtcDateTimeStringToCivilDate(const std::string& utcString,
                                         CivilDateTime& outputDate)
{
   //---
   // conversion :
   // ossimRadarSatModel handles string civil dates of the
   // form yyyymmddhhmmssddd, 
   // while Rds2 date format is UTC (xs:dateTime) :
   // yyyy-mm-ddThh:mm:ss(.s+)zzzz (where zzzz is the timeZone).
   // Here, the timeZone is ignored.
   //---
   if (utcString.size() < 19) return false ;
   const char* stringUTCDate =  utcString.c_str() ;

   char year_str[5];
   for (int i=0;i<4;i++)
   {
      year_str[i] = stringUTCDate[i];
   }
   year_str[4] = '\0';

   char month_str[3];
   for (int i=5;i<7;i++)
   {
      month_str[i-5] = stringUTCDate[i];
   }
   month_str[2] = '\0';

   char day_str[3];
   for (int i=8;i<10;i++)
   {
      day_str[i-8] = stringUTCDate[i];
   }
   day_str[2] = '\0';

   char hour_str[3];
   for (int i=11;i<13;i++)
   {
      hour_str[i-11] = stringUTCDate[i];
   }
   hour_str[2] = '\0';

   char min_str[3];
   for (int i=14;i<16;i++)
   {
      min_str[i-14] = stringUTCDate[i];
   }
   min_str[2] = '\0';

   char sec_str[3];
   for (int i=17;i<19;i++)
   {
      sec_str[i-17] = stringUTCDate[i];
   }
   sec_str[2] = '\0';

   char mili_str[4]={'0','0','0','\0'};
   if ((utcString.size() > 19)&& (stringUTCDate[19] == '.')) {
      mili_str[0]=stringUTCDate[20];
      if ((utcString.size() > 21)&&(stringUTCDate[21] != ':')&&(stringUTCDate[21] != 'Z')) {
         mili_str[1]=stringUTCDate[21];
         if ((utcString.size() > 22)&&(stringUTCDate[22] != ':')&&(stringUTCDate[22] != 'Z')) {
            mili_str[2]=stringUTCDate[22];
         }
      }
   }

   int hour = atoi(hour_str);
   int min = atoi(min_str);
   int sec = atoi(sec_str);
   double mili = atof(mili_str);

   outputDate.set_year(atoi(year_str));
   outputDate.set_month(atoi(month_str));
   outputDate.set_day(atoi(day_str));
   outputDate.set_second(hour * 3600 + min * 60 + sec);
   outputDate.set_decimal(mili/1000.0);

   return true ;
}

