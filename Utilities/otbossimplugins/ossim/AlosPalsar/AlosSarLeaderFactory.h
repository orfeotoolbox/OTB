#ifndef AlosSarLeaderFactory_h
#define AlosSarLeaderFactory_h


#include <AlosSarRecordFactory.h>
#include <map>

namespace ossimplugins
{

/**
 * @ingroup AlosSarLeaderFile
 * @brief This class is a facory able to construct Record base classes
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 23-11-07
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
