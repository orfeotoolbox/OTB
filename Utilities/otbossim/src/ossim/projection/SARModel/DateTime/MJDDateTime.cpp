#include <ossim/projection/SARModel/DateTime/MJDDateTime.h>

MJDDateTime::MJDDateTime():
	_day(0),
	_second(0),
	_microsecond(0)
{
}

MJDDateTime::~MJDDateTime()
{
}

MJDDateTime::MJDDateTime(const MJDDateTime& rhs):
	_day(rhs._day),
	_second(rhs._second),
	_microsecond(rhs._microsecond)
{
}
	
MJDDateTime::MJDDateTime(long day, long second, long microsecond):
	_day(day),
	_second(second),
	_microsecond(microsecond)
{
}
	
MJDDateTime& MJDDateTime::operator=(const MJDDateTime& rhs)
{
	_day = rhs._day;
	_second = rhs._second;
	_microsecond = rhs._microsecond;

	return *this;
}