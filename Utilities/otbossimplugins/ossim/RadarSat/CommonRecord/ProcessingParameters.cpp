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

#include "ProcessingParameters.h"

namespace ossimplugins
{
   ProcessingParameters::ProcessingParameters() : RadarSatRecord("proc_param_rec")
   {
   }

   ProcessingParameters::~ProcessingParameters()
   {
   }

   std::ostream& operator<<(std::ostream& os, const ProcessingParameters& data)
   {
      os<<"rec_seq:"<<data._rec_seq<<std::endl;

      os<<"inp_media:"<<data._inp_media.c_str()<<std::endl;

      os<<"n_tape_id:"<<data._n_tape_id<<std::endl;

      for (int i=0;i<10;i++)
      {
         os<<"tape_id["<<i<<"]:"<<data._tape_id[i].c_str()<<std::endl;
      }

      os<<"exp_ing_start:"<<data._exp_ing_start.c_str()<<std::endl;

      os<<"exp_ing_stop:"<<data._exp_ing_stop.c_str()<<std::endl;

      os<<"act_ing_start:"<<data._act_ing_start.c_str()<<std::endl;

      os<<"act_ing_stop:"<<data._act_ing_stop.c_str()<<std::endl;

      os<<"proc_start:"<<data._proc_start.c_str()<<std::endl;

      os<<"proc_stop:"<<data._proc_stop.c_str()<<std::endl;

      for (int i=0;i<10;i++)
      {
         os<<"mn_sig_lev["<<i<<"]:"<<data._mn_sig_lev[i]<<std::endl;
      }

      os<<"scr_data_ind:"<<data._scr_data_ind<<std::endl;

      os<<"miss_ln:"<<data._miss_ln<<std::endl;

      os<<"rej_ln:"<<data._rej_ln<<std::endl;

      os<<"large_gap:"<<data._large_gap<<std::endl;

      os<<"bit_err_rate:"<<data._bit_err_rate<<std::endl;

      os<<"fm_crc_err:"<<data._fm_crc_err<<std::endl;

      os<<"date_incons:"<<data._date_incons<<std::endl;

      os<<"prf_changes:"<<data._prf_changes<<std::endl;

      os<<"delay_changes:"<<data._delay_changes<<std::endl;

      os<<"skipd_frams:"<<data._skipd_frams<<std::endl;

      os<<"rej_bf_start:"<<data._rej_bf_start<<std::endl;

      os<<"rej_few_fram:"<<data._rej_few_fram<<std::endl;

      os<<"rej_many_fram:"<<data._rej_many_fram<<std::endl;

      os<<"rej_mchn_err:"<<data._rej_mchn_err<<std::endl;

      os<<"rej_vchn_err:"<<data._rej_vchn_err<<std::endl;

      os<<"rej_rec_type:"<<data._rej_rec_type<<std::endl;

      os<<"sens_config:"<<data._sens_config.c_str()<<std::endl;

      os<<"sens_orient:"<<data._sens_orient.c_str()<<std::endl;

      os<<"sych_marker:"<<data._sych_marker.c_str()<<std::endl;

      os<<"rng_ref_src:"<<data._rng_ref_src.c_str()<<std::endl;

      for (int i=0;i<4;i++)
      {
         os<<"rng_amp_coef["<<i<<"]:"<<data._rng_amp_coef[i]<<std::endl;
      }

      for (int i=0;i<4;i++)
      {
         os<<"rng_phas_coef["<<i<<"]:"<<data._rng_phas_coef[i]<<std::endl;
      }

      for (int i=0;i<4;i++)
      {
         os<<"err_amp_coef["<<i<<"]:"<<data._err_amp_coef[i]<<std::endl;
      }

      for (int i=0;i<4;i++)
      {
         os<<"err_phas_coef["<<i<<"]:"<<data._err_phas_coef[i]<<std::endl;
      }

      os<<"pulse_bandw:"<<data._pulse_bandw<<std::endl;


      os<<"adc_samp_rate:"<<data._adc_samp_rate.c_str()<<std::endl;

      os<<"rep_agc_attn:"<<data._rep_agc_attn<<std::endl;

      os<<"gn_corctn_fctr:"<<data._gn_corctn_fctr<<std::endl;

      os<<"rep_energy_gn:"<<data._rep_energy_gn<<std::endl;

      os<<"orb_data_src:"<<data._orb_data_src.c_str()<<std::endl;

      os<<"pulse_cnt_1:"<<data._pulse_cnt_1<<std::endl;

      os<<"pulse_cnt_2:"<<data._pulse_cnt_2<<std::endl;

      os<<"beam_edge_rqd:"<<data._beam_edge_rqd.c_str()<<std::endl;

      os<<"beam_edge_conf:"<<data._beam_edge_conf<<std::endl;

      os<<"pix_overlap:"<<data._pix_overlap<<std::endl;

      os<<"n_beams:"<<data._n_beams<<std::endl;

      for (int i=0;i<4;i++)
      {
         os<<"beam_info["<<i<<"]:"<<data._beam_info[i]<<std::endl;
      }

      os<<"n_pix_updates:"<<data._n_pix_updates<<std::endl;

      for (int i=0;i<4;i++)
      {
         os<<"pix_count["<<i<<"]:"<<data._pix_count[i]<<std::endl;
      }

      os<<"pwin_start:"<<data._pwin_start<<std::endl;

      os<<"pwin_end:"<<data._pwin_end<<std::endl;

      os<<"recd_type:"<<data._recd_type.c_str()<<std::endl;

      os<<"temp_set_inc:"<<data._temp_set_inc<<std::endl;

      os<<"n_temp_set:"<<data._n_temp_set<<std::endl;

      for (int i=0;i<20;i++)
      {
         os<<"temp["<<i<<"]:"<<data._temp[i]<<std::endl;
      }

      os<<"n_image_pix:"<<data._n_image_pix<<std::endl;

      os<<"prc_zero_pix:"<<data._prc_zero_pix<<std::endl;

      os<<"prc_satur_pix:"<<data._prc_satur_pix<<std::endl;

      os<<"img_hist_mean:"<<data._img_hist_mean<<std::endl;

      for (int i=0;i<3;i++)
      {
         os<<"img_cumu_dist["<<i<<"]:"<<data._img_cumu_dist[i]<<std::endl;
      }

      os<<"pre_img_gn:"<<data._pre_img_gn<<std::endl;

      os<<"post_img_gn:"<<data._post_img_gn<<std::endl;

      os<<"dopcen_inc:"<<data._dopcen_inc<<std::endl;

      os<<"n_dopcen:"<<data._n_dopcen<<std::endl;

      for (int i=0;i<20;i++)
      {
         os<<"dopcen_est["<<i<<"]:"<<data._dopcen_est[i]<<std::endl;
      }

      os<<"dop_amb_err:"<<data._dop_amb_err<<std::endl;

      os<<"dopamb_conf:"<<data._dopamb_conf<<std::endl;

      for (int i=0;i<7;i++)
      {
         os<<"eph_orb_data["<<i<<"]:"<<data._eph_orb_data[i]<<std::endl;
      }

      os<<"appl_type:"<<data._appl_type.c_str()<<std::endl;

      for (int i=0;i<5;i++)
      {
         os<<"slow_time_coef["<<i<<"]:"<<data._slow_time_coef[i]<<std::endl;
      }

      os<<"n_srgr:"<<data._n_srgr<<std::endl;

      for (int i=0;i<20;i++)
      {
         os<<"srgr_coefset["<<i<<"]:"<<data._srgr_coefset[i]<<std::endl;
      }

      os<<"pixel_spacing:"<<data._pixel_spacing<<std::endl;

      os<<"gics_reqd:"<<data._gics_reqd.c_str()<<std::endl;

      os<<"wo_number:"<<data._wo_number.c_str()<<std::endl;

      os<<"wo_date:"<<data._wo_date.c_str()<<std::endl;

      os<<"satellite_id:"<<data._satellite_id.c_str()<<std::endl;

      os<<"user_id:"<<data._user_id.c_str()<<std::endl;

      os<<"complete_msg:"<<data._complete_msg.c_str()<<std::endl;

      os<<"scene_id:"<<data._scene_id.c_str()<<std::endl;

      os<<"density_in:"<<data._density_in.c_str()<<std::endl;

      os<<"media_id:"<<data._media_id.c_str()<<std::endl;

      os<<"angle_first:"<<data._angle_first<<std::endl;

      os<<"angle_last:"<<data._angle_last<<std::endl;

      os<<"prod_type:"<<data._prod_type.c_str()<<std::endl;

      os<<"map_system:"<<data._map_system.c_str()<<std::endl;

      os<<"centre_lat:"<<data._centre_lat<<std::endl;

      os<<"centre_long:"<<data._centre_long<<std::endl;

      os<<"span_x:"<<data._span_x<<std::endl;

      os<<"span_y:"<<data._span_y<<std::endl;

      os<<"apply_dtm:"<<data._apply_dtm.c_str()<<std::endl;

      os<<"density_out:"<<data._density_out.c_str()<<std::endl;

      os<<"state_time:"<<data._state_time.c_str()<<std::endl;

      os<<"num_state_vectors:"<<data._num_state_vectors<<std::endl;

      os<<"state_time_inc:"<<data._state_time_inc<<std::endl;

      return os;
   }

