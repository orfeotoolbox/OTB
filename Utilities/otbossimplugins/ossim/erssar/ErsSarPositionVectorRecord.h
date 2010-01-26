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

#ifndef ErsSarPositionVectorRecord_h
#define ErsSarPositionVectorRecord_h

#include <iostream>
#include <cstdlib>
#include "erssar/ErsSarRecord.h"

namespace ossimplugins
{

/**
 * @ingroup PlatformPositionDataRecord
 * @brief This class is able to read a position vector record
 *
 */
class ErsSarPositionVectorRecord
{
public:
  /**
   * @brief Constructor
   */
  ErsSarPositionVectorRecord();

  /**
   * @brief Destructor
   */
  virtual ~ErsSarPositionVectorRecord();

  /**
   * @brief Copy constructor
   */
  ErsSarPositionVectorRecord(const ErsSarPositionVectorRecord& rhs);

  /**
   * @brief Copy operator
   */
  ErsSarPositionVectorRecord& operator=(const ErsSarPositionVectorRecord& rhs);
  /**
   * @brief This function write the ErsSar PositionVectorRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const ErsSarPositionVectorRecord& data);

  /**
   * @brief This function read a ErsSar PositionVectorRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, ErsSarPositionVectorRecord& data);

  /**
   * @brief Data point position (m)
   */
  const double* get_pos() const
  {
    return _pos;
  };

  /**
   * @brief Data point velocity (mm/s)
   */
  const double* get_vel() const
  {
    return _vel;
  };

protected:
  /**
   * @brief Data point position (m)
   */
  double _pos[3];
  /**
   * @brief Data point velocity (mm/s)
   */
  double _vel[3];
private:


};
}
#endif
