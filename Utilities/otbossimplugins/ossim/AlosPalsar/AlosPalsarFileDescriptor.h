//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
// "Copyright Centre for Remote Imaging, Sensing and Processing"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef AlosPalsarFileDescriptor_h
#define AlosPalsarFileDescriptor_h

// TODO Rename to AlosPalsarLeaderFileDescriptor

#include<iostream>
#include <cstdlib>
#include <AlosPalsar/AlosPalsarRecordHeader.h>
#include <AlosPalsar/AlosPalsarRecord.h>

namespace ossimplugins
{

/**
 * @ingroup SARLeaderAlosPalsarFileDescriptorRecord
 * @brief This class is able to read the SAR leader file descriptor record of the leader file
 */
class AlosPalsarFileDescriptor : public AlosPalsarRecord
{
public:
  /**
   * @brief Constructor
   */
  AlosPalsarFileDescriptor();
  /**
   * @brief Destructor
   */
  virtual ~AlosPalsarFileDescriptor();

  /**
   * @brief This function write the AlosPalsarFileDescriptor in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const AlosPalsarFileDescriptor& data);

  /**
   * @brief This function read a AlosPalsarFileDescriptor from a stream
   */
  friend std::istream& operator>>(std::istream& is, AlosPalsarFileDescriptor& data);

  /**
   * @brief Copy constructor
   */
  AlosPalsarFileDescriptor(const AlosPalsarFileDescriptor& rhs);

