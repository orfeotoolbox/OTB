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
 * \todo Use the default generated copy- and move-constructors and assignment-operators
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
  std::string const& get_product_name() const
  {
    return _product_name;
  };
  /**
  * @brief product
  */
  std::string const& get_product() const
  {
    return _product;
  };
  /**
  * @brief proc_stage_title
  */
  std::string const& get_proc_stage_title() const
  {
    return _proc_stage_title;
  };
  /**
  * @brief proc_stage
  */
  std::string const& get_proc_stage() const
  {
    return _proc_stage;
  };
  /**
  * @brief ref_doc_title
  */
  std::string const& get_ref_doc_title() const
  {
    return _ref_doc_title;
  };
  /**
  * @brief ref_doc
  */
  std::string const& get_ref_doc() const
  {
    return _ref_doc;
  };
  /**
  * @brief acq_stat_id_title
  */
  std::string const& get_acq_stat_id_title() const
  {
    return _acq_stat_id_title;
  };
  /**
  * @brief acq_station
  */
  std::string const& get_acq_station() const
  {
    return _acq_station;
  };
  /**
  * @brief proc_center_title
  */
  std::string const& get_proc_center_title() const
  {
    return _proc_center_title;
  };
  /**
  * @brief proc_center
  */
  std::string const& get_proc_center() const
  {
    return _proc_center;
  };
  /**
  * @brief proc_time_title
  */
  std::string const& get_proc_time_title() const
  {
    return _proc_time_title;
  };
  /**
  * @brief proc_time
  */
  std::string const& get_proc_time() const
  {
    return _proc_time;
  };
  /**
  * @brief soft_version_title
  */
  std::string const& get_soft_version_title() const
  {
    return _soft_version_title;
  };
  /**
  * @brief soft_version
  */
  std::string const& get_soft_version() const
  {
    return _soft_version;
  };
  /**
  * @brief sensing_start_title
  */
  std::string const& get_sensing_start_title() const
  {
    return _sensing_start_title;
  };
  /**
  * @brief sensing_start
  */
  std::string const& get_sensing_start() const
  {
    return _sensing_start;
  };
  /**
  * @brief sensing_stop_title
  */
  std::string const& get_sensing_stop_title() const
  {
    return _sensing_stop_title;
  };
  /**
  * @brief sensing_stop
  */
  std::string const& get_sensing_stop() const
  {
    return _sensing_stop;
  };
  /**
  * @brief phase_title
  */
  std::string const& get_phase_title() const
  {
    return _phase_title;
  };
  /**
  * @brief phase
  */
  std::string const& get_phase() const
  {
    return _phase;
  };
  /**
  * @brief cycle_title
  */
  std::string const& get_cycle_title() const
  {
    return _cycle_title;
  };
  /**
  * @brief cycle
  */
  std::string const& get_cycle() const
  {
    return _cycle;
  };
  /**
  * @brief relative_orbit_title
  */
  std::string const& get_relative_orbit_title() const
  {
    return _relative_orbit_title;
  };
  /**
  * @brief rel_orbit
  */
  std::string const& get_rel_orbit() const
  {
    return _rel_orbit;
  };
  /**
  * @brief absolute_orbit_title
  */
  std::string const& get_absolute_orbit_title() const
  {
    return _absolute_orbit_title;
  };
  /**
  * @brief abs_orbit
  */
  std::string const& get_abs_orbit() const
  {
    return _abs_orbit;
  };
  /**
  * @brief state_vector_time_title
  */
  std::string const& get_state_vector_time_title() const
  {
    return _state_vector_time_title;
  };
  /**
  * @brief state_vector_time
  */
  std::string const& get_state_vector_time() const
  {
    return _state_vector_time;
  };
  /**
  * @brief delta_ut1_title
  */
  std::string const& get_delta_ut1_title() const
  {
    return _delta_ut1_title;
  };
  /**
  * @brief delta_ut1
  */
  std::string const& get_delta_ut1() const
  {
    return _delta_ut1;
  };
  /**
  * @brief delta_ut1_units
  */
  std::string const& get_delta_ut1_units() const
  {
    return _delta_ut1_units;
  };
  /**
  * @brief x_position_title
  */
  std::string const& get_x_position_title() const
  {
    return _x_position_title;
  };
  /**
  * @brief x_position
  */
  std::string const& get_x_position() const
  {
    return _x_position;
  };
  /**
  * @brief x_position_units
  */
  std::string const& get_x_position_units() const
  {
    return _x_position_units;
  };
  /**
  * @brief y_position_title
  */
  std::string const& get_y_position_title() const
  {
    return _y_position_title;
  };
  /**
  * @brief y_position
  */
  std::string const& get_y_position() const
  {
    return _y_position;
  };
  /**
  * @brief y_position_units
  */
  std::string const& get_y_position_units() const
  {
    return _y_position_units;
  };
  /**
  * @brief z_position_title
  */
  std::string const& get_z_position_title() const
  {
    return _z_position_title;
  };
  /**
  * @brief z_position
  */
  std::string const& get_z_position() const
  {
    return _z_position;
  };
  /**
  * @brief z_position_units
  */
  std::string const& get_z_position_units() const
  {
    return _z_position_units;
  };
  /**
  * @brief x_velocity_title
  */
  std::string const& get_x_velocity_title() const
  {
    return _x_velocity_title;
  };
  /**
  * @brief x_velocity
  */
  std::string const& get_x_velocity() const
  {
    return _x_velocity;
  };
  /**
  * @brief x_velocity_units
  */
  std::string const& get_x_velocity_units() const
  {
    return _x_velocity_units;
  };
  /**
  * @brief y_velocity_title
  */
  std::string const& get_y_velocity_title() const
  {
    return _y_velocity_title;
  };
  /**
  * @brief y_velocity
  */
  std::string const& get_y_velocity() const
  {
    return _y_velocity;
  };
  /**
  * @brief y_velocity_units
  */
  std::string const& get_y_velocity_units() const
  {
    return _y_velocity_units;
  };
  /**
  * @brief z_velocity_title
  */
  std::string const& get_z_velocity_title() const
  {
    return _z_velocity_title;
  };
  /**
  * @brief z_velocity
  */
  std::string const& get_z_velocity() const
  {
    return _z_velocity;
  };
  /**
  * @brief z_velocity_units
  */
  std::string const& get_z_velocity_units() const
  {
    return _z_velocity_units;
  };
  /**
  * @brief vector_source_title
  */
  std::string const& get_vector_source_title() const
  {
    return _vector_source_title;
  };
  /**
  * @brief vector_source
  */
  std::string const& get_vector_source() const
  {
    return _vector_source;
  };
  /**
  * @brief spare_4
  */
  std::string const& get_spare_4() const
  {
    return _spare_4;
  };
  /**
  * @brief utc_sbt_time_title
  */
  std::string const& get_utc_sbt_time_title() const
  {
    return _utc_sbt_time_title;
  };
  /**
  * @brief utc_sbt_time
  */
  std::string const& get_utc_sbt_time() const
  {
    return _utc_sbt_time;
  };
  /**
  * @brief sat_binary_time_title
  */
  std::string const& get_sat_binary_time_title() const
  {
    return _sat_binary_time_title;
  };
  /**
  * @brief sat_binary_time
  */
  std::string const& get_sat_binary_time() const
  {
    return _sat_binary_time;
  };
  /**
  * @brief clock_step_title
  */
  std::string const& get_clock_step_title() const
  {
    return _clock_step_title;
  };
  /**
  * @brief clock_step
  */
  std::string const& get_clock_step() const
  {
    return _clock_step;
  };
  /**
  * @brief clock_step_units
  */
  std::string const& get_clock_step_units() const
  {
    return _clock_step_units;
  };
  /**
  * @brief leap_utc_title
  */
  std::string const& get_leap_utc_title() const
  {
    return _leap_utc_title;
  };
  /**
  * @brief leap_utc
  */
  std::string const& get_leap_utc() const
  {
    return _leap_utc;
  };
  /**
  * @brief leap_sign_title
  */
  std::string const& get_leap_sign_title() const
  {
    return _leap_sign_title;
  };
  /**
  * @brief leap_sign
  */
  std::string const& get_leap_sign() const
  {
    return _leap_sign;
  };
  /**
  * @brief leap_err_title
  */
  std::string const& get_leap_err_title() const
  {
    return _leap_err_title;
  };
  /**
  * @brief leap_err
  */
  std::string const& get_leap_err() const
  {
    return _leap_err;
  };
  /**
  * @brief product_err_title
  */
  std::string const& get_product_err_title() const
  {
    return _product_err_title;
  };
  /**
  * @brief product_err
  */
  std::string const& get_product_err() const
  {
    return _product_err;
  };
  /**
  * @brief total_size_title
  */
  std::string const& get_total_size_title() const
  {
    return _total_size_title;
  };
  /**
  * @brief tot_size
  */
  std::string const& get_tot_size() const
  {
    return _tot_size;
  };
  /**
  * @brief total_size_units
  */
  std::string const& get_total_size_units() const
  {
    return _total_size_units;
  };
  /**
  * @brief sph_size_title
  */
  std::string const& get_sph_size_title() const
  {
    return _sph_size_title;
  };
  /**
  * @brief sph_size
  */
  std::string const& get_sph_size() const
  {
    return _sph_size;
  };
  /**
  * @brief sph_size_units
  */
  std::string const& get_sph_size_units() const
  {
    return _sph_size_units;
  };
  /**
  * @brief number_of_dsd_title
  */
  std::string const& get_number_of_dsd_title() const
  {
    return _number_of_dsd_title;
  };
  /**
  * @brief num_dsd
  */
  int   get_num_dsd() const
  {
    return _num_dsd;
  };
  /**
  * @brief size_of_dsd_title
  */
  std::string const& get_size_of_dsd_title() const
  {
    return _size_of_dsd_title;
  };
  /**
  * @brief dsd_size
  */
  int   get_dsd_size() const
  {
    return _dsd_size;
  };
  /**
  * @brief size_of_dsd_units
  */
  std::string const& get_size_of_dsd_units() const
  {
    return _size_of_dsd_units;
  };
  /**
  * @brief number_of_ds_att_title
  */
  std::string const& get_number_of_ds_att_title() const
  {
    return _number_of_ds_att_title;
  };
  /**
  * @brief num_data_sets
  */
  int   get_num_data_sets() const
  {
    return _num_data_sets;
  };
  /**
  * @brief Check mph validity
  */
  bool is_valid() const
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


