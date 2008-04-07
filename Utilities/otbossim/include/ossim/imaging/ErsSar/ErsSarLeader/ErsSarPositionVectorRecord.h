#ifndef ErsSarPositionVectorRecord_h
#define ErsSarPositionVectorRecord_h

#include <iostream>
#include <ossim/imaging/ErsSar/ErsSarRecord.h>
/**
 * @ingroup PlatformPositionDataRecord
 * @brief This class is able to read a position vector record
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 23-11-07
 */
class ErsSarPositionVectorRecord
{
public:
	/**
	 * @brief Constructor
	 */
	ErsSarPositionVectorRecord();

	/**
	 * @brief Destructor
	 */
	~ErsSarPositionVectorRecord();

	/**
	 * @brief Copy constructor
	 */
	ErsSarPositionVectorRecord(const ErsSarPositionVectorRecord& rhs);

	/**
	 * @brief Copy operator
	 */
	ErsSarPositionVectorRecord& operator=(const ErsSarPositionVectorRecord& rhs);
	/**
	 * @brief This function write the ErsSar PositionVectorRecord in a stream
	 */
	friend std::ostream& operator<<(std::ostream& os, const ErsSarPositionVectorRecord& data);

	/**
	 * @brief This function read a ErsSar PositionVectorRecord from a stream
	 */
	friend std::istream& operator>>(std::istream& is, ErsSarPositionVectorRecord& data);

	/**
	 * @brief Data point position (m)
	 */
	double* get_pos()
	{
		return _pos;
	};

	/** 
	 * @brief Data point velocity (mm/s)
	 */
	double* get_vel()
	{
		return _vel;
	};

protected:
	/**
	 * @brief Data point position (m)
	 */
	double _pos[3];
	/** 
	 * @brief Data point velocity (mm/s)
	 */
	double _vel[3];
private:


};

#endif