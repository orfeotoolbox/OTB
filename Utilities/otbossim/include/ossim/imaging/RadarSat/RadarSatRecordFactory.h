#ifndef RadarSatRecordFactory_h
#define RadarSatRecordFactory_h


#include <ossim/imaging/RadarSat/RadarSatRecordHeader.h>
#include <ossim/imaging/RadarSat/RadarSatRecord.h>
#include <map>
/**
 * @ingroup RadarSat
 * @brief This class is a factory able to construct Record base classes given the id of the wanted record
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 23-11-07
 */
class RadarSatRecordFactory
{
public:
	/**
	 * @brief Constructor
	 */
	RadarSatRecordFactory();
	/**
	 * @brief Destructor
	 */
	~RadarSatRecordFactory();

	/**
	 * @brief Add a new Record type available in this factory
	 * @param record Record to add in the factory
	 * @param id Id of the new available Record
	 */
	void RegisterRecord(int id, RadarSatRecord * record);

	/**
	 * @brief Instanciates a new Record
	 * @param id Id of the Record we want to instanciate
	 */
	RadarSatRecord* Instanciate(int id) ;
protected:

	/**
	 * @brief Contains all the available Records for the factory
	 */
	std::map<int, RadarSatRecord*> _availableRecords;

private:
};

#endif