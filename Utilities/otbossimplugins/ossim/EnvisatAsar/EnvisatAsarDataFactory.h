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

#ifndef EnvisatAsarDataFactory_h
#define EnvisatAsarDataFactory_h


#include <EnvisatAsar/EnvisatAsarRecordFactory.h>
#include <map>

namespace ossimplugins
{
/**
 * @ingroup EnvisatAsarDataFile
 * @brief This class is a factory able to construct Record base classes
 */
class EnvisatAsarDataFactory : public EnvisatAsarRecordFactory
{
public:
  /**
   * @brief Constructor
   */
  EnvisatAsarDataFactory();
  /**
   * @brief Destructor
   */
  ~EnvisatAsarDataFactory();


protected:


private:
};
}
#endif
