#include <ossim/projection/SARModel/Coordinates/Coordinate.h>


Coordinate::Coordinate():
	_x(0.0),
	_y(0.0),
	_z(0.0)
{
}

Coordinate::~Coordinate()
{
}

Coordinate::Coordinate(double x, double y, double z):
	_x(x),
	_y(y),
	_z(z)
{
}

Coordinate::Coordinate(const Coordinate& rhs):
	_x(rhs._x),
	_y(rhs._y),
	_z(rhs._z)
{
}
	
Coordinate& Coordinate::operator=(const Coordinate& rhs)
{
	_x = rhs._x;
	_y = rhs._y;
	_z = rhs._z;
	
	return *this;
}

void Coordinate::set_coordinates(double x, double y, double z)
{
	_x = x;
	_y = y;
	_z = z;

}