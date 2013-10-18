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

#ifndef ProcessingParameters_h
#define ProcessingParameters_h

#include <iostream>
#include <RadarSat/RadarSatRecord.h>
#include <RadarSat/RadarSatRecordHeader.h>

#include "BeamInformationRecord.h"
#include "DopplerCentroidEstimateRecord.h"
#include "BeamPixelCountRecord.h"
#include "SRGRCoefficientSetRecord.h"
#include "TemperatureSettingsRecord.h"

namespace ossimplugins
{
/**
 * @ingroup ProcessingParametersRecord
 * @brief This class is able to read a Detailed processing parameters record
 */
class ProcessingParameters : public RadarSatRecord
{
public:
  /**
   * @brief Constructor
   */
  ProcessingParameters();

  /**
   * @brief Destructor
   */
  ~ProcessingParameters();

  /**
   * @brief Copy constructor
   */
  ProcessingParameters(const ProcessingParameters& rhs);

  /**
   * @brief Copy operator
   */
  ProcessingParameters& operator=(const ProcessingParameters& rhs);
  /**
   * @brief This function writes the ProcessingParameters in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const ProcessingParameters& data);

  /**
   * @brief This function reads a ProcessingParameters from a stream
   */
  friend std::istream& operator>>(std::istream& is, ProcessingParameters& data);

