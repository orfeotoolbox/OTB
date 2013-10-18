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

#ifndef VolumeDescriptorRecord_h
#define VolumeDescriptorRecord_h


#include<iostream>
#include <RadarSat/RadarSatRecordHeader.h>
#include <RadarSat/RadarSatRecord.h>

namespace ossimplugins
{
/**
 * @ingroup SARLeaderVolumeDescriptorRecordRecord
 * @brief This class is able to read the SAR leader file descriptor record of the leader file
 */
class VolumeDescriptorRecord : public RadarSatRecord
{
public:
  /**
   * @brief Constructor
   */
  VolumeDescriptorRecord();
  /**
   * @brief Destructor
   */
  virtual ~VolumeDescriptorRecord();

  /**
   * @brief This function writes the VolumeDescriptorRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const VolumeDescriptorRecord& data);

  /**
   * @brief This function reads a VolumeDescriptorRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, VolumeDescriptorRecord& data);

  /**
   * @brief Copy constructor
   */
  VolumeDescriptorRecord(const VolumeDescriptorRecord& rhs);

  /**
   * @brief Copy operator
   */
  VolumeDescriptorRecord& operator=(const VolumeDescriptorRecord& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  RadarSatRecord* Instanciate()
  {
    return new VolumeDescriptorRecord();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  RadarSatRecord* Clone()
  {
    return new VolumeDescriptorRecord(*this);
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
  std::string get_format_ver()
  {
    return _format_ver;
  };

  /**
   * @brief Format doc revision
   */
  std::string getget_format_rev()
  {
    return _format_rev;
  };

  /**
   * @brief Software identifier
   */
  std::string get_software_id()
  {
    return _software_id;
  };

  /**
   * @brief Physical volume identifier
   */
  std::string get_phyvol_id()
  {
    return _phyvol_id;
  };

  /**
   * @brief Logical volume identifier
   */
  std::string get_logvol_id()
  {
    return _logvol_id;
  };

  /**
   * @brief Volume set identifier
   */
  std::string get_volset_id()
  {
    return _volset_id;
  };

  /**
   * @brief Total physical volume count
   */
  int get_phyvol_cnt()
  {
    return _phyvol_cnt;
  };

  /**
   * @brief Physical volume of first tape
   */
  int get_first_phyvol()
  {
    return _first_phyvol;
  };

  /**
   * @brief Physical volume of last tape
   */
  int get_last_phyvol()
  {
    return _last_phyvol;
  };

  /**
   * @brief Physical volume of current tape
   */
  int get_curr_phyvol()
  {
    return _curr_phyvol;
  };

  /**
   * @brief First file number in physical volume
   */
  int get_first_file()
  {
    return _first_file;
  };

  /**
   * @brief Logical volume within set
   */
  int get_volset_log()
  {
    return _volset_log;
  };

  /**
   * @brief Logical volume within phyvol
   */
  int get_phyvol_log()
  {
    return _phyvol_log;
  };

  /**
   * @brief Logvol creation date
   */
  std::string get_logvol_date()
  {
    return _logvol_date;
  };

  /**
   * @brief Logvol creation time
   */
  std::string get_logvol_time()
  {
    return _logvol_time;
  };

  /**
   * @brief Logvol generation country
   */
  std::string get_logvol_country()
  {
    return _logvol_country;
  };

  /**
   * @brief Logvol generation agency
   */
  std::string get_logvol_agency()
  {
    return _logvol_agency;
  };

  /**
   * @brief Logvol generation facility
   */
  std::string get_logvol_facility()
  {
    return _logvol_facility;
  };

  /**
   * @brief Number of file pointer records
   */
  int get_n_filepoint()
  {
    return _n_filepoint;
  };

  /**
   * @brief Number of records in volume directory file
   */
  int get_n_voldir()
  {
    return _n_voldir;
  };

  /**
   * @brief Product identifier
   */
  std::string get_product_id()
  {
    return _product_id;
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
  std::string _format_ver;

  /**
   * @brief Format doc revision
   */
  std::string _format_rev;

  /**
   * @brief Software identifier
   */
  std::string _software_id;

  /**
   * @brief Physical volume identifier
   */
  std::string _phyvol_id;

  /**
   * @brief Logical volume identifier
   */
  std::string _logvol_id;

  /**
   * @brief Volume set identifier
   */
  std::string _volset_id;

  /**
   * @brief Total physical volume count
   */
  int _phyvol_cnt;

  /**
   * @brief Physical volume of first tape
   */
  int _first_phyvol;

  /**
   * @brief Physical volume of last tape
   */
  int _last_phyvol;

  /**
   * @brief Physical volume of current tape
   */
  int _curr_phyvol;

  /**
   * @brief First file number in physical volume
   */
  int _first_file;

  /**
   * @brief Logical volume within set
   */
  int _volset_log;

  /**
   * @brief Logical volume within phyvol
   */
  int _phyvol_log;

  /**
   * @brief Logvol creation date
   */
  std::string _logvol_date;

  /**
   * @brief Logvol creation time
   */
  std::string _logvol_time;

  /**
   * @brief Logvol generation country
   */
  std::string _logvol_country;

  /**
   * @brief Logvol generation agency
   */
  std::string _logvol_agency;

  /**
   * @brief Logvol generation facility
   */
  std::string _logvol_facility;

  /**
   * @brief Number of file pointer records
   */
  int _n_filepoint;

  /**
   * @brief Number of records in volume directory file
   */
  int _n_voldir;

  /**
   * @brief Product identifier
   */
    std::string _product_id;


private:
};
}
#endif
