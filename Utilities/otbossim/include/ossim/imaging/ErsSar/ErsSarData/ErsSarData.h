#ifndef ErsSarData_h
#define ErsSarData_h

#include <iostream>
#include <map>
#include <ossim/imaging/ErsSar/ErsSarRecord.h>

class ErsSarImageOptionsFileDescriptor;
/**
 * @ingroup DataFile
 * @brief This class is able to read the data file of the RadarSat file structure
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 23-11-07
 */
class ErsSarData
{
public:
	/**
	 * @brief Constructor
	 */
	ErsSarData();

	/**
	 * @brief Destructor
	 */
	~ErsSarData();

	/**
	 * @brief This function write the Data in a stream
	 */
	friend std::ostream& operator<<(std::ostream& os, const ErsSarData& data);

	/**
	 * @brief This function read a Data from a stream
	 */
	friend std::istream& operator>>(std::istream& is, ErsSarData& data);

	/**
	 * @brief Copy constructor
	 */
	ErsSarData(const ErsSarData& rhs);

	/**
	 * @brief Copy operator
	 */
	ErsSarData& operator=(const ErsSarData& rhs);

	/**
	 * @brief Remove all the previous records from the Data
	 */
	void ClearRecords();

	/**
	 * @brief Insert an existing record in the Data
	 */
	void InsertRecord(int id, ErsSarRecord* record);

	ErsSarImageOptionsFileDescriptor* get_ErsSarImageOptionsFileDescriptor();


protected:

	std::map<int, ErsSarRecord*> _records;
	static const int ErsSarImageOptionsFileDescriptorID;
private:

};

#endif