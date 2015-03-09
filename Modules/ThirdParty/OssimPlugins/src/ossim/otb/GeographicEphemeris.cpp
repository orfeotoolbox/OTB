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

#include <otb/GeographicEphemeris.h>
#include <otb/GalileanEphemeris.h>

#include <otb/GMSTDateTime.h>

#include <cmath>

namespace ossimplugins
{


GeographicEphemeris::GeographicEphemeris() : Ephemeris()
{
}

GeographicEphemeris::~GeographicEphemeris()
{
}

GeographicEphemeris::GeographicEphemeris(JSDDateTime date, double pos[3], double speed[3]) : Ephemeris(date, pos, speed)
{
}

GeographicEphemeris::GeographicEphemeris(const GeographicEphemeris& rhs) : Ephemeris(rhs)
{
}

GeographicEphemeris& GeographicEphemeris::operator=(const GeographicEphemeris& rhs)
{
  ((Ephemeris)*this) = ((Ephemeris)rhs);
  return *this;
}


void GeographicEphemeris::ToGalilean(GalileanEphemeris* vGal)
{
  const double OMEGATERRE  = 6.28318530717958647693 / 86164.09054 ;
  GMSTDateTime h;
  h.set_origine(GMSTDateTime::AN1950);

  double s,c;
  _date.AsGMSTDateTime(&h) ;

    c = cos (h.get_tms()) ;
    s = sin (h.get_tms()) ;
  vGal->set_date(_date);

  double pos[3];
  double speed[3];

    pos[0] = _position[0] * c - _position[1] * s ;
    pos[1] = _position[0] * s + _position[1] * c ;
    pos[2] = _position[2] ;

    speed[0]  =  _speed[0]  * c -  _speed[1] * s - OMEGATERRE * (_position[0] * s + _position[1] * c) ;
    speed[1]  =  _speed[0]  * s +  _speed[1] * c + OMEGATERRE * (_position[0] * c - _position[1] * s) ;
    speed[2]  = _speed[2] ;

  vGal->set_position(pos);
    vGal->set_speed(speed);
}

GeographicEphemeris::operator GalileanEphemeris()
{
  GalileanEphemeris rhs;
  ToGalilean(&rhs);
  return rhs;
}

GeographicEphemeris::GeographicEphemeris(GalileanEphemeris& rhs)
{
  rhs.ToGeographic(this);
}
}
