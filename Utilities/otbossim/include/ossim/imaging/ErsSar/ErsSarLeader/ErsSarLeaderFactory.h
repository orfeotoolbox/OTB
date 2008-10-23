#ifndef ErsSarLeaderFactory_h
#define ErsSarLeaderFactory_h


#include <ossim/imaging/ErsSar/ErsSarRecordFactory.h>
#include <map>
/**
 * @ingroup ErsSarLeaderFile
 * @brief This class is a facory able to construct Record base classes
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 23-11-07
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
	~ErsSarLeaderFactory();

	
protected:


private:
};

#endif