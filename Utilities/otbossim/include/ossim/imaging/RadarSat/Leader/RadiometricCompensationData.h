#ifndef RadiometricCompensationData_h
#define RadiometricCompensationData_h

#include <iostream>

#include <ossim/imaging/RadarSat/RadarSatRecord.h>
#include <ossim/imaging/RadarSat/RadarSatRecordHeader.h>

#include <ossim/imaging/RadarSat/Leader/CompensationDataRecord.h>
/**
 * @ingroup RadiometricCompensationDataRecord
 * @brief This class is able to read a Radiometric Compensation data record
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 03-12-07
 */
class RadiometricCompensationData : public RadarSatRecord
{
public:
	/**
	 * @brief Constructor
	 */
	RadiometricCompensationData();

	/**
	 * @brief Destructor
	 */
	~RadiometricCompensationData();

	/**
	 * @brief Copy constructor
	 */
	RadiometricCompensationData(const RadiometricCompensationData& rhs);

	/**
	 * @brief Copy operator
	 */
	RadiometricCompensationData& operator=(const RadiometricCompensationData& rhs);
	/**
	 * @brief This function write the RadiometricCompensationData in a stream
	 */
	friend std::ostream& operator<<(std::ostream& os, const RadiometricCompensationData& data);

	/**
	 * @brief This function read a RadiometricCompensationData from a stream
	 */
	friend std::istream& operator>>(std::istream& is, RadiometricCompensationData& data);

	/**
	 * @brief This function is able to create a new instance of the class
	 */
	RadarSatRecord* Instanciate()
	{
		return new RadiometricCompensationData();
	};

	/**
	 * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
	 */
	RadarSatRecord* Clone()
	{
		return new RadiometricCompensationData(*this);
	};

	/**
	 * @brief Read the class data from a stream
	 */
	void Read(std::istream& is)
	{
		is>>*this;
	};

	/**
	 * @brief Write the class to a stream
	 */
	void Write(std::ostream& os)
	{
		os<<*this;
	};

	/**
	 * @brief Record sequence number
	 */
	int  get_seq_num()
	{
		return _seq_num;
	};
	/**
	 * @brief SAR channel indicator
	 */
	int   get_chan_ind()
	{
		return _chan_ind;
	};
    /**
	 * @brief Number of data sets in record
	 */
	int   get_n_dset()
	{
		return _n_dset;
	};
    /**
	 * @brief Compensation data set size
	 */
	int   get_dset_size()
	{
		return _dset_size;
	};
	/**
	 * @brief Compensation data set records
	 */
	CompensationDataRecord* get_dset()
	{
		return _dset;
	};

protected:
	/**
	 * @brief Record sequence number
	 */
	int   _seq_num;
	/**
	 * @brief SAR channel indicator
	 */
	int   _chan_ind;
    /**
	 * @brief Number of data sets in record
	 */
	int   _n_dset;
    /**
	 * @brief Compensation data set size
	 */
	int   _dset_size;
	/**
	 * @brief Compensation data set records
	 */
	CompensationDataRecord _dset[4];
private:
};
#endif
