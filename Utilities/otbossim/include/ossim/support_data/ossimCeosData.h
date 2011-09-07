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
#ifndef ossimCeosData_HEADER
#define ossimCeosData_HEADER

#include <cstdio>
#include <iostream>

#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimReferenced.h>

struct desc_rec                  // Record description                  
{
   ossim_int32   rec_seq;               // Record sequence number (B4) 
   ossim_uint8   rec_sub1;              // First record sub-type code (B1) 
   ossim_uint8   rec_type;              // Record type code (B1) 
   ossim_uint8   rec_sub2;              // Second record sub-type code (B1) 
   ossim_uint8   rec_sub3;              // Third record sub-type code (B1) 
   ossim_int32   length;                // Length of this record, bytes (B4) 
};


struct vol_desc_rec              // Volume descriptor record            
{
   struct desc_rec desc;         // Record description                  
   char   ascii_flag[2];         // ASCII flag (A2) 
   char   spare1[2];             // Unused (A2) 
   char   format_doc[12];        // Format control documentation (A12) 
   char   format_ver[2];         // Format doc version (A2) 
   char   format_rev[2];         // Format doc revision (A2) 
   char   software_id[12];       // Software identifier (A12) 
   char   phyvol_id[16];         // Physical volume identifier (A16) 
   char   logvol_id[16];         // Logical volume identifier (A16) 
   char   volset_id[16];         // Volume set identifier (A16) 
   char   phyvol_cnt[2];         // Total physical volume count (I2) 
   char   first_phyvol[2];       // Physical vol of first tape (I2) 
   char   last_phyvol[2];        // Physical vol of last tape (I2) 
   char   curr_phyvol[2];        // Physical vol of current tape (I2) 
   char   first_file[4];         // First file number in phyvol (I4) 
   char   volset_log[4];         // Logical volume within set (I4) 
   char   phyvol_log[4];         // Logical volume within phyvol (I4) 
   char   logvol_date[8];        // Logvol creation date (A8) 
   char   logvol_time[8];        // Logvol creation time (A8) 
   char   logvol_country[12];    // Logvol generation country (A12) 
   char   logvol_agency[8];      // Logvol generation agency (A8) 
   char   logvol_facility[12];   // Logvol generation facility (A12) 
   char   n_filepoint[4];        // Number of file pointer records (I4) 
   char   n_voldir[4];           // Number of voldir records (I4) 
   char   spare2[92];            // Unused (A92) 
   char   product_id[8];         // Product identifier (A8) 
   char   spare3[92];            // Local use segment (A92) 
};


struct file_pntr_rec             // File pointer record                 
{
   struct desc_rec desc;         // Record description                  
   char   ascii_flag[2];         // ASCII flag (A2) 
   char   spare1[2];             // Unused (A2) 
   char   file_num[4];           // Referenced file number (I4) 
   char   file_name[16];         // Referenced file name (A16) 
   char   file_class[28];        // Referenced file class (A28) 
   char   file_code[4];          // Referenced file class code (A4) 
   char   data_type[28];         // Referenced file data type (A28) 
   char   data_code[4];          // Referenced file data type code (A4) 
   char   nrec[8];               // Referenced file record count (I8) 
   char   first_len[8];          // First record length, bytes (I8) 
   char   max_len[8];            // Maximum record length, bytes (I8) 
   char   len_type[12];          // Record length type (A12) 
   char   len_code[4];           // Record length type code (A4) 
   char   first_phyvol[2];       // First physical volume (I2) 
   char   last_phyvol[2];        // Last physical volume (I2) 
   char   first_rec[8];          // First phyvol record (I8) 
   char   last_rec[8];           // Last phyvol record (I8) 
   char   spare2[100];           // Unused (A100) 
   char   spare3[100];           // Unused (A100) 
};


struct text_rec                  // Text record                         
{
   struct desc_rec desc;         // Record description                  
   char   ascii_flag[2];         // ASCII flag (A2) 
   char   cont_flag[2];          // Continuation flag (A2) 
   char   product_type[40];      // Product type specifier (A40) 
   char   product_create[60];    // Product creation info (A60) 
   char   phyvol_id[40];         // Physical volume identifier (A40) 
   char   scene_id[40];          // Scene identifier (A40) 
   char   scene_loc[40];         // Scene location (A40) 
   char   copyright_info[20];    // Unused (A20) 
   char   spare2[104];           // Unused (A104) 
};


struct sar_desc_rec              // SAR leader/trailer descriptor       
{
   struct desc_rec desc;         // Record description                  
   char   ascii_flag[2];         // ASCII flag (A2) 
   char   spare1[2];             // Unused (A2) 
   char   format_doc[12];        // Format control document (A12) 
   char   format_rev[2];         // Format document revision (A2) 
   char   design_rev[2];         // File design revision (A2) 
   char   software_id[12];       // Software identifier (A12) 
   char   file_num[4];           // File number (I4) 
   char   file_name[16];         // File name (A16) 
   char   rec_seq[4];            // Record sequence/location flag (A4) 
   char   seq_loc[8];            // Sequence number location (I8) 
   char   seq_len[4];            // Sequence number length (I4) 
   char   rec_code[4];           // Record code/location flag (A4) 
   char   code_loc[8];           // Record code location (I8) 
   char   code_len[4];           // Record code length (I4) 
   char   rec_len[4];            // Record length/location flag (A4) 
   char   rlen_loc[8];           // Record length location (I8) 
   char   rlen_len[4];           // Record length, bytes (I4) 
   char   spare2;                // Reserved (A1) 
   char   spare3;                // Reserved (A1) 
   char   spare4;                // Reserved (A1) 
   char   spare5;                // Reserved (A1) 
   char   spare6[64];            // Reserved segment (A64) 
   char   n_dataset[6];          // Number of dataset summary records (I6) 
   char   l_dataset[6];          // Data set summary record length, bytes (I6) 
   char   n_map_proj[6];         // Number of map proj records (I6) 
   char   l_map_proj[6];         // Map proj record length, bytes (I6) 
   char   n_plat_pos[6];         // Number of plat pos records (I6) 
   char   l_plat_pos[6];         // Plat pos record length, bytes (I6) 
   char   n_att_data[6];         // Number of att data records (I6) 
   char   l_att_data[6];         // Att data record length, bytes (I6) 
   char   n_radi_data[6];        // Number of radiometric data records (I6) 
   char   l_radi_data[6];        // Radi data record length, bytes (I6) 
   char   n_radi_comp[6];        // Number of radiometric compensation records (I6)
   char   l_radi_comp[6];        // Radiometric compensation record length, bytes (I6)
   char   n_qual_sum[6];         // Number of data qual sum records (I6) 
   char   l_qual_sum[6];         // Dat qual sum record length,bytes (I6) 
   char   n_data_hist[6];        // Number of data histogram records (I6) 
   char   l_data_hist[6];        // Data hist record length, bytes (I6) 
   char   n_rang_spec[6];        // Number of range spectra records (I6) 
   char   l_rang_spec[6];        // Range spectra record length, bytes (I6) 
   char   n_dem_desc[6];         // Number of DEM descriptor records (I6) 
   char   l_dem_desc[6];         // DEM desc record length, bytes (I6) 
   char   n_radar_par[6];        // Number of RADAR par records (I6) 
   char   l_radar_par[6];        // RADAR par record length, bytes (I6) 
   char   n_anno_data[6];        // Number of annotation data records (I6) 
   char   l_anno_data[6];        // Annotation data record length, bytes (I6) 
   char   n_det_proc[6];         // Number of processing parameter records (I6) 
   char   l_det_proc[6];         // Processing parameter record length, bytes (I6)
   char   n_cal[6];              // Number of calibration records (I6) 
   char   l_cal[6];              // Calibration record length, bytes (I6) 
   char   n_gcp[6];              // Number of GCP records (I6) 
   char   l_gcp[6];              // GCP record length, bytes (I6) 
   char   spare7[10][6];         // Unused (I6) 
   char   n_fac_data[6];         // Number of facility data records (I6) 
   char   l_fac_data[6];         // Fac data record length, bytes (I6) 
   char   spare8[288];           // Unused (A288) 
};


