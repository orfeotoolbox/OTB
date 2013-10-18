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

#include <EnvisatAsar/MainProcessingParameters.h>

namespace ossimplugins
{

   MainProcessingParameters::MainProcessingParameters() : EnvisatAsarRecord("MainProcessingParameters_rec")
   {
   }

   MainProcessingParameters::~MainProcessingParameters()
   {
   }

   std::ostream& operator<<(std::ostream& os, const MainProcessingParameters& data)
   {
      os<<"first_zero_doppler_time:"<<data._first_zero_doppler_time.c_str()<<std::endl;

      os<<"attach_flag:"<<data._attach_flag<<std::endl;

      os<<"last_zero_doppler_time:"<<data._last_zero_doppler_time.c_str()<<std::endl;

      os<<"work_order_id:"<<data._work_order_id.c_str()<<std::endl;

      os<<"time_diff:"<<data._time_diff<<std::endl;

      os<<"swath_num:"<<data._swath_num.c_str()<<std::endl;

      os<<"range_spacing:"<<data._range_spacing<<std::endl;

      os<<"azimut_spacing:"<<data._azimut_spacing<<std::endl;

      os<<"line_time_interval:"<<data._line_time_interval<<std::endl;

      os<<"num_output_lines:"<<data._num_output_lines<<std::endl;

      os<<"num_samples_per_line:"<<data._num_samples_per_line<<std::endl;

      os<<"data_type:"<<data._data_type.c_str()<<std::endl;

      os<<"num_range_lines_per_burst:"<<data._num_range_lines_per_burst<<std::endl;

      os<<"time_diff_zero_doppler:"<<data._time_diff_zero_doppler<<std::endl;

      os<<"data_analysis_flag:"<<data._data_analysis_flag<<std::endl;

      os<<"ant_elev_coor_flag:"<<data._ant_elev_coor_flag<<std::endl;

      os<<"chirp_extract_flag:"<<data._chirp_extract_flag<<std::endl;

      os<<"srgr_flag:"<<data._srgr_flag<<std::endl;

      os<<"dop_cen_flag:"<<data._dop_cen_flag<<std::endl;

      os<<"dop_amb_flag:"<<data._dop_amb_flag<<std::endl;

      os<<"range_spread_comp_flag:"<<data._range_spread_comp_flag<<std::endl;

      os<<"detected_flag:"<<data._detected_flag<<std::endl;

      os<<"look_sum_flag:"<<data._look_sum_flag<<std::endl;

      os<<"rms_equal_flag:"<<data._rms_equal_flag<<std::endl;

      os<<"ant_scal_flag:"<<data._ant_scal_flag<<std::endl;

      os<<"vga_com_echo_flag:"<<data._vga_com_echo_flag<<std::endl;

      os<<"vga_com_cal_flag:"<<data._vga_com_cal_flag<<std::endl;

      os<<"vga_com_nom_time_flag:"<<data._vga_com_nom_time_flag<<std::endl;

      os<<"gm_rng_comp_inv_filter_flag:"<<data._gm_rng_comp_inv_filter_flag<<std::endl;

      os<<"raw_data_analysis:"<<data._raw_data_analysis.c_str()<<std::endl;

      os<<"start_time_mds1:"<<data._start_time_mds1.c_str()<<std::endl;

      os<<"start_time_mds2:"<<data._start_time_mds2.c_str()<<std::endl;

      os<<"parameter_code:"<<data._parameter_code.c_str()<<std::endl;

      os<<"errors_counters:"<<data._errors_counters.c_str()<<std::endl;

      os<<"image_parameters 1:"<<data._image_parameters1.c_str()<<std::endl;

      os<<"prf_values[0]:"<<data._prf_values[0]<<std::endl;

      os<<"image_parameters 1:"<<data._image_parameters2.c_str()<<std::endl;

      os<<"first_proc_range_samp:"<<data._first_proc_range_samp<<std::endl;

      os<<"range_ref:"<<data._range_ref<<std::endl;

      os<<"range_samp_rate:"<<data._range_samp_rate<<std::endl;

      os<<"radar_freq:"<<data._radar_freq<<std::endl;

      os<<"num_looks_range:"<<data._num_looks_range<<std::endl;

      os<<"filter_range:"<<data._filter_range.c_str()<<std::endl;

      os<<"filter_coef_range:"<<data._filter_coef_range<<std::endl;

      os<<"bandwidth:"<<data._bandwidth.c_str()<<std::endl;

      os<<"nominal_chirp:"<<data._nominal_chirp.c_str()<<std::endl;

      os<<"num_lines_proc:"<<data._num_lines_proc<<std::endl;

      os<<"num_look_az:"<<data._num_look_az<<std::endl;

      os<<"look_bw_az:"<<data._look_bw_az<<std::endl;

      os<<"to_bw_az:"<<data._to_bw_az<<std::endl;

      os<<"filter_az:"<<data._filter_az.c_str()<<std::endl;

      os<<"filter_coef_az:"<<data._filter_coef_az<<std::endl;

      os<<"az_fm_rate[0]:"<<data._az_fm_rate[0]<<std::endl;
      os<<"az_fm_rate[1]:"<<data._az_fm_rate[1]<<std::endl;
      os<<"az_fm_rate[2]:"<<data._az_fm_rate[2]<<std::endl;

      os<<"ax_fm_origin:"<<data._ax_fm_origin<<std::endl;

      os<<"dop_amb_coef:"<<data._dop_amb_coef<<std::endl;

      os<<"calibration_factors:"<<data._calibration_factors.c_str()<<std::endl;

      os<<"noise_estimation:"<<data._noise_estimation.c_str()<<std::endl;

      os<<"output_statistics:"<<data._output_statistics.c_str()<<std::endl;

      os<<"avg_scene_height_ellpsoid:"<<data._avg_scene_height_ellpsoid<<std::endl;

      os<<"echo_comp:"<<data._echo_comp.c_str()<<std::endl;

      os<<"echo_comp_ratio:"<<data._echo_comp_ratio.c_str()<<std::endl;

      os<<"init_cal_comp:"<<data._init_cal_comp.c_str()<<std::endl;

      os<<"init_cal_ratio:"<<data._init_cal_ratio.c_str()<<std::endl;

      os<<"per_cal_comp:"<<data._per_cal_comp.c_str()<<std::endl;

      os<<"per_cal_ratio:"<<data._per_cal_ratio.c_str()<<std::endl;

      os<<"noise_comp:"<<data._noise_comp.c_str()<<std::endl;

      os<<"noise_comp_ratio:"<<data._noise_comp_ratio.c_str()<<std::endl;

      os<<"beam_overlap[0]:"<<data._beam_overlap[0]<<std::endl;
      os<<"beam_overlap[1]:"<<data._beam_overlap[1]<<std::endl;
      os<<"beam_overlap[2]:"<<data._beam_overlap[2]<<std::endl;
      os<<"beam_overlap[3]:"<<data._beam_overlap[3]<<std::endl;

      os<<"lines_per_burst[0]:"<<data._lines_per_burst[0]<<std::endl;
      os<<"lines_per_burst[1]:"<<data._lines_per_burst[1]<<std::endl;
      os<<"lines_per_burst[2]:"<<data._lines_per_burst[2]<<std::endl;
      os<<"lines_per_burst[3]:"<<data._lines_per_burst[3]<<std::endl;
      os<<"lines_per_burst[4]:"<<data._lines_per_burst[4]<<std::endl;

      os<<"time_first_SS1_echo:"<<data._time_first_SS1_echo.c_str()<<std::endl;

      os<<"state_vector_time_1:"<<data._state_vector_time_1_day<<std::endl;

      os<<"state_vector_time_1:"<<data._state_vector_time_1_sec<<std::endl;

      os<<"state_vector_time_1:"<<data._state_vector_time_1_microsec<<std::endl;

      os<<"x_pos_1:"<<data._x_pos_1<<std::endl;

      os<<"y_pos_1:"<<data._y_pos_1<<std::endl;

      os<<"z_pos_1:"<<data._z_pos_1<<std::endl;

      os<<"x_vel_1:"<<data._x_vel_1<<std::endl;

      os<<"y_vel_1:"<<data._y_vel_1<<std::endl;

      os<<"z_vel_1:"<<data._z_vel_1<<std::endl;

      os<<"state_vector_time_2:"<<data._state_vector_time_2_day<<std::endl;

      os<<"state_vector_time_2:"<<data._state_vector_time_2_sec<<std::endl;

      os<<"state_vector_time_2:"<<data._state_vector_time_2_microsec<<std::endl;

      os<<"x_pos_2:"<<data._x_pos_2<<std::endl;

      os<<"y_pos_2:"<<data._y_pos_2<<std::endl;

      os<<"z_pos_2:"<<data._z_pos_2<<std::endl;

      os<<"x_vel_2:"<<data._x_vel_2<<std::endl;

      os<<"y_vel_2:"<<data._y_vel_2<<std::endl;

      os<<"z_vel_2:"<<data._z_vel_2<<std::endl;

      os<<"state_vector_time_3:"<<data._state_vector_time_3_day<<std::endl;

      os<<"state_vector_time_3:"<<data._state_vector_time_3_sec<<std::endl;

      os<<"state_vector_time_3:"<<data._state_vector_time_3_microsec<<std::endl;

      os<<"x_pos_3:"<<data._x_pos_3<<std::endl;

      os<<"y_pos_3:"<<data._y_pos_3<<std::endl;

      os<<"z_pos_3:"<<data._z_pos_3<<std::endl;

      os<<"x_vel_3:"<<data._x_vel_3<<std::endl;

      os<<"y_vel_3:"<<data._y_vel_3<<std::endl;

      os<<"z_vel_3:"<<data._z_vel_3<<std::endl;

      os<<"state_vector_time_4:"<<data._state_vector_time_4_day<<std::endl;

      os<<"state_vector_time_4:"<<data._state_vector_time_4_sec<<std::endl;

      os<<"state_vector_time_4:"<<data._state_vector_time_4_microsec<<std::endl;

      os<<"x_pos_4:"<<data._x_pos_4<<std::endl;

      os<<"y_pos_4:"<<data._y_pos_4<<std::endl;

      os<<"z_pos_4:"<<data._z_pos_4<<std::endl;

      os<<"x_vel_4:"<<data._x_vel_4<<std::endl;

      os<<"y_vel_4:"<<data._y_vel_4<<std::endl;

      os<<"z_vel_4:"<<data._z_vel_4<<std::endl;

      os<<"state_vector_time_5:"<<data._state_vector_time_5_day<<std::endl;

      os<<"state_vector_time_5:"<<data._state_vector_time_5_sec<<std::endl;

      os<<"state_vector_time_5:"<<data._state_vector_time_5_microsec<<std::endl;

      os<<"x_pos_5:"<<data._x_pos_5<<std::endl;

      os<<"y_pos_5:"<<data._y_pos_5<<std::endl;

      os<<"z_pos_5:"<<data._z_pos_5<<std::endl;

      os<<"x_vel_5:"<<data._x_vel_5<<std::endl;

      os<<"y_vel_5:"<<data._y_vel_5<<std::endl;

      os<<"z_vel_5:"<<data._z_vel_5<<std::endl;

      return os;

   }

