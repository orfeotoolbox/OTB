#ifndef EnvisatAsarRecordFactory_h
#define EnvisatAsarRecordFactory_h

#include <ossim/imaging/EnvisatAsar/EnvisatAsarRecord.h>
#include <map>
#include <string>

/**
 * @ingroup EnvisatAsar
 * @brief This class is a factory able to construct Record base classes given the id of the record wanted
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 23-11-07
 */
class EnvisatAsarRecordFactory
{
public:
	/**
	 * @brief Constructor
	 */
	EnvisatAsarRecordFactory();
	/**
	 * @brief Destructor
	 */
	~EnvisatAsarRecordFactory();

	/**
	 * @brief Adds a new Record type available in this factory
	 * @param record Record to add in the factory
	 * @param id Id of the new avalaible Record
	 */
	void RegisterRecord(std::string id, EnvisatAsarRecord * record);

	/**
	 * @brief Instanciatse a new Record
	 * @param id Id of the Record we want to instanciate
	 */
	EnvisatAsarRecord* Instanciate(std::string id);

protected:
	/**
	 * @brief Contains all the available Records for the factory
	 */
	std::map<std::string, EnvisatAsarRecord*> _availableRecords;

private:
};

#endif