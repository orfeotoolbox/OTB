#include <ossim/projection/SARModel/DateTime/GMSTDateTime.h>
#include <ossim/projection/SARModel/DateTime/CivilDateTime.h>

GMSTDateTime::GMSTDateTime():
	_tms(0)
{
}

GMSTDateTime::~GMSTDateTime()
{
}

GMSTDateTime::GMSTDateTime(const GMSTDateTime& rhs):
	_tms(rhs._tms),
	_origine(rhs._origine)
{
}
	
GMSTDateTime::GMSTDateTime(double tms, Ori_TSM_Green origine):
	_tms(tms),
	_origine(origine)
{
}
	
GMSTDateTime::GMSTDateTime(CivilDateTime& rhs)
{
	rhs.AsGMSTDateTime(this);
}

GMSTDateTime& GMSTDateTime::operator=(const GMSTDateTime& rhs)
{
	_tms = rhs._tms;
	_origine = rhs._origine;
	return *this;
}
