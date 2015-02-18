//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
// "Copyright Centre for Remote Imaging, Sensing and Processing"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef AlosPalsarDataSetSummary_h
#define AlosPalsarDataSetSummary_h

#include <iostream>
#include <cstdlib>
#include <AlosPalsar/AlosPalsarRecordHeader.h>
#include <AlosPalsar/AlosPalsarRecord.h>

namespace ossimplugins
{

/**
 * @ingroup AlosPalsarDataSetSummaryRecord
 * @brief This class is able to read the SAR leader data set summary record of the leader file
 */
class AlosPalsarDataSetSummary : public AlosPalsarRecord
{
public:
  /**
   * @brief Constructor
   */
  AlosPalsarDataSetSummary();

  /**
   * @brief Destructor
   */
  virtual ~AlosPalsarDataSetSummary();

  /**
   * @brief This function write the AlosPalsarDataSetSummary in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const AlosPalsarDataSetSummary& data);

  /**
   * @brief This function read a AlosPalsarDataSetSummary from a stream
   */
  friend std::istream& operator>>(std::istream& is, AlosPalsarDataSetSummary& data);

  /**
   * @brief Copy constructor
   */
  AlosPalsarDataSetSummary(const AlosPalsarDataSetSummary& rhs);

  /**
   * @brief Copy operator
   */
  AlosPalsarDataSetSummary& operator=(const AlosPalsarDataSetSummary& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  AlosPalsarRecord* Instanciate()
  {
    return new AlosPalsarDataSetSummary();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  AlosPalsarRecord* Clone()
  {
    return new AlosPalsarDataSetSummary(*this);
  };

  /**
   * @brief Read the class data from a stream
   */
  void Read(std::istream& is)
  {
    is >> *this;
  };

  /**
   * @brief Write the class to a stream
   */
  void Write(std::ostream& os)
  {
    os << *this;
  };

  /**
   * @brief Sequence number
   */
  int   get_seq_num() const
  {
    return _seq_num;
  };
  /**
  * @brief SAR channel indicator
  */
  int   get_sar_chn() const
  {
    return _sar_chn;
  };
  /**
   * @brief Scene identifier
   */
  std::string   get_scene_id() const
  {
    return _scene_id;
  };
  /**
   * @brief Scene designator
   */
  std::string   get_scene_des() const
  {
    return _scene_des;
  };
  /**
   * @brief Input scene centre time
   */
  std::string   get_inp_sctim() const
  {
    return _inp_sctim;
  };
  /**
   * @brief Ascending/descending
   */
  std::string   get_asc_des() const
  {
    return _asc_des;
  };
  /**
  * @brief Processed scene centre latitude
  */
  double   get_pro_lat() const
  {
    return _pro_lat;
  };
  /**
  * @brief Processed scene centre longitude
  */
  double   get_pro_long() const
  {
    return _pro_long;
  };
  /**
  * @brief Processed scene centre headng
  */
  double   get_pro_head() const
  {
    return _pro_head;
  };
  /**
   * @brief Ellipsoid designator
   */
  std::string   get_ellip_des() const
  {
    return _ellip_des;
  };
  /**
  * @brief Ellipsoid semi-major axis, km
  */
  double   get_ellip_maj() const
  {
    return _ellip_maj;
  };
  /**
  * @brief Ellipsoid semi_minor axis, km
  */
  double  get_ellip_min() const
  {
    return _ellip_min;
  };
  /**
   * @brief Earth's mass
   */
  double   get_earth_mass() const
  {
    return _earth_mass;
  };
  /**
  * @brief Gravitational constant
  */
  double   get_grav_const() const
  {
    return _grav_const;
  };
  /**
  * @brief Ellipsoid J2-4 parameters
  */
  const double*   get_ellip_j() const
  {
    return _ellip_j;
  };

  /**
  * @brief Average terrain height, km
  */
  double   get_terrain_h() const
  {
    return _terrain_h;
  };
  /**
  * @brief Scene centre line number
  */
  int   get_sc_lin() const
  {
    return _sc_lin;
  };
  /**
  * @brief Scene centre pixel number
  */
  int   get_sc_pix() const
  {
    return _sc_pix;
  };
  /**
  * @brief Scene length, km
  */
  double   get_scene_len() const
  {
    return _scene_len;
  };
  /**
  * @brief Scene width, km
  */
  double   get_scene_wid() const
  {
    return _scene_wid;
  };

  /**
  * @brief Number of SAR channels
  */
  double   get_nchn() const
  {
    return _nchn;
  };

  /**
  * @brief Mission identifier
  */
  std::string   get_mission_id() const
  {
    return _mission_id;
  };
  /**
  * @brief Sensor identifier
  */
  std::string   get_sensor_id() const
  {
    return _sensor_id;
  };
  /**
  * @brief Orbit number
  */
  std::string   get_orbit_num() const
  {
    return _orbit_num;
  };
  /**
  * @brief Platform geodetic latitude
  */
  double   get_plat_lat() const
  {
    return _plat_lat;
  };
  /**
  * @brief Platform geodetic longitude
  */
  double   get_plat_long() const
  {
    return _plat_long;
  };
  /**
  * @brief Platform heading
  */
  double   get_plat_head() const
  {
    return _plat_head;
  };
  /**
  * @brief Sensor clock angle
  */
  double   get_clock_ang() const
  {
    return _clock_ang;
  };
  /**
  * @brief Incidence angle
  */
  double   get_incident_ang() const
  {
    return _incident_ang;
  };

  /**
  * @brief Radar wave length
  */
  double   get_wave_length() const
  {
    return _wave_length;
  };
  /**
  * @brief Motion compensation indicator
  */
  std::string   get_motion_comp() const
  {
    return _motion_comp;
  };
  /**
  * @brief Range pulse code specifier
  */
  std::string   get_pulse_code() const
  {
    return _pulse_code;
  };
  /**
  * @brief Range chirp coefficients
  */
  const double*   get_ampl_coef() const
  {
    return _ampl_coef;
  };
  /**
  * @brief Range phase coefficients
  */
  const double*   get_phas_coef() const
  {
    return _phas_coef;
  };
  /**
  * @brief Chirp extraction index
  */
  int   get_chirp_ext_ind() const
  {
    return _chirp_ext_ind;
  };

  /**
  * @brief Range sampling rate
  */
  double   get_fr() const
  {
    return _fr;
  };
  /**
  * @brief Range gate start time
  */
  double   get_rng_gate() const
  {
    return _rng_gate;
  };
  /**
  * @brief Range pulse length
  */
  double   get_rng_length() const
  {
    return _rng_length;
  };
  /**
  * @brief Baseband conversion flag
  */
  std::string   get_baseband_f() const
  {
    return _baseband_f;
  };
  /**
  * @brief Range compressed flag
  */
  std::string   get_rngcmp_f() const
  {
    return _rngcmp_f;
  };
  /**
  * @brief Like polarized gain
  */
  double   get_gn_polar() const
  {
    return _gn_polar;
  };
  /**
  * @brief Cross polarized gain
  */
  double   get_gn_cross() const
  {
    return _gn_cross;
  };
  /**
  * @brief Number of bits per channel
  */
  int   get_chn_bits() const
  {
    return _chn_bits;
  };
  /**
  * @brief Quantization descriptor
  */
  std::string   get_quant_desc() const
  {
    return _quant_desc;
  };
  /**
  * @brief I channel DC bias
  */
  double   get_i_bias() const
  {
    return _i_bias;
  };
  /**
  * @brief Q channel DC bias
  */
  double   get_q_bias() const
  {
    return _q_bias;
  };
  /**
  * @brief I/Q channel ratio
  */
  double   get_iq_ratio() const
  {
    return _iq_ratio;
  };


  /**
  * @brief Mechanical boresight
  */
  double   get_mech_sight() const
  {
    return _mech_sight;
  };

  /**
  * @brief Nominal PRF, Hz
  */
  double   get_fa() const
  {
    return _fa;
  };

  /**
  * @brief Satellite binary time
  */
  std::string    get_sat_bintim() const
  {
    return _sat_bintim;
  };
  /**
  * @brief Satellite clock time
  */
  std::string    get_sat_clktim() const
  {
    return _sat_clktim;
  };
  /**
  * @brief Satellite clock increment
  */
  std::string    get_sat_clkinc() const
  {
    return _sat_clkinc;
  };

  /**
  * @brief Processing facility identifier
  */
  std::string   get_fac_id() const
  {
    return _fac_id;
  };
  /**
  * @brief Processing system identifier
  */
  std::string   get_sys_id() const
  {
    return _sys_id;
  };
  /**
  * @brief Processing version identifier
  */
  std::string   get_ver_id() const
  {
    return _ver_id;
  };

  /**
  * @brief Product type specifier
  */
  std::string   get_prod_type() const
  {
    return _prod_type;
  };
  /**
  * @brief Processing algorithm identifier
  */
  std::string   get_algor_id() const
  {
    return _algor_id;
  };
  /**
  * @brief Number of azimuth looks
  */
  double   get_n_azilok() const
  {
    return _n_azilok;
  };
  /**
  * @brief Number of range looks
  */
  double   get_n_rnglok() const
  {
    return _n_rnglok;
  };
  /**
  * @brief Bandwidth per look in azimuth,Hz
  */
  double   get_bnd_azilok() const
  {
    return _bnd_azilok;
  };
  /**
  * @brief Bandwidth per look in range,Hz
  */
  double   get_bnd_rnglok() const
  {
    return _bnd_rnglok;
  };
  /**
  * @brief Total azimuth look bandwidth
  */
  double   get_bnd_azi() const
  {
    return _bnd_azi;
  };
  /**
  * @brief Total range look bandwidth
  */
  double   get_bnd_rng() const
  {
    return _bnd_rng;
  };
  /**
  * @brief Azimuth weighting designator
  */
  std::string   get_azi_weight() const
  {
    return _azi_weight;
  };
  /**
  * @brief Range weighting designator
  */
  std::string   get_rng_weight() const
  {
    return _rng_weight;
  };
  /**
  * @brief Data input source
  */
  std::string   get_data_inpsrc() const
  {
    return _data_inpsrc;
  };
  /**
  * @brief Range resolution, meter
  */
  double   get_rng_res() const
  {
    return _rng_res;
  };
  /**
  * @brief Azimuth resolution, meter
  */
  double   get_azi_res() const
  {
    return _azi_res;
  };
  /**
  * @brief Along track Doppler frequency terms
  */
  const double*   get_alt_dopcen() const
  {
    return _alt_dopcen;
  };

  /**
  * @brief Cross track Doppler frequency terms
  */
  const double*   get_crt_dopcen() const
  {
    return _crt_dopcen;
  };
  /**
  * @brief Pixel time direction indicator
  */
  std::string   get_time_dir_pix() const
  {
    return _time_dir_pix;
  };
  /**
  * @brief Line time direction indicator
  */
  std::string   get_time_dir_lin() const
  {
    return _time_dir_lin;
  };
  /**
  * @brief Along track Doppler rate term
  */
  const  double*   get_alt_rate() const
  {
    return _alt_rate;
  };

  /**
  * @brief Cross track Doppler rate term
  */
  const  double*   get_crt_rate() const
  {
    return _crt_rate;
  };

  /**
  * @brief Line content indicator
  */
  std::string   get_line_cont() const
  {
    return _line_cont;
  };
  /**
  * @brief Clutter lock applied flag
  */
  std::string   get_clutter_lock() const
  {
    return _clutter_lock;
  };
  /**
  * @brief Auto-focus applied flag
  */
  std::string   get_auto_focus() const
  {
    return _auto_focus;
  };
  /**
  * @brief Line spacing, meters
  */
  double   get_line_spacing() const
  {
    return _line_spacing;
  };
  /**
  * @brief Pixel spacing, meters
  */
  double   get_pix_spacing() const
  {
    return _pix_spacing;
  };
  /**
  * @brief Range compression designator
  */
  std::string   get_rngcmp_desg() const
  {
    return _rngcmp_desg;
  };

  /**
  * @brief Along track Doppler frequency terms (in range unit: km)
  */
  const double*   get_dopcen_range() const
  {
    return _dopcen_range;
  };

  /**
  * @Data from 1767 to 4096 to be added
  */


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
  * @brief Mechanical boresight
  */
  double   _mech_sight;
  /**
  * @brief Nominal PRF, Hz
  */
  double   _fa;
  /**
  * @brief Satellite binary time
  */
  std::string    _sat_bintim;
  /**
  * @brief Satellite clock time
  */
  std::string    _sat_clktim;
  /**
  * @brief Satellite clock increment
  */
  std::string    _sat_clkinc;

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

  /**
  * @brief Cross track Doppler frequency terms (in range unit: km)
  */
  double   _dopcen_range[2];

  /**
  * @Data from 1767 to 4096 to be added
  */

private:

};
}
#endif
