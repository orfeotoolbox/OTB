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

#ifndef DataSetSummary_h
#define DataSetSummary_h

#include <iostream>
#include <RadarSat/RadarSatRecordHeader.h>
#include <RadarSat/RadarSatRecord.h>

namespace ossimplugins
{
/**
 * @ingroup DataSetSummaryRecord
 * @brief This class is able to read the SAR leader data set summary record of the leader file
 */
class DataSetSummary : public RadarSatRecord
{
public:
  /**
   * @brief Constructor
   */
  DataSetSummary();

  /**
   * @brief Destructor
   */
  virtual ~DataSetSummary();

  /**
   * @brief This function write the DataSetSummary in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const DataSetSummary& data);

  /**
   * @brief This function read a DataSetSummary from a stream
   */
  friend std::istream& operator>>(std::istream& is, DataSetSummary& data);

  /**
   * @brief Copy constructor
   */
  DataSetSummary(const DataSetSummary& rhs);

  /**
   * @brief Copy operator
   */
  DataSetSummary& operator=(const DataSetSummary& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  RadarSatRecord* Instanciate()
  {
    return new DataSetSummary();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  RadarSatRecord* Clone()
  {
    return new DataSetSummary(*this);
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
   * @brief Sequence number
   */
  int   get_seq_num()
  {
    return _seq_num;
  };
    /**
   * @brief SAR channel indicator
   */
  int   get_sar_chn()
  {
    return _sar_chn;
  };
  /**
   * @brief Scene identifier
   */
  std::string   get_scene_id()
  {
    return _scene_id;
  };
  /**
   * @brief Scene designator
   */
  std::string   get_scene_des()
  {
    return _scene_des;
  };
  /**
   * @brief Input scene centre time
   */
  std::string   get_inp_sctim()
  {
    return _inp_sctim;
  };
  /**
   * @brief Ascending/descending
   */
  std::string   get_asc_des()
  {
    return _asc_des;
  };
    /**
   * @brief Processed scene centre latitude
   */
  double   get_pro_lat()
  {
    return _pro_lat;
  };
    /**
   * @brief Processed scene centre longitude
   */
  double   get_pro_long()
  {
    return _pro_long;
  };
    /**
   * @brief Processed scene centre headng
   */
  double   get_pro_head()
  {
    return _pro_head;
  };
  /**
   * @brief Ellipsoid designator
   */
  std::string   get_ellip_des()
  {
    return _ellip_des;
  };
    /**
   * @brief Ellipsoid semi-major axis, km
   */
  double   get_ellip_maj()
  {
    return _ellip_maj;
  };
    /**
   * @brief Ellipsoid semi_minor axis, km
   */
  double  get_ellip_min()
  {
    return _ellip_min;
  };
  /**
   * @brief Earth's mass
   */
    double   get_earth_mass()
  {
    return _earth_mass;
  };
    /**
   * @brief Gravitational constant
   */
    double   get_grav_const()
  {
    return _grav_const;
  };
    /**
   * @brief Ellipsoid J2-4 parameters
   */
    double*   get_ellip_j()
  {
    return _ellip_j;
  };

    /**
   * @brief Average terrain height, km
   */
    double   get_terrain_h()
  {
    return _terrain_h;
  };
    /**
   * @brief Scene centre line number
   */
    int   get_sc_lin()
  {
    return _sc_lin;
  };
    /**
   * @brief Scene centre pixel number
   */
    int   get_sc_pix()
  {
    return _sc_pix;
  };
    /**
   * @brief Scene length, km
   */
    double   get_scene_len()
  {
    return _scene_len;
  };
    /**
   * @brief Scene width, km
   */
    double   get_scene_wid()
  {
    return _scene_wid;
  };

    /**
   * @brief Number of SAR channels
   */
    double   get_nchn()
  {
    return _nchn;
  };

    /**
   * @brief Mission identifier
   */
  std::string   get_mission_id()
  {
    return _mission_id;
  };
    /**
   * @brief Sensor identifier
   */
  std::string   get_sensor_id()
  {
    return _sensor_id;
  };
    /**
   * @brief Orbit number
   */
  std::string   get_orbit_num()
  {
    return _orbit_num;
  };
    /**
   * @brief Platform geodetic latitude
   */
    double   get_plat_lat()
  {
    return _plat_lat;
  };
    /**
   * @brief Platform geodetic longitude
   */
    double   get_plat_long()
  {
    return _plat_long;
  };
    /**
   * @brief Platform heading
   */
    double   get_plat_head()
  {
    return _plat_head;
  };
    /**
   * @brief Sensor clock angle
   */
    double   get_clock_ang()
  {
    return _clock_ang;
  };
    /**
   * @brief Incidence angle
   */
    double   get_incident_ang()
  {
    return _incident_ang;
  };

    /**
   * @brief Radar wave length
   */
    double   get_wave_length()
  {
    return _wave_length;
  };
    /**
   * @brief Motion compensation indicator
   */
  std::string   get_motion_comp()
  {
    return _motion_comp;
  };
    /**
   * @brief Range pulse code specifier
   */
    std::string   get_pulse_code()
  {
    return _pulse_code;
  };
    /**
   * @brief Range chirp coefficients
   */
    double*   get_ampl_coef()
  {
    return _ampl_coef;
  };
    /**
   * @brief Range phase coefficients
   */
    double*   get_phas_coef()
  {
    return _phas_coef;
  };
    /**
   * @brief Chirp extraction index
   */
    int   get_chirp_ext_ind()
  {
    return _chirp_ext_ind;
  };

    /**
   * @brief Range sampling rate
   */
    double   get_fr()
  {
    return _fr;
  };
    /**
   * @brief Range gate start time
   */
    double   get_rng_gate()
  {
    return _rng_gate;
  };
    /**
   * @brief Range pulse length
   */
    double   get_rng_length()
  {
    return _rng_length;
  };
    /**
   * @brief Baseband conversion flag
   */
  std::string   get_baseband_f()
  {
    return _baseband_f;
  };
    /**
   * @brief Range compressed flag
   */
    std::string   get_rngcmp_f()
  {
    return _rngcmp_f;
  };
    /**
   * @brief Like polarized gain
   */
    double   get_gn_polar()
  {
    return _gn_polar;
  };
    /**
   * @brief Cross polarized gain
   */
    double   get_gn_cross()
  {
    return _gn_cross;
  };
    /**
   * @brief Number of bits per channel
   */
    int   get_chn_bits()
  {
    return _chn_bits;
  };
    /**
   * @brief Quantization descriptor
   */
    std::string   get_quant_desc()
  {
    return _quant_desc;
  };
    /**
   * @brief I channel DC bias
   */
    double   get_i_bias()
  {
    return _i_bias;
  };
    /**
   * @brief Q channel DC bias
   */
    double   get_q_bias()
  {
    return _q_bias;
  };
    /**
   * @brief I/Q channel ratio
   */
    double   get_iq_ratio()
  {
    return _iq_ratio;
  };


    /**
   * @brief Electronic boresight
   */
    double   get_ele_sight()
  {
    return _ele_sight;
  };
    /**
   * @brief Mechanical boresight
   */
    double   get_mech_sight()
  {
    return _mech_sight;
  };
    /**
   * @brief Echo tracker on/off flag
   */
  std::string   get_echo_track()
  {
    return _echo_track;
  };
    /**
   * @brief Nominal PRF, Hz
   */
    double   get_fa()
  {
    return _fa;
  };
    /**
   * @brief Elevation beamwidth
   */
    double   get_elev_beam()
  {
    return _elev_beam;
  };
    /**
   * @brief Azimuth beamwidth
   */
    double   get_azim_beam()
  {
    return _azim_beam;
  };
    /**
   * @brief Satellite binary time
   */
    int   get_sat_bintim()
  {
    return _sat_bintim;
  };
    /**
   * @brief Satellite clock time
   */
    int   get_sat_clktim()
  {
    return _sat_clktim;
  };
    /**
   * @brief Satellite clock increment
   */
    int   get_sat_clkinc()
  {
    return _sat_clkinc;
  };

    /**
   * @brief Processing facility identifier
   */
    std::string   get_fac_id()
  {
    return _fac_id;
  };
    /**
   * @brief Processing system identifier
   */
    std::string   get_sys_id()
  {
    return _sys_id;
  };
    /**
   * @brief Processing version identifier
   */
    std::string   get_ver_id()
  {
    return _ver_id;
  };
    /**
   * @brief Facility process code
   */
    std::string   get_fac_code()
  {
    return _fac_code;
  };
    /**
   * @brief Product level code
   */
    std::string   get_lev_code()
  {
    return _lev_code;
  };
    /**
   * @brief Product type specifier
   */
    std::string   get_prod_type()
  {
    return _prod_type;
  };
    /**
   * @brief Processing algorithm identifier
   */
    std::string   get_algor_id()
  {
    return _algor_id;
  };
    /**
   * @brief Number of azimuth looks
   */
    double   get_n_azilok()
  {
    return _n_azilok;
  };
    /**
   * @brief Number of range looks
   */
    double   get_n_rnglok()
  {
    return _n_rnglok;
  };
    /**
   * @brief Bandwidth per look in azimuth,Hz
   */
    double   get_bnd_azilok()
  {
    return _bnd_azilok;
  };
    /**
   * @brief Bandwidth per look in range,Hz
   */
    double   get_bnd_rnglok()
  {
    return _bnd_rnglok;
  };
    /**
   * @brief Total azimuth look bandwidth
   */
    double   get_bnd_azi()
  {
    return _bnd_azi;
  };
    /**
   * @brief Total range look bandwidth
   */
    double   get_bnd_rng()
  {
    return _bnd_rng;
  };
    /**
   * @brief Azimuth weighting designator
   */
    std::string   get_azi_weight()
  {
    return _azi_weight;
  };
    /**
   * @brief Range weighting designator
   */
    std::string   get_rng_weight()
  {
    return _rng_weight;
  };
    /**
   * @brief Data input source
   */
    std::string   get_data_inpsrc()
  {
    return _data_inpsrc;
  };
    /**
   * @brief Range resolution, meter
   */
    double   get_rng_res()
  {
    return _rng_res;
  };
    /**
   * @brief Azimuth resolution, meter
   */
    double   get_azi_res()
  {
    return _azi_res;
  };
    /**
   * @brief Radiometric stretch terms
   */
    double*   get_radi_stretch()
  {
    return _radi_stretch;
  };
    /**
   * @brief Along track Doppler frequency terms
   */
    double*   get_alt_dopcen()
  {
    return _alt_dopcen;
  };

    /**
   * @brief Cross track Doppler frequency terms
   */
    double*   get_crt_dopcen()
  {
    return _crt_dopcen;
  };
    /**
   * @brief Pixel time direction indicator
   */
    std::string   get_time_dir_pix()
  {
    return _time_dir_pix;
  };
    /**
   * @brief Line time direction indicator
   */
    std::string   get_time_dir_lin()
  {
    return _time_dir_lin;
  };
    /**
   * @brief Along track Doppler rate term
   */
    double*   get_alt_rate()
  {
    return _alt_rate;
  };

    /**
   * @brief Cross track Doppler rate term
   */
    double*   get_crt_rate()
  {
    return _crt_rate;
  };

    /**
   * @brief Line content indicator
   */
    std::string   get_line_cont()
  {
    return _line_cont;
  };
    /**
   * @brief Clutter lock applied flag
   */
    std::string   get_clutter_lock()
  {
    return _clutter_lock;
  };
    /**
   * @brief Auto-focus applied flag
   */
    std::string   get_auto_focus()
  {
    return _auto_focus;
  };
    /**
   * @brief Line spacing, meters
   */
    double   get_line_spacing()
  {
    return _line_spacing;
  };
    /**
   * @brief Pixel spacing, meters
   */
    double   get_pix_spacing()
  {
    return _pix_spacing;
  };
    /**
   * @brief Range compression designator
   */
    std::string   get_rngcmp_desg()
  {
    return _rngcmp_desg;
  };

protected:
  /**
   * @brief Sequence number
   */
  int   _seq_num;
    /**
   * @brief SAR channel indicator
   */
  int   _sar_chn;
  /**
   * @brief Scene identifier
   */
  std::string   _scene_id;
  /**
   * @brief Scene designator
   */
  std::string   _scene_des;
  /**
   * @brief Input scene centre time
   */
  std::string   _inp_sctim;
  /**
   * @brief Ascending/descending
   */
  std::string   _asc_des;
    /**
   * @brief Processed scene centre latitude
   */
  double   _pro_lat;
    /**
   * @brief Processed scene centre longitude
   */
  double   _pro_long;
    /**
   * @brief Processed scene centre headng
   */
  double   _pro_head;
  /**
   * @brief Ellipsoid designator
   */
  std::string   _ellip_des;
    /**
   * @brief Ellipsoid semi-major axis, km
   */
  double   _ellip_maj;
    /**
   * @brief Ellipsoid semi_minor axis, km
   */
  double  _ellip_min;
  /**
   * @brief Earth's mass
   */
    double   _earth_mass;
    /**
   * @brief Gravitational constant
   */
    double   _grav_const;
    /**
   * @brief Ellipsoid J2-4 parameters
   */
    double   _ellip_j[3];

    /**
   * @brief Average terrain height, km
   */
    double   _terrain_h;
    /**
   * @brief Scene centre line number
   */
    int   _sc_lin;
    /**
   * @brief Scene centre pixel number
   */
    int   _sc_pix;
    /**
   * @brief Scene length, km
   */
    double   _scene_len;
    /**
   * @brief Scene width, km
   */
    double   _scene_wid;

    /**
   * @brief Number of SAR channels
   */
    double   _nchn;

    /**
   * @brief Mission identifier
   */
  std::string   _mission_id;
    /**
   * @brief Sensor identifier
   */
  std::string   _sensor_id;
    /**
   * @brief Orbit number
   */
  std::string   _orbit_num;
    /**
   * @brief Platform geodetic latitude
   */
    double   _plat_lat;
    /**
   * @brief Platform geodetic longitude
   */
    double   _plat_long;
    /**
   * @brief Platform heading
   */
    double   _plat_head;
    /**
   * @brief Sensor clock angle
   */
    double   _clock_ang;
    /**
   * @brief Incidence angle
   */
    double   _incident_ang;

    /**
   * @brief Radar wave length
   */
    double   _wave_length;
    /**
   * @brief Motion compensation indicator
   */
  std::string   _motion_comp;
    /**
   * @brief Range pulse code specifier
   */
    std::string   _pulse_code;
    /**
   * @brief Range chirp coefficients
   */
    double   _ampl_coef[5];
    /**
   * @brief Range phase coefficients
   */
    double   _phas_coef[5];
    /**
   * @brief Chirp extraction index
   */
    int   _chirp_ext_ind;

    /**
   * @brief Range sampling rate
   */
    double   _fr;
    /**
   * @brief Range gate start time
   */
    double   _rng_gate;
    /**
   * @brief Range pulse length
   */
    double   _rng_length;
    /**
   * @brief Baseband conversion flag
   */
  std::string   _baseband_f;
    /**
   * @brief Range compressed flag
   */
    std::string   _rngcmp_f;
    /**
   * @brief Like polarized gain
   */
    double   _gn_polar;
    /**
   * @brief Cross polarized gain
   */
    double   _gn_cross;
    /**
   * @brief Number of bits per channel
   */
    int   _chn_bits;
    /**
   * @brief Quantization descriptor
   */
    std::string   _quant_desc;
    /**
   * @brief I channel DC bias
   */
    double   _i_bias;
    /**
   * @brief Q channel DC bias
   */
    double   _q_bias;
    /**
   * @brief I/Q channel ratio
   */
    double   _iq_ratio;

    /**
   * @brief Electronic boresight
   */
    double   _ele_sight;
    /**
   * @brief Mechanical boresight
   */
    double   _mech_sight;
    /**
   * @brief Echo tracker on/off flag
   */
  std::string   _echo_track;
    /**
   * @brief Nominal PRF, Hz
   */
    double   _fa;
    /**
   * @brief Elevation beamwidth
   */
    double   _elev_beam;
    /**
   * @brief Azimuth beamwidth
   */
    double   _azim_beam;
    /**
   * @brief Satellite binary time
   */
    int   _sat_bintim;
    /**
   * @brief Satellite clock time
   */
    int   _sat_clktim;
    /**
   * @brief Satellite clock increment
   */
    int   _sat_clkinc;

    /**
   * @brief Processing facility identifier
   */
    std::string   _fac_id;
    /**
   * @brief Processing system identifier
   */
    std::string   _sys_id;
    /**
   * @brief Processing version identifier
   */
    std::string   _ver_id;
    /**
   * @brief Facility process code
   */
    std::string   _fac_code;
    /**
   * @brief Product level code
   */
    std::string   _lev_code;
    /**
   * @brief Product type specifier
   */
    std::string   _prod_type;
    /**
   * @brief Processing algorithm identifier
   */
    std::string   _algor_id;
    /**
   * @brief Number of azimuth looks
   */
    double   _n_azilok;
    /**
   * @brief Number of range looks
   */
    double   _n_rnglok;
    /**
   * @brief Bandwidth per look in azimuth,Hz
   */
    double   _bnd_azilok;
    /**
   * @brief Bandwidth per look in range,Hz
   */
    double   _bnd_rnglok;
    /**
   * @brief Total azimuth look bandwidth
   */
    double   _bnd_azi;
    /**
   * @brief Total range look bandwidth
   */
    double   _bnd_rng;
    /**
   * @brief Azimuth weighting designator
   */
    std::string   _azi_weight;
    /**
   * @brief Range weighting designator
   */
    std::string   _rng_weight;
    /**
   * @brief Data input source
   */
    std::string   _data_inpsrc;
    /**
   * @brief Range resolution, meter
   */
    double   _rng_res;
    /**
   * @brief Azimuth resolution, meter
   */
    double   _azi_res;
    /**
   * @brief Radiometric stretch terms
   */
    double   _radi_stretch[2];
    /**
   * @brief Along track Doppler frequency terms
   */
    double   _alt_dopcen[3];

    /**
   * @brief Cross track Doppler frequency terms
   */
    double   _crt_dopcen[3];
    /**
   * @brief Pixel time direction indicator
   */
    std::string   _time_dir_pix;
    /**
   * @brief Line time direction indicator
   */
    std::string   _time_dir_lin;
    /**
   * @brief Along track Doppler rate term
   */
    double   _alt_rate[3];

    /**
   * @brief Cross track Doppler rate term
   */
    double   _crt_rate[3];

    /**
   * @brief Line content indicator
   */
    std::string   _line_cont;
    /**
   * @brief Clutter lock applied flag
   */
    std::string   _clutter_lock;
    /**
   * @brief Auto-focus applied flag
   */
    std::string   _auto_focus;
    /**
   * @brief Line spacing, meters
   */
    double   _line_spacing;
    /**
   * @brief Pixel spacing, meters
   */
    double   _pix_spacing;
    /**
   * @brief Range compression designator
   */
    std::string   _rngcmp_desg;

private:

};
}
#endif
