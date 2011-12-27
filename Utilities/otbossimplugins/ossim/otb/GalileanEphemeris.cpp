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

#include <otb/GalileanEphemeris.h>
#include <otb/GeographicEphemeris.h>

#include <otb/GMSTDateTime.h>

#include <cmath>

namespace ossimplugins
{


GalileanEphemeris::GalileanEphemeris()
{

}

GalileanEphemeris::~GalileanEphemeris()
{

}

GalileanEphemeris::GalileanEphemeris(JSDDateTime date, double pos[3], double speed[3]) : Ephemeris(date, pos, speed)
{

}

GalileanEphemeris::GalileanEphemeris(const GalileanEphemeris& rhs) : Ephemeris(rhs)
{

}

GalileanEphemeris& GalileanEphemeris::operator=(const GalileanEphemeris& rhs)
{
  ((Ephemeris)*this) = ((Ephemeris)rhs);
  return *this;
}

void GalileanEphemeris::ToGeographic(GeographicEphemeris* vGeo)
{
  const double OMEGATERRE  = 6.28318530717958647693 / 86164.09054 ;
  GMSTDateTime h;
  h.set_origine(GMSTDateTime::AN1950);
  double s,c;

  double pos[3];
  double speed[3];

  _date.AsGMSTDateTime(&h) ;
    c = cos (h.get_tms()) ;
    s = sin (h.get_tms()) ;
  vGeo->set_date(_date);

    pos[0] = _position[0]   * c + _position[1] * s ;
    pos[1] = - _position[0] * s + _position[1] * c ;
    pos[2] = _position[2] ;
    speed[0]  =   _speed[0]  * c +  _speed[1] * s - OMEGATERRE * (_position[0] * s - _position[1] * c) ;
    speed[1]  = - _speed[0]  * s +  _speed[1] * c - OMEGATERRE * (_position[0] * c + _position[1] * s) ;
    speed[2]  = _speed[2] ;

    vGeo->set_position(pos);
    vGeo->set_speed(speed);
}

void GalileanEphemeris::ToGeographic(double greenwich,GeographicEphemeris* vGeo)
{
  const double OMEGATERRE  = 6.28318530717958647693 / 86164.09054 ;
  int etat, ierr ;
  double p[10],pd[10];
  double day;

  etat = 0 ;

  vGeo->set_date(this->_date);


    day= this->_date.get_day0hTU().get_julianDate();

    ierr = p2nutt( 2, greenwich, day, p, pd ) ;

  double position[3];
  double speed[3];

    position[0] = _position[0]*p[1] + _position[1]*p[4] + _position[2]*p[7] ;

    position[1] = _position[0]*p[2] + _position[1]*p[5] + _position[2]*p[8] ;

    position[2] = _position[0]*p[3] + _position[1]*p[6] + _position[2]*p[9] ;

    speed[0] = _speed[0]*p[1] +_speed[1]*p[4] + _speed[2]*p[7] + OMEGATERRE * (_position[0]*pd[1] + _position[1]*pd[4] + _position[2]*pd[7] );

    speed[1] = _speed[0]*p[2] + _speed[1]*p[5] + _speed[2] *p[8] + OMEGATERRE * (_position[0]*pd[2] + _position[1]*pd[5] + _position[2]*pd[8] );

    speed[2] = _speed[0]*p[3] + _speed[1]*p[6] + _speed[2]*p[9] + OMEGATERRE * (_position[0]*pd[3] + _position[1]*pd[6] + _position[2]*pd[9] );

  vGeo->set_position(position);
  vGeo->set_speed(speed);
}

GalileanEphemeris::operator GeographicEphemeris()
{
  GeographicEphemeris rhs;
  ToGeographic(&rhs);
  return rhs;
}

GalileanEphemeris::GalileanEphemeris(GeographicEphemeris& rhs)
{
  rhs.ToGalilean(this);
}

int GalileanEphemeris::p2nutt(int newcmb, double greenwich, double day,
            double p[], double pd[] )
{
  const double PI          = 3.14159265358979323846 ;
  const double TWOPI      = 6.28318530717958647693 ;
  // const double MU          = 3.9860047e+14 ;
  // const double JOURCIVIL_LENGTH   = 86400.0 ;
  // const double JOURSIDERAL = 86164.09054 ;
  // const double OMEGATERRE  = 6.28318530717958647693 / 86164.09054 ;
  // const double CLUM        = 2.99792458e+8 ;
  // const double A_WGS84     = 6378137.0 ;
  // const double B_WGS84     = 6356752.3141 ;
/*

  GIVES TRANSFORMATION MATRIX P(3,3) FROM QUASI-INERTIAL MEAN SYSTEM
  OF DATE TO TRUE SYSTEM OF DATE OR EARTH-FIXED ROTATING SYSTEM BY
  ROTATION (OPTIONAL) AND NUTATION. USES  SIMPLIFIED ANALYTICAL
  MODEL DERIVED FROM THE 1980 IAU WAHR THEORY OF NUTATION (REF:
  ASTRONOMICAL ALMANAC 1984, PP. S23-S26) BY SKIPPING MOST TERMS BELOW
  0.03 ARCSEC. ACCURACY = 0.5D-6 RADIANS.

  DAY = Julian Day
  newcmb =-1 => TO TRUE SYSTEM OF DATE
         = 0 => INTERMEDIATE TRANSFORMATION TO EARTH-FIXED ROTATING
                SYSTEM BUT WITHOUT NEWCOMB'S ANGLE. (NOT TRUE-OF-DATE)
         = 1 => TO EARTH-FIXED ROTATING SYSTEM

  P(3,3) = ROTATION MATRIX SUCH THAT
            THE TRANSFORMATION IS    R(TRANSF) = P * R(INERT)

   COMMON BLOCKS FROM PEPSOC SUBROUTINE 'P2COOT' ARE USED. */

  double arg[10] ;
  double t, ff, dd, dpsi, deps;
  double ecl, cecl, secl, dnu, srang;
  double sra, cra, snu, cnu, sde, cde;
  double facsin[10],faccos[10];

  int i;

  /*   DATA FOR NUTATION MODELLING */
  facsin[0]=0.e0;
  facsin[1]=-0.8338601e-04;
  facsin[2]=-0.6393238e-05;
  facsin[3]=-0.1102466e-05;
  facsin[4]=0.9996858e-06;
  facsin[5]=0.6913443e-06;
  facsin[6]=0.3451873e-06;
  facsin[7]=-0.2506487e-06;
  facsin[8]=-0.1871381e-06;
  facsin[9]=-0.1459289e-06;

  faccos[0]=0.e0;
  faccos[1]=0.4461498e-04;
  faccos[2]=0.2780891e-05;
  faccos[3]=0.4736630e-06;
  faccos[4]=-0.4339082e-06;
  faccos[5]=0.2617994e-07;
  faccos[6]=-0.3393696e-08;
  faccos[7]=0.1085983e-06;
  faccos[8]=0.9696274e-07;
  faccos[9]=0.6254096e-07;


  /*  ECL = OBLIQUITY OF ECLIPTIC AT EPOCH J2000.0
    ECL CHANGES BY -0.01300 DEG/CENTURY.
    ECL = 23 DEG + 26 ' + 21.4119 " */
  ecl = PI/180.e0*(23.e0 + (26.e0 + 21.4119e0/6.e1)/6.e1);
  cecl = cos(ecl);
  secl = sin(ecl);

  /*t= Julian day relative to year 2000 at 12:00:00 */
  t = day-2451540. - 0.5 ;

  /*  L = MEAN ANOMALY OF THE MOON */
  arg[6] = fmod( 0.2355548394e+01 + t*( 0.2280271437e+00 + t* 0.1137830e-12 ),TWOPI);

  /*C  L-PRIME = MEAN ANOMALY OF THE SUN (EARTH) */
  arg[5] = fmod( 0.6240035939e+01 + t*( 0.1720197005e-01 - t* 0.2096864e-14 ),TWOPI);

  /*  F = L - OMEGA (SEE ABOVE AND BELOW) */
  ff = fmod( 0.1627901934e+01 + t*( 0.2308957196e+00 - t* 0.4817699e-13 ),TWOPI);

  /*  D = MEAN ELONGATION OF THE MOON FROM THE SUN */
  dd = fmod( 0.5198469514e+01 + t*( 0.2127687104e+00 - t* 0.2504244e-13 ),TWOPI);

  /*  OMEGA = LONGITUDE OF MOON'S ASCENDING NODE FROM MEAN EQUINOX OF DATE */
  arg[0]=0.e0;
  arg[1] = fmod( 0.2182438624e+01 - t*( 0.9242175478e-03 - t* 0.2709206e-13 ),TWOPI);

    arg[3] = 2.e0*(ff + arg[1]);
    arg[2] = arg[3] - 2.e0*dd ;
    arg[4] = 2.e0*arg[1] ;
    arg[7] = arg[5] + arg[2] ;
    arg[8] = 2.e0*ff + arg[1] ;
    arg[9] = arg[6] + arg[3] ;

    dpsi = 0.e0;
    deps = 0.e0;
    for ( i=1 ; i<=9; i++ )
  {
    dpsi = dpsi + facsin[i] * sin(arg[i]);
    deps = deps + faccos[i] * cos(arg[i]);
      }


  /*  ROTATION OF -deps AROUND THE X-AXIS */
  cde=cos(deps);
  sde=sin(deps);

  /*  ROTATION OF DNU = dpsi*SIN(ECLIPT) AROUND THE Y-AXIS */
  dnu = dpsi*secl;
  cnu=cos(dnu);
  snu=sin(dnu);

  /*  SELECT WHICH ROTATION IS WANTED FOR THE Z-AXIS */
  srang=0.e0;
  if (newcmb == -1)
  {
    /*  ROTATION OF SRANG = -DMU = -dpsi*COS(ECLIPT) AROUND THE Z-AXIS */
    srang = -dpsi*cecl;
  }
  else if ( newcmb == 0)
  {
    srang = 0.e0;
  }
  else if ( newcmb == 1 )
  {
    /*  SIDEREAL ANGLE, FROM 'COOT20' WITH NEWCOMB'S FORMULA:*/
    /*srang = fmod(STD20R+(OMT20R+OMQ20R*day)*day,TWOPI);*/
  }
  else if ( newcmb == 2 )
  {
    /*  SIDEREAL ANGLE, FROM INPUT */
    srang = fmod(greenwich*PI/180.e0,TWOPI) ;
    /*printf("srang dans p2nutt %g",greenwich);*/
  }
  else
  {
    return -1;;
  }


  sra=sin(srang);
  cra=cos(srang);

  /*  ROTATION MATRIX = RZ(SRANG)*RX(-deps)*RY(DNU) */
  p[0] = 0.e0;
  p[1] = cra*cnu - sra*sde*snu ;
  p[2] = -sra*cnu - cra*sde*snu ;
  p[3] = cde*snu ;
  p[4] = sra*cde ;
  p[5] = cra*cde ;
  p[6] = sde ;
  p[7] = -cra*snu - sra*sde*cnu ;
  p[8] = sra*snu - cra*sde*cnu ;
  p[9] = cde*cnu ;

  pd[0] = 0.e0;
  pd[1] = -sra*cnu - cra*sde*snu ;
  pd[2] = -cra*cnu + sra*sde*snu ;
  pd[3] = 0. ;
  pd[4] = cra*cde ;
  pd[5] = -sra*cde ;
  pd[6] = 0. ;
  pd[7] = sra*snu - cra*sde*cnu ;
  pd[8] = cra*snu + sra*sde*cnu ;
  pd[9] = 0. ;

  return 1;

}
}
