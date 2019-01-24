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


#ifndef CompensationDataRecord_h
#define CompensationDataRecord_h


#include <RadarSat/RadarSatRecord.h>
#include <RadarSat/RadarSatRecordHeader.h>
#include <string>

namespace ossimplugins
{
/**
 * @ingroup RadiometricCompensationDataRecord
 * @brief This class is able to read a Compensation data record
 */
class CompensationDataRecord
{
public:
  /**
   * @brief Constructor
   */
  CompensationDataRecord();

  /**
   * @brief Destructor
   */
  ~CompensationDataRecord();

  /**
   * @brief Copy constructor
   */
  CompensationDataRecord(const CompensationDataRecord& rhs);

  /**
   * @brief Copy operator
   */
  CompensationDataRecord& operator=(const CompensationDataRecord& rhs);
  /**
   * @brief This function writes the CompensationDataRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const CompensationDataRecord& data);

  /**
   * @brief This function reads a CompensationDataRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, CompensationDataRecord& data);

  /**
   * @brief Compensation data designator
   */
  std::string   get_comp_desig()
  {
    return _comp_desig;
  };
    /**
   * @brief Compensation data descriptor
   */
  std::string   get_comp_descr()
  {
    return _comp_descr;
  };
    /**
   * @brief Number of compensation records
   */
  int   get_n_comp_rec()
  {
    return _n_comp_rec;
  };
    /**
   * @brief Compensation record sequence number
   */
  int   get_comp_seq_no()
  {
    return _comp_seq_no;
  };
    /**
   * @brief Number of beam table entries
   */
  int   get_beam_tab_size()
  {
    return _beam_tab_size;
  };
    /**
   * @brief Elevation gain beam profile
   */
  double*  get_beam_tab()
  {
    return _beam_tab;
  };
    /**
   * @brief Beam type
   */
  std::string   get_beam_type()
  {
    return _beam_type;
  };
    /**
   * @brief Look angle of beam table centre
   */
  double   get_look_angle()
  {
    return _look_angle;
  };
    /**
   * @brief Increment between beam table entries
   */
  double   get_beam_tab_inc()
  {
    return _beam_tab_inc;
  };

protected:
  /**
   * @brief Compensation data designator
   */
  std::string   _comp_desig;
    /**
   * @brief Compensation data descriptor
   */
  std::string   _comp_descr;
    /**
   * @brief Number of compensation records
   */
  int   _n_comp_rec;
    /**
   * @brief Compensation record sequence number
   */
  int   _comp_seq_no;
    /**
   * @brief Number of beam table entries
   */
  int   _beam_tab_size;
    /**
   * @brief Elevation gain beam profile
   */
  double  _beam_tab[256];
    /**
   * @brief Beam type
   */
  std::string   _beam_type;
    /**
   * @brief Look angle of beam table centre
   */
  double   _look_angle;
    /**
   * @brief Increment between beam table entries
   */
  double   _beam_tab_inc;
private:
};
}
#endif
