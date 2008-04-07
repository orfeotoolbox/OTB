#include <ossim/projection/SARModel/Ephemeris/Ephemeris.h>

Ephemeris::Ephemeris()
{
	_position[0] = 0.0;
	_position[1] = 0.0;
	_position[2] = 0.0;
	_vitesse[0] = 0.0;
	_vitesse[1] = 0.0;
	_vitesse[2] = 0.0;
}

Ephemeris::~Ephemeris()
{
}

Ephemeris::Ephemeris(JSDDateTime date, double pos[3], double vitesse[3]):
	_date(date)
{
	_position[0] = pos[0];
	_position[1] = pos[1];
	_position[2] = pos[2];
	_vitesse[0] = vitesse[0];
	_vitesse[1] = vitesse[1];
	_vitesse[2] = vitesse[2];
}

Ephemeris::Ephemeris(const Ephemeris& rhs):
	_date(rhs._date)
{
	_position[0] = rhs._position[0];
	_position[1] = rhs._position[1];
	_position[2] = rhs._position[2];
	_vitesse[0] = rhs._vitesse[0];
	_vitesse[1] = rhs._vitesse[1];
	_vitesse[2] = rhs._vitesse[2];
}

Ephemeris& Ephemeris::operator=(const Ephemeris& rhs)
{
	_position[0] = rhs._position[0];
	_position[1] = rhs._position[1];
	_position[2] = rhs._position[2];
	_vitesse[0] = rhs._vitesse[0];
	_vitesse[1] = rhs._vitesse[1];
	_vitesse[2] = rhs._vitesse[2];
	_date = rhs._date;
	
	return *this;
}
