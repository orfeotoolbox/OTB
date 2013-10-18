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

#ifndef ImageOptionsFileDescriptor_h
#define ImageOptionsFileDescriptor_h

#include <iostream>

#include <RadarSat/RadarSatRecord.h>
#include <RadarSat/RadarSatRecordHeader.h>

namespace ossimplugins
{
/**
 * @ingroup ImageOptionsFileDescriptionRecord
 * @brief This class is able to read an Image options file descriptor record
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 03-12-07
 */
class ImageOptionsFileDescriptor : public RadarSatRecord
{
public:
  /**
   * @brief Constructor
   */
  ImageOptionsFileDescriptor();

  /**
   * @brief Destructor
   */
  ~ImageOptionsFileDescriptor();

  /**
   * @brief Copy constructor
   */
  ImageOptionsFileDescriptor(const ImageOptionsFileDescriptor& rhs);

  /**
   * @brief Copy operator
   */
  ImageOptionsFileDescriptor& operator=(const ImageOptionsFileDescriptor& rhs);
  /**
   * @brief This function writes the ImageOptionsFileDescriptor in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const ImageOptionsFileDescriptor& data);

  /**
   * @brief This function reads a ImageOptionsFileDescriptor from a stream
   */
  friend std::istream& operator>>(std::istream& is, ImageOptionsFileDescriptor& data);

