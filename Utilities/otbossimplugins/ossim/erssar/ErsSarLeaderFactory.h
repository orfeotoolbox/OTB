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

#ifndef ErsSarLeaderFactory_h
#define ErsSarLeaderFactory_h


#include "erssar/ErsSarRecordFactory.h"
#include <map>


namespace ossimplugins
{

/**
 * @ingroup ErsSarLeaderFile
 * @brief This class is a facory able to construct Record base classes
 *
 */
class ErsSarLeaderFactory : public ErsSarRecordFactory
{
public:
  /**
   * @brief Contstructor
   */
  ErsSarLeaderFactory();
  /**
   * @brief Destructor
   */
  virtual ~ErsSarLeaderFactory();


protected:


private:
};
}

#endif
