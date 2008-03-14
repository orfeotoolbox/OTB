#ifndef SRGRCoefficientSetRecord_h
#define SRGRCoefficientSetRecord_h

#include <iostream>
#include <ossim/imaging/RadarSat/RadarSatRecord.h>

/**
 * @ingroup ProcessingParametersRecord
 * @brief This class is able to read a SRGR coefficients set record
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 30-11-07
 */
class SRGRCoefficientSetRecord
{
public:
	/**
	 * @brief Constructor
	 */
	SRGRCoefficientSetRecord();

	/**
	 * @brief Destructor
	 */
	~SRGRCoefficientSetRecord();

	/**
	 * @brief Copy constructor
	 */
	SRGRCoefficientSetRecord(const SRGRCoefficientSetRecord& rhs);

	/**
	 * @brief Copy operator
	 */
	SRGRCoefficientSetRecord& operator=(const SRGRCoefficientSetRecord& rhs);

	/**
	 * @brief This function write the SRGRCoefficientSetRecord in a stream
	 */
	friend std::ostream& operator<<(std::ostream& os, const SRGRCoefficientSetRecord& data);

	/**
	 * @brief This function read a SRGRCoefficientSetRecord from a stream
	 */
	friend std::istream& operator>>(std::istream& is, SRGRCoefficientSetRecord& data);

	/**
	 * @brief SRGR update date/time
	 */
	std::string   get_srgr_update()
	{
		return _srgr_update;
	}

    /**
	 * @brief SRGR coefficients 
	 */
	double*   get_srgr_coef()
	{
		return _srgr_coef;
	}

protected:
	/**
	 * @brief SRGR update date/time
	 */
	std::string   _srgr_update;

    /**
	 * @brief SRGR coefficients 
	 */
	double   _srgr_coef[6];
private:

};
#endif