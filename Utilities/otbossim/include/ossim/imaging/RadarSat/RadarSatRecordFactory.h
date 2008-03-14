#ifndef RadarSatRecordFactory_h
#define RadarSatRecordFactory_h


#include <ossim/imaging/RadarSat/RadarSatRecordHeader.h>
#include <ossim/imaging/RadarSat/RadarSatRecord.h>
#include <map>
/**
 * @ingroup RadarSat
 * @brief This class is a facory able to construct Record base classes given the id of the record wanted
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
	 * @param id Id of the new avalaible Record
	 */
	void RegisterRecord(int id, RadarSatRecord * record);

	/**
	 * @brief Instanciate a new Record
	 * @param id Id of the Record we want to instanciate
	 */
	RadarSatRecord* Instanciate(int id) ;
protected:

	/**
	 * @brief Contain all the available Records for the factory
	 */
	std::map<int, RadarSatRecord*> _availableRecords;

private:
};

#endif