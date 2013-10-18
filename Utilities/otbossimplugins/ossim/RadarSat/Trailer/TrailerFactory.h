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

#ifndef TrailerFactory_h
#define TrailerFactory_h


#include <RadarSat/RadarSatRecordFactory.h>
#include <map>

namespace ossimplugins
{
/**
 * @ingroup TrailerFile
 * @brief This class is a factory able to construct Record base classes
 */
class TrailerFactory : public RadarSatRecordFactory
{
public:
  /**
   * @brief Contstructor
   */
  TrailerFactory();
  /**
   * @brief Destructor
   */
  ~TrailerFactory();


protected:


private:
};
}
#endif
