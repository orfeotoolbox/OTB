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


#ifndef RadiometricData_h
#define RadiometricData_h

#include <RadarSat/RadarSatRecord.h>
#include <RadarSat/RadarSatRecordHeader.h>
#include <string>

namespace ossimplugins
{
/**
 * @ingroup RadiometricDataRecord
 * @brief This class is able to read a radiometric data record
 */
class RadiometricData : public RadarSatRecord
{
public:
  /**
   * @brief Constructor
   */
  RadiometricData();

  /**
   * @brief Destructor
   */
  ~RadiometricData();

  /**
   * @brief Copy constructor
   */
  RadiometricData(const RadiometricData& rhs);

  /**
   * @brief Copy operator
   */
  RadiometricData& operator=(const RadiometricData& rhs);
  /**
   * @brief This function writes the RadiometricData in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const RadiometricData& data);

  /**
   * @brief This function reads a RadiometricData from a stream
   */
  friend std::istream& operator>>(std::istream& is, RadiometricData& data);

  /**
   * @brief This function is able to create a new instance of the class
   */
  RadarSatRecord* Instantiate()
  {
    return new RadiometricData();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  RadarSatRecord* Clone()
  {
    return new RadiometricData(*this);
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
   * @brief Record sequence number
   */
  int   get_seq_num()
  {
    return _seq_num;
  };
    /**
   * @brief Number of data fields
   */
  int   get_n_data()
  {
    return _n_data;
  };
    /**
   * @brief Field size in bytes
   */
  int   get_field_size()
  {
    return _field_size;
  };

  /**
   * @brief SAR channel indicator
   */
  std::string   get_chan_ind()
  {
    return _chan_ind;
  };

    /**
   * @brief Table designator
   */
  std::string   get_table_desig()
  {
    return _table_desig;
  };
    /**
   * @brief Number of lookup table samples
   */
  int   get_n_samp()
  {
    return _n_samp;
  };
    /**
   * @brief Sample type designator
   */
  std::string   get_samp_type()
  {
    return _samp_type;
  };
    /**
   * @brief Increment between table entries, range samples
   */
  int   get_samp_inc()
  {
    return _samp_inc;
  };
    /**
   * @brief Output scaling gain table
   */
  double*   get_lookup_tab()
  {
    return _lookup_tab;
  };

    /**
   * @brief Thermal noise reference level
   */
  double   get_noise_scale()
  {
    return _noise_scale;
  };

    /**
   * @brief Scaling offset
   */
  double   get_offset()
  {
    return _offset;
  };
    /**
   * @brief Calibration constant
   */
  double   get_calib_const()
  {
    return _calib_const;
  };


protected:
  /**
   * @brief Record sequence number
   */
  int   _seq_num;
    /**
   * @brief Number of data fields
   */
  int   _n_data;
    /**
   * @brief Field size in bytes
   */
  int   _field_size;

  /**
   * @brief SAR channel indicator
   */
  std::string   _chan_ind;

    /**
   * @brief Table designator
   */
  std::string   _table_desig;
    /**
   * @brief Number of lookup table samples
   */
  int   _n_samp;
    /**
   * @brief Sample type designator
   */
  std::string   _samp_type;
    /**
   * @brief Increment between table entries, range samples
   */
  int   _samp_inc;
    /**
   * @brief Output scaling gain table
   */
  double   _lookup_tab[512];

    /**
   * @brief Thermal noise reference level
   */
  double   _noise_scale;

    /**
   * @brief Scaling offset
   */
  double   _offset;
    /**
   * @brief Calibration constant
   */
  double   _calib_const;

private:
};
}
#endif
