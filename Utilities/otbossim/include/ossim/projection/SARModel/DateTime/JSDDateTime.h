#ifndef JSDDateTime_h
#define JSDDateTime_h
#include <ossim/projection/SARModel/DateTime/JulianDate.h>
#include <ossim/projection/SARModel/DateTime/MJDDateTime.h>

class CivilDateTime;
class GMSTDateTime;
class JulianDate;

/**
 * @brief This class represents a date 
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
	 * @brief Constructor with date initialisation using a MJDDateTime (Modified Julian Date - Used for ENVISAT)
	 */
	JSDDateTime(MJDDateTime& rhs);

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
	
	void NormDate() ;

	/**
	 * @brief This function converts the current JSDDateTime into Julian Date
	 * @return 0 if no error or an int greater than 0 indicating the error
	 * @param julianDate [out] The classe that receives the result of the conversion of the current JSDDateTime in JulianDate
	 */
	int AsJulianDate(JulianDate* julian);

	/**
	 * @brief This function converts the current JSDDateTime into GMST Date
	 * @return 0 if no error or an int greater than 0 indicating the error
	 * @param julianDate [out] The class that receives the result of the conversion of the current JSDDateTime into GMST Date
	 */
	int JSDDateTime::AsGMSTDateTime(GMSTDateTime* GMST);

	operator JulianDate() ;
	operator GMSTDateTime() ;

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