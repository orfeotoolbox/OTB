//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
// "Copyright Centre for Remote Imaging, Sensing and Processing"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef AlosPalsarPlatformPositionData_h
#define AlosPalsarPlatformPositionData_h

#include <iostream>
#include <cstdlib>
#include <AlosPalsar/AlosPalsarRecord.h>
#include <AlosPalsar/AlosPalsarRecordHeader.h>

#include <AlosPalsar/AlosPalsarPositionVectorRecord.h>

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
  AlosPalsarRecord* Instanciate()
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
