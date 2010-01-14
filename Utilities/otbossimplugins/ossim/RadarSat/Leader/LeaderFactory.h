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

#ifndef LeaderFactory_h
#define LeaderFactory_h


#include <RadarSat/RadarSatRecordFactory.h>
#include <map>

namespace ossimplugins
{
/**
 * @ingroup LeaderFile
 * @brief This class is a factory able to construct Record base classes
 */
class LeaderFactory : public RadarSatRecordFactory
{
public:
  /**
   * @brief Contstructor
   */
  LeaderFactory();
  /**
   * @brief Destructor
   */
  ~LeaderFactory();


protected:


private:
};
}
#endif
