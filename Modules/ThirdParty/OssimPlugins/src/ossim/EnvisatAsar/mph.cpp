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

#include <EnvisatAsar/mph.h>

namespace ossimplugins
{
   mph::mph() : EnvisatAsarRecord("mph_rec")
   {
   }

   mph::~mph()
   {
   }

   std::ostream& operator<<(std::ostream& os, const mph& data)
   {
      os<<"product_name:"<<data._product_name.c_str()<<std::endl;

      os<<"product:"<<data._product.c_str()<<std::endl;

      os<<"proc_stage_title:"<<data._proc_stage_title.c_str()<<std::endl;

      os<<"proc_stage:"<<data._proc_stage.c_str()<<std::endl;

      os<<"ref_doc_title:"<<data._ref_doc_title.c_str()<<std::endl;

      os<<"ref_doc:"<<data._ref_doc.c_str()<<std::endl;

      os<<"acq_stat_id_title:"<<data._acq_stat_id_title.c_str()<<std::endl;

      os<<"acq_station:"<<data._acq_station.c_str()<<std::endl;

      os<<"proc_center_title:"<<data._proc_center_title.c_str()<<std::endl;

      os<<"proc_center:"<<data._proc_center.c_str()<<std::endl;

      os<<"proc_time_title:"<<data._proc_time_title.c_str()<<std::endl;

      os<<"proc_time:"<<data._proc_time.c_str()<<std::endl;

      os<<"soft_version_title:"<<data._soft_version_title.c_str()<<std::endl;

      os<<"soft_version:"<<data._soft_version.c_str()<<std::endl;

      os<<"sensing_start_title:"<<data._sensing_start_title.c_str()<<std::endl;

      os<<"sensing_start:"<<data._sensing_start.c_str()<<std::endl;

      os<<"sensing_stop_title:"<<data._sensing_stop_title.c_str()<<std::endl;

      os<<"sensing_stop:"<<data._sensing_stop.c_str()<<std::endl;

      os<<"phase_title:"<<data._phase_title.c_str()<<std::endl;

      os<<"phase:"<<data._phase.c_str()<<std::endl;

      os<<"cycle_title:"<<data._cycle_title.c_str()<<std::endl;

      os<<"cycle:"<<data._cycle.c_str()<<std::endl;

      os<<"relative_orbit_title:"<<data._relative_orbit_title.c_str()<<std::endl;

      os<<"rel_orbit:"<<data._rel_orbit.c_str()<<std::endl;

      os<<"absolute_orbit_title:"<<data._absolute_orbit_title.c_str()<<std::endl;

      os<<"abs_orbit:"<<data._abs_orbit.c_str()<<std::endl;

      os<<"state_vector_time_title:"<<data._state_vector_time_title.c_str()<<std::endl;

      os<<"state_vector_time:"<<data._state_vector_time.c_str()<<std::endl;

      os<<"delta_ut1_title:"<<data._delta_ut1_title.c_str()<<std::endl;

      os<<"delta_ut1:"<<data._delta_ut1.c_str()<<std::endl;

      os<<"delta_ut1_units:"<<data._delta_ut1_units.c_str()<<std::endl;

      os<<"x_position_title:"<<data._x_position_title.c_str()<<std::endl;

      os<<"x_position:"<<data._x_position.c_str()<<std::endl;

      os<<"x_position_units:"<<data._x_position_units.c_str()<<std::endl;

      os<<"y_position_title:"<<data._y_position_title.c_str()<<std::endl;

      os<<"y_position:"<<data._y_position.c_str()<<std::endl;

      os<<"y_position_units:"<<data._y_position_units.c_str()<<std::endl;

      os<<"z_position_title:"<<data._z_position_title.c_str()<<std::endl;

      os<<"z_position:"<<data._z_position.c_str()<<std::endl;

      os<<"z_position_units:"<<data._z_position_units.c_str()<<std::endl;

      os<<"x_velocity_title:"<<data._x_velocity_title.c_str()<<std::endl;

      os<<"x_velocity:"<<data._x_velocity.c_str()<<std::endl;

      os<<"x_velocity_units:"<<data._x_velocity_units.c_str()<<std::endl;

      os<<"y_velocity_title:"<<data._y_velocity_title.c_str()<<std::endl;

      os<<"y_velocity:"<<data._y_velocity.c_str()<<std::endl;

      os<<"y_velocity_units:"<<data._y_velocity_units.c_str()<<std::endl;

      os<<"z_velocity_title:"<<data._z_velocity_title.c_str()<<std::endl;

      os<<"z_velocity:"<<data._z_velocity.c_str()<<std::endl;

      os<<"z_velocity_units:"<<data._z_velocity_units.c_str()<<std::endl;

      os<<"vector_source_title:"<<data._vector_source_title.c_str()<<std::endl;

      os<<"vector_source:"<<data._vector_source.c_str()<<std::endl;

      os<<"spare_4:"<<data._spare_4.c_str()<<std::endl;

      os<<"utc_sbt_time_title:"<<data._utc_sbt_time_title.c_str()<<std::endl;

      os<<"utc_sbt_time:"<<data._utc_sbt_time.c_str()<<std::endl;

      os<<"sat_binary_time_title:"<<data._sat_binary_time_title.c_str()<<std::endl;

      os<<"sat_binary_time:"<<data._sat_binary_time.c_str()<<std::endl;

      os<<"clock_step_title:"<<data._clock_step_title.c_str()<<std::endl;

      os<<"clock_step:"<<data._clock_step.c_str()<<std::endl;

      os<<"clock_step_units:"<<data._clock_step_units.c_str()<<std::endl;

      os<<"leap_utc_title:"<<data._leap_utc_title.c_str()<<std::endl;

      os<<"leap_utc:"<<data._leap_utc.c_str()<<std::endl;

      os<<"leap_sign_title:"<<data._leap_sign_title.c_str()<<std::endl;

      os<<"leap_sign:"<<data._leap_sign.c_str()<<std::endl;

      os<<"leap_err_title:"<<data._leap_err_title.c_str()<<std::endl;

      os<<"leap_err:"<<data._leap_err.c_str()<<std::endl;

      os<<"product_err_title:"<<data._product_err_title.c_str()<<std::endl;

      os<<"product_err:"<<data._product_err.c_str()<<std::endl;

      os<<"total_size_title:"<<data._total_size_title.c_str()<<std::endl;

      os<<"tot_size:"<<data._tot_size.c_str()<<std::endl;

      os<<"total_size_units:"<<data._total_size_units.c_str()<<std::endl;

      os<<"sph_size_title:"<<data._sph_size_title.c_str()<<std::endl;

      os<<"sph_size:"<<data._sph_size.c_str()<<std::endl;

      os<<"sph_size_units:"<<data._sph_size_units.c_str()<<std::endl;

      os<<"number_of_dsd_title:"<<data._number_of_dsd_title.c_str()<<std::endl;

      os<<"num_dsd:"<<data._num_dsd<<std::endl;

      os<<"size_of_dsd_title:"<<data._size_of_dsd_title.c_str()<<std::endl;

      os<<"dsd_size:"<<data._dsd_size<<std::endl;

      os<<"size_of_dsd_units:"<<data._size_of_dsd_units.c_str()<<std::endl;

      os<<"number_of_ds_att_title:"<<data._number_of_ds_att_title.c_str()<<std::endl;

      os<<"num_data_sets:"<<data._num_data_sets<<std::endl;

      return os;

   }

