#ifndef ErsSarLeader_h
#define ErsSarLeader_h

#include <iostream>
#include "ErsSarFileDescriptor.h"
#include "ErsSarDataSetSummary.h"
#include "ErsSarMapProjectionData.h"
#include "ErsSarPlatformPositionData.h"
#include "ErsSarFacilityData.h"
#include <map>

class ErsSarPlatformPositionData;
class ErsSarMapProjectionData;
class ErsSarDataSetSummary;
class ErsSarFileDescriptor;
class ErsSarFacilityData;

/**
 * @ingroup ErsSarLeaderFile
 * @brief This class is able to read the Leader file of the ErsSar file structure
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 23-11-07
 */
class ErsSarLeader
{
public:
	/**
	 * @brief Constructor
	 */
	ErsSarLeader();

	/**
	 * @brief Destructor
	 */
	~ErsSarLeader();

	/**
	 * @brief This function write the ErsSarLeader in a stream
	 */
	friend std::ostream& operator<<(std::ostream& os, const ErsSarLeader& data);

	/**
	 * @brief This function read a ErsSarLeader from a stream
	 */
	friend std::istream& operator>>(std::istream& is, ErsSarLeader& data);

	/**
	 * @brief Copy constructor
	 */
	ErsSarLeader(const ErsSarLeader& rhs);

	/**
	 * @brief Copy operator
	 */
	ErsSarLeader& operator=(const ErsSarLeader& rhs);

	/**
	 * @brief Remove all the previous records from the ErsSarLeader
	 */
	void ClearRecords();
	ErsSarFacilityData * get_ErsSarFacilityData();
	ErsSarPlatformPositionData * get_ErsSarPlatformPositionData();
	ErsSarMapProjectionData * get_ErsSarMapProjectionData();
	ErsSarDataSetSummary * get_ErsSarDataSetSummary();
	ErsSarFileDescriptor * get_ErsSarFileDescriptor();

protected:
	std::map<int, ErsSarRecord*> _records;

	static const int ErsSarFacilityDataID;
	static const int ErsSarPlatformPositionDataID;
	static const int ErsSarMapProjectionDataID;
	static const int ErsSarDataSetSummaryID;
	static const int ErsSarFileDescriptorID;
private:

};

#endif