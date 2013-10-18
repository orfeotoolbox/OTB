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

#ifndef EnvisatAsarData_h
#define EnvisatAsarData_h

#include <iostream>
#include "sph.h"
#include "mph.h"
#include "SQ_ADSR.h"
#include "MainProcessingParameters.h"
#include "AntennaElevationPatterns.h"
#include "DopplerCentroidParameters.h"
#include "SRGRConversionParameters.h"
#include "ChirpParameters.h"
#include "GeolocationGrid.h"
#include <map>
#include <list>
#include <vector>

namespace ossimplugins
{
/**
 * @ingroup EnvisatAsarDataFile
 * @brief This class is able to read the Envisat file structure
 */
class EnvisatAsarData
{
public:
  /**
   * @brief Constructor
   */
  EnvisatAsarData();

  /**
   * @brief Destructor
   */
  ~EnvisatAsarData();

  /**
   * @brief This function writes the EnvisatAsarData in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const EnvisatAsarData& data);

  /**
   * @brief This function reads a EnvisatAsarData from a stream
   */
  friend std::istream& operator>>(std::istream& is, EnvisatAsarData& data);

  /**
   * @brief Copy constructor
   */
  EnvisatAsarData(const EnvisatAsarData& rhs);

  /**
   * @brief Copy operator
   */
  EnvisatAsarData& operator=(const EnvisatAsarData& rhs);

  /**
   * @brief Removes all the previous records from the EnvisatAsarData
   */
  void ClearRecords();

  mph * get_mph();
  sph * get_sph();
  SQ_ADSR * get_SQ_ADSR();
  MainProcessingParameters * get_MainProcessingParameters();
  AntennaElevationPatterns * get_AntennaElevationPatterns();
  DopplerCentroidParameters * get_DopplerCentroidParameters();
  SRGRConversionParameters * get_SRGRConversionParameters(int pos = 0);
  ChirpParameters * get_ChirpParameters();
  GeolocationGrid * get_GeolocationGrid(int num_ds = 0);

  int get_num_ds(EnvisatAsarRecord * record);

protected:
  std::list<EnvisatAsarRecord*> _records;

private:

};
}
#endif
