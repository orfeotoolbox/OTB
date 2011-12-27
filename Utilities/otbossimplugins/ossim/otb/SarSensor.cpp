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

int SarSensor::ImageToWorld(double distance, JSDDateTime time, double height, double& lon, double& lat)
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

int SarSensor::localisationSAR ( GeographicEphemeris PosVit , double lambda ,
                        double dist , double fDop , int sensVisee ,
                        double rayonEqu , double rayonPol ,
                        double h , RectangularCoordinate* cart )
{
  double coordCart[3];
        coordCart[0]=0.0;
        coordCart[1]=0.0;
        coordCart[2]=0.0;


  int* ordre;
  int etat , fin , n , i , i1 , i2 , nRac , indice[4] ;
  double posX , posY , posZ , vitX , vitY , vitZ ;
  double dist1 , fDop1 , he , hp ;
  double rho2 , k , a , b , c , d , u , v , w ;
  double x[4] , y[4] , z[4] , r[4] ;
  std::complex<double> aa[5];
  std::complex<double> *racine;
  GeographicEphemeris PosVitGeo ;



  /* Initialization of characteristical values                */
  /* note :  for precise computation :                */
  /*           - ranges are processed in mm                  */
  /*           - velocities are processed in  km/s           */
  /*           - frequencies are processed in kHz             */
    const double KILO = 1.0e-3 ;
  const double MEGA = 1.0e-6 ;
  const double EPSILON = 1.0e-12 ;

  etat  = 0 ;

  dist1 = dist * MEGA ;
  fDop1 = fDop * KILO ;

  he    = (rayonEqu + h) * MEGA ;       /* Equatorial radius + h */
  hp    = (rayonPol + h) * MEGA ;       /* Polar radius + h    */

  posX  = PosVit.get_position()[0] * MEGA ;
  posY  = PosVit.get_position()[1] * MEGA ;
  posZ  = PosVit.get_position()[2] * MEGA ;
  vitX = - PosVit.get_speed()[0] * KILO ;
  vitY = - PosVit.get_speed()[1] * KILO ;
  vitZ = - PosVit.get_speed()[2] * KILO ;


  /* Coefficients computation and equation solving */
  if (etat == 0)
  {

    u = vitX * posY - vitY * posX ;
    a = (vitX / u) * (1.0 - (he / hp) * (he / hp)) / 2.0 ;
    b = (vitX * posZ - vitZ * posX) / u ;
    rho2 = posX * posX + posY * posY + posZ * posZ ;
    k    = posX * vitX + posY * vitY + posZ * vitZ -
           lambda * dist1 * fDop1 / 2.0 ;
    c = (vitX * (he * he + rho2 - dist1 * dist1) - 2.0 * k * posX) / (2.0 * u);

    u     = vitZ - b * vitY ;
    v     = c * vitY - k ;
    w     = v * v - (vitX * vitX) * (he * he - c * c) ;
    aa[0] = std::complex<double>(w,0.0);            /* Constant coefficient     */
    w     = 2.0 * (u * v - (b * c) * (vitX * vitX)) ;
    aa[1] = std::complex<double>(w,0.0) ;                     /* First order coefficient  */
    w     = u * u + 2.0 * a * v * vitY +
            (vitX * vitX) * ((he / hp) * (he / hp) + b * b + 2.0 * a * c) ;
    aa[2] = std::complex<double>(w,0.0) ;                     /* Second order coefficient */
    w     = 2.0 * a * (u * vitY - b * vitX * vitX) ;
    aa[3] = std::complex<double>(w,0.0) ;                     /* Third order coefficient  */
    w     = (vitX * vitX + vitY * vitY) * a * a ;
    aa[4] = std::complex<double>(w,0.0) ;                     /* Fourth order coefficient */

    Equation eq(4,aa);    /* Equation solving */
    eq.Solve();

    n = eq.get_nbrSol();
    racine = eq.get_solutions();
    ordre = eq.get_order();

    nRac = 0 ;
    for (i = 0 ; i < n ; i++)               /* Real root selection */
    {
      d = fabs(racine[i].imag()) ;
      if (d < EPSILON)
      {
        z[nRac] = racine[i].real();
        y[nRac] = (a * z[nRac] - b) * z[nRac] + c ;
        x[nRac] = (k - (vitY * y[nRac] + vitZ * z[nRac])) / vitX ;
        nRac    = nRac + 1 ;
      }
    }
    if (nRac == 0)
      etat = 2 ;  /* No root */
  }

  /* Computed roots sort */
  if (etat == 0)
  {
    for (i = 0 ; i < nRac ; i++)
    {
      /* Computation of the "distance" between roots images and equation values */
      u = x[i] - posX ;
      v = y[i] - posY ;
      w = z[i] - posZ ;
      r[i] = fabs ((u * u + v * v + w * w) / (dist1 * dist1) - 1.0 ) ;
      u = u * vitX ;
      v = v * vitY ;
      w = w * vitZ ;
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
    fin = 0 ;
    while (fin == 0)
    {
      fin = 1 ;
      for (i = 0 ; i < (nRac - 1) ; i++)
      {
        i1 = indice[i] ;
        i2 = indice[i+1] ;
        if (r[i2] < r[i1])
        {
          indice[i] = i2 ;
          indice[i+1] = i1 ;
          fin = 0 ;
        }
      }
    }

    /* Selection of the correct root (corresponding to the imaging direction) */
    fin = 0 ;
    i = 0 ;
    while ((fin == 0) && (i < nRac))
    {
      i1 = indice[i] ;
      u  = posY * vitZ - posZ * vitY ;
      v  = posZ * vitX - posX * vitZ ;
      w  = posX * vitY - posY * vitX ;
      a  = x[i1] - posX ;
      b  = y[i1] - posY ;
      c  = z[i1] - posZ ;
      d  = (a * u + b * v + c * w) * sensVisee;
      if (d >= 0.0)
      {
        coordCart[0] = x[i1] / MEGA ;   /* Coordinates in the      */
        coordCart[1] = y[i1] / MEGA ;   /* geographic referential,  */
        coordCart[2] = z[i1] / MEGA ;   /* in legal units (m)      */
        fin = 1 ;
      }
      i++ ;
    }
    if (fin == 0)
      etat = 1 ;  /* No root in the imaging direction */
  }

  cart->set_coordinates(coordCart[0], coordCart[1], coordCart[2]);
  return etat ;
}
}
