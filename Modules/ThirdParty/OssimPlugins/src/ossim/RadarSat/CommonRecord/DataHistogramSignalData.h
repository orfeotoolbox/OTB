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


#ifndef DataHistogramSignalData_h
#define DataHistogramSignalData_h


#include <RadarSat/RadarSatRecord.h>
#include <RadarSat/RadarSatRecordHeader.h>
#include "DataHistogramRecord.h"

namespace ossimplugins
{
/**
 * @ingroup SignalDataHistogramRecord
 * @brief This class is able to read a data histogram record - Signal data
 */
class DataHistogramSignalData : public RadarSatRecord
{
public:
  /**
   * @brief Constructor
   */
  DataHistogramSignalData();

  /**
   * @brief Destructor
   */
  virtual ~DataHistogramSignalData();

  /**
   * @brief Copy constructor
   */
  DataHistogramSignalData(const DataHistogramSignalData& rhs);

  /**
   * @brief Copy operator
   */
  DataHistogramSignalData& operator=(const DataHistogramSignalData& rhs);
  /**
   * @brief This function writes the DataHistogramSignalData in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const DataHistogramSignalData& data);

  /**
   * @brief This function reads a DataHistogramSignalData from a stream
   */
  friend std::istream& operator>>(std::istream& is, DataHistogramSignalData& data);

  /**
   * @brief This function is able to create a new instance of the class
   */
  RadarSatRecord* Instantiate()
  {
    return new DataHistogramSignalData();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  RadarSatRecord* Clone()
  {
    return new DataHistogramSignalData(*this);
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
  int   get_rec_seq()
  {
    return _rec_seq;
  };
  /**
   * @brief SAR channel indicator
   */
  int   get_sar_chn()
  {
    return _sar_chn;
  };

  /**
   * @brief Number of histogram table data sets in this record
   */
  int   get_ntab()
  {
    return _ntab;
  };

  /**
   * @brief Histogram table data set size
   */
  int   get_ltab()
  {
    return _ltab;
  };

  /**
   * @brief Histogram data record
   */
  DataHistogramRecord get_histogram()
  {
    return _histogram;
  };

protected:
  /**
   * @brief Record sequence number
   */
  int   _rec_seq;
  /**
   * @brief SAR channel indicator
   */
  int   _sar_chn;

  /**
   * @brief Number of histogram table data sets in this record
   */
  int   _ntab;
  /**
   * @brief Histogram table data set size
   */
  int   _ltab;
  /**
   * @brief Histogram data record
   */
  DataHistogramRecord _histogram;

private:


};

typedef DataHistogramSignalData DataHistogramProcessedData8;
}
#endif
