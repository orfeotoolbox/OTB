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


#ifndef MisregistrationRecord_h
#define MisregistrationRecord_h


#include <RadarSat/RadarSatRecord.h>

namespace ossimplugins
{
/**
 * @ingroup DataQualitySummaryRecord
 * @brief This class is able to read a misregistration record
 */
class MisregistrationRecord
{
public:
  /**
   * @brief Constructor
   */
  MisregistrationRecord();

  /**
   * @brief Destructor
   */
  ~MisregistrationRecord();

  /**
   * @brief Copy constructor
   */
  MisregistrationRecord(const MisregistrationRecord& rhs);

  /**
   * @brief Copy operator
   */
  MisregistrationRecord& operator=(const MisregistrationRecord& rhs);
  /**
   * @brief This function writes the MisregistrationRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const MisregistrationRecord& data);

  /**
   * @brief This function reads a MisregistrationRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, MisregistrationRecord& data);

  /**
   * @brief Nominal along track misregistration
   */
  double   get_alt_m()
  {
    return _alt_m;
  };
    /**
   * @brief Nominal cross track misregistration
   */
  double   get_crt_m()
  {
    return _crt_m;
  };
protected:

  /**
   * @brief Nominal along track misregistration
   */
  double   _alt_m;
    /**
   * @brief Nominal cross track misregistration
   */
  double   _crt_m;
private:


};
}
#endif
