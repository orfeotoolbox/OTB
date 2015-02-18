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

#ifndef VolumeDirFactory_h
#define VolumeDirFactory_h


#include <RadarSat/RadarSatRecordFactory.h>
#include <map>

namespace ossimplugins
{
/**
 * @ingroup VolumeDirFile
 * @brief This class is a factory able to construct Record base classes
 */
class VolumeDirFactory : public RadarSatRecordFactory
{
public:
  /**
   * @brief Contstructor
   */
  VolumeDirFactory();
  /**
   * @brief Destructor
   */
  ~VolumeDirFactory();


protected:


private:
};
}
#endif
