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

#ifndef DataHistogramProcessedData_h
#define DataHistogramProcessedData_h

#include<iostream>

#include <RadarSat/RadarSatRecord.h>
#include <RadarSat/RadarSatRecordHeader.h>
#include <RadarSat/CommonRecord/DataHistogramRecord.h>

namespace ossimplugins
{
/**
 * @ingroup ProcessedData16bitHistogramRecord
 * @brief This class is able to read a data histogram record - Processed data
 * @warning There is an error in the documentation for the spare at the end of the record.
 *      They say 3 bytes (A3) but they also say that the spare is on the bytes 16917, 16918, 16919 and 16920.
 *      In the data files I have, I need to use a spare of 4 bytes.
 */
class DataHistogramProcessedData : public RadarSatRecord
{
public:
  /**
   * @brief Constructor
   */
  DataHistogramProcessedData();

  /**
   * @brief Destructor
   */
  virtual ~DataHistogramProcessedData();

  /**
   * @brief Copy constructor
   */
  DataHistogramProcessedData(const DataHistogramProcessedData& rhs);

  /**
   * @brief Copy operator
   */
  DataHistogramProcessedData& operator=(const DataHistogramProcessedData& rhs);
  /**
   * @brief This function writes the DataHistogramProcessedData in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const DataHistogramProcessedData& data);

  /**
   * @brief This function reads a DataHistogramProcessedData from a stream
   */
  friend std::istream& operator>>(std::istream& is, DataHistogramProcessedData& data);

  /**
   * @brief This function is able to create a new instance of the class
   */
  RadarSatRecord* Instanciate()
  {
    return new DataHistogramProcessedData();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  RadarSatRecord* Clone()
  {
    return new DataHistogramProcessedData(*this);
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
   * @brief Histogram data record #1
   */
  DataHistogramRecord get_histogram1()
  {
    return _histogram1;
  };

  /**
   * @brief Histogram data record #2
   */
  DataHistogramRecord get_histogram2()
  {
    return _histogram2;
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
   * @brief Histogram data record #1
   */
  DataHistogramRecord _histogram1;
  /**
   * @brief Histogram data record #2
   */
  DataHistogramRecord _histogram2;

private:


};
}
#endif