struct dataset_sum_rec           // Data set summary record                  
{
   struct desc_rec desc;         // Record description                       
   char   seq_num[4];            // Sequence number (I4) 
   char   sar_chn[4];            // SAR channel indicator (I4) 
   char   scene_id[16];          // Scene identifier (A16) 
   char   scene_des[32];         // Scene designator (A32) 
   char   inp_sctim[32];         // Input scene centre time (A32) 
   char   asc_des[16];           // Ascending/descending (A16) 
   char   pro_lat[16];           // Processed scene centre latitude (F16.7) 
   char   pro_long[16];          // Processed scene centre longitude (F16.7) 
   char   pro_head[16];          // Processed scene centre headng (F16.7) 
   char   ellip_des[16];         // Ellipsoid designator (A16) 
   char   ellip_maj[16];         // Ellipsoid semi-major axis, km (F16.7) 
   char   ellip_min[16];         // Ellipsoid semi_minor axis, km (F16.7) 
   char   earth_mass[16];        // Earth's mass (E16.7) 
   char   grav_const[16];        // Gravitational constant (E16.7) 
   char   ellip_j[3][16];        // Ellipsoid J2-4 parameters (E16.7) 
   char   spare1[16];            // Unused (A16) 
   char   terrain_h[16];         // Average terrain height, km (F16.7) 
   char   sc_lin[8];             // Scene centre line number (I8) 
   char   sc_pix[8];             // Scene centre pixel number (I8) 
   char   scene_len[16];         // Scene length, km (F16.7) 
   char   scene_wid[16];         // Scene width, km (F16.7) 
   char   spare2[16];            // Unused (A16) 
   char   nchn[4];               // Number of SAR channels (I4) 
   char   spare3[4];             // Unused (A4) 
   char   mission_id[16];        // Mission identifier (A16) 
   char   sensor_id[32];         // Sensor identifier (A32) 
   char   orbit_num[8];          // Orbit number (A8) 
   char   plat_lat[8];           // Platform geodetic latitude (F8.3) 
   char   plat_long[8];          // Platform geodetic longitude (F8.3) 
   char   plat_head[8];          // Platform heading (F8.3) 
   char   clock_ang[8];          // Sensor clock angle (F8.3) 
   char   incident_ang[8];       // Incidence angle (F8.3) 
   char   spare4[8];             // Unused (A8) 
   char   wave_length[16];       // Radar wave length (F16.7) 
   char   motion_comp[2];        // Motion compensation indicator (A2) 
   char   pulse_code[16];        // Range pulse code specifier (A16) 
   char   ampl_coef[5][16];      // Range chirp coefficients (E16.7) 
   char   phas_coef[5][16];      // Range phase coefficients (E16.7) 
   char   chirp_ext_ind[8];      // Chirp extraction index (I8) 
   char   spare5[8];             // Unused (A8) 
   char   fr[16];                // Range sampling rate (F16.7) 
   char   rng_gate[16];          // Range gate start time (F16.7) 
   char   rng_length[16];        // Range pulse length (F16.7) 
   char   baseband_f[4];         // Baseband conversion flag (A4) 
   char   rngcmp_f[4];           // Range compressed flag (A4) 
   char   gn_polar[16];          // Like polarized gain (F16.7) 
   char   gn_cross[16];          // Cross polarized gain (F16.7) 
   char   chn_bits[8];           // Number of bits per channel (I8) 
   char   quant_desc[12];        // Quantization descriptor (A12) 
   char   i_bias[16];            // I channel DC bias (F16.7) 
   char   q_bias[16];            // Q channel DC bias (F16.7) 
   char   iq_ratio[16];          // I/Q channel ratio (F16.7) 
   char   spare6[16];            // Unused (F16.7) 
   char   spare7[16];            // Unused (F16.7) 
   char   ele_sight[16];         // Electronic boresight (F16.7) 
   char   mech_sight[16];        // Mechanical boresight (F16.7) 
   char   echo_track[4];         // Echo tracker on/off flag (A4) 
   char   fa[16];                // Nominal PRF, Hz (F16.7) 
   char   elev_beam[16];         // Elevation beamwidth (F16.7) 
   char   azim_beam[16];         // Azimuth beamwidth (F16.7) 
   char   sat_bintim[16];        // Satellite binary time (I16) 
   char   sat_clktim[32];        // Satellite clock time (I32) 
   char   sat_clkinc[8];         // Satellite clock increment (I8) 
   char   spare8[8];             // Unused (A8) 
   char   fac_id[16];            // Processing facility identifier (A16) 
   char   sys_id[8];             // Processing system identifier (A8) 
   char   ver_id[8];             // Processing version identifier (A8) 
   char   fac_code[16];          // Facility process code (A16) 
   char   lev_code[16];          // Product level code (A16) 
   char   prod_type[32];         // Product type specifier (A32) 
   char   algor_id[32];          // Processing algorithm identifier (A32) 
   char   n_azilok[16];          // Number of azimuth looks (F16.7) 
   char   n_rnglok[16];          // Number of range looks (F16.7) 
   char   bnd_azilok[16];        // Bandwidth per look in azimuth,Hz (F16.7) 
   char   bnd_rnglok[16];        // Bandwidth per look in range,Hz (F16.7) 
   char   bnd_azi[16];           // Total azimuth look bandwidth (F16.7) 
   char   bnd_rng[16];           // Total range look bandwidth (F16.7) 
   char   azi_weight[32];        // Azimuth weighting designator (A32) 
   char   rng_weight[32];        // Range weighting designator (A32) 
   char   data_inpsrc[16];       // Data input source (A16) 
   char   rng_res[16];           // Range resolution, meter (F16.7) 
   char   azi_res[16];           // Azimuth resolution, meter (F16.7) 
   char   radi_stretch[2][16];   // Radiometric stretch terms (F16.7) 
   char   alt_dopcen[3][16];     // Along track Doppler frequency terms (E16.7) 
   char   spare9[16];            // Unused (A16) 
   char   crt_dopcen[3][16];     // Cross track Doppler frequency terms (E16.7) 
   char   time_dir_pix[8];       // Pixel time direction indicator (A8) 
   char   time_dir_lin[8];       // Line time direction indicator (A8) 
   char   alt_rate[3][16];       // Along track Doppler rate term (E16.7)
   char   spare10[16];           // Unused (A16) 
   char   crt_rate[3][16];       // Cross track Doppler rate term (E16.7)
   char   spare11[16];           // Unused (A16) 
   char   line_cont[8];          // Line content indicator (A8) 
   char   clutter_lock[4];       // Clutter lock applied flag (A4) 
   char   auto_focus[4];         // Auto-focus applied flag (A4) 
   char   line_spacing[16];      // Line spacing, meters (F16.7) 
   char   pix_spacing[16];       // Pixel spacing, meters (F16.7) 
   char   rngcmp_desg[16];       // Range compression designator (A16) 
   char   spare12[2362];         // Unused (A2362) 
};


