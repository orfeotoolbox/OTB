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

#include <otb/JSDDateTime.h>
#include <otb/CivilDateTime.h>
#include <otb/JulianDate.h>
#include <otb/GMSTDateTime.h>

#include <otb/Arithmetic.h>

#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdio>

namespace ossimplugins
{


JSDDateTime::JSDDateTime():
  _day0hTU(0),
  _second(0.0),
  _decimal(0.0)
{
}

JSDDateTime::~JSDDateTime()
{
}

JSDDateTime::JSDDateTime(const JSDDateTime& rhs):
  _day0hTU(rhs._day0hTU),
  _second(rhs._second),
  _decimal(rhs._decimal)
{
}

JSDDateTime::JSDDateTime(JulianDate day0hTU, double second, double decimal):
  _day0hTU(day0hTU),
  _second(second),
  _decimal(decimal)
{
}

JSDDateTime::JSDDateTime(CivilDateTime& rhs)
{
  rhs.AsJSDDateTime(this);
}

JSDDateTime::JSDDateTime(JulianDate& rhs)
{
  rhs.AsJSDDateTime(this);
}


JSDDateTime::JSDDateTime(MJDDateTime& rhs)
{
  CivilDateTime referenceCiv;

  /* Initialisation of the MJD reference day (01/01/2000, 0h00) */
  referenceCiv.set_year(2000);
  referenceCiv.set_month(01);
  referenceCiv.set_day(01);
  referenceCiv.set_second(0);
  referenceCiv.set_decimal(0.0);

  /* JSD day of the ref. MJD date */
  JSDDateTime referenceJSD(referenceCiv);

  /* JSD day computation */
  JulianDate JD((double) rhs.get_day());

  _day0hTU = referenceJSD.get_day0hTU() + JD;
  _second  = referenceJSD.get_second()  + (double) rhs.get_second();
  _decimal = referenceJSD.get_decimal() + 0.000001 * (double) rhs.get_microsecond();

  this->NormDate();
}

JSDDateTime& JSDDateTime::operator=(const JSDDateTime& rhs)
{
  _day0hTU = rhs._day0hTU;
  _second = rhs._second;
  _decimal = rhs._decimal;

  return *this;
}

int JSDDateTime::AsJulianDate(JulianDate* julian)
{
  double jourJulien = _day0hTU.get_julianDate() + (_second + _decimal) / CivilDateTime::JOURCIVIL_LENGTH ;
  julian->set_julianDate(jourJulien);

  return 0 ;
}

int JSDDateTime::AsGMSTDateTime(GMSTDateTime* GMST)
{
  const double TWOPI      = 6.28318530717958647693 ;
  const double OMEGATERRE  = 6.28318530717958647693 / 86164.09054 ;

  int etat ;
  JulianDate jourJulien;
  double  t = 0.0;
        double tsm0hTU = 0.0;
        double tsm = 0.0;
  JSDDateTime date0hTU ;


  date0hTU         = *this ;
  date0hTU.set_second(0);
  date0hTU.set_decimal(0.0);

  etat = date0hTU.AsJulianDate(&jourJulien ) ;

  if (etat == 0)
  {
    if (GMST->get_origine() == GMSTDateTime::AN1900)
    {
      t       = (jourJulien.get_julianDate() - 2415020.0) / 36525.0 ;
      tsm0hTU = 23925.836 + t * (8640184.542 + t * 0.0929) ;
      tsm0hTU = tsm0hTU * TWOPI / CivilDateTime::JOURCIVIL_LENGTH ;
    }
    else if (GMST->get_origine() == GMSTDateTime::AN1950)
    {
      t       = jourJulien.get_julianDate() - 2433281.5 ;
      tsm0hTU = 1.72944494 + t * 6.3003880987 ;
    }
    else if (GMST->get_origine() == GMSTDateTime::AN2000)
    {
      t       = (jourJulien.get_julianDate() - 2451545.0) / 36525.0 ;
      tsm0hTU =   24110.54841 + t * (8640184.812866
                + t * (0.093103 - t * 6.210e-6)) ;
      tsm0hTU = tsm0hTU * TWOPI / CivilDateTime::JOURCIVIL_LENGTH ;
    }
    t = tsm0hTU + (_second + _decimal) * OMEGATERRE ;
    tsm = fmod (t,TWOPI) ;
    if (tsm < 0.0)
    {
      tsm = tsm + TWOPI ;
    }

    GMST->set_tms(tsm);
  }

  return etat ;
}

void JSDDateTime::NormDate()
{
  const double epsilon = 1.0e-12 ;

  int k, iu, iv, iw ;
  double u, v, w, ru, rv, rw ;


  w = _day0hTU.get_julianDate() - 0.5 ;
  if (fabs (w - fnint (w)) < epsilon)
  {
    iw = (int)inint(w) ;
    rw = 0.0 ;
  }
  else
  {
    iw = (int)w ;
    rw = w - (double)iw ;
    if ((iw < 0) && (rw != 0.0))
    {
    iw = iw - 1 ;
    rw = rw + 1.0 ;
    }
  }

  v = _second + rw * CivilDateTime::JOURCIVIL_LENGTH ;
  if (fabs(v - fnint (v)) < epsilon)
  {
    iv = (int)inint (v) ;
    rv = 0.0 ;
  }
  else
  {
    iv = (int)v ;
    rv = v - (double)iv ;
    if ((iv < 0) && (rv != 0.0))
    {
      iv = iv - 1 ;
      rv = rv + 1.0 ;
    }
  }

  u  = _decimal + rv ;
  iu = (int)u ;
  ru = u - (double)iu ;
  if ((iu < 0) && (ru != 0.0))
  {
    iu = iu - 1 ;
    ru = ru + 1.0 ;
  }

  iv = iv + iu ;
  k  = iv / ((int)inint(CivilDateTime::JOURCIVIL_LENGTH)) ;
  iw = iw + k ;
  iv = iv - k * ((int)inint(CivilDateTime::JOURCIVIL_LENGTH)) ;
  if (iv < 0)
  {
    iw = iw - 1 ;
    iv = iv + (int)inint(CivilDateTime::JOURCIVIL_LENGTH) ;
  }

  _decimal    = ru ;
  _second  = (double)iv ;
  _day0hTU = (double)iw + 0.5 ;
}

JSDDateTime::operator JulianDate()
{
  JulianDate date;
  this->AsJulianDate(&date);
  return date;
}

JSDDateTime::operator GMSTDateTime()
{
  GMSTDateTime date;
  this->AsGMSTDateTime(&date);
  return date;
}
}
