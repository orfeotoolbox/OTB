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


#ifndef PositionVectorRecord_h
#define PositionVectorRecord_h

#include <RadarSat/RadarSatRecord.h>

namespace ossimplugins
{
/**
 * @ingroup PlatformPositionDataRecord
 * @brief This class is able to read a position vector record
 */
class PositionVectorRecord
{
public:
  /**
   * @brief Constructor
   */
  PositionVectorRecord();

  /**
   * @brief Destructor
   */
  ~PositionVectorRecord();

  /**
   * @brief Copy constructor
   */
  PositionVectorRecord(const PositionVectorRecord& rhs);

  /**
   * @brief Copy operator
   */
  PositionVectorRecord& operator=(const PositionVectorRecord& rhs);
  /**
   * @brief This function writes the PositionVectorRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const PositionVectorRecord& data);

  /**
   * @brief This function reads a PositionVectorRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, PositionVectorRecord& data);

  /**
   * @brief Data point position (m)
   */
  double* get_pos()
  {
    return _pos;
  };

  /**
   * @brief Data point velocity (mm/s)
   */
  double* get_vel()
  {
    return _vel;
  };

protected:
  /**
   * @brief Data point position (m)
   */
  double _pos[3];
  /**
   * @brief Data point velocity (mm/s)
   */
  double _vel[3];
private:

};
}
#endif
