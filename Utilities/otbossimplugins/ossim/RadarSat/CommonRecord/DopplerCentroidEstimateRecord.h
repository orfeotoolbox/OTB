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

#ifndef DopplerCentroidEstimateRecord_h
#define DopplerCentroidEstimateRecord_h

#include <iostream>

#include <RadarSat/RadarSatRecord.h>

namespace ossimplugins
{
/**
 * @ingroup ProcessingParametersRecord
 * @brief This class is able to read a Doppler centroid estimates record
 */
class DopplerCentroidEstimateRecord
{
public:
  /**
   * @brief Constructor
   */
  DopplerCentroidEstimateRecord();

  /**
   * @brief Destructor
   */
  ~DopplerCentroidEstimateRecord();

  /**
   * @brief Copy constructor
   */
  DopplerCentroidEstimateRecord(const DopplerCentroidEstimateRecord& rhs);

  /**
   * @brief Copy operator
   */
  DopplerCentroidEstimateRecord& operator=(const DopplerCentroidEstimateRecord& rhs);

  /**
   * @brief This function writes the DopplerCentroidEstimateRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const DopplerCentroidEstimateRecord& data);

  /**
   * @brief This function reads a DopplerCentroidEstimateRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, DopplerCentroidEstimateRecord& data);

  /**
   * @brief Doppler centroid confidence measure
   */
  double   get_dopcen_conf()
  {
    return _dopcen_conf;
  }
    /**
   * @brief Doppler centroid reference time (sec)
   */
  double   get_dopcen_ref_tim()
  {
    return _dopcen_ref_tim;
  }
    /**
   * @brief Doppler centroid coefficients
   */
  double*   get_dopcen_coef()
  {
    return _dopcen_coef;
  }

protected:
  /**
   * @brief Doppler centroid confidence measure
   */
  double   _dopcen_conf;
    /**
   * @brief Doppler centroid reference time (sec)
   */
  double   _dopcen_ref_tim;
    /**
   * @brief Doppler centroid coefficients
   */
  double   _dopcen_coef[4];
private:

};
}
#endif
