//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id$
#ifndef ossimDuration_HEADER
#define ossimDuration_HEADER
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimReferenced.h>
#include <iostream>

class OSSIM_DLL ossimDuration
{
public:
   /**
    * This will take an iso8601 encoded duration string and parse out
    * the individual values
    */
   ossimDuration(const ossimString& iso8601Duration = ossimString(""));
   
   
   /**
    * zero out all fields
    */
   void clearFields();
   
   /**
    * This will take an iso8601 encoded duration stream and parse out
    * the individual values it will stop when a blank character or whitespace is found
    */
   bool readIso8601Encoding(std::istream& in);
   
   /**
    * This will take an iso8601 encoded duration stream and parse out
    * the individual values it will stop when a blank character or whitespace is found
    */
   bool setByIso8601DurationString(const ossimString& iso8601Duration);
   
   /**
    * Will take the field values and encode into a iso8601 string format.
    * Note, anything that is 0 will not be output.
    *
    */
   void toIso8601DurationString(ossimString& result);
   
   /**
    * this will not use the months field or the years field but will use all other
    * fields to calculate a total value in seconds.  We can not determine leap years
    * and how many days are in a month so those are omitted and so this serves as
    * a utility method to just calculate the total seconds if you give a duration string
    * that contains only one or all or any of the following: weeks, days, minutes, hours,
    * and/or seconds.
    *
    * If the sign is set to negative it will return a negative value.
    */
   ossim_float64 toSeconds()const;
   
   void setSign(ossim_int32 value)
   {
      theSign = ((value < 0)?-1:1);
   }
   /**
    * returns the sign.  Should be either -1 or 1 for the return
    */
   ossim_int32 sign()const
   {
      return theSign;
   }
   void setYears(ossim_uint64 value)
   {
      theYears = value;
   }
   ossim_int64 years()const
   {
      return theYears;
   }
   void setMonths(ossim_uint64 value)
   {
      theMonths = value;
   }
   ossim_int64 months()const
   {
      return theMonths;
   }
   void setWeeks(ossim_uint64 value)
   {
      theWeeks = value;
   }
   ossim_int64 weeks()const
   {
      return theWeeks;
   }
   void setDays(ossim_uint64 value)
   {
      theDays = value;
   }
   ossim_int64 days()const
   {
      return theDays;
   }
   void setHours(ossim_uint64 value)
   {
      theHours = value;
   }
   ossim_int64 hours()const
   {
      return theHours;
   }
   void setMinutes(ossim_uint64 value)
   {
      theMinutes = value;
   }
   ossim_int64 minutes()const
   {
      return theMinutes;
   }
   void setSeconds(ossim_float64 value)
   {
      theSeconds = value;
   }
   ossim_float64 seconds()const
   {
      return theSeconds;
   }
   
   /**
    * Sets all values in one call
    */
   void setAll(ossim_int32 signValue,
               ossim_uint64 yearsValue,
               ossim_uint64 monthsValue,
               ossim_uint64 weeksValue,
               ossim_uint64 daysValue,
               ossim_uint64 hoursValue,
               ossim_uint64 minutesValue,
               ossim_float64 secondsValue)
   {
      theSign    = ((signValue < 0)?-1:1);
      theYears   = yearsValue;
      theMonths  = monthsValue;
      theWeeks   = weeksValue;
      theDays    = daysValue;
      theHours   = hoursValue;
      theMinutes = minutesValue;
      theSeconds = secondsValue;
   }
               
   
protected:
   ossim_int32 theSign; // indicates -1 for negative and anything else is positive
   ossim_int64 theYears; // number of years
   ossim_int64 theMonths; // number of months
   ossim_int64 theWeeks; // number of months
   ossim_int64 theDays;   // number of days
   ossim_int64 theHours;  // number of hours
   ossim_int64 theMinutes; // number of minutes
   ossim_float64 theSeconds; // umber of Seconds
};

#endif
