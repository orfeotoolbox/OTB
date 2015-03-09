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

#include "erssar/ErsSarFacilityData.h"

namespace ossimplugins
{

   ErsSarFacilityData::ErsSarFacilityData() : ErsSarRecord("facility_data_rec")
   {
   }

   ErsSarFacilityData::~ErsSarFacilityData()
   {
   }

   std::ostream& operator<<(std::ostream& os, const ErsSarFacilityData& data)
   {
      os << "last_release_qc_date:" << data._last_release_qc_date.c_str() << std::endl;

      os << "last_release_cal_date:" << data._last_release_cal_date.c_str() << std::endl;

      os << "qa_summary_flag:" << data._qa_summary_flag << std::endl;

      os << "prf_code_change_flag:" << data._prf_code_change_flag << std::endl;

      os << "sampling_win_change_flag:" << data._sampling_win_change_flag << std::endl;

      os << "cal_gain_change_flag:" << data._cal_gain_change_flag << std::endl;

      os << "quirp_qu_flag:" << data._quirp_qu_flag << std::endl;

      os << "inp_data_stat_flag:" << data._inp_data_stat_flag << std::endl;

      os << "dopp_cent_conf_meas_flag:" << data._dopp_cent_conf_meas_flag << std::endl;

      os << "dopp_cent_val_flag:" << data._dopp_cent_val_flag << std::endl;

      os << "dopp_ambig_conf_meas_flag:" << data._dopp_ambig_conf_meas_flag << std::endl;

      os << "outp_data_mean_flag:" << data._outp_data_mean_flag << std::endl;

      os << "OGOB_flag:" << data._OGOB_flag << std::endl;

      os << "PRF_changes:" << data._PRF_changes << std::endl;

      os << "sampling_win_changes:" << data._sampling_win_changes << std::endl;

      os << "cal_gain_changes:" << data._cal_gain_changes << std::endl;

      os << "missing_lines:" << data._missing_lines << std::endl;

      os << "rec_gain_changes:" << data._rec_gain_changes << std::endl;

      os << "pulse_width_of_ACF_3db:" << data._pulse_width_of_ACF_3db << std::endl;

      os << "first_side_lobe_lev_of_ACF:" << data._first_side_lobe_lev_of_ACF << std::endl;

      os << "ISLR_of_ACF:" << data._ISLR_of_ACF << std::endl;

      os << "dopp_cent_conf_meas:" << data._dopp_cent_conf_meas << std::endl;

      os << "dopp_ambig_conf_meas:" << data._dopp_ambig_conf_meas << std::endl;

      os << "inp_data_I_mean:" << data._inp_data_I_mean << std::endl;

      os << "inp_data_Q_mean:" << data._inp_data_Q_mean << std::endl;

      os << "inp_data_I_stddev:" << data._inp_data_I_stddev << std::endl;

      os << "inp_data_Q_stddev:" << data._inp_data_Q_stddev << std::endl;

      os << "cal_sys_gain:" << data._cal_sys_gain << std::endl;

      os << "first_rec_gain_read:" << data._first_rec_gain_read << std::endl;

      os << "dopp_ambig_num:" << data._dopp_ambig_num << std::endl;

      os << "I_channel_bias_correction:" << data._I_channel_bias_correction << std::endl;

      os << "Q_channel_bias_correction:" << data._Q_channel_bias_correction << std::endl;

      os << "I_channel_gain_correction:" << data._I_channel_gain_correction << std::endl;

      os << "Q_channel_gain_correction:" << data._Q_channel_gain_correction << std::endl;

      os << "Q_channel_I_Q_correction:" << data._Q_channel_I_Q_correction << std::endl;

      os << "noise_power:" << data._noise_power << std::endl;

      os << "int_cal_utc:" << data._int_cal_utc << std::endl;

      os << "num_valid_cal_pulses:" << data._num_valid_cal_pulses << std::endl;

      os << "num_valid_noise_pulses:" << data._num_valid_noise_pulses << std::endl;

      os << "num_valid_replicas:" << data._num_valid_replicas << std::endl;

      os << "first_replica_sample:" << data._first_replica_sample << std::endl;

      os << "mean_cal_pulse_power:" << data._mean_cal_pulse_power << std::endl;

      os << "mean_noise_power:" << data._mean_noise_power << std::endl;

      os << "range_comp_norm_fact:" << data._range_comp_norm_fact << std::endl;

      os << "replica_power:" << data._replica_power << std::endl;

      os << "first_range_pixel_mid_az_inc:" << data._first_range_pixel_mid_az_inc << std::endl;

      os << "center_range_pix_mid_az_inc:" << data._center_range_pix_mid_az_inc << std::endl;

      os << "last_range_pix_mid_az_inc:" << data._last_range_pix_mid_az_inc << std::endl;

      os << "norm_ref_range_ro:" << data._norm_ref_range_ro << std::endl;

      os << "antenna_elev_flag:" << data._antenna_elev_flag << std::endl;

      os << "abs_cal_const_K:" << data._abs_cal_const_K << std::endl;

      os << "upp_bound_K:" << data._upp_bound_K << std::endl;

      os << "low_bound_K:" << data._low_bound_K << std::endl;

      os << "proc_noise_scale_fact:" << data._proc_noise_scale_fact << std::endl;

      os << "K_gen_date:" << data._K_gen_date.c_str() << std::endl;

      os << "K_vers_num:" << data._K_vers_num.c_str() << std::endl;

      os << "num_duplic_input_lines:" << data._num_duplic_input_lines << std::endl;

      os << "estim_bit_error_rate:" << data._estim_bit_error_rate << std::endl;

      os << "out_image_mean:" << data._out_image_mean << std::endl;

      os << "out_image_std_dev:" << data._out_image_std_dev << std::endl;

      os << "out_image_max_value:" << data._out_image_max_value << std::endl;

      os << "time_raw_data_first_input:" << data._time_raw_data_first_input.c_str() << std::endl;

      os << "time_asc_node_state_vectors:" << data._time_asc_node_state_vectors.c_str() << std::endl;

      os << "asc_node_pos_X_comp:" << data._asc_node_pos_X_comp.c_str() << std::endl;

      os << "asc_node_pos_Y_comp:" << data._asc_node_pos_Y_comp.c_str() << std::endl;

      os << "asc_node_pos_Z_comp:" << data._asc_node_pos_Z_comp.c_str() << std::endl;

      os << "asc_node_vel_X_comp:" << data._asc_node_vel_X_comp.c_str() << std::endl;

      os << "asc_node_vel_Y_comp:" << data._asc_node_vel_Y_comp.c_str() << std::endl;

      os << "asc_node_vel_Z_comp:" << data._asc_node_vel_Z_comp.c_str() << std::endl;

      os << "out_pixel_bit_length:" << data._out_pixel_bit_length << std::endl;

      os << "proc_gain_param_1:" << data._proc_gain_param_1 << std::endl;

      os << "proc_gain_param_2:" << data._proc_gain_param_2 << std::endl;

      os << "proc_gain_param_3:" << data._proc_gain_param_3 << std::endl;

      os << "peak_loc_cross_correl_fun:" << data._peak_loc_cross_correl_fun << std::endl;

      os << "3_dB_width_CCF:" << data._3_dB_width_CCF << std::endl;

      os << "first_side_lobe_level:" << data._first_side_lobe_level << std::endl;

      os << "ISLR_CCF_between_last:" << data._ISLR_CCF_between_last << std::endl;

      os << "peak_loc_CCF_betw_last:" << data._peak_loc_CCF_betw_last << std::endl;

      os << "Roll_Tilt_Mode_flag:" << data._Roll_Tilt_Mode_flag << std::endl;

      os << "raw_data_correction_flag:" << data._raw_data_correction_flag << std::endl;

      os << "look_detecion_flag:" << data._look_detecion_flag << std::endl;

      os << "doppler_ambiguity_estimat_flag:" << data._doppler_ambiguity_estimat_flag << std::endl;

      os << "azimuth_baseband_convers_flag:" << data._azimuth_baseband_convers_flag << std::endl;

      os << "samples_per_line_used:" << data._samples_per_line_used << std::endl;

      os << "range_lines_skip_factor:" << data._range_lines_skip_factor << std::endl;

      os << "time_of_inp_state_vectors:" << data._time_of_inp_state_vectors.c_str() << std::endl;

      os << "inp_state_vect_pos_X_comp:" << data._inp_state_vect_pos_X_comp.c_str() << std::endl;

      os << "inp_state_vect_pos_Y_comp:" << data._inp_state_vect_pos_Y_comp.c_str() << std::endl;

      os << "inp_state_vect_pos_Z_comp:" << data._inp_state_vect_pos_Z_comp.c_str() << std::endl;

      os << "inp_state_vect_vel_Vx_comp:" << data._inp_state_vect_vel_Vx_comp.c_str() << std::endl;

      os << "inp_state_vect_vel_Vy_comp:" << data._inp_state_vect_vel_Vy_comp.c_str() << std::endl;

      os << "inp_state_vect_vel_Vz_comp:" << data._inp_state_vect_vel_Vz_comp.c_str() << std::endl;

      os << "inp_state_vector_type_flag:" << data._inp_state_vector_type_flag << std::endl;

      os << "win_coeff_for_range_match:" << data._win_coeff_for_range_match << std::endl;

      os << "win_coeff_for_azi_match:" << data._win_coeff_for_azi_match << std::endl;

      os << "update_period_range_match:" << data._update_period_range_match << std::endl;

      os << "look_scalar_gain_1:" << data._look_scalar_gain_1 << std::endl;

      os << "look_scalar_gain_2:" << data._look_scalar_gain_2 << std::endl;

      os << "look_scalar_gain_3:" << data._look_scalar_gain_3 << std::endl;

      os << "look_scalar_gain_4:" << data._look_scalar_gain_4 << std::endl;

      os << "look_scalar_gain_5:" << data._look_scalar_gain_5 << std::endl;

      os << "look_scalar_gain_6:" << data._look_scalar_gain_6 << std::endl;

      os << "look_scalar_gain_7:" << data._look_scalar_gain_7 << std::endl;

      os << "look_scalar_gain_8:" << data._look_scalar_gain_8 << std::endl;

      os << "samp_window_start_time_bias:" << data._samp_window_start_time_bias << std::endl;

      os << "doppler_centroid_cubic_coeff:" << data._doppler_centroid_cubic_coeff << std::endl;

      os << "PRF_code_first_range_line:" << data._PRF_code_first_range_line << std::endl;

      os << "PRF_code_last_range_line:" << data._PRF_code_last_range_line << std::endl;

      os << "samp_win_start_first:" << data._samp_win_start_first << std::endl;

      os << "samp_win_start_last:" << data._samp_win_start_last << std::endl;

      os << "cal_syst_gain_last_proc:" << data._cal_syst_gain_last_proc << std::endl;

      os << "receiver_gain_last_proc:" << data._receiver_gain_last_proc << std::endl;

      os << "first_processed_range_sample:" << data._first_processed_range_sample << std::endl;

      os << "azimuth_FFT_IFFT_ratio:" << data._azimuth_FFT_IFFT_ratio << std::endl;

      os << "num_azimuth_blocks_proc:" << data._num_azimuth_blocks_proc << std::endl;

      os << "num_input_raw_data_lines:" << data._num_input_raw_data_lines << std::endl;

      os << "initial_doppler_ambiguity_num:" << data._initial_doppler_ambiguity_num << std::endl;

      os << "thresh_no_1_flag:" << data._thresh_no_1_flag << std::endl;

      os << "thresh_no_2_flag:" << data._thresh_no_2_flag << std::endl;

      os << "thresh_no_3_flag:" << data._thresh_no_3_flag << std::endl;

      os << "thresh_no_4_flag:" << data._thresh_no_4_flag << std::endl;

      os << "thresh_no_5_flag:" << data._thresh_no_5_flag << std::endl;

      os << "thresh_no_6_flag:" << data._thresh_no_6_flag << std::endl;

      os << "thresh_no_7_flag:" << data._thresh_no_7_flag << std::endl;

      os << "thresh_no_8_flag:" << data._thresh_no_8_flag << std::endl;

      os << "thresh_no_9_flag:" << data._thresh_no_9_flag << std::endl;

      os << "thresh_no_10_flag:" << data._thresh_no_10_flag << std::endl;

      os << "thresh_no_11_flag:" << data._thresh_no_11_flag << std::endl;

      os << "sat_binary_time_of_first:" << data._sat_binary_time_of_first << std::endl;

      os << "num_valid_pixels_per_range:" << data._num_valid_pixels_per_range << std::endl;

      os << "num_range_samp_discarded:" << data._num_range_samp_discarded << std::endl;

      os << "I_gain_imb_lower_bound:" << data._I_gain_imb_lower_bound << std::endl;

      os << "I_gain_imb_upper_bound:" << data._I_gain_imb_upper_bound << std::endl;

      os << "I_Q_quad_depar_lower_bound:" << data._I_Q_quad_depar_lower_bound << std::endl;

      os << "I_Q_quad_depar_upper_bound:" << data._I_Q_quad_depar_upper_bound << std::endl;

      os << "3_dB_look_bandwidth:" << data._3_dB_look_bandwidth << std::endl;

      os << "3_dB_look_proc_dopp_bandw:" << data._3_dB_look_proc_dopp_bandw << std::endl;

      os << "range_spread_loss_comp_flag:" << data._range_spread_loss_comp_flag << std::endl;

      os << "datation_flag:" << data._datation_flag << std::endl;

      os << "max_error_range_line_timing:" << data._max_error_range_line_timing << std::endl;

      os << "form_num_range_line_used:" << data._form_num_range_line_used << std::endl;

      os << "autom_look_scal_gain_flag:" << data._autom_look_scal_gain_flag << std::endl;

      os << "max_value_look_scalar_gain:" << data._max_value_look_scalar_gain << std::endl;

      os << "replica_norm_method_flag:" << data._replica_norm_method_flag << std::endl;

      os << "coef_ground_range_1:" << data._coef_ground_range_1 << std::endl;

      os << "coef_ground_range_2:" << data._coef_ground_range_2 << std::endl;

      os << "coef_ground_range_3:" << data._coef_ground_range_3 << std::endl;

      os << "coef_ground_range_4:" << data._coef_ground_range_4 << std::endl;

      os << "coef_ant_elev_1:" << data._coef_ant_elev_1 << std::endl;

      os << "coef_ant_elev_2:" << data._coef_ant_elev_2 << std::endl;

      os << "coef_ant_elev_3:" << data._coef_ant_elev_3 << std::endl;

      os << "coef_ant_elev_4:" << data._coef_ant_elev_4 << std::endl;

      os << "coef_ant_elev_5:" << data._coef_ant_elev_5 << std::endl;

      os << "range_time_origin_ant:" << data._range_time_origin_ant << std::endl;

      return os;

   }

