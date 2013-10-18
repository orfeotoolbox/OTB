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

#include <otb/RectangularCoordinate.h>
#include <otb/GeodesicCoordinate.h>

#include <cmath>

namespace ossimplugins
{


RectangularCoordinate::RectangularCoordinate()
{
}

RectangularCoordinate::~RectangularCoordinate()
{
}

RectangularCoordinate::RectangularCoordinate(double x, double y, double z): Coordinate(x,y,z)
{
}

RectangularCoordinate::RectangularCoordinate(const RectangularCoordinate& rhs): Coordinate((Coordinate)rhs)
{
}

RectangularCoordinate& RectangularCoordinate::operator=(const RectangularCoordinate& rhs)
{
  _x = rhs._x;
  _y = rhs._x;
  _z = rhs._x;

  return *this;
}

void RectangularCoordinate::AsGeodesicCoordinates(double demiGdAxe, double demiPtAxe, GeodesicCoordinate* geod)
{
  const double PI          = 3.14159265358979323846 ;
  const int itmax = 40 ;
  const double epsilon = 1.0e-6 ;

  int fin , iter ;
  double u, a2_b2, d, e, f, df, c, s, p, q ;

  double coordCart[3];
  double coordGeod[3];

  coordCart[0] = _x;
  coordCart[1] = _y;
  coordCart[2] = _z;

  u = sqrt (coordCart[0] * coordCart[0] + coordCart[1] * coordCart[1]) ;
  if (u < epsilon)
  {
    coordGeod[0] = 0.0 ;
    if (coordCart[2] >= 0.0)
    coordGeod[1] = PI / 2.0 ;
    else
    coordGeod[1] = - PI / 2.0 ;
    coordGeod[2] = fabs (coordCart[2]) - demiPtAxe ;
  }
  else
  {
    coordGeod[0] = atan2 (coordCart[1] , coordCart[0]) ;
    a2_b2 = demiGdAxe * demiGdAxe - demiPtAxe * demiPtAxe ;
    e     = atan (coordCart[2] / u) ;
    fin   = 0 ;
    iter  = 0 ;
    while (fin == 0)
    {
      iter++ ;
      d  = e ;
      c  = cos (e) ;
      s  = sin (e) ;
      p  = demiGdAxe * u ;
      q  = demiPtAxe * coordCart[2] ;
      f  = p * s - q * c - a2_b2 * s * c ;
      df = p * c + q * s - a2_b2 * (c * c - s * s) ;
      e  = e - f / df ;
      d  = fabs (e - d) ;
      if ((d < epsilon) || (iter >= itmax))
        fin = 1 ;
    }
    coordGeod[1] = atan (tan (e) * demiGdAxe / demiPtAxe) ;

    p = cos(coordGeod[1]) ;

    if (fabs(coordGeod[1]) <= (PI * 0.5))
      coordGeod[2] = (u - demiGdAxe * cos(e)) / cos(coordGeod[1]) ;
    else
      coordGeod[2] = (coordCart[2] - demiPtAxe * sin(e)) / sin(coordGeod[1]) ;

    geod->set_coordinates(coordGeod[0],coordGeod[1], coordGeod[2]);
  }
  //return 0 ;
}
}
