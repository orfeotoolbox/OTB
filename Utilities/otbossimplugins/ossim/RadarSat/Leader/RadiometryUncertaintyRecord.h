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

#ifndef RadiometryUncertaintyRecord_h
#define RadiometryUncertaintyRecord_h

#include<iostream>
#include <RadarSat/RadarSatRecord.h>

namespace ossimplugins
{
/**
 * @ingroup DataQualitySummaryRecord
 * @brief This class is able to read a radiometry uncertainty record
 */
class RadiometryUncertaintyRecord
{
public:
  /**
   * @brief Constructor
   */
  RadiometryUncertaintyRecord();

  /**
   * @brief Destructor
   */
  ~RadiometryUncertaintyRecord();

  /**
   * @brief Copy constructor
   */
  RadiometryUncertaintyRecord(const RadiometryUncertaintyRecord& rhs);

  /**
   * @brief Copy operator
   */
  RadiometryUncertaintyRecord& operator=(const RadiometryUncertaintyRecord& rhs);
  /**
   * @brief This function writes the RadiometryUncertaintyRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const RadiometryUncertaintyRecord& data);

  /**
   * @brief This function reads a RadiometryUncertaintyRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, RadiometryUncertaintyRecord& data);

  /**
   * @brief Units of db
   */
  double   get_db()
  {
    return _db;
  };
    /**
   * @brief Units of deg
   */
  double   get_deg()
  {
    return _deg;
  };
protected:

  /**
   * @brief Units of db
   */
  double   _db;
    /**
   * @brief Units of deg
   */
  double   _deg;
private:


};
}
#endif
