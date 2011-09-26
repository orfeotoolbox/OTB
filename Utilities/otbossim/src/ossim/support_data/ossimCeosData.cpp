//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Oscar Kramer
//
// Description:
//
// Class for reading CEOS-formatted metadata.
// 
//*******************************************************************
//  $Id:$

#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;

#include <ossim/support_data/ossimCeosData.h>
#include "ossim/base/ossimCommon.h"
#include <ossim/base/ossimTrace.h>

// Define Trace flags for use within this file:
static ossimTrace traceExec  ("ossimCeosData:exec");
static ossimTrace traceDebug ("ossimCeosData:debug");

static const ossimFilename   CEOS_VOL_DIR_FILENAME  = "vdf_dat.001";
static const ossimFilename   CEOS_LEADER_FILENAME   = "lea_01.001";
static const ossimFilename   CEOS_IMAGE_FILENAME    = "dat_01.001";
static const ossimFilename   CEOS_TRAILER_FILENAME  = "tra_01.001";


//******************************************************************************
//  CONSTRUCTOR: 
//******************************************************************************
ossimCeosData::ossimCeosData(const ossimFilename& volDirFile,
                   const ossimFilename& leaderFile,
                   const ossimFilename& imageFile,
                   const ossimFilename& trailerFile)
   :
      theVolDirFileName  (volDirFile),
      theLeaderFileName  (leaderFile),
      theImageFileName   (imageFile),
      theTrailerFileName (trailerFile),
      theErrorStatus     (OK),
      theVolDescRec      (0),
      theTextRec         (0),
      theDataSetSumRec   (0),
      theQualSumRec      (0),
      theSdrHistRec      (0),
      thePdr16HistRec    (0),
      thePdr8HistRec     (0),
      theProcParmRec     (0),
      theMapProjRec      (0),
      thePosDataRec      (0),
      theAttDataRec      (0),
      theRadiDataRec     (0),
      theRadiCompRec     (0)
{
   static const char MODULE[] = "ossimCeosData Constructor #1";
   if (traceExec())  CLOG << "entering..." << endl;

   loadCeosRecords();
   
   if (traceExec())  CLOG << "returning..." << endl;
}


//******************************************************************************
//  CONSTRUCTOR: ossimCeosData(ossimFilename)
//  
//  Initializes given the image directory (assumes default filenames).
//  
//******************************************************************************
ossimCeosData::ossimCeosData(const ossimFilename& imageDir)
   :
      theErrorStatus   (OK),
      theVolDescRec    (0),
      theTextRec       (0),
      theDataSetSumRec (0),
      theQualSumRec    (0),
      theSdrHistRec    (0),
      thePdr16HistRec  (0),
      thePdr8HistRec   (0),
      theProcParmRec   (0),
      theMapProjRec    (0),
      thePosDataRec    (0),
      theAttDataRec    (0),
      theRadiDataRec   (0),
      theRadiCompRec   (0)
{
   static const char MODULE[] = "ossimCeosData Constructor #1";
   if (traceExec())  CLOG << "entering..." << endl;

   //***
   // Assign values to ossimFilename data members:
   //***
   theVolDirFileName  = imageDir.dirCat(CEOS_VOL_DIR_FILENAME);
   theLeaderFileName  = imageDir.dirCat(CEOS_LEADER_FILENAME);
   theImageFileName   = imageDir.dirCat(CEOS_IMAGE_FILENAME);
   theTrailerFileName = imageDir.dirCat(CEOS_TRAILER_FILENAME);

   loadCeosRecords();
   
   if (traceExec())  CLOG << "returning..." << endl;
}


//******************************************************************************
// PUBLIC METHOD: ossimCeosData::dump(ostream&)
//  
//******************************************************************************
void ossimCeosData::dump(ostream& stream) const 
{
   static const char MODULE[] = "ossimCeosData::dump(ostream& stream)";
   if (traceExec())  CLOG << "entering..." << endl;

   stream << "\nDump of ossimCeosData Object -----------------------------" << endl;

   if (theVolDescRec)
      dumpVolDescRec(stream);
   else
      stream << "\n\n\tvol_desc_rec not present." << endl;

   if (theTextRec)
      dumpTextRec(stream);
   else
      stream << "\n\n\ttext_rec not present." << endl;

   if (imopDescRec())
      dumpImopDescRec(stream);
   else
      stream << "\n\n\timop_desc_rec not present." << endl;

   if (theDataSetSumRec)
      dumpDataSetSumRec(stream);
   else
      stream << "\n\n\tdataset_sum_rec not present." << endl;

   if (theQualSumRec)
      dumpQualSumRec(stream);
   else
      stream << "\n\n\tqual_sum_rec not present." << endl;

   if (theSdrHistRec)
      dumpSdrHistRec(stream);
   else
      stream << "\n\n\tsdr_hist_rec not present." << endl;

   if (thePdr16HistRec)
      dumpPdr16HistRec(stream);
   else
      stream << "\n\n\tpdr16_hist_rec not present." << endl;

   if (thePdr8HistRec)
      dumpPdr8HistRec(stream);
   else
      stream << "\n\n\tpdr8_hist_rec not present." << endl;

   if (theProcParmRec)
      dumpProcParmRec(stream);
   else
      stream << "\n\n\tproc_parm_rec not present." << endl;

   if (theMapProjRec)
      dumpMapProjRec(stream);
   else
      stream << "\n\n\tmap_proj_rec not present." << endl;

   if (thePosDataRec)
      dumpPosDataRec(stream);
   else
      stream << "\n\n\tpos_data_rec not present." << endl;

   if (theAttDataRec)
      dumpAttDataRec(stream);
   else
      stream << "\n\n\tatt_data_rec not present." << endl;

   if (theRadiDataRec)
      dumpRadiDataRec(stream);
   else
      stream << "\n\n\tradi_data_rec not present." << endl;

   if (theRadiCompRec)
      dumpRadiCompRec(stream);
   else
      stream << "\n\n\tradi_comp_rec not present." << endl;

   stream << "\nEnd of ossimCeosData Object Dump -------------------------" << endl;

   if (traceExec())  CLOG << "returning..." << endl;
   return;
}

//******************************************************************************
// PRIVATE METHOD: ossimCeosData::dumpVolDescRec()
//  
//******************************************************************************
void ossimCeosData::dumpVolDescRec(ostream& stream) const
{
   const vol_desc_rec* p = theVolDescRec;

   stream << "\n\n\tContents of vol_desc_rec:\n";
   stream << "\n\t  ascii_flag         \""; stream.write(p->ascii_flag, 2);
   stream << "\"\n\t  format_doc         \""; stream.write(p->format_doc, 12);
   stream << "\"\n\t  format_ver         \""; stream.write(p->format_ver, 2);
   stream << "\"\n\t  format_rev         \""; stream.write(p->format_rev, 2);
   stream << "\"\n\t  software_id        \""; stream.write(p->software_id, 12);
   stream << "\"\n\t  phyvol_id          \""; stream.write(p->phyvol_id, 16);
   stream << "\"\n\t  logvol_id          \""; stream.write(p->logvol_id, 16);
   stream << "\"\n\t  volset_id          \""; stream.write(p->volset_id, 16);
   stream << "\"\n\t  phyvol_cnt         \""; stream.write(p->phyvol_cnt, 2);
   stream << "\"\n\t  first_phyvol       \""; stream.write(p->first_phyvol, 2);
   stream << "\"\n\t  last_phyvol        \""; stream.write(p->last_phyvol, 2);
   stream << "\"\n\t  curr_phyvol        \""; stream.write(p->curr_phyvol, 2);
   stream << "\"\n\t  first_file         \""; stream.write(p->first_file, 4);
   stream << "\"\n\t  volset_log         \""; stream.write(p->volset_log, 4);
   stream << "\"\n\t  phyvol_log         \""; stream.write(p->phyvol_log, 4);
   stream << "\"\n\t  logvol_date        \""; stream.write(p->logvol_date, 8);
   stream << "\"\n\t  logvol_time        \""; stream.write(p->logvol_time, 8);
   stream << "\"\n\t  logvol_country     \"";
   stream.write(p->logvol_country, 12);
   stream << "\"\n\t  logvol_agency      \""; stream.write(p->logvol_agency, 8);
   stream << "\"\n\t  logvol_facility    \"";
   stream.write(p->logvol_facility,12);
   stream << "\"\n\t  n_filepoint        \""; stream.write(p->n_filepoint, 4);
   stream << "\"\n\t  n_voldir           \""; stream.write(p->n_voldir, 4);
   stream << "\"\n\t  product_id         \""; stream.write(p->product_id, 8);

   stream << "\"" << endl;
}

//******************************************************************************
// PRIVATE METHOD: ossimCeosData::dumpTextRec()
//  
//******************************************************************************
void ossimCeosData::dumpTextRec(ostream& stream) const
{
   const text_rec* p = theTextRec;
   
   stream << "\n\n\tContents of text_rec:\n\t";
   stream << "\n\t  ascii_flag         \""; stream.write(p->ascii_flag, 2);
   stream << "\"\n\t  cont_flag          \""; stream.write(p->cont_flag, 2);
   stream << "\"\n\t  product_type       \""; stream.write(p->product_type, 40);
   stream << "\"\n\t  product_create     \"";
   stream.write(p->product_create, 60);
   stream << "\"\n\t  phyvol_id          \""; stream.write(p->phyvol_id, 40);
   stream << "\"\n\t  scene_id           \""; stream.write(p->scene_id, 40);
   stream << "\"\n\t  scene_loc          \""; stream.write(p->scene_loc, 40);
   stream << "\"" << endl;
}

