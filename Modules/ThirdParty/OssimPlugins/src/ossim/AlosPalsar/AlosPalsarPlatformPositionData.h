/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2008-2010 by Centre for Remote Imaging, Sensing and Processing (CRISP)
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


#ifndef AlosPalsarPlatformPositionData_h
#define AlosPalsarPlatformPositionData_h

#include <cstdlib>
#include <AlosPalsar/AlosPalsarRecord.h>
#include <AlosPalsar/AlosPalsarRecordHeader.h>

#include <AlosPalsar/AlosPalsarPositionVectorRecord.h>
#include <string>

namespace ossimplugins
{

/**
 * @ingroup PlatformPositionDataRecord
 * @brief This class is able to read a Platform position data record
 */
class AlosPalsarPlatformPositionData : public AlosPalsarRecord
{
public:
  /**
   * @brief Constructor
   */
  AlosPalsarPlatformPositionData();

  /**
   * @brief Destructor
   */
  virtual ~AlosPalsarPlatformPositionData();

  /**
   * @brief Copy constructor
   */
  AlosPalsarPlatformPositionData(const AlosPalsarPlatformPositionData& rhs);

  /**
   * @brief Copy operator
   */
  AlosPalsarPlatformPositionData& operator=(const AlosPalsarPlatformPositionData& rhs);
  /**
   * @brief This function write the AlosPalsar PlatformPositionData in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const AlosPalsarPlatformPositionData& data);

  /**
   * @brief This function read a AlosPalsar PlatformPositionData from a stream
   */
  friend std::istream& operator>>(std::istream& is, AlosPalsarPlatformPositionData& data);

  /**
   * @brief This function is able to create a new instance of the class
   */
  AlosPalsarRecord* Instantiate()
  {
    return new AlosPalsarPlatformPositionData();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  AlosPalsarRecord* Clone()
  {
    return new AlosPalsarPlatformPositionData(*this);
  };

  /**
   * @brief Read the class data from a stream
   */
  void Read(std::istream& is)
  {
    is >> *this;
  };

  /**
   * @brief Write the class to a stream
   */
  void Write(std::ostream& os)
  {
    os << *this;
  };

  /**
   * @brief Orbital elements designator
   */
  std::string   get_orbit_ele_desg() const
  {
    return _orbit_ele_desg;
  };
  /**
  * @brief Orbital elements
  */
  const double*   get_orbit_ele() const
  {
    return _orbit_ele;
  };
  /**
  * @brief Number of data points
  */
  int  get_ndata() const
  {
    return _ndata;
  };
  /**
  * @brief Year of data point
  */
  int   get_year() const
  {
    return _year;
  };
  /**
  * @brief Month of data point
  */
  int   get_month() const
  {
    return _month;
  };
  /**
  * @brief Day of data point
  */
  int   get_day() const
  {
    return _day;
  };
  /**
  * @brief Day of year
  */
  int   get_gmt_day() const
  {
    return _gmt_day;
  };
  /**
  * @brief Seconds of day
  */
  double   get_gmt_sec() const
  {
    return _gmt_sec;
  };
  /**
  * @brief Data sampling interval
  */
  double   get_data_int() const
  {
    return _data_int;
  };
  /**
  * @brief Reference coordinate system
  */
  std::string   get_ref_coord() const
  {
    return _ref_coord;
  };
  /**
  * @brief Greenwich mean hour angle
  */
  double   get_hr_angle() const
  {
    return _hr_angle;
  };
  /**
  * @brief Along track position error
  */
  double   get_alt_poserr() const
  {
    return _alt_poserr;
  };
  /**
  * @brief Cross track position error
  */
  double   get_crt_poserr() const
  {
    return _crt_poserr;
  };
  /**
  * @brief Radial position error
  */
  double   get_rad_poserr() const
  {
    return _rad_poserr;
  };
  /**
  * @brief Along track velocity error
  */
  double   get_alt_velerr() const
  {
    return _alt_velerr;
  };
  /**
  * @brief Cross track velocity error
  */
  double  get_crt_velerr() const
  {
    return _crt_velerr;
  };
  /**
  * @brief Radial velocity error
  */
  double  get_rad_velerr() const
  {
    return _rad_velerr;
  };
  /**
  * @brief Data point position/velocity
  */
  const AlosPalsarPositionVectorRecord* get_pos_vect() const
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
  AlosPalsarPositionVectorRecord _pos_vect[64];

private:
};
}
#endif