   std::istream& operator>>(std::istream& is, ErsSarFacilityData& data)
   {
      char buf64[65];
      buf64[64] = '\0';
      // char buf32[33];
      // buf32[32] = '\0';
      char buf24[25];
      buf24[24] = '\0';
      char buf22[23];
      buf22[22] = '\0';
      char buf20[21];
      buf20[20] = '\0';
      char buf16[17];
      buf16[16] = '\0';
      char buf12[13];
      buf12[12] = '\0';
      char buf7[8];
      buf7[7] = '\0';
      char buf8[9];
      buf8[8] = '\0';
      char buf6[7];
      buf6[6] = '\0';
      char buf4[5];
      buf4[4] = '\0';
      char buf2[3];
      buf2[2] = '\0';
      char buf1[2];
      buf1[1] = '\0';

      is.read(buf64, 64);
      data._name_of_facil_rec = buf64;

      is.read(buf6, 6);
      data._last_release_qc_date = buf6;

      is.read(buf2, 2);

      is.read(buf6, 6);
      data._last_release_cal_date = buf6;

      is.read(buf4, 4);
      data._qa_summary_flag = atoi(buf4);

      is.read(buf4, 4);
      data._prf_code_change_flag = atoi(buf4);

      is.read(buf4, 4);
      data._sampling_win_change_flag = atoi(buf4);

      is.read(buf4, 4);
      data._cal_gain_change_flag = atoi(buf4);

      is.read(buf4, 4);
      data._quirp_qu_flag = atoi(buf4);

      is.read(buf4, 4);
      data._inp_data_stat_flag = atoi(buf4);

      is.read(buf4, 4);
      data._dopp_cent_conf_meas_flag = atoi(buf4);

      is.read(buf4, 4);
      data._dopp_cent_val_flag = atoi(buf4);

      is.read(buf4, 4);
      data._dopp_ambig_conf_meas_flag = atoi(buf4);

      is.read(buf4, 4);
      data._outp_data_mean_flag = atoi(buf4);

      is.read(buf4, 4);
      data._OGOB_flag = atoi(buf4);

      is.read(buf4, 4);
      data._PRF_changes = atoi(buf4);

      is.read(buf4, 4);
      data._sampling_win_changes = atoi(buf4);

      is.read(buf4, 4);
      data._cal_gain_changes = atoi(buf4);

      is.read(buf4, 4);
      data._missing_lines = atoi(buf4);

      is.read(buf4, 4);
      data._rec_gain_changes = atoi(buf4);

      is.read(buf16, 16);
      data._pulse_width_of_ACF_3db = atof(buf16);

      is.read(buf16, 16);
      data._first_side_lobe_lev_of_ACF = atof(buf16);

      is.read(buf16, 16);
      data._ISLR_of_ACF = atof(buf16);

      is.read(buf16, 16);
      data._dopp_cent_conf_meas = atof(buf16);

      is.read(buf16, 16);
      data._dopp_ambig_conf_meas = atof(buf16);

      is.read(buf16, 16);
      data._inp_data_I_mean = atof(buf16);

      is.read(buf16, 16);
      data._inp_data_Q_mean = atof(buf16);

      is.read(buf16, 16);
      data._inp_data_I_stddev = atof(buf16);

      is.read(buf16, 16);
      data._inp_data_Q_stddev = atof(buf16);

      is.read(buf16, 16);
      data._cal_sys_gain = atof(buf16);

      is.read(buf16, 16);
      data._first_rec_gain_read = atof(buf16);

      is.read(buf16, 16);
      data._dopp_ambig_num = atof(buf16);

      is.read(buf16, 16);

      is.read(buf16, 16);
      data._I_channel_bias_correction = atof(buf16);

      is.read(buf16, 16);
      data._Q_channel_bias_correction = atof(buf16);

      is.read(buf16, 16);
      data._I_channel_gain_correction = atof(buf16);

      is.read(buf16, 16);
      data._Q_channel_gain_correction = atof(buf16);

      is.read(buf16, 16);
      data._Q_channel_I_Q_correction = atof(buf16);

      is.read(buf16, 16);

      is.read(buf16, 16);
      data._noise_power = atof(buf16);

      is.read(buf16, 16);
      data._int_cal_utc = atoi(buf16);

      is.read(buf4, 4);
      data._num_valid_cal_pulses = atoi(buf4);

      is.read(buf4, 4);
      data._num_valid_noise_pulses = atoi(buf4);

      is.read(buf4, 4);
      data._num_valid_replicas = atoi(buf4);

      is.read(buf16, 16);
      data._first_replica_sample = atof(buf16);

      is.read(buf16, 16);
      data._mean_cal_pulse_power = atof(buf16);

      is.read(buf16, 16);
      data._mean_noise_power = atof(buf16);

      is.read(buf16, 16);
      data._range_comp_norm_fact = atof(buf16);

      is.read(buf16, 16);
      data._replica_power = atof(buf16);

      is.read(buf16, 16);
      data._first_range_pixel_mid_az_inc = atof(buf16);

      is.read(buf16, 16);
      data._center_range_pix_mid_az_inc = atof(buf16);

      is.read(buf16, 16);
      data._last_range_pix_mid_az_inc = atof(buf16);

      is.read(buf16, 16);
      data._norm_ref_range_ro = atof(buf16);

      is.read(buf12, 12);

      is.read(buf4, 4);
      data._antenna_elev_flag = atoi(buf4);

      is.read(buf16, 16);
      data._abs_cal_const_K = atof(buf16);

      is.read(buf16, 16);
      data._upp_bound_K = atof(buf16);

      is.read(buf16, 16);
      data._low_bound_K = atof(buf16);

      is.read(buf16, 16);
      data._proc_noise_scale_fact = atof(buf16);

      is.read(buf6, 6);
      data._K_gen_date = buf6;

      is.read(buf4, 4);
      data._K_vers_num = buf4;

      is.read(buf4, 4);
      data._num_duplic_input_lines = atoi(buf4);

      is.read(buf16, 16);
      data._estim_bit_error_rate = atof(buf16);

      is.read(buf12, 12);

      is.read(buf16, 16);
      data._out_image_mean = atof(buf16);

      is.read(buf16, 16);
      data._out_image_std_dev = atof(buf16);

      is.read(buf16, 16);
      data._out_image_max_value = atof(buf16);

      is.read(buf24, 24);
      data._time_raw_data_first_input = buf24;

      is.read(buf24, 24);
      data._time_asc_node_state_vectors = buf24;

      is.read(buf22, 22);
      data._asc_node_pos_X_comp = buf22;

      is.read(buf22, 22);
      data._asc_node_pos_Y_comp = buf22;

      is.read(buf22, 22);
      data._asc_node_pos_Z_comp = buf22;

      is.read(buf22, 22);
      data._asc_node_vel_X_comp = buf22;

      is.read(buf22, 22);
      data._asc_node_vel_Y_comp = buf22;

      is.read(buf22, 22);
      data._asc_node_vel_Z_comp = buf22;

      is.read(buf4, 4);
      data._out_pixel_bit_length = atoi(buf4);

      is.read(buf16, 16);
      data._proc_gain_param_1 = atof(buf16);

      is.read(buf16, 16);
      data._proc_gain_param_2 = atof(buf16);

      is.read(buf16, 16);
      data._proc_gain_param_3 = atof(buf16);

      is.read(buf4, 4);
      data._peak_loc_cross_correl_fun = atoi(buf4);

      is.read(buf16, 16);
      data._3_dB_width_CCF = atof(buf16);

      is.read(buf16, 16);
      data._first_side_lobe_level = atof(buf16);

      is.read(buf16, 16);
      data._ISLR_CCF_between_last = atof(buf16);

      is.read(buf4, 4);
      data._peak_loc_CCF_betw_last = atoi(buf4);

      is.read(buf4, 4);
      data._Roll_Tilt_Mode_flag = atoi(buf4);

      is.read(buf4, 4);
      data._raw_data_correction_flag = atoi(buf4);

      is.read(buf4, 4);
      data._look_detecion_flag = atoi(buf4);

      is.read(buf4, 4);
      data._doppler_ambiguity_estimat_flag = atoi(buf4);

      is.read(buf4, 4);
      data._azimuth_baseband_convers_flag = atoi(buf4);

      is.read(buf4, 4);
      data._samples_per_line_used = atoi(buf4);

      is.read(buf4, 4);
      data._range_lines_skip_factor = atoi(buf4);

      is.read(buf24, 24);
      data._time_of_inp_state_vectors = buf24;

      is.read(buf22, 22);
      data._inp_state_vect_pos_X_comp = buf22;

      is.read(buf22, 22);
      data._inp_state_vect_pos_Y_comp = buf22;

      is.read(buf22, 22);
      data._inp_state_vect_pos_Z_comp = buf22;

      is.read(buf22, 22);
      data._inp_state_vect_vel_Vx_comp = buf22;

      is.read(buf22, 22);
      data._inp_state_vect_vel_Vy_comp = buf22;

      is.read(buf22, 22);
      data._inp_state_vect_vel_Vz_comp = buf22;

      is.read(buf4, 4);
      data._inp_state_vector_type_flag = atoi(buf4);

      is.read(buf16, 16);
      data._win_coeff_for_range_match = atof(buf16);

      is.read(buf16, 16);
      data._win_coeff_for_azi_match = atof(buf16);

      is.read(buf4, 4);
      data._update_period_range_match = atoi(buf4);

      is.read(buf16, 16);
      data._look_scalar_gain_1 = atof(buf16);

      is.read(buf16, 16);
      data._look_scalar_gain_2 = atof(buf16);

      is.read(buf16, 16);
      data._look_scalar_gain_3 = atof(buf16);

      is.read(buf16, 16);
      data._look_scalar_gain_4 = atof(buf16);

      is.read(buf16, 16);
      data._look_scalar_gain_5 = atof(buf16);

      is.read(buf16, 16);
      data._look_scalar_gain_6 = atof(buf16);

      is.read(buf16, 16);
      data._look_scalar_gain_7 = atof(buf16);

      is.read(buf16, 16);
      data._look_scalar_gain_8 = atof(buf16);

      is.read(buf4, 4);
      data._samp_window_start_time_bias = atoi(buf4);

      is.read(buf22, 22);
      data._doppler_centroid_cubic_coeff = atof(buf22);

      is.read(buf4, 4);
      data._PRF_code_first_range_line = atoi(buf4);

      is.read(buf4, 4);
      data._PRF_code_last_range_line = atoi(buf4);

      is.read(buf4, 4);
      data._samp_win_start_first = atoi(buf4);

      is.read(buf4, 4);
      data._samp_win_start_last = atoi(buf4);

      is.read(buf4, 4);
      data._cal_syst_gain_last_proc = atoi(buf4);

      is.read(buf4, 4);
      data._receiver_gain_last_proc = atoi(buf4);

      is.read(buf4, 4);
      data._first_processed_range_sample = atoi(buf4);

      is.read(buf4, 4);
      data._azimuth_FFT_IFFT_ratio = atoi(buf4);

      is.read(buf4, 4);
      data._num_azimuth_blocks_proc = atoi(buf4);

      is.read(buf8, 8);
      data._num_input_raw_data_lines = atol(buf8);

      is.read(buf4, 4);
      data._initial_doppler_ambiguity_num = atoi(buf4);

      is.read(buf16, 16);
      data._thresh_no_1_flag = atof(buf16);

      is.read(buf16, 16);
      data._thresh_no_2_flag = atof(buf16);

      is.read(buf16, 16);
      data._thresh_no_3_flag = atof(buf16);

      is.read(buf16, 16);
      data._thresh_no_4_flag = atof(buf16);

      is.read(buf16, 16);
      data._thresh_no_5_flag = atof(buf16);

      is.read(buf16, 16);
      data._thresh_no_6_flag = atof(buf16);

      is.read(buf16, 16);
      data._thresh_no_7_flag = atof(buf16);

      is.read(buf16, 16);
      data._thresh_no_8_flag = atof(buf16);

      is.read(buf16, 16);
      data._thresh_no_9_flag = atof(buf16);

      is.read(buf16, 16);
      data._thresh_no_10_flag = atof(buf16);

      is.read(buf16, 16);
      data._thresh_no_11_flag = atof(buf16);

      is.read(buf16, 16);
      data._sat_binary_time_of_first = atol(buf16);

      is.read(buf4, 4);
      data._num_valid_pixels_per_range = atoi(buf4);

      is.read(buf4, 4);
      data._num_range_samp_discarded = atoi(buf4);

      is.read(buf16, 16);
      data._I_gain_imb_lower_bound = atof(buf16);

      is.read(buf16, 16);
      data._I_gain_imb_upper_bound = atof(buf16);

      is.read(buf16, 16);
      data._I_Q_quad_depar_lower_bound = atof(buf16);

      is.read(buf16, 16);
      data._I_Q_quad_depar_upper_bound = atof(buf16);

      is.read(buf16, 16);
      data._3_dB_look_bandwidth = atof(buf16);

      is.read(buf16, 16);
      data._3_dB_look_proc_dopp_bandw = atof(buf16);

      is.read(buf4, 4);
      data._range_spread_loss_comp_flag = atoi(buf4);

      is.read(buf1, 1);
      data._datation_flag = (atoi(buf1) == 0) ? false : true;

      is.read(buf7, 7);
      data._max_error_range_line_timing = atoi(buf7);

      is.read(buf7, 7);
      data._form_num_range_line_used = atoi(buf7);

      is.read(buf1, 1);
      data._autom_look_scal_gain_flag = (atoi(buf1) == 0) ? false : true;

      is.read(buf4, 4);
      data._max_value_look_scalar_gain = atoi(buf4);

      is.read(buf4, 4);
      data._replica_norm_method_flag = atoi(buf4);

      is.read(buf20, 20);
      data._coef_ground_range_1 = atof(buf20);

      is.read(buf20, 20);
      data._coef_ground_range_2 = atof(buf20);

      is.read(buf20, 20);
      data._coef_ground_range_3 = atof(buf20);

      is.read(buf20, 20);
      data._coef_ground_range_4 = atof(buf20);

      is.read(buf20, 20);
      data._coef_ant_elev_1 = atof(buf20);

      is.read(buf20, 20);
      data._coef_ant_elev_2 = atof(buf20);

      is.read(buf20, 20);
      data._coef_ant_elev_3 = atof(buf20);

      is.read(buf20, 20);
      data._coef_ant_elev_4 = atof(buf20);

      is.read(buf20, 20);
      data._coef_ant_elev_5 = atof(buf20);

      is.read(buf16, 16);
      data._range_time_origin_ant = atof(buf16);

      char buf10238[10238];
      is.read(buf10238, 10238);
      return is;
   }