  /**
   * @brief This function is able to create a new instance of the class
   */
  RadarSatRecord* Instanciate()
  {
    return new ImageOptionsFileDescriptor();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  RadarSatRecord* Clone()
  {
    return new ImageOptionsFileDescriptor(*this);
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
   * @brief Set the number of lines per data set
   */
  void set_nlin(int inNbLines)
  {
    _nlin = inNbLines;
  };
  /**
   * @brief ASCII flag
   */
  std::string   get_ascii_flag()
  {
    return _ascii_flag;
  };

    /**
   * @brief Format control document
   */
  std::string   get_format_doc()
  {
    return _format_doc;
  };
    /**
   * @brief Format document revision
   */
  std::string   get_format_rev()
  {
    return _format_rev;
  };
    /**
   * @brief File design revision
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
    return _file_num;
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
   * @brief Sequence number length
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
  std::string   get_rec_len()
  {
    return _rec_len;
  };
    /**
   * @brief Record length location
   */
  int   get_rlen_loc()
  {
    return _rlen_loc;
  };
    /**
   * @brief Record length length
   */
  int   get_rlen_len()
  {
    return _rlen_len;
  };

    /**
   * @brief Number of SAR data records
   */
  int   get_n_dataset()
  {
    return _n_dataset;
  };
    /**
   * @brief SAR data record length
   */
  int   get_l_dataset()
  {
    return _l_dataset;
  };

    /**
   * @brief Number of bits per sample
   */
  int   get_nbit()
  {
    return _nbit;
  };
    /**
   * @brief Samples per data group
   */
  int   get_nsamp()
  {
    return _nsamp;
  };
    /**
   * @brief Bytes per data group
   */
  int   get_nbyte()
  {
    return _nbyte;
  };
    /**
   * @brief Sample justification and order
   */
  std::string   get_justify()
  {
    return _justify;
  };
    /**
   * @brief Number of SAR channels
   */
  int   get_nchn()
  {
    return _nchn;
  };
    /**
   * @brief Lines per data set
   */
  int   get_nlin()
  {
    return _nlin;
  };
    /**
   * @brief Left border pixels per line
   */
  int   get_nleft()
  {
    return _nleft;
  };
    /**
   * @brief Groups per line per channel
   */
  int   get_ngrp()
  {
    return _ngrp;
  };
    /**
   * @brief Right border pixels per line
   */
  int   get_nright()
  {
    return _nright;
  };
    /**
   * @brief Top border lines
   */
  int   get_ntop()
  {
    return _ntop;
  };
    /**
   * @brief Bottom border lines
   */
  int   get_nbott()
  {
    return _nbott;
  };
    /**
   * @brief Interleave indicator
   */
  std::string   get_intleav()
  {
    return _intleav;
  };
    /**
   * @brief Number of physical records per line
   */
  int   get_nrec_lin()
  {
    return _nrec_lin;
  };
    /**
   * @brief Number of physical records per multi-channel line in this file
   */
  int   get_nrec_chn()
  {
    return _nrec_chn;
  };
    /**
   * @brief Number of bytes of prefix data per record
   */
  int   get_n_prefix()
  {
    return _n_prefix;
  };
    /**
   * @brief Number of bytes of SAR data (or pixel data) per record
   */
  int   get_n_sar()
  {
    return _n_sar;
  };
    /**
   * @brief Number of bytes of suffix data per record
   */
  int   get_n_suffix()
  {
    return _n_suffix;
  };

    /**
   * @brief Line number locator
   */
  std::string   get_lin_loc()
  {
    return _lin_loc;
  };
    /**
   * @brief Channel number locator
   */
  std::string   get_chn_loc()
  {
    return _chn_loc;
  };
    /**
   * @brief Time locator
   */
  std::string   get_tim_loc()
  {
    return _tim_loc;
  };
    /**
   * @brief Left fill locator
   */
  std::string   get_left_loc()
  {
    return _left_loc;
  };
    /**
   * @brief Right fill locator
   */
  std::string   get_right_loc()
  {
    return _right_loc;
  };
    /**
   * @brief Pad pixel indicator
   */
  std::string   get_pad_ind()
  {
    return _pad_ind;
  };

    /**
   * @brief Quality code locator
   */
  std::string   get_qual_loc()
  {
    return _qual_loc;
  };
    /**
   * @brief Calibration info locator
   */
  std::string   get_cali_loc()
  {
    return _cali_loc;
  };
    /**
   * @brief Gain value locator
   */
  std::string   get_gain_loc()
  {
    return _gain_loc;
  };
    /**
   * @brief Bias value locator
   */
  std::string   get_bias_loc()
  {
    return _bias_loc;
  };
    /**
   * @brief Data type identifier
   */
  std::string   get_type_id()
  {
    return _type_id;
  };
  /**
   * @brief Data type code
   */
  std::string   get_type_code()
  {
    return _type_code;
  };
    /**
   * @brief Number of left fill bits
   */
  int   get_left_fill()
  {
    return _left_fill;
  };
    /**
   * @brief Number of right fill bits
   */
  int   get_right_fill()
  {
    return _right_fill;
  };
    /**
   * @brief Maximum data range of pixel
   */
  int   get_pix_rng()
  {
    return _pix_rng;
  };

protected:
  /**
   * @brief ASCII flag
   */
  std::string   _ascii_flag;

    /**
   * @brief Format control document
   */
  std::string   _format_doc;
    /**
   * @brief Format document revision
   */
  std::string   _format_rev;
    /**
   * @brief File design revision
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
   * @brief Sequence number length
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
  std::string   _rec_len;
    /**
   * @brief Record length location
   */
  int   _rlen_loc;
    /**
   * @brief Record length length
   */
  int   _rlen_len;


    /**
   * @brief Number of SAR data records
   */
  int   _n_dataset;
    /**
   * @brief SAR data record length
   */
  int   _l_dataset;

    /**
   * @brief Number of bits per sample
   */
  int   _nbit;
    /**
   * @brief Samples per data group
   */
  int   _nsamp;
    /**
   * @brief Bytes per data group
   */
  int   _nbyte;
    /**
   * @brief Sample justification and order
   */
  std::string   _justify;
    /**
   * @brief Number of SAR channels
   */
  int   _nchn;
    /**
   * @brief Lines per data set
   */
  int   _nlin;
    /**
   * @brief Left border pixels per line
   */
  int   _nleft;
    /**
   * @brief Groups per line per channel
   */
  int   _ngrp;
    /**
   * @brief Right border pixels per line
   */
  int   _nright;
    /**
   * @brief Top border lines
   */
  int   _ntop;
    /**
   * @brief Bottom border lines
   */
  int   _nbott;
    /**
   * @brief Interleave indicator
   */
  std::string   _intleav;
    /**
   * @brief Number of physical records per line
   */
  int   _nrec_lin;
    /**
   * @brief Number of physical records per multi-channel line in this file
   */
  int   _nrec_chn;
    /**
   * @brief Number of bytes of prefix data per record
   */
  int   _n_prefix;
    /**
   * @brief Number of bytes of SAR data (or pixel data) per record
   */
  int   _n_sar;
    /**
   * @brief Number of bytes of suffix data per record
   */
  int   _n_suffix;

    /**
   * @brief Line number locator
   */
  std::string   _lin_loc;
    /**
   * @brief Channel number locator
   */
  std::string   _chn_loc;
    /**
   * @brief Time locator
   */
  std::string   _tim_loc;
    /**
   * @brief Left fill locator
   */
  std::string   _left_loc;
    /**
   * @brief Right fill locator
   */
  std::string   _right_loc;
    /**
   * @brief Pad pixel indicator
   */
  std::string   _pad_ind;

    /**
   * @brief Quality code locator
   */
  std::string   _qual_loc;
    /**
   * @brief Calibration info locator
   */
  std::string   _cali_loc;
    /**
   * @brief Gain value locator
   */
  std::string   _gain_loc;
    /**
   * @brief Bias value locator
   */
  std::string   _bias_loc;
    /**
   * @brief Data type identifier
   */
  std::string   _type_id;
  /**
   * @brief Data type code
   */
  std::string   _type_code;
    /**
   * @brief Number of left fill bits
   */
  int   _left_fill;
    /**
   * @brief Number of right fill bits
   */
  int   _right_fill;
    /**
   * @brief Maximum data range of pixel
   */
  int   _pix_rng;

private:
};
}
#endif
