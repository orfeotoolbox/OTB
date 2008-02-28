#ifndef DataFactory_h
#define DataFactory_h

#include <ossim/imaging/RadarSat/RadarSatRecordFactory.h>
#include <map>
/**
 * @ingroup DataFile
 * @brief This class is a facory able to construct Record base classes
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 03-12-07
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

#endif