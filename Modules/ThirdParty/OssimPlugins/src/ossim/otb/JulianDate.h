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


#ifndef JulianData_h
#define JulianData_h

#include <ossimPluginConstants.h>

namespace ossimplugins
{


class CivilDateTime;
class GMSTDateTime;
class JSDDateTime;
/**
 * @brief This class represents a date in the julian referential
 */
class OSSIM_PLUGINS_DLL JulianDate
{
public:
  /**
   * @brief constructor
   */
  JulianDate();
  /**
   * @brief Destructor
   */
  ~JulianDate();
  /**
   * @brief Copy constructor
   */
  JulianDate(const JulianDate& rhs);

  /**
   * @brief Constructor with date initialization using a CivilDateTime
   */
  JulianDate(CivilDateTime& rhs);

  /**
   * @brief Constructor with date initialization using a JSDDateTime
   */
  JulianDate(JSDDateTime& rhs);
  /**
   * @brief Constructor with date initialization
   */
  JulianDate(double julianDate);
  /**
   * @brief Affectation operator
   */
  JulianDate& operator=(const JulianDate& rhs);
  /**
   * @brief Summation operator
   */
  JulianDate& operator+(const JulianDate& rhs);

  void set_julianDate(double julianDate)
  {
    _julianDate = julianDate;
  }

  double get_julianDate() const
  {
    return _julianDate;
  }

  /**
   * @brief This function converts the current JulianDate in JSDDateTime
   * @return 0 if no error or an int greater than 0 indicating the error
   * @param JSDdate [out] The class that receives the result of the conversion of the current JulianDate in JSDDateTime
   */
  int AsJSDDateTime(JSDDateTime* JSDdate);

  /**
   * @brief This function converts the current JulianDate into GMSTDateTime
   * @return 0 if no error or an int greater than 0 indicating the error
   * @param GMSTDateTime [out] The class that receives the result of the conversion of the current JulianDate into GMSTDateTime
   */
  int AsGMSTDateTime(GMSTDateTime* GMST);

  operator JSDDateTime() ;
  operator GMSTDateTime() ;


protected:

  double _julianDate;

private:
};
}

#endif