   std::istream& operator>>(std::istream& is, mph& data)
   {
      char buf64[65];
      buf64[64] = '\0';
      char buf32[33];
      buf32[32] = '\0';
      char buf27[28];
      buf27[27]='\0';
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
      char buf11[12];
      buf11[11] = '\0';
      char buf10[11];
      buf10[10] = '\0';
      // char buf9[10];
      // buf9[9] = '\0';
      char buf8[9];
      buf8[8] = '\0';
      char buf7[8];
      buf7[7] = '\0';
      char buf6[7];
      buf6[6] = '\0';
      // char buf5[6];
      // buf5[5] = '\0';
      char buf4[5];
      buf4[4] = '\0';
      char buf3[4];
      buf3[3] = '\0';
      char buf2[3];
      buf2[2] = '\0';
      char buf1[1];
      buf1[0] = '\0';

      is.read(buf8,8);
      data._product_name = buf8;

      is.read(buf1,1);

      is.read(buf64,62);
      buf64[62]='\0';
      data._product = buf64;

      is.read(buf1,1);

      is.read(buf1,1);

      is.read(buf16,11);
      buf16[11]='\0';
      data._proc_stage_title = buf16;

      is.read(buf1,1);
      data._proc_stage = buf1;

      is.read(buf1,1);

      is.read(buf8,8);
      data._ref_doc_title = buf8;

      is.read(buf1,1);

      is.read(buf32,23);
      buf32[23]='\0';
      data._ref_doc = buf32;

      is.read(buf1,1);

      is.read(buf1,1);

      is.read(buf64,40);

      is.read(buf1,1);

      is.read(buf32,20);
      buf32[20]='\0';
      data._acq_stat_id_title = buf32;

      is.read(buf1,1);

      is.read(buf32,20);
      buf32[20]='\0';
      data._acq_station = buf32;

      is.read(buf1,1);

      is.read(buf1,1);

      is.read(buf16,12);
      buf16[12]='\0';
      data._proc_center_title = buf16;

      is.read(buf1,1);

      is.read(buf8,6);
      buf8[6]='\0';
      data._proc_center = buf8;

      is.read(buf1,1);

      is.read(buf1,1);

      is.read(buf16,10);
      buf16[10]='\0';
      data._proc_time_title = buf16;

      is.read(buf1,1);

      is.read(buf32,27);
      buf32[27]='\0';
      data._proc_time = buf32;

      is.read(buf1,1);

      is.read(buf1,1);

      is.read(buf16,13);
      buf16[13]='\0';
      data._soft_version_title = buf16;

      is.read(buf1,1);

      is.read(buf16,14);
      buf16[14]='\0';
      data._soft_version = buf16;

      is.read(buf1,1);

      is.read(buf1,1);

      is.read(buf64,40);

      is.read(buf1,1);

      is.read(buf16,14);
      buf16[14]='\0';
      data._sensing_start_title = buf16;

      is.read(buf1,1);

      is.read(buf32,27);
      buf32[27]='\0';
      data._sensing_start = buf32;

      is.read(buf1,1);

      is.read(buf1,1);

      is.read(buf16,13);
      buf16[13]='\0';
      data._sensing_stop_title = buf16;

      is.read(buf1,1);

      is.read(buf32,27);
      buf32[27]='\0';
      data._sensing_stop = buf32;

      is.read(buf1,1);

      is.read(buf1,1);

      is.read(buf64,40);

      is.read(buf1,1);

      is.read(buf6,6);
      data._phase_title = buf6;

      is.read(buf1,1);
      data._phase = buf1;

      is.read(buf1,1);

      is.read(buf6,6);
      data._cycle_title = buf6;

      is.read(buf4,4);
      data._cycle = buf4;

      is.read(buf1,1);

      is.read(buf10,10);
      data._relative_orbit_title = buf10;

      is.read(buf6,6);
      data._rel_orbit = buf6;

      is.read(buf1,1);

      is.read(buf10,10);
      data._absolute_orbit_title = buf10;

      is.read(buf6,6);
      data._abs_orbit = buf6;

      is.read(buf1,1);

      is.read(buf32,18);
      buf32[18]='\0';
      data._state_vector_time_title = buf32;

      is.read(buf1,1);

      is.read(buf27,27);
      data._state_vector_time = buf27;

      is.read(buf1,1);

      is.read(buf1,1);

      is.read(buf10,10);
      data._delta_ut1_title = buf10;

      is.read(buf8,8);
      data._delta_ut1 = buf8;

      is.read(buf4,3);
      buf4[3]='\0';
      data._delta_ut1_units = buf4;

      is.read(buf1,1);

      is.read(buf11,11);
      data._x_position_title = buf11;

      is.read(buf12,12);
      data._x_position = buf12;

      is.read(buf3,3);
      data._x_position_units = buf3;

      is.read(buf1,1);

      is.read(buf11,11);
      data._y_position_title = buf11;

      is.read(buf12,12);
      data._y_position = buf12;

      is.read(buf3,3);
      data._y_position_units = buf3;

      is.read(buf1,1);

      is.read(buf11,11);
      data._z_position_title = buf11;

      is.read(buf12,12);
      data._z_position = buf12;

      is.read(buf3,3);
      data._z_position_units = buf3;

      is.read(buf1,1);

      is.read(buf11,11);
      data._x_velocity_title = buf11;

      is.read(buf12,12);
      data._x_velocity = buf12;

      is.read(buf8,5);
      buf8[5] = '\0';
      data._x_velocity_units = buf8;

      is.read(buf1,1);

      is.read(buf11,11);
      data._y_velocity_title = buf11;

      is.read(buf12,12);
      data._y_velocity = buf12;

      is.read(buf8,5);
      buf8[5] = '\0';
      data._y_velocity_units = buf8;

      is.read(buf1,1);

      is.read(buf11,11);
      data._z_velocity_title = buf11;

      is.read(buf12,12);
      data._z_velocity = buf12;

      is.read(buf8,5);
      buf8[5] = '\0';
      data._z_velocity_units = buf8;

      is.read(buf1,1);

      is.read(buf16,14);
      buf16[14] = '\0';
      data._vector_source_title = buf16;

      is.read(buf1,1);

      is.read(buf2,2);
      data._vector_source = buf2;

      is.read(buf1,1);

      is.read(buf1,1);

      is.read(buf64,40);
      buf64[40] = '\0';
      data._spare_4 = buf64;

      is.read(buf1,1);

      is.read(buf16,13);
      buf16[13] = '\0';
      data._utc_sbt_time_title = buf16;

      is.read(buf1,1);

      is.read(buf27,27);
      data._utc_sbt_time = buf27;

      is.read(buf1,1);

      is.read(buf1,1);

      is.read(buf16,16);
      data._sat_binary_time_title = buf16;

      is.read(buf11,11);
      data._sat_binary_time = buf11;

      is.read(buf1,1);

      is.read(buf11,11);
      data._clock_step_title = buf11;

      is.read(buf11,11);
      data._clock_step = buf11;

      is.read(buf4,4);
      data._clock_step_units = buf4;

      is.read(buf1,1);

      is.read(buf32,32);

      is.read(buf1,1);

      is.read(buf10,9);
      buf10[9] = '\0';
      data._leap_utc_title = buf10;

      is.read(buf1,1);

      is.read(buf27,27);
      data._leap_utc = buf27;

      is.read(buf1,1);

      is.read(buf1,1);

      is.read(buf10,10);
      data._leap_sign_title = buf10;

      is.read(buf4,4);
      data._leap_sign = buf4;

      is.read(buf1,1);

      is.read(buf10,9);
      buf10[9] = '\0';
      data._leap_err_title = buf10;

      is.read(buf1,1);
      data._leap_err = buf1;

      is.read(buf1,1);

      is.read(buf64,40);

      is.read(buf1,1);

      is.read(buf12,12);
      data._product_err_title = buf12;

      is.read(buf1,1);
      data._product_err = buf1;

      is.read(buf1,1);

      is.read(buf10,9);
      buf10[9] = '\0';
      data._total_size_title = buf10;

      is.read(buf32,21);
      buf32[21] = '\0';
      data._tot_size = buf32;

      is.read(buf7,7);
      data._total_size_units = buf7;

      is.read(buf1,1);

      is.read(buf10,9);
      buf10[9] = '\0';
      data._sph_size_title = buf10;

      is.read(buf11,11);
      data._sph_size = buf11;

      is.read(buf8,7);
      buf8[7] = '\0';
      data._sph_size_units = buf8;

      is.read(buf1,1);

      is.read(buf8,8);
      data._number_of_dsd_title = buf8;

      is.read(buf11,11);
      data._num_dsd = atoi(buf11);

      is.read(buf1,1);

      is.read(buf10,9);
      buf10[9] = '\0';
      data._size_of_dsd_title = buf10;

      is.read(buf11,11);
      data._dsd_size = atoi(buf11);

      is.read(buf8,7);
      buf8[7] = '\0';
      data._size_of_dsd_units = buf8;

      is.read(buf1,1);

      is.read(buf16,14);
      buf16[14] = '\0';
      data._number_of_ds_att_title = buf16;

      is.read(buf11,11);
      data._num_data_sets = atoi(buf11);

      is.read(buf1,1);

      is.read(buf64,40);

      is.read(buf1,1);

      return is;
   }


