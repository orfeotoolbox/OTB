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

#ifndef sph_h
#define sph_h

#include <iostream>
#include <vector>
#include <EnvisatAsar/EnvisatAsarRecord.h>
#include <EnvisatAsar/mph.h>
#include <EnvisatAsar/dsd.h>

namespace ossimplugins
{
/**
 * @ingroup sphRecord
 * @brief This class is able to read the SAR sph record of the ASAR file
 */
class sph : public EnvisatAsarRecord
{
public:
  /**
   * @brief Constructor
   */
  sph();

  /**
   * @brief Constructor Using an mph record to initialize num_dsd and dsd_size
   */
  sph(const mph& data);
  /**
   * @brief Destructor
   */
  virtual ~sph();

  /**
   * @brief This function write the sph in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const sph& data);

  /**
   * @brief This function read a sph from a stream
   */
  friend std::istream& operator>>(std::istream& is, sph& data);

  /**
   * @brief Copy constructor
   */
  sph(const sph& rhs);

  /**
   * @brief Copy operator
   */
  sph& operator=(const sph& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  EnvisatAsarRecord* Instanciate()
  {
    return new sph();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  EnvisatAsarRecord* Clone()
  {
    return new sph(*this);
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
  * @brief sph_desc_title
  */
  std::string   get_sph_desc_title()
  {
    return _sph_desc_title;
  };
  /**
  * @brief sph_desc
  */
  std::string   get_sph_desc()
  {
    return _sph_desc;
  };
  /**
  * @brief strip_cont_ind_title
  */
  std::string   get_strip_cont_ind_title()
  {
    return _strip_cont_ind_title;
  };
  /**
  * @brief strip_cont_ind
  */
  std::string   get_strip_cont_ind()
  {
    return _strip_cont_ind;
  };
  /**
  * @brief slice_pos_title
  */
  std::string   get_slice_pos_title()
  {
    return _slice_pos_title;
  };
  /**
  * @brief slice_position
  */
  std::string   get_slice_position()
  {
    return _slice_position;
  };
  /**
  * @brief num_slices_title
  */
  std::string   get_num_slices_title()
  {
    return _num_slices_title;
  };
  /**
  * @brief num_slices
  */
  std::string   get_num_slices()
  {
    return _num_slices;
  };
  /**
  * @brief first_line_time_title
  */
  std::string   get_first_line_time_title()
  {
    return _first_line_time_title;
  };
  /**
  * @brief first_line_time
  */
  std::string   get_first_line_time()
  {
    return _first_line_time;
  };
  /**
  * @brief last_line_time_title
  */
  std::string   get_last_line_time_title()
  {
    return _last_line_time_title;
  };
  /**
  * @brief last_line_time
  */
  std::string   get_last_line_time()
  {
    return _last_line_time;
  };
  /**
  * @brief first_near_lat_title
  */
  std::string   get_first_near_lat_title()
  {
    return _first_near_lat_title;
  };
  /**
  * @brief first_near_lat
  */
  std::string   get_first_near_lat()
  {
    return _first_near_lat;
  };
  /**
  * @brief first_near_lat_units
  */
  std::string   get_first_near_lat_units()
  {
    return _first_near_lat_units;
  };
  /**
  * @brief first_near_long_title
  */
  std::string   get_first_near_long_title()
  {
    return _first_near_long_title;
  };
  /**
  * @brief first_near_long
  */
  std::string   get_first_near_long()
  {
    return _first_near_long;
  };
  /**
  * @brief first_near_long_units
  */
  std::string   get_first_near_long_units()
  {
    return _first_near_long_units;
  };
  /**
  * @brief first_mid_lat_title
  */
  std::string   get_first_mid_lat_title()
  {
    return _first_mid_lat_title;
  };
  /**
  * @brief first_mid_lat
  */
  std::string   get_first_mid_lat()
  {
    return _first_mid_lat;
  };
  /**
  * @brief first_mid_lat_units
  */
  std::string   get_first_mid_lat_units()
  {
    return _first_mid_lat_units;
  };
  /**
  * @brief first_mid_long_title
  */
  std::string   get_first_mid_long_title()
  {
    return _first_mid_long_title;
  };
  /**
  * @brief first_mid_long
  */
  std::string   get_first_mid_long()
  {
    return _first_mid_long;
  };
  /**
  * @brief first_mid_long_units
  */
  std::string   get_first_mid_long_units()
  {
    return _first_mid_long_units;
  };
  /**
  * @brief first_far_lat_title
  */
  std::string   get_first_far_lat_title()
  {
    return _first_far_lat_title;
  };
  /**
  * @brief first_far_lat
  */
  std::string   get_first_far_lat()
  {
    return _first_far_lat;
  };
  /**
  * @brief first_far_lat_units
  */
  std::string   get_first_far_lat_units()
  {
    return _first_far_lat_units;
  };
  /**
  * @brief first_far_long_title
  */
  std::string   get_first_far_long_title()
  {
    return _first_far_long_title;
  };
  /**
  * @brief first_far_long
  */
  std::string   get_first_far_long()
  {
    return _first_far_long;
  };
  /**
  * @brief first_far_long_units
  */
  std::string   get_first_far_long_units()
  {
    return _first_far_long_units;
  };
  /**
  * @brief last_near_lat_title
  */
  std::string   get_last_near_lat_title()
  {
    return _last_near_lat_title;
  };
  /**
  * @brief last_near_lat
  */
  std::string   get_last_near_lat()
  {
    return _last_near_lat;
  };
  /**
  * @brief last_near_lat_units
  */
  std::string   get_last_near_lat_units()
  {
    return _last_near_lat_units;
  };
  /**
  * @brief last_near_long_title
  */
  std::string   get_last_near_long_title()
  {
    return _last_near_long_title;
  };
  /**
  * @brief last_near_long
  */
  std::string   get_last_near_long()
  {
    return _last_near_long;
  };
  /**
  * @brief last_near_long_units
  */
  std::string   get_last_near_long_units()
  {
    return _last_near_long_units;
  };
  /**
  * @brief last_mid_lat_title
  */
  std::string   get_last_mid_lat_title()
  {
    return _last_mid_lat_title;
  };
  /**
  * @brief last_mid_lat
  */
  std::string   get_last_mid_lat()
  {
    return _last_mid_lat;
  };
  /**
  * @brief last_mid_lat_units
  */
  std::string   get_last_mid_lat_units()
  {
    return _last_mid_lat_units;
  };
  /**
  * @brief last_mid_long_title
  */
  std::string   get_last_mid_long_title()
  {
    return _last_mid_long_title;
  };
  /**
  * @brief last_mid_long
  */
  std::string   get_last_mid_long()
  {
    return _last_mid_long;
  };
  /**
  * @brief last_mid_long_units
  */
  std::string   get_last_mid_long_units()
  {
    return _last_mid_long_units;
  };
  /**
  * @brief last_far_lat_title
  */
  std::string   get_last_far_lat_title()
  {
    return _last_far_lat_title;
  };
  /**
  * @brief last_far_lat
  */
  std::string   get_last_far_lat()
  {
    return _last_far_lat;
  };
  /**
  * @brief last_far_lat_units
  */
  std::string   get_last_far_lat_units()
  {
    return _last_far_lat_units;
  };
  /**
  * @brief last_far_long_title
  */
  std::string   get_last_far_long_title()
  {
    return _last_far_long_title;
  };
  /**
  * @brief last_far_long
  */
  std::string   get_last_far_long()
  {
    return _last_far_long;
  };
  /**
  * @brief last_far_long_units
  */
  std::string   get_last_far_long_units()
  {
    return _last_far_long_units;
  };
  /**
  * @brief swath_title
  */
  std::string   get_swath_title()
  {
    return _swath_title;
  };
  /**
  * @brief swath
  */
  std::string   get_swath()
  {
    return _swath;
  };
  /**
  * @brief pass_title
  */
  std::string   get_pass_title()
  {
    return _pass_title;
  };
  /**
  * @brief pass
  */
  std::string   get_pass()
  {
    return _pass;
  };
  /**
  * @brief sample_type_title
  */
  std::string   get_sample_type_title()
  {
    return _sample_type_title;
  };
  /**
  * @brief sample_type
  */
  std::string   get_sample_type()
  {
    return _sample_type;
  };
  /**
  * @brief algo_title
  */
  std::string   get_algo_title()
  {
    return _algo_title;
  };
  /**
  * @brief algorithm
  */
  std::string   get_algorithm()
  {
    return _algorithm;
  };
  /**
  * @brief mds1_tx_rx_polar_title
  */
  std::string   get_mds1_tx_rx_polar_title()
  {
    return _mds1_tx_rx_polar_title;
  };
  /**
  * @brief mds1_tx_rx_polar
  */
  std::string   get_mds1_tx_rx_polar()
  {
    return _mds1_tx_rx_polar;
  };
  /**
  * @brief mds2_tx_rx_polar_title
  */
  std::string   get_mds2_tx_rx_polar_title()
  {
    return _mds2_tx_rx_polar_title;
  };
  /**
  * @brief mds2_tx_rx_polar
  */
  std::string   get_mds2_tx_rx_polar()
  {
    return _mds2_tx_rx_polar;
  };
  /**
  * @brief compression_title
  */
  std::string   get_compression_title()
  {
    return _compression_title;
  };
  /**
  * @brief compression
  */
  std::string   get_compression()
  {
    return _compression;
  };
  /**
  * @brief azimut_looks_title
  */
  std::string   get_azimut_looks_title()
  {
    return _azimut_looks_title;
  };
  /**
  * @brief azimut_looks
  */
  std::string   get_azimut_looks()
  {
    return _azimut_looks;
  };
  /**
  * @brief range_looks_title
  */
  std::string   get_range_looks_title()
  {
    return _range_looks_title;
  };
  /**
  * @brief range_looks
  */
  std::string   get_range_looks()
  {
    return _range_looks;
  };
  /**
  * @brief range_spacing_title
  */
  std::string   get_range_spacing_title()
  {
    return _range_spacing_title;
  };
  /**
  * @brief range_spacing
  */
  std::string   get_range_spacing()
  {
    return _range_spacing;
  };
  /**
  * @brief range_spacing_unit
  */
  std::string   get_range_spacing_unit()
  {
    return _range_spacing_unit;
  };
  /**
  * @brief azimut_spacing_title
  */
  std::string   get_azimut_spacing_title()
  {
    return _azimut_spacing_title;
  };
  /**
  * @brief azimut_spacing
  */
  std::string   get_azimut_spacing()
  {
    return _azimut_spacing;
  };
  /**
  * @brief azimut_spacing_unit
  */
  std::string   get_azimut_spacing_unit()
  {
    return _azimut_spacing_unit;
  };
  /**
  * @brief line_time_interval_title
  */
  std::string   get_line_time_interval_title()
  {
    return _line_time_interval_title;
  };
  /**
  * @brief line_time_interval
  */
  std::string   get_line_time_interval()
  {
    return _line_time_interval;
  };
  /**
  * @brief line_time_interv_units
  */
  std::string   get_line_time_interv_units()
  {
    return _line_time_interv_units;
  };
  /**
  * @brief line_length_title
  */
  std::string   get_line_length_title()
  {
    return _line_length_title;
  };
  /**
  * @brief line_length
  */
  std::string   get_line_length()
  {
    return _line_length;
  };
  /**
  * @brief line_length_units
  */
  std::string   get_line_length_units()
  {
    return _line_length_units;
  };
  /**
  * @brief data_type_title
  */
  std::string   get_data_type_title()
  {
    return _data_type_title;
  };
  /**
  * @brief data_type
  */
  std::string   get_data_type()
  {
    return _data_type;
  };
  /**
  * @brief data_type
  */
  std::vector<dsd>  get_dsd_vector()
  {
    return _dsd_vector;
  }

    /**
  * @brief update DSD number and dsd size from a mph record
  */
  bool  update_sph_from_mph( mph& mph_rec)
  {
    _num_dsd = mph_rec._num_dsd;
    _size_dsd = mph_rec._dsd_size;

    return 1;
  };


protected:
  /**
  * @brief Number of Data Set Descriptors (DSD)
  */
  long   _num_dsd;

  /**
  * @brief Size of each Data Set Descriptors (DSD)
  */
  long   _size_dsd;

  /**
  * @brief sph_desc_title
  */
  std::string   _sph_desc_title;
  /**
  * @brief sph_desc
  */
  std::string   _sph_desc;
  /**
  * @brief strip_cont_ind_title
  */
  std::string   _strip_cont_ind_title;
  /**
  * @brief strip_cont_ind
  */
  std::string   _strip_cont_ind;
  /**
  * @brief slice_pos_title
  */
  std::string   _slice_pos_title;
  /**
  * @brief slice_position
  */
  std::string   _slice_position;
  /**
  * @brief num_slices_title
  */
  std::string   _num_slices_title;
  /**
  * @brief num_slices
  */
  std::string   _num_slices;
  /**
  * @brief first_line_time_title
  */
  std::string   _first_line_time_title;
  /**
  * @brief first_line_time
  */
  std::string   _first_line_time;
  /**
  * @brief last_line_time_title
  */
  std::string   _last_line_time_title;
  /**
  * @brief last_line_time
  */
  std::string   _last_line_time;
  /**
  * @brief first_near_lat_title
  */
  std::string   _first_near_lat_title;
  /**
  * @brief first_near_lat
  */
  std::string   _first_near_lat;
  /**
  * @brief first_near_lat_units
  */
  std::string   _first_near_lat_units;
  /**
  * @brief first_near_long_title
  */
  std::string   _first_near_long_title;
  /**
  * @brief first_near_long
  */
  std::string   _first_near_long;
  /**
  * @brief first_near_long_units
  */
  std::string   _first_near_long_units;
  /**
  * @brief first_mid_lat_title
  */
  std::string   _first_mid_lat_title;
  /**
  * @brief first_mid_lat
  */
  std::string   _first_mid_lat;
  /**
  * @brief first_mid_lat_units
  */
  std::string   _first_mid_lat_units;
  /**
  * @brief first_mid_long_title
  */
  std::string   _first_mid_long_title;
  /**
  * @brief first_mid_long
  */
  std::string   _first_mid_long;
  /**
  * @brief first_mid_long_units
  */
  std::string   _first_mid_long_units;
  /**
  * @brief first_far_lat_title
  */
  std::string   _first_far_lat_title;
  /**
  * @brief first_far_lat
  */
  std::string   _first_far_lat;
  /**
  * @brief first_far_lat_units
  */
  std::string   _first_far_lat_units;
  /**
  * @brief first_far_long_title
  */
  std::string   _first_far_long_title;
  /**
  * @brief first_far_long
  */
  std::string   _first_far_long;
  /**
  * @brief first_far_long_units
  */
  std::string   _first_far_long_units;
  /**
  * @brief last_near_lat_title
  */
  std::string   _last_near_lat_title;
  /**
  * @brief last_near_lat
  */
  std::string   _last_near_lat;
  /**
  * @brief last_near_lat_units
  */
  std::string   _last_near_lat_units;
  /**
  * @brief last_near_long_title
  */
  std::string   _last_near_long_title;
  /**
  * @brief last_near_long
  */
  std::string   _last_near_long;
  /**
  * @brief last_near_long_units
  */
  std::string   _last_near_long_units;
  /**
  * @brief last_mid_lat_title
  */
  std::string   _last_mid_lat_title;
  /**
  * @brief last_mid_lat
  */
  std::string   _last_mid_lat;
  /**
  * @brief last_mid_lat_units
  */
  std::string   _last_mid_lat_units;
  /**
  * @brief last_mid_long_title
  */
  std::string   _last_mid_long_title;
  /**
  * @brief last_mid_long
  */
  std::string   _last_mid_long;
  /**
  * @brief last_mid_long_units
  */
  std::string   _last_mid_long_units;
  /**
  * @brief last_far_lat_title
  */
  std::string   _last_far_lat_title;
  /**
  * @brief last_far_lat
  */
  std::string   _last_far_lat;
  /**
  * @brief last_far_lat_units
  */
  std::string   _last_far_lat_units;
  /**
  * @brief last_far_long_title
  */
  std::string   _last_far_long_title;
  /**
  * @brief last_far_long
  */
  std::string   _last_far_long;
  /**
  * @brief last_far_long_units
  */
  std::string   _last_far_long_units;
  /**
  * @brief swath_title
  */
  std::string   _swath_title;
  /**
  * @brief swath
  */
  std::string   _swath;
  /**
  * @brief pass_title
  */
  std::string   _pass_title;
  /**
  * @brief pass
  */
  std::string   _pass;
  /**
  * @brief sample_type_title
  */
  std::string   _sample_type_title;
  /**
  * @brief sample_type
  */
  std::string   _sample_type;
  /**
  * @brief algo_title
  */
  std::string   _algo_title;
  /**
  * @brief algorithm
  */
  std::string   _algorithm;
  /**
  * @brief mds1_tx_rx_polar_title
  */
  std::string   _mds1_tx_rx_polar_title;
  /**
  * @brief mds1_tx_rx_polar
  */
  std::string   _mds1_tx_rx_polar;
  /**
  * @brief mds2_tx_rx_polar_title
  */
  std::string   _mds2_tx_rx_polar_title;
  /**
  * @brief mds2_tx_rx_polar
  */
  std::string   _mds2_tx_rx_polar;
  /**
  * @brief compression_title
  */
  std::string   _compression_title;
  /**
  * @brief compression
  */
  std::string   _compression;
  /**
  * @brief azimut_looks_title
  */
  std::string   _azimut_looks_title;
  /**
  * @brief azimut_looks
  */
  std::string   _azimut_looks;
  /**
  * @brief range_looks_title
  */
  std::string   _range_looks_title;
  /**
  * @brief range_looks
  */
  std::string   _range_looks;
  /**
  * @brief range_spacing_title
  */
  std::string   _range_spacing_title;
  /**
  * @brief range_spacing
  */
  std::string   _range_spacing;
  /**
  * @brief range_spacing_unit
  */
  std::string   _range_spacing_unit;
  /**
  * @brief azimut_spacing_title
  */
  std::string   _azimut_spacing_title;
  /**
  * @brief azimut_spacing
  */
  std::string   _azimut_spacing;
  /**
  * @brief azimut_spacing_unit
  */
  std::string   _azimut_spacing_unit;
  /**
  * @brief line_time_interval_title
  */
  std::string   _line_time_interval_title;
  /**
  * @brief line_time_interval
  */
  std::string   _line_time_interval;
  /**
  * @brief line_time_interv_units
  */
  std::string   _line_time_interv_units;
  /**
  * @brief line_length_title
  */
  std::string   _line_length_title;
  /**
  * @brief line_length
  */
  std::string   _line_length;
  /**
  * @brief line_length_units
  */
  std::string   _line_length_units;
  /**
  * @brief data_type_title
  */
  std::string   _data_type_title;
  /**
  * @brief data_type
  */
  std::string   _data_type;
    /**
  * @brief DSD records
  */
  std::vector<dsd>  _dsd_vector;

private:

};
}
#endif
