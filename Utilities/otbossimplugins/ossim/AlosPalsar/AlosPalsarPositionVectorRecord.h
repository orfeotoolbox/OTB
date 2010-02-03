//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
// "Copyright Centre for Remote Imaging, Sensing and Processing"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef AlosPalsarPositionVectorRecord_h
#define AlosPalsarPositionVectorRecord_h

#include <iostream>
#include <cstdlib>
#include <AlosPalsar/AlosPalsarRecord.h>

namespace ossimplugins
{

/**
 * @ingroup PlatformPositionDataRecord
 * @brief This class is able to read a position vector record
 */
class AlosPalsarPositionVectorRecord
{
public:
  /**
   * @brief Constructor
   */
  AlosPalsarPositionVectorRecord();

  /**
   * @brief Destructor
   */
  ~AlosPalsarPositionVectorRecord();

  /**
   * @brief Copy constructor
   */
  AlosPalsarPositionVectorRecord(const AlosPalsarPositionVectorRecord& rhs);

  /**
   * @brief Copy operator
   */
  AlosPalsarPositionVectorRecord& operator=(const AlosPalsarPositionVectorRecord& rhs);
  /**
   * @brief This function write the AlosPalsar PositionVectorRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const AlosPalsarPositionVectorRecord& data);

  /**
   * @brief This function read a AlosPalsar PositionVectorRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, AlosPalsarPositionVectorRecord& data);

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
