#ifndef Data_h
#define Data_h

#include <iostream>
#include <map>
#include <ossim/imaging/RadarSat/RadarSatRecord.h>

class ImageOptionsFileDescriptor;
/**
 * @ingroup DataFile
 * @brief This class is able to read the data file of the RadarSat file structure
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 23-11-07
 */
class Data
{
public:
	/**
	 * @brief Constructor
	 */
	Data();

	/**
	 * @brief Destructor
	 */
	~Data();

	/**
	 * @brief This function write the Data in a stream
	 */
	friend std::ostream& operator<<(std::ostream& os, const Data& data);

	/**
	 * @brief This function read a Data from a stream
	 */
	friend std::istream& operator>>(std::istream& is, Data& data);

	/**
	 * @brief Copy constructor
	 */
	Data(const Data& rhs);

	/**
	 * @brief Copy operator
	 */
	Data& operator=(const Data& rhs);

	/**
	 * @brief Remove all the previous records from the Data
	 */
	void ClearRecords();

	/**
	 * @brief Insert an existing record in the Data
	 */
	void InsertRecord(int id, RadarSatRecord* record);

	ImageOptionsFileDescriptor* get_ImageOptionsFileDescriptor();
protected:
	std::map<int, RadarSatRecord*> _records;
	static const int ImageOptionsFileDescriptorID;
private:

};

#endif