   std::istream& operator>>(std::istream& is, ProcessingParameters& data)
   {
      char buf[2407];
      buf[2406] = '\0';

      is.read(buf,4);
      buf[4] = '\0';
      data._rec_seq = atoi(buf);

      is.read(buf,4);
      buf[4] = '\0';

      is.read(buf,3);
      buf[3] = '\0';
      data._inp_media = buf;

      is.read(buf,4);
      buf[4] = '\0';
      data._n_tape_id = atoi(buf);

      for (int i=0;i<10;i++)
      {
         is.read(buf,8);
         buf[8] = '\0';
         data._tape_id[i] = buf;
      }

      is.read(buf,21);
      buf[21] = '\0';
      data._exp_ing_start = buf;

      is.read(buf,21);
      buf[21] = '\0';
      data._exp_ing_stop = buf;

      is.read(buf,21);
      buf[21] = '\0';
      data._act_ing_start = buf;

      is.read(buf,21);
      buf[21] = '\0';
      data._act_ing_stop = buf;

      is.read(buf,21);
      buf[21] = '\0';
      data._proc_start = buf;

      is.read(buf,21);
      buf[21] = '\0';
      data._proc_stop = buf;

      for (int i=0;i<10;i++)
      {
         is.read(buf,16);
         buf[16] = '\0';
         data._mn_sig_lev[i] = atof(buf);
      }

      is.read(buf,4);
      buf[4] = '\0';
      data._scr_data_ind = atoi(buf);

      is.read(buf,8);
      buf[8] = '\0';
      data._miss_ln = atoi(buf);

      is.read(buf,8);
      buf[8] = '\0';
      data._rej_ln = atoi(buf);

      is.read(buf,8);
      buf[8] = '\0';
      data._large_gap = atoi(buf);

      is.read(buf,16);
      buf[16] = '\0';
      data._bit_err_rate = atof(buf);

      is.read(buf,16);
      buf[16] = '\0';
      data._fm_crc_err = atof(buf);

      is.read(buf,8);
      buf[8] = '\0';
      data._date_incons = atoi(buf);

      is.read(buf,8);
      buf[8] = '\0';
      data._prf_changes = atoi(buf);

      is.read(buf,8);
      buf[8] = '\0';
      data._delay_changes = atoi(buf);

      is.read(buf,8);
      buf[8] = '\0';
      data._skipd_frams = atoi(buf);

      is.read(buf,8);
      buf[8] = '\0';
      data._rej_bf_start = atoi(buf);

      is.read(buf,8);
      buf[8] = '\0';
      data._rej_few_fram = atoi(buf);

      is.read(buf,8);
      buf[8] = '\0';
      data._rej_many_fram = atoi(buf);

      is.read(buf,8);
      buf[8] = '\0';
      data._rej_mchn_err = atoi(buf);

      is.read(buf,8);
      buf[8] = '\0';
      data._rej_vchn_err = atoi(buf);

      is.read(buf,8);
      buf[8] = '\0';
      data._rej_rec_type = atoi(buf);

      is.read(buf,10);
      buf[10] = '\0';
      data._sens_config = buf;

      is.read(buf,9);
      buf[9] = '\0';
      data._sens_orient = buf;

      is.read(buf,8);
      buf[8] = '\0';
      data._sych_marker = buf;

      is.read(buf,12);
      buf[12] = '\0';
      data._rng_ref_src = buf;

      for (int i=0;i<4;i++)
      {
         is.read(buf,16);
         buf[16] = '\0';
         data._rng_amp_coef[i] = atof(buf);
      }

      for (int i=0;i<4;i++)
      {
         is.read(buf,16);
         buf[16] = '\0';
         data._rng_phas_coef[i] = atof(buf);
      }

      for (int i=0;i<4;i++)
      {
         is.read(buf,16);
         buf[16] = '\0';
         data._err_amp_coef[i] = atof(buf);
      }

      for (int i=0;i<4;i++)
      {
         is.read(buf,16);
         buf[16] = '\0';
         data._err_phas_coef[i] = atof(buf);
      }

      is.read(buf,4);
      buf[4] = '\0';
      data._pulse_bandw = atoi(buf);


      is.read(buf,5);
      buf[5] = '\0';
      data._adc_samp_rate = buf;

      is.read(buf,16);
      buf[16] = '\0';
      data._rep_agc_attn = atof(buf);

      is.read(buf,16);
      buf[16] = '\0';
      data._gn_corctn_fctr = atof(buf);

      is.read(buf,16);
      buf[16] = '\0';
      data._rep_energy_gn = atof(buf);

      is.read(buf,11);
      buf[11] = '\0';
      data._orb_data_src = buf;

      is.read(buf,4);
      buf[4] = '\0';
      data._pulse_cnt_1 = atoi(buf);

      is.read(buf,4);
      buf[4] = '\0';
      data._pulse_cnt_2 = atoi(buf);

      is.read(buf,3);
      buf[3] = '\0';
      data._beam_edge_rqd = buf;

      is.read(buf,16);
      buf[16] = '\0';
      data._beam_edge_conf = atof(buf);

      is.read(buf,4);
      buf[4] = '\0';
      data._pix_overlap = atoi(buf);

      is.read(buf,4);
      buf[4] = '\0';
      data._n_beams = atoi(buf);

      for (int i=0;i<4;i++)
      {
         is>>data._beam_info[i];
      }

      is.read(buf,4);
      buf[4] = '\0';
      data._n_pix_updates = atoi(buf);

      for (int i=0;i<20;i++)
      {
         is>>data._pix_count[i];
      }

      is.read(buf,16);
      buf[16] = '\0';
      data._pwin_start = atof(buf);

      is.read(buf,16);
      buf[16] = '\0';
      data._pwin_end = atof(buf);

      is.read(buf,9);
      buf[9] = '\0';
      data._recd_type = buf;

      is.read(buf,16);
      buf[16] = '\0';
      data._temp_set_inc = atof(buf);

      is.read(buf,4);
      buf[4] = '\0';
      data._n_temp_set = atoi(buf);

      for (int i=0;i<20;i++)
      {
         is>>data._temp[i];
      }

      is.read(buf,8);
      buf[8] = '\0';
      data._n_image_pix = atoi(buf);

      is.read(buf,16);
      buf[16] = '\0';
      data._prc_zero_pix = atof(buf);

      is.read(buf,16);
      buf[16] = '\0';
      data._prc_satur_pix = atof(buf);

      is.read(buf,16);
      buf[16] = '\0';
      data._img_hist_mean = atof(buf);

      for (int i=0;i<3;i++)
      {
         is.read(buf,16);
         buf[16] = '\0';
         data._img_cumu_dist[i] = atof(buf);
      }

      is.read(buf,16);
      buf[16] = '\0';
      data._pre_img_gn = atof(buf);

      is.read(buf,16);
      buf[16] = '\0';
      data._post_img_gn = atof(buf);

      is.read(buf,16);
      buf[16] = '\0';
      data._dopcen_inc = atof(buf);

      is.read(buf,4);
      buf[4] = '\0';
      data._n_dopcen = atoi(buf);

      for (int i=0;i<20;i++)
      {
         is>>data._dopcen_est[i];
      }

      is.read(buf,4);
      buf[4] = '\0';
      data._dop_amb_err = atoi(buf);

      is.read(buf,16);
      buf[16] = '\0';
      data._dopamb_conf = atof(buf);

      for (int i=0;i<7;i++)
      {
         is.read(buf,16);
         buf[16] = '\0';
         data._eph_orb_data[i] = atof(buf);
      }

      is.read(buf,12);
      buf[12] = '\0';
      data._appl_type = buf;

      for (int i=0;i<5;i++)
      {
         is.read(buf,22);
         buf[22] = '\0';
         data._slow_time_coef[i] = atof(buf);
      }

      is.read(buf,4);
      buf[4] = '\0';
      data._n_srgr = atoi(buf);

      for (int i=0;i<20;i++)
      {
         is>>data._srgr_coefset[i];
      }

      is.read(buf,16);
      buf[16] = '\0';
      data._pixel_spacing = atof(buf);

      is.read(buf,3);
      buf[3] = '\0';
      data._gics_reqd = buf;

      is.read(buf,8);
      buf[8] = '\0';
      data._wo_number = buf;

      is.read(buf,20);
      buf[20] = '\0';
      data._wo_date = buf;

      is.read(buf,10);
      buf[10] = '\0';
      data._satellite_id = buf;

      is.read(buf,20);
      buf[20] = '\0';
      data._user_id = buf;

      is.read(buf,3);
      buf[3] = '\0';
      data._complete_msg = buf;

      is.read(buf,15);
      buf[15] = '\0';
      data._scene_id = buf;

      is.read(buf,4);
      buf[4] = '\0';
      data._density_in = buf;

      is.read(buf,8);
      buf[8] = '\0';
      data._media_id = buf;

      is.read(buf,16);
      buf[16] = '\0';
      data._angle_first = atof(buf);

      is.read(buf,16);
      buf[16] = '\0';
      data._angle_last = atof(buf);

      is.read(buf,3);
      buf[3] = '\0';
      data._prod_type = buf;

      is.read(buf,16);
      buf[16] = '\0';
      data._map_system = buf;

      is.read(buf,22);
      buf[22] = '\0';
      data._centre_lat = atof(buf);

      is.read(buf,22);
      buf[22] = '\0';
      data._centre_long = atof(buf);

      is.read(buf,22);
      buf[22] = '\0';
      data._span_x = atof(buf);

      is.read(buf,22);
      buf[22] = '\0';
      data._span_y = atof(buf);

      is.read(buf,3);
      buf[3] = '\0';
      data._apply_dtm = buf;

      is.read(buf,4);
      buf[4] = '\0';
      data._density_out = buf;

      is.read(buf,21);
      buf[21] = '\0';
      data._state_time = buf;

      is.read(buf,4);
      buf[4] = '\0';
      data._num_state_vectors = atoi(buf);

      is.read(buf,16);
      buf[16] = '\0';
      data._state_time_inc = atof(buf);

      is.read(buf,206);
      buf[206] = '\0';

      return is;
   }

