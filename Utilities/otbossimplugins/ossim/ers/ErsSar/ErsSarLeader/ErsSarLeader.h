#ifndef ErsSarLeader_h
#define ErsSarLeader_h

#include <iostream>
#include "ErsSarFileDescriptor.h"
#include "ErsSarDataSetSummary.h"
#include "ErsSarMapProjectionData.h"
#include "ErsSarPlatformPositionData.h"
#include "ErsSarFacilityData.h"
#include <map>

#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>

namespace ossimplugins
{

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

  /**
   * @brief Method to save object state to a keyword list.
   * @param kwl Keyword list to save to.
   * @param prefix added to keys when saved.
   * @return true on success, false on error.
   */
  virtual bool saveState(ossimKeywordlist& kwl,
                         const char* prefix=0) const;

	ErsSarFacilityData * get_ErsSarFacilityData() const;
	ErsSarPlatformPositionData * get_ErsSarPlatformPositionData() const;
	ErsSarMapProjectionData * get_ErsSarMapProjectionData() const;
	ErsSarDataSetSummary * get_ErsSarDataSetSummary() const;
	ErsSarFileDescriptor * get_ErsSarFileDescriptor() const;

protected:
  // Made mutable because the get_* methods need to access it.
  // (modifying the definition of the map might be another solution)
	mutable std::map<int, ErsSarRecord*> _records;

	static const int ErsSarFacilityDataID;
	static const int ErsSarPlatformPositionDataID;
	static const int ErsSarMapProjectionDataID;
	static const int ErsSarDataSetSummaryID;
	static const int ErsSarFileDescriptorID;
private:

};
}

#endif