   std::istream& operator>>(std::istream& is, MainProcessingParameters& data)
   {
      char buf64[65];
      buf64[64] = '\0';
      char buf32[33];
      buf32[32] = '\0';
      // char buf27[28];
      // buf27[27] = '\0';
      char buf20[21];
      buf20[20] = '\0';
      char buf16[17];
      buf16[16]='\0';
      // char buf15[16];
      // buf15[15] = '\0';
      // char buf14[15];
      // buf14[14] = '\0';
      // char buf13[14];
      // buf13[13] = '\0';
      char buf12[13];
      buf12[12] = '\0';
      // char buf11[12];
      // buf11[11] = '\0';
      // char buf10[11];
      // buf10[10] = '\0';
      // char buf9[10];
      // buf9[9] = '\0';
      // char buf8[9];
      // buf8[8] = '\0';
      char buf7[8];
      buf7[7] = '\0';
      char buf6[7];
      buf6[6] = '\0';
      char buf5[6];
      buf5[5] = '\0';
      char buf4[5];
      buf4[4] = '\0';
      char buf3[4];
      buf3[3] = '\0';
      // char buf2[3];
      // buf2[2] = '\0';
//	char buf1[1]; // not used avoid warning

//	int * var_long; // not used avoid warning

      is.read(buf12,12);
      data._first_zero_doppler_time = buf12;

      is.read((char*)&(data._attach_flag),1);

      is.read(buf12,12);
      data._last_zero_doppler_time = buf12;

      is.read(buf12,12);
      data._work_order_id = buf12;

      is.read((char*)&(data._time_diff),4);
      data.SwitchEndian(data._time_diff);

      is.read(buf3,3);
      data._swath_num = buf3;

      is.read((char*)&(data._range_spacing),4);
      data.SwitchEndian(data._range_spacing);

      is.read((char*)&(data._azimut_spacing),4);
      data.SwitchEndian(data._azimut_spacing);

      is.read((char*)&(data._line_time_interval),4);
      data.SwitchEndian(data._line_time_interval);

      is.read((char*)&(data._num_output_lines),4);
      data.SwitchEndian(data._num_output_lines);

      is.read((char*)&(data._num_samples_per_line),4);
      data.SwitchEndian(data._num_samples_per_line);

      is.read(buf5,5);
      data._data_type = buf5;

      is.read((char*)&(data._num_range_lines_per_burst),4);
      data.SwitchEndian(data._num_range_lines_per_burst);

      is.read((char*)&(data._time_diff_zero_doppler),4);
      data.SwitchEndian(data._time_diff_zero_doppler);

      is.read(buf64,43);

      is.read((char*)&(data._data_analysis_flag),1);

      is.read((char*)&(data._ant_elev_coor_flag),1);

      is.read((char*)&(data._chirp_extract_flag),1);

      is.read((char*)&(data._srgr_flag),1);

      is.read((char*)&(data._dop_cen_flag),1);

      is.read((char*)&(data._dop_amb_flag),1);

      is.read((char*)&(data._range_spread_comp_flag),1);

      is.read((char*)&(data._detected_flag),1);

      is.read((char*)&(data._look_sum_flag),1);

      is.read((char*)&(data._rms_equal_flag),1);

      is.read((char*)&(data._ant_scal_flag),1);

      is.read((char*)&(data._vga_com_echo_flag),1);

      is.read((char*)&(data._vga_com_cal_flag),1);

      is.read((char*)&(data._vga_com_nom_time_flag),1);

      is.read((char*)&(data._gm_rng_comp_inv_filter_flag),1);

      is.read(buf6,6);

      char buf184[185];
      buf184[184] = '\0';
      is.read(buf184,184);
      data._raw_data_analysis = buf184;

      is.read(buf32,32);

      is.read(buf20,20);
      data._start_time_mds1 = buf20;

      is.read(buf20,20);
      data._start_time_mds2 = buf20;

      char buf120[121];
      buf120[120] = '\0';
      is.read(buf120,120);
      data._parameter_code = buf120;

      is.read(buf64,60);

      is.read(buf64,40);
      buf64[40] = '\0';
      data._errors_counters = buf64;

      is.read(buf32,26);

      is.read(buf64,60);
      buf64[60] = '\0';
      data._image_parameters1 = buf64;

      for (int i = 0; i<5; i++)
      {
         is.read((char*)&(data._prf_values[i]),4);
         data.SwitchEndian(data._prf_values[i]);
      }

      char buf190[191];
      buf190[190] = '\0';
      is.read(buf190,190);
      data._image_parameters2 = buf190;

      is.read(buf64,62);

      is.read((char*)&(data._first_proc_range_samp),4);
      data.SwitchEndian(data._first_proc_range_samp);

      is.read((char*)&(data._range_ref),4);
      data.SwitchEndian(data._range_ref);

      is.read((char*)&(data._range_samp_rate),4);
      data.SwitchEndian(data._range_samp_rate);

      is.read((char*)&(data._radar_freq),4);
      data.SwitchEndian(data._radar_freq);

      is.read((char*)&(data._num_looks_range),2);
      data.SwitchEndian(data._num_looks_range);

      is.read(buf7,7);
      data._filter_range = buf7;

      is.read((char*)&(data._filter_coef_range),4);
      data.SwitchEndian(data._filter_coef_range);

      is.read(buf64,40);
      buf64[40] = '\0';
      data._bandwidth = buf64;

      char buf160[161];
      buf160[160] = '\0';
      is.read(buf160,160);
      data._nominal_chirp = buf160;

      is.read(buf64,60);

      is.read((char*)&(data._num_lines_proc),4);
      data.SwitchEndian(data._num_lines_proc);

      is.read((char*)&(data._num_look_az),2);
      data.SwitchEndian(data._num_look_az);

      is.read((char*)&(data._look_bw_az),4);
      data.SwitchEndian(data._look_bw_az);

      is.read((char*)&(data._to_bw_az),4);
      data.SwitchEndian(data._to_bw_az);

      is.read(buf7,7);
      data._filter_az = buf7;

      is.read((char*)&(data._filter_coef_az),4);
      data.SwitchEndian(data._filter_coef_az);

      for (int i = 0; i <3; i++) {
         is.read((char*)&(data._az_fm_rate[i]),4);
         data.SwitchEndian(data._az_fm_rate[i]);
      }

      is.read((char*)&(data._ax_fm_origin),4);
      data.SwitchEndian(data._ax_fm_origin);

      is.read((char*)&(data._dop_amb_coef),4);
      data.SwitchEndian(data._dop_amb_coef);

      char buf128[129];
      buf128[128] = '\0';
      is.read(buf128,68);

      is.read(buf16,16);
      data._calibration_factors = buf16;

      is.read(buf64,40);
      buf64[40]='\0';
      data._noise_estimation = buf64;

      is.read(buf64,64);

      is.read(buf12,12);

      is.read(buf32,32);
      data._output_statistics = buf32;

      is.read((char*)&(data._avg_scene_height_ellpsoid),4);
      data.SwitchEndian(data._avg_scene_height_ellpsoid);

      is.read(buf64,48);

      is.read(buf4,4);
      data._echo_comp = buf4;

      is.read(buf3,3);
      data._echo_comp_ratio = buf3;

      is.read(buf4,4);
      data._init_cal_comp = buf4;

      is.read(buf3,3);
      data._init_cal_ratio = buf3;

      is.read(buf4,4);
      data._per_cal_comp = buf4;

      is.read(buf3,3);
      data._per_cal_ratio = buf3;

      is.read(buf4,4);
      data._noise_comp = buf4;

      is.read(buf3,3);
      data._noise_comp_ratio = buf3;

      is.read(buf64,64);

      for (int i=0;i<4;i++)
      {
         is.read((char*)&(data._beam_overlap[i]),4);
         data.SwitchEndian(data._beam_overlap[i]);
      }
      for (int i=0;i<4;i++)
      {
         is.read((char*)&(data._beam_param[i]),4);
         data.SwitchEndian(data._beam_param[i]);
      }
      for (int i=0;i<5;i++)
      {
         is.read((char*)&(data._lines_per_burst[i]),4);
         data.SwitchEndian(data._lines_per_burst[i]);
      }

      is.read(buf12,12);
      data._time_first_SS1_echo = buf12;

      is.read(buf16,16);

      is.read((char*)&(data._state_vector_time_1_day),4);
      data.SwitchEndian(data._state_vector_time_1_day);

      is.read((char*)&(data._state_vector_time_1_sec),4);
      data.SwitchEndian(data._state_vector_time_1_sec);

      is.read((char*)&(data._state_vector_time_1_microsec),4);
      data.SwitchEndian(data._state_vector_time_1_microsec);

      is.read((char*)&(data._x_pos_1),4);
      data.SwitchEndian(data._x_pos_1);

      is.read((char*)&(data._y_pos_1),4);
      data.SwitchEndian(data._y_pos_1);

      is.read((char*)&(data._z_pos_1),4);
      data.SwitchEndian(data._z_pos_1);

      is.read((char*)&(data._x_vel_1),4);
      data.SwitchEndian(data._x_vel_1);

      is.read((char*)&(data._y_vel_1),4);
      data.SwitchEndian(data._y_vel_1);

      is.read((char*)&(data._z_vel_1),4);
      data.SwitchEndian(data._z_vel_1);

      is.read((char*)&(data._state_vector_time_2_day),4);
      data.SwitchEndian(data._state_vector_time_2_day);

      is.read((char*)&(data._state_vector_time_2_sec),4);
      data.SwitchEndian(data._state_vector_time_2_sec);

      is.read((char*)&(data._state_vector_time_2_microsec),4);
      data.SwitchEndian(data._state_vector_time_2_microsec);

      is.read((char*)&(data._x_pos_2),4);
      data.SwitchEndian(data._x_pos_2);

      is.read((char*)&(data._y_pos_2),4);
      data.SwitchEndian(data._y_pos_2);

      is.read((char*)&(data._z_pos_2),4);
      data.SwitchEndian(data._z_pos_2);

      is.read((char*)&(data._x_vel_2),4);
      data.SwitchEndian(data._x_vel_2);

      is.read((char*)&(data._y_vel_2),4);
      data.SwitchEndian(data._y_vel_2);

      is.read((char*)&(data._z_vel_2),4);
      data.SwitchEndian(data._z_vel_2);

      is.read((char*)&(data._state_vector_time_3_day),4);
      data.SwitchEndian(data._state_vector_time_3_day);

      is.read((char*)&(data._state_vector_time_3_sec),4);
      data.SwitchEndian(data._state_vector_time_3_sec);

      is.read((char*)&(data._state_vector_time_3_microsec),4);
      data.SwitchEndian(data._state_vector_time_3_microsec);

      is.read((char*)&(data._x_pos_3),4);
      data.SwitchEndian(data._x_pos_3);

      is.read((char*)&(data._y_pos_3),4);
      data.SwitchEndian(data._y_pos_3);

      is.read((char*)&(data._z_pos_3),4);
      data.SwitchEndian(data._z_pos_3);

      is.read((char*)&(data._x_vel_3),4);
      data.SwitchEndian(data._x_vel_3);

      is.read((char*)&(data._y_vel_3),4);
      data.SwitchEndian(data._y_vel_3);

      is.read((char*)&(data._z_vel_3),4);
      data.SwitchEndian(data._z_vel_3);

      is.read((char*)&(data._state_vector_time_4_day),4);
      data.SwitchEndian(data._state_vector_time_4_day);

      is.read((char*)&(data._state_vector_time_4_sec),4);
      data.SwitchEndian(data._state_vector_time_4_sec);

      is.read((char*)&(data._state_vector_time_4_microsec),4);
      data.SwitchEndian(data._state_vector_time_4_microsec);

      is.read((char*)&(data._x_pos_4),4);
      data.SwitchEndian(data._x_pos_4);

      is.read((char*)&(data._y_pos_4),4);
      data.SwitchEndian(data._y_pos_4);

      is.read((char*)&(data._z_pos_4),4);
      data.SwitchEndian(data._z_pos_4);

      is.read((char*)&(data._x_vel_4),4);
      data.SwitchEndian(data._x_vel_4);

      is.read((char*)&(data._y_vel_4),4);
      data.SwitchEndian(data._y_vel_4);

      is.read((char*)&(data._z_vel_4),4);
      data.SwitchEndian(data._z_vel_4);

      is.read((char*)&(data._state_vector_time_5_day),4);
      data.SwitchEndian(data._state_vector_time_5_day);

      is.read((char*)&(data._state_vector_time_5_sec),4);
      data.SwitchEndian(data._state_vector_time_5_sec);

      is.read((char*)&(data._state_vector_time_5_microsec),4);
      data.SwitchEndian(data._state_vector_time_5_microsec);

      is.read((char*)&(data._x_pos_5),4);
      data.SwitchEndian(data._x_pos_5);

      is.read((char*)&(data._y_pos_5),4);
      data.SwitchEndian(data._y_pos_5);

      is.read((char*)&(data._z_pos_5),4);
      data.SwitchEndian(data._z_pos_5);

      is.read((char*)&(data._x_vel_5),4);
      data.SwitchEndian(data._x_vel_5);

      is.read((char*)&(data._y_vel_5),4);
      data.SwitchEndian(data._y_vel_5);

      is.read((char*)&(data._z_vel_5),4);
      data.SwitchEndian(data._z_vel_5);

      is.read(buf64,64);

      return is;

   }