//******************************************************************************
// PRIVATE METHOD: ossimCeosData::dumpImopDescRec()
//  
//******************************************************************************
void ossimCeosData::dumpImopDescRec(ostream& stream) const
{
   const imop_desc_rec* p = &theImopDescRec;
   
   stream << "\n\n\tContents of imop_desc_rec:\n\t";
   stream << "\n\t  ascii_flag         \""; stream.write(p->ascii_flag, 2);
   stream << "\"\n\t  spare1             \""; stream.write(p->spare1, 2);
   stream << "\"\n\t  format_doc         \""; stream.write(p->format_doc, 12);
   stream << "\"\n\t  format_rev         \""; stream.write(p->format_rev, 2);
   stream << "\"\n\t  design_rev         \""; stream.write(p->design_rev, 2);
   stream << "\"\n\t  software_id        \""; stream.write(p->software_id, 12);
   stream << "\"\n\t  file_num           \""; stream.write(p->file_num, 4);
   stream << "\"\n\t  file_name          \""; stream.write(p->file_name, 16);
   stream << "\"\n\t  rec_seq            \""; stream.write(p->rec_seq, 4);
   stream << "\"\n\t  seq_loc            \""; stream.write(p->seq_loc, 8);
   stream << "\"\n\t  seq_len            \""; stream.write(p->seq_len, 4);
   stream << "\"\n\t  rec_code           \""; stream.write(p->rec_code, 4);
   stream << "\"\n\t  code_loc           \""; stream.write(p->code_loc, 8);
   stream << "\"\n\t  code_len           \""; stream.write(p->code_len, 4);
   stream << "\"\n\t  rec_len            \""; stream.write(p->rec_len, 4);
   stream << "\"\n\t  rlen_loc           \""; stream.write(p->rlen_loc, 8);
   stream << "\"\n\t  rlen_len           \""; stream.write(p->rlen_len, 4);
   stream << "\"\n\t  spare2             \""; stream.write(p->spare2, 4);
   stream << "\"\n\t  spare3             \""; stream.write(p->spare3, 64);
   stream << "\"\n\t  n_dataset          \""; stream.write(p->n_dataset, 6);
   stream << "\"\n\t  l_dataset          \""; stream.write(p->l_dataset, 6);
   stream << "\"\n\t  spare4             \""; stream.write(p->spare4, 24);
   stream << "\"\n\t  nbit               \""; stream.write(p->nbit, 4);
   stream << "\"\n\t  nsamp              \""; stream.write(p->nsamp, 4);
   stream << "\"\n\t  nbyte              \""; stream.write(p->nbyte, 4);
   stream << "\"\n\t  justify            \""; stream.write(p->justify, 4);
   stream << "\"\n\t  nchn               \""; stream.write(p->nchn, 4);
   stream << "\"\n\t  nlin               \""; stream.write(p->nlin, 8);
   stream << "\"\n\t  nleft              \""; stream.write(p->nleft, 4);
   stream << "\"\n\t  ngrp               \""; stream.write(p->ngrp, 8);
   stream << "\"\n\t  nright             \""; stream.write(p->nright, 4);
   stream << "\"\n\t  ntop               \""; stream.write(p->ntop, 4);
   stream << "\"\n\t  nbott              \""; stream.write(p->nbott, 4);
   stream << "\"\n\t  intleav            \""; stream.write(p->intleav, 4);
   stream << "\"\n\t  nrec_lin           \""; stream.write(p->nrec_lin, 2);
   stream << "\"\n\t  nrec_chn           \""; stream.write(p->nrec_chn, 2);
   stream << "\"\n\t  n_prefix           \""; stream.write(p->n_prefix, 4);
   stream << "\"\n\t  n_sar              \""; stream.write(p->n_sar, 8);
   stream << "\"\n\t  n_suffix           \""; stream.write(p->n_suffix, 4);
   stream << "\"\n\t  spare5             \""; stream.write(p->spare5, 4);
   stream << "\"\n\t  lin_loc            \""; stream.write(p->lin_loc, 8);
   stream << "\"\n\t  chn_loc            \""; stream.write(p->chn_loc, 8);
   stream << "\"\n\t  tim_loc            \""; stream.write(p->tim_loc, 8);
   stream << "\"\n\t  left_loc           \""; stream.write(p->left_loc, 8);
   stream << "\"\n\t  right_loc          \""; stream.write(p->right_loc, 8);
   stream << "\"\n\t  pad_ind            \""; stream.write(p->pad_ind, 4);
   stream << "\"\n\t  spare6             \""; stream.write(p->spare6, 28);
   stream << "\"\n\t  qual_loc           \""; stream.write(p->qual_loc, 8);
   stream << "\"\n\t  cali_loc           \""; stream.write(p->cali_loc, 8);
   stream << "\"\n\t  gain_loc           \""; stream.write(p->gain_loc, 8);
   stream << "\"\n\t  bias_loc           \""; stream.write(p->bias_loc, 8);
   stream << "\"\n\t  type_id            \""; stream.write(p->type_id, 28);
   stream << "\"\n\t  type_code          \""; stream.write(p->type_code, 4);
   stream << "\"\n\t  left_fill          \""; stream.write(p->left_fill, 4);
   stream << "\"\n\t  right_fill         \""; stream.write(p->right_fill, 4);
   stream << "\"\n\t  pix_rng            \""; stream.write(p->pix_rng, 8);
   stream << "\"" << endl;
}

