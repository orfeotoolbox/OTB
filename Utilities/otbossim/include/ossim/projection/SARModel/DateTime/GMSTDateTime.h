#ifndef GMSTDateTime_h
#define GMSTDateTime_h
#include <ossim/projection/SARModel/DateTime/JulianDate.h>

class CivilDateTime;
class JulianDate;

/**
 * @ingroup Date
 * @brief This class represent a date 
 * @author @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 10-01-08
 */
class GMSTDateTime
{
public:

	enum Ori_TSM_Green { AN1900 , AN1950 , AN2000 } ;
	
	/**
	 * @brief constructor
	 */
	GMSTDateTime();
	/**
	 * @brief Destructor
	 */
	~GMSTDateTime();
	/**
	 * @brief Copy constructor
	 */
	GMSTDateTime(const GMSTDateTime& rhs);
	
	/**
	 * @brief Constructor with date initialisation
	 */
	GMSTDateTime(double tsm, Ori_TSM_Green origine);
	
	/**
	 * @brief Constructor with date initialisation using a CivilDateTime
	 */
	GMSTDateTime(CivilDateTime& rhs);
	
	/**
	 * @brief Affectation operator
	 */
	GMSTDateTime& operator=(const GMSTDateTime& rhs);
	
	double get_tms()
	{
		return _tms;
	};
	
	void set_tms(double  tms)
	{
		_tms = tms;
	};
	
	Ori_TSM_Green get_origine()
	{
		return _origine;
	};
	
	void set_origine(Ori_TSM_Green origine)
	{
		_origine = origine;
	}
protected:

	double _tms;	
	Ori_TSM_Green _origine;
	
private:
};
#endif