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


#ifndef SRGRCoefficientSetRecord_h
#define SRGRCoefficientSetRecord_h

#include <RadarSat/RadarSatRecord.h>
#include <string>

namespace ossimplugins
{
/**
 * @ingroup ProcessingParametersRecord
 * @brief This class is able to read a SRGR coefficients set record
 */
class SRGRCoefficientSetRecord
{
public:
  /**
   * @brief Constructor
   */
  SRGRCoefficientSetRecord();

  /**
   * @brief Destructor
   */
  ~SRGRCoefficientSetRecord();

  /**
   * @brief Copy constructor
   */
  SRGRCoefficientSetRecord(const SRGRCoefficientSetRecord& rhs);

  /**
   * @brief Copy operator
   */
  SRGRCoefficientSetRecord& operator=(const SRGRCoefficientSetRecord& rhs);

  /**
   * @brief This function writes the SRGRCoefficientSetRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const SRGRCoefficientSetRecord& data);

  /**
   * @brief This function reads a SRGRCoefficientSetRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, SRGRCoefficientSetRecord& data);

  /**
   * @brief SRGR update date/time
   */
  std::string   get_srgr_update()
  {
    return _srgr_update;
  }

    /**
   * @brief SRGR coefficients
   */
  double*   get_srgr_coef()
  {
    return _srgr_coef;
  }

protected:
  /**
   * @brief SRGR update date/time
   */
  std::string   _srgr_update;

    /**
   * @brief SRGR coefficients
   */
  double   _srgr_coef[6];
private:

};
}
#endif
