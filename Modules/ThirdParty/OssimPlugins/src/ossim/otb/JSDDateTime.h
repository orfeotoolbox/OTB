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


#ifndef JSDDateTime_h
#define JSDDateTime_h

#include <ossimPluginConstants.h>
#include <otb/JulianDate.h>
#include <otb/MJDDateTime.h>

namespace ossimplugins
{


class CivilDateTime;
class GMSTDateTime;
class JulianDate;

/**
 * @brief This class represents a date
 */
class OSSIM_PLUGINS_DLL JSDDateTime
{
public:
   /**
    * @brief constructor
    */
   JSDDateTime();
   /**
    * @brief Destructor
    */
   ~JSDDateTime();
   /**
    * @brief Copy constructor
    */
   JSDDateTime(const JSDDateTime& rhs);

   /**
    * @brief Constructor with date initialization
    */
   JSDDateTime(JulianDate day0hTU, double second, double decimal);

   /**
    * @brief Constructor with date initialization using a CivilDateTime
    */
   JSDDateTime(CivilDateTime& rhs);

   /**
    * @brief Constructor with date initialization using a CivilDateTime
    */
   JSDDateTime(JulianDate& rhs);

   /**
    * @brief Constructor with date initialization using a MJDDateTime (Modified Julian Date - Used for ENVISAT)
    */
   JSDDateTime(MJDDateTime& rhs);

   /**
    * @brief Affectation operator
    */
   JSDDateTime& operator=(const JSDDateTime& rhs);

   JulianDate get_day0hTU() const
   {
      return _day0hTU;
   }

   double get_second() const
   {
      return _second;
   }

   double get_decimal() const
   {
      return _decimal;
   }

   void set_day0hTU(JulianDate  day)
   {
      _day0hTU = day;
   }

   void set_second(double second)
   {
      _second = second;
   }

   void set_decimal(double decimal)
   {
      _decimal = decimal;
   }

   void NormDate() ;

   /**
    * @brief This function converts the current JSDDateTime into Julian Date
    * @return 0 if no error or an int greater than 0 indicating the error
    * @param julianDate [out] The class that receives the result of the conversion of the current JSDDateTime in JulianDate
    */
   int AsJulianDate(JulianDate* julian);

   /**
    * @brief This function converts the current JSDDateTime into GMST Date
    * @return 0 if no error or an int greater than 0 indicating the error
    * @param julianDate [out] The class that receives the result of the conversion of the current JSDDateTime into GMST Date
    */
   int AsGMSTDateTime(GMSTDateTime* GMST);

   operator JulianDate() ;
   operator GMSTDateTime() ;

protected:

   /**
    * @brief Julian day at 0h00 UT
    */
   JulianDate _day0hTU;
   double _second;
   double _decimal;


private:
};
}

#endif
