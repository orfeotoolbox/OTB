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

#ifndef AlosSarLeaderFactory_h
#define AlosSarLeaderFactory_h


#include <AlosPalsar/AlosSarRecordFactory.h>
#include <map>

namespace ossimplugins
{

/**
 * @ingroup AlosSarLeaderFile
 * @brief This class is a facory able to construct Record base classes
 */
class AlosSarLeaderFactory : public AlosSarRecordFactory
{
public:
  /**
   * @brief Contstructor
   */
  AlosSarLeaderFactory();
  /**
   * @brief Destructor
   */
  ~AlosSarLeaderFactory();


protected:


private:
};
}
#endif
