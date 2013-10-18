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

#include "erssar/ErsSarDataSetSummary.h"

namespace ossimplugins
{

ErsSarDataSetSummary::ErsSarDataSetSummary() : ErsSarRecord("dataset_sum_rec")
{
}

ErsSarDataSetSummary::~ErsSarDataSetSummary()
{
}

std::ostream& operator<<(std::ostream& os, const ErsSarDataSetSummary& data)
{
  os << "seq_num:" << data._seq_num << std::endl;

  os << "sar_chn:" << data._sar_chn << std::endl;

  os << "scene_id:" << data._scene_id.c_str() << std::endl;

  os << "scene_des:" << data._scene_des.c_str() << std::endl;

  os << "inp_sctim:" << data._inp_sctim.c_str() << std::endl;

  os << "asc_des:" << data._asc_des.c_str() << std::endl;

  os << "pro_lat:" << data._pro_lat << std::endl;

  os << "pro_long:" << data._pro_long << std::endl;

  os << "pro_head:" << data._pro_head << std::endl;

  os << "ellip_des:" << data._ellip_des.c_str() << std::endl;

  os << "ellip_maj:" << data._ellip_maj << std::endl;

  os << "ellip_min:" << data._ellip_min << std::endl;
  os << "earth_mass:" << data._earth_mass << std::endl;
  os << "grav_const:" << data._grav_const << std::endl;
  os << "ellip_j[0]:" << data._ellip_j[0] << std::endl;
  os << "ellip_j[1]:" << data._ellip_j[1] << std::endl;
  os << "ellip_j[2]:" << data._ellip_j[2] << std::endl;
  os << "terrain_h:" << data._terrain_h << std::endl;
  os << "sc_lin:" << data._sc_lin << std::endl;
  os << "sc_pix:" << data._sc_pix << std::endl;
  os << "scene_len:" << data._scene_len << std::endl;
  os << "scene_wid:" << data._scene_wid << std::endl;
  os << "nchn:" << data._nchn << std::endl;
  os << "mission_id:" << data._mission_id.c_str() << std::endl;
  os << "sensor_id:" << data._sensor_id.c_str() << std::endl;
  os << "orbit_num:" << data._orbit_num.c_str() << std::endl;

  os << "plat_lat:" << data._plat_lat << std::endl;

  os << "plat_long:" << data._plat_long << std::endl;

  os << "plat_head:" << data._plat_head << std::endl;

  os << "clock_ang:" << data._clock_ang << std::endl;

  os << "incident_ang:" << data._incident_ang << std::endl;


  os << "wave_length:" << data._wave_length << std::endl;

  os << "motion_comp:" << data._motion_comp.c_str() << std::endl;

  os << "pulse_code:" << data._pulse_code.c_str() << std::endl;

  for (int i = 0; i < 5; i++)
  {
    os << "ampl_coef[" << i << "]:" << data._ampl_coef[i] << std::endl;
  }

  for (int i = 0; i < 5; i++)
  {
    os << "phas_coef[" << i << "]:" << data._phas_coef[i] << std::endl;
  }

  os << "chirp_ext_ind:" << data._chirp_ext_ind << std::endl;

  os << "fr:" << data._fr << std::endl;

  os << "rng_gate:" << data._rng_gate << std::endl;

  os << "rng_length:" << data._rng_length << std::endl;

  os << "baseband_f:" << data._baseband_f.c_str() << std::endl;

  os << "rngcmp_f:" << data._rngcmp_f.c_str() << std::endl;

  os << "gn_polar:" << data._gn_polar << std::endl;

  os << "gn_cross:" << data._gn_cross << std::endl;

  os << "chn_bits:" << data._chn_bits << std::endl;

  os << "quant_desc:" << data._quant_desc.c_str() << std::endl;

  os << "i_bias:" << data._i_bias << std::endl;

  os << "q_bias:" << data._q_bias << std::endl;

  os << "iq_ratio:" << data._iq_ratio << std::endl;

  os << "mech_sight:" << data._mech_sight << std::endl;

  os << "fa:" << data._fa << std::endl;

  os << "sat_bintim:" << data._sat_bintim.c_str() << std::endl;

  os << "sat_clktim:" << data._sat_clktim.c_str() << std::endl;

  os << "sat_clkinc:" << data._sat_clkinc.c_str() << std::endl;

  os << "fac_id:" << data._fac_id.c_str() << std::endl;

  os << "sys_id:" << data._sys_id.c_str() << std::endl;

  os << "ver_id:" << data._ver_id.c_str() << std::endl;

  os << "prod_type:" << data._prod_type.c_str() << std::endl;

  os << "algor_id:" << data._algor_id.c_str() << std::endl;

  os << "n_azilok:" << data._n_azilok << std::endl;

  os << "n_rnglok:" << data._n_rnglok << std::endl;

  os << "bnd_azilok:" << data._bnd_azilok << std::endl;

  os << "bnd_rnglok:" << data._bnd_rnglok << std::endl;

  os << "bnd_azi:" << data._bnd_azi << std::endl;

  os << "bnd_rng:" << data._bnd_rng << std::endl;

  os << "azi_weight:" << data._azi_weight.c_str() << std::endl;

  os << "rng_weight:" << data._rng_weight.c_str() << std::endl;

  os << "data_inpsrc:" << data._data_inpsrc.c_str() << std::endl;

  os << "rng_res:" << data._rng_res << std::endl;

  os << "azi_res:" << data._azi_res << std::endl;

  os << "alt_dopcen[0]:" << data._alt_dopcen[0] << std::endl;
  os << "alt_dopcen[1]:" << data._alt_dopcen[1] << std::endl;
  os << "alt_dopcen[2]:" << data._alt_dopcen[2] << std::endl;

  os << "crt_dopcen[0]:" << data._crt_dopcen[0] << std::endl;
  os << "crt_dopcen[1]:" << data._crt_dopcen[1] << std::endl;
  os << "crt_dopcen[2]:" << data._crt_dopcen[2] << std::endl;

  os << "time_dir_pix:" << data._time_dir_pix.c_str() << std::endl;

  os << "time_dir_lin:" << data._time_dir_lin.c_str() << std::endl;

  os << "alt_rate[0]:" << data._alt_rate[0] << std::endl;
  os << "alt_rate[1]:" << data._alt_rate[1] << std::endl;
  os << "alt_rate[2]:" << data._alt_rate[2] << std::endl;

  os << "crt_rate[0]:" << data._crt_rate[0] << std::endl;
  os << "crt_rate[1]:" << data._crt_rate[1] << std::endl;
  os << "crt_rate[2]:" << data._crt_rate[2] << std::endl;

  os << "clutter_lock:" << data._line_cont.c_str() << std::endl;

  os << "clutter_lock:" << data._clutter_lock.c_str() << std::endl;

  os << "auto_focus:" << data._auto_focus.c_str() << std::endl;

  os << "line_spacing:" << data._line_spacing << std::endl;

  os << "pix_spacing:" << data._pix_spacing << std::endl;

  os << "rngcmp_desg:" << data._rngcmp_desg.c_str() << std::endl;

  os << "zero_dop_range_time_f_pixel:" << data._zero_dop_range_time_f_pixel << std::endl;

  os << "zero_dop_range_time_c_pixel:" << data._zero_dop_range_time_c_pixel << std::endl;

  os << "zero_dop_range_time_l_pixel:" << data._zero_dop_range_time_l_pixel << std::endl;

  os << "zero_dop_az_time_f_pixel :" << data._zero_dop_az_time_f_pixel.c_str() << std::endl;

  os << " zero_dop_az_time_c_pixel:" << data._zero_dop_az_time_c_pixel.c_str() << std::endl;

  os << " zero_dop_az_time_l_pixel:" << data._zero_dop_az_time_l_pixel.c_str() << std::endl;

  return os;

}

std::istream& operator>>(std::istream& is, ErsSarDataSetSummary& data)
{
  char buf16[17];
  buf16[16] = '\0';
  char buf32[33];
  buf32[32] = '\0';
  char buf8[9];
  buf8[8] = '\0';
  char buf4[5];
  buf4[4] = '\0';

  is.read(buf4, 4);
  data._seq_num = atoi(buf4);

  is.read(buf4, 4);
  data._sar_chn = atoi(buf4);

  is.read(buf16, 16);
  data._scene_id = buf16;

  is.read(buf32, 32);
  data._scene_des = buf32;

  is.read(buf32, 32);
  data._inp_sctim = buf32;

  is.read(buf16, 16);
  data._asc_des = buf16;

  is.read(buf16, 16);
  data._pro_lat = atof(buf16);

  is.read(buf16, 16);
  data._pro_long = atof(buf16);

  is.read(buf16, 16);
  data._pro_head = atof(buf16);

  is.read(buf16, 16);
  data._ellip_des = buf16;

  is.read(buf16, 16);
  data._ellip_maj = atof(buf16);

  is.read(buf16, 16);
  data._ellip_min = atof(buf16);

  is.read(buf16, 16);
  data._earth_mass = atof(buf16);

  is.read(buf16, 16);
  data._grav_const = atof(buf16);

  is.read(buf16, 16);
  data._ellip_j[0] = atof(buf16);
  is.read(buf16, 16);
  data._ellip_j[1] = atof(buf16);
  is.read(buf16, 16);
  data._ellip_j[2] = atof(buf16);

  is.read(buf16, 16);

  is.read(buf16, 16);
  data._terrain_h = atof(buf16);

  is.read(buf8, 8);
  data._sc_lin = atoi(buf8);

  is.read(buf8, 8);
  data._sc_pix = atoi(buf8);

  is.read(buf16, 16);
  data._scene_len = atof(buf16);

  is.read(buf16, 16);
  data._scene_wid = atof(buf16);

  is.read(buf16, 16);

  is.read(buf4, 4);
  data._nchn = atoi(buf4);

  is.read(buf4, 4);

  is.read(buf16, 16);
  data._mission_id = buf16;

  is.read(buf32, 32);
  data._sensor_id = buf32;

  is.read(buf8, 8);
  data._orbit_num = buf8;

  is.read(buf8, 8);
  data._plat_lat = atof(buf8);

  is.read(buf8, 8);
  data._plat_long = atof(buf8);

  is.read(buf8, 8);
  data._plat_head = atof(buf8);

  is.read(buf8, 8);
  data._clock_ang = atof(buf8);

  is.read(buf8, 8);
  data._incident_ang = atof(buf8);

  is.read(buf8, 8);

  is.read(buf16, 16);
  data._wave_length = atof(buf16);

  is.read(buf8, 2);
  buf8[2] = '\0';
  data._motion_comp = buf8;

  is.read(buf16, 16);
  data._pulse_code = buf16;

  for (int i = 0; i < 5; i++)
  {
    is.read(buf16, 16);
    data._ampl_coef[i] = atof(buf16);
  }

  for (int i = 0; i < 5; i++)
  {
    is.read(buf16, 16);
    data._phas_coef[i] = atof(buf16);
  }

  is.read(buf8, 8);
  data._chirp_ext_ind = atoi(buf8);

  is.read(buf8, 8);

  is.read(buf16, 16);
  data._fr = atof(buf16);

  is.read(buf16, 16);
  data._rng_gate = atof(buf16);

  is.read(buf16, 16);
  data._rng_length = atof(buf16);

  is.read(buf8, 4);
  buf8[4] = '\0';
  data._baseband_f = buf8;

  is.read(buf8, 4);
  buf8[4] = '\0';
  data._rngcmp_f = buf8;

  is.read(buf16, 16);
  data._gn_polar = atof(buf16);

  is.read(buf16, 16);
  data._gn_cross = atof(buf16);

  is.read(buf8, 8);
  data._chn_bits = atoi(buf8);

  is.read(buf16, 12);
  buf16[12] = '\0';
  data._quant_desc = buf16;

  is.read(buf16, 16);
  data._i_bias = atof(buf16);

  is.read(buf16, 16);
  data._q_bias = atof(buf16);

  is.read(buf16, 16);
  data._iq_ratio = atof(buf16);

  is.read(buf32, 32);

  is.read(buf16, 16);

  is.read(buf16, 16);
  data._mech_sight = atof(buf16);

  is.read(buf4, 4);

  is.read(buf16, 16);
  data._fa = atof(buf16);

  is.read(buf16, 16);

  is.read(buf16, 16);

  is.read(buf16, 16);
  data._sat_bintim = buf16;

  is.read(buf32, 32);
  data._sat_clktim = buf32;

  is.read(buf8, 8);
  data._sat_clkinc = buf8;

  is.read(buf8, 8);

  is.read(buf16, 16);
  data._fac_id = buf16;

  is.read(buf8, 8);
  data._sys_id = buf8;

  is.read(buf8, 8);
  data._ver_id = buf8;

  is.read(buf32, 32);

  is.read(buf32, 32);
  data._prod_type = buf32;

  is.read(buf32, 32);
  data._algor_id = buf32;

  is.read(buf16, 16);
  data._n_azilok = atof(buf16);

  is.read(buf16, 16);
  data._n_rnglok = atof(buf16);

  is.read(buf16, 16);
  data._bnd_azilok = atof(buf16);

  is.read(buf16, 16);
  data._bnd_rnglok = atof(buf16);

  is.read(buf16, 16);
  data._bnd_azi = atof(buf16);

  is.read(buf16, 16);
  data._bnd_rng = atof(buf16);

  is.read(buf32, 32);
  data._azi_weight = buf32;

  is.read(buf32, 32);
  data._rng_weight = buf32;

  is.read(buf16, 16);
  data._data_inpsrc = buf16;

  is.read(buf16, 16);
  data._rng_res = atof(buf16);

  is.read(buf16, 16);
  data._azi_res = atof(buf16);

  is.read(buf32, 32);

  is.read(buf16, 16);
  data._alt_dopcen[0] = atof(buf16);
  is.read(buf16, 16);
  data._alt_dopcen[1] = atof(buf16);
  is.read(buf16, 16);
  data._alt_dopcen[2] = atof(buf16);

  is.read(buf16, 16);

  is.read(buf16, 16);
  data._crt_dopcen[0] = atof(buf16);
  is.read(buf16, 16);
  data._crt_dopcen[1] = atof(buf16);
  is.read(buf16, 16);
  data._crt_dopcen[2] = atof(buf16);

  is.read(buf8, 8);
  data._time_dir_pix = buf8;

  is.read(buf8, 8);
  data._time_dir_lin = buf8;

  is.read(buf16, 16);
  data._alt_rate[0] = atof(buf16);
  is.read(buf16, 16);
  data._alt_rate[1] = atof(buf16);
  is.read(buf16, 16);
  data._alt_rate[2] = atof(buf16);

  is.read(buf16, 16);

  is.read(buf16, 16);
  data._crt_rate[0] = atof(buf16);
  is.read(buf16, 16);
  data._crt_rate[1] = atof(buf16);
  is.read(buf16, 16);
  data._crt_rate[2] = atof(buf16);

  is.read(buf16, 16);

  is.read(buf8, 8);
  data._line_cont = buf8;

  is.read(buf4, 4);
  data._clutter_lock = buf4;

  is.read(buf4, 4);
  data._auto_focus = buf4;

  is.read(buf16, 16);
  data._line_spacing = atof(buf16);

  is.read(buf16, 16);
  data._pix_spacing = atof(buf16);

  is.read(buf16, 16);
  data._rngcmp_desg = buf16;

  is.read(buf32, 32);

  is.read(buf16, 16);
  data._zero_dop_range_time_f_pixel = atof(buf16);

  is.read(buf16, 16);
  data._zero_dop_range_time_c_pixel = atof(buf16);

  is.read(buf16, 16);
  data._zero_dop_range_time_l_pixel  = atof(buf16);

  is.read(buf32, 24);
  buf32[24] = '\0';
  data._zero_dop_az_time_f_pixel = buf32;

  is.read(buf32, 24);
  buf32[24] = '\0';
  data._zero_dop_az_time_c_pixel = buf32;

  is.read(buf32, 24);
  buf32[24] = '\0';
  data._zero_dop_az_time_l_pixel = buf32;

  return is;
}


ErsSarDataSetSummary::ErsSarDataSetSummary(const ErsSarDataSetSummary& rhs):
    ErsSarRecord(rhs),
    _seq_num(rhs._seq_num),
    _sar_chn(rhs._sar_chn),
    _scene_id(rhs._scene_id),
    _scene_des(rhs._scene_des),
    _inp_sctim(rhs._inp_sctim),
    _asc_des(rhs._asc_des),
    _pro_lat(rhs._pro_lat),
    _pro_long(rhs._pro_long),
    _pro_head(rhs._pro_head),
    _ellip_des(rhs._ellip_des),
    _ellip_maj(rhs._ellip_maj),
    _ellip_min(rhs._ellip_min),
    _earth_mass(rhs._earth_mass),
    _grav_const(rhs._grav_const),
    _terrain_h(rhs._terrain_h),
    _sc_lin(rhs._sc_lin),
    _sc_pix(rhs._sc_pix),
    _scene_len(rhs._scene_len),
    _scene_wid(rhs._scene_wid),
    _nchn(rhs._nchn),
    _mission_id(rhs._mission_id),
    _sensor_id(rhs._sensor_id),
    _orbit_num(rhs._orbit_num),
    _plat_lat(rhs._plat_lat),
    _plat_long(rhs._plat_long),
    _plat_head(rhs._plat_head),
    _clock_ang(rhs._clock_ang),
    _incident_ang(rhs._incident_ang),
    _wave_length(rhs._wave_length),
    _motion_comp(rhs._motion_comp),
    _pulse_code(rhs._pulse_code),
    _chirp_ext_ind(rhs._chirp_ext_ind),
    _fr(rhs._fr),
    _rng_gate(rhs._rng_gate),
    _rng_length(rhs._rng_length),
    _baseband_f(rhs._baseband_f),
    _rngcmp_f(rhs._rngcmp_f),
    _gn_polar(rhs._gn_polar),
    _gn_cross(rhs._gn_cross),
    _chn_bits(rhs._chn_bits),
    _quant_desc(rhs._quant_desc),
    _i_bias(rhs._i_bias),
    _q_bias(rhs._q_bias),
    _iq_ratio(rhs._iq_ratio),
    _mech_sight(rhs._mech_sight),
    _fa(rhs._fa),
    _sat_bintim(rhs._sat_bintim),
    _sat_clktim(rhs._sat_clktim),
    _sat_clkinc(rhs._sat_clkinc),
    _fac_id(rhs._fac_id),
    _sys_id(rhs._sys_id),
    _ver_id(rhs._ver_id),
    _prod_type(rhs._prod_type),
    _algor_id(rhs._algor_id),
    _n_azilok(rhs._n_azilok),
    _n_rnglok(rhs._n_rnglok),
    _bnd_azilok(rhs._bnd_azilok),
    _bnd_rnglok(rhs._bnd_rnglok),
    _bnd_azi(rhs._bnd_azi),
    _bnd_rng(rhs._bnd_rng),
    _azi_weight(rhs._azi_weight),
    _rng_weight(rhs._rng_weight),
    _data_inpsrc(rhs._data_inpsrc),
    _rng_res(rhs._rng_res),
    _azi_res(rhs._azi_res),
    _time_dir_pix(rhs._time_dir_pix),
    _time_dir_lin(rhs._time_dir_lin),
    _line_cont(rhs._line_cont),
    _clutter_lock(rhs._clutter_lock),
    _auto_focus(rhs._auto_focus),
    _line_spacing(rhs._line_spacing),
    _pix_spacing(rhs._pix_spacing),
    _rngcmp_desg(rhs._rngcmp_desg),
    _zero_dop_range_time_f_pixel(rhs._zero_dop_range_time_f_pixel),
    _zero_dop_range_time_c_pixel(rhs._zero_dop_range_time_c_pixel),
    _zero_dop_range_time_l_pixel(rhs._zero_dop_range_time_l_pixel),
    _zero_dop_az_time_f_pixel(rhs._zero_dop_az_time_f_pixel),
    _zero_dop_az_time_c_pixel(rhs._zero_dop_az_time_c_pixel),
    _zero_dop_az_time_l_pixel(rhs._zero_dop_az_time_l_pixel)
{
  _ellip_j[0] = rhs._ellip_j[0];
  _ellip_j[1] = rhs._ellip_j[1];
  _ellip_j[2] = rhs._ellip_j[2];
  _ampl_coef[0] = rhs._ampl_coef[0];
  _ampl_coef[1] = rhs._ampl_coef[1];
  _ampl_coef[2] = rhs._ampl_coef[2];
  _ampl_coef[3] = rhs._ampl_coef[3];
  _ampl_coef[4] = rhs._ampl_coef[4];
  _phas_coef[0] = rhs._phas_coef[0];
  _phas_coef[1] = rhs._phas_coef[1];
  _phas_coef[2] = rhs._phas_coef[2];
  _phas_coef[3] = rhs._phas_coef[3];
  _phas_coef[4] = rhs._phas_coef[4];

  _alt_dopcen[0] = rhs._alt_dopcen[0];
  _alt_dopcen[1] = rhs._alt_dopcen[1];
  _alt_dopcen[2] = rhs._alt_dopcen[2];

  _crt_dopcen[0] = rhs._crt_dopcen[0];
  _crt_dopcen[1] = rhs._crt_dopcen[1];
  _crt_dopcen[2] = rhs._crt_dopcen[2];

  _alt_rate[0] = rhs._alt_rate[0];
  _alt_rate[1] = rhs._alt_rate[1];
  _alt_rate[2] = rhs._alt_rate[2];
  _crt_rate[0] = rhs._crt_rate[0];
  _crt_rate[1] = rhs._crt_rate[1];
  _crt_rate[2] = rhs._crt_rate[2];
}

ErsSarDataSetSummary& ErsSarDataSetSummary::operator=(const ErsSarDataSetSummary& rhs)
{
  _seq_num = rhs._seq_num;
  _sar_chn = rhs._sar_chn;
  _scene_id = rhs._scene_id;
  _scene_des = rhs._scene_des;
  _inp_sctim = rhs._inp_sctim;
  _asc_des = rhs._asc_des;
  _pro_lat = rhs._pro_lat;
  _pro_long = rhs._pro_long;
  _pro_head = rhs._pro_head;
  _ellip_des = rhs._ellip_des;
  _ellip_maj = rhs._ellip_maj;
  _ellip_min = rhs._ellip_min;
  _earth_mass = rhs._earth_mass;
  _grav_const = rhs._grav_const;
  _terrain_h = rhs._terrain_h;
  _sc_lin = rhs._sc_lin;
  _sc_pix = rhs._sc_pix;
  _scene_len = rhs._scene_len;
  _scene_wid = rhs._scene_wid;
  _nchn = rhs._nchn;
  _mission_id = rhs._mission_id;
  _sensor_id = rhs._sensor_id;
  _orbit_num = rhs._orbit_num;
  _plat_lat = rhs._plat_lat;
  _plat_long = rhs._plat_long;
  _plat_head = rhs._plat_head;
  _clock_ang = rhs._clock_ang;
  _incident_ang = rhs._incident_ang;
  _wave_length = rhs._wave_length;
  _motion_comp = rhs._motion_comp;
  _pulse_code = rhs._pulse_code;
  _chirp_ext_ind = rhs._chirp_ext_ind;
  _fr = rhs._fr;
  _rng_gate = rhs._rng_gate;
  _rng_length = rhs._rng_length;
  _baseband_f = rhs._baseband_f;
  _rngcmp_f = rhs._rngcmp_f;
  _gn_polar = rhs._gn_polar;
  _gn_cross = rhs._gn_cross;
  _chn_bits = rhs._chn_bits;
  _quant_desc = rhs._quant_desc;
  _i_bias = rhs._i_bias;
  _q_bias = rhs._q_bias;
  _iq_ratio = rhs._iq_ratio;
  _mech_sight = rhs._mech_sight;
  _fa = rhs._fa;
  _sat_bintim = rhs._sat_bintim;
  _sat_clktim = rhs._sat_clktim;
  _sat_clkinc = rhs._sat_clkinc;
  _fac_id = rhs._fac_id;
  _sys_id = rhs._sys_id;
  _ver_id = rhs._ver_id;
  _prod_type = rhs._prod_type;
  _algor_id = rhs._algor_id;
  _n_azilok = rhs._n_azilok;
  _n_rnglok = rhs._n_rnglok;
  _bnd_azilok = rhs._bnd_azilok;
  _bnd_rnglok = rhs._bnd_rnglok;
  _bnd_azi = rhs._bnd_azi;
  _bnd_rng = rhs._bnd_rng;
  _azi_weight = rhs._azi_weight;
  _rng_weight = rhs._rng_weight;
  _data_inpsrc = rhs._data_inpsrc;
  _rng_res = rhs._rng_res;
  _azi_res = rhs._azi_res;
  _time_dir_pix = rhs._time_dir_pix;
  _time_dir_lin = rhs._time_dir_lin;
  _line_cont = rhs._line_cont;
  _clutter_lock = rhs._clutter_lock;
  _auto_focus = rhs._auto_focus;
  _line_spacing = rhs._line_spacing;
  _pix_spacing = rhs._pix_spacing;
  _rngcmp_desg = rhs._rngcmp_desg;

  _ellip_j[0] = rhs._ellip_j[0];
  _ellip_j[1] = rhs._ellip_j[1];
  _ellip_j[2] = rhs._ellip_j[2];
  _ampl_coef[0] = rhs._ampl_coef[0];
  _ampl_coef[1] = rhs._ampl_coef[1];
  _ampl_coef[2] = rhs._ampl_coef[2];
  _ampl_coef[3] = rhs._ampl_coef[3];
  _ampl_coef[4] = rhs._ampl_coef[4];
  _phas_coef[0] = rhs._phas_coef[0];
  _phas_coef[1] = rhs._phas_coef[1];
  _phas_coef[2] = rhs._phas_coef[2];
  _phas_coef[3] = rhs._phas_coef[3];
  _phas_coef[4] = rhs._phas_coef[4];

  _alt_dopcen[0] = rhs._alt_dopcen[0];
  _alt_dopcen[1] = rhs._alt_dopcen[1];
  _alt_dopcen[2] = rhs._alt_dopcen[2];

  _crt_dopcen[0] = rhs._crt_dopcen[0];
  _crt_dopcen[1] = rhs._crt_dopcen[1];
  _crt_dopcen[2] = rhs._crt_dopcen[2];

  _alt_rate[0] = rhs._alt_rate[0];
  _alt_rate[1] = rhs._alt_rate[1];
  _alt_rate[2] = rhs._alt_rate[2];
  _crt_rate[0] = rhs._crt_rate[0];
  _crt_rate[1] = rhs._crt_rate[1];
  _crt_rate[2] = rhs._crt_rate[2];

  _zero_dop_range_time_f_pixel = rhs._zero_dop_range_time_f_pixel;
  _zero_dop_range_time_c_pixel = rhs._zero_dop_range_time_c_pixel;
  _zero_dop_range_time_l_pixel = rhs._zero_dop_range_time_l_pixel;
  _zero_dop_az_time_f_pixel = rhs._zero_dop_az_time_f_pixel;
  _zero_dop_az_time_c_pixel = rhs._zero_dop_az_time_c_pixel;
  _zero_dop_az_time_l_pixel = rhs._zero_dop_az_time_l_pixel;


  return *this;
}
}