struct rad_unc_rec               // Radiometric uncertainty record
{
   char   db[16];                // Units of db (F16.7)  
   char   deg[16];               // Units of deg (F16.7)  
};

struct misreg_rec                // Misregistration record 
{
   char   alt_m[16];             // Nominal along track misregistration (F16.7)
   char   crt_m[16];             // Nominal cross track misregistration (F16.7)
};

struct qual_sum_rec              // Data quality summary record 
{
   struct desc_rec desc;         // Record description
   char   rec_seq[4];            // Record sequence number (I4) 
   char   sar_chn[4];            // SAR channel indicator (A4) 
   char   cali_date[6];          // Calibration update date (A6) 
   char   nchn[4];               // Number of channels (I4) 
   char   islr[16];              // Nominal integrated side lobe ratio (F16.7) 
   char   pslr[16];              // Nominal peak side lobe ratio (F16.7) 
   char   azi_ambig[16];         // Nominal azimuth ambiguity (F16.7) 
   char   rng_ambig[16];         // Nominal range ambiguity (F16.7) 
   char   snr[16];               // Nominal signal to noise ratio (F16.7) 
   char   ber[16];               // Nominal bit error rate (F16.7) 
   char   rng_res[16];           // Nominal slant range resolution (F16.7) 
   char   azi_res[16];           // Nominal azimuth resolution,meter (F16.7) 
   char   rad_res[16];           // Nominal radiometric resolution,dB (F16.7) 
   char   dyn_rng[16];           // Instantaneous dynamic range (F16.7) 
   char   rad_unc_db[16];        // Nominal radiometric uncertainty,dB (F16.7) 
   char   rad_unc_deg[16];       // Nominal Radiometric uncertainty,deg (F16.7) 
   struct rad_unc_rec rad_unc[16];   // Radiometric data quality          
   char   alt_locerr[16];        // Nominal along track location error (F16.7) 
   char   crt_locerr[16];        // Nominal cross track location error (F16.7) 
   char   alt_scale[16];         // Nominal along track distortion scale (F16.7)
   char   crt_scale[16];         // Nominal cross track distortion scale (F16.7)
   char   dis_skew[16];          // Nominal distortion skew (F16.7) 
   char   ori_err[16];           // Nominal Scene orientation error (F16.7) 
   struct misreg_rec misreg[16]; // Misregistration error
   char   nesz[16];              // Nominal noise equivalent sigma zero (F16.7) 
   char   enl[16];               // Nominal equivalent number of looks (F16.7) 
   char   tb_update[8];          // Default parameters table update date (A8) 
   char   spare[238];            // Unused (A238) 
};


struct sdr_htab_rec              // Histogram table record 
{
   char   hist_desc[32];         // Histogram descriptor (A32) 
   char   nrec[4];               // Records per table (I4) 
   char   tab_seq[4];            // Table sequence number (I4) 
   char   nbin[8];               // Total number of table bins (I8) 
   char   ns_lin[8];             // Total number of lines (I8) 
   char   ns_pix[8];             // Total number of pixels per line (I8) 
   char   ngrp_lin[8];           // Group size along line axis (I8) 
   char   ngrp_pix[8];           // Group size along pixel axis (I8) 
   char   nsamp_lin[8];          // Number of samples used per group along line axis (I8) 
   char   nsamp_pix[8];          // Number of samples used per group along pixel (I8) 
   char   min_smp[16];           // Minimum first bin (E16.7) 
   char   max_smp[16];           // Maximum last bin (E16.7) 
   char   mean_smp[16];          // Mean sample value (E16.7) 
   char   std_smp[16];           // Sample standard deviation (E16.7) 
   char   smp_inc[16];           // Sample value increment (E16.7) 
   char   min_hist[16];          // Minimum histogram value (E16.7) 
   char   max_hist[16];          // Maximum histogram value (E16.7) 
   char   mean_hist[16];         // Histogram mean value (E16.7) 
   char   std_hist[16];          // Histogram standard deviation (E16.7) 
   char   nhist[8];              // Histogram table size (I8) 
   char   hist[256][8];          // 256 histogram table values of 16 bins
   //   I x 16 bins Q (I8) 
};

struct sdr_hist_rec              // Data histogram record
{
   struct desc_rec desc;         // Record description
   char   rec_seq[4];            // Record sequence number (I4) 
   char   sar_chn[4];            // SAR channel number (I4) 
   char   ntab[8];               // Number of histogram table data sets in this record (I8) 
   char   ltab[8];               // Histogram table data set size (I8) 
   struct sdr_htab_rec htab;     // Histogram tables
   char   spare[14588];          // Unused (A14588) 
};


struct pdr16_htab_rec            // Histogram table record              
{
   char   hist_desc[32];         // Histogram descriptor (A32) 
   char   nrec[4];               // Records per table (I4) 
   char   tab_seq[4];            // Table sequence number (I4) 
   char   nbin[8];               // Total number of table bins (I8) 
   char   ns_lin[8];             // Total number of lines n (I8) 
   char   ns_pix[8];             // Total number of pixels per line (I8) 
   char   ngrp_lin[8];           // Group size along line axis (I8) 
   char   ngrp_pix[8];           // Group size along pixel axis (I8) 
   char   nsamp_lin[8];          // Number of samples used per group along line axis (I8) 
   char   nsamp_pix[8];          // Number of samples used per group along pixel (I8) 
   char   min_smp[16];           // Minimum first bin (E16.7) 
   char   max_smp[16];           // Maximum last bin (E16.7) 
   char   mean_smp[16];          // Mean sample value (E16.7) 
   char   std_smp[16];           // Sample standard deviation (E16.7) 
   char   smp_inc[16];           // Sample value increment (E16.7) 
   char   min_hist[16];          // Minimum histogram value (E16.7) 
   char   max_hist[16];          // Maximum histogram value (E16.7) 
   char   mean_hist[16];         // Histogram mean value (E16.7) 
   char   std_hist[16];          // Histogram standard deviation (E16.7) 
   char   nhist[8];              // Histogram table size (I8) 
   char   hist[1024][8];         // 1024 histogram table values for 1024 bins
   // (I8) 
};

struct pdr16_hist_rec            // Data histogram record
{
   struct desc_rec desc;         // Record description
   char   rec_seq[4];            // Record sequence number (I4) 
   char   sar_chn[4];            // SAR channel number (I4) 
   char   ntab[8];               // Number of histogram table data sets in this record (I8) 
   char   ltab[8];               // Histogram table data set size (I8) 
   struct pdr16_htab_rec htab[2];// Histogram tables 
   char   spare[3];              // Unused (A3) 
};


struct pix_count_rec             // Beam pixel count record 
{
   char   pix_update[21];        // Pixel count update date/time (A21) 
   char   n_pix[4][8];           // Count of image pixels in beams (4I8) 
};

