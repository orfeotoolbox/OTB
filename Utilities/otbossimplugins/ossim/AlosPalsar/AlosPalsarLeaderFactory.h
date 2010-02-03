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

#ifndef AlosPalsarLeaderFactory_h
#define AlosPalsarLeaderFactory_h


#include <AlosPalsar/AlosPalsarRecordFactory.h>
#include <map>

namespace ossimplugins
{

/**
 * @ingroup AlosPalsarLeaderFile
 * @brief This class is a facory able to construct Record base classes
 */
class AlosPalsarLeaderFactory : public AlosPalsarRecordFactory
{
public:
  /**
   * @brief Contstructor
   */
  AlosPalsarLeaderFactory();
  /**
   * @brief Destructor
   */
  ~AlosPalsarLeaderFactory();


protected:


private:
};
}
#endif
