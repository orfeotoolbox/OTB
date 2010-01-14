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

#ifndef DataFactory_h
#define DataFactory_h

#include <RadarSat/RadarSatRecordFactory.h>
#include <map>

namespace ossimplugins
{

/**
 * @ingroup DataFile
 * @brief This class is a factory able to construct Record base classes
 */
class DataFactory : public RadarSatRecordFactory
{
public:
  /**
   * @brief Contstructor
   */
  DataFactory();
  /**
   * @brief Destructor
   */
  ~DataFactory();

protected:
private:
};
}
#endif
