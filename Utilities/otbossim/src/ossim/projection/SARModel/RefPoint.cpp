#include <ossim/projection/SARModel/RefPoint.h>
#include <ossim/projection/SARModel/Ephemeris/Ephemeris.h>
#include <string.h>

RefPoint::RefPoint():
	_ephemeris(NULL),
	_distance(0.0),
	_pix_line(0.0),
	_pix_col(0.0)
{
}

RefPoint::~RefPoint()
{
	if(_ephemeris != NULL)
	{
		delete _ephemeris;
	}
}

RefPoint::RefPoint(const RefPoint& rhs):
	_distance(rhs._distance),
	_pix_line(rhs._pix_line),
	_pix_col(rhs._pix_col)
{
	_ephemeris = rhs._ephemeris->Clone();
}

RefPoint& RefPoint::operator=(const RefPoint& rhs)
{
	_distance = rhs._distance;
	_pix_line = rhs._pix_line;
	_pix_col = rhs._pix_col;
	if(_ephemeris != NULL)
	{
		delete _ephemeris;
		_ephemeris = NULL;
	}
	_ephemeris = rhs._ephemeris->Clone();

	return *this;
}

void RefPoint::set_ephemeris(Ephemeris* ephemeris)
{
	if(_ephemeris != NULL)
	{
		delete _ephemeris;
		_ephemeris = NULL;
	}
	_ephemeris = ephemeris->Clone();
}

void RefPoint::set_distance(double distance)
{
	_distance = distance;
}

void RefPoint::set_pix_line(double pix_line)
{
	_pix_line = pix_line;
}

void RefPoint::set_pix_col(double pix_col)
{
	_pix_col = pix_col;
}

Ephemeris* RefPoint::get_ephemeris()
{
	return _ephemeris;
}

double RefPoint::get_distance()
{
	return _distance;
}

double RefPoint::get_pix_line()
{
	return _pix_line;
}

double RefPoint::get_pix_col()
{
	return _pix_col;
}