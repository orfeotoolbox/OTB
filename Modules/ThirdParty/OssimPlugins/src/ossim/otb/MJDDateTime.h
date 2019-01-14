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


#ifndef MJDDateTime_h
#define MJDDateTime_h

#include <ossimPluginConstants.h>

namespace ossimplugins
{


class CivilDateTime;
/**
 * @brief This class represents an MJD date (Modified Julian date)
 */
class OSSIM_PLUGINS_DLL MJDDateTime
{
public:
  /**
   * @brief constructor
   */
  MJDDateTime();
  /**
   * @brief Destructor
   */
  ~MJDDateTime();
  /**
   * @brief Copy constructor
   */
  MJDDateTime(const MJDDateTime& rhs);

  /**
   * @brief Constructor with date initialization
   */
  MJDDateTime(long day, unsigned long  second, unsigned long  microsecond);

  /**
   * @brief Affectation operator
   */
  MJDDateTime& operator=(const MJDDateTime& rhs);

  long get_day()
  {
    return _day;
  };

  unsigned long get_second()
  {
    return _second;
  };

  unsigned long get_microsecond()
  {
    return _microsecond;
  };

  void set_day(long  day)
  {
    _day = day;
  };

  void set_second(unsigned long second)
  {
    _second = second;
  };

  void set_microsecond(unsigned long microsecond)
  {
    _microsecond = microsecond;
  };
protected:

  long _day;
  unsigned long _second;
  unsigned long _microsecond;


private:
};
}

#endif