struct beam_info_rec             // Beam information record
{
   char   beam_type[3];          // Beam type (A3) 
   char   beam_look_src[9];      // Elevation beam look angle source (A9) 
   char   beam_look_ang[16];     // Applied elevation beam look angle (deg) (F16.7) 
   char   prf[16];               // Actual PRF (Hz) (F16.7) 
};

struct temp_rec                  // Temperature settings record
{
   char   temp_set[4][4];        // Temperature settings (4I4) 
};

struct dopcen_est_rec
{
   char   dopcen_conf[16];       // Doppler centroid confidence measure (F16.7) 
   char   dopcen_ref_tim[16];    // Doppler centroid reference time (sec) (F16.7) 
   char   dopcen_coef[4][16];    // Doppler centroid coefficients (4F16.7) 
};

struct srgr_coefset_rec
{
   char   srgr_update[21];       // SRGR update date/time (A21) 
   char   srgr_coef[6][16];      // SRGR coefficients (6F16.7) 
};

struct proc_parm_rec             // Detailed processing parameters record
{
   struct desc_rec desc;         // Record description
   char   rec_seq[4];            // Record sequence number (I4) 
   char   spare1[4];             // Unused (A4) 
   char   inp_media[3];          // Input media (A3) 
   char   n_tape_id[4];          // Number of tape input ids (I4) 
   char   tape_id[10][8];        // Tape identifiers (10A8) 
   char   exp_ing_start[21];     // Expected ingest start time (A21) 
   char   exp_ing_stop[21];      // Expected ingest stop time (A21)  
   char   act_ing_start[21];     // Actual ingest start time (A21) 
   char   act_ing_stop[21];      // Actual ingest stop time (A21) 
   char   proc_start[21];        // Processing start time (A21) 
   char   proc_stop[21];         // Processing stop time (A21) 
   char   mn_sig_lev[10][16];    // Mean signal levels across range (10F16.7) 
   char   scr_data_ind[4];       // Source data quality indicator (I4) 
   char   miss_ln[8];            // Number of missing lines (I8) 
   char   rej_ln[8];             // Number of rejected lines (I8) 
   char   large_gap[8];          // Number of time inconsistencies (large gaps) (I8) 
   char   bit_err_rate[16];      // Measured bit error rate (E16.7) 
   char   fm_crc_err[16];        // Percent of frames with CRC errors (E16.7) 
   char   date_incons[8];        // Number of date inconsistencies (I8) 
   char   prf_changes[8];        // Number of unexpected PRF changes (I8) 
   char   delay_changes[8];      // Number of delay changes (I8) 
   char   skipd_frams[8];        // Number of skipped frames (I8) 
   char   rej_bf_start[8];       // Range lines rejected before start time (I8) 
   char   rej_few_fram[8];       // Range lines rejected due to too few frames   (I8) 
   char   rej_many_fram[8];      // Range lines rejected due to too many frames   (I8) 
   char   rej_mchn_err[8];       // Frames rejected due to master channel error (I8) 
   char   rej_vchn_err[8];       // Frames rejected due to virtual channel error (I8) 
   char   rej_rec_type[8];       // Frames rejected due to incorrect recording type (I8) 
   char   sens_config[10];       // Sensor configuration (A10) 
   char   sens_orient[9];        // Sensor orientation (A9) 
   char   sych_marker[8];        // Frame synch marker (I8) 
   char   rng_ref_src[12];       // Range reference function source (A12) 
   char   rng_amp_coef[4][16];   // Range reference amplitude coefficients (4E16.7) 
   char   rng_phas_coef[4][16];  // Range reference phase coefficients (4E16.7) 
   char   err_amp_coef[4][16];   // Error function amplitude coefficients (4E16.7) 
   char   err_phas_coef[4][16];  // Error function phase coefficients (4E16.7) 
   char   pulse_bandw[4];        // Pulse bandwidth code (I4) 
   char   adc_samp_rate[5];      // ADC sampling rate (I5) 
   char   rep_agc_attn[16];      // Replica AGC attenuation (F16.7) 
   char   gn_corctn_fctr[16];    // Gain correction factor (dB) (F16.7) 
   char   rep_energy_gn[16];     // Replica energy gain correction (F16.7) 
   char   orb_data_src[11];      // Orbit data source (A11) 
   char   pulse_cnt_1[4];        // Pulse count 1 (I4) 
   char   pulse_cnt_2[4];        // Pulse count 2 (I4) 
   char   beam_edge_rqd[3];      // Beam edge detection requested (A3) 
   char   beam_edge_conf[16];    // Beam edge confidence measure (F16.7) 
   char   pix_overlap[4];        // Number of pixels in beam overlap (I4) 
   char   n_beams[4];            // Number of beams (I4) 
   struct beam_info_rec beam_info[4]; // Beam info 
   char   n_pix_updates[4];      // Number of pixel count updates (I4) 
   struct pix_count_rec pix_count[20];   // Beam pixel counts
   char   pwin_start[16];        // Processing window start time (sec) (F16.7) 
   char   pwin_end[16];          // Processing window end time (sec) (F16.7) 
   char   recd_type[9];          // Recording type (A9) 
   char   temp_set_inc[16];      // Time increment between temperature settings (sec) (F16.7) 
   char   n_temp_set[4];         // Number of temperature settings (I4) 
   struct temp_rec temp[20];     // Temperature settings 
   char   n_image_pix[8];        // Number of image pixels (I8) 
   char   prc_zero_pix[16];      // Percent zero pixels (F16.7) 
   char   prc_satur_pix[16];     // Percent saturated pixels (F16.7) 
   char   img_hist_mean[16];     // Image histogram mean intensity (F16.7) 
   char   img_cumu_dist[3][16];  // Image cumulative distribution (3F16.7) 
   char   pre_img_gn[16];        // Pre-image calibration gain factor (F16.7) 
   char   post_img_gn[16];       // Post-image calibration gain factor (F16.7) 
   char   dopcen_inc[16];        // Time increment between Dopcen estimates (sec) (F16.7) 
   char   n_dopcen[4];           // Number of Doppler centroid estimates (I4) 
   struct dopcen_est_rec dopcen_est[20]; // Doppler centroid estimates
   char   dop_amb_err[4];        // Doppler ambiguity error (I4) 
   char   dopamb_conf[16];       // Doppler ambiguity confidence measure (F16.7)
   char   eph_orb_data[7][16];   // Ephemeris orbit data (7E16.7) 
   char   appl_type[12];         // Application type (A12) 
   char   slow_time_coef[5][22]; // Slow time coefficients (5D22.15) 
   char   n_srgr[4];             // Number of SRGR coefficient sets (I4) 
   struct srgr_coefset_rec srgr_coefset[20]; // SRGR coefficient sets 
   char   pixel_spacing[16];     // SGF product pixel spacing (F16.7) 
   char   gics_reqd[3];          // GICS product required (A3) 
   char   wo_number[8];          // Work order identifier (A8) 
   char   wo_date[20];           // Work order entry date (A20) 
   char   satellite_id[10];      // Satellite identifier (A10) 
   char   user_id[20];           // User id (A20) 
   char   complete_msg[3];       // Completion message required flag (A3) 
   char   scene_id[15];          // SGF product scene identifier (A15) 
   char   density_in[4];         // Density of SGF product media (A4) 
   char   media_id[8];           // SGF product identifier (A8) 
   char   angle_first[16];       // Incidence angle of first pixel in SGF product line (F16.7) 
   char   angle_last[16];        // Incidence angle of last pixel in SGF product line (F16.7) 
   char   prod_type[3];          // GICS output product type (A3) 
   char   map_system[16];        // Map system identifier (A16) 
   char   centre_lat[22];        // GICS output product scene centre latitude (D22.15) 
   char   centre_long[22];       // GICS output product scene centre longitude (D22.15) 
   char   span_x[22];            // GICS output product size - map eastings (km) (D22.15) 
   char   span_y[22];            // GICS output product size - map northings (km) (D22.15) 
   char   apply_dtm[3];          // DTM correction to be applied flag (A3) 
   char   density_out[4];        // GICS output product density (A4) 
   char   state_time[21];        // Time of the first state vector (A21) 
   char   num_state_vectors[4];  // Number of state vectors (I4) 
   char   state_time_inc[16];    // Time increment between state vectors (F16.7)
   char   spare2[206];           // Unused (A207) 
};


