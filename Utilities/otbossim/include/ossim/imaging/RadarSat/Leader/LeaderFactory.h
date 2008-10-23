#ifndef LeaderFactory_h
#define LeaderFactory_h


#include <ossim/imaging/RadarSat/RadarSatRecordFactory.h>
#include <map>
/**
 * @ingroup LeaderFile
 * @brief This class is a factory able to construct Record base classes
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 23-11-07
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

#endif