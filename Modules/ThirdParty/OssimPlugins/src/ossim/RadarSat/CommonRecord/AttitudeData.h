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


#ifndef AttitudeData_h
#define AttitudeData_h


#include <RadarSat/RadarSatRecord.h>
#include <RadarSat/RadarSatRecordHeader.h>

#include "DataPointDataRecord.h"

namespace ossimplugins
{
/**
 * @ingroup AttitudeDataRecord
 * @brief This class is able to read a Attitude data record
 */
class AttitudeData : public RadarSatRecord
{
public:
  /**
   * @brief Constructor
   */
  AttitudeData();

  /**
   * @brief Destructor
   */
  ~AttitudeData();

  /**
   * @brief Copy constructor
   */
  AttitudeData(const AttitudeData& rhs);

  /**
   * @brief Copy operator
   */
  AttitudeData& operator=(const AttitudeData& rhs);
  /**
   * @brief This function writes the AttitudeData in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const AttitudeData& data);

  /**
   * @brief This function reads a AttitudeData from a stream
   */
  friend std::istream& operator>>(std::istream& is, AttitudeData& data);

  /**
   * @brief This function is able to create a new instance of the class
   */
  RadarSatRecord* Instantiate()
  {
    return new AttitudeData();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  RadarSatRecord* Clone()
  {
    return new AttitudeData(*this);
  };

  /**
   * @brief Reads the class data from a stream
   */
  void Read(std::istream& is)
  {
    is>>*this;
  };

  /**
   * @brief Writes the class to a stream
   */
  void Write(std::ostream& os)
  {
    os<<*this;
  };

  /**
   * @brief Number of data points
   */
  int   get_npoint()
  {
    return _npoint;
  };
    /**
   * @brief Data points
   */
  DataPointDataRecord* get_att_vect()
  {
    return _att_vect;
  };
    /**
   * @brief Pitch bias (degrees)
   */
  double   get_pitch_bias()
  {
    return _pitch_bias;
  };
    /**
   * @brief Roll bias (degrees)
   */
  double   get_roll_bias()
  {
    return _roll_bias;
  };
    /**
   * @brief Yaw bias (degrees)
   */
  double   get_yaw_bias()
  {
    return _yaw_bias;
  };


protected:
  /**
   * @brief Number of data points
   */
  int   _npoint;
    /**
   * @brief Data points
   */
  DataPointDataRecord _att_vect[20];
    /**
   * @brief Pitch bias (degrees)
   */
  double   _pitch_bias;
    /**
   * @brief Roll bias (degrees)
   */
  double   _roll_bias;
    /**
   * @brief Yaw bias (degrees)
   */
  double   _yaw_bias;


private:
};
}
#endif
