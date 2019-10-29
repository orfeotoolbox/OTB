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


#ifndef DopplerCentroidEstimateRecord_h
#define DopplerCentroidEstimateRecord_h


#include <RadarSat/RadarSatRecord.h>

namespace ossimplugins
{
/**
 * @ingroup ProcessingParametersRecord
 * @brief This class is able to read a Doppler centroid estimates record
 */
class DopplerCentroidEstimateRecord
{
public:
  /**
   * @brief Constructor
   */
  DopplerCentroidEstimateRecord();

  /**
   * @brief Destructor
   */
  ~DopplerCentroidEstimateRecord();

  /**
   * @brief Copy constructor
   */
  DopplerCentroidEstimateRecord(const DopplerCentroidEstimateRecord& rhs);

  /**
   * @brief Copy operator
   */
  DopplerCentroidEstimateRecord& operator=(const DopplerCentroidEstimateRecord& rhs);

  /**
   * @brief This function writes the DopplerCentroidEstimateRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const DopplerCentroidEstimateRecord& data);

  /**
   * @brief This function reads a DopplerCentroidEstimateRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, DopplerCentroidEstimateRecord& data);

  /**
   * @brief Doppler centroid confidence measure
   */
  double   get_dopcen_conf()
  {
    return _dopcen_conf;
  }
    /**
   * @brief Doppler centroid reference time (sec)
   */
  double   get_dopcen_ref_tim()
  {
    return _dopcen_ref_tim;
  }
    /**
   * @brief Doppler centroid coefficients
   */
  double*   get_dopcen_coef()
  {
    return _dopcen_coef;
  }

protected:
  /**
   * @brief Doppler centroid confidence measure
   */
  double   _dopcen_conf;
    /**
   * @brief Doppler centroid reference time (sec)
   */
  double   _dopcen_ref_tim;
    /**
   * @brief Doppler centroid coefficients
   */
  double   _dopcen_coef[4];
private:

};
}
#endif
