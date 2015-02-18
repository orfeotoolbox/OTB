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

#ifndef MainProcessingParameters_h
#define MainProcessingParameters_h

#include <iostream>
#include <sstream>
#include <EnvisatAsar/EnvisatAsarRecord.h>

namespace ossimplugins
{
/**
 * @ingroup MainProcessingParametersRecord
 * @brief This class is able to read the ASAR MainProcessingParameters record
 */
class MainProcessingParameters : public EnvisatAsarRecord
{
public:
  /**
   * @brief Constructor
   */
  MainProcessingParameters();

  /**
   * @brief Destructor
   */
  virtual ~MainProcessingParameters();

  /**
   * @brief This function writes the MainProcessingParameters in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const MainProcessingParameters& data);

  /**
   * @brief This function reads a MainProcessingParameters from a stream
   */
  friend std::istream& operator>>(std::istream& is, MainProcessingParameters& data);

  /**
   * @brief Copy constructor
   */
  MainProcessingParameters(const MainProcessingParameters& rhs);

  /**
   * @brief Copy operator
   */
  MainProcessingParameters& operator=(const MainProcessingParameters& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  EnvisatAsarRecord* Instanciate()
  {
    return new MainProcessingParameters();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  EnvisatAsarRecord* Clone()
  {
    return new MainProcessingParameters(*this);
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
  * @brief first_zero_doppler_time
  */
  std::string   get_first_zero_doppler_time()
  {
    return _first_zero_doppler_time;
  };
  /**
  * @brief attach_flag
  */
  bool   get_attach_flag()
  {
    return _attach_flag;
  };
  /**
  * @brief last_zero_doppler_time
  */
  std::string   get_last_zero_doppler_time()
  {
    return _last_zero_doppler_time;
  };
  /**
  * @brief work_order_id
  */
  std::string   get_work_order_id()
  {
    return _work_order_id;
  };
  /**
  * @brief time_diff
  */
  float  get_time_diff()
  {
    return _time_diff;
  };
  /**
  * @brief swath_num
  */
  std::string   get_swath_num()
  {
    return _swath_num;
  };
  /**
  * @brief range_spacing
  */
  float  get_range_spacing()
  {
    return _range_spacing;
  };
  /**
  * @brief azimut_spacing
  */
  float  get_azimut_spacing()
  {
    return _azimut_spacing;
  };
  /**
  * @brief line_time_interval
  */
  float  get_line_time_interval()
  {
    return _line_time_interval;
  };
  /**
  * @brief num_output_lines
  */
  unsigned int  get_num_output_lines()
  {
    return _num_output_lines;
  };
  /**
  * @brief num_samples_per_line
  */
  unsigned int  get_num_samples_per_line()
  {
    return _num_samples_per_line;
  };
  /**
  * @brief data_type
  */
  std::string   get_data_type()
  {
    return _data_type;
  };
  /**
  * @brief num_range_lines_per_burst
  */
  unsigned int  get_num_range_lines_per_burst()
  {
    return _num_range_lines_per_burst;
  };
  /**
  * @brief time_diff_zero_doppler
  */
  float  get_time_diff_zero_doppler()
  {
    return _time_diff_zero_doppler;
  };
  /**
  * @brief data_analysis_flag
  */
  bool   get_data_analysis_flag()
  {
    return _data_analysis_flag;
  };
  /**
  * @brief ant_elev_coor_flag
  */
  bool   get_ant_elev_coor_flag()
  {
    return _ant_elev_coor_flag;
  };
  /**
  * @brief chirp_extract_flag
  */
  bool   get_chirp_extract_flag()
  {
    return _chirp_extract_flag;
  };
  /**
  * @brief srgr_flag
  */
  bool   get_srgr_flag()
  {
    return _srgr_flag;
  };
  /**
  * @brief dop_cen_flag
  */
  bool   get_dop_cen_flag()
  {
    return _dop_cen_flag;
  };
  /**
  * @brief dop_amb_flag
  */
  bool   get_dop_amb_flag()
  {
    return _dop_amb_flag;
  };
  /**
  * @brief range_spread_comp_flag
  */
  bool   get_range_spread_comp_flag()
  {
    return _range_spread_comp_flag;
  };
  /**
  * @brief detected_flag
  */
  bool   get_detected_flag()
  {
    return _detected_flag;
  };
  /**
  * @brief look_sum_flag
  */
  bool   get_look_sum_flag()
  {
    return _look_sum_flag;
  };
  /**
  * @brief rms_equal_flag
  */
  bool   get_rms_equal_flag()
  {
    return _rms_equal_flag;
  };
  /**
  * @brief ant_scal_flag
  */
  bool   get_ant_scal_flag()
  {
    return _ant_scal_flag;
  };
  /**
  * @brief vga_com_echo_flag
  */
  bool   get_vga_com_echo_flag()
  {
    return _vga_com_echo_flag;
  };
  /**
  * @brief vga_com_cal_flag
  */
  bool   get_vga_com_cal_flag()
  {
    return _vga_com_cal_flag;
  };
  /**
  * @brief vga_com_nom_time_flag
  */
  bool   get_vga_com_nom_time_flag()
  {
    return _vga_com_nom_time_flag;
  };
  /**
  * @brief gm_rng_comp_inv_filter_flag
  */
  bool   get_gm_rng_comp_inv_filter_flag()
  {
    return _gm_rng_comp_inv_filter_flag;
  };
  /**
  * @brief raw_data_analysis
  */
  std::string   get_raw_data_analysis()
  {
    return _raw_data_analysis;
  };
  /**
  * @brief start_time_mds1
  */
  std::string   get_start_time_mds1()
  {
    return _start_time_mds1;
  };
  /**
  * @brief start_time_mds2
  */
  std::string   get_start_time_mds2()
  {
    return _start_time_mds2;
  };
  /**
  * @brief parameter_code
  */
  std::string   get_parameter_code()
  {
    return _parameter_code;
  };
  /**
  * @brief errors_counters
  */
  std::string   get_errors_counters()
  {
    return _errors_counters;
  };
  /**
  * @brief image_parameters  block1
  */
  std::string   get_image_parameters1()
  {
    return _image_parameters1;
  };
  /**
  * @brief prf_values[5]
  */
  float*   get_prf_values()
  {
    return _prf_values;
  };
  /**
  * @brief prf (i.e prf_values[0])
  */
  float   get_prf()
  {
    return _prf_values[0];
  };
  /**
  * @brief image_parameters  block2
  */
  std::string   get_image_parameters2()
  {
    return _image_parameters2;
  };
  /**
  * @brief first_proc_range_samp
  */
  unsigned int  get_first_proc_range_samp()
  {
    return _first_proc_range_samp;
  };
  /**
  * @brief range_ref
  */
  float  get_range_ref()
  {
    return _range_ref;
  };
  /**
  * @brief range_samp_rate
  */
  float  get_range_samp_rate()
  {
    return _range_samp_rate;
  };
  /**
  * @brief radar_freq
  */
  float  get_radar_freq()
  {
    return _radar_freq;
  };
  /**
  * @brief num_looks_range
  */
  unsigned short   get_num_looks_range()
  {
    return _num_looks_range;
  };
  /**
  * @brief filter_range
  */
  std::string   get_filter_range()
  {
    return _filter_range;
  };
  /**
  * @brief filter_coef_range
  */
  float  get_filter_coef_range()
  {
    return _filter_coef_range;
  };
  /**
  * @brief bandwidth
  */
  std::string   get_bandwidth()
  {
    return _bandwidth;
  };
  /**
  * @brief nominal_chirp
  */
  std::string   get_nominal_chirp()
  {
    return _nominal_chirp;
  };
  /**
  * @brief num_lines_proc
  */
  unsigned int  get_num_lines_proc()
  {
    return _num_lines_proc;
  };
  /**
  * @brief num_look_az
  */
  unsigned short   get_num_look_az()
  {
    return _num_look_az;
  };
  /**
  * @brief look_bw_az
  */
  float  get_look_bw_az()
  {
    return _look_bw_az;
  };
  /**
  * @brief to_bw_az
  */
  float  get_to_bw_az()
  {
    return _to_bw_az;
  };
  /**
  * @brief filter_az
  */
  std::string   get_filter_az()
  {
    return _filter_az;
  };
  /**
  * @brief filter_coef_az
  */
  float  get_filter_coef_az()
  {
    return _filter_coef_az;
  };
  /**
  * @brief az_fm_rate[3]
  */
  float*  get_az_fm_rate()
  {
    return _az_fm_rate;
  };
  /**
  * @brief ax_fm_origin
  */
  float  get_ax_fm_origin()
  {
    return _ax_fm_origin;
  };
  /**
  * @brief dop_amb_coef
  */
  float  get_dop_amb_coef()
  {
    return _dop_amb_coef;
  };
  /**
  * @brief calibration_factors
  */
  std::string   get_calibration_factors()
  {
    return _calibration_factors;
  };
  /**
  * @brief noise_estimation
  */
  std::string   get_noise_estimation()
  {
    return _noise_estimation;
  };
  /**
  * @brief output_statistics
  */
  std::string   get_output_statistics()
  {
    return _output_statistics;
  };
  /**
  * @brief avg_scene_height_ellpsoid
  */
  float  get_avg_scene_height_ellpsoid()
  {
    return _avg_scene_height_ellpsoid;
  };
  /**
  * @brief echo_comp
  */
  std::string   get_echo_comp()
  {
    return _echo_comp;
  };
  /**
  * @brief echo_comp_ratio
  */
  std::string   get_echo_comp_ratio()
  {
    return _echo_comp_ratio;
  };
  /**
  * @brief init_cal_comp
  */
  std::string   get_init_cal_comp()
  {
    return _init_cal_comp;
  };
  /**
  * @brief init_cal_ratio
  */
  std::string   get_init_cal_ratio()
  {
    return _init_cal_ratio;
  };
  /**
  * @brief per_cal_comp
  */
  std::string   get_per_cal_comp()
  {
    return _per_cal_comp;
  };
  /**
  * @brief per_cal_ratio
  */
  std::string   get_per_cal_ratio()
  {
    return _per_cal_ratio;
  };
  /**
  * @brief noise_comp
  */
  std::string   get_noise_comp()
  {
    return _noise_comp;
  };
  /**
  * @brief noise_comp_ratio
  */
  std::string   get_noise_comp_ratio()
  {
    return _noise_comp_ratio;
  };
  /**
  * @brief beam_overlap[4]
  */
  unsigned int* get_beam_overlap()
  {
    return _beam_overlap;
  };
  /**
  * @brief beam_param[4]
  */
  float* get_beam_param()
  {
    return _beam_param;
  };
  /**
  * @brief lines_per_burst[5]
  */
  unsigned int*  get_lines_per_burst()
  {
    return _lines_per_burst;
  };
  /**
  * @brief time_first_SS1_echo
  */
  std::string   get_time_first_SS1_echo()
  {
    return _time_first_SS1_echo;
  };
  /**
  * @brief state_vector_time_1 day
  */
  int get_state_vector_time_1_day()
  {
    return _state_vector_time_1_day;
  };
  /**
  * @brief state_vector_time_1 sec
  */
  unsigned int get_state_vector_time_1_sec()
  {
    return _state_vector_time_1_sec;
  };
  /**
  * @brief state_vector_time_1 microsec
  */
  unsigned int get_state_vector_time_1_microsec()
  {
    return _state_vector_time_1_microsec;
  };
  /**
  * @brief x_pos_1
  */
  int   get_x_pos_1()
  {
    return _x_pos_1;
  };
  /**
  * @brief y_pos_1
  */
  int   get_y_pos_1()
  {
    return _y_pos_1;
  };
  /**
  * @brief z_pos_1
  */
  int   get_z_pos_1()
  {
    return _z_pos_1;
  };
  /**
  * @brief x_vel_1
  */
  int   get_x_vel_1()
  {
    return _x_vel_1;
  };
  /**
  * @brief y_vel_1
  */
  int   get_y_vel_1()
  {
    return _y_vel_1;
  };
  /**
  * @brief z_vel_1
  */
  int   get_z_vel_1()
  {
    return _z_vel_1;
  };
  /**
  * @brief state_vector_time_2 day
  */
  int get_state_vector_time_2_day()
  {
    return _state_vector_time_2_day;
  };
  /**
  * @brief state_vector_time_2 sec
  */
  unsigned int get_state_vector_time_2_sec()
  {
    return _state_vector_time_2_sec;
  };
  /**
  * @brief state_vector_time_1 microsec
  */
  unsigned int get_state_vector_time_2_microsec()
  {
    return _state_vector_time_2_microsec;
  };
  /**
  * @brief x_pos_2
  */
  int   get_x_pos_2()
  {
    return _x_pos_2;
  };
  /**
  * @brief y_pos_2
  */
  int   get_y_pos_2()
  {
    return _y_pos_2;
  };
  /**
  * @brief z_pos_2
  */
  int   get_z_pos_2()
  {
    return _z_pos_2;
  };
  /**
  * @brief x_vel_2
  */
  int   get_x_vel_2()
  {
    return _x_vel_2;
  };
  /**
  * @brief y_vel_2
  */
  int   get_y_vel_2()
  {
    return _y_vel_2;
  };
  /**
  * @brief z_vel_2
  */
  int   get_z_vel_2()
  {
    return _z_vel_2;
  };
  /**
  * @brief state_vector_time_3 day
  */
  int get_state_vector_time_3_day()
  {
    return _state_vector_time_3_day;
  };
  /**
  * @brief state_vector_time_3 sec
  */
  unsigned int get_state_vector_time_3_sec()
  {
    return _state_vector_time_3_sec;
  };
  /**
  * @brief state_vector_time_3 microsec
  */
  unsigned int get_state_vector_time_3_microsec()
  {
    return _state_vector_time_3_microsec;
  };
  /**
  * @brief x_pos_3
  */
  int   get_x_pos_3()
  {
    return _x_pos_3;
  };
  /**
  * @brief y_pos_3
  */
  int   get_y_pos_3()
  {
    return _y_pos_3;
  };
  /**
  * @brief z_pos_3
  */
  int   get_z_pos_3()
  {
    return _z_pos_3;
  };
  /**
  * @brief x_vel_3
  */
  int   get_x_vel_3()
  {
    return _x_vel_3;
  };
  /**
  * @brief y_vel_3
  */
  int   get_y_vel_3()
  {
    return _y_vel_3;
  };
  /**
  * @brief z_vel_3
  */
  int   get_z_vel_3()
  {
    return _z_vel_3;
  };
  /**
  * @brief state_vector_time_4 day
  */
  int get_state_vector_time_4_day()
  {
    return _state_vector_time_4_day;
  };
  /**
  * @brief state_vector_time_1 sec
  */
  unsigned int get_state_vector_time_4_sec()
  {
    return _state_vector_time_4_sec;
  };
  /**
  * @brief state_vector_time_4 microsec
  */
  unsigned int get_state_vector_time_4_microsec()
  {
    return _state_vector_time_4_microsec;
  };
  /**
  * @brief x_pos_4
  */
  int   get_x_pos_4()
  {
    return _x_pos_4;
  };
  /**
  * @brief y_pos_4
  */
  int   get_y_pos_4()
  {
    return _y_pos_4;
  };
  /**
  * @brief z_pos_4
  */
  int   get_z_pos_4()
  {
    return _z_pos_4;
  };
  /**
  * @brief x_vel_4
  */
  int   get_x_vel_4()
  {
    return _x_vel_4;
  };
  /**
  * @brief y_vel_4
  */
  int   get_y_vel_4()
  {
    return _y_vel_4;
  };
  /**
  * @brief z_vel_4
  */
  int   get_z_vel_4()
  {
    return _z_vel_4;
  };
  /**
  * @brief state_vector_time_5 day
  */
  int get_state_vector_time_5_day()
  {
    return _state_vector_time_5_day;
  };
  /**
  * @brief state_vector_time_5 sec
  */
  unsigned int get_state_vector_time_5_sec()
  {
    return _state_vector_time_5_sec;
  };
  /**
  * @brief state_vector_time_5 microsec
  */
  unsigned int get_state_vector_time_5_microsec()
  {
    return _state_vector_time_5_microsec;
  };
  /**
  * @brief x_pos_5
  */
  int   get_x_pos_5()
  {
    return _x_pos_5;
  };
  /**
  * @brief y_pos_5
  */
  int   get_y_pos_5()
  {
    return _y_pos_5;
  };
  /**
  * @brief z_pos_5
  */
  int   get_z_pos_5()
  {
    return _z_pos_5;
  };
  /**
  * @brief x_vel_5
  */
  int   get_x_vel_5()
  {
    return _x_vel_5;
  };
  /**
  * @brief y_vel_5
  */
  int   get_y_vel_5()
  {
    return _y_vel_5;
  };
  /**
  * @brief z_vel_5
  */
  int   get_z_vel_5()
  {
    return _z_vel_5;
  };

  /**
  * @brief first_zero_doppler_time
  */
  std::string   _first_zero_doppler_time;
  /**
  * @brief attach_flag
  */
  bool   _attach_flag;
  /**
  * @brief last_zero_doppler_time
  */
  std::string   _last_zero_doppler_time;
  /**
  * @brief work_order_id
  */
  std::string   _work_order_id;
  /**
  * @brief time_diff
  */
  float  _time_diff;
  /**
  * @brief swath_num
  */
  std::string   _swath_num;
  /**
  * @brief range_spacing
  */
  float  _range_spacing;
  /**
  * @brief azimut_spacing
  */
  float  _azimut_spacing;
  /**
  * @brief line_time_interval
  */
  float  _line_time_interval;
  /**
  * @brief num_output_lines
  */
  unsigned int   _num_output_lines;
  /**
  * @brief num_samples_per_line
  */
  unsigned int   _num_samples_per_line;
  /**
  * @brief data_type
  */
  std::string   _data_type;
  /**
  * @brief num_range_lines_per_burst
  */
  unsigned int   _num_range_lines_per_burst;
  /**
  * @brief time_diff_zero_doppler
  */
  float  _time_diff_zero_doppler;
  /**
  * @brief data_analysis_flag
  */
  bool   _data_analysis_flag;
  /**
  * @brief ant_elev_coor_flag
  */
  bool   _ant_elev_coor_flag;
  /**
  * @brief chirp_extract_flag
  */
  bool   _chirp_extract_flag;
  /**
  * @brief srgr_flag
  */
  bool   _srgr_flag;
  /**
  * @brief dop_cen_flag
  */
  bool   _dop_cen_flag;
  /**
  * @brief dop_amb_flag
  */
  bool   _dop_amb_flag;
  /**
  * @brief range_spread_comp_flag
  */
  bool   _range_spread_comp_flag;
  /**
  * @brief detected_flag
  */
  bool   _detected_flag;
  /**
  * @brief look_sum_flag
  */
  bool   _look_sum_flag;
  /**
  * @brief rms_equal_flag
  */
  bool   _rms_equal_flag;
  /**
  * @brief ant_scal_flag
  */
  bool   _ant_scal_flag;
  /**
  * @brief vga_com_echo_flag
  */
  bool   _vga_com_echo_flag;
  /**
  * @brief vga_com_cal_flag
  */
  bool   _vga_com_cal_flag;
  /**
  * @brief vga_com_nom_time_flag
  */
  bool   _vga_com_nom_time_flag;
  /**
  * @brief gm_rng_comp_inv_filter_flag
  */
  bool   _gm_rng_comp_inv_filter_flag;
  /**
  * @brief raw_data_analysis
  */
  std::string   _raw_data_analysis;
  /**
  * @brief start_time_mds1
  */
  std::string   _start_time_mds1;
  /**
  * @brief start_time_mds2
  */
  std::string   _start_time_mds2;
  /**
  * @brief parameter_code
  */
  std::string   _parameter_code;
  /**
  * @brief errors_counters
  */
  std::string   _errors_counters;
  /**
  * @brief image_parameters block 1
  */
  std::string   _image_parameters1;
  /**
  * @brief PRF
  */
  float  _prf_values[5];
  /**
  * @brief image_parameters block 2
  */
  std::string   _image_parameters2;
  /**
  * @brief first_proc_range_samp
  */
  unsigned int   _first_proc_range_samp;
  /**
  * @brief range_ref
  */
  float  _range_ref;
  /**
  * @brief range_samp_rate
  */
  float  _range_samp_rate;
  /**
  * @brief radar_freq
  */
  float  _radar_freq;
  /**
  * @brief num_looks_range
  */
  unsigned short   _num_looks_range;
  /**
  * @brief filter_range
  */
  std::string   _filter_range;
  /**
  * @brief filter_coef_range
  */
  float  _filter_coef_range;
  /**
  * @brief bandwidth
  */
  std::string   _bandwidth;
  /**
  * @brief nominal_chirp
  */
  std::string   _nominal_chirp;
  /**
  * @brief num_lines_proc
  */
  unsigned int   _num_lines_proc;
  /**
  * @brief num_look_az
  */
  unsigned short   _num_look_az;
  /**
  * @brief look_bw_az
  */
  float  _look_bw_az;
  /**
  * @brief to_bw_az
  */
  float  _to_bw_az;
  /**
  * @brief filter_az
  */
  std::string   _filter_az;
  /**
  * @brief filter_coef_az
  */
  float  _filter_coef_az;
  /**
  * @brief az_fm_rate[3]
  */
  float  _az_fm_rate[3];
  /**
  * @brief ax_fm_origin
  */
  float  _ax_fm_origin;
  /**
  * @brief dop_amb_coef
  */
  float  _dop_amb_coef;
  /**
  * @brief calibration_factors
  */
  std::string   _calibration_factors;
  /**
  * @brief noise_estimation
  */
  std::string   _noise_estimation;
  /**
  * @brief output_statistics
  */
  std::string   _output_statistics;
  /**
  * @brief avg_scene_height_ellpsoid
  */
  float  _avg_scene_height_ellpsoid;
  /**
  * @brief echo_comp
  */
  std::string   _echo_comp;
  /**
  * @brief echo_comp_ratio
  */
  std::string   _echo_comp_ratio;
  /**
  * @brief init_cal_comp
  */
  std::string   _init_cal_comp;
  /**
  * @brief init_cal_ratio
  */
  std::string   _init_cal_ratio;
  /**
  * @brief per_cal_comp
  */
  std::string   _per_cal_comp;
  /**
  * @brief per_cal_ratio
  */
  std::string   _per_cal_ratio;
  /**
  * @brief noise_comp
  */
  std::string   _noise_comp;
  /**
  * @brief noise_comp_ratio
  */
  std::string   _noise_comp_ratio;
  /**
  * @brief beam_overlap[4]
  */
  unsigned int   _beam_overlap[4];
  /**
  * @brief beam_overlap[4]
  */
  float   _beam_param[4];
  /**
  * @brief lines_per_burst[5]
  */
  unsigned int   _lines_per_burst[5];
  /**
  * @brief time_first_SS1_echo
  */
  std::string   _time_first_SS1_echo;
  /**
  * @brief state_vector_time_1_day
  */
  int   _state_vector_time_1_day;
  /**
  * @brief state_vector_time_1_sec
  */
  unsigned int   _state_vector_time_1_sec;
  /**
  * @brief state_vector_time_1_microsec
  */
  unsigned int   _state_vector_time_1_microsec;
  /**
  * @brief x_pos_1
  */
  int   _x_pos_1;
  /**
  * @brief y_pos_1
  */
  int   _y_pos_1;
  /**
  * @brief z_pos_1
  */
  int   _z_pos_1;
  /**
  * @brief x_vel_1
  */
  int   _x_vel_1;
  /**
  * @brief y_vel_1
  */
  int   _y_vel_1;
  /**
  * @brief z_vel_1
  */
  int   _z_vel_1;
  /**
  * @brief state_vector_time_2_day
  */
  int   _state_vector_time_2_day;
  /**
  * @brief state_vector_time_2_sec
  */
  unsigned int   _state_vector_time_2_sec;
  /**
  * @brief state_vector_time_2_microsec
  */
  unsigned int   _state_vector_time_2_microsec;
  /**
  * @brief x_pos_2
  */
  int   _x_pos_2;
  /**
  * @brief y_pos_2
  */
  int   _y_pos_2;
  /**
  * @brief z_pos_2
  */
  int   _z_pos_2;
  /**
  * @brief x_vel_2
  */
  int   _x_vel_2;
  /**
  * @brief y_vel_2
  */
  int   _y_vel_2;
  /**
  * @brief z_vel_2
  */
  int   _z_vel_2;
  /**
  * @brief state_vector_time_3_day
  */
  int   _state_vector_time_3_day;
  /**
  * @brief state_vector_time_3_sec
  */
  unsigned int   _state_vector_time_3_sec;
  /**
  * @brief state_vector_time_3_microsec
  */
  unsigned int   _state_vector_time_3_microsec;
  /**
  * @brief x_pos_3
  */
  int   _x_pos_3;
  /**
  * @brief y_pos_3
  */
  int   _y_pos_3;
  /**
  * @brief z_pos_3
  */
  int   _z_pos_3;
  /**
  * @brief x_vel_3
  */
  int   _x_vel_3;
  /**
  * @brief y_vel_3
  */
  int   _y_vel_3;
  /**
  * @brief z_vel_3
  */
  int   _z_vel_3;
  /**
  * @brief state_vector_time_4_day
  */
  int   _state_vector_time_4_day;
  /**
  * @brief state_vector_time_4_sec
  */
  unsigned int   _state_vector_time_4_sec;
  /**
  * @brief state_vector_time_4_microsec
  */
  unsigned int   _state_vector_time_4_microsec;
  /**
  * @brief x_pos_4
  */
  int   _x_pos_4;
  /**
  * @brief y_pos_4
  */
  int   _y_pos_4;
  /**
  * @brief z_pos_4
  */
  int   _z_pos_4;
  /**
  * @brief x_vel_4
  */
  int   _x_vel_4;
  /**
  * @brief y_vel_4
  */
  int   _y_vel_4;
  /**
  * @brief z_vel_4
  */
  int   _z_vel_4;
  /**
  * @brief state_vector_time_5_day
  */
  int   _state_vector_time_5_day;
  /**
  * @brief state_vector_time_5_sec
  */
  unsigned int   _state_vector_time_5_sec;
  /**
  * @brief state_vector_time_5_microsec
  */
  unsigned int   _state_vector_time_5_microsec;
  /**
  * @brief x_pos_5
  */
  int   _x_pos_5;
  /**
  * @brief y_pos_5
  */
  int   _y_pos_5;
  /**
  * @brief z_pos_5
  */
  int   _z_pos_5;
  /**
  * @brief x_vel_5
  */
  int   _x_vel_5;
  /**
  * @brief y_vel_5
  */
  int   _y_vel_5;
  /**
  * @brief z_vel_5
  */
  int   _z_vel_5;


private:

};
}
#endif