struct map_proj_rec              // Map projection data record          
{
   struct desc_rec desc;         // Record description                  
   char   spare1[16];            // Unused (A16) 
   char   map_desc[32];          // Map projection descriptor (A32) 
   char   n_pixel[16];           // Pixels per line (I16) 
   char   n_line[16];            // Lines per processed band (I16) 
   char   pixel_spacing[16];     // Inter-pixel distance (F16.7) 
   char   line_spacing[16];      // Inter-line distance (F16.7) 
   char   osc_orient[16];        // Output scene cntr orientation (F16.7)
   char   orb_incl[16];          // Orbital inclination (F16.7) 
   char   asc_node[16];          // Ascending node (F16.7) 
   char   isc_dist[16];          // Input scene cntr plat dist (F16.7) 
   char   geo_alt[16];           // Geodedic platform altitude (F16.7) 
   char   isc_vel[16];           // Ground speed at input scene (F16.7) 
   char   plat_head[16];         // Platform heading (F16.7) 
   char   ref_ellip[32];         // Reference ellipsoid name (A32) 
   char   semi_major[16];        // Ellipsoid semi-major axis (F16.7) 
   char   semi_minor[16];        // Ellipsoid semi-minor axis (F16.7) 
   char   datum_shift[3][16];    // Datum shift parameters (F16.7) 
   char   aux_datum_shift[3][16];// Aux datum shift parameters (F16.7) 
   char   scal_ellip[16];        // Ellisoid scale factor (F16.7) 
   char   proj_desc[32];         // Map projection description (A32) 
   char   utm_desc[32];          // UTM descriptor (A32) 
   char   utm_zone_sig[4];       // UTM zone signature (A4) 
   char   utm_east_orig[16];     // Map origin, false easting (F16.7) 
   char   utm_north_orig[16];    // Map origin, false northing (F16.7) 
   char   utm_cent_long[16];     // Projection centre longitude (F16.7) 
   char   utm_cent_lat[16];      // Projection centre latitude (F16.7) 
   char   utm_stand_par[2][16];  // Standard parallels (F16.7) 
   char   utm_scale[16];         // Scale factor (F16.7) 
   char   ups_desc[32];          // UPS descriptor (A32) 
   char   ups_cent_long[16];     // Projection centre longitude (F16.7) 
   char   ups_cent_lat[16];      // Projection centre latitude (F16.7) 
   char   ups_scale[16];         // Scale factor (F16.7) 
   char   nsp_desc[32];          // NSP descriptor (A32) 
   char   nsp_east_orig[16];     // Map origin, false easting (F16.7) 
   char   nsp_north_orig[16];    // Map origin, false northing (F16.7) 
   char   nsp_cent_long[16];     // Projection centre longitude (F16.7) 
   char   nsp_cent_lat[16];      // Projection centre latitude (F16.7) 
   char   nsp_stand_par[4][16];  // Standard parallels (F16.7) 
   char   nsp_stand_mer[3][16];  // Standard meridians (F16.7) 
   char   nsp_spare[4][16];      // Project dependent (4A16) 
   char   corner_ne[8][16];      // Corner northing/easting (F16.7) 
   char   corner_ll[8][16];      // Corner latitude/longitude (F16.7) 
   char   terr_height[4][16];    // Corner terrain height (F16.7) 
   char   lp_conv_coef[8][20];   // Line/pixel coefficients (E20.10) 
   char   mp_conv_coef[8][20];   // Map proj coefficients (E20.10) 
   char   dem_type[4];           // DEM type (A4) 
   char   spare3[32];            // Unused (A32) 
};


struct pos_vect_rec              // Position vector record              
{
   char   pos[3][22];            // Data point position (D22.15) 
   char   vel[3][22];            // Data point velocity (D22.15) 
};

struct pos_data_rec               // Platform position data record       
{
   struct desc_rec desc;         // Record description                  
   char   orbit_ele_desg[32];    // Orbital elements designator (A32) 
   char   orbit_ele[6][16];      // Orbital elements (F16.7) 
   char   ndata[4];              // Number of data points (I4) 
   char   year[4];               // Year of data point (I4) 
   char   month[4];              // Month of data point (I4) 
   char   day[4];                // Day of data point (I4) 
   char   gmt_day[4];            // Day of year (I4) 
   char   gmt_sec[22];           // Seconds of day (D22.15) 
   char   data_int[22];          // Data sampling interval (D22.15) 
   char   ref_coord[64];         // Reference coordinate system (A64) 
   char   hr_angle[22];          // Greenwich mean hour angle (D22.15) 
   char   alt_poserr[16];        // Along track position error (F16.7) 
   char   crt_poserr[16];        // Cross track position error (F16.7) 
   char   rad_poserr[16];        // Radial position error (F16.7) 
   char   alt_velerr[16];        // Along track velocity error (F16.7) 
   char   crt_velerr[16];        // Cross track velocity error (F16.7) 
   char   rad_velerr[16];        // Radial velocity error (F16.7) 
   struct pos_vect_rec pos_vect[64]; // Data point position/velocity        
   char   spare[126];            // Unused (A126) 
};


struct att_vect_rec              // Attitude point record               
{
   char   gmt_day[4];            // Day of the year, GMT (I4) 
   char   gmt_sec[8];            // Milliseconds of day, GMT (I8) 
   char   pitch_flag[4];         // Pitch data quality flag (I4) 
   char   roll_flag[4];          // Roll data quality flag (I4) 
   char   yaw_flag[4];           // Yaw data quality flag (I4) 
   char   pitch[14];             // Pitch error, degrees (E14.6) 
   char   roll[14];              // Roll error, degrees (E14.6) 
   char   yaw[14];               // Yaw error, degrees (E14.6) 
   char   pitch_rate_flag[4];    // Pitch rate data quality flag (I4) 
   char   roll_rate_flag[4];     // Roll rate data quality flag (I4) 
   char   yaw_rate_flag[4];      // Yaw rate data quality flag (I4) 
   char   pitch_rate[14];        // Pitch rate, degrees/sec (E14.6) 
   char   roll_rate[14];         // Roll rate, degrees/sec (E14.6) 
   char   yaw_rate[14];          // Yaw rate, degrees/sec (E14.6) 
};

struct att_data_rec              // Attitude data record                
{
   struct desc_rec desc;         // Record description                  
   char   npoint[4];             // Number of data points (I4) 
   struct att_vect_rec att_vect[20]; // Data points                         
   char   pitch_bias[14];        // Pitch bias (degrees) (E14.6) 
   char   roll_bias[14];         // Roll bias (degrees) (E14.6) 
   char   yaw_bias[14];          // Yaw bias (degrees) (E14.6) 
   char   spare[6502];           // Unused (A6502) 
};


