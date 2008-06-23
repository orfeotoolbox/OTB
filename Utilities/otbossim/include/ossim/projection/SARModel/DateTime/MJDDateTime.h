#ifndef MJDDateTime_h
#define MJDDateTime_h

class CivilDateTime;
/**
 * @brief This class represents an MJD date (Modified Julian date)
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
	MJDDateTime(long day, unsigned long  second, unsigned long  microsecond);
	
	/**
	 * @brief Affectation operator
	 */
	MJDDateTime& operator=(const MJDDateTime& rhs);
	
	long get_day()
	{
		return _day;
	};
	
	unsigned long get_second()
	{
		return _second;
	};
	
	unsigned long get_microsecond()
	{
		return _microsecond;
	};
	
	void set_day(long  day)
	{
		_day = day;
	};
	
	void set_second(unsigned long second)
	{
		_second = second;
	};
	
	void set_microsecond(unsigned long microsecond)
	{
		_microsecond = microsecond;
	};
protected:

	long _day;
	unsigned long _second;
	unsigned long _microsecond;
	
	
private:
};
#endif