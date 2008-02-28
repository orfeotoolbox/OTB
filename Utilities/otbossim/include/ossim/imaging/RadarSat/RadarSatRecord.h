#ifndef RadarSatRecord_h
#define RadarSatRecord_h


#include <ossim/imaging/RadarSat/RadarSatRecordHeader.h>
#include <iostream>
/**
 * @ingroup RadarSat
 * @brief This class is the base class of all the record classes
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 23-11-07
 */
class RadarSatRecord
{
public:
	/**
	 * @brief Constructor
	 * @param mnemonic Name of the record
	 */
	RadarSatRecord(std::string mnemonic);
	/**
	 * @brief Destructor
	 */
	~RadarSatRecord();

	/**
	 * @brief Copy constructor
	 */
	RadarSatRecord(const RadarSatRecord& rhs);

	/**
	 * @brief This function is able to create a new instance of the class
	 */
	virtual RadarSatRecord* Instanciate() =0;

	/**
	 * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
	 */
	virtual RadarSatRecord* Clone()=0;

	/**
	 * @brief Read the class data from a stream
	 */
	virtual void Read(std::istream& is) =0;

	/**
	 * @brief Write the class to a stream
	 */
	virtual void Write(std::ostream& os)=0;

	std::string get_mnemonic()
	{
		return _mnemonic;
	};

protected:

	std::string _mnemonic;
	
private:
};

#endif