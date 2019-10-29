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



#ifndef CivilDateTime_h
#define CivilDateTime_h

#include <ossimPluginConstants.h>
#include <iosfwd>

namespace ossimplugins
{


class JulianDate;
class JSDDateTime;
class GMSTDateTime;

/**
 * @brief This class represents a date and time in the civil format
 */
class OSSIM_PLUGINS_DLL CivilDateTime
{
public:

   /**
    * @brief Constructor
    */
   CivilDateTime();

   /**
    * @brief Constructor with date and time initialization
    */
   CivilDateTime(int year, int month, int day, int second, double decimal);

   /**
    * @brief Constructor with date and time initialization using an utc string
    */
   CivilDateTime(char* Utc);

   CivilDateTime(const char* Utc);

   /**
    * @brief Destructor
    */
   ~CivilDateTime();

   /**
    * @brief Copy constructor
    */
   CivilDateTime(const CivilDateTime& rhs);

   /**
    * @brief Affectation operator
    */
   CivilDateTime& operator=(const CivilDateTime& rhs);

   /**
    * @brief This function converts the current CivilDateTime into Julian Date
    * @return 0 if no error or an int greater than 0 indicating the error
    * @param julianDate [out] The class that receives the result of the conversion of the current CivilDateTime into JulianDate
    */
   int AsJulianDate(JulianDate* julianDate);

   /**
    * @brief This function converts the current CivilDateTime into JSDDateTime
    * @return 0 if no error or an int greater than 0 indicating the error
    * @param JSDdate [out] The class that receives the result of the conversion of the current CivilDateTime into JSDDateTime
    */
   int AsJSDDateTime(JSDDateTime* JSDdate);

   /**
    * @brief This function converts the current CivilDateTime into GMSTDateTime
    * @return 0 if no error or an int greater than 0 indicating the error
    * @param GMSTdate [out] The class that receives the result of the conversion of the current CivilDateTime into GMSTDateTime
    */
   int AsGMSTDateTime(GMSTDateTime* GMSTdate);

   int get_year()
   {
      return _year;
   };

   int get_month()
   {
      return _month;
   };

   int get_day()
   {
      return _day;
   };

   int get_second()
   {
      return _second;
   };

   double get_decimal()
   {
      return _decimal;
   };

   void set_year(int year)
   {
      _year = year;
   };

   void set_month(int month)
   {
      _month = month;
   };

   void set_day(int day)
   {
      _day = day;
   };

   void set_second(int second)
   {
      _second = second;
   };

   void set_decimal(double decimal)
   {
      _decimal = decimal;
   };

   /**
    * @brief This function initialize the date and the the time of the class using an utc string
    * @param Utc Utc string containing the date and time
    * @return 0 if no error or an int gretter than 0 indiquating the error
    */
   int SetUtcDateTime(char* Utc);

   int SetUtcDateTime(const char* Utc);


   operator JulianDate() ;

   operator JSDDateTime() ;

   operator GMSTDateTime() ;

   std::ostream& print(std::ostream& out) const;

   friend std::ostream& operator<<(std::ostream& out,
                                   const CivilDateTime& obj);


   static const double JOURCIVIL_LENGTH;
protected:
   /**
    * @brief
    */
   int _year;
   /**
    * @brief
    */
   int _month;
   /**
    * @brief
    */
   int _day;
   /**
    * @brief Number of secondes since the beginning of the day
    */
   int _second;
   /**
    * @brief
    */
   double _decimal;


private:
};
}

#endif
