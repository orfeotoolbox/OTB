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


#ifndef GeolocationGrid_h
#define GeolocationGrid_h

#include <EnvisatAsar/EnvisatAsarRecord.h>
#include <string>

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
  EnvisatAsarRecord* Instantiate()
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


