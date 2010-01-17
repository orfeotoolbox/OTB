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
#include <ossim/base/ossimDuration.h>
#include <ossim/base/ossimCommon.h>
#include <sstream>
#include <cctype>
ossimDuration::ossimDuration(const ossimString& iso8601Duration)
:theSign(1),
theYears(0),
theMonths(0),
theWeeks(0),
theDays(0),
theHours(0),
theMinutes(0),
theSeconds(0)
{
   if(!iso8601Duration.empty())
   {
      setByIso8601DurationString(iso8601Duration);
   }
}

void ossimDuration::clearFields()
{
   theSign = 1; 
   theYears = 0;
   theMonths = 0;
   theWeeks = 0;
   theDays = 0;
   theHours = 0;
   theMinutes = 0;
   theSeconds = 0; 
}

static bool isWhiteSpace(int c)
{
   return ((c==' ') ||
           (c=='\n')||
           (c=='\r')||
           (c=='\t'));
}

bool ossimDuration::readIso8601Encoding(std::istream& in)
{
   clearFields();
   
   if(in.peek()=='-')
   {
      theSign = -1;
      in.ignore();
   }
   if(in.peek()!='P')
   {
      return false;
   }
   // skip the period indicater
   in.ignore();
   
   // now we start parsing the date portion and the time portion
   ossimString value;
   bool doneFlag     = false;
   bool badParseFlag = false;
   bool doingTimeFlag = false;
   while(!doneFlag)
   {
      int c = in.peek();
      if(!in)
      {
         doneFlag = true;
      }
      else
      {
         if(isWhiteSpace(c))
         {
            doneFlag = true; // parse til blank character is met
         }
         else if(isalpha(c))
         {
            // we are done with current value 
            //
            // check to see if was a Time seaprator of value
            // 'T'
            if(c == 'T')
            {
               // then it was a time separator so do nothing
               value = ""; // go ahead and null it out for now
               doingTimeFlag = true; // now in time parsing portion
            }
            else
            {
               if(doingTimeFlag)
               {
                  // check time values
                  if(c == 'H')
                  {
                     theHours = value.toUInt64();
                  }
                  else if(c == 'M')
                  {
                     theMinutes = value.toUInt64();
                  }
                  else if(c == 'S')
                  {
                     theSeconds = value.toFloat64();
                  }
                  else
                  {
                     doneFlag = true;
                     badParseFlag = true;
                  }
                  value = ""; // reset the value
               }
               else // check date characters instead
               {
                  if(c == 'Y')
                  {
                     theYears = value.toUInt64();
                  }
                  else if(c == 'M')
                  {
                     theMonths = value.toUInt64();
                  }
                  else if(c == 'W')
                  {
                     theWeeks = value.toUInt64();
                  }
                  else if(c == 'D')
                  {
                     theDays = value.toUInt64();
                  }
                  else
                  {
                     doneFlag = true;
                     badParseFlag = true;
                  }
                  value = ""; // reset the value
               }
            }
         }
         else if(isdigit(c)||(c=='.'))// not an alphabetic character so add it to the value string
         {
            value += static_cast<char>(c);
         }
         else
         {
            doneFlag = true;
            badParseFlag = true;
         }
      }
      if(!doneFlag)
      {
         in.ignore();
      }
   }
   
   return badParseFlag;
}

bool ossimDuration::setByIso8601DurationString(const ossimString& iso8601Duration)
{
   if(iso8601Duration.empty())
   {
      clearFields();
      return true;
   }
   std::istringstream in(iso8601Duration);
   return readIso8601Encoding(in);
}

void ossimDuration::toIso8601DurationString(ossimString& result)
{
   result = "";
   bool hasDatePeriod = ((theYears!=0)||
                         (theMonths!=0)||
                         (theWeeks!=0)||
                         (theDays!=0));
   bool hasTimePeriod = ((theHours!=0)||
                         (theMinutes!=0)||
                         (!ossim::almostEqual(theSeconds, 0.0, .00000000001)));
   // if no time or date period present then return empty
   if(!(hasDatePeriod || hasTimePeriod))
   {
      return;
   }
   if(theSign < 0)
   {
      result += "-";
   }
   result += "P";
   if(hasDatePeriod)
   {
      if(theYears > 0)
      {
         result+=ossimString::toString(theYears);
         result+="Y";
      }
      if(theMonths>0)
      {
         result+=ossimString::toString(theMonths);
         result+="M";
      }
      if(theWeeks>0)
      {
         result+=ossimString::toString(theWeeks);
         result+="W";
      }
      if(theDays>0)
      {
         result+=ossimString::toString(theDays);
         result+="D";
      }
   }
   if(hasTimePeriod)
   {
      result+="T";
      if(theHours>0)
      {
         result+=ossimString::toString(theHours);
         result+="H";
      }
      if(theMinutes>0)
      {
         result+=ossimString::toString(theMinutes);
         result+="M";
      }
      if(theSeconds>0)
      {
         result+=ossimString::toString(theSeconds, 15);
         result+="S";
      }
   }
}

ossim_float64 ossimDuration::toSeconds()const
{
   ossim_float64 result = theSeconds;
   
   if(theMinutes > 0)
   {
      result += theMinutes*60.0;
   }
   if(theHours > 0)
   {
      result += theHours*3600.0;
   }
   if(theDays > 0)
   {
      result += theDays*86400.0;
   }
   if(theWeeks > 0)
   {
      result += theWeeks*604800;
   }
   if(theSign < 0)
   {
      result *= -1.0;
   }
   return result;
}


