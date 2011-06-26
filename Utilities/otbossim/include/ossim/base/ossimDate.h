//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimDate.h 19704 2011-05-31 22:20:40Z dburken $
#ifndef ossimDate_HEADER
#define ossimDate_HEADER
#include <ctime>
#include <iosfwd>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimXmlNode.h>

class OSSIM_DLL ossimLocalTm : public std::tm
{
public:
   friend OSSIM_DLL std::ostream& operator<<(std::ostream& out, const ossimLocalTm& src);
   friend OSSIM_DLL int operator== (const ossimLocalTm& t1,
                                    const ossimLocalTm& src);
   friend OSSIM_DLL int operator!= (const ossimLocalTm& t1,
                                    const ossimLocalTm& src);
   friend OSSIM_DLL int operator<  (const ossimLocalTm& t1,
                                    const ossimLocalTm& src);
   friend OSSIM_DLL int operator<= (const ossimLocalTm& t1,
                                    const ossimLocalTm& src);
   friend OSSIM_DLL int operator>  (const ossimLocalTm& t1,
                                    const ossimLocalTm& src);
   friend OSSIM_DLL int operator>= (const ossimLocalTm& t1,
                                    const ossimLocalTm& src);
   
   ossimLocalTm (time_t t=0);     // Set to time, 0 is magic for 'now'
   ossimLocalTm (tm const & t); // Copy constructor
   
   ossimLocalTm & operator= (tm const & t);            // Assignment
   operator time_t (void) const;        // Conversion operator
   
   int isValid (void) const;             // Test for validity
   
   void now();
   int compare (ossimLocalTm const & t) const;      // Compare times
   int compare (std::time_t const tt) const;       // Compare times
   
   enum ossimLocalTmFormat     // Date format flags
   {
      ossimLocalTmFormatYear          = (int)0x0001,            // Print year
      ossimLocalTmFormatYearShort     = (int)0x0002, // Print last two digits
      ossimLocalTmFormatPadYear       = (int)0x0004,    // Pad year to 2 or 4
      ossimLocalTmFormatZeroYear      = (int)0x0008,        // Zero fill year
      ossimLocalTmFormatMonth         = (int)0x0010,           // Print month
      ossimLocalTmFormatMonText       = (int)0x0020,   // Print month in text
      ossimLocalTmFormatPadMon        = (int)0x0040, // Pad to 2 (trunc to 3)
      ossimLocalTmFormatZeroMon       = (int)0x0080,       // Zero fill month
      ossimLocalTmFormatDay           = (int)0x0100,            // Print date
      ossimLocalTmFormatDayOfWeek     = (int)0x0200,     // Print day of week
      ossimLocalTmFormatPadDay        = (int)0x0400,         // Pad date to 2
      ossimLocalTmFormatZeroDay       = (int)0x0800,         // Zero fill day
      ossimLocalTmFormatDMY           = (int)0x0111, // Print date, mth, year
      ossimLocalTmFormatPadDMY        = (int)0x0444,         // Pad all three
      ossimLocalTmFormatZeroDMY       = (int)0x0888,   // Zero fill all three
      ossimLocalTmFormatYearFirst     = (int)0x1000,      // Print year first
      ossimLocalTmFormatMonFirst      = (int)0x2000,     // Print month first
      ossimLocalTmFormatSepChar       = (int)0x4000,// Separate fields datech
      ossimLocalTmFormatSepSpace      = (int)0x8000,  // Separate fields space
      ossimLocalTmFormatInternational = (int)(ossimLocalTmFormatDMY|ossimLocalTmFormatMonText|ossimLocalTmFormatPadMon|
                                              ossimLocalTmFormatSepSpace),
      ossimLocalTmFormatIntlShort     = (int)(ossimLocalTmFormatDMY|ossimLocalTmFormatMonText|ossimLocalTmFormatPadMon|
                                              ossimLocalTmFormatSepSpace|ossimLocalTmFormatYearShort),
      ossimLocalTmFormatUsa           = (int)(ossimLocalTmFormatDMY|ossimLocalTmFormatMonFirst|ossimLocalTmFormatPadDMY|
                                              ossimLocalTmFormatZeroDMY|ossimLocalTmFormatYearShort|ossimLocalTmFormatSepChar),
      ossimLocalTmFormatEnglish       = (int)(ossimLocalTmFormatDMY|ossimLocalTmFormatYearShort|ossimLocalTmFormatPadDMY|
                                              ossimLocalTmFormatZeroDMY|ossimLocalTmFormatSepChar),
      ossimLocalTmFormatJapanese      = (int)(ossimLocalTmFormatDMY|ossimLocalTmFormatYearFirst|ossimLocalTmFormatPadDMY|
                                              ossimLocalTmFormatZeroDMY|ossimLocalTmFormatYearShort|ossimLocalTmFormatMonFirst|
                                              ossimLocalTmFormatSepChar),
      ossimLocalTmFormatFull          = (int)(ossimLocalTmFormatDMY|ossimLocalTmFormatDayOfWeek|ossimLocalTmFormatMonText|
                                              ossimLocalTmFormatSepSpace)
   };
   
