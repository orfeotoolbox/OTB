#ifndef MJDDateTime_h
#define MJDDateTime_h

class CivilDateTime;
/**
 * @ingroup Date
 * @brief This class represent a date 
 * @author @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 10-01-08
 */
class MJDDateTime
{
public:
	/**
	 * @brief constructor
	 */
	MJDDateTime();
	/**
	 * @brief Destructor
	 */
	~MJDDateTime();
	/**
	 * @brief Copy constructor
	 */
	MJDDateTime(const MJDDateTime& rhs);
	
	/**
	 * @brief Constructor with date initialisation
	 */
	MJDDateTime(long day, long second, long microsecond);
	
	/**
	 * @brief Affectation operator
	 */
	MJDDateTime& operator=(const MJDDateTime& rhs);
	
	long get_day()
	{
		return _day;
	};
	
	long get_second()
	{
		return _second;
	};
	
	long get_microsecond()
	{
		return _microsecond;
	};
	
	void set_day(long  day)
	{
		_day = day;
	};
	
	void set_second(long second)
	{
		_second = second;
	};
	
	void set_microsecond(long microsecond)
	{
		_microsecond = microsecond;
	};
protected:

	long _day;
	long _second;
	long _microsecond;
	
	
private:
};
#endif