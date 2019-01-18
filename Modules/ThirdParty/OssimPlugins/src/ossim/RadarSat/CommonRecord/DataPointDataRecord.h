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


#ifndef DataPointDataRecord_h
#define DataPointDataRecord_h


#include <RadarSat/RadarSatRecord.h>
#include <RadarSat/RadarSatRecordHeader.h>

namespace ossimplugins
{
/**
 * @ingroup AttitudeDataRecord
 * @brief This class is able to read a Data point data record
 */
class DataPointDataRecord
{
public:
  /**
   * @brief Constructor
   */
  DataPointDataRecord();

  /**
   * @brief Destructor
   */
  ~DataPointDataRecord();

  /**
   * @brief Copy constructor
   */
  DataPointDataRecord(const DataPointDataRecord& rhs);

  /**
   * @brief Copy operator
   */
  DataPointDataRecord& operator=(const DataPointDataRecord& rhs);
  /**
   * @brief This function writes the DataPointDataRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const DataPointDataRecord& data);

  /**
   * @brief This function reads a DataPointDataRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, DataPointDataRecord& data);

  /**
   * @brief Day of the year, GMT
   */
  int   get_gmt_day()
  {
    return _gmt_day;
  };
    /**
   * @brief Milliseconds of day, GMT
   */
  int  get_gmt_sec()
  {
    return _gmt_sec;
  };
    /**
   * @brief Pitch data quality flag
   */
  int   get_pitch_flag()
  {
    return _pitch_flag;
  };
    /**
   * @brief Roll data quality flag
   */
  int   get_roll_flag()
  {
    return _roll_flag;
  };
    /**
   * @brief Yaw data quality flag
   */
  int   get_yaw_flag()
  {
    return _yaw_flag;
  };
    /**
   * @brief Pitch error, degrees
   */
  double   get_pitch()
  {
    return _pitch;
  };
    /**
   * @brief Roll error, degrees
   */
  double   get_roll()
  {
    return _roll;
  };
    /**
   * @brief Yaw error, degrees
   */
  double   get_yaw()
  {
    return _yaw;
  };
    /**
   * @brief Pitch rate data quality flag
   */
  int   get_pitch_rate_flag()
  {
    return _pitch_rate_flag;
  };
    /**
   * @brief Roll rate data quality fla
   */
  int   get_roll_rate_flag()
  {
    return _roll_rate_flag;
  };
    /**
   * @brief Yaw rate data quality flag
   */
  int   get_yaw_rate_flag()
  {
    return _yaw_rate_flag;
  };
    /**
   * @brief Pitch rate, degrees/sec
   */
  double   get_pitch_rate()
  {
    return _pitch_rate;
  };
    /**
   * @brief Roll rate, degrees/sec
   */
  double   get_roll_rate()
  {
    return _roll_rate;
  };
    /**
   * @brief Yaw rate, degrees/sec
   */
  double   get_yaw_rate()
  {
    return _yaw_rate;
  };

protected:
  /**
   * @brief Day of the year, GMT
   */
  int   _gmt_day;
    /**
   * @brief Milliseconds of day, GMT
   */
  int   _gmt_sec;
    /**
   * @brief Pitch data quality flag
   */
  int   _pitch_flag;
    /**
   * @brief Roll data quality flag
   */
  int   _roll_flag;
    /**
   * @brief Yaw data quality flag
   */
  int   _yaw_flag;
    /**
   * @brief Pitch error, degrees
   */
  double   _pitch;
    /**
   * @brief Roll error, degrees
   */
  double   _roll;
    /**
   * @brief Yaw error, degrees
   */
  double   _yaw;
    /**
   * @brief Pitch rate data quality flag
   */
  int   _pitch_rate_flag;
    /**
   * @brief Roll rate data quality fla
   */
  int   _roll_rate_flag;
    /**
   * @brief Yaw rate data quality flag
   */
  int   _yaw_rate_flag;
    /**
   * @brief Pitch rate, degrees/sec
   */
  double   _pitch_rate;
    /**
   * @brief Roll rate, degrees/sec
   */
  double   _roll_rate;
    /**
   * @brief Yaw rate, degrees/sec
   */
  double   _yaw_rate;
private:
};
}
#endif