   mph::mph(const mph& rhs):
      EnvisatAsarRecord(rhs),
      _product_name(rhs._product_name),
      _product(rhs._product),
      _proc_stage_title(rhs._proc_stage_title),
      _proc_stage(rhs._proc_stage),
      _ref_doc_title(rhs._ref_doc_title),
      _ref_doc(rhs._ref_doc),
      _acq_stat_id_title(rhs._acq_stat_id_title),
      _acq_station(rhs._acq_station),
      _proc_center_title(rhs._proc_center_title),
      _proc_center(rhs._proc_center),
      _proc_time_title(rhs._proc_time_title),
      _proc_time(rhs._proc_time),
      _soft_version_title(rhs._soft_version_title),
      _soft_version(rhs._soft_version),
      _sensing_start_title(rhs._sensing_start_title),
      _sensing_start(rhs._sensing_start),
      _sensing_stop_title(rhs._sensing_stop_title),
      _sensing_stop(rhs._sensing_stop),
      _phase_title(rhs._phase_title),
      _phase(rhs._phase),
      _cycle_title(rhs._cycle_title),
      _cycle(rhs._cycle),
      _relative_orbit_title(rhs._relative_orbit_title),
      _rel_orbit(rhs._rel_orbit),
      _absolute_orbit_title(rhs._absolute_orbit_title),
      _abs_orbit(rhs._abs_orbit),
      _state_vector_time_title(rhs._state_vector_time_title),
      _state_vector_time(rhs._state_vector_time),
      _delta_ut1_title(rhs._delta_ut1_title),
      _delta_ut1(rhs._delta_ut1),
      _delta_ut1_units(rhs._delta_ut1_units),
      _x_position_title(rhs._x_position_title),
      _x_position(rhs._x_position),
      _x_position_units(rhs._x_position_units),
      _y_position_title(rhs._y_position_title),
      _y_position(rhs._y_position),
      _y_position_units(rhs._y_position_units),
      _z_position_title(rhs._z_position_title),
      _z_position(rhs._z_position),
      _z_position_units(rhs._z_position_units),
      _x_velocity_title(rhs._x_velocity_title),
      _x_velocity(rhs._x_velocity),
      _x_velocity_units(rhs._x_velocity_units),
      _y_velocity_title(rhs._y_velocity_title),
      _y_velocity(rhs._y_velocity),
      _y_velocity_units(rhs._y_velocity_units),
      _z_velocity_title(rhs._z_velocity_title),
      _z_velocity(rhs._z_velocity),
      _z_velocity_units(rhs._z_velocity_units),
      _vector_source_title(rhs._vector_source_title),
      _vector_source(rhs._vector_source),
      _spare_4(rhs._spare_4),
      _utc_sbt_time_title(rhs._utc_sbt_time_title),
      _utc_sbt_time(rhs._utc_sbt_time),
      _sat_binary_time_title(rhs._sat_binary_time_title),
      _sat_binary_time(rhs._sat_binary_time),
      _clock_step_title(rhs._clock_step_title),
      _clock_step(rhs._clock_step),
      _clock_step_units(rhs._clock_step_units),
      _leap_utc_title(rhs._leap_utc_title),
      _leap_utc(rhs._leap_utc),
      _leap_sign_title(rhs._leap_sign_title),
      _leap_sign(rhs._leap_sign),
      _leap_err_title(rhs._leap_err_title),
      _leap_err(rhs._leap_err),
      _product_err_title(rhs._product_err_title),
      _product_err(rhs._product_err),
      _total_size_title(rhs._total_size_title),
      _tot_size(rhs._tot_size),
      _total_size_units(rhs._total_size_units),
      _sph_size_title(rhs._sph_size_title),
      _sph_size(rhs._sph_size),
      _sph_size_units(rhs._sph_size_units),
      _number_of_dsd_title(rhs._number_of_dsd_title),
      _num_dsd(rhs._num_dsd),
      _size_of_dsd_title(rhs._size_of_dsd_title),
      _dsd_size(rhs._dsd_size),
      _size_of_dsd_units(rhs._size_of_dsd_units),
      _number_of_ds_att_title(rhs._number_of_ds_att_title),
      _num_data_sets(rhs._num_data_sets)
   {}


