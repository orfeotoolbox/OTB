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

#ifndef SRGRCoefficientSetRecord_h
#define SRGRCoefficientSetRecord_h

#include <iostream>
#include <RadarSat/RadarSatRecord.h>

namespace ossimplugins
{
/**
 * @ingroup ProcessingParametersRecord
 * @brief This class is able to read a SRGR coefficients set record
 */
class SRGRCoefficientSetRecord
{
public:
  /**
   * @brief Constructor
   */
  SRGRCoefficientSetRecord();

  /**
   * @brief Destructor
   */
  ~SRGRCoefficientSetRecord();

  /**
   * @brief Copy constructor
   */
  SRGRCoefficientSetRecord(const SRGRCoefficientSetRecord& rhs);

  /**
   * @brief Copy operator
   */
  SRGRCoefficientSetRecord& operator=(const SRGRCoefficientSetRecord& rhs);

  /**
   * @brief This function writes the SRGRCoefficientSetRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const SRGRCoefficientSetRecord& data);

  /**
   * @brief This function reads a SRGRCoefficientSetRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, SRGRCoefficientSetRecord& data);

  /**
   * @brief SRGR update date/time
   */
  std::string   get_srgr_update()
  {
    return _srgr_update;
  }

    /**
   * @brief SRGR coefficients
   */
  double*   get_srgr_coef()
  {
    return _srgr_coef;
  }

protected:
  /**
   * @brief SRGR update date/time
   */
  std::string   _srgr_update;

    /**
   * @brief SRGR coefficients
   */
  double   _srgr_coef[6];
private:

};
}
#endif