   enum ossimTimeFormat  // Time format (nb: time zones not implemented)
   {
      ossimTimeFormatSecs          = (int)0x0001,         // Print seconds
      ossimTimeFormatZeroSecs      = (int)0x0002,     // Zero fill seconds
      ossimTimeFormatPadSecs       = (int)0x0004,      // Pad seconds to 2
      ossimTimeFormatSecsAll       = (int)0x0007,
      ossimTimeFormatTimeZone      = (int)0x0008,        // Print timezone
      ossimTimeFormatMins          = (int)0x0010,         // Print minutes
      ossimTimeFormatZeroMins      = (int)0x0020,     // Zero fill minutes
      ossimTimeFormatPadMins       = (int)0x0040,      // Pad minutes to 2
      ossimTimeFormatMinsAll       = (int)0x0070,
      ossimTimeFormatTZNumeric     = (int)0x0080,      // Print numeric TZ
      ossimTimeFormatHour          = (int)0x0100,            // Print hour
      ossimTimeFormatZeroHour      = (int)0x0200,        // Zero fill hour
      ossimTimeFormatPadHour       = (int)0x0400,  // Pad hour to 2 digits
      ossimTimeFormatHourAll       = (int)0x0700,
      ossimTimeFormat24hour        = (int)0x0800,          // 24hour clock
      ossimTimeFormatSepChar       = (int)0x1000, // Separate field timech
      ossimTimeFormatSepSpace      = (int)0x2000, // Separate fields space
      ossimTimeFormatSepAbbrev     = (int)0x4000,     // Add abbreviations
      ossimTimeFormatAmPm          = (int)0x8000,       // Add 'am' or 'pm'
      ossimTimeFormatInternational = (int)(ossimTimeFormatHourAll|ossimTimeFormatMinsAll|ossimTimeFormatSecsAll|
                                           ossimTimeFormat24hour|ossimTimeFormatSepChar),
      ossimTimeFormatShortTime     = (int)(ossimTimeFormatHourAll|ossimTimeFormatMinsAll|ossimTimeFormat24hour|
                                           ossimTimeFormatSepChar),
      ossimTimeFormatClockTime     = (int)(ossimTimeFormatHour|ossimTimeFormatPadHour|ossimTimeFormatMinsAll|
                                           ossimTimeFormatAmPm|ossimTimeFormatSepChar),
      ossimTimeFormatLongTime      = (int)(ossimTimeFormatHour|ossimTimeFormatPadHour|ossimTimeFormatMinsAll|
                                           ossimTimeFormatSecsAll|ossimTimeFormatSepAbbrev|ossimTimeFormatSepSpace),
      ossimTimeFormatMillitary     = (int)(ossimTimeFormatHourAll|ossimTimeFormatMinsAll|ossimTimeFormat24hour)
   };
   
   
   static char timech;    // Character used for time separator
   static char datech;    // Character used for date separator
   static int datefmt;                  // Default date format
   static int timefmt;                  // Default time format
   
   // Output methods
   std::ostream& print(std::ostream & os,
                       int df =datefmt,
                       int tf =timefmt) const;
   std::ostream& printTime(std::ostream & os, int f =timefmt) const;
   std::ostream& printDate(std::ostream & os, int f =datefmt) const;
   
   /**
    * @brief method to dump all the data members.  This is the tm struct +
    * the theFractionalSecond from this class.
    *
    * @note: tm_gmtoff and  tm_zone from tm struct are not printed as they
    * are not standard.
    *
    * @param os Stream to dump to.
    *
    * @return stream.
    */
   std::ostream& dump(std::ostream& os) const;
   
   
   int getYear()const;
   
   int getShortYear()const;
   