   mph& mph::operator=(const mph& rhs)
   {
      _product_name = rhs._product_name;
      _product = rhs._product;
      _proc_stage_title = rhs._proc_stage_title;
      _proc_stage = rhs._proc_stage;
      _ref_doc_title = rhs._ref_doc_title;
      _ref_doc = rhs._ref_doc;
      _acq_stat_id_title = rhs._acq_stat_id_title;
      _acq_station = rhs._acq_station;
      _proc_center_title = rhs._proc_center_title;
      _proc_center = rhs._proc_center;
      _proc_time_title = rhs._proc_time_title;
      _proc_time = rhs._proc_time;
      _soft_version_title = rhs._soft_version_title;
      _soft_version = rhs._soft_version;
      _sensing_start_title = rhs._sensing_start_title;
      _sensing_start = rhs._sensing_start;
      _sensing_stop_title = rhs._sensing_stop_title;
      _sensing_stop = rhs._sensing_stop;
      _phase_title = rhs._phase_title;
      _phase = rhs._phase;
      _cycle_title = rhs._cycle_title;
      _cycle = rhs._cycle;
      _relative_orbit_title = rhs._relative_orbit_title;
      _rel_orbit = rhs._rel_orbit;
      _absolute_orbit_title = rhs._absolute_orbit_title;
      _abs_orbit = rhs._abs_orbit;
      _state_vector_time_title = rhs._state_vector_time_title;
      _state_vector_time = rhs._state_vector_time;
      _delta_ut1_title = rhs._delta_ut1_title;
      _delta_ut1 = rhs._delta_ut1;
      _delta_ut1_units = rhs._delta_ut1_units;
      _x_position_title = rhs._x_position_title;
      _x_position = rhs._x_position;
      _x_position_units = rhs._x_position_units;
      _y_position_title = rhs._y_position_title;
      _y_position = rhs._y_position;
      _y_position_units = rhs._y_position_units;
      _z_position_title = rhs._z_position_title;
      _z_position = rhs._z_position;
      _z_position_units = rhs._z_position_units;
      _x_velocity_title = rhs._x_velocity_title;
      _x_velocity = rhs._x_velocity;
      _x_velocity_units = rhs._x_velocity_units;
      _y_velocity_title = rhs._y_velocity_title;
      _y_velocity = rhs._y_velocity;
      _y_velocity_units = rhs._y_velocity_units;
      _z_velocity_title = rhs._z_velocity_title;
      _z_velocity = rhs._z_velocity;
      _z_velocity_units = rhs._z_velocity_units;
      _vector_source_title = rhs._vector_source_title;
      _vector_source = rhs._vector_source;
      _spare_4 = rhs._spare_4;
      _utc_sbt_time_title = rhs._utc_sbt_time_title;
      _utc_sbt_time = rhs._utc_sbt_time;
      _sat_binary_time_title = rhs._sat_binary_time_title;
      _sat_binary_time = rhs._sat_binary_time;
      _clock_step_title = rhs._clock_step_title;
      _clock_step = rhs._clock_step;
      _clock_step_units = rhs._clock_step_units;
      _leap_utc_title = rhs._leap_utc_title;
      _leap_utc = rhs._leap_utc;
      _leap_sign_title = rhs._leap_sign_title;
      _leap_sign = rhs._leap_sign;
      _leap_err_title = rhs._leap_err_title;
      _leap_err = rhs._leap_err;
      _product_err_title = rhs._product_err_title;
      _product_err = rhs._product_err;
      _total_size_title = rhs._total_size_title;
      _tot_size = rhs._tot_size;
      _total_size_units = rhs._total_size_units;
      _sph_size_title = rhs._sph_size_title;
      _sph_size = rhs._sph_size;
      _sph_size_units = rhs._sph_size_units;
      _number_of_dsd_title = rhs._number_of_dsd_title;
      _num_dsd = rhs._num_dsd;
      _size_of_dsd_title = rhs._size_of_dsd_title;
      _dsd_size = rhs._dsd_size;
      _size_of_dsd_units = rhs._size_of_dsd_units;
      _number_of_ds_att_title = rhs._number_of_ds_att_title;
      _num_data_sets = rhs._num_data_sets;

      return *this;
   }
}
