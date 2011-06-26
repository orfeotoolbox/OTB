//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
//----------------------------------------------------------------------------
// $Id: ossimDate.cpp 18270 2010-10-17 16:11:54Z dburken $

#include <ossim/base/ossimDate.h>
#include <cctype> /* for isdigit */
#include <iomanip>
#include <sstream>
#include <iostream>

std::ostream& operator<< (std::ostream& out, const ossimDate& src)
{
   return src.print(out);
}

std::ostream& operator<< (std::ostream & os, ossimLocalTm const & t)
{
   return t.print(os);
}

int operator== (ossimLocalTm const & t1, ossimLocalTm const & t2)
{
   return int(t1.compare(t2) == 0);
}

int operator!= (ossimLocalTm const & t1, ossimLocalTm const & t2)
{
   return int(t1.compare(t2) != 0);
}

int operator<  (ossimLocalTm const & t1, ossimLocalTm const & t2)
{
   return int(t1.compare(t2) < 0);
}

int operator<= (ossimLocalTm const & t1, ossimLocalTm const & t2)
{
   return int(t1.compare(t2) <= 0);
}

int operator>  (ossimLocalTm const & t1, ossimLocalTm const & t2)
{
   return int(t1.compare(t2) > 0);
}

int operator>=  (ossimLocalTm const & t1, ossimLocalTm const & t2)
{
   return int(t1.compare(t2) >= 0);
}

char ossimLocalTm::timech = ':';
char ossimLocalTm::datech = '/';

int ossimLocalTm::datefmt = ossimLocalTm::ossimLocalTmFormatFull;
int ossimLocalTm::timefmt = ossimLocalTm::ossimTimeFormatInternational;


ossimLocalTm::ossimLocalTm (time_t t)
   :theFractionalSecond(0.0)
{
    if (t == 0)
        t = time(0);
    *(tm *)this = *localtime(&t);
}

ossimLocalTm::ossimLocalTm (tm const & t)
   :theFractionalSecond(0.0)
{
    *((tm *)this) = t;
}

ossimLocalTm& ossimLocalTm::operator= (tm const & t)
{
    *((tm *)this) = t;
    return *this;
}

int ossimLocalTm::compare (ossimLocalTm const & t) const
{
    return compare ((time_t)t);
}

int ossimLocalTm::compare (time_t const tt) const
{
    time_t tx = (time_t)*this;
    return (tx == tt) ? 0 : (tx > tt) ? 1 : -1;
}

ossimLocalTm::operator time_t (void) const
{
    return mktime ((tm *)this);
}

