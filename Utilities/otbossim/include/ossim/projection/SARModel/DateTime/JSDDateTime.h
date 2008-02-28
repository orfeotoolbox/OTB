#ifndef JSDDateTime_h
#define JSDDateTime_h
#include <ossim/projection/SARModel/DateTime/JulianDate.h>

class CivilDateTime;
class JulianDate;
class GMSTDateTime;

/**
 * @ingroup Date
 * @brief This class represent a date 
 * @author @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 10-01-08
 */
class JSDDateTime
{
public:
	/**
	 * @brief constructor
	 */
	JSDDateTime();
	/**
	 * @brief Destructor
	 */
	~JSDDateTime();
	/**
	 * @brief Copy constructor
	 */
	JSDDateTime(const JSDDateTime& rhs);
	
	/**
	 * @brief Constructor with date initialisation
	 */
	JSDDateTime(JulianDate day0hTU, double second, double decimal);
	
	/**
	 * @brief Constructor with date initialisation using a CivilDateTime
	 */
	JSDDateTime(CivilDateTime& rhs);
	
	/**
	 * @brief Constructor with date initialisation using a CivilDateTime
	 */
	JSDDateTime(JulianDate& rhs);
	
	/**
	 * @brief Affectation operator
	 */
	JSDDateTime& operator=(const JSDDateTime& rhs);
	
	JulianDate get_day0hTU()
	{
		return _day0hTU;
	};
	
	double get_second()
	{
		return _second;
	};
	
	double get_decimal()
	{
		return _decimal;
	};
	
	void set_day0hTU(JulianDate  day)
	{
		_day0hTU = day;
	};
	
	void set_second(double second)
	{
		_second = second;
	};
	
	void set_decimal(double decimal)
	{
		_decimal = decimal;
	};
	
	/**
	 * @brief This function convert the current JSDDateTime in Julian Date
	 * @return 0 if no error or an int gretter than 0 indiquating the error
	 * @param julian [out] The classe that receive the result of the convertion of the current JSDDateTime in JulianDate
	 */
	int AsJulianDate(JulianDate* julian);
	
	/**
	 * @brief This function convert the current JSDDateTime in GMSTDateTime
	 * @return 0 if no error or an int gretter than 0 indiquating the error
	 * @param tms [out] The classe that receive the result of the convertion of the current CivilDateTime in GMSTDateTime
	 */
	int AsGMSTDateTime(GMSTDateTime* tms);
	
	/**
	 * @brief This fonction normalized the date
	 */
	void NormDate();

	/**
	 * @brief Cast operator JSDDateTime to JulianDate
	 *
	 * This fonction only call the function AsJulianDate
	 * @see AsJulianDate;
	 */
	operator JulianDate();

	/**
	 * @brief Cast operator JSDDateTime to GMSTDateTime
	 *
	 * This fonction only call the function AsGMSTDateTime
	 * @see AsGMSTDateTime;
	 */
	operator GMSTDateTime();
protected:

	/**
	 * @brief Julian day at 0h00 UT
	 */
	JulianDate _day0hTU;
	double _second;
	double _decimal;
	
	
private:
};
#endif