   ErsSarFacilityData::ErsSarFacilityData(const ErsSarFacilityData& rhs):
      ErsSarRecord(rhs),
      _last_release_qc_date(rhs._last_release_qc_date),
      _last_release_cal_date(rhs._last_release_cal_date),
      _qa_summary_flag(rhs._qa_summary_flag),
      _prf_code_change_flag(rhs._prf_code_change_flag),
      _sampling_win_change_flag(rhs._sampling_win_change_flag),
      _cal_gain_change_flag(rhs._cal_gain_change_flag),
      _quirp_qu_flag(rhs._quirp_qu_flag),
      _inp_data_stat_flag(rhs._inp_data_stat_flag),
      _dopp_cent_conf_meas_flag(rhs._dopp_cent_conf_meas_flag),
      _dopp_cent_val_flag(rhs._dopp_cent_val_flag),
      _dopp_ambig_conf_meas_flag(rhs._dopp_ambig_conf_meas_flag),
      _outp_data_mean_flag(rhs._outp_data_mean_flag),
      _OGOB_flag(rhs._OGOB_flag),
      _PRF_changes(rhs._PRF_changes),
      _sampling_win_changes(rhs._sampling_win_changes),
      _cal_gain_changes(rhs._cal_gain_changes),
      _missing_lines(rhs._missing_lines),
      _rec_gain_changes(rhs._rec_gain_changes),
      _pulse_width_of_ACF_3db(rhs._pulse_width_of_ACF_3db),
      _first_side_lobe_lev_of_ACF(rhs._first_side_lobe_lev_of_ACF),
      _ISLR_of_ACF(rhs._ISLR_of_ACF),
      _dopp_cent_conf_meas(rhs._dopp_cent_conf_meas),
      _dopp_ambig_conf_meas(rhs._dopp_ambig_conf_meas),
      _inp_data_I_mean(rhs._inp_data_I_mean),
      _inp_data_Q_mean(rhs._inp_data_Q_mean),
      _inp_data_I_stddev(rhs._inp_data_I_stddev),
      _inp_data_Q_stddev(rhs._inp_data_Q_stddev),
      _cal_sys_gain(rhs._cal_sys_gain),
      _first_rec_gain_read(rhs._first_rec_gain_read),
      _dopp_ambig_num(rhs._dopp_ambig_num),
      _I_channel_bias_correction(rhs._I_channel_bias_correction),
      _Q_channel_bias_correction(rhs._Q_channel_bias_correction),
      _I_channel_gain_correction(rhs._I_channel_gain_correction),
      _Q_channel_gain_correction(rhs._Q_channel_gain_correction),
      _Q_channel_I_Q_correction(rhs._Q_channel_I_Q_correction),
      _noise_power(rhs._noise_power),
      _int_cal_utc(rhs._int_cal_utc),
      _num_valid_cal_pulses(rhs._num_valid_cal_pulses),
      _num_valid_noise_pulses(rhs._num_valid_noise_pulses),
      _num_valid_replicas(rhs._num_valid_replicas),
      _first_replica_sample(rhs._first_replica_sample),
      _mean_cal_pulse_power(rhs._mean_cal_pulse_power),
      _mean_noise_power(rhs._mean_noise_power),
      _range_comp_norm_fact(rhs._range_comp_norm_fact),
      _replica_power(rhs._replica_power),
      _first_range_pixel_mid_az_inc(rhs._first_range_pixel_mid_az_inc),
      _center_range_pix_mid_az_inc(rhs._center_range_pix_mid_az_inc),
      _last_range_pix_mid_az_inc(rhs._last_range_pix_mid_az_inc),
      _norm_ref_range_ro(rhs._norm_ref_range_ro),
      _antenna_elev_flag(rhs._antenna_elev_flag),
      _abs_cal_const_K(rhs._abs_cal_const_K),
      _upp_bound_K(rhs._upp_bound_K),
      _low_bound_K(rhs._low_bound_K),
      _proc_noise_scale_fact(rhs._proc_noise_scale_fact),
      _K_gen_date(rhs._K_gen_date),
      _K_vers_num(rhs._K_vers_num),
      _num_duplic_input_lines(rhs._num_duplic_input_lines),
      _estim_bit_error_rate(rhs._estim_bit_error_rate),
      _out_image_mean(rhs._out_image_mean),
      _out_image_std_dev(rhs._out_image_std_dev),
      _out_image_max_value(rhs._out_image_max_value),
      _time_raw_data_first_input(rhs._time_raw_data_first_input),
      _time_asc_node_state_vectors(rhs._time_asc_node_state_vectors),
      _asc_node_pos_X_comp(rhs._asc_node_pos_X_comp),
      _asc_node_pos_Y_comp(rhs._asc_node_pos_Y_comp),
      _asc_node_pos_Z_comp(rhs._asc_node_pos_Z_comp),
      _asc_node_vel_X_comp(rhs._asc_node_vel_X_comp),
      _asc_node_vel_Y_comp(rhs._asc_node_vel_Y_comp),
      _asc_node_vel_Z_comp(rhs._asc_node_vel_Z_comp),
      _out_pixel_bit_length(rhs._out_pixel_bit_length),
      _proc_gain_param_1(rhs._proc_gain_param_1),
      _proc_gain_param_2(rhs._proc_gain_param_2),
      _proc_gain_param_3(rhs._proc_gain_param_3),
      _peak_loc_cross_correl_fun(rhs._peak_loc_cross_correl_fun),
      _3_dB_width_CCF(rhs._3_dB_width_CCF),
      _first_side_lobe_level(rhs._first_side_lobe_level),
      _ISLR_CCF_between_last(rhs._ISLR_CCF_between_last),
      _peak_loc_CCF_betw_last(rhs._peak_loc_CCF_betw_last),
      _Roll_Tilt_Mode_flag(rhs._Roll_Tilt_Mode_flag),
      _raw_data_correction_flag(rhs._raw_data_correction_flag),
      _look_detecion_flag(rhs._look_detecion_flag),
      _doppler_ambiguity_estimat_flag(rhs._doppler_ambiguity_estimat_flag),
      _azimuth_baseband_convers_flag(rhs._azimuth_baseband_convers_flag),
      _samples_per_line_used(rhs._samples_per_line_used),
      _range_lines_skip_factor(rhs._range_lines_skip_factor),
      _time_of_inp_state_vectors(rhs._time_of_inp_state_vectors),
      _inp_state_vect_pos_X_comp(rhs._inp_state_vect_pos_X_comp),
      _inp_state_vect_pos_Y_comp(rhs._inp_state_vect_pos_Y_comp),
      _inp_state_vect_pos_Z_comp(rhs._inp_state_vect_pos_Z_comp),
      _inp_state_vect_vel_Vx_comp(rhs._inp_state_vect_vel_Vx_comp),
      _inp_state_vect_vel_Vy_comp(rhs._inp_state_vect_vel_Vy_comp),
      _inp_state_vect_vel_Vz_comp(rhs._inp_state_vect_vel_Vz_comp),
      _inp_state_vector_type_flag(rhs._inp_state_vector_type_flag),
      _win_coeff_for_range_match(rhs._win_coeff_for_range_match),
      _win_coeff_for_azi_match(rhs._win_coeff_for_azi_match),
      _update_period_range_match(rhs._update_period_range_match),
      _look_scalar_gain_1(rhs._look_scalar_gain_1),
      _look_scalar_gain_2(rhs._look_scalar_gain_2),
      _look_scalar_gain_3(rhs._look_scalar_gain_3),
      _look_scalar_gain_4(rhs._look_scalar_gain_4),
      _look_scalar_gain_5(rhs._look_scalar_gain_5),
      _look_scalar_gain_6(rhs._look_scalar_gain_6),
      _look_scalar_gain_7(rhs._look_scalar_gain_7),
      _look_scalar_gain_8(rhs._look_scalar_gain_8),
      _samp_window_start_time_bias(rhs._samp_window_start_time_bias),
      _doppler_centroid_cubic_coeff(rhs._doppler_centroid_cubic_coeff),
      _PRF_code_first_range_line(rhs._PRF_code_first_range_line),
      _PRF_code_last_range_line(rhs._PRF_code_last_range_line),
      _samp_win_start_first(rhs._samp_win_start_first),
      _samp_win_start_last(rhs._samp_win_start_last),
      _cal_syst_gain_last_proc(rhs._cal_syst_gain_last_proc),
      _receiver_gain_last_proc(rhs._receiver_gain_last_proc),
      _first_processed_range_sample(rhs._first_processed_range_sample),
      _azimuth_FFT_IFFT_ratio(rhs._azimuth_FFT_IFFT_ratio),
      _num_azimuth_blocks_proc(rhs._num_azimuth_blocks_proc),
      _num_input_raw_data_lines(rhs._num_input_raw_data_lines),
      _initial_doppler_ambiguity_num(rhs._initial_doppler_ambiguity_num),
      _thresh_no_1_flag(rhs._thresh_no_1_flag),
      _thresh_no_2_flag(rhs._thresh_no_2_flag),
      _thresh_no_3_flag(rhs._thresh_no_3_flag),
      _thresh_no_4_flag(rhs._thresh_no_4_flag),
      _thresh_no_5_flag(rhs._thresh_no_5_flag),
      _thresh_no_6_flag(rhs._thresh_no_6_flag),
      _thresh_no_7_flag(rhs._thresh_no_7_flag),
      _thresh_no_8_flag(rhs._thresh_no_8_flag),
      _thresh_no_9_flag(rhs._thresh_no_9_flag),
      _thresh_no_10_flag(rhs._thresh_no_10_flag),
      _thresh_no_11_flag(rhs._thresh_no_11_flag),
      _sat_binary_time_of_first(rhs._sat_binary_time_of_first),
      _num_valid_pixels_per_range(rhs._num_valid_pixels_per_range),
      _num_range_samp_discarded(rhs._num_range_samp_discarded),
      _I_gain_imb_lower_bound(rhs._I_gain_imb_lower_bound),
      _I_gain_imb_upper_bound(rhs._I_gain_imb_upper_bound),
      _I_Q_quad_depar_lower_bound(rhs._I_Q_quad_depar_lower_bound),
      _I_Q_quad_depar_upper_bound(rhs._I_Q_quad_depar_upper_bound),
      _3_dB_look_bandwidth(rhs._3_dB_look_bandwidth),
      _3_dB_look_proc_dopp_bandw(rhs._3_dB_look_proc_dopp_bandw),
      _range_spread_loss_comp_flag(rhs._range_spread_loss_comp_flag),
      _datation_flag(rhs._datation_flag),
      _max_error_range_line_timing(rhs._max_error_range_line_timing),
      _form_num_range_line_used(rhs._form_num_range_line_used),
      _autom_look_scal_gain_flag(rhs._autom_look_scal_gain_flag),
      _max_value_look_scalar_gain(rhs._max_value_look_scalar_gain),
      _replica_norm_method_flag(rhs._replica_norm_method_flag),
      _coef_ground_range_1(rhs._coef_ground_range_1),
      _coef_ground_range_2(rhs._coef_ground_range_2),
      _coef_ground_range_3(rhs._coef_ground_range_3),
      _coef_ground_range_4(rhs._coef_ground_range_4),
      _coef_ant_elev_1(rhs._coef_ant_elev_1),
      _coef_ant_elev_2(rhs._coef_ant_elev_2),
      _coef_ant_elev_3(rhs._coef_ant_elev_3),
      _coef_ant_elev_4(rhs._coef_ant_elev_4),
      _coef_ant_elev_5(rhs._coef_ant_elev_5),
      _range_time_origin_ant(rhs._range_time_origin_ant)
   {}

