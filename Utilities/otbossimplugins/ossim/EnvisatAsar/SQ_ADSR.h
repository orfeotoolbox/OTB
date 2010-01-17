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

#ifndef SQ_ADSR_h
#define SQ_ADSR_h

#include <iostream>
#include <EnvisatAsar/EnvisatAsarRecord.h>

namespace ossimplugins
{
/**
 * @ingroup SQ_ADSRRecord
 * @brief This class is able to read the ASAR SQ_ADSR record
 */
class SQ_ADSR : public EnvisatAsarRecord
{
public:
  /**
   * @brief Constructor
   */
  SQ_ADSR();

  /**
   * @brief Destructor
   */
  virtual ~SQ_ADSR();

  /**
   * @brief This function write the SQ_ADSR in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const SQ_ADSR& data);

  /**
   * @brief This function read a SQ_ADSR from a stream
   */
  friend std::istream& operator>>(std::istream& is, SQ_ADSR& data);

  /**
   * @brief Copy constructor
   */
  SQ_ADSR(const SQ_ADSR& rhs);

  /**
   * @brief Copy operator
   */
  SQ_ADSR& operator=(const SQ_ADSR& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  EnvisatAsarRecord* Instanciate()
  {
    return new SQ_ADSR();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  EnvisatAsarRecord* Clone()
  {
    return new SQ_ADSR(*this);
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
  * @brief zero_doppler_time
  */
  std::string   get_zero_doppler_time()
  {
    return _zero_doppler_time;
  };
  /**
  * @brief attach_flag
  */
  bool   get_attach_flag()
  {
    return _attach_flag;
  };
  /**
  * @brief input_mean_flag
  */
  bool   get_input_mean_flag()
  {
    return _input_mean_flag;
  };
  /**
  * @brief input_std_dev_flag
  */
  bool   get_input_std_dev_flag()
  {
    return _input_std_dev_flag;
  };
  /**
  * @brief input_gaps_flag
  */
  bool   get_input_gaps_flag()
  {
    return _input_gaps_flag;
  };
  /**
  * @brief input_missing_lines_flag
  */
  bool   get_input_missing_lines_flag()
  {
    return _input_missing_lines_flag;
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
  * @brief output_mean_flag
  */
  bool   get_output_mean_flag()
  {
    return _output_mean_flag;
  };
  /**
  * @brief output_std_dev_flag
  */
  bool   get_output_std_dev_flag()
  {
    return _output_std_dev_flag;
  };
  /**
  * @brief chirp_flag
  */
  bool   get_chirp_flag()
  {
    return _chirp_flag;
  };
  /**
  * @brief missing_data_set_flag
  */
  bool   get_missing_data_set_flag()
  {
    return _missing_data_set_flag;
  };
  /**
  * @brief invalid_downlink_flag
  */
  bool   get_invalid_downlink_flag()
  {
    return _invalid_downlink_flag;
  };
  /**
  * @brief thres_chirp_broadening
  */
  float   get_thres_chirp_broadening()
  {
    return _thres_chirp_broadening;
  };
  /**
  * @brief thresh_chirp_sidelobe
  */
  float   get_thresh_chirp_sidelobe()
  {
    return _thresh_chirp_sidelobe;
  };
  /**
  * @brief thresh_chirp_islr
  */
  float   get_thresh_chirp_islr()
  {
    return _thresh_chirp_islr;
  };
  /**
  * @brief thresh_input_mean
  */
  float   get_thresh_input_mean()
  {
    return _thresh_input_mean;
  };
  /**
  * @brief exp_input_mean
  */
  float   get_exp_input_mean()
  {
    return _exp_input_mean;
  };
  /**
  * @brief thresh_input_std_dev
  */
  float   get_thresh_input_std_dev()
  {
    return _thresh_input_std_dev;
  };
  /**
  * @brief exp_input_std_dev
  */
  float   get_exp_input_std_dev()
  {
    return _exp_input_std_dev;
  };
  /**
  * @brief thresh_dop_cen
  */
  float   get_thresh_dop_cen()
  {
    return _thresh_dop_cen;
  };
  /**
  * @brief thresh_dop_amb
  */
  float   get_thresh_dop_amb()
  {
    return _thresh_dop_amb;
  };
  /**
  * @brief thresh_output_mean
  */
  float   get_thresh_output_mean()
  {
    return _thresh_output_mean;
  };
  /**
  * @brief exp_output_mean
  */
  float   get_exp_output_mean()
  {
    return _exp_output_mean;
  };
  /**
  * @brief thresh_output_std_dev
  */
  float   get_thresh_output_std_dev()
  {
    return _thresh_output_std_dev;
  };
  /**
  * @brief exp_output_std_dev
  */
  float   get_exp_output_std_dev()
  {
    return _exp_output_std_dev;
  };
  /**
  * @brief thresh_input_missing_lines
  */
  float   get_thresh_input_missing_lines()
  {
    return _thresh_input_missing_lines;
  };
  /**
  * @brief thresh_input_gaps
  */
  float   get_thresh_input_gaps()
  {
    return _thresh_input_gaps;
  };
  /**
  * @brief lines_per_gaps
  */
  unsigned int   get_lines_per_gaps()
  {
    return _lines_per_gaps;
  };
  /**
  * @brief input_mean[2]
  */
  float*   get_input_mean()
  {
    return _input_mean;
  };
  /**
  * @brief input_std_dev[2]
  */
  float*   get_input_std_dev()
  {
    return _input_std_dev;
  };
  /**
  * @brief num_gaps
  */
  float   get_num_gaps()
  {
    return _num_gaps;
  };
  /**
  * @brief num_missing_lines
  */
  float   get_num_missing_lines()
  {
    return _num_missing_lines;
  };
  /**
  * @brief output_mean[2]
  */
  float*   get_output_mean()
  {
    return _output_mean;
  };
  /**
  * @brief output_std_dev[2]
  */
  float*   get_output_std_dev()
  {
    return _output_std_dev;
  };
  /**
  * @brief tot_errors
  */
  unsigned int   get_tot_errors()
  {
    return _tot_errors;
  };


  /**
   * @brief zero_doppler_time
   */
  std::string   _zero_doppler_time;
  /**
   * @brief attach_flag
   */
  bool   _attach_flag;
  /**
   * @brief input_mean_flag
   */
  bool   _input_mean_flag;
  /**
   * @brief input_std_dev_flag
   */
  bool   _input_std_dev_flag;
  /**
   * @brief input_gaps_flag
   */
  bool   _input_gaps_flag;
  /**
   * @brief input_missing_lines_flag
   */
  bool   _input_missing_lines_flag;
  /**
   * @brief dop_cen_flag
   */
  bool   _dop_cen_flag;
  /**
   * @brief dop_amb_flag
   */
  bool   _dop_amb_flag;
  /**
   * @brief output_mean_flag
   */
  bool   _output_mean_flag;
  /**
   * @brief output_std_dev_flag
   */
  bool   _output_std_dev_flag;
  /**
   * @brief chirp_flag
   */
  bool   _chirp_flag;
  /**
   * @brief missing_data_set_flag
   */
  bool   _missing_data_set_flag;
  /**
   * @brief invalid_downlink_flag
   */
  bool   _invalid_downlink_flag;
  /**
   * @brief thres_chirp_broadening
   */
  float   _thres_chirp_broadening;
  /**
   * @brief thresh_chirp_sidelobe
   */
  float   _thresh_chirp_sidelobe;
  /**
   * @brief thresh_chirp_islr
   */
  float   _thresh_chirp_islr;
  /**
   * @brief thresh_input_mean
   */
  float   _thresh_input_mean;
  /**
   * @brief exp_input_mean
   */
  float   _exp_input_mean;
  /**
   * @brief thresh_input_std_dev
   */
  float   _thresh_input_std_dev;
  /**
   * @brief exp_input_std_dev
   */
  float   _exp_input_std_dev;
  /**
   * @brief thresh_dop_cen
   */
  float   _thresh_dop_cen;
  /**
   * @brief thresh_dop_amb
   */
  float   _thresh_dop_amb;
  /**
   * @brief thresh_output_mean
   */
  float   _thresh_output_mean;
  /**
   * @brief exp_output_mean
   */
  float   _exp_output_mean;
  /**
   * @brief thresh_output_std_dev
   */
  float   _thresh_output_std_dev;
  /**
   * @brief exp_output_std_dev
   */
  float   _exp_output_std_dev;
  /**
   * @brief thresh_input_missing_lines
   */
  float   _thresh_input_missing_lines;
  /**
   * @brief thresh_input_gaps
   */
  float   _thresh_input_gaps;
  /**
   * @brief lines_per_gaps
   */
  unsigned int   _lines_per_gaps;
  /**
   * @brief input_mean[2]
   */
  float   _input_mean[2];
  /**
   * @brief input_std_dev[2]
   */
  float   _input_std_dev[2];
  /**
   * @brief num_gaps
   */
  float   _num_gaps;
  /**
   * @brief num_missing_lines
   */
  float   _num_missing_lines;
  /**
   * @brief output_mean[2]
   */
  float   _output_mean[2];
  /**
   * @brief output_std_dev[2]
   */
  float   _output_std_dev[2];
  /**
   * @brief tot_errors
   */
  unsigned int   _tot_errors;


private:

};
}
#endif