   MainProcessingParameters::MainProcessingParameters(const MainProcessingParameters& rhs):
      EnvisatAsarRecord(rhs),
      _first_zero_doppler_time(rhs._first_zero_doppler_time),
      _attach_flag(rhs._attach_flag),
      _last_zero_doppler_time(rhs._last_zero_doppler_time),
      _work_order_id(rhs._work_order_id),
      _time_diff(rhs._time_diff),
      _swath_num(rhs._swath_num),
      _range_spacing(rhs._range_spacing),
      _azimut_spacing(rhs._azimut_spacing),
      _line_time_interval(rhs._line_time_interval),
      _num_output_lines(rhs._num_output_lines),
      _num_samples_per_line(rhs._num_samples_per_line),
      _data_type(rhs._data_type),
      _num_range_lines_per_burst(rhs._num_range_lines_per_burst),
      _time_diff_zero_doppler(rhs._time_diff_zero_doppler),
      _data_analysis_flag(rhs._data_analysis_flag),
      _ant_elev_coor_flag(rhs._ant_elev_coor_flag),
      _chirp_extract_flag(rhs._chirp_extract_flag),
      _srgr_flag(rhs._srgr_flag),
      _dop_cen_flag(rhs._dop_cen_flag),
      _dop_amb_flag(rhs._dop_amb_flag),
      _range_spread_comp_flag(rhs._range_spread_comp_flag),
      _detected_flag(rhs._detected_flag),
      _look_sum_flag(rhs._look_sum_flag),
      _rms_equal_flag(rhs._rms_equal_flag),
      _ant_scal_flag(rhs._ant_scal_flag),
      _vga_com_echo_flag(rhs._vga_com_echo_flag),
      _vga_com_cal_flag(rhs._vga_com_cal_flag),
      _vga_com_nom_time_flag(rhs._vga_com_nom_time_flag),
      _gm_rng_comp_inv_filter_flag(rhs._gm_rng_comp_inv_filter_flag),
      _raw_data_analysis(rhs._raw_data_analysis),
      _start_time_mds1(rhs._start_time_mds1),
      _start_time_mds2(rhs._start_time_mds2),
      _parameter_code(rhs._parameter_code),
      _errors_counters(rhs._errors_counters),
      _image_parameters1(rhs._image_parameters1),
      _image_parameters2(rhs._image_parameters2),
      _first_proc_range_samp(rhs._first_proc_range_samp),
      _range_ref(rhs._range_ref),
      _range_samp_rate(rhs._range_samp_rate),
      _radar_freq(rhs._radar_freq),
      _num_looks_range(rhs._num_looks_range),
      _filter_range(rhs._filter_range),
      _filter_coef_range(rhs._filter_coef_range),
      _bandwidth(rhs._bandwidth),
      _nominal_chirp(rhs._nominal_chirp),
      _num_lines_proc(rhs._num_lines_proc),
      _num_look_az(rhs._num_look_az),
      _look_bw_az(rhs._look_bw_az),
      _to_bw_az(rhs._to_bw_az),
      _filter_az(rhs._filter_az),
      _filter_coef_az(rhs._filter_coef_az),
      _ax_fm_origin(rhs._ax_fm_origin),
      _dop_amb_coef(rhs._dop_amb_coef),
      _calibration_factors(rhs._calibration_factors),
      _noise_estimation(rhs._noise_estimation),
      _output_statistics(rhs._output_statistics),
      _avg_scene_height_ellpsoid(rhs._avg_scene_height_ellpsoid),
      _echo_comp(rhs._echo_comp),
      _echo_comp_ratio(rhs._echo_comp_ratio),
      _init_cal_comp(rhs._init_cal_comp),
      _init_cal_ratio(rhs._init_cal_ratio),
      _per_cal_comp(rhs._per_cal_comp),
      _per_cal_ratio(rhs._per_cal_ratio),
      _noise_comp(rhs._noise_comp),
      _noise_comp_ratio(rhs._noise_comp_ratio),
      _time_first_SS1_echo(rhs._time_first_SS1_echo),
      _state_vector_time_1_day(rhs._state_vector_time_1_day),
      _state_vector_time_1_sec(rhs._state_vector_time_1_sec),
      _state_vector_time_1_microsec(rhs._state_vector_time_1_microsec),
      _x_pos_1(rhs._x_pos_1),
      _y_pos_1(rhs._y_pos_1),
      _z_pos_1(rhs._z_pos_1),
      _x_vel_1(rhs._x_vel_1),
      _y_vel_1(rhs._y_vel_1),
      _z_vel_1(rhs._z_vel_1),
      _state_vector_time_2_day(rhs._state_vector_time_2_day),
      _state_vector_time_2_sec(rhs._state_vector_time_2_sec),
      _state_vector_time_2_microsec(rhs._state_vector_time_2_microsec),
      _x_pos_2(rhs._x_pos_2),
      _y_pos_2(rhs._y_pos_2),
      _z_pos_2(rhs._z_pos_2),
      _x_vel_2(rhs._x_vel_2),
      _y_vel_2(rhs._y_vel_2),
      _z_vel_2(rhs._z_vel_2),
      _state_vector_time_3_day(rhs._state_vector_time_3_day),
      _state_vector_time_3_sec(rhs._state_vector_time_3_sec),
      _state_vector_time_3_microsec(rhs._state_vector_time_3_microsec),
      _x_pos_3(rhs._x_pos_3),
      _y_pos_3(rhs._y_pos_3),
      _z_pos_3(rhs._z_pos_3),
      _x_vel_3(rhs._x_vel_3),
      _y_vel_3(rhs._y_vel_3),
      _z_vel_3(rhs._z_vel_3),
      _state_vector_time_4_day(rhs._state_vector_time_4_day),
      _state_vector_time_4_sec(rhs._state_vector_time_4_sec),
      _state_vector_time_4_microsec(rhs._state_vector_time_4_microsec),
      _x_pos_4(rhs._x_pos_4),
      _y_pos_4(rhs._y_pos_4),
      _z_pos_4(rhs._z_pos_4),
      _x_vel_4(rhs._x_vel_4),
      _y_vel_4(rhs._y_vel_4),
      _z_vel_4(rhs._z_vel_4),
      _state_vector_time_5_day(rhs._state_vector_time_5_day),
      _state_vector_time_5_sec(rhs._state_vector_time_5_sec),
      _state_vector_time_5_microsec(rhs._state_vector_time_5_microsec),
      _x_pos_5(rhs._x_pos_5),
      _y_pos_5(rhs._y_pos_5),
      _z_pos_5(rhs._z_pos_5),
      _x_vel_5(rhs._x_vel_5),
      _y_vel_5(rhs._y_vel_5),
      _z_vel_5(rhs._z_vel_5)
   {
      _az_fm_rate[0] = rhs._az_fm_rate[0];
      _az_fm_rate[1] = rhs._az_fm_rate[1];
      _az_fm_rate[2] = rhs._az_fm_rate[2];

      _prf_values[0] = rhs._prf_values[0];
      _prf_values[1] = rhs._prf_values[1];
      _prf_values[2] = rhs._prf_values[2];
      _prf_values[3] = rhs._prf_values[3];
      _prf_values[4] = rhs._prf_values[4];

      _beam_overlap[0]= rhs._beam_overlap[0];
      _beam_overlap[1]= rhs._beam_overlap[1];
      _beam_overlap[2]= rhs._beam_overlap[2];
      _beam_overlap[3]= rhs._beam_overlap[3];

      _beam_param[0]= rhs._beam_param[0];
      _beam_param[1]= rhs._beam_param[1];
      _beam_param[2]= rhs._beam_param[2];
      _beam_param[3]= rhs._beam_param[3];

      _lines_per_burst[0] =rhs._lines_per_burst[0];
      _lines_per_burst[1] =rhs._lines_per_burst[1];
      _lines_per_burst[2] =rhs._lines_per_burst[2];
      _lines_per_burst[3] =rhs._lines_per_burst[3];
      _lines_per_burst[4] =rhs._lines_per_burst[4];
   }


