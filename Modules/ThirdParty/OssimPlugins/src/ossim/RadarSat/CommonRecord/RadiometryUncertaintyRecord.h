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


#ifndef RadiometryUncertaintyRecord_h
#define RadiometryUncertaintyRecord_h

#include<iostream>
#include <RadarSat/RadarSatRecord.h>

namespace ossimplugins
{
/**
 * @ingroup DataQualitySummaryRecord
 * @brief This class is able to read a radiometry uncertainty record
 */
class RadiometryUncertaintyRecord
{
public:
  /**
   * @brief Constructor
   */
  RadiometryUncertaintyRecord();

  /**
   * @brief Destructor
   */
  ~RadiometryUncertaintyRecord();

  /**
   * @brief Copy constructor
   */
  RadiometryUncertaintyRecord(const RadiometryUncertaintyRecord& rhs);

  /**
   * @brief Copy operator
   */
  RadiometryUncertaintyRecord& operator=(const RadiometryUncertaintyRecord& rhs);
  /**
   * @brief This function writes the RadiometryUncertaintyRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const RadiometryUncertaintyRecord& data);

  /**
   * @brief This function reads a RadiometryUncertaintyRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, RadiometryUncertaintyRecord& data);

  /**
   * @brief Units of db
   */
  double   get_db()
  {
    return _db;
  };
    /**
   * @brief Units of deg
   */
  double   get_deg()
  {
    return _deg;
  };
protected:

  /**
   * @brief Units of db
   */
  double   _db;
    /**
   * @brief Units of deg
   */
  double   _deg;
private:


};
}
#endif