//******************************************************************************
// PRIVATE METHOD: ossimCeosData::dumpDataSetSumRec()
//  
//******************************************************************************
void ossimCeosData::dumpDataSetSumRec(ostream& stream) const
{
   const dataset_sum_rec* p = theDataSetSumRec;
   
   stream << "\n\n\tContents of dataset_sum_rec:\n\t      \"";
   stream << "\n\t  seq_num            \""; stream.write(p->seq_num, 4);
   stream << "\"\n\t  sar_chn            \""; stream.write(p->sar_chn, 4);
   stream << "\"\n\t  scene_id           \""; stream.write(p->scene_id, 16);
   stream << "\"\n\t  scene_des          \""; stream.write(p->scene_des, 32);
   stream << "\"\n\t  inp_sctim          \""; stream.write(p->inp_sctim, 32);
   stream << "\"\n\t  asc_des            \""; stream.write(p->asc_des, 16);
   stream << "\"\n\t  pro_lat            \""; stream.write(p->pro_lat, 16);
   stream << "\"\n\t  pro_long           \""; stream.write(p->pro_long, 16);
   stream << "\"\n\t  pro_head           \""; stream.write(p->pro_head, 16);
   stream << "\"\n\t  ellip_des          \""; stream.write(p->ellip_des, 16);
   stream << "\"\n\t  ellip_maj          \""; stream.write(p->ellip_maj, 16);
   stream << "\"\n\t  ellip_min          \""; stream.write(p->ellip_min, 16);
   stream << "\"\n\t  earth_mass         \""; stream.write(p->earth_mass, 16);
   stream << "\"\n\t  grav_const         \""; stream.write(p->grav_const, 16);
   stream << "\"\n\t  ellip_j[0]         \""; stream.write(p->ellip_j[0], 16);
   stream << "\"\n\t  ellip_j[1]         \""; stream.write(p->ellip_j[1], 16);
   stream << "\"\n\t  ellip_j[2]         \""; stream.write(p->ellip_j[2], 16);
   stream << "\"\n\t  spare1             \""; stream.write(p->spare1, 16);
   stream << "\"\n\t  terrain_h          \""; stream.write(p->terrain_h, 16);
   stream << "\"\n\t  sc_lin             \""; stream.write(p->sc_lin, 8);
   stream << "\"\n\t  sc_pix             \""; stream.write(p->sc_pix, 8);
   stream << "\"\n\t  scene_len          \""; stream.write(p->scene_len, 16);
   stream << "\"\n\t  scene_wid          \""; stream.write(p->scene_wid, 16);
   stream << "\"\n\t  spare2             \""; stream.write(p->spare2, 16);
   stream << "\"\n\t  nchn               \""; stream.write(p->nchn, 4);
   stream << "\"\n\t  spare3             \""; stream.write(p->spare3, 4);
   stream << "\"\n\t  mission_id         \""; stream.write(p->mission_id, 16);
   stream << "\"\n\t  sensor_id          \""; stream.write(p->sensor_id, 32);
   stream << "\"\n\t  orbit_num          \""; stream.write(p->orbit_num, 8);
   stream << "\"\n\t  plat_lat           \""; stream.write(p->plat_lat, 8);
   stream << "\"\n\t  plat_long          \""; stream.write(p->plat_long, 8);
   stream << "\"\n\t  plat_head          \""; stream.write(p->plat_head, 8);
   stream << "\"\n\t  clock_ang          \""; stream.write(p->clock_ang, 8);
   stream << "\"\n\t  incident_ang       \""; stream.write(p->incident_ang, 8);
   stream << "\"\n\t  spare4             \""; stream.write(p->spare4, 8);
   stream << "\"\n\t  wave_length        \""; stream.write(p->wave_length, 16);
   stream << "\"\n\t  motion_comp        \""; stream.write(p->motion_comp, 2);
   stream << "\"\n\t  pulse_code         \""; stream.write(p->pulse_code, 16);
   stream << "\"\n\t  ampl_coef[0]       \""; stream.write(p->ampl_coef[0], 16);
   stream << "\"\n\t  ampl_coef[1]       \""; stream.write(p->ampl_coef[1], 16);
   stream << "\"\n\t  ampl_coef[2]       \""; stream.write(p->ampl_coef[2], 16);
   stream << "\"\n\t  ampl_coef[3]       \""; stream.write(p->ampl_coef[3], 16);
   stream << "\"\n\t  ampl_coef[4]       \""; stream.write(p->ampl_coef[4], 16);
   stream << "\"\n\t  phas_coef[0]       \""; stream.write(p->phas_coef[0], 16);
   stream << "\"\n\t  phas_coef[1]       \""; stream.write(p->phas_coef[1], 16);
   stream << "\"\n\t  phas_coef[2]       \""; stream.write(p->phas_coef[2], 16);
   stream << "\"\n\t  phas_coef[3]       \""; stream.write(p->phas_coef[3], 16);
   stream << "\"\n\t  phas_coef[4]       \""; stream.write(p->phas_coef[4], 16);
   stream << "\"\n\t  chirp_ext_ind      \""; stream.write(p->chirp_ext_ind, 8);
   stream << "\"\n\t  spare5             \""; stream.write(p->spare5, 8);
   stream << "\"\n\t  fr                 \""; stream.write(p->fr, 16);
   stream << "\"\n\t  rng_gate           \""; stream.write(p->rng_gate, 16);
   stream << "\"\n\t  rng_length         \""; stream.write(p->rng_length, 16);
   stream << "\"\n\t  baseband_f         \""; stream.write(p->baseband_f, 4);
   stream << "\"\n\t  rngcmp_f           \""; stream.write(p->rngcmp_f, 4);
   stream << "\"\n\t  gn_polar           \""; stream.write(p->gn_polar, 16);
   stream << "\"\n\t  gn_cross           \""; stream.write(p->gn_cross, 16);
   stream << "\"\n\t  chn_bits           \""; stream.write(p->chn_bits, 8);
   stream << "\"\n\t  quant_desc         \""; stream.write(p->quant_desc, 12);
   stream << "\"\n\t  i_bias             \""; stream.write(p->i_bias, 16);
   stream << "\"\n\t  q_bias             \""; stream.write(p->q_bias, 16);
   stream << "\"\n\t  iq_ratio           \""; stream.write(p->iq_ratio, 16);
   stream << "\"\n\t  spare6             \""; stream.write(p->spare6, 16);
   stream << "\"\n\t  spare7             \""; stream.write(p->spare7, 16);
   stream << "\"\n\t  ele_sight          \""; stream.write(p->ele_sight, 16);
   stream << "\"\n\t  mech_sight         \""; stream.write(p->mech_sight, 16);
   stream << "\"\n\t  echo_track         \""; stream.write(p->echo_track, 4);
   stream << "\"\n\t  fa                 \""; stream.write(p->fa, 16);
   stream << "\"\n\t  elev_beam          \""; stream.write(p->elev_beam, 16);
   stream << "\"\n\t  azim_beam          \""; stream.write(p->azim_beam, 16);
   stream << "\"\n\t  sat_bintim         \""; stream.write(p->sat_bintim, 16);
   stream << "\"\n\t  sat_clktim         \""; stream.write(p->sat_clktim, 32);
   stream << "\"\n\t  sat_clkinc         \""; stream.write(p->sat_clkinc, 8);
   stream << "\"\n\t  spare8             \""; stream.write(p->spare8, 8);
   stream << "\"\n\t  fac_id             \""; stream.write(p->fac_id, 16);
   stream << "\"\n\t  sys_id             \""; stream.write(p->sys_id, 8);
   stream << "\"\n\t  ver_id             \""; stream.write(p->ver_id, 8);
   stream << "\"\n\t  fac_code           \""; stream.write(p->fac_code, 16);
   stream << "\"\n\t  lev_code           \""; stream.write(p->lev_code, 16);
   stream << "\"\n\t  prod_type          \""; stream.write(p->prod_type, 32);
   stream << "\"\n\t  algor_id           \""; stream.write(p->algor_id, 32);
   stream << "\"\n\t  n_azilok           \""; stream.write(p->n_azilok, 16);
   stream << "\"\n\t  n_rnglok           \""; stream.write(p->n_rnglok, 16);
   stream << "\"\n\t  bnd_azilok         \""; stream.write(p->bnd_azilok, 16);
   stream << "\"\n\t  bnd_rnglok         \""; stream.write(p->bnd_rnglok, 16);
   stream << "\"\n\t  bnd_azi            \""; stream.write(p->bnd_azi, 16);
   stream << "\"\n\t  bnd_rng            \""; stream.write(p->bnd_rng, 16);
   stream << "\"\n\t  azi_weight         \""; stream.write(p->azi_weight, 32);
   stream << "\"\n\t  rng_weight         \""; stream.write(p->rng_weight, 32);
   stream << "\"\n\t  data_inpsrc        \""; stream.write(p->data_inpsrc, 16);
   stream << "\"\n\t  rng_res            \""; stream.write(p->rng_res, 16);
   stream << "\"\n\t  azi_res            \""; stream.write(p->azi_res, 16);
   stream << "\"\n\t  radi_stretch[0]    \"";
   stream.write(p->radi_stretch[0],16);
   stream << "\"\n\t  radi_stretch[1]    \"";
   stream.write(p->radi_stretch[1],16);
   stream << "\"\n\t  alt_dopcen[0]      \""; stream.write(p->alt_dopcen[0],16);
   stream << "\"\n\t  alt_dopcen[1]      \""; stream.write(p->alt_dopcen[1],16);
   stream << "\"\n\t  alt_dopcen[2]      \""; stream.write(p->alt_dopcen[2],16);
   stream << "\"\n\t  spare9             \""; stream.write(p->spare9, 16);
   stream << "\"\n\t  crt_dopcen[0]      \""; stream.write(p->crt_dopcen[0],16);
   stream << "\"\n\t  crt_dopcen[1]      \""; stream.write(p->crt_dopcen[1],16);
   stream << "\"\n\t  crt_dopcen[2]      \""; stream.write(p->crt_dopcen[2],16);
   stream << "\"\n\t  time_dir_pix       \""; stream.write(p->time_dir_pix, 8);
   stream << "\"\n\t  time_dir_lin       \""; stream.write(p->time_dir_lin, 8);
   stream << "\"\n\t  alt_rate[0]        \""; stream.write(p->alt_rate[0], 16);
   stream << "\"\n\t  alt_rate[1]        \""; stream.write(p->alt_rate[1], 16);
   stream << "\"\n\t  alt_rate[2]        \""; stream.write(p->alt_rate[2], 16);
   stream << "\"\n\t  spare10            \""; stream.write(p->spare10, 16);
   stream << "\"\n\t  crt_rate[0]        \""; stream.write(p->crt_rate[0], 16);
   stream << "\"\n\t  crt_rate[1]        \""; stream.write(p->crt_rate[1], 16);
   stream << "\"\n\t  crt_rate[2]        \""; stream.write(p->crt_rate[2], 16);
   stream << "\"\n\t  spare11            \""; stream.write(p->spare11, 16);
   stream << "\"\n\t  line_cont          \""; stream.write(p->line_cont, 8);
   stream << "\"\n\t  clutter_lock       \""; stream.write(p->clutter_lock, 4);
   stream << "\"\n\t  auto_focus         \""; stream.write(p->auto_focus, 4);
   stream << "\"\n\t  line_spacing       \""; stream.write(p->line_spacing, 16);
   stream << "\"\n\t  pix_spacing        \""; stream.write(p->pix_spacing, 16);
   stream << "\"\n\t  rngcmp_desg        \""; stream.write(p->rngcmp_desg, 16);
   stream << "\"" << endl;
}

//******************************************************************************
// PRIVATE METHOD: ossimCeosData::dumpQualSumRec()
//  
//******************************************************************************
void ossimCeosData::dumpQualSumRec(ostream& stream) const
{
   const qual_sum_rec* p = theQualSumRec;
   
   stream << "\n\n\tContents of qual_sum_rec:\n\t";
   stream << "\n\t  rec_seq            \""; stream.write(p->rec_seq, 4);
   stream << "\"\n\t  sar_chn            \""; stream.write(p->sar_chn, 4);
   stream << "\"\n\t  cali_date          \""; stream.write(p->cali_date, 6);
   stream << "\"\n\t  nchn               \""; stream.write(p->nchn, 4);
   stream << "\"\n\t  islr               \""; stream.write(p->islr, 16);
   stream << "\"\n\t  pslr               \""; stream.write(p->pslr, 16);
   stream << "\"\n\t  azi_ambig          \""; stream.write(p->azi_ambig, 16);
   stream << "\"\n\t  rng_ambig          \""; stream.write(p->rng_ambig, 16);
   stream << "\"\n\t  snr                \""; stream.write(p->snr, 16);
   stream << "\"\n\t  ber                \""; stream.write(p->ber, 16);
   stream << "\"\n\t  rng_res            \""; stream.write(p->rng_res, 16);
   stream << "\"\n\t  azi_res            \""; stream.write(p->azi_res, 16);
   stream << "\"\n\t  rad_res            \""; stream.write(p->rad_res, 16);
   stream << "\"\n\t  dyn_rng            \""; stream.write(p->dyn_rng, 16);
   stream << "\"\n\t  rad_unc_db         \""; stream.write(p->rad_unc_db, 16);
   stream << "\"\n\t  rad_unc_deg        \""; stream.write(p->rad_unc_deg, 16);
   int i;
   for (i=0; i<16; i++)
   {
      stream << "\"\n\t  rad_unc[" << i << "]:";
      stream << "\n\t    db               \"";
      stream.write(p->rad_unc[i].db, 16);
      stream << "\"\n\t    deg              \"";
      stream.write(p->rad_unc[i].deg, 16);
   }
   stream << "\"\n\t  alt_locerr         \""; stream.write(p->alt_locerr, 16);
   stream << "\"\n\t  crt_locerr         \""; stream.write(p->crt_locerr, 16);
   stream << "\"\n\t  alt_scale          \""; stream.write(p->alt_scale, 16);
   stream << "\"\n\t  crt_scale          \""; stream.write(p->crt_scale, 16);
   stream << "\"\n\t  dis_skew           \""; stream.write(p->dis_skew, 16);
   stream << "\"\n\t  ori_err            \""; stream.write(p->ori_err, 16);
   for (i=0; i<16; i++)
   {
      stream << "\"\n\t  misreg[" << i << "]:";
      stream << "\n\t    alt_m            \"";
      stream.write(p->misreg[i].alt_m, 16);
      stream << "\"\n\t    crt_m            \"";
      stream.write(p->misreg[i].crt_m, 16);
   }
   stream << "\"\n\t  nesz               \""; stream.write(p->nesz, 16);
   stream << "\"\n\t  enl                \""; stream.write(p->enl, 16);
   stream << "\"\n\t  tb_update          \""; stream.write(p->tb_update, 8);
   stream << "\"" << endl;
}