int ossimLocalTm::isValid (void) const
{
    static int maxd[] =
    {
        31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

    return ((tm_year > 0) &&
            (tm_mon >= 0) && (tm_mon < 12) &&
            (tm_mday > 0) && (tm_mday <= maxd[tm_mon]) &&
            (tm_wday < 7) && (tm_yday < 367) &&
            (tm_sec < 60) && (tm_min < 60) && (tm_hour < 24));
}
void ossimLocalTm::now()
{
   time_t t = time(0);
   *this = *localtime(&t);
}

void ossimLocalTm::dSfx (std::ostream & os, int fmt) const
{
    if (fmt & ossimLocalTmFormatSepChar)
        os << datech;
    if (fmt & ossimLocalTmFormatSepSpace)
        os << ' ';
}

void ossimLocalTm::pYear (std::ostream & os, int fmt) const
{
    if (fmt & ossimLocalTmFormatYear)
    {
        int year = tm_year;
        int dig;
        if (fmt & ossimLocalTmFormatYearShort)
        {
            dig = 2;
            year %= 100;
        }
        else
        {
            dig = 4;
            if (year < 200)
                year += 1900;
        }
        os << std::setw((fmt & ossimLocalTmFormatPadYear) ? dig : 0);
        os << std::setfill((fmt & ossimLocalTmFormatZeroYear) ? '0' : ' ');
        os << year;
        if ((fmt & ossimLocalTmFormatYearFirst))
        {
            fmt &= (ossimLocalTmFormatSepChar|ossimLocalTmFormatSepSpace);
            dSfx (os, fmt);
        }
    }
}

void ossimLocalTm::pMonth (std::ostream & os, int fmt) const
{

    static const char * _months[] =
    {
        "January", "February", "March", "April",
        "May", "June", "July", "August", "September",
        "October", "November", "December"
    };

    if (fmt & ossimLocalTmFormatMonth)
    {
        int mon = (tm_mon % 12);
        if (fmt & ossimLocalTmFormatMonText)
        {
            char const * tmon = _months[mon];
            if (!(fmt & ossimLocalTmFormatPadMon))
                os << tmon;
            else
                for (int x = 0; x < 3; ++x)
                    os << tmon[x];
        }
        else
        {
            ++mon;
            os << std::setw((fmt & ossimLocalTmFormatPadMon) ? 2 : 0);
            os << std::setfill((fmt & ossimLocalTmFormatZeroMon) ? '0' : ' ');
            os << mon;
        }
        if (((fmt & ossimLocalTmFormatYear) && !(fmt & ossimLocalTmFormatYearFirst)) ||
            ((fmt & ossimLocalTmFormatDay) && (fmt & ossimLocalTmFormatMonFirst)))
        {
            fmt &= (ossimLocalTmFormatSepChar|ossimLocalTmFormatSepSpace);
            dSfx (os, fmt);
        }
    }
}


void ossimLocalTm::pDate (std::ostream & os, int fmt) const
{
    if (fmt & ossimLocalTmFormatDay)
    {
        int day = tm_mday;
        os << std::setw((fmt & ossimLocalTmFormatPadDay) ? 2 : 0);
        os << std::setfill((fmt & ossimLocalTmFormatZeroDay) ? '0' : ' ');
        os << day;
        if (!(fmt & ossimLocalTmFormatYearFirst) || !(fmt & ossimLocalTmFormatMonFirst))
        {
            fmt &= (ossimLocalTmFormatSepChar|ossimLocalTmFormatSepSpace);
            dSfx (os, fmt);
        }
    }
}

std::ostream& ossimLocalTm::print(std::ostream & os,
                                    int df,
                                    int tf) const
{
    std::ostringstream pTmp;
    printDate (pTmp, df);
    pTmp << ' ';
    printTime (pTmp, tf);

    return os << pTmp.str();
}



std::ostream& ossimLocalTm::printDate (std::ostream & os, int fmt) const
{
    std::ostringstream pTmp;

    static const char * _days[] =
    {
        "Sunday", "Monday", "Tuesday", "Wednesday",
        "Thursday", "Friday", "Saturday"
    };

    if (fmt & ossimLocalTmFormatDayOfWeek)
    {
        int day = tm_wday % 7;
        char const * p = _days[day];
        if (fmt & ossimLocalTmFormatPadDay)
            for (int x = 0; x < 3; ++x)
                pTmp << p[x];
        else
        {
            pTmp << p;
            if (fmt & ossimLocalTmFormatDMY)
                pTmp << ',';
        }
        if ((fmt & ossimLocalTmFormatDMY) && fmt & ossimLocalTmFormatSepSpace)
            pTmp << ' ';
    }
    if (fmt & ossimLocalTmFormatYearFirst)
        pYear (pTmp, fmt);
    if (fmt & ossimLocalTmFormatMonFirst)
        pMonth (pTmp, fmt);
    pDate (pTmp, fmt);
    if (!(fmt & ossimLocalTmFormatMonFirst))
        pMonth (pTmp, fmt);
    if (!(fmt & ossimLocalTmFormatYearFirst))
        pYear (pTmp, fmt);

    return os << pTmp.str();
}

std::ostream& ossimLocalTm::dump(std::ostream& os) const
{
   // Capture the original flags.
   std::ios_base::fmtflags f = os.flags();

   os << setiosflags(ios::fixed) << setprecision(8) << setiosflags(ios::left);

   os << "fr_sec:   " << setw(12) << theFractionalSecond
      << "fractional second\n"
      << "tm_sec:   " << setw(12) << tm_sec
      << "seconds [0-60] (1 leap second)\n"
      << "tm_min:   " << setw(12) << tm_min   << "minutes [0-59]\n"
      << "tm_hour:  " << setw(12) << tm_hour  << "hours [0-23]\n"
      << "tm_mday:  " << setw(12) << tm_mday  << "day [1-31]\n"
      << "tm_mon:   " << setw(12) << tm_mon   << "month [0-11]\n"
      << "tm_year:  " << setw(12) << tm_year  << "year - 1900\n"
      << "tm_wday:  " << setw(12) << tm_wday  << "day of week [0-6]\n"
      << "tm_yday:  " << setw(12) << tm_yday  << "days in year[0-365]\n"
      << "tm_isdst: " << setw(12) << tm_isdst << "DST.[-1/0/1]\n"
      << std::endl;

   // Reset flags.
   os.setf(f);
   
   return os;
}

void ossimLocalTm::tSfx (std::ostream & os, int fmt, char ch) const
{
    if (fmt & ossimTimeFormatSepAbbrev)
        os << ch;
    if (fmt & ossimTimeFormatSepChar)
        os << timech;
    if (fmt & ossimTimeFormatSepSpace)
        os << ' ';
}

void ossimLocalTm::pHour (std::ostream & os, int fmt) const
{
    if (fmt & ossimTimeFormatHour)
    {
        int hour = tm_hour;
        if (!(fmt & ossimTimeFormat24hour))
        {
            if (hour > 12)
                hour -= 12;
            else if (!hour && (fmt & ossimTimeFormatAmPm))
                hour += 12;
        }
        os << std::setw((fmt & ossimTimeFormatPadHour) ? 2 : 0);
        os << std::setfill((fmt & ossimTimeFormatZeroHour) ? '0' : ' ');
        os << hour;
        if (!(fmt & ossimTimeFormatMins))
            fmt &= ossimTimeFormatSepAbbrev;
        tSfx (os, fmt, 'h');
    }
}

void ossimLocalTm::pMins (std::ostream & os, int fmt) const
{
    if (fmt & ossimTimeFormatMins)
    {
        int min = tm_min;
        int dig = 2;
        if (!(fmt & ossimTimeFormatHour))
        {
            min += (tm_hour * 60);
            dig += 2;
        }
        os << std::setw((fmt & ossimTimeFormatPadMins) ? dig : 0);
        os << std::setfill((fmt & ossimTimeFormatZeroMins) ? '0' : ' ');
        os << min;
        if (!(fmt & ossimTimeFormatSecs))
            fmt &= ossimTimeFormatSepAbbrev;
        tSfx (os, fmt, 'm');
    }
}

void ossimLocalTm::pSecs (std::ostream & os, int fmt) const
{
    if (fmt & ossimTimeFormatSecs)
    {
        int sec = tm_sec;
        int dig = 2;
        if (!(fmt & (ossimTimeFormatHour|ossimTimeFormatMins)))
        {
            sec += ((tm_hour * 60) + tm_min) + 60;
            dig += 3;
        }
        os << std::setw((fmt & ossimTimeFormatPadSecs) ? dig : 0);
        os << std::setfill((fmt & ossimTimeFormatZeroSecs) ? '0' : ' ');
        os << sec;
        if (fmt & ossimTimeFormatAmPm)
            fmt &= ~ossimTimeFormatSepChar;
        else
            fmt &= (ossimTimeFormatSepAbbrev|ossimTimeFormatSepSpace);
        tSfx (os, fmt, 's');
    }
}

std::ostream &ossimLocalTm::printTime (std::ostream & os, int fmt) const
{
    std::ostringstream pTmp;
    pHour (pTmp, fmt);
    pMins (pTmp, fmt);
    pSecs (pTmp, fmt);
    if (fmt & ossimTimeFormatAmPm)
    {
        pTmp << (tm_hour > 11 ? "pm" : "am");
    }

    return os << pTmp.str();
}


int ossimLocalTm::getYear()const
{
   int result = tm_year;
//   if (result < 200)
   {
      result += 1900;
   }
   return result;
}

int ossimLocalTm::getShortYear()const
{
   return (getYear()%100);
}

int ossimLocalTm::getMonth()const
{
   return ((tm_mon % 12)+1); 
}

int ossimLocalTm::getDay()const
{
   return tm_mday;
}

double ossimLocalTm::getJulian()const
{
   int J = getMonth();
   int K = getDay();
   int I = getYear();

   return (K-32075+1461*(I+4800+(J-14)/12)/4+367*(J-2-(J-14)/12*12)
           /12-3*((I+4900+(J-14)/12)/100)/4+
           (getHour()/24.0)+
           (getMin()/1440.0)+
           ((getSec()+theFractionalSecond)/86400.0));
}

double ossimLocalTm::getModifiedJulian()const
{
   return getJulian() - 2400000.5;
}

ossimLocalTm& ossimLocalTm::setDay(int day)
{
   tm_mday = day;
   
   return *this;
}

ossimLocalTm& ossimLocalTm::setMonth(int month)
{
   tm_mon = month - 1;

   return *this;
}

ossimLocalTm& ossimLocalTm::setYear(int year)
{
   if(year < 2099)
   {
      tm_year = year - 1900;
   }
   else
   {
      tm_year = year;
   }

   return *this;
}

ossimLocalTm& ossimLocalTm::setDateFromJulian(double jd)
{
   double fractional = jd - (long)jd;
   long l;
   long n;
   long i;
   long j;
   long k;


   l= (long)(jd+68569);
   n= 4*l/146097;
   l= l-(146097*n+3)/4;
   i= 4000*(l+1)/1461001;
   l= l-1461*i/4+31l;
   j= 80*l/2447;
   k= l-2447*j/80;
   l= j/11;
   j= j+2-12*l;
   i= 100*(n-49)+i+l;
   setDay(k);
   setMonth(j);
   setYear(i);

   setFractionalDay(fractional);
   
   return *this;
}

ossimLocalTm& ossimLocalTm::setDateFromModifiedJulian(double mjd)
{
   setDateFromJulian(mjd + 2400000.5);

   return *this;
}

void ossimLocalTm::setFractionalDay(double fractionalDay)
{
   int h, m, s;
   double fractionalSecond;
   extractHmsFromFractionalDay(fractionalDay, h, m, s, fractionalSecond);

   setHour(h);
   setMin(m);
   setSec(s);
   setFractionalSecond(fractionalSecond);
}

void ossimLocalTm::extractHmsFromFractionalDay(double fractionalDay,
                                               int &h,
                                               int &m,
                                               int &s,
                                               double& fractionalSecond)
{
   fractionalDay *=24;
   h = (int)fractionalDay;
   fractionalDay = fractionalDay-h;
   fractionalDay*=60;
   m = (int)fractionalDay;
   fractionalDay = fractionalDay-m;
   fractionalDay*=60;
   s = (int)fractionalDay;
   fractionalDay = fractionalDay-s;
   fractionalSecond = fractionalDay;
}

int ossimLocalTm::getHour()const
{
   return tm_hour;
}

int ossimLocalTm::getMin()const
{
   return tm_min;
}

int ossimLocalTm::getSec()const
{
   return tm_sec;
}

double ossimLocalTm::getFractionalSecond()const
{
   return theFractionalSecond;
}

ossimLocalTm& ossimLocalTm::setHour(int h)
{
   tm_hour = h;

   return *this;
}

ossimLocalTm& ossimLocalTm::setMin(int m)
{
   tm_min = m;

   return *this;
}

ossimLocalTm& ossimLocalTm::setSec(int s)
{
   tm_sec = s;

   return *this;
}

ossimLocalTm& ossimLocalTm::setFloatSec(double s)
{
   tm_sec = (int)s;
   return setFractionalSecond(s-tm_sec);
}

ossimLocalTm& ossimLocalTm::setFractionalSecond(double fractionalSecond)
{
   theFractionalSecond = fractionalSecond;

   return *this;
}

time_t ossimLocalTm::getTicks()const
{
   return getEpoc();
}

time_t ossimLocalTm::getEpoc()const
{
   std::tm temp = *this;

   return mktime(&temp);
}

void ossimLocalTm::addSeconds(ossim_float64 n)
{
   // use julian to help in this addition.  Julian is in days
   setDateFromJulian(getJulian() + (n/86400.0));
}

void ossimLocalTm::addMinutes(ossim_float64 n)
{
   setDateFromJulian(getJulian() + (n/1440.0));
}

void ossimLocalTm::addHours(ossim_float64 n)
{
   setDateFromJulian(getJulian() + (n/24.0));
}

void ossimLocalTm::addDays(ossim_float64 n)
{
   setDateFromJulian(getJulian() + n);
}

ossim_float64 ossimLocalTm::deltaInSeconds(const ossimLocalTm& d)const
{
   return (getJulian()-d.getJulian())*86400.0;
}

ossim_float64 ossimLocalTm::deltaInMinutes(const ossimLocalTm& d)const
{
   return (getJulian()-d.getJulian())*1440.0;
}

ossim_float64 ossimLocalTm::delatInHours(const ossimLocalTm& d)const
{
   return (getJulian()-d.getJulian())*24;
}

ossim_float64 ossimLocalTm::deltaInDays(const ossimLocalTm& d)const
{
   return (getJulian()-d.getJulian());
}

ossimLocalTm ossimLocalTm::convertToGmt()const
{
   struct tm gmt = *this;
#if !defined(_MSC_VER) 
   tzset();
#else
   _tzset();
#endif

#if ( defined(__APPLE__) || defined(__FreeBSD__)  || defined(__OpenBSD__) )
   gmt.tm_sec -= tm_gmtoff; // Seconds east of UTC
#else
   gmt.tm_sec += timezone; // Seconds west of UTC
   if ( tm_isdst )
   {
      gmt.tm_sec -= 3600; // Subtract an hour.
   }
#endif
   
   time_t t = mktime(&gmt);
   ossimLocalTm result(*localtime(&t));
   
   return result;
}

void ossimLocalTm::setTimeNoAdjustmentGivenEpoc(time_t ticks)
{
   *this = *gmtime(&ticks);
}

void ossimLocalTm::setTimeGivenEpoc(time_t ticks)
{
   *this = *localtime(&ticks);
}


static bool readIntegerFromString(ossim_int32& result,
                                 const std::string& input,
                                 std::string::size_type& currentPos,
                                 int digits)
{
   ossimString number;
   while((digits>0)&&
         (currentPos < input.size()))
   {
      if(isdigit(input[currentPos]))
      {
         number += input[currentPos];
         ++currentPos;
         --digits;
      }
      else
      {
         return false;
      }
   }
   result = number.toInt32();
   return (digits <= 0);
}

static bool readTimeZoneOffset(ossim_int32& result,
                               const std::string& input,
                               std::string::size_type& currentPos)
{
   bool returnValue = false;
   result = 0;
   if(input[currentPos] == '+'||
      input[currentPos] == '-')
   {
      returnValue = true;
      ossim_int32 signMult = ((input[0] == '+')?1:-1);
      ossim_int32 zoneMin = 0;
      ossim_int32 zoneHour = 0;
      ++currentPos;
      if(readIntegerFromString(zoneHour,
                               input,
                               currentPos,
                               2))
      {
         if(!isdigit(input[currentPos]))
         {
            ++currentPos; // skip :
         }
         if(readIntegerFromString(zoneMin,
                                  input,
                                  currentPos,
                                  2))
         {
            result = signMult*(zoneMin*60 + zoneHour*3600);
         }
      }
   }
   
   return returnValue;
}

bool ossimLocalTm::setIso8601(const std::string& timeString, bool shiftToGmtOffsetZero)
{
   ossimDate now;
   std::string::size_type pos = 0;
   ossim_int32 year  = 0;
   ossim_int32 month = 0;
   ossim_int32 day   = 0;
   ossim_int32 timeZoneOffset = 0;
   
   if(timeString[0] != 'T') // make sure it's not time only
   {
      // look for year
      //
      if(readIntegerFromString(year,
                               timeString,
                               pos,
                               4))
      {
         // retrieved the year portion
         // now check for separator not digit
         //
         
         // we at least have a year
         // now check for others
         setYear(year);
         if(!isdigit(timeString[pos]))
         {
            // skip separator
            ++pos;
         }
         if(readIntegerFromString(month,
                                  timeString,
                                  pos,
                                  2))
         
         {
            setMonth(month);
            if(!isdigit(timeString[pos]))
            {
               // skip separator
               ++pos;
            }
            if(readIntegerFromString(day,
                                     timeString,
                                     pos,
                                     2))
            {
               setDay(day);
            }
         }
      }
      else
      {
         return false;
      }
   }
   else // set year month day to current
   {
      setYear(now.getYear());
      setMonth(now.getMonth());
      setDay(now.getDay());
   }
   // check to see if we need to read time portion
   if(timeString[pos] == 'T')
   {
      ++pos; // skip T character
      ossim_int32 hours=0, minutes=0;
      
      if(readIntegerFromString(hours,
                               timeString,
                               pos,
                               2))
      {
         setHour(hours);
         
         // now check for separator
         if(!std::isdigit(timeString[pos]))
         {
            ++pos; // skip separator if present
         }
         if(readIntegerFromString(minutes,
                                  timeString,
                                  pos,
                                  2))
         {
            setMin(minutes);
            // now check for time zone if only a hour minute time
            //
            if(timeString[pos] == 'Z')
            {
               // no adjustment needed
            }
            else if(!readTimeZoneOffset(timeZoneOffset,
                                       timeString,
                                       pos))
            {
               double fractionalSeconds = 0.0;
               if(!std::isdigit(timeString[pos]))
               {
                  ++pos;
               }
               std::string::size_type endPos = timeString.find_first_not_of("0123456789.", pos);
               if(endPos == std::string::npos)
               {
                  fractionalSeconds = ossimString(timeString.begin()+pos,
                                                  timeString.end()).toDouble();
               }
               else
               {
                  fractionalSeconds = ossimString(timeString.begin()+pos,
                                                  timeString.begin()+endPos).toDouble();
               }
               setFloatSec(fractionalSeconds);
               pos = endPos;
               if(pos == std::string::npos)
               {
                  // we will not be too strict so if at the end then just return we got enough
                  return true;
               }
               if(timeString[pos] == 'Z')
               {
                  // no adjustment needed
               }
               else
               {
                  readTimeZoneOffset(timeZoneOffset,
                                          timeString,
                                     pos);
               }
            }
         }
      }
      else
      {
         // need at least hours 
         return false;
      }
   }
   else if(std::isdigit(timeString[pos]))
   {
      ossim_int32 hours=0, minutes=0;
      
      if(readIntegerFromString(hours,
                               timeString,
                               pos,
                               2))
      {
         setHour(hours);
         
         // now check for separator
         if(!std::isdigit(timeString[pos]))
         {
            ++pos; // skip separator if present
         }
         if(readIntegerFromString(minutes,
                                  timeString,
                                  pos,
                                  2))
         {
            setMin(minutes);
            
            if(!readTimeZoneOffset(timeZoneOffset,
                                  timeString,
                                  pos))
            {
               double fractionalSeconds = 0.0;
               if(!std::isdigit(timeString[pos]))
               {
                  ++pos;
               }
               std::string::size_type endPos = timeString.find_first_not_of("0123456789.", pos);
               if(endPos == std::string::npos)
               {
                  fractionalSeconds = ossimString(timeString.begin()+pos,
                                                  timeString.end()).toDouble();
               }
               else
               {
                  fractionalSeconds = ossimString(timeString.begin()+pos,
                                                  timeString.begin()+endPos).toDouble();
               }
               setFloatSec(fractionalSeconds);
               pos = endPos;
               if(pos == std::string::npos)
               {
                  // we will not be too strict so if at the end then just return we got enough
                  return true;
               }
               if(timeString[pos] == 'Z')
               {
                  // no adjustment needed
               }
               else
               {
                  readTimeZoneOffset(timeZoneOffset,
                                     timeString,
                                     pos);
               }
            }
         }
      }  
   }
   else
   {
      // need at least hours 
      return false;
   }
   
   if(shiftToGmtOffsetZero && (timeZoneOffset!=0))
   {
      addSeconds(-timeZoneOffset);
   }
   return true;
}

ossimRefPtr<ossimXmlNode> ossimLocalTm::saveXml()const
{
   ossimRefPtr<ossimXmlNode> result = new ossimXmlNode;

   result->setTag("ossimDate");
   result->addAttribute("version", "1");
   result->addChildNode("month", ossimString::toString(getMonth()));
   result->addChildNode("day", ossimString::toString(getDay()));
   result->addChildNode("year", ossimString::toString(getYear()));
   result->addChildNode("hour", ossimString::toString(getHour()));
   result->addChildNode("minutes", ossimString::toString(getMin()));
   result->addChildNode("seconds", ossimString::toString(getSec()));
   result->addChildNode("fractionalSecond", ossimString::toString(getFractionalSecond()));
   
   return result.get();
}

bool ossimLocalTm::loadXml(ossimRefPtr<ossimXmlNode> dateNode)
{
   bool result = true;
   ossimRefPtr<ossimXmlNode> month = dateNode->findFirstNode("month");
   ossimRefPtr<ossimXmlNode> day = dateNode->findFirstNode("day");
   ossimRefPtr<ossimXmlNode> year = dateNode->findFirstNode("year");
   ossimRefPtr<ossimXmlNode> hour = dateNode->findFirstNode("hour");
   ossimRefPtr<ossimXmlNode> minutes = dateNode->findFirstNode("minutes");
   ossimRefPtr<ossimXmlNode> seconds = dateNode->findFirstNode("seconds");
   ossimRefPtr<ossimXmlNode> fractionalSecond = dateNode->findFirstNode("fractionalSecond");
   ossimRefPtr<ossimXmlNode> julian = dateNode->findFirstNode("julian");
   ossimRefPtr<ossimXmlNode> modifiedJulian = dateNode->findFirstNode("modifiedJulian");

   if(month.valid()&&
      day.valid()&&
      year.valid()&&
      hour.valid()&&
      minutes.valid()&&
      seconds.valid())
   {
      setMonth(month->getText().toInt32());
      setDay(day->getText().toInt32());
      setYear(year->getText().toInt32());
      setHour(hour->getText().toInt32());
      setMin(minutes->getText().toInt32());
      setSec(seconds->getText().toInt32());
      if(fractionalSecond.valid())
      {
         setFractionalSecond(fractionalSecond->getText().toDouble());
      }
      else
      {
         setFractionalSecond(0.0);
      }
   }
   else if(modifiedJulian.valid())
   {
      setDateFromModifiedJulian(modifiedJulian->getText().toDouble());
   }
   else if(julian.valid())
   {
      setDateFromJulian(julian->getText().toDouble());
   }
   else
   {
      result = false;
   }

   return result;
}

ossimDate::ossimDate(int datefmt)
   :ossimLocalTm(0), _fmt(datefmt)
{}

ossimDate::ossimDate (ossimLocalTm const & t,
                      int dtfmt)
   : ossimLocalTm (t), _fmt(dtfmt)
{}

ossimDate::ossimDate (time_t t, int dtfmt)
   : ossimLocalTm (t), _fmt(dtfmt)
{}

ossimDate::ossimDate(int month,
                     int day,
                     int year,
                     int dtfmt)
   :ossimLocalTm (0), _fmt(dtfmt)
{
   setMonth(month);
   setDay(day);
   setYear(year);
   setHour(0);
   setMin(0);
   setSec(0);
   setFractionalSecond(0.0);
}

int ossimDate::fmt(int f)
{
   return _fmt = f;
}

int ossimDate::fmt(void) const
{
   return _fmt;
}

std::ostream& ossimDate::print (std::ostream & os) const
{
   return printDate (os, _fmt);
}

std::ostream& operator <<(std::ostream& out, const ossimTime& src)
{
   return src.print(out);
}
ossimTime::ossimTime(int tmfmt)
      : ossimLocalTm(0), _fmt(tmfmt)
{
}
ossimTime::ossimTime(ossimTime const & t,
                      int tmfmt)
      : ossimLocalTm (t), _fmt(tmfmt)
{
}

ossimTime::ossimTime(time_t t, int tmfmt)
      : ossimLocalTm (t), _fmt(tmfmt)
{
}
      
int ossimTime::fmt(int f)
{
   return _fmt = f;
}

int ossimTime::fmt(void) const
{
   return _fmt;
}

std::ostream& ossimTime::print (std::ostream & os) const
{
   return printTime(os, _fmt);
}
