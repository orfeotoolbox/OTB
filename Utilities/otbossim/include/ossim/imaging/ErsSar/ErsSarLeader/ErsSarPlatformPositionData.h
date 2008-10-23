#ifndef ErsSarPlatformPositionData_h
#define ErsSarPlatformPositionData_h

#include <iostream>
#include <ossim/imaging/ErsSar/ErsSarRecord.h>
#include <ossim/imaging/ErsSar/ErsSarRecordHeader.h>

#include <ossim/imaging/ErsSar/ErsSarLeader/ErsSarPositionVectorRecord.h>
/**
 * @ingroup PlatformPositionDataRecord
 * @brief This class is able to read a Platform position data record
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 02-12-07
 */
class ErsSarPlatformPositionData : public ErsSarRecord
{
public:
	/**
	 * @brief Constructor
	 */
	ErsSarPlatformPositionData();

	/**
	 * @brief Destructor
	 */
	~ErsSarPlatformPositionData();

	/**
	 * @brief Copy constructor
	 */
	ErsSarPlatformPositionData(const ErsSarPlatformPositionData& rhs);

	/**
	 * @brief Copy operator
	 */
	ErsSarPlatformPositionData& operator=(const ErsSarPlatformPositionData& rhs);
	/**
	 * @brief This function write the ErsSar PlatformPositionData in a stream
	 */
	friend std::ostream& operator<<(std::ostream& os, const ErsSarPlatformPositionData& data);

	/**
	 * @brief This function read a ErsSar PlatformPositionData from a stream
	 */
	friend std::istream& operator>>(std::istream& is, ErsSarPlatformPositionData& data);

	/**
	 * @brief This function is able to create a new instance of the class
	 */
	ErsSarRecord* Instanciate()
	{
		return new ErsSarPlatformPositionData();
	};

	/**
	 * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
	 */
	ErsSarRecord* Clone()
	{
		return new ErsSarPlatformPositionData(*this);
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
	 * @brief Orbital elements designator
	 */
	std::string   get_orbit_ele_desg()
	{
		return _orbit_ele_desg;
	};
    /**
	 * @brief Orbital elements
	 */
	double*   get_orbit_ele()
	{
		return _orbit_ele;
	};
    /**
	 * @brief Number of data points
	 */
	int  get_ndata()
	{
		return _ndata;
	};
    /**
	 * @brief Year of data point
	 */
	int   get_year()
	{
		return _year;
	};
    /**
	 * @brief Month of data point
	 */
	int   get_month()
	{
		return _month;
	};
    /**
	 * @brief Day of data point
	 */
	int   get_day()
	{
		return _day;
	};
    /**
	 * @brief Day of year
	 */
	int   get_gmt_day()
	{
		return _gmt_day;
	};
    /**
	 * @brief Seconds of day
	 */
	double   get_gmt_sec()
	{
		return _gmt_sec;
	};
    /**
	 * @brief Data sampling interval
	 */
	double   get_data_int()
	{
		return _data_int;
	};
    /**
	 * @brief Reference coordinate system
	 */
	std::string   get_ref_coord()
	{
		return _ref_coord;
	};
    /**
	 * @brief Greenwich mean hour angle
	 */
	double   get_hr_angle()
	{
		return _hr_angle;
	};
    /**
	 * @brief Along track position error
	 */
	double   get_alt_poserr()
	{
		return _alt_poserr;
	};
    /**
	 * @brief Cross track position error
	 */
	double   get_crt_poserr()
	{
		return _crt_poserr;
	};
    /**
	 * @brief Radial position error
	 */
	double   get_rad_poserr()
	{
		return _rad_poserr;
	};
    /**
	 * @brief Along track velocity error
	 */
	double   get_alt_velerr()
	{
		return _alt_velerr;
	};
    /**
	 * @brief Cross track velocity error
	 */
	double  get_crt_velerr()
	{
		return _crt_velerr;
	};
    /**
	 * @brief Radial velocity error
	 */
	double  get_rad_velerr()
	{
		return _rad_velerr;
	};
    /**
	 * @brief Data point position/velocity
	 */
	ErsSarPositionVectorRecord* get_pos_vect()
	{
		return _pos_vect;
	};
    
protected:
	/**
	 * @brief Orbital elements designator
	 */
	std::string   _orbit_ele_desg;
    /**
	 * @brief Orbital elements
	 */
	double   _orbit_ele[6];
    /**
	 * @brief Number of data points
	 */
	int   _ndata;
    /**
	 * @brief Year of data point
	 */
	int   _year;
    /**
	 * @brief Month of data point
	 */
	int   _month;
    /**
	 * @brief Day of data point
	 */
	int   _day;
    /**
	 * @brief Day of year
	 */
	int   _gmt_day;
    /**
	 * @brief Seconds of day
	 */
	double   _gmt_sec;
    /**
	 * @brief Data sampling interval
	 */
	double   _data_int;
    /**
	 * @brief Reference coordinate system
	 */
	std::string   _ref_coord;
    /**
	 * @brief Greenwich mean hour angle
	 */
	double   _hr_angle;
    /**
	 * @brief Along track position error
	 */
	double   _alt_poserr;
    /**
	 * @brief Cross track position error
	 */
	double   _crt_poserr;
    /**
	 * @brief Radial position error
	 */
	double   _rad_poserr;
    /**
	 * @brief Along track velocity error
	 */
	double   _alt_velerr;
    /**
	 * @brief Cross track velocity error
	 */
	double   _crt_velerr;
    /**
	 * @brief Radial velocity error
	 */
	double   _rad_velerr;
    /**
	 * @brief Data point position/velocity
	 */
	ErsSarPositionVectorRecord _pos_vect[64];
    
private:
};
#endif