  /**
   * @brief Copy operator
   */
  AlosPalsarFileDescriptor& operator=(const AlosPalsarFileDescriptor& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  AlosPalsarRecord* Instanciate()
  {
    return new AlosPalsarFileDescriptor();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  AlosPalsarRecord* Clone()
  {
    return new AlosPalsarFileDescriptor(*this);
  };

  /**
   * @brief Read the class data from a stream
   */
  void Read(std::istream& is)
  {
    is >> *this;
  };

  /**
   * @brief Write the class to a stream
   */
  void Write(std::ostream& os)
  {
    os << *this;
  };

  /**
   * @brief ASCII flag
   */
  std::string get_ascii_flag() const
  {
    return _ascii_flag;
  };

  /**
   * @brief Continuation flag
   */
  std::string get_continuation_flag() const
  {
    return _continuation_flag;
  };


  /**
   * @brief Format control documentation
   */
  std::string get_format_doc() const
  {
    return _format_doc;
  };
  /**
   * @brief Format doc version
   */
  std::string   get_format_ver() const
  {
    return _format_ver;
  };
  /**
   * @brief Format doc revision
   */
  std::string   get_design_rev() const
  {
    return _design_rev;
  };
  /**
   * @brief Software identifier
   */
  std::string   get_software_id() const
  {
    return _software_id;
  };
  /**
   * @brief File number
   */
  int   get_file_num() const
  {
    return _file_num ;
  };
  /**
   * @brief File name
   */
  std::string   get_file_name() const
  {
    return _file_name;
  };
  /**
   * @brief Record sequence/location flag
   */
  std::string   get_rec_seq() const
  {
    return _rec_seq;
  };
  /**
   * @brief Sequence number location
   */
  int   get_seq_loc() const
  {
    return _seq_loc;
  };
  /**
   * @brief Sequence number lenght
   */
  int   get_seq_len() const
  {
    return _seq_len;
  };
  /**
   * @brief Record code/location flag
   */
  std::string   get_rec_code() const
  {
    return _rec_code;
  };
  /**
   * @brief Record code location
   */
  int   get_code_loc() const
  {
    return _code_loc;
  };
  /**
   * @brief Record code length
   */
  int   get_code_len() const
  {
    return _code_len;
  };
  /**
   * @brief Record length/location flag
   */
  std::string get_rec_len() const
  {
    return _rec_len;
  };
  /**
   * @brief Record lenght location
   */
  int get_rlen_loc() const
  {
    return _rlen_loc;
  };
  /**
   * @brief Record length, bytes
   */
  int get_rlen_len() const
  {
    return _rlen_len;
  };
  /**
   * @brief Number of dataset summ records
   */
  int get_n_dataset() const
  {
    return _n_dataset;
  };
  /**
   * @brief Data set summary record length, bytes
   */
  int get_l_dataset() const
  {
    return _l_dataset;
  };
  /**
   * @brief Number of map proj records
   */
  int get_n_map_proj() const
  {
    return _n_map_proj;
  };
  /**
   * @brief Map projection record length, bytes
   */
  int get_l_map_proj() const
  {
    return _l_map_proj;
  };
  /**
   * @brief Number of platform position records
   */
  int get_n_plat_pos() const
  {
    return _n_plat_pos;
  };
  /**
   * @brief Platform position record length, bytes
   */
  int get_l_plat_pos() const
  {
    return _l_plat_pos;
  };
  /**
   * @brief Number of attitude data records
   */
  int get_n_att_data() const
  {
    return _n_att_data;
  };
  /**
   * @brief Attitude data record length, bytes
   */
  int get_l_att_data() const
  {
    return _l_att_data;
  };
  /**
   * @brief Number of radiometric data records
   */
  int get_n_radi_data() const
  {
    return _n_radi_data;
  };
  /**
   * @brief Radiometric data record length, bytes
   */
  int get_l_radi_data() const
  {
    return _l_radi_data;
  };
  /**
   * @brief Number of radiometric compensation records
   */
  int get_n_radi_comp() const
  {
    return _n_radi_comp;
  };
  /**
   *  @brief Radiometric compensation record length, bytes
   */
  int get_l_radi_comp() const
  {
    return _l_radi_comp;
  };
  /**
   * @brief Number of data quality summary records
   */
  int get_n_qual_sum() const
  {
    return _n_qual_sum;
  };
  /**
   * @brief Data quality summary record length, bytes
   */
  int get_l_qual_sum() const
  {
    return _l_qual_sum;
  };
  /**
   * @brief Number of data histogram records
   */
  int get_n_data_his() const
  {
    return _n_data_his;
  };
  /**
   * @brief Data histogram record length, bytes
   */
  int get_l_data_his() const
  {
    return _l_data_his;
  };
  /**
   * @brief Number of range spectra records
   */
  int get_n_rang_spec() const
  {
    return _n_rang_spec ;
  };
  /**
   * @brief Range spectra record length, bytes
   */
  int get_l_rang_spec() const
  {
    return _l_rang_spec;
  };
  /**
   * @brief Number of DEM descriptor records
   */
  int get_n_dem_desc() const
  {
    return _n_dem_desc;
  };
  /**
   * @brief DEM desc record length, bytes
   */
  int get_l_dem_desc() const
  {
    return _l_dem_desc;
  };
  /**
   * @brief Number of RADAR par records
   */
  int get_n_radar_par() const
  {
    return _n_radar_par;
  };
  /**
   * @brief RADAR par record length, bytes
   */
  int get_l_radar_par() const
  {
    return _l_radar_par;
  };
  /**
   * @brief Number of annotation data records
   */
  int get_n_anno_data() const
  {
    return _n_anno_data;
  };
  /**
   * @brief Annotation data record length, bytes
   */
  int get_l_anno_data() const
  {
    return _l_anno_data;
  };
  /**
   * @brief Number of processing parameter records
   */
  int get_n_det_proc() const
  {
    return _n_det_proc;
  };
  /**
   * @brief Processing parameter record length, bytes
   */
  int get_l_det_proc() const
  {
    return _l_det_proc;
  };
  /**
   * @brief Number of calibration records
   */
  int get_n_cal() const
  {
    return _n_cal;
  };
  /**
   * @brief Calibration record length, bytes
   */
  int get_l_cal() const
  {
    return _l_cal;
  };
  /**
   * @brief Number of GCP records
   */
  int get_n_gcp() const
  {
    return _n_gcp;
  };
  /**
   * @brief GCP record length, bytes
   */
  int get_l_gcp() const
  {
    return _l_gcp;
  };
  /**
   * @brief Number of facility data records
   */
  int get_n_fac_data() const
  {
    return _n_fac_data;
  };
  /**
   * @brief Fac data record length, bytes
   */
  int get_l_fac_data() const
  {
    return _l_fac_data;
  };

  /**
   * @brief Number of facility data records
   */
  int get_n_fac_data2()
  {
    return _n_fac_data;
  };
  /**
   * @brief Fac data record length, bytes
   */
  int get_l_fac_data2()
  {
    return _l_fac_data;
  };
  /**
   * @brief Number of facility data records
   */
  int get_n_fac_data3()
  {
    return _n_fac_data;
  };
  /**
   * @brief Fac data record length, bytes
   */
  int get_l_fac_data3()
  {
    return _l_fac_data;
  };
  /**
   * @brief Number of facility data records
   */
  int get_n_fac_data4()
  {
    return _n_fac_data;
  };
  /**
   * @brief Fac data record length, bytes
   */
  int get_l_fac_data4()
  {
    return _l_fac_data;
  };
  /**
   * @brief Number of facility data records
   */
  int get_n_fac_data5()
  {
    return _n_fac_data;
  };
  /**
   * @brief Fac data record length, bytes
   */
  int get_l_fac_data5()
  {
    return _l_fac_data;
  };
  /**
   * @brief Number of facility data records
   */
  int get_n_fac_data6()
  {
    return _n_fac_data;
  };
  /**
   * @brief Fac data record length, bytes
   */
  int get_l_fac_data6()
  {
    return _l_fac_data;
  };
  /**
   * @brief Number of facility data records
   */
  int get_n_fac_data7()
  {
    return _n_fac_data;
  };
  /**
   * @brief Fac data record length, bytes
   */
  int get_l_fac_data7()
  {
    return _l_fac_data;
  };
  /**
   * @brief Number of facility data records
   */
  int get_n_fac_data8()
  {
    return _n_fac_data;
  };
  /**
   * @brief Fac data record length, bytes
   */
  int get_l_fac_data8()
  {
    return _l_fac_data;
  };
  /**
   * @brief Number of facility data records
   */
  int get_n_fac_data9()
  {
    return _n_fac_data;
  };
  /**
   * @brief Fac data record length, bytes
   */
  int get_l_fac_data9()
  {
    return _l_fac_data;
  };
  /**
   * @brief Number of facility data records
   */
  int get_n_fac_data10()
  {
    return _n_fac_data;
  };
  /**
   * @brief Fac data record length, bytes
   */
  int get_l_fac_data10()
  {
    return _l_fac_data;
  };
  /**
   * @brief Number of facility data records
   */
  int get_n_fac_data11()
  {
    return _n_fac_data;
  };
  /**
   * @brief Fac data record length, bytes
   */
  int get_l_fac_data11()
  {
    return _l_fac_data;
  };

protected:
  /**
   * @brief ASCII flag
   */
  std::string _ascii_flag;

  /**
   * @brief Continuation flag
   */
  std::string _continuation_flag;

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

  /**
   * @brief Number of facility data records
   */
  int _n_fac_data2;
  /**
   * @brief Fac data record length, bytes
   */
  int _l_fac_data2;

  /**
   * @brief Number of facility data records
   */
  int _n_fac_data3;
  /**
   * @brief Fac data record length, bytes
   */
  int _l_fac_data3;

  /**
   * @brief Number of facility data records
   */
  int _n_fac_data4;
  /**
   * @brief Fac data record length, bytes
   */
  int _l_fac_data4;

  /**
   * @brief Number of facility data records
   */
  int _n_fac_data5;
  /**
   * @brief Fac data record length, bytes
   */
  int _l_fac_data5;

  /**
   * @brief Number of facility data records
   */
  int _n_fac_data6;
  /**
   * @brief Fac data record length, bytes
   */
  int _l_fac_data6;

  /**
   * @brief Number of facility data records
   */
  int _n_fac_data7;
  /**
   * @brief Fac data record length, bytes
   */
  int _l_fac_data7;

  /**
   * @brief Number of facility data records
   */
  int _n_fac_data8;
  /**
   * @brief Fac data record length, bytes
   */
  int _l_fac_data8;

  /**
   * @brief Number of facility data records
   */
  int _n_fac_data9;
  /**
   * @brief Fac data record length, bytes
   */
  int _l_fac_data9;

  /**
   * @brief Number of facility data records
   */
  int _n_fac_data10;
  /**
   * @brief Fac data record length, bytes
   */
  int _l_fac_data10;

  /**
   * @brief Number of facility data records
   */
  int _n_fac_data11;
  /**
   * @brief Fac data record length, bytes
   */
  int _l_fac_data11;

private:
};
}
#endif
