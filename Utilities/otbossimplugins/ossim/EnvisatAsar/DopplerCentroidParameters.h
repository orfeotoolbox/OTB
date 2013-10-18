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

#ifndef DopplerCentroidParameters_h
#define DopplerCentroidParameters_h

#include <iostream>
#include <EnvisatAsar/EnvisatAsarRecord.h>

namespace ossimplugins
{
/**
 * @ingroup DopplerCentroidParametersRecord
 * @brief This class is able to read the ASAR DopplerCentroidParameters record
 */
class DopplerCentroidParameters : public EnvisatAsarRecord
{
public:
  /**
   * @brief Constructor
   */
  DopplerCentroidParameters();

  /**
   * @brief Destructor
   */
  virtual ~DopplerCentroidParameters();

  /**
   * @brief This function writes the DopplerCentroidParameters in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const DopplerCentroidParameters& data);

  /**
   * @brief This function reads a DopplerCentroidParameters from a stream
   */
  friend std::istream& operator>>(std::istream& is, DopplerCentroidParameters& data);

  /**
   * @brief Copy constructor
   */
  DopplerCentroidParameters(const DopplerCentroidParameters& rhs);

  /**
   * @brief Copy operator
   */
  DopplerCentroidParameters& operator=(const DopplerCentroidParameters& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  EnvisatAsarRecord* Instanciate()
  {
    return new DopplerCentroidParameters();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  EnvisatAsarRecord* Clone()
  {
    return new DopplerCentroidParameters(*this);
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
  * @brief zero_doppler_time
  */
  std::string   get_zero_doppler_time()
  {
    return _zero_doppler_time;
  };
  /**
  * @brief attach_flag
  */
  bool   get_attach_flag()
  {
    return _attach_flag;
  };
  /**
  * @brief slant_range_time
  */
  float   get_slant_range_time()
  {
    return _slant_range_time;
  };
  /**
  * @brief dop_coef[5]
  */
  float*   get_dop_coef()
  {
    return _dop_coef;
  };
  /**
  * @brief dop_conf
  */
  float   get_dop_conf()
  {
    return _dop_conf;
  };
  /**
  * @brief dop_conf_below_thresh_flag
  */
  bool   get_dop_conf_below_thresh_flag()
  {
    return _dop_conf_below_thresh_flag;
  };
  /**
  * @brief delta_dopp_coeff[5]
  */
  signed short  * get_delta_dopp_coeff()
  {
    return _delta_dopp_coeff;
  };

  /**
  * @brief zero_doppler_time
  */
  std::string   _zero_doppler_time;
  /**
  * @brief attach_flag
  */
  bool   _attach_flag;
  /**
  * @brief slant_range_time
  */
  float   _slant_range_time;
  /**
  * @brief dop_coef[5]
  */
  float   _dop_coef[5];
  /**
  * @brief dop_conf
  */
  float   _dop_conf;
  /**
  * @brief dop_conf_below_thresh_flag
  */
  bool   _dop_conf_below_thresh_flag;
  /**
  * @brief delta_dopp_coeff[5]
  */
  signed short   _delta_dopp_coeff[5];

private:

};
}
#endif


