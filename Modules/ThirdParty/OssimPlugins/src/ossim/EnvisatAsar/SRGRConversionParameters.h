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

#ifndef SRGRConversionParameters_h
#define SRGRConversionParameters_h

#include <iostream>
#include <EnvisatAsar/EnvisatAsarRecord.h>

namespace ossimplugins
{

/**
 * @ingroup SRGRConversionParametersRecord
 * @brief This class is able to read the ASAR SRGRConversionParameters record
 */
class SRGRConversionParameters : public EnvisatAsarRecord
{
public:
  /**
   * @brief Constructor
   */
  SRGRConversionParameters();

  /**
   * @brief Destructor
   */
  virtual ~SRGRConversionParameters();

  /**
   * @brief This function writes the SRGRConversionParameters in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const SRGRConversionParameters& data);

  /**
   * @brief This function reads a SRGRConversionParameters from a stream
   */
  friend std::istream& operator>>(std::istream& is, SRGRConversionParameters& data);

  /**
   * @brief Copy constructor
   */
  SRGRConversionParameters(const SRGRConversionParameters& rhs);

  /**
   * @brief Copy operator
   */
  SRGRConversionParameters& operator=(const SRGRConversionParameters& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  EnvisatAsarRecord* Instanciate()
  {
    return new SRGRConversionParameters();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  EnvisatAsarRecord* Clone()
  {
    return new SRGRConversionParameters(*this);
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
  * @brief first_zero_doppler_time day
  */
  int  get_first_zero_doppler_time_day()
  {
    return _first_zero_doppler_time_day;
  };
  /**
  * @brief first_zero_doppler_time second
  */
  unsigned int  get_first_zero_doppler_time_sec()
  {
    return _first_zero_doppler_time_sec;
  };
  /**
  * @brief first_zero_doppler_time micro sec
  */
  unsigned int  get_first_zero_doppler_time_microsec()
  {
    return _first_zero_doppler_time_microsec;
  };
  /**
  * @brief attach_flag
  */
  bool   get_attach_flag()
  {
    return _attach_flag;
  };
  /**
  * @brief input_mean_flag
  */
  float   get_slant_range_time()
  {
    return _slant_range_time;
  };
  /**
  * @brief input_std_dev_flag
  */
  float  get_ground_range_origin()
  {
    return _ground_range_origin;
  };
  /**
  * @brief input_gaps_flag
  */
  float*  get_srgr_coef()
  {
    return _srgr_coef;
  };

  /**
  * @brief first_zero_doppler_time day
  */
  int   _first_zero_doppler_time_day;
  /**
  * @brief first_zero_doppler_time sec
  */
  unsigned int   _first_zero_doppler_time_sec;
  /**
  * @brief first_zero_doppler_time microsec
  */
  unsigned int   _first_zero_doppler_time_microsec;
  /**
   * @brief attach_flag
   */
  bool   _attach_flag;
  /**
   * @brief input_mean_flag
   */
  float   _slant_range_time;
  /**
   * @brief input_std_dev_flag
   */
  float   _ground_range_origin;
  /**
   * @brief input_gaps_flag
   */
  float   _srgr_coef[5];


private:

};
}
#endif