   ErsSarFacilityData& ErsSarFacilityData::operator=(const ErsSarFacilityData& rhs)
   {
      _last_release_qc_date = rhs._last_release_qc_date;
      _last_release_cal_date = rhs._last_release_cal_date;
      _qa_summary_flag = rhs._qa_summary_flag;
      _prf_code_change_flag = rhs._prf_code_change_flag;
      _sampling_win_change_flag = rhs._sampling_win_change_flag;
      _cal_gain_change_flag = rhs._cal_gain_change_flag;
      _quirp_qu_flag = rhs._quirp_qu_flag;
      _inp_data_stat_flag = rhs._inp_data_stat_flag;
      _dopp_cent_conf_meas_flag = rhs._dopp_cent_conf_meas_flag;
      _dopp_cent_val_flag = rhs._dopp_cent_val_flag;
      _dopp_ambig_conf_meas_flag = rhs._dopp_ambig_conf_meas_flag;
      _outp_data_mean_flag = rhs._outp_data_mean_flag;
      _OGOB_flag = rhs._OGOB_flag;
      _PRF_changes = rhs._PRF_changes;
      _sampling_win_changes = rhs._sampling_win_changes;
      _cal_gain_changes = rhs._cal_gain_changes;
      _missing_lines = rhs._missing_lines;
      _rec_gain_changes = rhs._rec_gain_changes;
      _pulse_width_of_ACF_3db = rhs._pulse_width_of_ACF_3db;
      _first_side_lobe_lev_of_ACF = rhs._first_side_lobe_lev_of_ACF;
      _ISLR_of_ACF = rhs._ISLR_of_ACF;
      _dopp_cent_conf_meas = rhs._dopp_cent_conf_meas;
      _dopp_ambig_conf_meas = rhs._dopp_ambig_conf_meas;
      _inp_data_I_mean = rhs._inp_data_I_mean;
      _inp_data_Q_mean = rhs._inp_data_Q_mean;
      _inp_data_I_stddev = rhs._inp_data_I_stddev;
      _inp_data_Q_stddev = rhs._inp_data_Q_stddev;
      _cal_sys_gain = rhs._cal_sys_gain;
      _first_rec_gain_read = rhs._first_rec_gain_read;
      _dopp_ambig_num = rhs._dopp_ambig_num;
      _I_channel_bias_correction = rhs._I_channel_bias_correction;
      _Q_channel_bias_correction = rhs._Q_channel_bias_correction;
      _I_channel_gain_correction = rhs._I_channel_gain_correction;
      _Q_channel_gain_correction = rhs._Q_channel_gain_correction;
      _Q_channel_I_Q_correction = rhs._Q_channel_I_Q_correction;
      _noise_power = rhs._noise_power;
      _int_cal_utc = rhs._int_cal_utc;
      _num_valid_cal_pulses = rhs._num_valid_cal_pulses;
      _num_valid_noise_pulses = rhs._num_valid_noise_pulses;
      _num_valid_replicas = rhs._num_valid_replicas;
      _first_replica_sample = rhs._first_replica_sample;
      _mean_cal_pulse_power = rhs._mean_cal_pulse_power;
      _mean_noise_power = rhs._mean_noise_power;
      _range_comp_norm_fact = rhs._range_comp_norm_fact;
      _replica_power = rhs._replica_power;
      _first_range_pixel_mid_az_inc = rhs._first_range_pixel_mid_az_inc;
      _center_range_pix_mid_az_inc = rhs._center_range_pix_mid_az_inc;
      _last_range_pix_mid_az_inc = rhs._last_range_pix_mid_az_inc;
      _norm_ref_range_ro = rhs._norm_ref_range_ro;
      _antenna_elev_flag = rhs._antenna_elev_flag;
      _abs_cal_const_K = rhs._abs_cal_const_K;
      _upp_bound_K = rhs._upp_bound_K;
      _low_bound_K = rhs._low_bound_K;
      _proc_noise_scale_fact = rhs._proc_noise_scale_fact;
      _K_gen_date = rhs._K_gen_date;
      _K_vers_num = rhs._K_vers_num;
      _num_duplic_input_lines = rhs._num_duplic_input_lines;
      _estim_bit_error_rate = rhs._estim_bit_error_rate;
      _out_image_mean = rhs._out_image_mean;
      _out_image_std_dev = rhs._out_image_std_dev;
      _out_image_max_value = rhs._out_image_max_value;
      _time_raw_data_first_input = rhs._time_raw_data_first_input;
      _time_asc_node_state_vectors = rhs._time_asc_node_state_vectors;
      _asc_node_pos_X_comp = rhs._asc_node_pos_X_comp;
      _asc_node_pos_Y_comp = rhs._asc_node_pos_Y_comp;
      _asc_node_pos_Z_comp = rhs._asc_node_pos_Z_comp;
      _asc_node_vel_X_comp = rhs._asc_node_vel_X_comp;
      _asc_node_vel_Y_comp = rhs._asc_node_vel_Y_comp;
      _asc_node_vel_Z_comp = rhs._asc_node_vel_Z_comp;
      _out_pixel_bit_length = rhs._out_pixel_bit_length;
      _proc_gain_param_1 = rhs._proc_gain_param_1;
      _proc_gain_param_2 = rhs._proc_gain_param_2;
      _proc_gain_param_3 = rhs._proc_gain_param_3;
      _peak_loc_cross_correl_fun = rhs._peak_loc_cross_correl_fun;
      _3_dB_width_CCF = rhs._3_dB_width_CCF;
      _first_side_lobe_level = rhs._first_side_lobe_level;
      _ISLR_CCF_between_last = rhs._ISLR_CCF_between_last;
      _peak_loc_CCF_betw_last = rhs._peak_loc_CCF_betw_last;
      _Roll_Tilt_Mode_flag = rhs._Roll_Tilt_Mode_flag;
      _raw_data_correction_flag = rhs._raw_data_correction_flag;
      _look_detecion_flag = rhs._look_detecion_flag;
      _doppler_ambiguity_estimat_flag = rhs._doppler_ambiguity_estimat_flag;
      _azimuth_baseband_convers_flag = rhs._azimuth_baseband_convers_flag;
      _samples_per_line_used = rhs._samples_per_line_used;
      _range_lines_skip_factor = rhs._range_lines_skip_factor;
      _time_of_inp_state_vectors = rhs._time_of_inp_state_vectors;
      _inp_state_vect_pos_X_comp = rhs._inp_state_vect_pos_X_comp;
      _inp_state_vect_pos_Y_comp = rhs._inp_state_vect_pos_Y_comp;
      _inp_state_vect_pos_Z_comp = rhs._inp_state_vect_pos_Z_comp;
      _inp_state_vect_vel_Vx_comp = rhs._inp_state_vect_vel_Vx_comp;
      _inp_state_vect_vel_Vy_comp = rhs._inp_state_vect_vel_Vy_comp;
      _inp_state_vect_vel_Vz_comp = rhs._inp_state_vect_vel_Vz_comp;
      _inp_state_vector_type_flag = rhs._inp_state_vector_type_flag;
      _win_coeff_for_range_match = rhs._win_coeff_for_range_match;
      _win_coeff_for_azi_match = rhs._win_coeff_for_azi_match;
      _update_period_range_match = rhs._update_period_range_match;
      _look_scalar_gain_1 = rhs._look_scalar_gain_1;
      _look_scalar_gain_2 = rhs._look_scalar_gain_2;
      _look_scalar_gain_3 = rhs._look_scalar_gain_3;
      _look_scalar_gain_4 = rhs._look_scalar_gain_4;
      _look_scalar_gain_5 = rhs._look_scalar_gain_5;
      _look_scalar_gain_6 = rhs._look_scalar_gain_6;
      _look_scalar_gain_7 = rhs._look_scalar_gain_7;
      _look_scalar_gain_8 = rhs._look_scalar_gain_8;
      _samp_window_start_time_bias = rhs._samp_window_start_time_bias;
      _doppler_centroid_cubic_coeff = rhs._doppler_centroid_cubic_coeff;
      _PRF_code_first_range_line = rhs._PRF_code_first_range_line;
      _PRF_code_last_range_line = rhs._PRF_code_last_range_line;
      _samp_win_start_first = rhs._samp_win_start_first;
      _samp_win_start_last = rhs._samp_win_start_last;
      _cal_syst_gain_last_proc = rhs._cal_syst_gain_last_proc;
      _receiver_gain_last_proc = rhs._receiver_gain_last_proc;
      _first_processed_range_sample = rhs._first_processed_range_sample;
      _azimuth_FFT_IFFT_ratio = rhs._azimuth_FFT_IFFT_ratio;
      _num_azimuth_blocks_proc = rhs._num_azimuth_blocks_proc;
      _num_input_raw_data_lines = rhs._num_input_raw_data_lines;
      _initial_doppler_ambiguity_num = rhs._initial_doppler_ambiguity_num;
      _thresh_no_1_flag = rhs._thresh_no_1_flag;
      _thresh_no_2_flag = rhs._thresh_no_2_flag;
      _thresh_no_3_flag = rhs._thresh_no_3_flag;
      _thresh_no_4_flag = rhs._thresh_no_4_flag;
      _thresh_no_5_flag = rhs._thresh_no_5_flag;
      _thresh_no_6_flag = rhs._thresh_no_6_flag;
      _thresh_no_7_flag = rhs._thresh_no_7_flag;
      _thresh_no_8_flag = rhs._thresh_no_8_flag;
      _thresh_no_9_flag = rhs._thresh_no_9_flag;
      _thresh_no_10_flag = rhs._thresh_no_10_flag;
      _thresh_no_11_flag = rhs._thresh_no_11_flag;
      _sat_binary_time_of_first = rhs._sat_binary_time_of_first;
      _num_valid_pixels_per_range = rhs._num_valid_pixels_per_range;
      _num_range_samp_discarded = rhs._num_range_samp_discarded;
      _I_gain_imb_lower_bound = rhs._I_gain_imb_lower_bound;
      _I_gain_imb_upper_bound = rhs._I_gain_imb_upper_bound;
      _I_Q_quad_depar_lower_bound = rhs._I_Q_quad_depar_lower_bound;
      _I_Q_quad_depar_upper_bound = rhs._I_Q_quad_depar_upper_bound;
      _3_dB_look_bandwidth = rhs._3_dB_look_bandwidth;
      _3_dB_look_proc_dopp_bandw = rhs._3_dB_look_proc_dopp_bandw;
      _range_spread_loss_comp_flag = rhs._range_spread_loss_comp_flag;
      _datation_flag = rhs._datation_flag;
      _max_error_range_line_timing = rhs._max_error_range_line_timing;
      _form_num_range_line_used = rhs._form_num_range_line_used;
      _autom_look_scal_gain_flag = rhs._autom_look_scal_gain_flag;
      _max_value_look_scalar_gain = rhs._max_value_look_scalar_gain;
      _replica_norm_method_flag = rhs._replica_norm_method_flag;
      _coef_ground_range_1 = rhs._coef_ground_range_1;
      _coef_ground_range_2 = rhs._coef_ground_range_2;
      _coef_ground_range_3 = rhs._coef_ground_range_3;
      _coef_ground_range_4 = rhs._coef_ground_range_4;
      _coef_ant_elev_1 = rhs._coef_ant_elev_1;
      _coef_ant_elev_2 = rhs._coef_ant_elev_2;
      _coef_ant_elev_3 = rhs._coef_ant_elev_3;
      _coef_ant_elev_4 = rhs._coef_ant_elev_4;
      _coef_ant_elev_5 = rhs._coef_ant_elev_5;
      _range_time_origin_ant = rhs._range_time_origin_ant;

      return *this;
   }
}
