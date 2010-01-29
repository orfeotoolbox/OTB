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

#ifndef ErsSarFacilityData_h
#define ErsSarFacilityData_h

#include <iostream>
#include <cstdlib>
#include "erssar/ErsSarRecordHeader.h"
#include "erssar/ErsSarRecord.h"

namespace ossimplugins
{

/**
 * @ingroup ErsSarFacilityDataRecord
 * @brief This class is able to read the SAR leader data set summary record of the leader file
 *
 */
class ErsSarFacilityData : public ErsSarRecord
{
public:
  /**
   * @brief Constructor
   */
  ErsSarFacilityData();

  /**
   * @brief Destructor
   */
  virtual ~ErsSarFacilityData();

  /**
   * @brief This function write the ErsSarFacilityData in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const ErsSarFacilityData& data);

  /**
   * @brief This function read a ErsSarFacilityData from a stream
   */
  friend std::istream& operator>>(std::istream& is, ErsSarFacilityData& data);

  /**
   * @brief Copy constructor
   */
  ErsSarFacilityData(const ErsSarFacilityData& rhs);

  /**
   * @brief Copy operator
   */
  ErsSarFacilityData& operator=(const ErsSarFacilityData& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  ErsSarRecord* Instanciate()
  {
    return new ErsSarFacilityData();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  ErsSarRecord* Clone()
  {
    return new ErsSarFacilityData(*this);
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
  * @brief name_of_facil_rec
  */
  std::string   get_name_of_facil_rec() const
  {
    return _name_of_facil_rec;
  };

  /**
  * @brief last_release_qc_date
  */
  std::string   get_last_release_qc_date() const
  {
    return _last_release_qc_date;
  };
  /**
  * @brief last_release_cal_date
  */
  std::string   get_last_release_cal_date() const
  {
    return _last_release_cal_date;
  };
  /**
  * @brief qa_summary_flag
  */
  int   get_qa_summary_flag() const
  {
    return _qa_summary_flag;
  };
  /**
  * @brief prf_code_change_flag
  */
  int   get_prf_code_change_flag() const
  {
    return _prf_code_change_flag;
  };
  /**
  * @brief sampling_win_change_flag
  */
  int   get_sampling_win_change_flag() const
  {
    return _sampling_win_change_flag;
  };
  /**
  * @brief cal_gain_change_flag
  */
  int   get_cal_gain_change_flag() const
  {
    return _cal_gain_change_flag;
  };
  /**
  * @brief quirp_qu_flag
  */
  int   get_quirp_qu_flag() const
  {
    return _quirp_qu_flag;
  };
  /**
  * @brief inp_data_stat_flag
  */
  int   get_inp_data_stat_flag() const
  {
    return _inp_data_stat_flag;
  };
  /**
  * @brief dopp_cent_conf_meas_flag
  */
  int   get_dopp_cent_conf_meas_flag() const
  {
    return _dopp_cent_conf_meas_flag;
  };
  /**
  * @brief dopp_cent_val_flag
  */
  int   get_dopp_cent_val_flag() const
  {
    return _dopp_cent_val_flag;
  };
  /**
  * @brief dopp_ambig_conf_meas_flag
  */
  int   get_dopp_ambig_conf_meas_flag() const
  {
    return _dopp_ambig_conf_meas_flag;
  };
  /**
  * @brief outp_data_mean_flag
  */
  int   get_outp_data_mean_flag() const
  {
    return _outp_data_mean_flag;
  };
  /**
  * @brief OGOB_flag
  */
  int   get_OGOB_flag() const
  {
    return _OGOB_flag;
  };
  /**
  * @brief PRF_changes
  */
  int   get_PRF_changes() const
  {
    return _PRF_changes;
  };
  /**
  * @brief sampling_win_changes
  */
  int   get_sampling_win_changes() const
  {
    return _sampling_win_changes;
  };
  /**
  * @brief cal_gain_changes
  */
  int   get_cal_gain_changes() const
  {
    return _cal_gain_changes;
  };
  /**
  * @brief missing_lines
  */
  int   get_missing_lines() const
  {
    return _missing_lines;
  };
  /**
  * @brief rec_gain_changes
  */
  int   get_rec_gain_changes() const
  {
    return _rec_gain_changes;
  };
  /**
  * @brief pulse_width_of_ACF_3db
  */
  double   get_pulse_width_of_ACF_3db() const
  {
    return _pulse_width_of_ACF_3db;
  };
  /**
  * @brief first_side_lobe_lev_of_ACF
  */
  double   get_first_side_lobe_lev_of_ACF() const
  {
    return _first_side_lobe_lev_of_ACF;
  };
  /**
  * @brief ISLR_of_ACF
  */
  double   get_ISLR_of_ACF() const
  {
    return _ISLR_of_ACF;
  };
  /**
  * @brief dopp_cent_conf_meas
  */
  double   get_dopp_cent_conf_meas() const
  {
    return _dopp_cent_conf_meas;
  };
  /**
  * @brief dopp_ambig_conf_meas
  */
  double   get_dopp_ambig_conf_meas() const
  {
    return _dopp_ambig_conf_meas;
  };
  /**
  * @brief inp_data_I_mean
  */
  double   get_inp_data_I_mean() const
  {
    return _inp_data_I_mean;
  };
  /**
  * @brief inp_data_Q_mean
  */
  double   get_inp_data_Q_mean() const
  {
    return _inp_data_Q_mean;
  };
  /**
  * @brief inp_data_I_stddev
  */
  double   get_inp_data_I_stddev() const
  {
    return _inp_data_I_stddev;
  };
  /**
  * @brief inp_data_Q_stddev
  */
  double   get_inp_data_Q_stddev() const
  {
    return _inp_data_Q_stddev;
  };
  /**
  * @brief cal_sys_gain
  */
  double   get_cal_sys_gain() const
  {
    return _cal_sys_gain;
  };
  /**
  * @brief first_rec_gain_read
  */
  double   get_first_rec_gain_read() const
  {
    return _first_rec_gain_read;
  };
  /**
  * @brief dopp_ambig_num
  */
  double   get_dopp_ambig_num() const
  {
    return _dopp_ambig_num;
  };
  /**
  * @brief I_channel_bias_correction
  */
  double   get_I_channel_bias_correction() const
  {
    return _I_channel_bias_correction;
  };
  /**
  * @brief Q_channel_bias_correction
  */
  double   get_Q_channel_bias_correction() const
  {
    return _Q_channel_bias_correction;
  };
  /**
  * @brief I_channel_gain_correction
  */
  double   get_I_channel_gain_correction() const
  {
    return _I_channel_gain_correction;
  };
  /**
  * @brief Q_channel_gain_correction
  */
  double   get_Q_channel_gain_correction() const
  {
    return _Q_channel_gain_correction;
  };
  /**
  * @brief Q_channel_I_Q_correction
  */
  double   get_Q_channel_I_Q_correction() const
  {
    return _Q_channel_I_Q_correction;
  };
  /**
  * @brief noise_power
  */
  double   get_noise_power() const
  {
    return _noise_power;
  };
  /**
  * @brief int_cal_utc
  */
  int   get_int_cal_utc() const
  {
    return _int_cal_utc;
  };
  /**
  * @brief num_valid_cal_pulses
  */
  int   get_num_valid_cal_pulses() const
  {
    return _num_valid_cal_pulses;
  };
  /**
  * @brief num_valid_noise_pulses
  */
  int   get_num_valid_noise_pulses() const
  {
    return _num_valid_noise_pulses;
  };
  /**
  * @brief num_valid_replicas
  */
  int   get_num_valid_replicas() const
  {
    return _num_valid_replicas;
  };
  /**
  * @brief first_replica_sample
  */
  double   get_first_replica_sample() const
  {
    return _first_replica_sample;
  };
  /**
  * @brief mean_cal_pulse_power
  */
  double   get_mean_cal_pulse_power() const
  {
    return _mean_cal_pulse_power;
  };
  /**
  * @brief mean_noise_power
  */
  double   get_mean_noise_power() const
  {
    return _mean_noise_power;
  };
  /**
  * @brief range_comp_norm_fact
  */
  double   get_range_comp_norm_fact() const
  {
    return _range_comp_norm_fact;
  };
  /**
  * @brief replica_power
  */
  double   get_replica_power() const
  {
    return _replica_power;
  };
  /**
  * @brief first_range_pixel_mid_az_inc
  */
  double   get_first_range_pixel_mid_az_inc() const
  {
    return _first_range_pixel_mid_az_inc;
  };
  /**
  * @brief center_range_pix_mid_az_inc
  */
  double   get_center_range_pix_mid_az_inc() const
  {
    return _center_range_pix_mid_az_inc;
  };
  /**
  * @brief last_range_pix_mid_az_inc
  */
  double   get_last_range_pix_mid_az_inc() const
  {
    return _last_range_pix_mid_az_inc;
  };
  /**
  * @brief norm_ref_range_ro
  */
  double   get_norm_ref_range_ro() const
  {
    return _norm_ref_range_ro;
  };
  /**
  * @brief antenna_elev_flag
  */
  int   get_antenna_elev_flag() const
  {
    return _antenna_elev_flag;
  };
  /**
  * @brief abs_cal_const_K
  */
  double   get_abs_cal_const_K() const
  {
    return _abs_cal_const_K;
  };
  /**
  * @brief upp_bound_K
  */
  double   get_upp_bound_K() const
  {
    return _upp_bound_K;
  };
  /**
  * @brief low_bound_K
  */
  double   get_low_bound_K() const
  {
    return _low_bound_K;
  };
  /**
  * @brief proc_noise_scale_fact
  */
  double   get_proc_noise_scale_fact() const
  {
    return _proc_noise_scale_fact;
  };
  /**
  * @brief K_gen_date
  */
  std::string   get_K_gen_date() const
  {
    return _K_gen_date;
  };
  /**
  * @brief K_vers_num
  */
  std::string   get_K_vers_num() const
  {
    return _K_vers_num;
  };
  /**
  * @brief num_duplic_input_lines
  */
  int   get_num_duplic_input_lines() const
  {
    return _num_duplic_input_lines;
  };
  /**
  * @brief estim_bit_error_rate
  */
  double   get_estim_bit_error_rate() const
  {
    return _estim_bit_error_rate;
  };
  /**
  * @brief out_image_mean
  */
  double   get_out_image_mean() const
  {
    return _out_image_mean;
  };
  /**
  * @brief out_image_std_dev
  */
  double   get_out_image_std_dev() const
  {
    return _out_image_std_dev;
  };
  /**
  * @brief out_image_max_value
  */
  double   get_out_image_max_value() const
  {
    return _out_image_max_value;
  };
  /**
  * @brief time_raw_data_first_input
  */
  std::string   get_time_raw_data_first_input() const
  {
    return _time_raw_data_first_input;
  };
  /**
  * @brief time_asc_node_state_vectors
  */
  std::string   get_time_asc_node_state_vectors() const
  {
    return _time_asc_node_state_vectors;
  };
  /**
  * @brief asc_node_pos_X_comp
  */
  std::string   get_asc_node_pos_X_comp() const
  {
    return _asc_node_pos_X_comp;
  };
  /**
  * @brief asc_node_pos_Y_comp
  */
  std::string   get_asc_node_pos_Y_comp() const
  {
    return _asc_node_pos_Y_comp;
  };
  /**
  * @brief asc_node_pos_Z_comp
  */
  std::string   get_asc_node_pos_Z_comp() const
  {
    return _asc_node_pos_Z_comp;
  };
  /**
  * @brief asc_node_vel_X_comp
  */
  std::string   get_asc_node_vel_X_comp() const
  {
    return _asc_node_vel_X_comp;
  };
  /**
  * @brief asc_node_vel_Y_comp
  */
  std::string   get_asc_node_vel_Y_comp() const
  {
    return _asc_node_vel_Y_comp;
  };
  /**
  * @brief asc_node_vel_Z_comp
  */
  std::string   get_asc_node_vel_Z_comp() const
  {
    return _asc_node_vel_Z_comp;
  };
  /**
  * @brief out_pixel_bit_length
  */
  int   get_out_pixel_bit_length() const
  {
    return _out_pixel_bit_length;
  };
  /**
  * @brief proc_gain_param_1
  */
  double   get_proc_gain_param_1() const
  {
    return _proc_gain_param_1;
  };
  /**
  * @brief proc_gain_param_2
  */
  double   get_proc_gain_param_2() const
  {
    return _proc_gain_param_2;
  };
  /**
  * @brief proc_gain_param_3
  */
  double   get_proc_gain_param_3() const
  {
    return _proc_gain_param_3;
  };
  /**
  * @brief peak_loc_cross_correl_fun
  */
  int   get_peak_loc_cross_correl_fun() const
  {
    return _peak_loc_cross_correl_fun;
  };
  /**
  * @brief 3_dB_width_CCF
  */
  double   get_3_dB_width_CCF() const
  {
    return _3_dB_width_CCF;
  };
  /**
  * @brief first_side_lobe_level
  */
  double   get_first_side_lobe_level() const
  {
    return _first_side_lobe_level;
  };
  /**
  * @brief ISLR_CCF_between_last
  */
  double   get_ISLR_CCF_between_last() const
  {
    return _ISLR_CCF_between_last;
  };
  /**
  * @brief peak_loc_CCF_betw_last
  */
  int   get_peak_loc_CCF_betw_last() const
  {
    return _peak_loc_CCF_betw_last;
  };
  /**
  * @brief Roll_Tilt_Mode_flag
  */
  int   get_Roll_Tilt_Mode_flag() const
  {
    return _Roll_Tilt_Mode_flag;
  };
  /**
  * @brief raw_data_correction_flag
  */
  int   get_raw_data_correction_flag() const
  {
    return _raw_data_correction_flag;
  };
  /**
  * @brief look_detecion_flag
  */
  int   get_look_detecion_flag() const
  {
    return _look_detecion_flag;
  };
  /**
  * @brief doppler_ambiguity_estimat_flag
  */
  int   get_doppler_ambiguity_estimat_flag() const
  {
    return _doppler_ambiguity_estimat_flag;
  };
  /**
  * @brief azimuth_baseband_convers_flag
  */
  int   get_azimuth_baseband_convers_flag() const
  {
    return _azimuth_baseband_convers_flag;
  };
  /**
  * @brief samples_per_line_used
  */
  int   get_samples_per_line_used() const
  {
    return _samples_per_line_used;
  };
  /**
  * @brief range_lines_skip_factor
  */
  int   get_range_lines_skip_factor() const
  {
    return _range_lines_skip_factor;
  };
  /**
  * @brief time_of_inp_state_vectors
  */
  std::string   get_time_of_inp_state_vectors() const
  {
    return _time_of_inp_state_vectors;
  };
  /**
  * @brief inp_state_vect_pos_X_comp
  */
  std::string   get_inp_state_vect_pos_X_comp() const
  {
    return _inp_state_vect_pos_X_comp;
  };
  /**
  * @brief inp_state_vect_pos_Y_comp
  */
  std::string   get_inp_state_vect_pos_Y_comp() const
  {
    return _inp_state_vect_pos_Y_comp;
  };
  /**
  * @brief inp_state_vect_pos_Z_comp
  */
  std::string   get_inp_state_vect_pos_Z_comp() const
  {
    return _inp_state_vect_pos_Z_comp;
  };
  /**
  * @brief inp_state_vect_vel_Vx_comp
  */
  std::string   get_inp_state_vect_vel_Vx_comp() const
  {
    return _inp_state_vect_vel_Vx_comp;
  };
  /**
  * @brief inp_state_vect_vel_Vy_comp
  */
  std::string   get_inp_state_vect_vel_Vy_comp() const
  {
    return _inp_state_vect_vel_Vy_comp;
  };
  /**
  * @brief inp_state_vect_vel_Vz_comp
  */
  std::string   get_inp_state_vect_vel_Vz_comp() const
  {
    return _inp_state_vect_vel_Vz_comp;
  };
  /**
  * @brief inp_state_vector_type_flag
  */
  int   get_inp_state_vector_type_flag() const
  {
    return _inp_state_vector_type_flag;
  };
  /**
  * @brief win_coeff_for_range_match
  */
  double   get_win_coeff_for_range_match() const
  {
    return _win_coeff_for_range_match;
  };
  /**
  * @brief win_coeff_for_azi_match
  */
  double   get_win_coeff_for_azi_match() const
  {
    return _win_coeff_for_azi_match;
  };
  /**
  * @brief update_period_range_match
  */
  int   get_update_period_range_match() const
  {
    return _update_period_range_match;
  };
  /**
  * @brief look_scalar_gain_1
  */
  double   get_look_scalar_gain_1() const
  {
    return _look_scalar_gain_1;
  };
  /**
  * @brief look_scalar_gain_2
  */
  double   get_look_scalar_gain_2() const
  {
    return _look_scalar_gain_2;
  };
  /**
  * @brief look_scalar_gain_3
  */
  double   get_look_scalar_gain_3() const
  {
    return _look_scalar_gain_3;
  };
  /**
  * @brief look_scalar_gain_4
  */
  double   get_look_scalar_gain_4() const
  {
    return _look_scalar_gain_4;
  };
  /**
  * @brief look_scalar_gain_5
  */
  double   get_look_scalar_gain_5() const
  {
    return _look_scalar_gain_5;
  };
  /**
  * @brief look_scalar_gain_6
  */
  double   get_look_scalar_gain_6() const
  {
    return _look_scalar_gain_6;
  };
  /**
  * @brief look_scalar_gain_7
  */
  double   get_look_scalar_gain_7() const
  {
    return _look_scalar_gain_7;
  };
  /**
  * @brief look_scalar_gain_8
  */
  double   get_look_scalar_gain_8() const
  {
    return _look_scalar_gain_8;
  };
  /**
  * @brief samp_window_start_time_bias
  */
  int   get_samp_window_start_time_bias() const
  {
    return _samp_window_start_time_bias;
  };
  /**
  * @brief doppler_centroid_cubic_coeff
  */
  double   get_doppler_centroid_cubic_coeff() const
  {
    return _doppler_centroid_cubic_coeff;
  };
  /**
  * @brief PRF_code_first_range_line
  */
  int   get_PRF_code_first_range_line() const
  {
    return _PRF_code_first_range_line;
  };
  /**
  * @brief PRF_code_last_range_line
  */
  int   get_PRF_code_last_range_line() const
  {
    return _PRF_code_last_range_line;
  };
  /**
  * @brief samp_win_start_first
  */
  int   get_samp_win_start_first() const
  {
    return _samp_win_start_first;
  };
  /**
  * @brief samp_win_start_last
  */
  int   get_samp_win_start_last() const
  {
    return _samp_win_start_last;
  };
  /**
  * @brief cal_syst_gain_last_proc
  */
  int   get_cal_syst_gain_last_proc() const
  {
    return _cal_syst_gain_last_proc;
  };
  /**
  * @brief receiver_gain_last_proc
  */
  int   get_receiver_gain_last_proc() const
  {
    return _receiver_gain_last_proc;
  };
  /**
  * @brief first_processed_range_sample
  */
  int   get_first_processed_range_sample() const
  {
    return _first_processed_range_sample;
  };
  /**
  * @brief azimuth_FFT_IFFT_ratio
  */
  int   get_azimuth_FFT_IFFT_ratio() const
  {
    return _azimuth_FFT_IFFT_ratio;
  };
  /**
  * @brief num_azimuth_blocks_proc
  */
  int   get_num_azimuth_blocks_proc() const
  {
    return _num_azimuth_blocks_proc;
  };
  /**
  * @brief num_input_raw_data_lines
  */
  int   get_num_input_raw_data_lines() const
  {
    return _num_input_raw_data_lines;
  };
  /**
  * @brief initial_doppler_ambiguity_num
  */
  int   get_initial_doppler_ambiguity_num() const
  {
    return _initial_doppler_ambiguity_num;
  };
  /**
  * @brief thresh_no_1_flag
  */
  double   get_thresh_no_1_flag() const
  {
    return _thresh_no_1_flag;
  };
  /**
  * @brief thresh_no_2_flag
  */
  double   get_thresh_no_2_flag() const
  {
    return _thresh_no_2_flag;
  };
  /**
  * @brief thresh_no_3_flag
  */
  double   get_thresh_no_3_flag() const
  {
    return _thresh_no_3_flag;
  };
  /**
  * @brief thresh_no_4_flag
  */
  double   get_thresh_no_4_flag() const
  {
    return _thresh_no_4_flag;
  };
  /**
  * @brief thresh_no_5_flag
  */
  double   get_thresh_no_5_flag() const
  {
    return _thresh_no_5_flag;
  };
  /**
  * @brief thresh_no_6_flag
  */
  double   get_thresh_no_6_flag() const
  {
    return _thresh_no_6_flag;
  };
  /**
  * @brief thresh_no_7_flag
  */
  double   get_thresh_no_7_flag() const
  {
    return _thresh_no_7_flag;
  };
  /**
  * @brief thresh_no_8_flag
  */
  double   get_thresh_no_8_flag() const
  {
    return _thresh_no_8_flag;
  };
  /**
  * @brief thresh_no_9_flag
  */
  double   get_thresh_no_9_flag() const
  {
    return _thresh_no_9_flag;
  };
  /**
  * @brief thresh_no_10_flag
  */
  double   get_thresh_no_10_flag() const
  {
    return _thresh_no_10_flag;
  };
  /**
  * @brief thresh_no_11_flag
  */
  double   get_thresh_no_11_flag() const
  {
    return _thresh_no_11_flag;
  };
  /**
  * @brief sat_binary_time_of_first
  */
  int   get_sat_binary_time_of_first() const
  {
    return _sat_binary_time_of_first;
  };
  /**
  * @brief num_valid_pixels_per_range
  */
  int   get_num_valid_pixels_per_range() const
  {
    return _num_valid_pixels_per_range;
  };
  /**
  * @brief num_range_samp_discarded
  */
  int   get_num_range_samp_discarded() const
  {
    return _num_range_samp_discarded;
  };
  /**
  * @brief I_gain_imb_lower_bound
  */
  double   get_I_gain_imb_lower_bound() const
  {
    return _I_gain_imb_lower_bound;
  };
  /**
  * @brief I_gain_imb_upper_bound
  */
  double   get_I_gain_imb_upper_bound() const
  {
    return _I_gain_imb_upper_bound;
  };
  /**
  * @brief I_Q_quad_depar_lower_bound
  */
  double   get_I_Q_quad_depar_lower_bound() const
  {
    return _I_Q_quad_depar_lower_bound;
  };
  /**
  * @brief I_Q_quad_depar_upper_bound
  */
  double   get_I_Q_quad_depar_upper_bound() const
  {
    return _I_Q_quad_depar_upper_bound;
  };
  /**
  * @brief 3_dB_look_bandwidth
  */
  double   get_3_dB_look_bandwidth() const
  {
    return _3_dB_look_bandwidth;
  };
  /**
  * @brief 3_dB_look_proc_dopp_bandw
  */
  double   get_3_dB_look_proc_dopp_bandw() const
  {
    return _3_dB_look_proc_dopp_bandw;
  };
  /**
  * @brief range_spread_loss_comp_flag
  */
  int   get_range_spread_loss_comp_flag() const
  {
    return _range_spread_loss_comp_flag;
  };
  /**
  * @brief datation_flag
  */
  bool   get_datation_flag() const
  {
    return _datation_flag;
  };
  /**
  * @brief max_error_range_line_timing
  */
  int   get_max_error_range_line_timing() const
  {
    return _max_error_range_line_timing;
  };
  /**
  * @brief form_num_range_line_used
  */
  int   get_form_num_range_line_used() const
  {
    return _form_num_range_line_used;
  };
  /**
  * @brief autom_look_scal_gain_flag
  */
  bool   get_autom_look_scal_gain_flag() const
  {
    return _autom_look_scal_gain_flag;
  };
  /**
  * @brief max_value_look_scalar_gain
  */
  int   get_max_value_look_scalar_gain() const
  {
    return _max_value_look_scalar_gain;
  };
  /**
  * @brief replica_norm_method_flag
  */
  int   get_replica_norm_method_flag() const
  {
    return _replica_norm_method_flag;
  };
  /**
  * @brief coef_ground_range_1
  */
  double   get_coef_ground_range_1() const
  {
    return _coef_ground_range_1;
  };
  /**
  * @brief coef_ground_range_2
  */
  double   get_coef_ground_range_2() const
  {
    return _coef_ground_range_2;
  };
  /**
  * @brief coef_ground_range_3
  */
  double   get_coef_ground_range_3() const
  {
    return _coef_ground_range_3;
  };
  /**
  * @brief coef_ground_range_4
  */
  double   get_coef_ground_range_4() const
  {
    return _coef_ground_range_4;
  };
  /**
  * @brief coef_ant_elev_1
  */
  double   get_coef_ant_elev_1() const
  {
    return _coef_ant_elev_1;
  };
  /**
  * @brief coef_ant_elev_2
  */
  double   get_coef_ant_elev_2() const
  {
    return _coef_ant_elev_2;
  };
  /**
  * @brief coef_ant_elev_3
  */
  double   get_coef_ant_elev_3() const
  {
    return _coef_ant_elev_3;
  };
  /**
  * @brief coef_ant_elev_4
  */
  double   get_coef_ant_elev_4() const
  {
    return _coef_ant_elev_4;
  };
  /**
  * @brief coef_ant_elev_5
  */
  double   get_coef_ant_elev_5() const
  {
    return _coef_ant_elev_5;
  };
  /**
  * @brief range_time_origin_ant
  */
  double   get_range_time_origin_ant() const
  {
    return _range_time_origin_ant;
  };


protected:

  /**
  * @brief last_release_qc_date
  */
  std::string  _name_of_facil_rec;
  /**
  * @brief last_release_qc_date
  */
  std::string  _last_release_qc_date;
  /**
  * @brief last_release_cal_date
  */
  std::string   _last_release_cal_date;
  /**
  * @brief qa_summary_flag
  */
  int   _qa_summary_flag;
  /**
  * @brief prf_code_change_flag
  */
  int   _prf_code_change_flag;
  /**
  * @brief sampling_win_change_flag
  */
  int   _sampling_win_change_flag;
  /**
  * @brief cal_gain_change_flag
  */
  int   _cal_gain_change_flag;
  /**
  * @brief quirp_qu_flag
  */
  int   _quirp_qu_flag;
  /**
  * @brief inp_data_stat_flag
  */
  int   _inp_data_stat_flag;
  /**
  * @brief dopp_cent_conf_meas_flag
  */
  int   _dopp_cent_conf_meas_flag;
  /**
  * @brief dopp_cent_val_flag
  */
  int   _dopp_cent_val_flag;
  /**
  * @brief dopp_ambig_conf_meas_flag
  */
  int   _dopp_ambig_conf_meas_flag;
  /**
  * @brief outp_data_mean_flag
  */
  int   _outp_data_mean_flag;
  /**
  * @brief OGOB_flag
  */
  int   _OGOB_flag;
  /**
  * @brief PRF_changes
  */
  int   _PRF_changes;
  /**
  * @brief sampling_win_changes
  */
  int   _sampling_win_changes;
  /**
  * @brief cal_gain_changes
  */
  int   _cal_gain_changes;
  /**
  * @brief missing_lines
  */
  int   _missing_lines;
  /**
  * @brief rec_gain_changes
  */
  int   _rec_gain_changes;
  /**
  * @brief pulse_width_of_ACF_3db
  */
  double   _pulse_width_of_ACF_3db;
  /**
  * @brief first_side_lobe_lev_of_ACF
  */
  double   _first_side_lobe_lev_of_ACF;
  /**
  * @brief ISLR_of_ACF
  */
  double   _ISLR_of_ACF;
  /**
  * @brief dopp_cent_conf_meas
  */
  double   _dopp_cent_conf_meas;
  /**
  * @brief dopp_ambig_conf_meas
  */
  double   _dopp_ambig_conf_meas;
  /**
  * @brief inp_data_I_mean
  */
  double   _inp_data_I_mean;
  /**
  * @brief inp_data_Q_mean
  */
  double   _inp_data_Q_mean;
  /**
  * @brief inp_data_I_stddev
  */
  double   _inp_data_I_stddev;
  /**
  * @brief inp_data_Q_stddev
  */
  double   _inp_data_Q_stddev;
  /**
  * @brief cal_sys_gain
  */
  double   _cal_sys_gain;
  /**
  * @brief first_rec_gain_read
  */
  double   _first_rec_gain_read;
  /**
  * @brief dopp_ambig_num
  */
  double   _dopp_ambig_num;
  /**
  * @brief I_channel_bias_correction
  */
  double   _I_channel_bias_correction;
  /**
  * @brief Q_channel_bias_correction
  */
  double   _Q_channel_bias_correction;
  /**
  * @brief I_channel_gain_correction
  */
  double   _I_channel_gain_correction;
  /**
  * @brief Q_channel_gain_correction
  */
  double   _Q_channel_gain_correction;
  /**
  * @brief Q_channel_I_Q_correction
  */
  double   _Q_channel_I_Q_correction;
  /**
  * @brief noise_power
  */
  double   _noise_power;
  /**
  * @brief int_cal_utc
  */
  int   _int_cal_utc;
  /**
  * @brief num_valid_cal_pulses
  */
  int   _num_valid_cal_pulses;
  /**
  * @brief num_valid_noise_pulses
  */
  int   _num_valid_noise_pulses;
  /**
  * @brief num_valid_replicas
  */
  int   _num_valid_replicas;
  /**
  * @brief first_replica_sample
  */
  double   _first_replica_sample;
  /**
  * @brief mean_cal_pulse_power
  */
  double   _mean_cal_pulse_power;
  /**
  * @brief mean_noise_power
  */
  double   _mean_noise_power;
  /**
  * @brief range_comp_norm_fact
  */
  double   _range_comp_norm_fact;
  /**
  * @brief replica_power
  */
  double   _replica_power;
  /**
  * @brief first_range_pixel_mid_az_inc
  */
  double   _first_range_pixel_mid_az_inc;
  /**
  * @brief center_range_pix_mid_az_inc
  */
  double   _center_range_pix_mid_az_inc;
  /**
  * @brief last_range_pix_mid_az_inc
  */
  double   _last_range_pix_mid_az_inc;
  /**
  * @brief norm_ref_range_ro
  */
  double   _norm_ref_range_ro;
  /**
  * @brief antenna_elev_flag
  */
  int   _antenna_elev_flag;
  /**
  * @brief abs_cal_const_K
  */
  double   _abs_cal_const_K;
  /**
  * @brief upp_bound_K
  */
  double   _upp_bound_K;
  /**
  * @brief low_bound_K
  */
  double   _low_bound_K;
  /**
  * @brief proc_noise_scale_fact
  */
  double   _proc_noise_scale_fact;
  /**
  * @brief K_gen_date
  */
  std::string   _K_gen_date;
  /**
  * @brief K_vers_num
  */
  std::string   _K_vers_num;
  /**
  * @brief num_duplic_input_lines
  */
  int   _num_duplic_input_lines;
  /**
  * @brief estim_bit_error_rate
  */
  double   _estim_bit_error_rate;
  /**
  * @brief out_image_mean
  */
  double   _out_image_mean;
  /**
  * @brief out_image_std_dev
  */
  double   _out_image_std_dev;
  /**
  * @brief out_image_max_value
  */
  double   _out_image_max_value;
  /**
  * @brief time_raw_data_first_input
  */
  std::string   _time_raw_data_first_input;
  /**
  * @brief time_asc_node_state_vectors
  */
  std::string   _time_asc_node_state_vectors;
  /**
  * @brief asc_node_pos_X_comp
  */
  std::string   _asc_node_pos_X_comp;
  /**
  * @brief asc_node_pos_Y_comp
  */
  std::string   _asc_node_pos_Y_comp;
  /**
  * @brief asc_node_pos_Z_comp
  */
  std::string   _asc_node_pos_Z_comp;
  /**
  * @brief asc_node_vel_X_comp
  */
  std::string   _asc_node_vel_X_comp;
  /**
  * @brief asc_node_vel_Y_comp
  */
  std::string   _asc_node_vel_Y_comp;
  /**
  * @brief asc_node_vel_Z_comp
  */
  std::string   _asc_node_vel_Z_comp;
  /**
  * @brief out_pixel_bit_length
  */
  int   _out_pixel_bit_length;
  /**
  * @brief proc_gain_param_1
  */
  double   _proc_gain_param_1;
  /**
  * @brief proc_gain_param_2
  */
  double   _proc_gain_param_2;
  /**
  * @brief proc_gain_param_3
  */
  double   _proc_gain_param_3;
  /**
  * @brief peak_loc_cross_correl_fun
  */
  int   _peak_loc_cross_correl_fun;
  /**
  * @brief 3_dB_width_CCF
  */
  double   _3_dB_width_CCF;
  /**
  * @brief first_side_lobe_level
  */
  double   _first_side_lobe_level;
  /**
  * @brief ISLR_CCF_between_last
  */
  double   _ISLR_CCF_between_last;
  /**
  * @brief peak_loc_CCF_betw_last
  */
  int   _peak_loc_CCF_betw_last;
  /**
  * @brief Roll_Tilt_Mode_flag
  */
  int   _Roll_Tilt_Mode_flag;
  /**
  * @brief raw_data_correction_flag
  */
  int   _raw_data_correction_flag;
  /**
  * @brief look_detecion_flag
  */
  int   _look_detecion_flag;
  /**
  * @brief doppler_ambiguity_estimat_flag
  */
  int   _doppler_ambiguity_estimat_flag;
  /**
  * @brief azimuth_baseband_convers_flag
  */
  int   _azimuth_baseband_convers_flag;
  /**
  * @brief samples_per_line_used
  */
  int   _samples_per_line_used;
  /**
  * @brief range_lines_skip_factor
  */
  int   _range_lines_skip_factor;
  /**
  * @brief time_of_inp_state_vectors
  */
  std::string   _time_of_inp_state_vectors;
  /**
  * @brief inp_state_vect_pos_X_comp
  */
  std::string   _inp_state_vect_pos_X_comp;
  /**
  * @brief inp_state_vect_pos_Y_comp
  */
  std::string   _inp_state_vect_pos_Y_comp;
  /**
  * @brief inp_state_vect_pos_Z_comp
  */
  std::string   _inp_state_vect_pos_Z_comp;
  /**
  * @brief inp_state_vect_vel_Vx_comp
  */
  std::string   _inp_state_vect_vel_Vx_comp;
  /**
  * @brief inp_state_vect_vel_Vy_comp
  */
  std::string   _inp_state_vect_vel_Vy_comp;
  /**
  * @brief inp_state_vect_vel_Vz_comp
  */
  std::string   _inp_state_vect_vel_Vz_comp;
  /**
  * @brief inp_state_vector_type_flag
  */
  int   _inp_state_vector_type_flag;
  /**
  * @brief win_coeff_for_range_match
  */
  double   _win_coeff_for_range_match;
  /**
  * @brief win_coeff_for_azi_match
  */
  double   _win_coeff_for_azi_match;
  /**
  * @brief update_period_range_match
  */
  int   _update_period_range_match;
  /**
  * @brief look_scalar_gain_1
  */
  double   _look_scalar_gain_1;
  /**
  * @brief look_scalar_gain_2
  */
  double   _look_scalar_gain_2;
  /**
  * @brief look_scalar_gain_3
  */
  double   _look_scalar_gain_3;
  /**
  * @brief look_scalar_gain_4
  */
  double   _look_scalar_gain_4;
  /**
  * @brief look_scalar_gain_5
  */
  double   _look_scalar_gain_5;
  /**
  * @brief look_scalar_gain_6
  */
  double   _look_scalar_gain_6;
  /**
  * @brief look_scalar_gain_7
  */
  double   _look_scalar_gain_7;
  /**
  * @brief look_scalar_gain_8
  */
  double   _look_scalar_gain_8;
  /**
  * @brief samp_window_start_time_bias
  */
  int   _samp_window_start_time_bias;
  /**
  * @brief doppler_centroid_cubic_coeff
  */
  double   _doppler_centroid_cubic_coeff;
  /**
  * @brief PRF_code_first_range_line
  */
  int   _PRF_code_first_range_line;
  /**
  * @brief PRF_code_last_range_line
  */
  int   _PRF_code_last_range_line;
  /**
  * @brief samp_win_start_first
  */
  int   _samp_win_start_first;
  /**
  * @brief samp_win_start_last
  */
  int   _samp_win_start_last;
  /**
  * @brief cal_syst_gain_last_proc
  */
  int   _cal_syst_gain_last_proc;
  /**
  * @brief receiver_gain_last_proc
  */
  int   _receiver_gain_last_proc;
  /**
  * @brief first_processed_range_sample
  */
  int   _first_processed_range_sample;
  /**
  * @brief azimuth_FFT_IFFT_ratio
  */
  int   _azimuth_FFT_IFFT_ratio;
  /**
  * @brief num_azimuth_blocks_proc
  */
  int   _num_azimuth_blocks_proc;
  /**
  * @brief num_input_raw_data_lines
  */
  int   _num_input_raw_data_lines;
  /**
  * @brief initial_doppler_ambiguity_num
  */
  int   _initial_doppler_ambiguity_num;
  /**
  * @brief thresh_no_1_flag
  */
  double   _thresh_no_1_flag;
  /**
  * @brief thresh_no_2_flag
  */
  double   _thresh_no_2_flag;
  /**
  * @brief thresh_no_3_flag
  */
  double   _thresh_no_3_flag;
  /**
  * @brief thresh_no_4_flag
  */
  double   _thresh_no_4_flag;
  /**
  * @brief thresh_no_5_flag
  */
  double   _thresh_no_5_flag;
  /**
  * @brief thresh_no_6_flag
  */
  double   _thresh_no_6_flag;
  /**
  * @brief thresh_no_7_flag
  */
  double   _thresh_no_7_flag;
  /**
  * @brief thresh_no_8_flag
  */
  double   _thresh_no_8_flag;
  /**
  * @brief thresh_no_9_flag
  */
  double   _thresh_no_9_flag;
  /**
  * @brief thresh_no_10_flag
  */
  double   _thresh_no_10_flag;
  /**
  * @brief thresh_no_11_flag
  */
  double   _thresh_no_11_flag;
  /**
  * @brief sat_binary_time_of_first
  */
  int   _sat_binary_time_of_first;
  /**
  * @brief num_valid_pixels_per_range
  */
  int   _num_valid_pixels_per_range;
  /**
  * @brief num_range_samp_discarded
  */
  int   _num_range_samp_discarded;
  /**
  * @brief I_gain_imb_lower_bound
  */
  double   _I_gain_imb_lower_bound;
  /**
  * @brief I_gain_imb_upper_bound
  */
  double   _I_gain_imb_upper_bound;
  /**
  * @brief I_Q_quad_depar_lower_bound
  */
  double   _I_Q_quad_depar_lower_bound;
  /**
  * @brief I_Q_quad_depar_upper_bound
  */
  double   _I_Q_quad_depar_upper_bound;
  /**
  * @brief 3_dB_look_bandwidth
  */
  double   _3_dB_look_bandwidth;
  /**
  * @brief 3_dB_look_proc_dopp_bandw
  */
  double   _3_dB_look_proc_dopp_bandw;
  /**
  * @brief range_spread_loss_comp_flag
  */
  int   _range_spread_loss_comp_flag;
  /**
  * @brief datation_flag
  */
  bool   _datation_flag;
  /**
  * @brief max_error_range_line_timing
  */
  int   _max_error_range_line_timing;
  /**
  * @brief form_num_range_line_used
  */
  int   _form_num_range_line_used;
  /**
  * @brief autom_look_scal_gain_flag
  */
  bool   _autom_look_scal_gain_flag;
  /**
  * @brief max_value_look_scalar_gain
  */
  int   _max_value_look_scalar_gain;
  /**
  * @brief replica_norm_method_flag
  */
  int   _replica_norm_method_flag;
  /**
  * @brief coef_ground_range_1
  */
  double   _coef_ground_range_1;
  /**
  * @brief coef_ground_range_2
  */
  double   _coef_ground_range_2;
  /**
  * @brief coef_ground_range_3
  */
  double   _coef_ground_range_3;
  /**
  * @brief coef_ground_range_4
  */
  double   _coef_ground_range_4;
  /**
  * @brief coef_ant_elev_1
  */
  double   _coef_ant_elev_1;
  /**
  * @brief coef_ant_elev_2
  */
  double   _coef_ant_elev_2;
  /**
  * @brief coef_ant_elev_3
  */
  double   _coef_ant_elev_3;
  /**
  * @brief coef_ant_elev_4
  */
  double   _coef_ant_elev_4;
  /**
  * @brief coef_ant_elev_5
  */
  double   _coef_ant_elev_5;
  /**
  * @brief range_time_origin_ant
  */
  double   _range_time_origin_ant;

private:

};
}

#endif