//******************************************************************************
// PRIVATE METHOD: ossimCeosData::dumpSdrHistRec()
//  
//******************************************************************************
void ossimCeosData::dumpSdrHistRec(ostream& stream) const
{
   const sdr_hist_rec* p = theSdrHistRec;
   
   stream << "\n\n\tContents of sdr_hist_rec:\n\t";
   stream << "\n\t  rec_seq            \""; stream.write(p->rec_seq, 4);
   stream << "\"\n\t  sar_chn            \""; stream.write(p->sar_chn, 4);
   stream << "\"\n\t  ntab               \""; stream.write(p->ntab, 8);
   stream << "\"\n\t  ltab               \""; stream.write(p->ltab, 8);
   stream << "\"\n\t  htab[0] (only one sdr_htab_rec declared in data struct):";
   stream << "\n\t    hist_desc        \"";
   stream.write(p->htab.hist_desc, 32);
   stream << "\"\n\t    nrec             \""; stream.write(p->htab.nrec, 4);
   stream << "\"\n\t    tab_seq          \""; stream.write(p->htab.tab_seq, 4);
   stream << "\"\n\t    nbin             \""; stream.write(p->htab.nbin, 8);
   stream << "\"\n\t    ns_lin           \""; stream.write(p->htab.ns_lin, 8);
   stream << "\"\n\t    ns_pix           \""; stream.write(p->htab.ns_pix, 8);
   stream << "\"\n\t    ngrp_lin         \""; stream.write(p->htab.ngrp_lin, 8);
   stream << "\"\n\t    ngrp_pix         \""; stream.write(p->htab.ngrp_pix, 8);
   stream << "\"\n\t    nsamp_lin        \""; stream.write(p->htab.nsamp_lin,8);
   stream << "\"\n\t    nsamp_pix        \""; stream.write(p->htab.nsamp_pix,8);
   stream << "\"\n\t    min_smp          \""; stream.write(p->htab.min_smp, 16);
   stream << "\"\n\t    max_smp          \""; stream.write(p->htab.max_smp, 16);
   stream << "\"\n\t    mean_smp         \""; stream.write(p->htab.mean_smp,16);
   stream << "\"\n\t    std_smp          \""; stream.write(p->htab.std_smp, 16);
   stream << "\"\n\t    smp_inc          \""; stream.write(p->htab.smp_inc, 16);
   stream << "\"\n\t    min_hist         \""; stream.write(p->htab.min_hist,16);
   stream << "\"\n\t    max_hist         \""; stream.write(p->htab.max_hist,16);
   stream << "\"\n\t    mean_hist        \"";
   stream.write(p->htab.mean_hist, 16);
   stream << "\"\n\t    std_hist         \""; stream.write(p->htab.std_hist,16);
   stream << "\"\n\t    nhist            \""; stream.write(p->htab.nhist, 16);
   stream << "\"\n\t    [Use debugger to view histogram]";
   stream << endl;
}

//******************************************************************************
// PRIVATE METHOD: ossimCeosData::dumpPdr16HistRec()
//  
//******************************************************************************
void ossimCeosData::dumpPdr16HistRec(ostream& stream) const
{
   const  pdr16_hist_rec* p = thePdr16HistRec;
   
   stream << "\n\n\tContents of pdr16_hist_rec:\n\t";
   stream << "\n\t  rec_seq            \""; stream.write(p->rec_seq, 4);
   stream << "\"\n\t  sar_chn            \""; stream.write(p->sar_chn, 4);
   stream << "\"\n\t  ntab               \""; stream.write(p->ntab, 8);
   stream << "\"\n\t  ltab               \""; stream.write(p->ltab, 8);

   char buf[] = "12345678";
   strncpy(buf, p->ntab, 8);
   int n = atoi(buf);
   for (int i=0; i<n; i++)
   {
      stream << "\"\n\t  htab[" << i << "]:";
      stream << "\n\t    hist_desc        \"";
      stream.write(p->htab[i].hist_desc, 32);
      stream << "\"\n\t    nrec             \"";
      stream.write(p->htab[i].nrec, 4);
      stream << "\"\n\t    tab_seq          \"";
      stream.write(p->htab[i].tab_seq, 4);
      stream << "\"\n\t    nbin             \"";
      stream.write(p->htab[i].nbin, 8);
      stream << "\"\n\t    ns_lin           \"";
      stream.write(p->htab[i].ns_lin, 8);
      stream << "\"\n\t    ns_pix           \"";
      stream.write(p->htab[i].ns_pix, 8);
      stream << "\"\n\t    ngrp_lin         \"";
      stream.write(p->htab[i].ngrp_lin, 8);
      stream << "\"\n\t    ngrp_pix         \"";
      stream.write(p->htab[i].ngrp_pix, 8);
      stream << "\"\n\t    nsamp_lin        \"";
      stream.write(p->htab[i].nsamp_lin, 8);
      stream << "\"\n\t    nsamp_pix        \"";
      stream.write(p->htab[i].nsamp_pix, 8);
      stream << "\"\n\t    min_smp          \"";
      stream.write(p->htab[i].min_smp, 16);
      stream << "\"\n\t    max_smp          \"";
      stream.write(p->htab[i].max_smp, 16);
      stream << "\"\n\t    mean_smp         \"";
      stream.write(p->htab[i].mean_smp, 16);
      stream << "\"\n\t    std_smp          \"";
      stream.write(p->htab[i].std_smp, 16);
      stream << "\"\n\t    smp_inc          \"";
      stream.write(p->htab[i].smp_inc, 16);
      stream << "\"\n\t    min_hist         \"";
      stream.write(p->htab[i].min_hist, 16);
      stream << "\"\n\t    max_hist         \"";
      stream.write(p->htab[i].max_hist, 16);
      stream << "\"\n\t    mean_hist        \"";
      stream.write(p->htab[i].mean_hist, 16);
      stream << "\"\n\t    std_hist         \"";
      stream.write(p->htab[i].std_hist, 16);
      stream << "\"\n\t    nhist            \"";
      stream.write(p->htab[i].nhist, 16);
      stream << "\n\t    [Use debugger to view histogram]";
   }
   stream << "\"" << endl;
}

//******************************************************************************
// PRIVATE METHOD: ossimCeosData::dumpPdr8HistRec()
//  
//******************************************************************************
void ossimCeosData::dumpPdr8HistRec(ostream& stream) const
{
   const pdr8_hist_rec* p = thePdr8HistRec;
   
   stream << "\n\n\tContents of pdr18hist_rec:\n\t";
   stream << "\n\t  rec_seq            \""; stream.write(p->rec_seq, 4);
   stream << "\"\n\t  sar_chn            \""; stream.write(p->sar_chn, 4);
   stream << "\"\n\t  ntab               \""; stream.write(p->ntab, 8);
   stream << "\"\n\t  ltab               \""; stream.write(p->ltab, 8);
   stream << "\"\n\t  htab[0] (only one sdr_htab_rec declared in data struct):";
   stream << "\n\t    hist_desc        \"";
   stream.write(p->htab.hist_desc, 32);
   stream << "\"\n\t    nrec             \""; stream.write(p->htab.nrec, 4);
   stream << "\"\n\t    tab_seq          \""; stream.write(p->htab.tab_seq, 4);
   stream << "\"\n\t    nbin             \""; stream.write(p->htab.nbin, 8);
   stream << "\"\n\t    ns_lin           \""; stream.write(p->htab.ns_lin, 8);
   stream << "\"\n\t    ns_pix           \""; stream.write(p->htab.ns_pix, 8);
   stream << "\"\n\t    ngrp_lin         \""; stream.write(p->htab.ngrp_lin, 8);
   stream << "\"\n\t    ngrp_pix         \""; stream.write(p->htab.ngrp_pix, 8);
   stream << "\"\n\t    nsamp_lin        \""; stream.write(p->htab.nsamp_lin,8);
   stream << "\"\n\t    nsamp_pix        \""; stream.write(p->htab.nsamp_pix,8);
   stream << "\"\n\t    min_smp          \""; stream.write(p->htab.min_smp, 16);
   stream << "\"\n\t    max_smp          \""; stream.write(p->htab.max_smp, 16);
   stream << "\"\n\t    mean_smp         \""; stream.write(p->htab.mean_smp,16);
   stream << "\"\n\t    std_smp          \""; stream.write(p->htab.std_smp, 16);
   stream << "\"\n\t    smp_inc          \""; stream.write(p->htab.smp_inc, 16);
   stream << "\"\n\t    min_hist         \""; stream.write(p->htab.min_hist,16);
   stream << "\"\n\t    max_hist         \""; stream.write(p->htab.max_hist,16);
   stream << "\"\n\t    mean_hist        \"";
   stream.write(p->htab.mean_hist, 16);
   stream << "\"\n\t    std_hist         \""; stream.write(p->htab.std_hist,16);
   stream << "\"\n\t    nhist            \""; stream.write(p->htab.nhist, 16);
   stream << "\"\n\t    [Use debugger to view histogram]";
   stream << endl;
}