struct radi_dset_rec             // Radiometric data set record
{
   char   chan_ind[4];           // SAR channel indicator (A4) 
   char   spare1[4];             // Unused (A4) 
   char   table_desig[24];       // Table designator (A24) 
   char   n_samp[8];             // Number of lookup table samples (I8) 
   char   samp_type[16];         // Sample type designator (A16) 
   char   samp_inc[4];           // Increment between table entries, range samples (I4) 
   char   lookup_tab[512][16];   // Output scaling gain table (E16.7) 
   char   spare2[4];             // Unused (A4) 
   char   noise_scale[16];       // Thermal noise reference level (F16.7) 
   char   spare3[16];            // Unused (A16) 
   char   offset[16];            // Scaling offset (E16.7) 
   char   calib_const[16];       // Calibration constant (E16.7) 
};

struct radi_data_rec             // Radiometric data record
{
   struct desc_rec desc;         // Record description
   char   seq_num[4];            // Record sequence number (I4) 
   char   n_data[4];             // Number of data fields (I4) 
   char   field_size[8];         // Field size in bytes (I8) 
   struct radi_dset_rec dset[1]; // Radiometric data set records
   char   spare1[1512];          // Unused (A1512) 
};


struct comp_dset_rec             // Compensation data set record
{
   char   comp_desig[8];         // Compensation data designator (A8) 
   char   comp_descr[32];        // Compensation data descriptor (A32) 
   char   n_comp_rec[4];         // Number of compensation records (I4) 
   char   comp_seq_no[4];        // Compensation record sequence number (I4) 
   char   beam_tab_size[8];      // Number of beam table entries (I8) 
   char   beam_tab[256][16];     // Elevation gain beam profile (256F16.7) 
   char   beam_type[16];         // Beam type (A16) 
   char   look_angle[16];        // Look angle of beam table centre (F16.7) 
   char   beam_tab_inc[16];      // Increment between beam table entries (F16.7)
};

struct radi_comp_rec             // Radiometric compensation data record
{
   struct desc_rec desc;         // Record description 
   char   seq_num[4];            // Record sequence number (I4) 
   char   chan_ind[4];           // SAR channel indicator (A4) 
   char   n_dset[8];             // Number of data sets in record (I8) 
   char   dset_size[8];          // Compensation data set size (I8) 
   struct comp_dset_rec dset[4]; // Compensation data set records
};


struct imop_desc_rec             // Image options file descriptor       
{
   struct desc_rec desc;         // Record description                  
   char   ascii_flag[2];         // ASCII flag (A2) 
   char   spare1[2];             // Unused (A2) 
   char   format_doc[12];        // Format control document (A12) 
   char   format_rev[2];         // Format document revision (A2) 
   char   design_rev[2];         // File design revision (A2) 
   char   software_id[12];       // Software identifier (A12) 
   char   file_num[4];           // File number (I4) 
   char   file_name[16];         // File name (A16) 
   char   rec_seq[4];            // Record sequence/location flag (A4) 
   char   seq_loc[8];            // Sequence number location (I8) 
   char   seq_len[4];            // Sequence number length (I4) 
   char   rec_code[4];           // Record code/location flag (A4) 
   char   code_loc[8];           // Record code location (I8) 
   char   code_len[4];           // Record code length (I4) 
   char   rec_len[4];            // Record length/location flag (A4) 
   char   rlen_loc[8];           // Record length location (I8) 
   char   rlen_len[4];           // Record length length (I4) 
   char   spare2[4];             // Reserved (4A1) 
   char   spare3[64];            // Reserved segment (A64) 
   char   n_dataset[6];          // Number of SAR data records (I6) 
   char   l_dataset[6];          // SAR data record length (I6) 
   char   spare4[24];            // Unused (A24) 
   char   nbit[4];               // Number of bits per sample (I4) 
   char   nsamp[4];              // Samples per data group (I4) 
   char   nbyte[4];              // Bytes per data group (I4) 
   char   justify[4];            // Sample justification and order (A4) 
   char   nchn[4];               // Number of SAR channels (I4) 
   char   nlin[8];               // Lines per data set (I8) 
   char   nleft[4];              // Left border pixels per line (I4) 
   char   ngrp[8];               // Groups per line per channel (I8) 
   char   nright[4];             // Right border pixels per line (I4) 
   char   ntop[4];               // Top border lines (I4) 
   char   nbott[4];              // Bottom border lines (I4) 
   char   intleav[4];            // Interleave indicator (A4) 
   char   nrec_lin[2];           // Number of physical records per line (I2) 
   char   nrec_chn[2];           // Number of physical records per multi-channel line in this file (I2) 
   char   n_prefix[4];           // Number of bytes of prefix data per record (I4) 
   char   n_sar[8];              // Number of bytes of SAR data (or pixel data) per record (I8) 
   char   n_suffix[4];           // Number of bytes of suffix data per record (I4) 
   char   spare5[4];             // Unused (A4) 
   char   lin_loc[8];            // Line number locator (A8) 
   char   chn_loc[8];            // Channel number locator (A8) 
   char   tim_loc[8];            // Time locator (A8) 
   char   left_loc[8];           // Left fill locator (A8) 
   char   right_loc[8];          // Right fill locator (A8) 
   char   pad_ind[4];            // Pad pixel indicator (A4) 
   char   spare6[28];            // Unused (A28) 
   char   qual_loc[8];           // Quality code locator (A8) 
   char   cali_loc[8];           // Calibration info locator (A8) 
   char   gain_loc[8];           // Gain value locator (A8) 
   char   bias_loc[8];           // Bias value locator (A8) 
   char   type_id[28];           // Data type identifier (A28) 
   char   type_code[4];          // Data type code (A4) 
   char   left_fill[4];          // Number of left fill bits (I4) 
   char   right_fill[4];         // Number of right fill bits (I4) 
   char   pix_rng[8];            // Maximum data range of pixel (I8) 
   char   spare7[15804];         // Unused (A15804) 
};


struct acq_date_rec              // Acquisition date/time               
{
   ossim_int32  acq_year;              // Acquisition year (B4) 
   ossim_int32  acq_day;               // Acquisition day of year (B4) 
   ossim_int32  acq_msec;              // Acquisition msecs of day (B4) 
};

