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

#ifndef DataHistogramRecord_h
#define DataHistogramRecord_h

#include<iostream>

#include <RadarSat/RadarSatRecord.h>
#include <RadarSat/RadarSatRecordHeader.h>

namespace ossimplugins
{
/**
 * @ingroup ProcessedData16bitHistogramRecord
 * @ingroup SignalDataHistogramRecord
 * @brief This class is able to read a data histogram record
 */
class DataHistogramRecord
{
public:
  /**
   * @brief Constructor
   */
  DataHistogramRecord();

  /**
   * @brief Destructor
   */
  ~DataHistogramRecord();

  /**
   * @brief Copy constructor
   */
  DataHistogramRecord(const DataHistogramRecord& rhs);

  /**
   * @brief Copy operator
   */
  DataHistogramRecord& operator=(const DataHistogramRecord& rhs);

  /**
   * @brief This function writes the DataHistogramRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const DataHistogramRecord& data);

  /**
   * @brief This function reads a DataHistogramRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, DataHistogramRecord& data);



  /**
   * @brief Histogram descriptor
   */
  std::string   get_hist_desc()
  {
    return _hist_desc;
  };
    /**
   * @brief Records per table
   */
  int   get_nrec()
  {
    return _nrec;
  };
    /**
   * @brief Table sequence number
   */
  int   get_tab_seq()
  {
    return _tab_seq;
  };
    /**
   * @brief Total number of table bins
   */
  int   get_nbin()
  {
    return _nbin;
  };
    /**
   * @brief Total number of lines
   */
  int   get_ns_lin()
  {
    return _ns_lin;
  };
    /**
   * @brief Total number of pixels per line
   */
  int   get_ns_pix()
  {
    return _ns_pix;
  };
    /**
   * @brief Group size along line axis
   */
  int   get_ngrp_lin()
  {
    return _ngrp_lin;
  };
    /**
   * @brief Group size along pixel axis
   */
  int   get_ngrp_pix()
  {
    return _ngrp_pix;
  };
    /**
   * @brief Number of samples used per group along line axis
   */
  int   get_nsamp_lin()
  {
    return _nsamp_lin;
  };
    /**
   * @brief Number of samples used per group along pixel
   */
  int   get_nsamp_pix()
  {
    return _nsamp_pix;
  };
    /**
   * @brief Minimum first bin
   */
  double   get_min_smp()
  {
    return _min_smp;
  };
    /**
   * @brief Maximum last bin
   */
  double   get_max_smp()
  {
    return _max_smp;
  };
    /**
   * @brief Mean sample value
   */
  double   get_mean_smp()
  {
    return _mean_smp;
  };
    /**
   * @brief Sample standard deviation
   */
  double   get_std_smp()
  {
    return _std_smp;
  };
    /**
   * @brief Sample value increment
   */
  double   get_smp_inc()
  {
    return _smp_inc;
  };
    /**
   * @brief Minimum histogram value
   */
  double   get_min_hist()
  {
    return _min_hist;
  };
    /**
   * @brief Maximum histogram value
   */
  double   get_max_hist()
  {
    return _max_hist;
  };
    /**
   * @brief Histogram mean value
   */
  double   get_mean_hist()
  {
    return _mean_hist;
  };
    /**
   * @brief Histogram standard deviation
   */
  double   get_std_hist()
  {
    return _std_hist;
  };
    /**
   * @brief Histogram table size
   */
  int   get_nhist()
  {
    return _nhist;
  };

  /**
   * @brief Histogram table values of 16 bins for Ix16 bins for Q
   */
  int* get_hist()
  {
    return _hist;
  };
protected:


  /**
   * @brief Histogram descriptor
   */
  std::string   _hist_desc;
    /**
   * @brief Records per table
   */
  int   _nrec;
    /**
   * @brief Table sequence number
   */
  int   _tab_seq;
    /**
   * @brief Total number of table bins
   */
  int   _nbin;
    /**
   * @brief Total number of lines
   */
  int   _ns_lin;
    /**
   * @brief Total number of pixels per line
   */
  int   _ns_pix;
    /**
   * @brief Group size along line axis
   */
  int   _ngrp_lin;
    /**
   * @brief Group size along pixel axis
   */
  int   _ngrp_pix;
    /**
   * @brief Number of samples used per group along line axis
   */
  int   _nsamp_lin;
    /**
   * @brief Number of samples used per group along pixel
   */
  int   _nsamp_pix;
    /**
   * @brief Minimum first bin
   */
  double   _min_smp;
    /**
   * @brief Maximum last bin
   */
  double   _max_smp;
    /**
   * @brief Mean sample value
   */
  double   _mean_smp;
    /**
   * @brief Sample standard deviation
   */
  double   _std_smp;
    /**
   * @brief Sample value increment
   */
  double   _smp_inc;
    /**
   * @brief Minimum histogram value
   */
  double   _min_hist;
    /**
   * @brief Maximum histogram value
   */
  double   _max_hist;
    /**
   * @brief Histogram mean value
   */
  double   _mean_hist;
    /**
   * @brief Histogram standard deviation
   */
  double   _std_hist;
    /**
   * @brief Histogram table size
   */
  int   _nhist;
  /**
   * @brief Histogram table values
   */
  int* _hist;
private:


};
}
#endif
