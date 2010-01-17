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

#ifndef ProcessedDataRecord_h
#define ProcessedDataRecord_h

#include <iostream>

#include <RadarSat/RadarSatRecord.h>
#include <RadarSat/RadarSatRecordHeader.h>
#include <ossim/base/ossimCommon.h>

namespace ossimplugins
{
/**
 * @ingroup ProcessedDataRecord
 * @brief This class is able to read an Processed Data Record
 */
class ProcessedDataRecord : public RadarSatRecord
{
public:
  /**
   * @brief Constructor
   */
  ProcessedDataRecord();

  /**
   * @brief Destructor
   */
  ~ProcessedDataRecord();

  /**
   * @brief Copy constructor
   */
  ProcessedDataRecord(const ProcessedDataRecord& rhs);

  /**
   * @brief Copy operator
   */
  ProcessedDataRecord& operator=(const ProcessedDataRecord& rhs);
  /**
   * @brief This function writes the ProcessedDataRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const ProcessedDataRecord& data);

  /**
   * @brief This function reads a ProcessedDataRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, ProcessedDataRecord& data);

  /**
   * @brief This function is able to create a new instance of the class
   */
  RadarSatRecord* Instanciate()
  {
    return new ProcessedDataRecord();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  RadarSatRecord* Clone()
  {
    return new ProcessedDataRecord(*this);
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
   * @brief Image data line number
   */
  int get_line_num() {return _line_num;};

    /**
   * @brief Image data record index
   */
  int get_rec_num() {return _rec_num;};
    /**
   * @brief Left fill pixel count
   */
  int get_n_left_pixel() {return _n_left_pixel;};
    /**
   * @brief Data pixel count
   */
  int get_n_data_pixel() {return _n_data_pixel;};
    /**
   * @brief Right fill pixel count
   */
  int get_n_right_pixel() {return _n_right_pixel;};
    /**
   * @brief Sensor parameter update flag
   */
  int get_sensor_updf() {return _sensor_updf;};
    /**
   * @brief Acquisition year
   */
  int   get_acq_year() {return _acq_year;};
    /**
   * @brief Acquisition day
   */
  int   get_acq_day() {return _acq_day;};
    /**
   * @brief Acquisition msecs
   */
  int   get_acq_msec() {return _acq_msec;};
    /**
   * @brief SAR channel indicator
   */
  int   get_sar_chan_ind() {return _sar_chan_ind;};
    /**
   * @brief SAR channel code
   */
  int   get_sar_chan_code() {return _sar_chan_code;};
    /**
   * @brief Transmitted polarization
   */
  int   get_tran_polar() {return _tran_polar;};
    /**
   * @brief Received polarization
   */
  int   get_recv_polar() {return _recv_polar;};
    /**
   * @brief PRF
   */
  int  get_prf() {return _prf;};
    /**
   * @brief Slant range to first pixel, m
   */
  int   get_sr_first() {return _sr_first;};
    /**
   * @brief Slant range to mid-pixel, m
   */
  int get_sr_mid() {return _sr_mid;};
    /**
   * @brief Slant range to last pixel, m
   */
  int get_sr_last() {return _sr_last;};
    /**
   * @brief First pixel Doppler centroid, Hz
   */
  int get_fdc_first() {return _fdc_first;};
    /**
   * @brief Mid pixel Doppler centroid, Hz
   */
  int   get_fdc_mid() {return _fdc_mid;};
    /**
   * @brief Last pixel Doppler centroid, Hz
   */
  int   get_fdc_last() {return _fdc_last;};
    /**
   * @brief First pixel azimuth FM rate, Hz
   */
  int get_ka_first() {return _ka_first;};
     /**
   * @brief Mid pixel azimuth FM rate, Hz
   */
  int get_ka_mid() {return _ka_mid;};
   /**
   * @brief Last pixel azimuth FM rate, Hz
   */
  int get_ka_last() {return _ka_last;};
    /**
   * @brief Nadir look angle, 10e-6 deg
   */
  int get_nadir_ang() {return _nadir_ang;};
    /**
   * @brief Azimuth squint angle, 10e-6 deg
   */
  int get_squint_ang() {return _squint_ang;};
    /**
   * @brief Geographic ref parameter update flag
   */
  int get_geo_updf() {return _geo_updf;};
    /**
   * @brief First pixel latitude, 10e-6 deg
   */
  int get_lat_first() {return _lat_first;};
    /**
   * @brief Mid-pixel latitude, 10e-6 deg
   */
  int get_lat_mid() {return _lat_mid;};
   /**
   * @brief Last-pixel latitude, 10e-6 deg
   */
  int get_lat_last() {return _lat_last;};
  /**
   * @brief First pixel longitude, 10e-6 deg
   */
  int get_lon_first() {return _lon_first;};
    /**
   * @brief Mid-pixel longitude, 10e-6 deg
   */
  int get_lon_mid() {return _lon_mid;};
   /**
   * @brief Last-pixel longitude, 10e-6 deg
   */
  int get_lon_last() {return _lon_last;};
    /**
   * @brief Northing of first pixel, m
   */
  int get_north_first() {return _north_first;};
   /**
   * @brief Northing of last pixel, m
   */
  int get_north_last() {return _north_last;};
  /**
   * @brief Easting of first pixel, m
   */
  int get_east_first() {return _east_first;};
   /**
   * @brief Easting of last pixel, m
   */
  int get_east_last() {return _east_last;};
   /**
   * @brief Line heading, 10e-6 deg
   */
  int get_heading() {return _heading;};

protected:
  /**
   * @brief Image data line number
   */
  int _line_num;

    /**
   * @brief Image data record index
   */
  int _rec_num;
    /**
   * @brief Left fill pixel count
   */
  int _n_left_pixel;
    /**
   * @brief Data pixel count
   */
  int _n_data_pixel;
    /**
   * @brief Right fill pixel count
   */
  int _n_right_pixel;
    /**
   * @brief Sensor parameter update flag
   */
  int _sensor_updf;
    /**
   * @brief Acquisition year
   */
  int   _acq_year;
    /**
   * @brief Acquisition day
   */
  int   _acq_day;
    /**
   * @brief Acquisition msecs
   */
  int   _acq_msec;
    /**
   * @brief SAR channel indicator
   */
  int   _sar_chan_ind;
    /**
   * @brief SAR channel code
   */
  int   _sar_chan_code;
    /**
   * @brief Transmitted polarization
   */
  int   _tran_polar;
    /**
   * @brief Received polarization
   */
  int   _recv_polar;
    /**
   * @brief PRF
   */
  int   _prf;
    /**
   * @brief Slant range to first pixel, m
   */
  int   _sr_first;
    /**
   * @brief Slant range to mid-pixel, m
   */
  int _sr_mid;

    /**
   * @brief Slant range to last pixel, m
   */
  int _sr_last;
    /**
   * @brief First pixel Doppler centroid, Hz
   */
  int _fdc_first;

    /**
   * @brief Mid pixel Doppler centroid, Hz
   */
  int   _fdc_mid;
    /**
   * @brief Last pixel Doppler centroid, Hz
   */
  int   _fdc_last;
    /**
   * @brief First pixel azimuth FM rate, Hz
   */
  int _ka_first;
     /**
   * @brief Mid pixel azimuth FM rate, Hz
   */
  int _ka_mid;
   /**
   * @brief Last pixel azimuth FM rate, Hz
   */
  int _ka_last;
    /**
   * @brief Nadir look angle, 10e-6 deg
   */
  int _nadir_ang;
    /**
   * @brief Azimuth squint angle, 10e-6 deg
   */
  int _squint_ang;
    /**
   * @brief Geographic ref parameter update flag
   */
  int _geo_updf;
    /**
   * @brief First pixel latitude, 10e-6 deg
   */
  int _lat_first;
    /**
   * @brief Mid-pixel latitude, 10e-6 deg
   */
  int _lat_mid;
   /**
   * @brief Last-pixel latitude, 10e-6 deg
   */
  int _lat_last;
  /**
   * @brief First pixel longitude, 10e-6 deg
   */
  int _lon_first;
    /**
   * @brief Mid-pixel longitude, 10e-6 deg
   */
  int _lon_mid;
   /**
   * @brief Last-pixel longitude, 10e-6 deg
   */
  int _lon_last;
    /**
   * @brief Northing of first pixel, m
   */
  int _north_first;
   /**
   * @brief Northing of last pixel, m
   */
  int _north_last;
  /**
   * @brief Easting of first pixel, m
   */
  int _east_first;
   /**
   * @brief Easting of last pixel, m
   */
  int _east_last;
   /**
   * @brief Line heading, 10e-6 deg
   */
  int _heading;

private:
  /**
   * @brief This function switches the LSB value and the MSB value of the parameter
   */
  template<typename T>
  void SwitchEndian(T &value) {

    char *buffer;
    char *res;
    int nb_octets = sizeof(T);

    if (ossim::byteOrder() == OSSIM_LITTLE_ENDIAN) {
      buffer = (char* )malloc(nb_octets);
      res = (char* )malloc(nb_octets);

      memcpy(buffer,&value,nb_octets);

      for (int i=0; i<nb_octets/2; i++) {
        res[i] = buffer[nb_octets-1-i];
        res[nb_octets-1-i] = buffer[i];
      }
      memcpy(&value,res,nb_octets);

      free(buffer);
      free(res);
    }
  }
};
}
#endif
