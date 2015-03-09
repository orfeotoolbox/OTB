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

#include <otb/CivilDateTime.h>
#include <otb/JulianDate.h>
#include <otb/JSDDateTime.h>
#include <otb/GMSTDateTime.h>


#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdio>

namespace ossimplugins
{


JulianDate::JulianDate():
  _julianDate(0.0)
{
}

JulianDate::JulianDate(CivilDateTime& rhs)
{
  rhs.AsJulianDate(this);
}

JulianDate::JulianDate(double julianDate):
  _julianDate(julianDate)
{
}

JulianDate::JulianDate(JSDDateTime& rhs)
{
  rhs.AsJulianDate(this);
}

JulianDate::~JulianDate()
{
}

JulianDate::JulianDate(const JulianDate& rhs):
  _julianDate(rhs._julianDate)
{
}

JulianDate& JulianDate::operator=(const JulianDate& rhs)
{
  _julianDate = rhs._julianDate;
  return *this;
}

JulianDate& JulianDate::operator+(const JulianDate& rhs)
{
  _julianDate += rhs._julianDate;
  return *this;
}

int JulianDate::AsJSDDateTime(JSDDateTime* JSDdate)
{
  int k ;
  double r ;

  k = (int)(_julianDate - 0.5) ;
  double jJul0hTU = (double)k + 0.5;

  JSDdate->set_day0hTU (jJul0hTU);
  r = (_julianDate - jJul0hTU) * CivilDateTime::JOURCIVIL_LENGTH ;
  JSDdate->set_second(floor (r));
  JSDdate->set_decimal(r - JSDdate->get_second()) ;

  return 0 ;
}

int JulianDate::AsGMSTDateTime(GMSTDateTime* GMST)
{
  const double TWOPI      = 6.28318530717958647693 ;
  const double OMEGATERRE  = 6.28318530717958647693 / 86164.09054 ;

  int etat=0 , n ;
  double j0hTU = 0.0;
        double tsm0hTU = 0.0;
        double t = 0.0;
        double d = 0.0;
        double tsm = 0.0;


  n     = (int)(_julianDate - 0.5) ;
  j0hTU = (double)n + 0.5 ;
  d     = _julianDate - j0hTU ;

  if (etat == 0)
  {
    if (GMST->get_origine() == GMSTDateTime::AN1900)
    {
      t       = (j0hTU - 2415020.0) / 36525.0 ;
      tsm0hTU = 23925.836 + t * (8640184.542 + t * 0.0929) ;
      tsm0hTU = tsm0hTU * TWOPI / CivilDateTime::JOURCIVIL_LENGTH ;
    }
    else if (GMST->get_origine() == GMSTDateTime::AN1950)
    {
      t       = j0hTU - 2433281.5 ;
      tsm0hTU = 1.72944494 + t * 6.3003880987 ;
    }
    else if (GMST->get_origine() == GMSTDateTime::AN2000)
    {
      t       = (j0hTU - 2451545.0) / 36525.0 ;
      tsm0hTU =   24110.54841 + t * (8640184.812866 + t * (0.093103 - t * 6.210e-6)) ;
      tsm0hTU = tsm0hTU * TWOPI / CivilDateTime::JOURCIVIL_LENGTH ;
    }
    t = tsm0hTU + d * CivilDateTime::JOURCIVIL_LENGTH * OMEGATERRE ;
    tsm = fmod (t,TWOPI) ;
    if (tsm < 0.0)
    {
      tsm = tsm + TWOPI ;
    }

    GMST->set_tms(tsm);
  }

  return etat ;
}

JulianDate::operator JSDDateTime()
{
  JSDDateTime date;
  AsJSDDateTime(&date);
  return date;
}

JulianDate::operator GMSTDateTime()
{
  GMSTDateTime date;
  AsGMSTDateTime(&date);
  return date;
}
}
