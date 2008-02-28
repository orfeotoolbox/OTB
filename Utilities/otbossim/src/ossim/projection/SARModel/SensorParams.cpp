#include <ossim/projection/SARModel/SensorParams.h>


SensorParams::SensorParams():
	_prf(0.0),
	_sf(0.0),
	_rwl(0.0),
	_sightDirection(Right),
	_col_direction(1),
	_lin_direction(1)
{
}

SensorParams::~SensorParams()
{
}

SensorParams::SensorParams(const SensorParams& rhs):
	_prf(rhs._prf),
	_sf(rhs._sf),
	_rwl(rhs._rwl),
	_sightDirection(rhs._sightDirection),
	_col_direction(rhs._col_direction),
	_lin_direction(rhs._lin_direction)
{
}

SensorParams& SensorParams::operator=(const SensorParams& rhs)
{
	_prf = rhs._prf;
	_sf = rhs._sf;
	_rwl = rhs._rwl;
	_sightDirection = rhs._sightDirection;
	_col_direction = rhs._col_direction;
	_lin_direction = rhs._lin_direction;
	return *this;
}