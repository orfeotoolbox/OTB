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


#ifndef PlatformPositionData_h
#define PlatformPositionData_h

#include <RadarSat/RadarSatRecord.h>
#include <RadarSat/RadarSatRecordHeader.h>

#include <RadarSat/Leader/PositionVectorRecord.h>
#include <string>

namespace ossimplugins
{
/**
 * @ingroup PlatformPositionDataRecord
 * @brief This class is able to read a Platform position data record
 */
class PlatformPositionData : public RadarSatRecord
{
public:
  /**
   * @brief Constructor
   */
  PlatformPositionData();

  /**
   * @brief Destructor
   */
  ~PlatformPositionData();

  /**
   * @brief Copy constructor
   */
  PlatformPositionData(const PlatformPositionData& rhs);

  /**
   * @brief Copy operator
   */
  PlatformPositionData& operator=(const PlatformPositionData& rhs);
  /**
   * @brief This function writes the PlatformPositionData in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const PlatformPositionData& data);

  /**
   * @brief This function reads a PlatformPositionData from a stream
   */
  friend std::istream& operator>>(std::istream& is, PlatformPositionData& data);

  /**
   * @brief This function is able to create a new instance of the class
   */
  RadarSatRecord* Instantiate()
  {
    return new PlatformPositionData();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  RadarSatRecord* Clone()
  {
    return new PlatformPositionData(*this);
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
   * @brief Orbital elements designator
   */
  std::string   get_orbit_ele_desg()
  {
    return _orbit_ele_desg;
  };
    /**
   * @brief Orbital elements
   */
  double*   get_orbit_ele()
  {
    return _orbit_ele;
  };
    /**
   * @brief Number of data points
   */
  int  get_ndata()
  {
    return _ndata;
  };
    /**
   * @brief Year of data point
   */
  int   get_year()
  {
    return _year;
  };
    /**
   * @brief Month of data point
   */
  int   get_month()
  {
    return _month;
  };
    /**
   * @brief Day of data point
   */
  int   get_day()
  {
    return _day;
  };
    /**
   * @brief Day of year
   */
  int   get_gmt_day()
  {
    return _gmt_day;
  };
    /**
   * @brief Seconds of day
   */
  double   get_gmt_sec()
  {
    return _gmt_sec;
  };
    /**
   * @brief Data sampling interval
   */
  double   get_data_int()
  {
    return _data_int;
  };
    /**
   * @brief Reference coordinate system
   */
  std::string   get_ref_coord()
  {
    return _ref_coord;
  };
    /**
   * @brief Greenwich mean hour angle
   */
  double   get_hr_angle()
  {
    return _hr_angle;
  };
    /**
   * @brief Along track position error
   */
  double   get_alt_poserr()
  {
    return _alt_poserr;
  };
    /**
   * @brief Cross track position error
   */
  double   get_crt_poserr()
  {
    return _crt_poserr;
  };
    /**
   * @brief Radial position error
   */
  double   get_rad_poserr()
  {
    return _rad_poserr;
  };
    /**
   * @brief Along track velocity error
   */
  double   get_alt_velerr()
  {
    return _alt_velerr;
  };
    /**
   * @brief Cross track velocity error
   */
  double  get_crt_velerr()
  {
    return _crt_velerr;
  };
    /**
   * @brief Radial velocity error
   */
  double  get_rad_velerr()
  {
    return _rad_velerr;
  };
    /**
   * @brief Data point position/velocity
   */
  PositionVectorRecord* get_pos_vect()
  {
    return _pos_vect;
  };

protected:
  /**
   * @brief Orbital elements designator
   */
  std::string   _orbit_ele_desg;
    /**
   * @brief Orbital elements
   */
  double   _orbit_ele[6];
    /**
   * @brief Number of data points
   */
  int   _ndata;
    /**
   * @brief Year of data point
   */
  int   _year;
    /**
   * @brief Month of data point
   */
  int   _month;
    /**
   * @brief Day of data point
   */
  int   _day;
    /**
   * @brief Day of year
   */
  int   _gmt_day;
    /**
   * @brief Seconds of day
   */
  double   _gmt_sec;
    /**
   * @brief Data sampling interval
   */
  double   _data_int;
    /**
   * @brief Reference coordinate system
   */
  std::string   _ref_coord;
    /**
   * @brief Greenwich mean hour angle
   */
  double   _hr_angle;
    /**
   * @brief Along track position error
   */
  double   _alt_poserr;
    /**
   * @brief Cross track position error
   */
  double   _crt_poserr;
    /**
   * @brief Radial position error
   */
  double   _rad_poserr;
    /**
   * @brief Along track velocity error
   */
  double   _alt_velerr;
    /**
   * @brief Cross track velocity error
   */
  double   _crt_velerr;
    /**
   * @brief Radial velocity error
   */
  double   _rad_velerr;
    /**
   * @brief Data point position/velocity
   */
  PositionVectorRecord _pos_vect[64];

private:
};
}
#endif
