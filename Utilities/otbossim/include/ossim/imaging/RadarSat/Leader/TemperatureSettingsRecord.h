#ifndef TemperatureSettingsRecord_h
#define TemperatureSettingsRecord_h

#include <iostream>
#include <ossim/imaging/RadarSat/RadarSatRecord.h>

/**
 * @ingroup ProcessingParametersRecord
 * @brief This class is able to read a Temperature Settings record
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 30-11-07
 */
class TemperatureSettingsRecord
{
public:
	/**
	 * @brief Constructor
	 */
	TemperatureSettingsRecord();

	/**
	 * @brief Destructor
	 */
	~TemperatureSettingsRecord();

	/**
	 * @brief Copy constructor
	 */
	TemperatureSettingsRecord(const TemperatureSettingsRecord& rhs);

	/**
	 * @brief Copy operator
	 */
	TemperatureSettingsRecord& operator=(const TemperatureSettingsRecord& rhs);

	/**
	 * @brief This function write the TemperatureSettingsRecord in a stream
	 */
	friend std::ostream& operator<<(std::ostream& os, const TemperatureSettingsRecord& data);

	/**
	 * @brief This function read a TemperatureSettingsRecord from a stream
	 */
	friend std::istream& operator>>(std::istream& is, TemperatureSettingsRecord& data);

	/**
	 * @brief Temperature settings
	 */
	int*   get_temp_set()
	{
		return _temp_set;
	};

protected:

	/**
	 * @brief Temperature settings
	 */
	int _temp_set[4];
private:

};
#endif