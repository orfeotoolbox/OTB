/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


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