//******************************************************************************
// PRIVATE METHOD: ossimCeosData::dumpProcParmRec()
//  
//******************************************************************************
void ossimCeosData::dumpProcParmRec(ostream& stream) const
{
   const proc_parm_rec* p = theProcParmRec;

   stream << "\n\n\tContents of proc_parm_rec:\n\t";
   stream << "\n\t  slow_time_coef[1]  \"";
   stream.write(p->slow_time_coef[1], 22);
   stream << "\"\n\t  rec_seq            \""; stream.write(p->rec_seq, 4);
   stream << "\"\n\t  spare1             \""; stream.write(p->spare1, 4);
   stream << "\"\n\t  inp_media          \""; stream.write(p->inp_media, 3);
   stream << "\"\n\t  n_tape_id          \""; stream.write(p->n_tape_id, 4);
   stream << "\"\n\t  tape_id[0]         \""; stream.write(p->tape_id[0], 8);
   stream << "\"\n\t  exp_ing_start      \""; stream.write(p->exp_ing_start,21);
   stream << "\"\n\t  exp_ing_stop       \""; stream.write(p->exp_ing_stop, 21);
   stream << "\"\n\t  act_ing_start      \""; stream.write(p->act_ing_start,21);
   stream << "\"\n\t  act_ing_stop       \""; stream.write(p->act_ing_stop, 21);
   stream << "\"\n\t  proc_start         \""; stream.write(p->proc_start, 21);
   stream << "\"\n\t  proc_stop          \""; stream.write(p->proc_stop, 21);
   stream << "\"\n\t  mn_sig_lev[0]      \""; stream.write(p->mn_sig_lev[0],16);
   stream << "\"\n\t  mn_sig_lev[1]      \""; stream.write(p->mn_sig_lev[1],16);
   stream << "\"\n\t  mn_sig_lev[2]      \""; stream.write(p->mn_sig_lev[2],16);
   stream << "\"\n\t  mn_sig_lev[3]      \""; stream.write(p->mn_sig_lev[3],16);
   stream << "\"\n\t  mn_sig_lev[4]      \""; stream.write(p->mn_sig_lev[4],16);
   stream << "\"\n\t  mn_sig_lev[5]      \""; stream.write(p->mn_sig_lev[5],16);
   stream << "\"\n\t  mn_sig_lev[6]      \""; stream.write(p->mn_sig_lev[6],16);
   stream << "\"\n\t  mn_sig_lev[7]      \""; stream.write(p->mn_sig_lev[7],16);
   stream << "\"\n\t  mn_sig_lev[8]      \""; stream.write(p->mn_sig_lev[8],16);
   stream << "\"\n\t  mn_sig_lev[9]      \""; stream.write(p->mn_sig_lev[9],16);
   stream << "\"\n\t  scr_data_ind       \""; stream.write(p->scr_data_ind, 4);
   stream << "\"\n\t  miss_ln            \""; stream.write(p->miss_ln, 8);
   stream << "\"\n\t  rej_ln             \""; stream.write(p->rej_ln, 8);
   stream << "\"\n\t  large_gap          \""; stream.write(p->large_gap, 8);
   stream << "\"\n\t  bit_err_rate       \""; stream.write(p->bit_err_rate, 16);
   stream << "\"\n\t  fm_crc_err         \""; stream.write(p->fm_crc_err, 16);
   stream << "\"\n\t  date_incons        \""; stream.write(p->date_incons, 8);
   stream << "\"\n\t  prf_changes        \""; stream.write(p->prf_changes, 8);
   stream << "\"\n\t  delay_changes      \""; stream.write(p->delay_changes, 8);
   stream << "\"\n\t  skipd_frams        \""; stream.write(p->skipd_frams, 8);
   stream << "\"\n\t  rej_bf_start       \""; stream.write(p->rej_bf_start, 8);
   stream << "\"\n\t  rej_few_fram       \""; stream.write(p->rej_few_fram, 8);
   stream << "\"\n\t  rej_many_fram      \""; stream.write(p->rej_many_fram, 8);
   stream << "\"\n\t  rej_mchn_err       \""; stream.write(p->rej_mchn_err, 8);
   stream << "\"\n\t  rej_vchn_err       \""; stream.write(p->rej_vchn_err, 8);
   stream << "\"\n\t  rej_rec_type       \""; stream.write(p->rej_rec_type, 8);
   stream << "\"\n\t  sens_config        \""; stream.write(p->sens_config, 10);
   stream << "\"\n\t  sens_orient        \""; stream.write(p->sens_orient, 9);
   stream << "\"\n\t  sych_marker        \""; stream.write(p->sych_marker, 8);
   stream << "\"\n\t  rng_ref_src        \""; stream.write(p->rng_ref_src, 12);
   stream << "\"\n\t  rng_amp_coef[0]    \"";
   stream.write(p->rng_amp_coef[0],16);
   stream << "\"\n\t  rng_amp_coef[1]    \"";
   stream.write(p->rng_amp_coef[1],16);
   stream << "\"\n\t  rng_amp_coef[2]    \"";
   stream.write(p->rng_amp_coef[2],16);
   stream << "\"\n\t  rng_amp_coef[3]    \"";
   stream.write(p->rng_amp_coef[3],16);
   stream << "\"\n\t  rng_phas_coef[0]   \"";
   stream.write(p->rng_phas_coef[0], 16);
   stream << "\"\n\t  rng_phas_coef[1]   \"";
   stream.write(p->rng_phas_coef[1], 16);
   stream << "\"\n\t  rng_phas_coef[2]   \"";
   stream.write(p->rng_phas_coef[2], 16);
   stream << "\"\n\t  rng_phas_coef[3]   \"";
   stream.write(p->rng_phas_coef[3], 16);
   stream << "\"\n\t  err_amp_coef[0]    \"";
   stream.write(p->err_amp_coef[0],16);
   stream << "\"\n\t  err_amp_coef[1]    \"";
   stream.write(p->err_amp_coef[1],16);
   stream << "\"\n\t  err_amp_coef[2]    \"";
   stream.write(p->err_amp_coef[2],16);
   stream << "\"\n\t  err_amp_coef[3]    \"";
   stream.write(p->err_amp_coef[3],16);
   stream << "\"\n\t  err_phas_coef[0]   \"";
   stream.write(p->err_phas_coef[0], 16);
   stream << "\"\n\t  err_phas_coef[1]   \"";
   stream.write(p->err_phas_coef[1], 16);
   stream << "\"\n\t  err_phas_coef[2]   \"";
   stream.write(p->err_phas_coef[2], 16);
   stream << "\"\n\t  err_phas_coef[3]   \"";
   stream.write(p->err_phas_coef[3], 16);
   stream << "\"\n\t  pulse_bandw        \""; stream.write(p->pulse_bandw, 4);
   stream << "\"\n\t  adc_samp_rate      \""; stream.write(p->adc_samp_rate, 5);
   stream << "\"\n\t  rep_agc_attn       \""; stream.write(p->rep_agc_attn, 16);
   stream << "\"\n\t  gn_corctn_fctr     \"";
   stream.write(p->gn_corctn_fctr, 16);
   stream << "\"\n\t  rep_energy_gn      \""; stream.write(p->rep_energy_gn,16);
   stream << "\"\n\t  orb_data_src       \""; stream.write(p->orb_data_src, 11);
   stream << "\"\n\t  pulse_cnt_1        \""; stream.write(p->pulse_cnt_1, 4);
   stream << "\"\n\t  pulse_cnt_2        \""; stream.write(p->pulse_cnt_2, 4);
   stream << "\"\n\t  beam_edge_rqd      \""; stream.write(p->beam_edge_rqd, 3);
   stream << "\"\n\t  beam_edge_conf     \"";
   stream.write(p->beam_edge_conf, 16);
   stream << "\"\n\t  pix_overlap        \""; stream.write(p->pix_overlap, 4);
   stream << "\"\n\t  n_beams            \""; stream.write(p->n_beams, 4);
   stream << "\"\n\t  [Use debugger to view contents of beam_info record] ";
   stream << "\n\t  n_pix_updates      \""; stream.write(p->n_pix_updates, 4);
   stream << "\"\n\t  [Use debugger to view contents of pix_count record] ";
   stream << "\n\t  pwin_start         \""; stream.write(p->pwin_start, 16);
   stream << "\"\n\t  pwin_end           \""; stream.write(p->pwin_end, 16);
   stream << "\"\n\t  recd_type          \""; stream.write(p->recd_type, 9);
   stream << "\"\n\t  temp_set_inc       \""; stream.write(p->temp_set_inc, 16);
   stream << "\"\n\t  n_temp_set         \""; stream.write(p->n_temp_set, 4);
   stream << "\"\n\t  [Use debugger to view contents of temp record] ";
   stream << "\n\t  n_image_pix        \""; stream.write(p->n_image_pix, 8);
   stream << "\"\n\t  prc_zero_pix       \""; stream.write(p->prc_zero_pix, 16);
   stream << "\"\n\t  prc_satur_pix      \""; stream.write(p->prc_satur_pix,16);
   stream << "\"\n\t  img_hist_mean      \""; stream.write(p->img_hist_mean,16);
   stream << "\"\n\t  img_cumu_dist[0]   \"";
   stream.write(p->img_cumu_dist[0], 16);
   stream << "\"\n\t  img_cumu_dist[1]   \"";
   stream.write(p->img_cumu_dist[1], 16);
   stream << "\"\n\t  img_cumu_dist[2]   \"";
   stream.write(p->img_cumu_dist[2], 16);
   stream << "\"\n\t  pre_img_gn         \""; stream.write(p->pre_img_gn, 16);
   stream << "\"\n\t  post_img_gn        \""; stream.write(p->post_img_gn, 16);
   stream << "\"\n\t  dopcen_inc         \""; stream.write(p->dopcen_inc, 16);
   stream << "\"\n\t  n_dopcen           \""; stream.write(p->n_dopcen, 4);
   stream << "\"\n\t  [Use debugger to view contents of dopcen_est record] ";
   stream << "\n\t  dop_amb_err        \""; stream.write(p->dop_amb_err, 4);
   stream << "\"\n\t  dopamb_conf        \""; stream.write(p->dopamb_conf, 16);
   stream << "\"\n\t  eph_orb_data[0]    \"";
   stream.write(p->eph_orb_data[0],16);
   stream << "\"\n\t  eph_orb_data[1]    \"";
   stream.write(p->eph_orb_data[1],16);
   stream << "\"\n\t  eph_orb_data[2]    \"";
   stream.write(p->eph_orb_data[2],16);
   stream << "\"\n\t  eph_orb_data[3]    \"";
   stream.write(p->eph_orb_data[3],16);
   stream << "\"\n\t  eph_orb_data[4]    \"";
   stream.write(p->eph_orb_data[4],16);
   stream << "\"\n\t  eph_orb_data[5]    \"";
   stream.write(p->eph_orb_data[5],16);
   stream << "\"\n\t  eph_orb_data[6]    \"";
   stream.write(p->eph_orb_data[6],16);
   stream << "\"\n\t  appl_type          \""; stream.write(p->appl_type, 12);
   stream << "\"\n\t  slow_time_coef[1]  \"";
   stream.write(p->slow_time_coef[1], 22);
   stream << "\"\n\t  slow_time_coef[2]  \"";
   stream.write(p->slow_time_coef[2], 22);
   stream << "\"\n\t  slow_time_coef[3]  \"";
   stream.write(p->slow_time_coef[3], 22);
   stream << "\"\n\t  slow_time_coef[4]  \"";
   stream.write(p->slow_time_coef[4], 22);
   stream << "\"\n\t  n_srgr             \""; stream.write(p->n_srgr, 4);
   stream << "\"\n\t  [Use debugger to view contents of srgr_coefset record]";
   stream << "\n\t  pixel_spacing      \""; stream.write(p->pixel_spacing,16);
   stream << "\"\n\t  gics_reqd          \""; stream.write(p->gics_reqd, 3);
   stream << "\"\n\t  wo_number          \""; stream.write(p->wo_number, 8);
   stream << "\"\n\t  wo_date            \""; stream.write(p->wo_date, 20);
   stream << "\"\n\t  satellite_id       \""; stream.write(p->satellite_id, 10);
   stream << "\"\n\t  user_id            \""; stream.write(p->user_id, 20);
   stream << "\"\n\t  complete_msg       \""; stream.write(p->complete_msg, 3);
   stream << "\"\n\t  scene_id           \""; stream.write(p->scene_id, 5);
   stream << "\"\n\t  density_in         \""; stream.write(p->density_in, 4);
   stream << "\"\n\t  media_id           \""; stream.write(p->media_id, 8);
   stream << "\"\n\t  angle_first        \""; stream.write(p->angle_first, 16);
   stream << "\"\n\t  angle_last         \""; stream.write(p->angle_last, 16);
   stream << "\"\n\t  prod_type          \""; stream.write(p->prod_type, 3);
   stream << "\"\n\t  map_system         \""; stream.write(p->map_system, 16);
   stream << "\"\n\t  centre_lat         \""; stream.write(p->centre_lat, 22);
   stream << "\"\n\t  centre_long        \""; stream.write(p->centre_long, 22);
   stream << "\"\n\t  span_x             \""; stream.write(p->span_x, 22);
   stream << "\"\n\t  span_y             \""; stream.write(p->span_y, 22);
   stream << "\"\n\t  apply_dtm          \""; stream.write(p->apply_dtm, 3);
   stream << "\"\n\t  density_out        \""; stream.write(p->density_out, 4);
   stream << "\"\n\t  state_time         \""; stream.write(p->state_time, 21);
   stream << "\"\n\t  num_state_vectors  \"";
   stream.write(p->num_state_vectors, 4);
   stream << "\"\n\t  state_time_inc     \"";
   stream.write(p->state_time_inc, 16);
   stream << "\"" << endl;
}

