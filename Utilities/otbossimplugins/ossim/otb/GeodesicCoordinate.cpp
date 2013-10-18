//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#include <otb/GeodesicCoordinate.h>

namespace ossimplugins
{



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

}
