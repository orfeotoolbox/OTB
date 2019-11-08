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


#ifndef ChirpParameters_h
#define ChirpParameters_h

#include <EnvisatAsar/EnvisatAsarRecord.h>
#include <string>

namespace ossimplugins
{
/**
 * @ingroup ChirpParametersRecord
 * @brief This class is able to read the ASAR ChirpParameters record
 */
class ChirpParameters : public EnvisatAsarRecord
{
public:
  /**
   * @brief Constructor
   */
  ChirpParameters();

  /**
   * @brief Destructor
   */
  virtual ~ChirpParameters();

  /**
   * @brief This function writes the ChirpParameters in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const ChirpParameters& data);

  /**
   * @brief This function reads a ChirpParameters from a stream
   */
  friend std::istream& operator>>(std::istream& is, ChirpParameters& data);

  /**
   * @brief Copy constructor
   */
  ChirpParameters(const ChirpParameters& rhs);

  /**
   * @brief Copy operator
   */
  ChirpParameters& operator=(const ChirpParameters& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  EnvisatAsarRecord* Instantiate()
  {
    return new ChirpParameters();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  EnvisatAsarRecord* Clone()
  {
    return new ChirpParameters(*this);
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
  * @brief zero_doppler_time
  */
  std::string   get_zero_doppler_time()
  {
    return _zero_doppler_time;
  };
  /**
  * @brief attach_flag
  */
  bool   get_attach_flag()
  {
    return _attach_flag;
  };
  /**
  * @brief swath
  */
  std::string   get_swath()
  {
    return _swath;
  };
  /**
  * @brief polar
  */
  std::string   get_polar()
  {
    return _polar;
  };
  /**
  * @brief chirp_width
  */
  float   get_chirp_width()
  {
    return _chirp_width;
  };
  /**
  * @brief chirp_sidelobe
  */
  float   get_chirp_sidelobe()
  {
    return _chirp_sidelobe;
  };
  /**
  * @brief chirp_islr
  */
  float   get_chirp_islr()
  {
    return _chirp_islr;
  };
  /**
  * @brief chirp_peak_loc
  */
  float   get_chirp_peak_loc()
  {
    return _chirp_peak_loc;
  };
  /**
  * @brief re_chirp_power
  */
  float   get_re_chirp_power()
  {
    return _re_chirp_power;
  };
  /**
  * @brief elev_chirp_power
  */
  float   get_elev_chirp_power()
  {
    return _elev_chirp_power;
  };
  /**
  * @brief chirp_quality_flag
  */
  bool   get_chirp_quality_flag()
  {
    return _chirp_quality_flag;
  };
  /**
  * @brief ref_chirp_power
  */
  float   get_ref_chirp_power()
  {
    return _ref_chirp_power;
  };
  /**
  * @brief normalisation_source
  */
  std::string   get_normalisation_source()
  {
    return _normalisation_source;
  };
  /**
  * @brief cal_pulse_info[352]
  */
  float*   get_cal_pulse_info()
  {
    return _cal_pulse_info;
  };


  /**
  * @brief zero_doppler_time
  */
  std::string   _zero_doppler_time;
  /**
  * @brief attach_flag
  */
  bool   _attach_flag;
  /**
  * @brief swath
  */
  std::string   _swath;
  /**
  * @brief polar
  */
  std::string   _polar;
  /**
  * @brief chirp_width
  */
  float   _chirp_width;
  /**
  * @brief chirp_sidelobe
  */
  float   _chirp_sidelobe;
  /**
  * @brief chirp_islr
  */
  float   _chirp_islr;
  /**
  * @brief chirp_peak_loc
  */
  float   _chirp_peak_loc;
  /**
  * @brief re_chirp_power
  */
  float   _re_chirp_power;
  /**
  * @brief elev_chirp_power
  */
  float   _elev_chirp_power;
  /**
  * @brief chirp_quality_flag
  */
  bool   _chirp_quality_flag;
  /**
  * @brief ref_chirp_power
  */
  float   _ref_chirp_power;
  /**
  * @brief normalisation_source
  */
  std::string   _normalisation_source;
  /**
  * @brief cal_pulse_info[352]
  */
  float   _cal_pulse_info[352];

private:

};
}
#endif