//******************************************************************************
// PRIVATE METHOD: ossimCeosData::dumpMapProjRec()
//  
//******************************************************************************
void ossimCeosData::dumpMapProjRec(ostream& stream) const
{
   const map_proj_rec* p = theMapProjRec;

   stream << "\n\n\tContents of map_proj_rec:\n\t";
   stream << "\n\t  spare1             \""; stream.write(p->spare1, 16);
   stream << "\"\n\t  map_desc           \""; stream.write(p->map_desc, 32);
   stream << "\"\n\t  n_pixel            \""; stream.write(p->n_pixel, 16);
   stream << "\"\n\t  n_line             \""; stream.write(p->n_line, 16);
   stream << "\"\n\t  pixel_spacing      \""; stream.write(p->pixel_spacing,16);
   stream << "\"\n\t  line_spacing       \""; stream.write(p->line_spacing, 16);
   stream << "\"\n\t  osc_orient         \""; stream.write(p->osc_orient, 16);
   stream << "\"\n\t  orb_incl           \""; stream.write(p->orb_incl, 16);
   stream << "\"\n\t  asc_node           \""; stream.write(p->asc_node, 16);
   stream << "\"\n\t  isc_dist           \""; stream.write(p->isc_dist, 16);
   stream << "\"\n\t  geo_alt            \""; stream.write(p->geo_alt, 16);
   stream << "\"\n\t  isc_vel            \""; stream.write(p->isc_vel, 16);
   stream << "\"\n\t  plat_head          \""; stream.write(p->plat_head, 16);
   stream << "\"\n\t  ref_ellip          \""; stream.write(p->ref_ellip, 32);
   stream << "\"\n\t  semi_major         \""; stream.write(p->semi_major, 16);
   stream << "\"\n\t  semi_minor         \""; stream.write(p->semi_minor, 16);
   stream << "\"\n\t  datum_shift[0]     \"";
   stream.write(p->datum_shift[0], 16);
   stream << "\"\n\t  datum_shift[1]     \"";
   stream.write(p->datum_shift[1], 16);
   stream << "\"\n\t  datum_shift[2]     \"";
   stream.write(p->datum_shift[2], 16);
   stream << "\"\n\t  aux_datum_shift[0] \"";
   stream.write(p->aux_datum_shift[0], 16);
   stream << "\"\n\t  aux_datum_shift[1] \"";
   stream.write(p->aux_datum_shift[1], 16);
   stream << "\"\n\t  aux_datum_shift[2] \"";
   stream.write(p->aux_datum_shift[2], 16);
   stream << "\"\n\t  scal_ellip         \""; stream.write(p->scal_ellip, 16);
   stream << "\"\n\t  proj_desc          \""; stream.write(p->proj_desc, 32);
   stream << "\"\n\t  utm_desc           \""; stream.write(p->utm_desc, 32);
   stream << "\"\n\t  utm_zone_sig       \""; stream.write(p->utm_zone_sig, 4);
   stream << "\"\n\t  utm_east_orig      \""; stream.write(p->utm_east_orig,16);
   stream << "\"\n\t  utm_north_orig     \"";
   stream.write(p->utm_north_orig, 16);
   stream << "\"\n\t  utm_cent_long      \""; stream.write(p->utm_cent_long,16);
   stream << "\"\n\t  utm_cent_lat       \""; stream.write(p->utm_cent_lat, 16);
   stream << "\"\n\t  utm_stand_par[0]   \"";
   stream.write(p->utm_stand_par[0], 16);
   stream << "\"\n\t  utm_stand_par[1]   \"";
   stream.write(p->utm_stand_par[1], 16);
   stream << "\"\n\t  utm_scale          \""; stream.write(p->utm_scale, 16);
   stream << "\"\n\t  ups_desc           \""; stream.write(p->ups_desc, 32);
   stream << "\"\n\t  ups_cent_long      \""; stream.write(p->ups_cent_long,16);
   stream << "\"\n\t  ups_cent_lat       \""; stream.write(p->ups_cent_lat, 16);
   stream << "\"\n\t  ups_scale          \""; stream.write(p->ups_scale, 16);
   stream << "\"\n\t  nsp_desc           \""; stream.write(p->nsp_desc, 32);
   stream << "\"\n\t  nsp_east_orig      \""; stream.write(p->nsp_east_orig,16);
   stream << "\"\n\t  nsp_north_orig     \"";
   stream.write(p->nsp_north_orig,16);
   stream << "\"\n\t  nsp_cent_long      \""; stream.write(p->nsp_cent_long,16);
   stream << "\"\n\t  nsp_cent_lat       \""; stream.write(p->nsp_cent_lat, 16);
   stream << "\"\n\t  nsp_stand_par[0]   \"";
   stream.write(p->nsp_stand_par[0], 16);
   stream << "\"\n\t  nsp_stand_par[1]   \"";
   stream.write(p->nsp_stand_par[1], 16);
   stream << "\"\n\t  nsp_stand_par[2]   \"";
   stream.write(p->nsp_stand_par[2], 16);
   stream << "\"\n\t  nsp_stand_par[3]   \"";
   stream.write(p->nsp_stand_par[3], 16);
   stream << "\"\n\t  nsp_stand_mer[0]   \"";
   stream.write(p->nsp_stand_mer[0], 16);
   stream << "\"\n\t  nsp_stand_mer[1]   \"";
   stream.write(p->nsp_stand_mer[1], 16);
   stream << "\"\n\t  nsp_stand_mer[2]   \"";
   stream.write(p->nsp_stand_mer[2], 16);
   stream << "\"\n\t  nsp_spare[0]       \""; stream.write(p->nsp_spare[0], 16);
   stream << "\"\n\t  nsp_spare[1]       \""; stream.write(p->nsp_spare[1], 16);
   stream << "\"\n\t  nsp_spare[2]       \""; stream.write(p->nsp_spare[2], 16);
   stream << "\"\n\t  nsp_spare[3]       \""; stream.write(p->nsp_spare[3], 16);
   stream << "\"\n\t  corner_ne[0]       \""; stream.write(p->corner_ne[0], 16);
   stream << "\"\n\t  corner_ne[1]       \""; stream.write(p->corner_ne[1], 16);
   stream << "\"\n\t  corner_ne[2]       \""; stream.write(p->corner_ne[2], 16);
   stream << "\"\n\t  corner_ne[3]       \""; stream.write(p->corner_ne[3], 16);
   stream << "\"\n\t  corner_ne[4]       \""; stream.write(p->corner_ne[4], 16);
   stream << "\"\n\t  corner_ne[5]       \""; stream.write(p->corner_ne[5], 16);
   stream << "\"\n\t  corner_ne[6]       \""; stream.write(p->corner_ne[6], 16);
   stream << "\"\n\t  corner_ne[7]       \""; stream.write(p->corner_ne[7], 16);
   stream << "\"\n\t  corner_ll[0]       \""; stream.write(p->corner_ll[0], 16);
   stream << "\"\n\t  corner_ll[1]       \""; stream.write(p->corner_ll[1], 16);
   stream << "\"\n\t  corner_ll[2]       \""; stream.write(p->corner_ll[2], 16);
   stream << "\"\n\t  corner_ll[3]       \""; stream.write(p->corner_ll[3], 16);
   stream << "\"\n\t  corner_ll[4]       \""; stream.write(p->corner_ll[4], 16);
   stream << "\"\n\t  corner_ll[5]       \""; stream.write(p->corner_ll[5], 16);
   stream << "\"\n\t  corner_ll[6]       \""; stream.write(p->corner_ll[6], 16);
   stream << "\"\n\t  corner_ll[7]       \""; stream.write(p->corner_ll[7], 16);
   stream << "\"\n\t  terr_height[0]     \"";
   stream.write(p->terr_height[0], 16);
   stream << "\"\n\t  terr_height[1]     \"";
   stream.write(p->terr_height[1], 16);
   stream << "\"\n\t  terr_height[2]     \"";
   stream.write(p->terr_height[2], 16);
   stream << "\"\n\t  terr_height[3]     \"";
   stream.write(p->terr_height[3], 16);
   stream << "\"\n\t  lp_conv_coef[0]    \"";
   stream.write(p->lp_conv_coef[0],20);
   stream << "\"\n\t  lp_conv_coef[1]    \"";
   stream.write(p->lp_conv_coef[1],20);
   stream << "\"\n\t  lp_conv_coef[2]    \"";
   stream.write(p->lp_conv_coef[2],20);
   stream << "\"\n\t  lp_conv_coef[3]    \"";
   stream.write(p->lp_conv_coef[3],20);
   stream << "\"\n\t  lp_conv_coef[4]    \"";
   stream.write(p->lp_conv_coef[4],20);
   stream << "\"\n\t  lp_conv_coef[5]    \"";
   stream.write(p->lp_conv_coef[5],20);
   stream << "\"\n\t  lp_conv_coef[6]    \"";
   stream.write(p->lp_conv_coef[6],20);
   stream << "\"\n\t  lp_conv_coef[7]    \"";
   stream.write(p->lp_conv_coef[7],20);
   stream << "\"\n\t  mp_conv_coef[0]    \"";
   stream.write(p->mp_conv_coef[0],20);
   stream << "\"\n\t  mp_conv_coef[1]    \"";
   stream.write(p->mp_conv_coef[1],20);
   stream << "\"\n\t  mp_conv_coef[2]    \"";
   stream.write(p->mp_conv_coef[2],20);
   stream << "\"\n\t  mp_conv_coef[3]    \"";
   stream.write(p->mp_conv_coef[3],20);
   stream << "\"\n\t  mp_conv_coef[4]    \"";
   stream.write(p->mp_conv_coef[4],20);
   stream << "\"\n\t  mp_conv_coef[5]    \"";
   stream.write(p->mp_conv_coef[5],20);
   stream << "\"\n\t  mp_conv_coef[6]    \"";
   stream.write(p->mp_conv_coef[6],20);
   stream << "\"\n\t  mp_conv_coef[7]    \"";
   stream.write(p->mp_conv_coef[7],20);
   stream << "\"\n\t  dem_type           \""; stream.write(p->dem_type, 4);
   stream << "\"\n\t  spare3             \""; stream.write(p->spare3, 32);
   stream << "\"" << endl;
}

