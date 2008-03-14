#ifndef DataHistogramSignalData_h
#define DataHistogramSignalData_h

#include <iostream>

#include <ossim/imaging/RadarSat/RadarSatRecord.h>
#include <ossim/imaging/RadarSat/RadarSatRecordHeader.h>
#include <ossim/imaging/RadarSat/Leader/DataHistogramRecord.h>
/**
 * @ingroup SignalDataHistogramRecord
 * @brief This class is able to read a data histogram record - Signal data
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 23-11-07
 */
class DataHistogramSignalData : public RadarSatRecord
{
public:
	/**
	 * @brief Constructor
	 */
	DataHistogramSignalData();

	/**
	 * @brief Destructor
	 */
	~DataHistogramSignalData();

	/**
	 * @brief Copy constructor
	 */
	DataHistogramSignalData(const DataHistogramSignalData& rhs);

	/**
	 * @brief Copy operator
	 */
	DataHistogramSignalData& operator=(const DataHistogramSignalData& rhs);
	/**
	 * @brief This function write the DataHistogramSignalData in a stream
	 */
	friend std::ostream& operator<<(std::ostream& os, const DataHistogramSignalData& data);

	/**
	 * @brief This function read a DataHistogramSignalData from a stream
	 */
	friend std::istream& operator>>(std::istream& is, DataHistogramSignalData& data);

	/**
	 * @brief This function is able to create a new instance of the class
	 */
	RadarSatRecord* Instanciate()
	{
		return new DataHistogramSignalData();
	};

	/**
	 * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
	 */
	RadarSatRecord* Clone()
	{
		return new DataHistogramSignalData(*this);
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
	int   get_rec_seq()
	{
		return _rec_seq;
	};
	/**
	 * @brief SAR channel indicator
	 */
	int   get_sar_chn()
	{
		return _sar_chn;
	};

	/**
	 * @brief Number of histogram table data sets in this record
	 */
	int   get_ntab()
	{
		return _ntab;
	};

	/**
	 * @brief Histogram table data set size
	 */
	int   get_ltab()
	{
		return _ltab;
	};

	/**
	 * @brief Histogram data record
	 */
	DataHistogramRecord get_histogram()
	{
		return _histogram;
	};

protected:
	/**
	 * @brief Record sequence number
	 */
	int   _rec_seq;
	/**
	 * @brief SAR channel indicator
	 */
	int   _sar_chn;

	/**
	 * @brief Number of histogram table data sets in this record
	 */
	int   _ntab;
	/**
	 * @brief Histogram table data set size
	 */
	int   _ltab;
	/**
	 * @brief Histogram data record
	 */
	DataHistogramRecord _histogram;

private:


};

#endif