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

#ifndef FileDescriptor_h
#define FileDescriptor_h


#include<iostream>
#include <RadarSat/RadarSatRecordHeader.h>
#include <RadarSat/RadarSatRecord.h>

namespace ossimplugins
{
/**
 * @ingroup SARLeaderFileDescriptorRecord
 * @brief This class is able to read the SAR leader file descriptor record of the leader file
 */
class FileDescriptor : public RadarSatRecord
{
public:
  /**
   * @brief Constructor
   */
  FileDescriptor();
  /**
   * @brief Destructor
   */
  virtual ~FileDescriptor();

  /**
   * @brief This function writes the FileDescriptor in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const FileDescriptor& data);

  /**
   * @brief This function reads a FileDescriptor from a stream
   */
  friend std::istream& operator>>(std::istream& is, FileDescriptor& data);

  /**
   * @brief Copy constructor
   */
  FileDescriptor(const FileDescriptor& rhs);

  /**
   * @brief Copy operator
   */
  FileDescriptor& operator=(const FileDescriptor& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  RadarSatRecord* Instanciate()
  {
    return new FileDescriptor();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  RadarSatRecord* Clone()
  {
    return new FileDescriptor(*this);
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
   * @brief ASCII flag
   */
  std::string get_ascii_flag()
  {
    return _ascii_flag;
  };

  /**
   * @brief Format control documentation
   */
  std::string get_format_doc()
  {
    return _format_doc;
  };
  /**
   * @brief Format doc version
   */
  std::string   get_format_ver()
  {
    return _format_ver;
  };
  /**
   * @brief Format doc revision
   */
    std::string   get_design_rev()
  {
    return _design_rev;
  };
  /**
   * @brief Software identifier
   */
    std::string   get_software_id()
  {
    return _software_id;
  };
  /**
   * @brief File number
   */
  int   get_file_num()
  {
    return _file_num ;
  };
  /**
   * @brief File name
   */
    std::string   get_file_name()
  {
    return _file_name;
  };
  /**
   * @brief Record sequence/location flag
   */
    std::string   get_rec_seq()
  {
    return _rec_seq;
  };
  /**
   * @brief Sequence number location
   */
  int   get_seq_loc()
  {
    return _seq_loc;
  };
  /**
   * @brief Sequence number lenght
   */
  int   get_seq_len()
  {
    return _seq_len;
  };
  /**
   * @brief Record code/location flag
   */
  std::string   get_rec_code()
  {
    return _rec_code;
  };
  /**
   * @brief Record code location
   */
  int   get_code_loc()
  {
    return _code_loc;
  };
  /**
   * @brief Record code length
   */
    int   get_code_len()
  {
    return _code_len;
  };
  /**
   * @brief Record length/location flag
   */
  std::string get_rec_len()
  {
    return _rec_len;
  };
  /**
   * @brief Record lenght location
   */
  int get_rlen_loc()
  {
    return _rlen_loc;
  };
  /**
   * @brief Record length, bytes
   */
  int get_rlen_len()
  {
    return _rlen_len;
  };
  /**
   * @brief Number of dataset summ records
   */
  int get_n_dataset()
  {
    return _n_dataset;
  };
  /**
   * @brief Data set summary record length, bytes
   */
  int get_l_dataset()
  {
    return _l_dataset;
  };
  /**
   * @brief Number of map proj records
   */
  int get_n_map_proj()
  {
    return _n_map_proj;
  };
  /**
   * @brief Map projection record length, bytes
   */
  int get_l_map_proj()
  {
    return _l_map_proj;
  };
  /**
   * @brief Number of platform position records
   */
  int get_n_plat_pos()
  {
    return _n_plat_pos;
  };
  /**
   * @brief Platform position record length, bytes
   */
  int get_l_plat_pos()
  {
    return _l_plat_pos;
  };
  /**
   * @brief Number of attitude data records
   */
  int get_n_att_data()
  {
    return _n_att_data;
  };
  /**
   * @brief Attitude data record length, bytes
   */
  int get_l_att_data()
  {
    return _l_att_data;
  };
  /**
   * @brief Number of radiometric data records
   */
  int get_n_radi_data()
  {
    return _n_radi_data;
  };
  /**
   * @brief Radiometric data record length, bytes
   */
  int get_l_radi_data()
  {
    return _l_radi_data;
  };
  /**
   * @brief Number of radiometric compensation records
   */
  int get_n_radi_comp()
  {
    return _n_radi_comp;
  };
  /**
   *  @brief Radiometric compensation record length, bytes
   */
  int get_l_radi_comp()
  {
    return _l_radi_comp;
  };
  /**
   * @brief Number of data quality summary records
   */
  int get_n_qual_sum()
  {
    return _n_qual_sum;
  };
  /**
   * @brief Data quality summary record length, bytes
   */
  int get_l_qual_sum()
  {
    return _l_qual_sum;
  };
  /**
   * @brief Number of data histogram records
   */
  int get_n_data_his()
  {
    return _n_data_his;
  };
  /**
   * @brief Data histogram record length, bytes
   */
  int get_l_data_his()
  {
    return _l_data_his;
  };
  /**
   * @brief Number of range spectra records
   */
  int get_n_rang_spec()
  {
    return _n_rang_spec ;
  };
  /**
   * @brief Range spectra record length, bytes
   */
  int get_l_rang_spec()
  {
    return _l_rang_spec;
  };
  /**
   * @brief Number of DEM descriptor records
   */
  int get_n_dem_desc()
  {
    return _n_dem_desc;
  };
  /**
   * @brief DEM desc record length, bytes
   */
  int get_l_dem_desc()
  {
    return _l_dem_desc;
  };
  /**
   * @brief Number of RADAR par records
   */
  int get_n_radar_par()
  {
    return _n_radar_par;
  };
  /**
   * @brief RADAR par record length, bytes
   */
  int get_l_radar_par()
  {
    return _l_radar_par;
  };
  /**
   * @brief Number of annotation data records
   */
  int get_n_anno_data()
  {
    return _n_anno_data;
  };
  /**
   * @brief Annotation data record length, bytes
   */
  int get_l_anno_data()
  {
    return _l_anno_data;
  };
  /**
   * @brief Number of processing parameter records
   */
  int get_n_det_proc()
  {
    return _n_det_proc;
  };
  /**
   * @brief Processing parameter record length, bytes
   */
  int get_l_det_proc()
  {
    return _l_det_proc;
  };
  /**
   * @brief Number of calibration records
   */
  int get_n_cal()
  {
    return _n_cal;
  };
  /**
   * @brief Calibration record length, bytes
   */
  int get_l_cal()
  {
    return _l_cal;
  };
  /**
   * @brief Number of GCP records
   */
  int get_n_gcp()
  {
    return _n_gcp;
  };
  /**
   * @brief GCP record length, bytes
   */
  int get_l_gcp()
  {
    return _l_gcp;
  };
  /**
   * @brief Number of facility data records
   */
  int get_n_fac_data()
  {
    return _n_fac_data;
  };
  /**
   * @brief Fac data record length, bytes
   */
  int get_l_fac_data()
  {
    return _l_fac_data;
  };

protected:
  /**
   * @brief ASCII flag
   */
  std::string _ascii_flag;

  /**
   * @brief Format control documentation
   */
  std::string _format_doc;
  /**
   * @brief Format doc version
   */
  std::string   _format_ver;
  /**
   * @brief Format doc revision
   */
    std::string   _design_rev;
  /**
   * @brief Software identifier
   */
    std::string   _software_id;
  /**
   * @brief File number
   */
  int   _file_num;
  /**
   * @brief File name
   */
    std::string   _file_name;
  /**
   * @brief Record sequence/location flag
   */
    std::string   _rec_seq;
  /**
   * @brief Sequence number location
   */
  int   _seq_loc;
  /**
   * @brief Sequence number lenght
   */
  int   _seq_len;
  /**
   * @brief Record code/location flag
   */
  std::string   _rec_code;
  /**
   * @brief Record code location
   */
  int   _code_loc;
  /**
   * @brief Record code length
   */
    int   _code_len;
  /**
   * @brief Record length/location flag
   */
  std::string _rec_len;
  /**
   * @brief Record lenght location
   */
  int _rlen_loc;
  /**
   * @brief Record length, bytes
   */
  int _rlen_len;


  /**
   * @brief Number of dataset summ records
   */
  int _n_dataset;
  /**
   * @brief Data set summary record length, bytes
   */
  int _l_dataset;
  /**
   * @brief Number of map proj records
   */
  int _n_map_proj;
  /**
   * @brief Map projection record length, bytes
   */
  int _l_map_proj;
  /**
   * @brief Number of platform position records
   */
  int _n_plat_pos;
  /**
   * @brief Platform position record length, bytes
   */
  int _l_plat_pos;
  /**
   * @brief Number of attitude data records
   */
  int _n_att_data;
  /**
   * @brief Attitude data record length, bytes
   */
  int _l_att_data;
  /**
   * @brief Number of radiometric data records
   */
  int _n_radi_data;
  /**
   * @brief Radiometric data record length, bytes
   */
  int _l_radi_data;
  /**
   * @brief Number of radiometric compensation records
   */
  int _n_radi_comp;
  /**
   *  @brief Radiometric compensation record length, bytes
   */
  int _l_radi_comp;
  /**
   * @brief Number of data quality summary records
   */
  int _n_qual_sum;
  /**
   * @brief Data quality summary record length, bytes
   */
  int _l_qual_sum;
  /**
   * @brief Number of data histogram records
   */
  int _n_data_his;
  /**
   * @brief Data histogram record length, bytes
   */
  int _l_data_his;
  /**
   * @brief Number of range spectra records
   */
  int _n_rang_spec;
  /**
   * @brief Range spectra record length, bytes
   */
  int _l_rang_spec;
  /**
   * @brief Number of DEM descriptor records
   */
  int _n_dem_desc;
  /**
   * @brief DEM desc record length, bytes
   */
  int _l_dem_desc;
  /**
   * @brief Number of RADAR par records
   */
  int _n_radar_par;
  /**
   * @brief RADAR par record length, bytes
   */
  int _l_radar_par;
  /**
   * @brief Number of annotation data records
   */
  int _n_anno_data;
  /**
   * @brief Annotation data record length, bytes
   */
  int _l_anno_data;
  /**
   * @brief Number of processing parameter records
   */
  int _n_det_proc;
  /**
   * @brief Processing parameter record length, bytes
   */
  int _l_det_proc;
  /**
   * @brief Number of calibration records
   */
  int _n_cal;
  /**
   * @brief Calibration record length, bytes
   */
  int _l_cal;
  /**
   * @brief Number of GCP records
   */
  int _n_gcp;
  /**
   * @brief GCP record length, bytes
   */
  int _l_gcp;

  /**
   * @brief Number of facility data records
   */
  int _n_fac_data;
  /**
   * @brief Fac data record length, bytes
   */
  int _l_fac_data;

private:
};
}
#endif
