#ifndef JulianData_h
#define JulianData_h

class CivilDateTime;
class GMSTDateTime;
class JSDDateTime;
/**
 * @brief This class represent a date in the julian referential
 * @author @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 10-01-08
 */
class JulianDate
{
public:
	/**
	 * @brief constructor
	 */
	JulianDate();
	/**
	 * @brief Destructor
	 */
	~JulianDate();
	/**
	 * @brief Copy constructor
	 */
	JulianDate(const JulianDate& rhs);
	
	/**
	 * @brief Constructor with date initialisation using a CivilDateTime
	 */
	JulianDate(CivilDateTime& rhs);
	
	/**
	 * @brief Constructor with date initialisation using a JSDDateTime
	 */
	JulianDate(JSDDateTime& rhs);
	
	/**
	 * @brief Constructor with date initialisation
	 */
	JulianDate(double julianDate);
	/**
	 * @brief Affectation operator
	 */
	JulianDate& operator=(const JulianDate& rhs);
	
	void set_julianDate(double julianDate)
	{
		_julianDate = julianDate;
	};
	
	double get_julianDate()
	{
		return _julianDate;
	};
	
	/**
	 * @brief This function convert the current JulianDate in JSDDateTime
	 * @return 0 if no error or an int gretter than 0 indiquating the error
	 * @param JSDdate [out] The classe that receive the result of the convertion of the current JulianDate in JSDDateTime
	 */
	int AsJSDDateTime(JSDDateTime* JSDdate);
	
	/**
	 * @brief This function converts the current JulianDate into GMSTDateTime
	 * @return 0 if no error or an int greater than 0 indicating the error
	 * @param GMSTDateTime [out] The class that receives the result of the conversion of the current JulianDate into GMSTDateTime
	 */
	int AsGMSTDateTime(GMSTDateTime* GMST);

	operator JSDDateTime() ;
	operator GMSTDateTime() ;


protected:

	double _julianDate;
	
private:
};
#endif