//******************************************************************************
// PRIVATE METHOD: ossimCeosData::dumpPosDataRec()
//  
//******************************************************************************
void ossimCeosData::dumpPosDataRec(ostream& stream) const
{
   const pos_data_rec* p = thePosDataRec;
   
   stream << "\n\n\tContents of pos_data_rec:\n\t";
   stream << "\n\t  orbit_ele_desg     \""; stream.write(p->orbit_ele_desg, 32);
   stream << "\"\n\t  orbit_ele[0]       \""; stream.write(p->orbit_ele[0], 16);
   stream << "\"\n\t  orbit_ele[1]       \""; stream.write(p->orbit_ele[1], 16);
   stream << "\"\n\t  orbit_ele[2]       \""; stream.write(p->orbit_ele[2], 16);
   stream << "\"\n\t  orbit_ele[3]       \""; stream.write(p->orbit_ele[3], 16);
   stream << "\"\n\t  orbit_ele[4]       \""; stream.write(p->orbit_ele[4], 16);
   stream << "\"\n\t  orbit_ele[5]       \""; stream.write(p->orbit_ele[5], 16);
   stream << "\"\n\t  ndata              \""; stream.write(p->ndata, 4);
   stream << "\"\n\t  year               \""; stream.write(p->year, 4);
   stream << "\"\n\t  month              \""; stream.write(p->month, 4);
   stream << "\"\n\t  day                \""; stream.write(p->day, 4);
   stream << "\"\n\t  gmt_day            \""; stream.write(p->gmt_day, 4);
   stream << "\"\n\t  gmt_sec            \""; stream.write(p->gmt_sec, 22);
   stream << "\"\n\t  data_int           \""; stream.write(p->data_int, 22);
   stream << "\"\n\t  ref_coord          \""; stream.write(p->ref_coord, 64);
   stream << "\"\n\t  hr_angle           \""; stream.write(p->hr_angle, 22);
   stream << "\"\n\t  alt_poserr         \""; stream.write(p->alt_poserr, 16);
   stream << "\"\n\t  crt_poserr         \""; stream.write(p->crt_poserr, 16);
   stream << "\"\n\t  rad_poserr         \""; stream.write(p->rad_poserr, 16);
   stream << "\"\n\t  alt_velerr         \""; stream.write(p->alt_velerr, 16);
   stream << "\"\n\t  crt_velerr         \""; stream.write(p->crt_velerr, 16);
   stream << "\"\n\t  rad_velerr         \""; stream.write(p->rad_velerr, 16);
   stream << "\"\n\t  [Use debugger to view contents of pos_vect record]";
   stream << endl;
}

//******************************************************************************
// PRIVATE METHOD: ossimCeosData::dumpAttDataRec()
//  
//******************************************************************************
void ossimCeosData::dumpAttDataRec(ostream& stream) const
{
   const att_data_rec* p = theAttDataRec;
   
   stream << "\n\n\tContents of att_data_rec:\n\t";
   stream << "\n\t  npoint             \""; stream.write(p->npoint, 4);
   stream << "\"\n\t  [Use debugger to view contents of att_vect record]";
   stream << "\n\t  pitch_bias         \""; stream.write(p->pitch_bias, 14);
   stream << "\"\n\t  roll_bias          \""; stream.write(p->roll_bias, 14);
   stream << "\"\n\t  yaw_bias           \""; stream.write(p->yaw_bias, 14);
   stream << "\"" << endl;
}

//******************************************************************************
// PRIVATE METHOD: ossimCeosData::dumpRadiDataRec()
//  
//******************************************************************************
void ossimCeosData::dumpRadiDataRec(ostream& stream) const
{
   const radi_data_rec* p = theRadiDataRec;
   
   stream << "\n\n\tContents of radi_data_rec:\n\t";
   stream << "\n\t  seq_num            \""; stream.write(p->seq_num, 4);
   stream << "\"\n\t  n_data             \""; stream.write(p->n_data, 4);
   stream << "\"\n\t  field_size         \""; stream.write(p->field_size, 8);
   stream << "\"\n\t  [Use debugger to view contents of dset record]";
   stream << endl;
}

//******************************************************************************
// PRIVATE METHOD: ossimCeosData::dumpRadiCompRec()
//  
//******************************************************************************
void ossimCeosData::dumpRadiCompRec(ostream& stream) const
{
   const radi_comp_rec* p = theRadiCompRec;
   
   stream << "\n\n\tContents of radi_comp_rec:\n\t";
   stream << "\n\t  seq_num            \""; stream.write(p->seq_num, 4);
   stream << "\"\n\t  chan_ind           \""; stream.write(p->chan_ind, 4);
   stream << "\"\n\t  n_dset             \""; stream.write(p->n_dset, 8);
   stream << "\"\n\t  dset_size          \""; stream.write(p->dset_size, 8);
   stream << "\"\n\t  [Use debugger to view contents of dset record]";
   stream << endl;
}