   /*!
    * This is one based i.e. values are returned between 1-12
    */
   int getMonth()const;
   
   /*!
    * returns the day 1-31
    */
   int getDay()const;
   double getJulian()const;
   double getModifiedJulian()const;
   
   ossimLocalTm& setDay(int day);
   /*!
    * The month is a number bewteen 1 and 12.  We will shift it to the internal
    * representation
    */
   ossimLocalTm& setMonth(int month);
   ossimLocalTm& setYear(int year);
   ossimLocalTm& setDateFromJulian(double jd);
   ossimLocalTm& setDateFromModifiedJulian(double mjd);
   
   void setFractionalDay(double fractionalDay);
   static void extractHmsFromFractionalDay(double fractionalDay,
                                           int &h,
                                           int &m,
                                           int &s,
                                           double& fractionalSecond);
   int getHour()const;
   int getMin()const;
   int getSec()const;
   double getFractionalSecond()const;
   ossimLocalTm& setHour(int h);
   ossimLocalTm& setMin(int m);
   ossimLocalTm& setSec(int s);
   ossimLocalTm& setFloatSec(double s);
   ossimLocalTm& setFractionalSecond(double fractS);
   
   /**
    * getTicks() will call getEpoc.  This is te number of microseconds passed
    * since (00:00:00 UTC, January 1, 1970)
    */ 
   time_t getTicks()const;
   time_t getEpoc()const;
   
   void addSeconds(ossim_float64 n);
   void addMinutes(ossim_float64 n);
   void addHours(ossim_float64 n);
   void addDays(ossim_float64 n);
   
   ossim_float64 deltaInSeconds(const ossimLocalTm& d)const;
   ossim_float64 deltaInMinutes(const ossimLocalTm& d)const;
   ossim_float64 delatInHours(const ossimLocalTm& d)const;
   ossim_float64 deltaInDays(const ossimLocalTm& d)const;
   
   ossimLocalTm convertToGmt()const;
   /**
    * Will not adjust for timezone.  The passed in value is based on seconds.
    */ 
   void setTimeNoAdjustmentGivenEpoc(time_t ticks);
   
   /**
    * Will adjust for timezone. The passed in value is based on seconds.
    */ 
   void setTimeGivenEpoc(time_t ticks);
   
   bool setIso8601(const std::string& timeString, bool shiftToGmtOffsetZero=false);
   
   ossimRefPtr<ossimXmlNode> saveXml()const;
   bool loadXml(ossimRefPtr<ossimXmlNode> dateNode);
   
protected:
   // Time suffix
   void tSfx(std::ostream & os, int fmt, char ch) const;
   // Time field formatters
   void pHour(std::ostream & os, int fmt) const;
   void pMins(std::ostream & os, int fmt) const;
   void pSecs(std::ostream & os, int fmt) const;
   // Date suffix
   void dSfx(std::ostream & os, int fmt) const;
   // Date field formatters
   void pDate(std::ostream & os, int fmt) const;
   void pMonth(std::ostream & os, int fmt) const;
   void pYear(std::ostream & os, int fmt) const;
   
   double theFractionalSecond;
};


class OSSIM_DLL ossimDate : public ossimLocalTm
{
public:
   friend OSSIM_DLL std::ostream& operator<<(std::ostream& out, const ossimDate& src);
   
   ossimDate(int datefmt =ossimLocalTm::datefmt);
   ossimDate (ossimLocalTm const & t,
              int dtfmt =ossimLocalTm::datefmt);
   ossimDate (time_t t, int dtfmt =ossimLocalTm::datefmt);
   ossimDate(int month, int day, int year,int dtfmt=ossimLocalTm::datefmt);
   
   int fmt(int f);
   int fmt(void) const;
   
   std::ostream & print (std::ostream & os) const;
   
private:
   int _fmt;
};

class OSSIM_DLL ossimTime :public ossimLocalTm
{
public:
   friend OSSIM_DLL std::ostream& operator<<(std::ostream& out, const ossimTime& src);
   ossimTime(int tmfmt =ossimLocalTm::timefmt);
   ossimTime (ossimTime const & t,
              int tmfmt =ossimLocalTm::timefmt);
   ossimTime (time_t t, int tmfmt =ossimLocalTm::timefmt);
   
   int fmt (int f);
   int fmt (void) const;
   
   std::ostream& print (std::ostream & os) const;
   
private:
   int _fmt;   
};
#endif
