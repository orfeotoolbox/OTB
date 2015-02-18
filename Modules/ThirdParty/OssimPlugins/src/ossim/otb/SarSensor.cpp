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

#include <otb/SarSensor.h>
#include <otb/JSDDateTime.h>
#include <otb/Ephemeris.h>
#include <otb/GeographicEphemeris.h>
#include <otb/Sensor.h>
#include <otb/SensorParams.h>
#include <otb/PlatformPosition.h>
#include <otb/Equation.h>
#include <otb/RectangularCoordinate.h>
#include <otb/GeodesicCoordinate.h>
#include <complex>


namespace ossimplugins
{


SarSensor::SarSensor(SensorParams* params, PlatformPosition* position) : Sensor(params, position)
{
}

SarSensor::~SarSensor()
{
}

int SarSensor::ImageToWorld(double distance, JSDDateTime time, double height, double& lon, double& lat) const
{
  const double TWOPI      = 6.28318530717958647693 ;

  double semiMajorAxis = _params->get_semiMajorAxis() ;  // default : WGS84
  double semiMinorAxis = _params->get_semiMinorAxis() ; // default : WGS84

  double lambda = _params->get_rwl();
  int sensVisee ;
  if (_params->get_sightDirection() == SensorParams::Right) sensVisee = 1 ;
  else sensVisee = -1 ;

  Ephemeris* satPosition = _position->Interpolate(time);

  GeographicEphemeris *geoEph = (GeographicEphemeris*)satPosition;

  RectangularCoordinate cart;

  double dopplerCentroid = _params->get_dopcen();
  double dopcenLinear = _params->get_dopcenLinear();
  if (dopcenLinear != 0.0)
  {
	  dopplerCentroid += dopcenLinear * distance/1000; // Hz/km
  }

  // note : the Doppler frequency is set to zero
  int etatLoc = localisationSAR(*geoEph, lambda, distance, dopplerCentroid, sensVisee, semiMajorAxis , semiMinorAxis , height, &cart);

  GeodesicCoordinate geo;
  cart.AsGeodesicCoordinates(semiMajorAxis , semiMinorAxis, &geo);
  lon = (geo.get_x())*360.0/TWOPI;
  lat = (geo.get_y())*360.0/TWOPI;

  delete satPosition;

  return etatLoc ;
}

int SarSensor::localisationSAR ( GeographicEphemeris posSpeed , double lambda ,
                        double dist , double fDop , int sensVisee ,
                        double equRadius , double polRadius ,
                        double h , RectangularCoordinate* cart ) const
{
  double coordCart[3];
  coordCart[0]=0.0;
  coordCart[1]=0.0;
  coordCart[2]=0.0;


  double rho2 , k , a , b , c , d , u , v , w ;
  double x[4] , y[4] , z[4] , r[4] ;
  bool isEnd;


  /* Initialization of characteristical values                */
  /* note :  for precise computation :                */
  /*           - ranges are processed in mm                  */
  /*           - velocities are processed in  km/s           */
  /*           - frequencies are processed in kHz             */
  const double KILO = 1.0e-3 ;
  const double MEGA = 1.0e-6 ;
  const double EPSILON = 1.0e-12 ;


  double dist1 = dist * MEGA ;
  double fDop1 = fDop * KILO ;

  double he    = (equRadius + h) * MEGA ;       /* Equatorial radius + h */
  double hp    = (polRadius + h) * MEGA ;       /* Polar radius + h    */

  double posX  = posSpeed.get_position()[0] * MEGA ;
  double posY  = posSpeed.get_position()[1] * MEGA ;
  double posZ  = posSpeed.get_position()[2] * MEGA ;
  double speedX = - posSpeed.get_speed()[0] * KILO ;
  double speedY = - posSpeed.get_speed()[1] * KILO ;
  double speedZ = - posSpeed.get_speed()[2] * KILO ;


  /* Coefficients computation and equation solving */
  int state  = 0;
  u = speedX * posY - speedY * posX ;
  a = (speedX / u) * (1.0 - (he / hp) * (he / hp)) / 2.0 ;
  b = (speedX * posZ - speedZ * posX) / u ;
  rho2 = posX * posX + posY * posY + posZ * posZ ;
  k    = posX * speedX + posY * speedY + posZ * speedZ -
         lambda * dist1 * fDop1 / 2.0 ;
  c = (speedX * (he * he + rho2 - dist1 * dist1) - 2.0 * k * posX) / (2.0 * u);

  u     = speedZ - b * speedY ;
  v     = c * speedY - k ;
  w     = v * v - (speedX * speedX) * (he * he - c * c) ;

  std::complex<double> aa[5];
  aa[0] = std::complex<double>(w,0.0);            /* Constant coefficient     */
  w     = 2.0 * (u * v - (b * c) * (speedX * speedX)) ;
  aa[1] = std::complex<double>(w,0.0) ;                     /* First order coefficient  */
  w     = u * u + 2.0 * a * v * speedY +
          (speedX * speedX) * ((he / hp) * (he / hp) + b * b + 2.0 * a * c) ;
  aa[2] = std::complex<double>(w,0.0) ;                     /* Second order coefficient */
  w     = 2.0 * a * (u * speedY - b * speedX * speedX) ;
  aa[3] = std::complex<double>(w,0.0) ;                     /* Third order coefficient  */
  w     = (speedX * speedX + speedY * speedY) * a * a ;
  aa[4] = std::complex<double>(w,0.0) ;                     /* Fourth order coefficient */

  Equation eq(4,aa);    /* Equation solving */
  eq.Solve();

  int n = eq.get_nbrSol();
  const std::complex<double> *root = eq.get_solutions();

  int nRoot = 0 ;
  for (int i = 0 ; i < n ; i++)               /* Real root selection */
  {
    d = fabs(root[i].imag()) ;
    if (d < EPSILON)
    {
      z[nRoot] = root[i].real();
      y[nRoot] = (a * z[nRoot] - b) * z[nRoot] + c ;
      x[nRoot] = (k - (speedY * y[nRoot] + speedZ * z[nRoot])) / speedX ;
      nRoot    = nRoot + 1 ;
    }
  }
  if (nRoot == 0)
    state = 2 ;  /* No root */


  /* Computed roots sort */

  if (state == 0)
  {
    int  indice[4] ;
    for (int i = 0 ; i < nRoot ; i++)
    {
      /* Computation of the "distance" between roots images and equation values */
      u = x[i] - posX ;
      v = y[i] - posY ;
      w = z[i] - posZ ;
      r[i] = fabs ((u * u + v * v + w * w) / (dist1 * dist1) - 1.0 ) ;
      u = u * speedX ;
      v = v * speedY ;
      w = w * speedZ ;
      if (fabs (fDop) > EPSILON)
        r[i] = r[i] + fabs (1.0 + 2.0 * (u + v + w) / (lambda * dist1 * fDop1));
      else
        r[i] = r[i] + fabs (2.0 * (u + v + w) / (lambda * dist1)) ;
      u = x[i] / he ;
      v = y[i] / he ;
      w = z[i] / hp ;
      r[i] = r[i] + fabs (u * u + v * v + w * w - 1.0) ;
      indice[i] = i ;
    }

    /* Roots sort by increasing differences */
    isEnd = false ;
    while (!isEnd)
    {
      isEnd = true;
      for (int i = 0 ; i < (nRoot - 1) ; i++)
      {
        int i1 = indice[i] ;
        int i2 = indice[i+1] ;
        if (r[i2] < r[i1])
        {
          indice[i] = i2 ;
          indice[i+1] = i1 ;
          isEnd = false ;
        }
      }
    }

    /* Selection of the correct root (corresponding to the imaging direction) */
    isEnd = false ;
    int i = 0 ;
    while (!isEnd && (i < nRoot))
    {
      int i1 = indice[i] ;
      u  = posY * speedZ - posZ * speedY ;
      v  = posZ * speedX - posX * speedZ ;
      w  = posX * speedY - posY * speedX ;
      a  = x[i1] - posX ;
      b  = y[i1] - posY ;
      c  = z[i1] - posZ ;
      d  = (a * u + b * v + c * w) * sensVisee;
      if (d >= 0.0)
      {
        coordCart[0] = x[i1] / MEGA ;   /* Coordinates in the      */
        coordCart[1] = y[i1] / MEGA ;   /* geographic referential,  */
        coordCart[2] = z[i1] / MEGA ;   /* in legal units (m)      */
        isEnd = true;
      }
      i++ ;
    }
    if (!isEnd)
      state = 1 ;  /* No root in the imaging direction */
  }

  cart->set_coordinates(coordCart[0], coordCart[1], coordCart[2]);
  return state ;
}
}