   ProcessingParameters::ProcessingParameters(const ProcessingParameters& rhs):
      RadarSatRecord(rhs)
   {

   }

   ProcessingParameters& ProcessingParameters::operator=(const ProcessingParameters& /* rhs */)
   {
      /*
	int   _rec_seq

	std::string   _spare1

	std::string  _inp_media

	int   _n_tape_id

	std::string   _tape_id

	std::string   _exp_ing_start

	std::string   _exp_ing_stop

	std::string   _act_ing_start

	std::string   _act_ing_stop

	std::string   _proc_start

	std::string   _proc_stop

	double*   _mn_sig_lev

	int   _scr_data_ind

	int   _miss_ln

	int   _rej_ln

	int   _large_gap

	double   _bit_err_rate

	double   _fm_crc_err

	int   _date_incons

	int   _prf_changes

	int   _delay_changes

	int   _skipd_frams

	int   _rej_bf_start

	int   _rej_few_fram

	int   _rej_many_fram

	int   _rej_mchn_err

	int   _rej_vchn_err

	int   _rej_rec_type

	std::string   _sens_config

	std::string   _sens_orient

	int   _sych_marker

	std::string   _rng_ref_src

	double*   _rng_amp_coef

	double*   _rng_phas_coef

	double*   _err_amp_coef

	double*   _err_phas_coef

	int   _pulse_bandw

	int   _adc_samp_rate

	double   _rep_agc_attn

	double   _gn_corctn_fctr

	double   _rep_energy_gn

	std::string   _orb_data_src

	int   _pulse_cnt_1

	int   _pulse_cnt_2

	std::string    _beam_edge_rqd

	double   _beam_edge_conf

	int   _pix_overlap

	int   _n_beams

	BeamInformationRecord* _beam_info

	int   _n_pix_updates

	BeamPixelCountRecord* _pix_count

	double   _pwin_start

	double   _pwin_end

	std::string   _recd_type

	double   _temp_set_inc

	int*   _n_temp_set

	TemperatureSettingsRecord* _temp

	int   _n_image_pix

	double   _prc_zero_pix

	double   _prc_satur_pix

	double   _img_hist_mean

	double*   _img_cumu_dist

	double   _pre_img_gn

	double   _post_img_gn

	double   _dopcen_inc

	int   _n_dopcen

	DopplerCentroidEstimateRecord* _dopcen_est

	int   _dop_amb_err

	double   _dopamb_conf

	double*   _eph_orb_data

	std::string   _appl_type

	double*   _slow_time_coef

	int   _n_srgr

	SRGRCoefficientSetRecord* _srgr_coefset

	double   _pixel_spacing

	std::string   _gics_reqd

	std::string   _wo_number

	std::string   _wo_date

	std::string   _satellite_id

	std::string   _user_id

	std::string   _complete_msg

	std::string   _scene_id

	std::string   _density_in

	std::string   _media_id

	double   _angle_first

	double   _angle_last

	std::string   _prod_type

	std::string   _map_system

	double   _centre_lat

	double   _centre_long

	double   _span_x

	double   _span_y

	std::string   _apply_dtm

	std::string   _density_out

	std::string   _state_time

	int   _num_state_vectors

	double   _state_time_inc

	std::string   _spare2*/
      return *this;
   }
}
