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


#ifndef GMSTDateTime_h
#define GMSTDateTime_h

#include <ossimPluginConstants.h>
#include <otb/JulianDate.h>

namespace ossimplugins
{


class CivilDateTime;
class JulianDate;

/**
 * @ingroup Date
 * @brief This class represents a date
 */
class OSSIM_PLUGINS_DLL GMSTDateTime
{
public:

  enum Ori_TSM_Green { AN1900 , AN1950 , AN2000 } ;

  /**
   * @brief constructor
   */
  GMSTDateTime();
  /**
   * @brief Destructor
   */
  ~GMSTDateTime();
  /**
   * @brief Copy constructor
   */
  GMSTDateTime(const GMSTDateTime& rhs);

  /**
   * @brief Constructor with date initialization
   */
  GMSTDateTime(double tsm, Ori_TSM_Green origine);

  /**
   * @brief Constructor with date initialization using a CivilDateTime
   */
  GMSTDateTime(CivilDateTime& rhs);

  /**
   * @brief Affectation operator
   */
  GMSTDateTime& operator=(const GMSTDateTime& rhs);

  double get_tms()
  {
    return _tms;
  };

  void set_tms(double  tms)
  {
    _tms = tms;
  };

  Ori_TSM_Green get_origine()
  {
    return _origine;
  };

  void set_origine(Ori_TSM_Green origine)
  {
    _origine = origine;
  }
protected:

  double _tms;
  Ori_TSM_Green _origine;

private:
};
}

#endif