//*****************************************************************************
// PRIVATE METHOD: ossimCeosData::loadCeosRecords()
//
//  Reads the data files and initializes the data members. 
//
//*****************************************************************************
void ossimCeosData::loadCeosRecords()
{
   static const char MODULE[] = "ossimCeosData::loadCeosFile()";
   if (traceExec()) CLOG << " Entered method." << endl;

   int       sizeOfRec;
   char      bufI6[] = "123456";
   int       numRecords;
   
   //***
   // Open Volume Directory File and parse:
   //***
   FILE* fptr = fopen(theVolDirFileName.chars(), "r");
   if (!fptr)
   {
      if(traceDebug())
      {
         CLOG << " ERROR:\n\tCannot open CEOS Volume Directory file: "
              << theVolDirFileName << endl;
      }
      theErrorStatus = ERROR;
      return;
   }
   sizeOfRec = sizeof(theVolumeDirectory);
   fread(&theVolumeDirectory, sizeOfRec, 1, fptr);
   fclose(fptr);
   theVolDescRec = &theVolumeDirectory.volDescRec;
   theTextRec    = &theVolumeDirectory.textRec;

   if (traceDebug())
   {
      CLOG << "DEBUG\n   -- List of Records Present ---" << endl;
   }

   //***
   // Open and parse Leader File:
   //***
   if (!theLeaderFileName.empty())
   {
      fptr = fopen(theLeaderFileName.chars(), "r");
      if (!fptr)
      {
         if(traceDebug())
         {
            CLOG << " ERROR:\n\tCannot open CEOS Leader file: "
                  << theLeaderFileName << endl;
         }
         theErrorStatus = ERROR;
         return;
      }
      fread(&(theSarLeaderFile.sarDescRec), 720, 1, fptr);

      //***
      // Establish which CEOS records are in the leader file, and read them.
      // First the dataset summary record:
      //***
      strncpy(bufI6, theSarLeaderFile.sarDescRec.n_dataset, 6);
      numRecords = atoi(bufI6);
      if (numRecords)
      {
         fread(&theSarLeaderFile.dataSetSumRec, sizeOfNextRec(fptr), 1, fptr);
         theDataSetSumRec = &(theSarLeaderFile.dataSetSumRec);
         if (traceDebug()) clog << "\t Leader: dataSetSumRec" << endl;
      }

      //***
      // Data quality summary record:
      //***
      strncpy(bufI6, theSarLeaderFile.sarDescRec.n_qual_sum, 6);
      numRecords = atoi(bufI6);
      if (numRecords)
      {
         fread(&(theSarLeaderFile.qualSumRec), sizeOfNextRec(fptr), 1, fptr);
         theQualSumRec = &(theSarLeaderFile.qualSumRec);
         if (traceDebug()) clog << "\t Leader: qualSumRec" << endl;
      }

      //***
      // Histogram records:
      //***
      strncpy(bufI6, theSarLeaderFile.sarDescRec.n_data_hist, 6);
      numRecords = atoi(bufI6);
      if (numRecords)
      {
         fread(&(theSarLeaderFile.sdrHistRec), sizeOfNextRec(fptr), 1, fptr);
         theSdrHistRec = &(theSarLeaderFile.sdrHistRec);
         fread(&(theSarLeaderFile.pdr16HistRec), sizeOfNextRec(fptr), 1, fptr);
         thePdr16HistRec = &(theSarLeaderFile.pdr16HistRec);
         if (traceDebug()) clog << "\t Leader: sdrHistRec & pdr16HistRec"
                                << endl;
      }

      //***
      // Processing parameter record:
      //***
      strncpy(bufI6, theSarLeaderFile.sarDescRec.n_det_proc, 6);
      numRecords = atoi(bufI6);
      if (numRecords)
      {
         fread(&(theSarLeaderFile.procParmRec), sizeOfNextRec(fptr), 1, fptr);
         theProcParmRec = &(theSarLeaderFile.procParmRec);
         if (traceDebug()) clog << "\t Leader: procParmRec" << endl;
      }

      //***
      // Map projection record:
      //***
      strncpy(bufI6, theSarLeaderFile.sarDescRec.n_map_proj, 6);
      numRecords = atoi(bufI6);
      if (numRecords)
      {
         fread(&(theSarLeaderFile.mapProjRec), sizeOfNextRec(fptr), 1, fptr);
         theMapProjRec = &(theSarLeaderFile.mapProjRec);
         if (traceDebug()) clog << "\t Leader: mapProjRec" << endl;
      }

      //***
      // Position data record:
      //***
      strncpy(bufI6, theSarLeaderFile.sarDescRec.n_plat_pos, 6);
      numRecords = atoi(bufI6);
      if (numRecords)
      {
         fread(&(theSarLeaderFile.posDataRec), sizeOfNextRec(fptr), 1, fptr);
         thePosDataRec = &(theSarLeaderFile.posDataRec);
         if (traceDebug()) clog << "\t Leader: posDataRec" << endl;
      }

      //***
      // Attitude data record:
      //***
      strncpy(bufI6, theSarLeaderFile.sarDescRec.n_att_data, 6);
      numRecords = atoi(bufI6);
      if (numRecords)
      {
         fread(&(theSarLeaderFile.attDataRec), sizeOfNextRec(fptr), 1, fptr);
         theAttDataRec = &(theSarLeaderFile.attDataRec);
         if (traceDebug()) clog << "\t Leader: attDataRec" << endl;
      }

      //***
      // Radiometric data record:
      //***
      strncpy(bufI6, theSarLeaderFile.sarDescRec.n_radi_data, 6);
      numRecords = atoi(bufI6);
      if (numRecords)
      {
         fread(&(theSarLeaderFile.radiDataRec), sizeOfNextRec(fptr), 1, fptr);
         theRadiDataRec = &(theSarLeaderFile.radiDataRec);
         if (traceDebug()) clog << "\t Leader: radiDataRec" << endl;
      }

      //***
      // Radiometric compensation data record:
      //***
      strncpy(bufI6, theSarLeaderFile.sarDescRec.n_radi_comp, 6);
      numRecords = atoi(bufI6);
      if (numRecords)
      {
         fread(&(theSarLeaderFile.radiCompRec), sizeOfNextRec(fptr), 1, fptr);
         theRadiCompRec = &(theSarLeaderFile.radiCompRec);
         if (traceDebug()) clog << "\t Leader: radiCompRec" << endl;
      }

      fclose(fptr);
   }
   
   //***
   // Open and parse CEOS SAR data File to read the image options:
   //***
   fptr = fopen(theImageFileName.chars(), "r");
   if (!fptr)
   {
      CLOG << " ERROR:\n\tCannot open CEOS image file: " << theImageFileName
           << endl;
      theErrorStatus = ERROR;
      return;
   }
   sizeOfRec = sizeof(theImopDescRec);
   fread(&theImopDescRec, sizeOfRec, 1, fptr);
   fclose(fptr);

   //***
   // Open and parse trailer file:
   //***
   if (!theTrailerFileName.empty())
   {
      fptr = fopen(theTrailerFileName.chars(), "r");
      if (!fptr)
      {
         CLOG << " ERROR:\n\tCannot open CEOS Trailer file: "
              << theTrailerFileName << endl;
         theErrorStatus = ERROR;
         return;
      }
      fread(&(theSarTrailerFile.sarDescRec), 720, 1, fptr);

      //***
      // Establish which CEOS records are in the Trailer file, and read them.
      // First the dataset summary record:
      //***
      strncpy(bufI6, theSarTrailerFile.sarDescRec.n_dataset, 6);
      numRecords = atoi(bufI6);
      if (numRecords)
      {
         fread(&(theSarTrailerFile.dataSetSumRec), sizeOfNextRec(fptr),1, fptr);
         theDataSetSumRec = &(theSarTrailerFile.dataSetSumRec);
         if (traceDebug()) clog << "\t Trailer: dataSetSumRec" << endl;
      }

      //***
      // Data quality summary record:
      //***
      strncpy(bufI6, theSarTrailerFile.sarDescRec.n_qual_sum, 6);
      numRecords = atoi(bufI6);
      if (numRecords)
      {
         fread(&(theSarTrailerFile.qualSumRec), sizeOfNextRec(fptr), 1, fptr);
         theQualSumRec = &(theSarTrailerFile.qualSumRec);
         if (traceDebug()) clog << "\t Trailer: qualSumRec" << endl;
      }

      //***
      // Histogram records:
      //***
      strncpy(bufI6, theSarTrailerFile.sarDescRec.n_data_hist, 6);
      numRecords = atoi(bufI6);
      if (numRecords)
      {
         fread(&(theSarTrailerFile.sdrHistRec), sizeOfNextRec(fptr), 1, fptr);
         theSdrHistRec = &(theSarTrailerFile.sdrHistRec);
         fread(&(theSarTrailerFile.pdr8HistRec), 16920, 1, fptr);
         thePdr8HistRec = &(theSarTrailerFile.pdr8HistRec);
         if (traceDebug()) clog << "\t Trailer: sdrHistRec & pdr8HistRec"
                                << endl;
      }

      //***
      // Processing parameter record:
      //***
      strncpy(bufI6, theSarTrailerFile.sarDescRec.n_det_proc, 6);
      numRecords = atoi(bufI6);
      if (numRecords)
      {
         fread(&(theSarTrailerFile.procParmRec), sizeOfNextRec(fptr), 1, fptr);
         theProcParmRec = &(theSarTrailerFile.procParmRec);
         if (traceDebug()) clog << "\t Trailer: procParmRec" << endl;
      }

      //***
      // Attitude data record:
      //***
      strncpy(bufI6, theSarTrailerFile.sarDescRec.n_att_data, 6);
      numRecords = atoi(bufI6);
      if (numRecords)
      {
         fread(&(theSarTrailerFile.attDataRec), sizeOfNextRec(fptr), 1, fptr);
         theAttDataRec = &(theSarTrailerFile.attDataRec);
         if (traceDebug()) clog << "\t Trailer: attDataRec" << endl;
      }

      //***
      // Radiometric data record:
      //***
      strncpy(bufI6, theSarTrailerFile.sarDescRec.n_radi_data, 6);
      numRecords = atoi(bufI6);
      if (numRecords)
      {
         fread(&(theSarTrailerFile.radiDataRec), sizeOfNextRec(fptr), 1, fptr);
         theRadiDataRec = &(theSarTrailerFile.radiDataRec);
         if (traceDebug()) clog << "\t Trailer: radiDataRec" << endl;
      }

      //***
      // Radiometric compensation data record:
      //***
      strncpy(bufI6, theSarTrailerFile.sarDescRec.n_radi_comp, 6);
      numRecords = atoi(bufI6);
      if (numRecords)
      {
         fread(&(theSarTrailerFile.radiCompRec), sizeOfNextRec(fptr), 1, fptr);
         theRadiCompRec = &(theSarTrailerFile.radiCompRec);
         if (traceDebug()) clog << "\t Trailer: radiCompRec" << endl;
      }

      fclose(fptr);
   }

   if (traceExec()) CLOG << " Exiting method." << endl;
}


//******************************************************************************
// PRIVATE METHOD: ossimCeosData::sizeOfNextRec(FILE*)
//  
//  Reads the descriptor record pointed to by the file ptr and returns the
//  size of the following record in bytes.
//  
//******************************************************************************
int ossimCeosData::sizeOfNextRec(FILE* fptr)
{
   static const char MODULE[] = "ossimCeosData::sizeOfNextRec(FILE* fptr)";
   if (traceExec())  CLOG << "entering..." << endl;
   
   static desc_rec descRec;
   static int DESC_REC_SIZE = 12;
   
   fread(&descRec, DESC_REC_SIZE, 1, fptr);
   fseek(fptr, (long) -DESC_REC_SIZE, SEEK_CUR);
   
   if (traceDebug())
   {
      CLOG << "DEBUG -- "
           << "\n\t length = " << descRec.length << endl;
   }

   if (traceExec())  CLOG << "returning..." << endl;
   return descRec.length;
}