struct sdr_prefix_rec            // Signal data prefix data                    
{
   ossim_int32  line_num;              // Image data line number (B4) 
   ossim_int32  rec_num;               // Image data record index (B4) 
   ossim_int32  n_left_pixel;          // Left fill pixel count (B4) 
   ossim_int32  n_data_pixel;          // Data pixel count (B4) 
   ossim_int32  n_right_pixel;         // Right fill pixel count. (B4) 
   ossim_int32  sensor_updf;           // Sensor parameter update flag (B4) 
   struct acq_date_rec acq_date; // Acquisition date                           
   ossim_int16  sar_chan_ind;          // SAR channel indicator (B2) 
   ossim_int16  sar_chan_code;         // SAR channel code (B2) 
   ossim_int16  tran_polar;            // Transmitted polarization (B2) 
   ossim_int16  recv_polar;            // Received polarization (B2) 
   ossim_int32  prf;                   // Pulse repetition frequency, Hz (B4) 
   ossim_uint8   spare1[4];             // Unused (B4) 
   ossim_int16  obrc;                  // On-board range compressed flag (B2) 
   ossim_int16  pulse_type;            // Pulse type designator (B2) 
   ossim_int32  chp_len;               // Chirp length, ns (B4) 
   ossim_int32  chp_coef1;             // Chirp constant coefficient,Hz (B4) 
   ossim_int32  chp_coef2;             // Chirp linear coefficient,Hz/usec (B4) 
   ossim_int32  chp_coef3;             // Chirp quadratic coefficient,Hz/usec^2 (B4) 
   ossim_uint8   spare2[2][4];          // Spare (2B4) 
   ossim_int32  recv_gain;             // Receiver gain (B4) 
   ossim_int32  nt_line;               // Nought line flag (B4) 
   ossim_int32  ele_nadir;             // Elec. nadir angle, 10**-6 deg (B4) 
   ossim_int32  mec_nadir;             // Mech. nadir angle, 10**-6 deg (B4) 
   ossim_int32  ele_squint;            // Elec. squint angle, 10**-6 deg (B4) 
   ossim_int32  mec_squint;            // Mech. squint angle, 10**-6 deg (B4) 
   ossim_int32  sr_first;              // First sample slant range, m (B4) 
   ossim_int32  dr_window;             // Data record window time, ns (B4) 
   ossim_uint8   spare3[4];             // Spare (B4) 
   ossim_int32  plat_updf;             // Platform position update flag (B4) 
   ossim_int32  plat_lat;              // Platform latitude, 10**-6 deg (B4) 
   ossim_int32  plat_long;             // Platform longitude, 10**-6 deg (B4) 
   ossim_int32  plat_alt;              // Platform altitude, m (B4) 
   ossim_int32  plat_speed;            // Platform speed, cm/s (B4) 
   ossim_int32  plat_vel[3];           // Platform velocity, cm/s (3B4) 
   ossim_int32  plat_acc[3];           // Platform acceleration, cm/s (3B4) 
   ossim_int32  plat_track;            // Platform track, 10**-6 deg (B4) 
   ossim_int32  plat_head;             // Platform heading, 10**-6 deg (B4) 
   ossim_int32  plat_pitch;            // Platform pitch, 10**-6 deg (B4) 
   ossim_int32  plat_roll;             // Platform roll, 10**-6 deg (B4) 
   ossim_int32  plat_yaw;              // Platform yaw, 10**-6 deg (B4) 
};

struct sdr_data_rec              // Signal data record                  
{
   struct desc_rec desc;         // Record description                  
   struct sdr_prefix_rec prefix; // Prefix data                         
   ossim_uint8   sdr_data;              // SAR signal data (jBk) 
};


struct pdr_prefix_rec            // Processed data prefix data
{
   ossim_int32  line_num;              // Image data line number (B4) 
   ossim_int32  rec_num;               // Image data record index (B4) 
   ossim_int32  n_left_pixel;          // Left fill pixel count (B4) 
   ossim_int32  n_data_pixel;          // Data pixel count (B4) 
   ossim_int32  n_right_pixel;         // Right fill pixel count (B4) 
   ossim_int32  sensor_updf;           // Sensor parameter update flag (B4) 
   struct acq_date_rec acq_date; // Acquisition date
   ossim_int16  sar_chan_ind;          // SAR channel indicator (B2) 
   ossim_int16  sar_chan_code;         // SAR channel code (B2) 
   ossim_int16  tran_polar;            // Transmitted polarization (B2) 
   ossim_int16  recv_polar;            // Received polarization (B2) 
   ossim_int32  prf;                   // Pulse repetition frequency, Hz (B4) 
   ossim_uint8   spare1[4];             // Unused (B4) 
   ossim_int32  sr_first;              // Slant range to first pixel, m (B4) 
   ossim_int32  sr_mid;                // Slant range to mid-pixel, m (B4) 
   ossim_int32  sr_last;               // Slant range to last pixel, m (B4) 
   ossim_int32  fdc_first;             // First pixel Doppler centroid, Hz (B4) 
   ossim_int32  fdc_mid;               // Mid-pixel Doppler centroid, Hz (B4) 
   ossim_int32  fdc_last;              // Last pixel Doppler centroid, Hz (B4) 
   ossim_int32  ka_first;              // First pixel azimuth FM rate, Hz (B4) 
   ossim_int32  ka_mid;                // Mid-pixel azimuth FM rate, Hz (B4) 
   ossim_int32  ka_last;               // Last pixel azimuth  FM rate, Hz (B4) 
   ossim_int32  nadir_ang;             // Nadir look angle, 10**-6 deg (B4) 
   ossim_int32  squint_ang;            // Azimuth squint angle, 10**-6 deg (B4) 
   ossim_int32  null_f;                // Null line flag - 1 for NULL line, 0 for otherwise (B4) 
   ossim_uint8   spare2[4][4];          // Unused (4B4) 
   ossim_int32  geo_updf;              // Geographic parameter update flag (B4) 
   ossim_int32  lat_first;             // First pixel latitude, millionths of deg (B4)
   ossim_int32  lat_mid;               // Mid-pixel latitude (B4) 
   ossim_int32  lat_last;              // Last pixel latitude (B4) 
   ossim_int32  long_first;            // First pixel longitude, millionths of deg (B4) 
   ossim_int32  long_mid;              // Mid pixel longitue (B4) 
   ossim_int32  long_last;             // Last pixel longitude (B4) 
   ossim_int32  north_first;           // Northing of first pixel, m (B4) 
   ossim_uint8   spare3[4];             // Unused (B4) 
   ossim_int32  north_last;            // Northing of last pixel, m (B4) 
   ossim_int32  east_first;            // Easting of first pixel, m (B4) 
   ossim_uint8   spare4[4];             // Spare (B4) 
   ossim_int32  east_last;             // Easting of last pixel, m (B4) 
   ossim_int32  heading;               // Line heading, millionths of deg (B4) 
   ossim_uint8   spare5[8];             // Unused (B8) 
};

struct pdr_data_rec              // Processed data record               
{
   struct desc_rec       desc;   // Record description                  
   struct pdr_prefix_rec prefix; // Prefix data                         
};


struct pdr8_htab_rec             // Histogram table record 
{
   char   hist_desc[32];         // Histogram descriptor (A32) 
   char   nrec[4];               // Records per table (I4) 
   char   tab_seq[4];            // Table sequence number (I4) 
   char   nbin[8];               // Total number of table bins (I8) 
   char   ns_lin[8];             // Total number of pixels in line direction (I8) 
   char   ns_pix[8];             // Total number of lines (I8) 
   char   ngrp_lin[8];           // Group size along pixel axis (I8) 
   char   ngrp_pix[8];           // Group size along line axis (I8) 
   char   nsamp_lin[8];          // Number of samples used per group along pixel axis (I8) 
   char   nsamp_pix[8];          // Number of samples used per group along line (I8)
   char   min_smp[16];           // Minimum first bin (E16.7) 
   char   max_smp[16];           // Maximum last bin (E16.7) 
   char   mean_smp[16];          // Mean sample value (E16.7) 
   char   std_smp[16];           // Sample standard deviation (E16.7) 
   char   smp_inc[16];           // Sample value increment (E16.7) 
   char   min_hist[16];          // Minimum histogram value (E16.7) 
   char   max_hist[16];          // Maximum histogram value (E16.7) 
   char   mean_hist[16];         // Histogram mean value (E16.7) 
   char   std_hist[16];          // Histogram standard deviation (E16.7) 
   char   nhist[8];              // Histogram table size (I8) 
   char   hist[256][8];          // 256 histogram table values for 256 bins (I8)
};