  /**
   * @brief This function is able to create a new instance of the class
   */
  RadarSatRecord* Instanciate()
  {
    return new ProcessingParameters();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  RadarSatRecord* Clone()
  {
    return new ProcessingParameters(*this);
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
   * @brief Input media
   */
  std::string  get_inp_media()
  {
    return  _inp_media;
  };
    /**
   * @brief Number of tape input ids
   */
  int   get_n_tape_id()
  {
    return _n_tape_id;
  };
    /**
   * @brief Tape identifiers
   */
  std::string*   get_tape_id()
  {
    return _tape_id;
  };
    /**
   * @brief Expected ingest start time
   */
  std::string   get_exp_ing_start()
  {
    return _exp_ing_start;
  };
    /**
   * @brief Expected ingest stop time
   */
  std::string   get_exp_ing_stop()
  {
    return _exp_ing_stop;
  };
    /**
   * @brief Actual ingest start time
   */
  std::string   get_act_ing_start()
  {
    return _act_ing_start;
  };
    /**
   * @brief Actual ingest stop time
   */
  std::string   get_act_ing_stop()
  {
    return _act_ing_stop;
  };
    /**
   * @brief Processing start time
   */
  std::string   get_proc_start()
  {
    return _proc_start;
  };
    /**
   * @brief Processing stop time
   */
  std::string   get_proc_stop()
  {
    return _proc_stop;
  };
    /**
   * @brief Mean signal levels across range
   */
  double*   get_mn_sig_lev()
  {
    return _mn_sig_lev;
  };
    /**
   * @brief Source data quality indicator
   */
  int   get_scr_data_ind()
  {
    return _scr_data_ind;
  };
    /**
   * @brief Number of missing lines
   */
  int   get_miss_ln()
  {
    return _miss_ln;
  };
    /**
   * @brief Number of rejected lines
   */
  int   get_rej_ln()
  {
    return _rej_ln;
  };
    /**
   * @brief Number of time inconsistencies (large gaps)
   */
  int   get_large_gap()
  {
    return _large_gap;
  };
    /**
   * @brief Measured bit error rate
   */
  double   get_bit_err_rate()
  {
    return _bit_err_rate;
  };
    /**
   * @brief Percent of frames with CRC errors
   */
  double   get_fm_crc_err()
  {
    return _fm_crc_err;
  };
    /**
   * @brief Number of date inconsistencies
   */
  int   get_date_incons()
  {
    return _date_incons;
  };
    /**
   * @brief Number of unexpected PRF changes
   */
  int   get_prf_changes()
  {
    return _prf_changes;
  };
    /**
   * @brief Number of delay changes
   */
  int   get_delay_changes()
  {
    return _delay_changes;
  };
    /**
   * @brief Number of skipped frames
   */
  int   get_skipd_frams()
  {
    return _skipd_frams;
  };
    /**
   * @brief Range lines rejected before start time
   */
  int   get_rej_bf_start()
  {
    return _rej_bf_start;
  };
    /**
   * @brief Range lines rejected due to too few frames
   */
  int   get_rej_few_fram()
  {
    return _rej_few_fram;
  };
    /**
   * @brief Range lines rejected due to too many frames
   */
  int   get_rej_many_fram()
  {
    return _rej_many_fram;
  };
    /**
   * @brief Frames rejected due to master channel error
   */
  int   get_rej_mchn_err()
  {
    return _rej_mchn_err;
  };
    /**
   * @brief Frames rejected due to virtual channel error
   */
  int   get_rej_vchn_err()
  {
    return _rej_vchn_err;
  };
    /**
   * @brief Frames rejected due to incorrect recording type
   */
  int   get_rej_rec_type()
  {
    return _rej_rec_type;
  };
    /**
   * @brief Sensor configuration
   */
  std::string   get_sens_config()
  {
    return _sens_config;
  };
    /**
   * @brief Sensor orientation
   */
  std::string   get_sens_orient()
  {
    return _sens_orient;
  };
    /**
   * @brief Frame synch marker
   */
  std::string   get_sych_marker()
  {
    return _sych_marker;
  };
    /**
   * @brief Range reference function source
   */
  std::string   get_rng_ref_src()
  {
    return _rng_ref_src;
  };
    /**
   * @brief Range reference amplitude coefficients
   */
  double*   get_rng_amp_coef()
  {
    return _rng_amp_coef;
  };
    /**
   * @brief Range reference phase coefficients
   */
  double*   get_rng_phas_coef()
  {
    return _rng_phas_coef;
  };
    /**
   * @brief Error function amplitude coefficients
   */
  double*   get_err_amp_coef()
  {
    return _err_amp_coef;
  };
    /**
   * @brief Error function phase coefficients
   */
  double*   get_err_phas_coef()
  {
    return _err_phas_coef;
  };
    /**
   * @brief Pulse bandwidth code
   */
  int   get_pulse_bandw()
  {
    return _pulse_bandw;
  };
    /**
   * @brief ADC sampling rate
   */
  std::string   get_adc_samp_rate()
  {
    return _adc_samp_rate;
  };
    /**
   * @brief Replica AGC attenuation
   */
  double   get_rep_agc_attn()
  {
    return _rep_agc_attn;
  };
    /**
   * @brief Gain correction factor (dB)
   */
  double   get_gn_corctn_fctr()
  {
    return _gn_corctn_fctr;
  };
    /**
   * @brief Replica energy gain correction
   */
  double   get_rep_energy_gn()
  {
    return _rep_energy_gn;
  };
    /**
   * @brief Orbit data source
   */
  std::string   get_orb_data_src()
  {
    return _orb_data_src;
  };
    /**
   * @brief Pulse count 1
   */
  int   get_pulse_cnt_1()
  {
    return _pulse_cnt_1;
  };
    /**
   * @brief Pulse count 2
   */
  int   get_pulse_cnt_2()
  {
    return _pulse_cnt_2;
  };
    /**
   * @brief Beam edge detection requested
   */
  std::string    get_beam_edge_rqd()
  {
    return _beam_edge_rqd;
  };
    /**
   * @brief Beam edge confidence measure
   */
  double   get_beam_edge_conf()
  {
    return _beam_edge_conf;
  };
    /**
   * @brief Number of pixels in beam overlap
   */
  int   get_pix_overlap()
  {
    return _pix_overlap;
  };
    /**
   * @brief Number of beams
   */
  int   get_n_beams()
  {
    return _n_beams;
  };
    /**
   * @brief Beam info
   */
  BeamInformationRecord* get_beam_info()
  {
    return _beam_info;
  };
    /**
   * @brief Number of pixel count updates
   */
  int   get_n_pix_updates()
  {
    return _n_pix_updates;
  };
  /**
   * @brief Beam pixel counts
   */
  BeamPixelCountRecord* get_pix_count()
  {
    return _pix_count;
  };
    /**
   * @brief Processing window start time (sec)
   */
  double   get_pwin_start()
  {
    return _pwin_start;
  };
    /**
   * @brief Processing window end time (sec)
   */
  double   get_pwin_end()
  {
    return _pwin_end;
  };
    /**
   * @brief Recording type
   */
  std::string   get_recd_type()
  {
    return _recd_type;
  };
    /**
   * @brief Time increment between temperature settings (sec)
   */
  double   get_temp_set_inc()
  {
    return _temp_set_inc;
  };
    /**
   * @brief Number of temperature settings
   */
  int   get_n_temp_set()
  {
    return _n_temp_set;
  };
  /**
   * @brief Temperature settings
   */
  TemperatureSettingsRecord* get_temp()
  {
    return _temp;
  };
    /**
   * @brief Number of image pixels
   */
  int   get_n_image_pix()
  {
    return _n_image_pix;
  };
    /**
   * @brief Percent zero pixels
   */
  double   get_prc_zero_pix()
  {
    return _prc_zero_pix;
  };
    /**
   * @brief Percent saturated pixels
   */
  double   get_prc_satur_pix()
  {
    return _prc_satur_pix;
  };
    /**
   * @brief Image histogram mean intensity
   */
  double   get_img_hist_mean()
  {
    return _img_hist_mean;
  };
    /**
   * @brief Image cumulative distribution
   */
  double*   get_img_cumu_dist()
  {
    return _img_cumu_dist;
  };
    /**
   * @brief Pre-image calibration gain factor
   */
  double   get_pre_img_gn()
  {
    return _pre_img_gn;
  };
    /**
   * @brief Post-image calibration gain factor
   */
  double   get_post_img_gn()
  {
    return _post_img_gn;
  };
    /**
   * @brief Time increment between Dopcen estimates (sec)
   */
  double   get_dopcen_inc()
  {
    return _dopcen_inc;
  };
    /**
   * @brief Number of Doppler centroid estimates
   */
  int   get_n_dopcen()
  {
    return _n_dopcen;
  };
  /**
   * @brief Doppler centroid estimates
   */
  DopplerCentroidEstimateRecord* get_dopcen_est()
  {
    return _dopcen_est;
  };
    /**
   * @brief Doppler ambiguity error
   */
  int   get_dop_amb_err()
  {
    return _dop_amb_err;
  };
    /**
   * @brief Doppler ambiguity confidence measure
   */
  double   get_dopamb_conf()
  {
    return _dopamb_conf;
  };
    /**
   * @brief Ephemeris orbit data
   */
  double*   get_eph_orb_data()
  {
    return _eph_orb_data;
  };
    /**
   * @brief Application type
   */
  std::string   get_appl_type()
  {
    return _appl_type;
  };
    /**
   * @brief  Slow time coefficients
   * @todo Verifier le type de la variable lors de la lecture de la donnee
   */
  double*   get_slow_time_coef()
  {
    return _slow_time_coef;
  };
    /**
   * @brief Number of SRGR coefficient sets
   */
  int   get_n_srgr()
  {
    return _n_srgr;
  };
  /**
   * @brief SRGR coefficient sets
   */
  SRGRCoefficientSetRecord* get_srgr_coefset()
  {
    return _srgr_coefset;
  };
    /**
   * @brief SGF product pixel spacing
   */
  double   get_pixel_spacing()
  {
    return _pixel_spacing;
  };
    /**
   * @brief GICS product required
   */
  std::string   get_gics_reqd()
  {
    return _gics_reqd;
  };
    /**
   * @brief Work order identifier
   */
  std::string   get_wo_number()
  {
    return _wo_number;
  };
    /**
   * @brief Work order entry date
   */
  std::string   get_wo_date()
  {
    return _wo_date;
  };
    /**
   * @brief Satellite identifier
   */
  std::string   get_satellite_id()
  {
    return _satellite_id;
  };
    /**
   * @brief User id
   */
  std::string   get_user_id()
  {
    return _user_id;
  };
    /**
   * @brief Completion message required flag
   */
  std::string   get_complete_msg()
  {
    return _complete_msg;
  };
    /**
   * @brief SGF product scene identifier
   */
  std::string   get_scene_id()
  {
    return _scene_id;
  };
    /**
   * @brief Density of SGF product media
   */
  std::string   get_density_in()
  {
    return _density_in;
  };
    /**
   * @brief SGF product identifier
   */
  std::string   get_media_id()
  {
    return _media_id;
  };
    /**
   * @brief Incidence angle of first pixel in SGF product line
   */
  double   get_angle_first()
  {
    return _angle_first;
  };
    /**
   * @brief Incidence angle of last pixel in SGF product line
   */
  double   get_angle_last()
  {
    return _angle_last;
  };
    /**
   * @brief GICS output product type
   */
  std::string   get_prod_type()
  {
    return _prod_type;
  };
    /**
   * @brief Map system identifier
   */
  std::string   get_map_system()
  {
    return _map_system;
  };
    /**
   * @brief GICS output product scene centre latitude
   */
  double   get_centre_lat()
  {
    return _centre_lat;
  };
    /**
   * @brief GICS output product scene centre longitude
   */
  double   get_centre_long()
  {
    return _centre_long;
  };
    /**
   * @brief GICS output product size - map eastings (km)
   */
  double   get_span_x()
  {
    return _span_x;
  };
    /**
   * @brief GICS output product size - map northings (km)
   */
  double   get_span_y()
  {
    return _span_y;
  };
    /**
   * @brief DTM correction to be applied flag
   */
  std::string   get_apply_dtm()
  {
    return _apply_dtm;
  };
    /**
   * @brief GICS output product density
   */
  std::string   get_density_out()
  {
    return _density_out;
  };
    /**
   * @brief Time of the first state vector
   */
  std::string   get_state_time()
  {
    return _state_time;
  };
    /**
   * @brief Number of state vectors
   */
  int   get_num_state_vectors()
  {
    return _num_state_vectors;
  };
    /**
   * @brief Time increment between state vectors
   */
  double   get_state_time_inc()
  {
    return _state_time_inc;
  };

protected:
  /**
   * @brief Record sequence number
   */
  int   _rec_seq;

    /**
   * @brief Input media
   */
  std::string   _inp_media;
    /**
   * @brief Number of tape input ids
   */
  int   _n_tape_id;
    /**
   * @brief Tape identifiers
   */
  std::string   _tape_id[10];
    /**
   * @brief Expected ingest start time
   */
  std::string   _exp_ing_start;
    /**
   * @brief Expected ingest stop time
   */
  std::string   _exp_ing_stop;
    /**
   * @brief Actual ingest start time
   */
  std::string   _act_ing_start;
    /**
   * @brief Actual ingest stop time
   */
  std::string   _act_ing_stop;
    /**
   * @brief Processing start time
   */
  std::string   _proc_start;
    /**
   * @brief Processing stop time
   */
  std::string   _proc_stop;
    /**
   * @brief Mean signal levels across range
   */
  double   _mn_sig_lev[10];
    /**
   * @brief Source data quality indicator
   */
  int   _scr_data_ind;
    /**
   * @brief Number of missing lines
   */
  int   _miss_ln;
    /**
   * @brief Number of rejected lines
   */
  int   _rej_ln;
    /**
   * @brief Number of time inconsistencies (large gaps)
   */
  int   _large_gap;
    /**
   * @brief Measured bit error rate
   */
  double   _bit_err_rate;
    /**
   * @brief Percent of frames with CRC errors
   */
  double   _fm_crc_err;
    /**
   * @brief Number of date inconsistencies
   */
  int   _date_incons;
    /**
   * @brief Number of unexpected PRF changes
   */
  int   _prf_changes;
    /**
   * @brief Number of delay changes
   */
  int   _delay_changes;
    /**
   * @brief Number of skipped frames
   */
  int   _skipd_frams;
    /**
   * @brief Range lines rejected before start time
   */
  int   _rej_bf_start;
    /**
   * @brief Range lines rejected due to too few frames
   */
  int   _rej_few_fram;
    /**
   * @brief Range lines rejected due to too many frames
   */
  int   _rej_many_fram;
    /**
   * @brief Frames rejected due to master channel error
   */
  int   _rej_mchn_err;
    /**
   * @brief Frames rejected due to virtual channel error
   */
  int   _rej_vchn_err;
    /**
   * @brief Frames rejected due to incorrect recording type
   */
  int   _rej_rec_type;
    /**
   * @brief Sensor configuration
   */
  std::string   _sens_config;
    /**
   * @brief Sensor orientation
   */
  std::string   _sens_orient;
    /**
   * @brief Frame synch marker
   */
  std::string   _sych_marker;
    /**
   * @brief Range reference function source
   */
  std::string   _rng_ref_src;
    /**
   * @brief Range reference amplitude coefficients
   */
  double   _rng_amp_coef[4];
    /**
   * @brief Range reference phase coefficients
   */
  double   _rng_phas_coef[4];
    /**
   * @brief Error function amplitude coefficients
   */
  double   _err_amp_coef[16];
    /**
   * @brief Error function phase coefficients
   */
  double   _err_phas_coef[4];
    /**
   * @brief Pulse bandwidth code
   */
  int   _pulse_bandw;
    /**
   * @brief ADC sampling rate
   */
  std::string   _adc_samp_rate;
    /**
   * @brief Replica AGC attenuation
   */
  double   _rep_agc_attn;
    /**
   * @brief Gain correction factor (dB)
   */
  double   _gn_corctn_fctr;
    /**
   * @brief Replica energy gain correction
   */
  double   _rep_energy_gn;
    /**
   * @brief Orbit data source
   */
  std::string   _orb_data_src;
    /**
   * @brief Pulse count 1
   */
  int   _pulse_cnt_1;
    /**
   * @brief Pulse count 2
   */
  int   _pulse_cnt_2;
    /**
   * @brief Beam edge detection requested
   */
  std::string    _beam_edge_rqd;
    /**
   * @brief Beam edge confidence measure
   */
  double   _beam_edge_conf;
    /**
   * @brief Number of pixels in beam overlap
   */
  int   _pix_overlap;
    /**
   * @brief Number of beams
   */
  int   _n_beams;
    /**
   * @brief Beam info
   */
  BeamInformationRecord _beam_info[4];
    /**
   * @brief Number of pixel count updates
   */
  int   _n_pix_updates;
  /**
   * @brief Beam pixel counts
   */
  BeamPixelCountRecord _pix_count[20];
    /**
   * @brief Processing window start time (sec)
   */
  double   _pwin_start;
    /**
   * @brief Processing window end time (sec)
   */
  double   _pwin_end;
    /**
   * @brief Recording type
   */
  std::string   _recd_type;
    /**
   * @brief Time increment between temperature settings (sec)
   */
  double   _temp_set_inc;
    /**
   * @brief Number of temperature settings
   */
  int   _n_temp_set;
  /**
   * @brief Temperature settings
   */
  TemperatureSettingsRecord _temp[20];
    /**
   * @brief Number of image pixels
   */
  int   _n_image_pix;
    /**
   * @brief Percent zero pixels
   */
  double   _prc_zero_pix;
    /**
   * @brief Percent saturated pixels
   */
  double   _prc_satur_pix;
    /**
   * @brief Image histogram mean intensity
   */
  double   _img_hist_mean;
    /**
   * @brief Image cumulative distribution
   */
  double   _img_cumu_dist[3];
    /**
   * @brief Pre-image calibration gain factor
   */
  double   _pre_img_gn;
    /**
   * @brief Post-image calibration gain factor
   */
  double   _post_img_gn;
    /**
   * @brief Time increment between Dopcen estimates (sec)
   */
  double   _dopcen_inc;
    /**
   * @brief Number of Doppler centroid estimates
   */
  int   _n_dopcen;
  /**
   * @brief Doppler centroid estimates
   */
  DopplerCentroidEstimateRecord _dopcen_est[20];
    /**
   * @brief Doppler ambiguity error
   */
  int   _dop_amb_err;
    /**
   * @brief Doppler ambiguity confidence measure
   */
  double   _dopamb_conf;
    /**
   * @brief Ephemeris orbit data
   */
  double   _eph_orb_data[7];
    /**
   * @brief Application type
   */
  std::string   _appl_type;
    /**
   * @brief  Slow time coefficients
   * @todo Verifier le type de la variable lors de la lecture de la donnee
   */
  double   _slow_time_coef[5];
    /**
   * @brief Number of SRGR coefficient sets
   */
  int   _n_srgr;
  /**
   * @brief SRGR coefficient sets
   */
  SRGRCoefficientSetRecord _srgr_coefset[20];
    /**
   * @brief SGF product pixel spacing
   */
  double   _pixel_spacing;
    /**
   * @brief GICS product required
   */
  std::string   _gics_reqd;
    /**
   * @brief Work order identifier
   */
  std::string   _wo_number;
    /**
   * @brief Work order entry date
   */
  std::string   _wo_date;
    /**
   * @brief Satellite identifier
   */
  std::string   _satellite_id;
    /**
   * @brief User id
   */
  std::string   _user_id;
    /**
   * @brief Completion message required flag
   */
  std::string   _complete_msg;
    /**
   * @brief SGF product scene identifier
   */
  std::string   _scene_id;
    /**
   * @brief Density of SGF product media
   */
  std::string   _density_in;
    /**
   * @brief SGF product identifier
   */
  std::string   _media_id;
    /**
   * @brief Incidence angle of first pixel in SGF product line
   */
  double   _angle_first;
    /**
   * @brief Incidence angle of last pixel in SGF product line
   */
  double   _angle_last;
    /**
   * @brief GICS output product type
   */
  std::string   _prod_type;
    /**
   * @brief Map system identifier
   */
  std::string   _map_system;
    /**
   * @brief GICS output product scene centre latitude
   */
  double   _centre_lat;
    /**
   * @brief GICS output product scene centre longitude
   */
  double   _centre_long;
    /**
   * @brief GICS output product size - map eastings (km)
   */
  double   _span_x;
    /**
   * @brief GICS output product size - map northings (km)
   */
  double   _span_y;
    /**
   * @brief DTM correction to be applied flag
   */
  std::string   _apply_dtm;
    /**
   * @brief GICS output product density
   */
  std::string   _density_out;
    /**
   * @brief Time of the first state vector
   */
  std::string   _state_time;
    /**
   * @brief Number of state vectors
   */
  int   _num_state_vectors;
    /**
   * @brief Time increment between state vectors
   */
  double   _state_time_inc;

private:
};
}
#endif
