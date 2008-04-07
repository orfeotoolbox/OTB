#include <ossim/projection/SARModel/Coordinates/GeodesicCoordinate.h>


GeodesicCoordinate::GeodesicCoordinate()
{
}

GeodesicCoordinate::~GeodesicCoordinate()
{
}

GeodesicCoordinate::GeodesicCoordinate(double x, double y, double z): Coordinate(x,y,z)
{
}

GeodesicCoordinate::GeodesicCoordinate(const GeodesicCoordinate& rhs): Coordinate((Coordinate)rhs)
{
}
	
GeodesicCoordinate& GeodesicCoordinate::operator=(const GeodesicCoordinate& rhs)
{
	_x = rhs._x;
	_y = rhs._x;
	_z = rhs._x;
	
	return *this;
}
	