struct pdr8_hist_rec             // Data histogram record
{
   struct desc_rec desc;         // Record description
   char   rec_seq[4];            // Record sequence number (I4) 
   char   sar_chn[4];            // SAR channel number (I4) 
   char   ntab[8];               // Num hist. table data sets in record (I8) 
   char   ltab[8];               // Histogram table data set size (I8) 
   struct pdr8_htab_rec htab;    // Histogram tables
   char   spare[14588];          // Unused (A14588) 
};


struct null_vol_rec              // Null volume descriptor record       
{
   struct desc_rec desc;         // Record description                  
   char   ascii_flag[2];         // ASCII flag (A2) 
   char   spare1[2];             // Unused (A2) 
   char   format_doc[12];        // Format control doc (A12) 
   char   format_ver[2];         // Format doc version (A2) 
   char   format_rev[2];         // Format doc revision (A2) 
   char   software_id[12];       // Software identifier (A12) 
   char   tape_id[16];           // Physical tape id (A16) 
   char   logvol_id[16];         // Logical volume id (A16) 
   char   phyvol_id[16];         // Physical volume id (A16) 
   char   n_phyvol[2];           // Total physical volume count (I2) 
   char   first_phyvol[2];       // First physical volume (I2) 
   char   last_phyvol[2];        // Last physical volume (I2) 
   char   curr_phyvol[2];        // Current physical volume (I2) 
   char   first_file[4];         // First file in volume (I4) 
   char   volset_log[4];         // Logical volume within set (I4) 
   char   logvol_vol[4];         // Logical volume within phyvol (I4) 
   char   spare2[248];           // Unused (A248) 
};

struct VolumeDirectoryFile
{
   vol_desc_rec      volDescRec;
   file_pntr_rec     leaderFilePntrRec;
   file_pntr_rec     imageOptionsFilePntrRec;
   file_pntr_rec     trailerFilePntrRec;
   text_rec          textRec;
};

struct SarLeaderFile
{
   sar_desc_rec      sarDescRec;
   dataset_sum_rec   dataSetSumRec;
   qual_sum_rec      qualSumRec;
   sdr_hist_rec      sdrHistRec;
   pdr16_hist_rec    pdr16HistRec;
   proc_parm_rec     procParmRec;
   map_proj_rec      mapProjRec;
   pos_data_rec      posDataRec;
   att_data_rec      attDataRec;
   radi_data_rec     radiDataRec;
   radi_comp_rec     radiCompRec;
};

struct SarTrailerFile
{
   sar_desc_rec      sarDescRec;
   dataset_sum_rec   dataSetSumRec;
   qual_sum_rec      qualSumRec;
   sdr_hist_rec      sdrHistRec;
   pdr8_hist_rec     pdr8HistRec;
   proc_parm_rec     procParmRec;
   att_data_rec      attDataRec;
   radi_data_rec     radiDataRec;
   radi_comp_rec     radiCompRec;
};

class ossimCeosData : public ossimReferenced
{
public:

   enum ErrorStatus
   {
      OK = 0,
      ERROR
   };
   
   ossimCeosData(const ossimFilename& volDirFile,
            const ossimFilename& leaderFile,
            const ossimFilename& imageFile,
            const ossimFilename& trailerFile);

   ossimCeosData(const ossimFilename& imageDir);

   bool errorStatus()   const { return theErrorStatus; }

   const ossimFilename& volDirFile()    const { return theVolDirFileName; }
   const ossimFilename& leaderFile()    const { return theLeaderFileName; }
   const ossimFilename& imageFile()     const { return theImageFileName; }
   const ossimFilename& trailerFile()   const { return theTrailerFileName; }

   const vol_desc_rec*    volDescRec()    const { return theVolDescRec; }
   const text_rec*        textRec()       const { return theTextRec; }
   const imop_desc_rec*   imopDescRec()   const { return &theImopDescRec; }
   const dataset_sum_rec* dataSetSumRec() const { return theDataSetSumRec; }
   const qual_sum_rec*    qualSumRec()    const { return theQualSumRec; }
   const sdr_hist_rec*    sdrHistRec()    const { return theSdrHistRec; }
   const pdr16_hist_rec*  pdr16HistRec()  const { return thePdr16HistRec; }
   const pdr8_hist_rec*   pdr8HistRec()   const { return thePdr8HistRec; }
   const proc_parm_rec*   procParmRec()   const { return theProcParmRec; }
   const map_proj_rec*    mapProjRec()    const { return theMapProjRec; }
   const pos_data_rec*    posDataRec()    const { return thePosDataRec; }
   const att_data_rec*    attDataRec()    const { return theAttDataRec; }
   const radi_data_rec*   radiDataRec()   const { return theRadiDataRec; }
   const radi_comp_rec*   radiCompRec()   const { return theRadiCompRec; }
   
   void                   dump(std::ostream& strm = std::clog) const;
   void                   dumpVolDescRec   (std::ostream&) const;
   void                   dumpTextRec      (std::ostream&) const;
   void                   dumpImopDescRec  (std::ostream&) const;
   void                   dumpDataSetSumRec(std::ostream&) const;
   void                   dumpQualSumRec   (std::ostream&) const;
   void                   dumpSdrHistRec   (std::ostream&) const;
   void                   dumpPdr16HistRec (std::ostream&) const;
   void                   dumpPdr8HistRec  (std::ostream&) const;
   void                   dumpProcParmRec  (std::ostream&) const;
   void                   dumpMapProjRec   (std::ostream&) const;
   void                   dumpPosDataRec   (std::ostream&) const;
   void                   dumpAttDataRec   (std::ostream&) const;
   void                   dumpRadiDataRec  (std::ostream&) const;
   void                   dumpRadiCompRec  (std::ostream&) const;

private:
   void                   loadCeosRecords();
   ossim_int32             sizeOfNextRec(FILE*);
   
   VolumeDirectoryFile    theVolumeDirectory;
   SarLeaderFile          theSarLeaderFile;
   imop_desc_rec          theImopDescRec;
   SarTrailerFile         theSarTrailerFile;

   ossimFilename          theVolDirFileName;
   ossimFilename          theLeaderFileName;
   ossimFilename          theImageFileName;
   ossimFilename          theTrailerFileName;
   
   bool                   theErrorStatus;

   vol_desc_rec*          theVolDescRec;
   text_rec*              theTextRec;
   dataset_sum_rec*       theDataSetSumRec;
   qual_sum_rec*          theQualSumRec;
   sdr_hist_rec*          theSdrHistRec;
   pdr16_hist_rec*        thePdr16HistRec;
   pdr8_hist_rec*         thePdr8HistRec;
   proc_parm_rec*         theProcParmRec;
   map_proj_rec*          theMapProjRec;
   pos_data_rec*          thePosDataRec;
   att_data_rec*          theAttDataRec;
   radi_data_rec*         theRadiDataRec;
   radi_comp_rec*         theRadiCompRec;
};



#endif
