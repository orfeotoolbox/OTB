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

#ifndef DataQuality_h
#define DataQuality_h

#include <iostream>

#include <RadarSat/RadarSatRecord.h>
#include <RadarSat/RadarSatRecordHeader.h>

#include "RadiometryUncertaintyRecord.h"
#include "MisregistrationRecord.h"

namespace ossimplugins
{
/**
 * @ingroup DataQualitySummaryRecord
 * @brief This class is able to read a data quality record
 */
class DataQuality : public RadarSatRecord
{
public:
  /**
   * @brief Constructor
   */
  DataQuality();

  /**
   * @brief Destructor
   */
  virtual ~DataQuality();

  /**
   * @brief Copy constructor
   */
  DataQuality(const DataQuality& rhs);

  /**
   * @brief Copy operator
   */
  DataQuality& operator=(const DataQuality& rhs);
  /**
   * @brief This function writes the DataQuality in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const DataQuality& data);

  /**
   * @brief This function reads a DataQuality from a stream
   */
  friend std::istream& operator>>(std::istream& is, DataQuality& data);

  /**
   * @brief This function is able to create a new instance of the class
   */
  RadarSatRecord* Instanciate()
  {
    return new DataQuality();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  RadarSatRecord* Clone()
  {
    return new DataQuality(*this);
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
  std::string   get_sar_chn()
  {
    return _sar_chn;
  };
    /**
   * @brief Calibration update date
   */
  std::string   get_cali_date()
  {
    return _cali_date;
  };
    /**
   * @brief Number of channels
   */
  int   get_nchn()
  {
    return _nchn;
  };
    /**
   * @brief Nominal integrated side lobe ratio
   */
  double   get_islr()
  {
    return _islr;
  };
    /**
   * @brief Nominal peak side lobe ratio
   */
  double   get_pslr()
  {
    return _pslr;
  };
    /**
   * @brief Nominal azimuth ambiguity
   */
  double   get_azi_ambig()
  {
    return _azi_ambig;
  };
    /**
   * @brief Nominal range ambiguity
   */
  double   get_rng_ambig()
  {
    return _rng_ambig;
  };
    /**
   * @brief Nominal signal to noise ratio
   */
  double   get_snr()
  {
    return _snr;
  };
    /**
   * @brief Nominal bit error rate
   */
  double   get_ber()
  {
    return _ber;
  };
    /**
   * @brief Nominal slant range resolution
   */
  double   get_rng_res()
  {
    return _rng_res;
  };
    /**
   * @brief Nominal azimuth resolution,meter
   */
  double   get_azi_res()
  {
    return _azi_res;
  };
    /**
   * @brief Nominal radiometric resolution,dB
   */
  double   get_rad_res()
  {
    return _rad_res;
  };
    /**
   * @brief Instantaneous dynamic range
   */
  double   get_dyn_rng()
  {
    return _dyn_rng;
  };
    /**
   * @brief Nominal radiometric uncertainty,dB
   */
  double   get_rad_unc_db()
  {
    return _rad_unc_db;
  };
    /**
   * @brief Nominal Radiometric uncertainty,deg
   */
  double   get_rad_unc_deg()
  {
    return _rad_unc_deg;
  };
  /**
   * @brief Radiometric data quality
   */
  RadiometryUncertaintyRecord* get_rad_unc()
  {
    return _rad_unc;
  };
    /**
   * @brief Nominal along track location error
   */
  double   get_alt_locerr()
  {
    return _alt_locerr;
  };
    /**
   * @brief Nominal cross track location error
   */
  double   get_crt_locerr()
  {
    return _crt_locerr;
  };
    /**
   * @brief Nominal along track distortion scale
   */
  double   get_alt_scale()
  {
    return _alt_scale;
  };
    /**
   * @brief Nominal cross track distortion scale
   */
  double   get_crt_scale()
  {
    return _crt_scale;
  };
    /**
   * @brief Nominal distortion skew
   */
  double   get_dis_skew()
  {
    return _dis_skew;
  };
    /**
   * @brief Nominal Scene orientation error
   */
  double   get_ori_err()
  {
    return _ori_err;
  };
  /**
   * @brief Misregistration error
   */
  MisregistrationRecord* get_misreg()
  {
    return _misreg;
  };
    /**
   * @brief Nominal noise equivalent sigma zero
   */
  double   get_nesz()
  {
    return _nesz;
  };
    /**
   * @brief Nominal equivalent number of looks
   */
  double   get_enl()
  {
    return _enl;
  };
    /**
   * @brief Default parameters table update date
   */
  std::string   get_tb_update()
  {
    return _tb_update;
  };


protected:

  /**
   * @brief Record sequence number
   */
  int   _rec_seq;
  /**
   * @brief SAR channel indicator
   */
  std::string   _sar_chn;
    /**
   * @brief Calibration update date
   */
  std::string   _cali_date;
    /**
   * @brief Number of channels
   */
  int   _nchn;
    /**
   * @brief Nominal integrated side lobe ratio
   */
  double   _islr;
    /**
   * @brief Nominal peak side lobe ratio
   */
  double   _pslr;
    /**
   * @brief Nominal azimuth ambiguity
   */
  double   _azi_ambig;
    /**
   * @brief Nominal range ambiguity
   */
  double   _rng_ambig;
    /**
   * @brief Nominal signal to noise ratio
   */
  double   _snr;
    /**
   * @brief Nominal bit error rate
   */
  double   _ber;
    /**
   * @brief Nominal slant range resolution
   */
  double   _rng_res;
    /**
   * @brief Nominal azimuth resolution,meter
   */
  double   _azi_res;
    /**
   * @brief Nominal radiometric resolution,dB
   */
  double   _rad_res;
    /**
   * @brief Instantaneous dynamic range
   */
  double   _dyn_rng;
    /**
   * @brief Nominal radiometric uncertainty,dB
   */
  double   _rad_unc_db;
    /**
   * @brief Nominal Radiometric uncertainty,deg
   */
  double   _rad_unc_deg;
  /**
   * @brief Radiometric data quality
   */
  RadiometryUncertaintyRecord _rad_unc[16];
    /**
   * @brief Nominal along track location error
   */
  double   _alt_locerr;
    /**
   * @brief Nominal cross track location error
   */
  double   _crt_locerr;
    /**
   * @brief Nominal along track distortion scale
   */
  double   _alt_scale;
    /**
   * @brief Nominal cross track distortion scale
   */
  double   _crt_scale;
    /**
   * @brief Nominal distortion skew
   */
  double   _dis_skew;
    /**
   * @brief Nominal Scene orientation error
   */
  double   _ori_err;
  /**
   * @brief Misregistration error
   */
  MisregistrationRecord _misreg[16];
    /**
   * @brief Nominal noise equivalent sigma zero
   */
  double   _nesz;
    /**
   * @brief Nominal equivalent number of looks
   */
  double   _enl;
    /**
   * @brief Default parameters table update date
   */
  std::string   _tb_update;

private:


};
}
#endif
