//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef GeolocationGrid_h
#define GeolocationGrid_h

#include <iostream>
#include <EnvisatAsar/EnvisatAsarRecord.h>

namespace ossimplugins
{
/**
 * @ingroup GeolocationGridRecord
 * @brief This class is able to read the ASAR GeolocationGrid record
 */
class GeolocationGrid : public EnvisatAsarRecord
{
public:
  /**
   * @brief Constructor
   */
  GeolocationGrid();

  /**
   * @brief Destructor
   */
  virtual ~GeolocationGrid();

  /**
   * @brief This function writes the GeolocationGrid in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const GeolocationGrid& data);

  /**
   * @brief This function reads a GeolocationGrid from a stream
   */
  friend std::istream& operator>>(std::istream& is, GeolocationGrid& data);

  /**
   * @brief Copy constructor
   */
  GeolocationGrid(const GeolocationGrid& rhs);

  /**
   * @brief Copy operator
   */
  GeolocationGrid& operator=(const GeolocationGrid& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  EnvisatAsarRecord* Instanciate()
  {
    return new GeolocationGrid();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  EnvisatAsarRecord* Clone()
  {
    return new GeolocationGrid(*this);
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
  * @brief first_zero_doppler_time day
  */
  int  get_first_zero_doppler_time_day()
  {
    return _first_zero_doppler_time_day;
  };
  /**
  * @brief first_zero_doppler_time second
  */
  unsigned int  get_first_zero_doppler_time_sec()
  {
    return _first_zero_doppler_time_sec;
  };
  /**
  * @brief first_zero_doppler_time micro sec
  */
  unsigned int  get_first_zero_doppler_time_microsec()
  {
    return _first_zero_doppler_time_microsec;
  };
  /**
  * @brief attach_flag
  */
  bool   get_attach_flag()
  {
    return _attach_flag;
  };
  /**
  * @brief line_num
  */
  unsigned int   get_line_num()
  {
    return _line_num;
  };
  /**
  * @brief num_lines
  */
  unsigned int   get_num_lines()
  {
    return _num_lines;
  };
  /**
  * @brief sub_sat_track
  */
  float   get_sub_sat_track()
  {
    return _sub_sat_track;
  };
  /**
  * @brief samp_numbvers
  */
  unsigned int *  get_samp_numbers()
  {
    return _samp_numbers;
  };
  /**
  * @brief slant_range_times
  */
  float *  get_slant_range_times()
  {
    return _slant_range_times;
  };
  /**
  * @brief angles
  */
  float *  get_angles()
  {
    return _angles;
  };
  /**
  * @brief lats
  */
  int *  get_lats()
  {
    return _lats;
  };
  /**
  * @brief longs
  */
  int *  get_longs()
  {
    return _longs;
  };
  /**
  * @brief last_zero_doppler_time
  */
  std::string   get_last_zero_doppler_time()
  {
    return _last_zero_doppler_time;
  };
  /**
  * @brief last_line_tie_points
  */
  std::string   get_last_line_tie_points()
  {
    return _last_line_tie_points;
  };


  /**
  * @brief first_zero_doppler_time day
  */
  int   _first_zero_doppler_time_day;
  /**
  * @brief first_zero_doppler_time sec
  */
  unsigned int   _first_zero_doppler_time_sec;
  /**
  * @brief first_zero_doppler_time microsec
  */
  unsigned int   _first_zero_doppler_time_microsec;
  /**
  * @brief attach_flag
  */
  bool   _attach_flag;
  /**
  * @brief line_num
  */
  unsigned int   _line_num;
  /**
  * @brief num_lines
  */
  unsigned int   _num_lines;
  /**
  * @brief sub_sat_track
  */
  float   _sub_sat_track;
  /**
  * @brief samp_numbers
  */
  unsigned int  _samp_numbers[11];
  /**
  * @brief slant_range_times[11]
  */
  float  _slant_range_times[11];
  /**
  * @brief angles[11]
  */
  float  _angles[11];
  /**
  * @brief lats[11]
  */
  int  _lats[11];
  /**
  * @brief longs[11]
  */
  int  _longs[11];
  /**
  * @brief last_zero_doppler_time
  */
  std::string   _last_zero_doppler_time;
  /**
  * @brief last_line_tie_points
  */
  std::string   _last_line_tie_points;


private:

};
}
#endif