   MainProcessingParameters& MainProcessingParameters::operator=(const MainProcessingParameters& rhs)
   {
      _first_zero_doppler_time = rhs._first_zero_doppler_time;
      _attach_flag = rhs._attach_flag;
      _last_zero_doppler_time = rhs._last_zero_doppler_time;
      _work_order_id = rhs._work_order_id;
      _time_diff = rhs._time_diff;
      _swath_num = rhs._swath_num;
      _range_spacing = rhs._range_spacing;
      _azimut_spacing = rhs._azimut_spacing;
      _line_time_interval = rhs._line_time_interval;
      _num_output_lines = rhs._num_output_lines;
      _num_samples_per_line = rhs._num_samples_per_line;
      _data_type = rhs._data_type;
      _num_range_lines_per_burst = rhs._num_range_lines_per_burst;
      _time_diff_zero_doppler = rhs._time_diff_zero_doppler;
      _data_analysis_flag = rhs._data_analysis_flag;
      _ant_elev_coor_flag = rhs._ant_elev_coor_flag;
      _chirp_extract_flag = rhs._chirp_extract_flag;
      _srgr_flag = rhs._srgr_flag;
      _dop_cen_flag = rhs._dop_cen_flag;
      _dop_amb_flag = rhs._dop_amb_flag;
      _range_spread_comp_flag = rhs._range_spread_comp_flag;
      _detected_flag = rhs._detected_flag;
      _look_sum_flag = rhs._look_sum_flag;
      _rms_equal_flag = rhs._rms_equal_flag;
      _ant_scal_flag = rhs._ant_scal_flag;
      _vga_com_echo_flag = rhs._vga_com_echo_flag;
      _vga_com_cal_flag = rhs._vga_com_cal_flag;
      _vga_com_nom_time_flag = rhs._vga_com_nom_time_flag;
      _gm_rng_comp_inv_filter_flag = rhs._gm_rng_comp_inv_filter_flag;
      _raw_data_analysis = rhs._raw_data_analysis;
      _start_time_mds1 = rhs._start_time_mds1;
      _start_time_mds2 = rhs._start_time_mds2;
      _parameter_code = rhs._parameter_code;
      _errors_counters = rhs._errors_counters;
      _image_parameters1 = rhs._image_parameters1;
      _image_parameters2 = rhs._image_parameters2;
      _first_proc_range_samp = rhs._first_proc_range_samp;
      _range_ref = rhs._range_ref;
      _range_samp_rate = rhs._range_samp_rate;
      _radar_freq = rhs._radar_freq;
      _num_looks_range = rhs._num_looks_range;
      _filter_range = rhs._filter_range;
      _filter_coef_range = rhs._filter_coef_range;
      _bandwidth = rhs._bandwidth;
      _nominal_chirp = rhs._nominal_chirp;
      _num_lines_proc = rhs._num_lines_proc;
      _num_look_az = rhs._num_look_az;
      _look_bw_az = rhs._look_bw_az;
      _to_bw_az = rhs._to_bw_az;
      _filter_az = rhs._filter_az;
      _filter_coef_az = rhs._filter_coef_az;
      _ax_fm_origin = rhs._ax_fm_origin;
      _dop_amb_coef = rhs._dop_amb_coef;
      _calibration_factors = rhs._calibration_factors;
      _noise_estimation = rhs._noise_estimation;
      _output_statistics = rhs._output_statistics;
      _avg_scene_height_ellpsoid = rhs._avg_scene_height_ellpsoid;
      _echo_comp = rhs._echo_comp;
      _echo_comp_ratio = rhs._echo_comp_ratio;
      _init_cal_comp = rhs._init_cal_comp;
      _init_cal_ratio = rhs._init_cal_ratio;
      _per_cal_comp = rhs._per_cal_comp;
      _per_cal_ratio = rhs._per_cal_ratio;
      _noise_comp = rhs._noise_comp;
      _noise_comp_ratio = rhs._noise_comp_ratio;
      _time_first_SS1_echo = rhs._time_first_SS1_echo;
      _state_vector_time_1_day = rhs._state_vector_time_1_day;
      _state_vector_time_1_sec = rhs._state_vector_time_1_sec;
      _state_vector_time_1_microsec = rhs._state_vector_time_1_microsec;
      _x_pos_1 = rhs._x_pos_1;
      _y_pos_1 = rhs._y_pos_1;
      _z_pos_1 = rhs._z_pos_1;
      _x_vel_1 = rhs._x_vel_1;
      _y_vel_1 = rhs._y_vel_1;
      _z_vel_1 = rhs._z_vel_1;
      _state_vector_time_2_day = rhs._state_vector_time_2_day;
      _state_vector_time_2_sec = rhs._state_vector_time_2_sec;
      _state_vector_time_2_microsec = rhs._state_vector_time_2_microsec;
      _x_pos_2 = rhs._x_pos_2;
      _y_pos_2 = rhs._y_pos_2;
      _z_pos_2 = rhs._z_pos_2;
      _x_vel_2 = rhs._x_vel_2;
      _y_vel_2 = rhs._y_vel_2;
      _z_vel_2 = rhs._z_vel_2;
      _state_vector_time_3_day = rhs._state_vector_time_3_day;
      _state_vector_time_3_sec = rhs._state_vector_time_3_sec;
      _state_vector_time_3_microsec = rhs._state_vector_time_3_microsec;
      _x_pos_3 = rhs._x_pos_3;
      _y_pos_3 = rhs._y_pos_3;
      _z_pos_3 = rhs._z_pos_3;
      _x_vel_3 = rhs._x_vel_3;
      _y_vel_3 = rhs._y_vel_3;
      _z_vel_3 = rhs._z_vel_3;
      _state_vector_time_4_day = rhs._state_vector_time_4_day;
      _state_vector_time_4_sec = rhs._state_vector_time_4_sec;
      _state_vector_time_4_microsec = rhs._state_vector_time_4_microsec;
      _x_pos_4 = rhs._x_pos_4;
      _y_pos_4 = rhs._y_pos_4;
      _z_pos_4 = rhs._z_pos_4;
      _x_vel_4 = rhs._x_vel_4;
      _y_vel_4 = rhs._y_vel_4;
      _z_vel_4 = rhs._z_vel_4;
      _state_vector_time_5_day = rhs._state_vector_time_5_day;
      _state_vector_time_5_sec = rhs._state_vector_time_5_sec;
      _state_vector_time_5_microsec = rhs._state_vector_time_5_microsec;
      _x_pos_5 = rhs._x_pos_5;
      _y_pos_5 = rhs._y_pos_5;
      _z_pos_5 = rhs._z_pos_5;
      _x_vel_5 = rhs._x_vel_5;
      _y_vel_5 = rhs._y_vel_5;
      _z_vel_5 = rhs._z_vel_5;

      _az_fm_rate[0] = rhs._az_fm_rate[0];
      _az_fm_rate[1] = rhs._az_fm_rate[1];
      _az_fm_rate[2] = rhs._az_fm_rate[2];

      _prf_values[0] = rhs._prf_values[0];
      _prf_values[1] = rhs._prf_values[1];
      _prf_values[2] = rhs._prf_values[2];
      _prf_values[3] = rhs._prf_values[3];
      _prf_values[4] = rhs._prf_values[4];

      _beam_overlap[0]= rhs._beam_overlap[0];
      _beam_overlap[1]= rhs._beam_overlap[1];
      _beam_overlap[2]= rhs._beam_overlap[2];
      _beam_overlap[3]= rhs._beam_overlap[3];

      _beam_param[0]= rhs._beam_param[0];
      _beam_param[1]= rhs._beam_param[1];
      _beam_param[2]= rhs._beam_param[2];
      _beam_param[3]= rhs._beam_param[3];

      _lines_per_burst[0] =rhs._lines_per_burst[0];
      _lines_per_burst[1] =rhs._lines_per_burst[1];
      _lines_per_burst[2] =rhs._lines_per_burst[2];
      _lines_per_burst[3] =rhs._lines_per_burst[3];
      _lines_per_burst[4] =rhs._lines_per_burst[4];

      return *this;
   }

}
