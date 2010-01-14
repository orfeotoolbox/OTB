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

#ifndef TemperatureSettingsRecord_h
#define TemperatureSettingsRecord_h

#include <iostream>
#include <RadarSat/RadarSatRecord.h>

namespace ossimplugins
{
/**
 * @ingroup ProcessingParametersRecord
 * @brief This class is able to read a Temperature Settings record
 */
class TemperatureSettingsRecord
{
public:
  /**
   * @brief Constructor
   */
  TemperatureSettingsRecord();

  /**
   * @brief Destructor
   */
  ~TemperatureSettingsRecord();

  /**
   * @brief Copy constructor
   */
  TemperatureSettingsRecord(const TemperatureSettingsRecord& rhs);

  /**
   * @brief Copy operator
   */
  TemperatureSettingsRecord& operator=(const TemperatureSettingsRecord& rhs);

  /**
   * @brief This function writes the TemperatureSettingsRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const TemperatureSettingsRecord& data);

  /**
   * @brief This function reads a TemperatureSettingsRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, TemperatureSettingsRecord& data);

  /**
   * @brief Temperature settings
   */
  int*   get_temp_set()
  {
    return _temp_set;
  };

protected:

  /**
   * @brief Temperature settings
   */
  int _temp_set[4];
private:

};
}
#endif
