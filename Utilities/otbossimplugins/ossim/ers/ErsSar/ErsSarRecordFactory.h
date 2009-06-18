#ifndef ErsSarRecordFactory_h
#define ErsSarRecordFactory_h


#include <ossim/imaging/ErsSar/ErsSarRecordHeader.h>
#include <ossim/imaging/ErsSar/ErsSarRecord.h>
#include <map>
/**
 * @ingroup ErsSar
 * @brief This class is a facory able to construct Record base classes given the id of the record wanted
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 23-11-07
 */
class ErsSarRecordFactory
{
public:
	/**
	 * @brief Constructor
	 */
	ErsSarRecordFactory();
	/**
	 * @brief Destructor
	 */
	~ErsSarRecordFactory();

	/**
	 * @brief Add a new Record type available in this factory
	 * @param record Record to add in the factory
	 * @param id Id of the new avalaible Record
	 */
	void RegisterRecord(int id, ErsSarRecord * record);

	/**
	 * @brief Instanciate a new Record
	 * @param id Id of the Record we want to instanciate
	 */
	ErsSarRecord* Instanciate(int id) ;
protected:

	/**
	 * @brief Contain all the available Records for the factory
	 */
	std::map<int, ErsSarRecord*> _availableRecords;

private:
};

#endif