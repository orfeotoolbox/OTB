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

#ifndef mph_h
#define mph_h

#include <iostream>
#include <EnvisatAsar/EnvisatAsarRecord.h>

namespace ossimplugins
{

/**
 * @ingroup mphRecord
 * @brief This class is able to read the ASAR mph record of the leader file
 */
class mph : public EnvisatAsarRecord
{
public:
  /**
   * @brief Constructor
   */
  mph();

  /**
   * @brief Destructor
   */
  virtual ~mph();

  /**
   * @brief This function write the mph in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const mph& data);

  /**
   * @brief This function read a mph from a stream
   */
  friend std::istream& operator>>(std::istream& is, mph& data);

  /**
   * @brief Copy constructor
   */
  mph(const mph& rhs);

  /**
   * @brief Copy operator
   */
  mph& operator=(const mph& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  EnvisatAsarRecord* Instanciate()
  {
    return new mph();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  EnvisatAsarRecord* Clone()
  {
    return new mph(*this);
  };

  /**
   * @brief Read the class data from a stream
   */
  void Read(std::istream& is)
  {
    is>>*this;
  };

  /**
   * @brief Write the class to a stream
   */
  void Write(std::ostream& os)
  {
    os<<*this;
  };

  /**
  * @brief product_name
  */
  std::string   get_product_name()
  {
    return _product_name;
  };
  /**
  * @brief product
  */
  std::string   get_product()
  {
    return _product;
  };
  /**
  * @brief proc_stage_title
  */
  std::string   get_proc_stage_title()
  {
    return _proc_stage_title;
  };
  /**
  * @brief proc_stage
  */
  std::string   get_proc_stage()
  {
    return _proc_stage;
  };
  /**
  * @brief ref_doc_title
  */
  std::string   get_ref_doc_title()
  {
    return _ref_doc_title;
  };
  /**
  * @brief ref_doc
  */
  std::string   get_ref_doc()
  {
    return _ref_doc;
  };
  /**
  * @brief acq_stat_id_title
  */
  std::string   get_acq_stat_id_title()
  {
    return _acq_stat_id_title;
  };
  /**
  * @brief acq_station
  */
  std::string   get_acq_station()
  {
    return _acq_station;
  };
  /**
  * @brief proc_center_title
  */
  std::string   get_proc_center_title()
  {
    return _proc_center_title;
  };
  /**
  * @brief proc_center
  */
  std::string   get_proc_center()
  {
    return _proc_center;
  };
  /**
  * @brief proc_time_title
  */
  std::string   get_proc_time_title()
  {
    return _proc_time_title;
  };
  /**
  * @brief proc_time
  */
  std::string   get_proc_time()
  {
    return _proc_time;
  };
  /**
  * @brief soft_version_title
  */
  std::string   get_soft_version_title()
  {
    return _soft_version_title;
  };
  /**
  * @brief soft_version
  */
  std::string   get_soft_version()
  {
    return _soft_version;
  };
  /**
  * @brief sensing_start_title
  */
  std::string   get_sensing_start_title()
  {
    return _sensing_start_title;
  };
  /**
  * @brief sensing_start
  */
  std::string   get_sensing_start()
  {
    return _sensing_start;
  };
  /**
  * @brief sensing_stop_title
  */
  std::string   get_sensing_stop_title()
  {
    return _sensing_stop_title;
  };
  /**
  * @brief sensing_stop
  */
  std::string   get_sensing_stop()
  {
    return _sensing_stop;
  };
  /**
  * @brief phase_title
  */
  std::string   get_phase_title()
  {
    return _phase_title;
  };
  /**
  * @brief phase
  */
  std::string   get_phase()
  {
    return _phase;
  };
  /**
  * @brief cycle_title
  */
  std::string   get_cycle_title()
  {
    return _cycle_title;
  };
  /**
  * @brief cycle
  */
  std::string   get_cycle()
  {
    return _cycle;
  };
  /**
  * @brief relative_orbit_title
  */
  std::string   get_relative_orbit_title()
  {
    return _relative_orbit_title;
  };
  /**
  * @brief rel_orbit
  */
  std::string   get_rel_orbit()
  {
    return _rel_orbit;
  };
  /**
  * @brief absolute_orbit_title
  */
  std::string   get_absolute_orbit_title()
  {
    return _absolute_orbit_title;
  };
  /**
  * @brief abs_orbit
  */
  std::string   get_abs_orbit()
  {
    return _abs_orbit;
  };
  /**
  * @brief state_vector_time_title
  */
  std::string   get_state_vector_time_title()
  {
    return _state_vector_time_title;
  };
  /**
  * @brief state_vector_time
  */
  std::string   get_state_vector_time()
  {
    return _state_vector_time;
  };
  /**
  * @brief delta_ut1_title
  */
  std::string   get_delta_ut1_title()
  {
    return _delta_ut1_title;
  };
  /**
  * @brief delta_ut1
  */
  std::string   get_delta_ut1()
  {
    return _delta_ut1;
  };
  /**
  * @brief delta_ut1_units
  */
  std::string   get_delta_ut1_units()
  {
    return _delta_ut1_units;
  };
  /**
  * @brief x_position_title
  */
  std::string   get_x_position_title()
  {
    return _x_position_title;
  };
  /**
  * @brief x_position
  */
  std::string   get_x_position()
  {
    return _x_position;
  };
  /**
  * @brief x_position_units
  */
  std::string   get_x_position_units()
  {
    return _x_position_units;
  };
  /**
  * @brief y_position_title
  */
  std::string   get_y_position_title()
  {
    return _y_position_title;
  };
  /**
  * @brief y_position
  */
  std::string   get_y_position()
  {
    return _y_position;
  };
  /**
  * @brief y_position_units
  */
  std::string   get_y_position_units()
  {
    return _y_position_units;
  };
  /**
  * @brief z_position_title
  */
  std::string   get_z_position_title()
  {
    return _z_position_title;
  };
  /**
  * @brief z_position
  */
  std::string   get_z_position()
  {
    return _z_position;
  };
  /**
  * @brief z_position_units
  */
  std::string   get_z_position_units()
  {
    return _z_position_units;
  };
  /**
  * @brief x_velocity_title
  */
  std::string   get_x_velocity_title()
  {
    return _x_velocity_title;
  };
  /**
  * @brief x_velocity
  */
  std::string   get_x_velocity()
  {
    return _x_velocity;
  };
  /**
  * @brief x_velocity_units
  */
  std::string   get_x_velocity_units()
  {
    return _x_velocity_units;
  };
  /**
  * @brief y_velocity_title
  */
  std::string   get_y_velocity_title()
  {
    return _y_velocity_title;
  };
  /**
  * @brief y_velocity
  */
  std::string   get_y_velocity()
  {
    return _y_velocity;
  };
  /**
  * @brief y_velocity_units
  */
  std::string   get_y_velocity_units()
  {
    return _y_velocity_units;
  };
  /**
  * @brief z_velocity_title
  */
  std::string   get_z_velocity_title()
  {
    return _z_velocity_title;
  };
  /**
  * @brief z_velocity
  */
  std::string   get_z_velocity()
  {
    return _z_velocity;
  };
  /**
  * @brief z_velocity_units
  */
  std::string   get_z_velocity_units()
  {
    return _z_velocity_units;
  };
  /**
  * @brief vector_source_title
  */
  std::string   get_vector_source_title()
  {
    return _vector_source_title;
  };
  /**
  * @brief vector_source
  */
  std::string   get_vector_source()
  {
    return _vector_source;
  };
  /**
  * @brief spare_4
  */
  std::string   get_spare_4()
  {
    return _spare_4;
  };
  /**
  * @brief utc_sbt_time_title
  */
  std::string   get_utc_sbt_time_title()
  {
    return _utc_sbt_time_title;
  };
  /**
  * @brief utc_sbt_time
  */
  std::string   get_utc_sbt_time()
  {
    return _utc_sbt_time;
  };
  /**
  * @brief sat_binary_time_title
  */
  std::string   get_sat_binary_time_title()
  {
    return _sat_binary_time_title;
  };
  /**
  * @brief sat_binary_time
  */
  std::string   get_sat_binary_time()
  {
    return _sat_binary_time;
  };
  /**
  * @brief clock_step_title
  */
  std::string   get_clock_step_title()
  {
    return _clock_step_title;
  };
  /**
  * @brief clock_step
  */
  std::string   get_clock_step()
  {
    return _clock_step;
  };
  /**
  * @brief clock_step_units
  */
  std::string   get_clock_step_units()
  {
    return _clock_step_units;
  };
  /**
  * @brief leap_utc_title
  */
  std::string   get_leap_utc_title()
  {
    return _leap_utc_title;
  };
  /**
  * @brief leap_utc
  */
  std::string   get_leap_utc()
  {
    return _leap_utc;
  };
  /**
  * @brief leap_sign_title
  */
  std::string   get_leap_sign_title()
  {
    return _leap_sign_title;
  };
  /**
  * @brief leap_sign
  */
  std::string   get_leap_sign()
  {
    return _leap_sign;
  };
  /**
  * @brief leap_err_title
  */
  std::string   get_leap_err_title()
  {
    return _leap_err_title;
  };
  /**
  * @brief leap_err
  */
  std::string   get_leap_err()
  {
    return _leap_err;
  };
  /**
  * @brief product_err_title
  */
  std::string   get_product_err_title()
  {
    return _product_err_title;
  };
  /**
  * @brief product_err
  */
  std::string   get_product_err()
  {
    return _product_err;
  };
  /**
  * @brief total_size_title
  */
  std::string   get_total_size_title()
  {
    return _total_size_title;
  };
  /**
  * @brief tot_size
  */
  std::string   get_tot_size()
  {
    return _tot_size;
  };
  /**
  * @brief total_size_units
  */
  std::string   get_total_size_units()
  {
    return _total_size_units;
  };
  /**
  * @brief sph_size_title
  */
  std::string   get_sph_size_title()
  {
    return _sph_size_title;
  };
  /**
  * @brief sph_size
  */
  std::string   get_sph_size()
  {
    return _sph_size;
  };
  /**
  * @brief sph_size_units
  */
  std::string   get_sph_size_units()
  {
    return _sph_size_units;
  };
  /**
  * @brief number_of_dsd_title
  */
  std::string   get_number_of_dsd_title()
  {
    return _number_of_dsd_title;
  };
  /**
  * @brief num_dsd
  */
  int   get_num_dsd()
  {
    return _num_dsd;
  };
  /**
  * @brief size_of_dsd_title
  */
  std::string   get_size_of_dsd_title()
  {
    return _size_of_dsd_title;
  };
  /**
  * @brief dsd_size
  */
  int   get_dsd_size()
  {
    return _dsd_size;
  };
  /**
  * @brief size_of_dsd_units
  */
  std::string   get_size_of_dsd_units()
  {
    return _size_of_dsd_units;
  };
  /**
  * @brief number_of_ds_att_title
  */
  std::string   get_number_of_ds_att_title()
  {
    return _number_of_ds_att_title;
  };
  /**
  * @brief num_data_sets
  */
  int   get_num_data_sets()
  {
    return _num_data_sets;
  };
  /**
  * @brief Check mph validity
  */
  bool is_valid()
  {
    if ((_product_name.compare("PRODUCT=") == 0) && (_number_of_dsd_title.compare("NUM_DSD=") == 0))
    {
      return true;
    } else return false;
  };


  /**
  * @brief product_name
  */
  std::string   _product_name;
  /**
  * @brief product
  */
  std::string   _product;
  /**
  * @brief proc_stage_title
  */
  std::string   _proc_stage_title;
  /**
  * @brief proc_stage
  */
  std::string   _proc_stage;
  /**
  * @brief ref_doc_title
  */
  std::string   _ref_doc_title;
  /**
  * @brief ref_doc
  */
  std::string   _ref_doc;
  /**
  * @brief acq_stat_id_title
  */
  std::string   _acq_stat_id_title;
  /**
  * @brief acq_station
  */
  std::string   _acq_station;
  /**
  * @brief proc_center_title
  */
  std::string   _proc_center_title;
  /**
  * @brief proc_center
  */
  std::string   _proc_center;
  /**
  * @brief proc_time_title
  */
  std::string   _proc_time_title;
  /**
  * @brief proc_time
  */
  std::string   _proc_time;
  /**
  * @brief soft_version_title
  */
  std::string   _soft_version_title;
  /**
  * @brief soft_version
  */
  std::string   _soft_version;
  /**
  * @brief sensing_start_title
  */
  std::string   _sensing_start_title;
  /**
  * @brief sensing_start
  */
  std::string   _sensing_start;
  /**
  * @brief sensing_stop_title
  */
  std::string   _sensing_stop_title;
  /**
  * @brief sensing_stop
  */
  std::string   _sensing_stop;
  /**
  * @brief phase_title
  */
  std::string   _phase_title;
  /**
  * @brief phase
  */
  std::string   _phase;
  /**
  * @brief cycle_title
  */
  std::string   _cycle_title;
  /**
  * @brief cycle
  */
  std::string   _cycle;
  /**
  * @brief relative_orbit_title
  */
  std::string   _relative_orbit_title;
  /**
  * @brief rel_orbit
  */
  std::string   _rel_orbit;
  /**
  * @brief absolute_orbit_title
  */
  std::string   _absolute_orbit_title;
  /**
  * @brief abs_orbit
  */
  std::string   _abs_orbit;
  /**
  * @brief state_vector_time_title
  */
  std::string   _state_vector_time_title;
  /**
  * @brief state_vector_time
  */
  std::string   _state_vector_time;
  /**
  * @brief delta_ut1_title
  */
  std::string   _delta_ut1_title;
  /**
  * @brief delta_ut1
  */
  std::string   _delta_ut1;
  /**
  * @brief delta_ut1_units
  */
  std::string   _delta_ut1_units;
  /**
  * @brief x_position_title
  */
  std::string   _x_position_title;
  /**
  * @brief x_position
  */
  std::string   _x_position;
  /**
  * @brief x_position_units
  */
  std::string   _x_position_units;
  /**
  * @brief y_position_title
  */
  std::string   _y_position_title;
  /**
  * @brief y_position
  */
  std::string   _y_position;
  /**
  * @brief y_position_units
  */
  std::string   _y_position_units;
  /**
  * @brief z_position_title
  */
  std::string   _z_position_title;
  /**
  * @brief z_position
  */
  std::string   _z_position;
  /**
  * @brief z_position_units
  */
  std::string   _z_position_units;
  /**
  * @brief x_velocity_title
  */
  std::string   _x_velocity_title;
  /**
  * @brief x_velocity
  */
  std::string   _x_velocity;
  /**
  * @brief x_velocity_units
  */
  std::string   _x_velocity_units;
  /**
  * @brief y_velocity_title
  */
  std::string   _y_velocity_title;
  /**
  * @brief y_velocity
  */
  std::string   _y_velocity;
  /**
  * @brief y_velocity_units
  */
  std::string   _y_velocity_units;
  /**
  * @brief z_velocity_title
  */
  std::string   _z_velocity_title;
  /**
  * @brief z_velocity
  */
  std::string   _z_velocity;
  /**
  * @brief z_velocity_units
  */
  std::string   _z_velocity_units;
  /**
  * @brief vector_source_title
  */
  std::string   _vector_source_title;
  /**
  * @brief vector_source
  */
  std::string   _vector_source;
  /**
  * @brief spare_4
  */
  std::string   _spare_4;
  /**
  * @brief utc_sbt_time_title
  */
  std::string   _utc_sbt_time_title;
  /**
  * @brief utc_sbt_time
  */
  std::string   _utc_sbt_time;
  /**
  * @brief sat_binary_time_title
  */
  std::string   _sat_binary_time_title;
  /**
  * @brief sat_binary_time
  */
  std::string   _sat_binary_time;
  /**
  * @brief clock_step_title
  */
  std::string   _clock_step_title;
  /**
  * @brief clock_step
  */
  std::string   _clock_step;
  /**
  * @brief clock_step_units
  */
  std::string   _clock_step_units;
  /**
  * @brief leap_utc_title
  */
  std::string   _leap_utc_title;
  /**
  * @brief leap_utc
  */
  std::string   _leap_utc;
  /**
  * @brief leap_sign_title
  */
  std::string   _leap_sign_title;
  /**
  * @brief leap_sign
  */
  std::string   _leap_sign;
  /**
  * @brief leap_err_title
  */
  std::string   _leap_err_title;
  /**
  * @brief leap_err
  */
  std::string   _leap_err;
  /**
  * @brief product_err_title
  */
  std::string   _product_err_title;
  /**
  * @brief product_err
  */
  std::string   _product_err;
  /**
  * @brief total_size_title
  */
  std::string   _total_size_title;
  /**
  * @brief tot_size
  */
  std::string   _tot_size;
  /**
  * @brief total_size_units
  */
  std::string   _total_size_units;
  /**
  * @brief sph_size_title
  */
  std::string   _sph_size_title;
  /**
  * @brief sph_size
  */
  std::string   _sph_size;
  /**
  * @brief sph_size_units
  */
  std::string   _sph_size_units;
  /**
  * @brief number_of_dsd_title
  */
  std::string   _number_of_dsd_title;
  /**
  * @brief num_dsd
  */
  int   _num_dsd;
  /**
  * @brief size_of_dsd_title
  */
  std::string   _size_of_dsd_title;
  /**
  * @brief dsd_size
  */
  int  _dsd_size;
  /**
  * @brief size_of_dsd_units
  */
  std::string   _size_of_dsd_units;
  /**
  * @brief number_of_ds_att_title
  */
  std::string   _number_of_ds_att_title;
  /**
  * @brief num_data_sets
  */
  int   _num_data_sets;

private:

};
}
#endif


