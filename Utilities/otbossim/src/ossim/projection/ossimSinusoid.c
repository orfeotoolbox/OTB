/***************************************************************************/
/* RSC IDENTIFIER: SINUSOIDAL
 *
 * ABSTRACT
 *
 *    This component provides conversions between Geodetic coordinates 
 *    (latitude and longitude in radians) and Sinusoid projection coordinates
 *    (easting and northing in meters).
 *
 * ERROR HANDLING
 *
 *    This component checks parameters for valid values.  If an invalid value
 *    is found, the error code is combined with the current error code using 
 *    the bitwise or.  This combining allows multiple error codes to be
 *    returned. The possible error codes are:
 *
 *          SINU_NO_ERROR           : No errors occurred in function
 *          SINU_LAT_ERROR          : Latitude outside of valid range
 *                                      (-90 to 90 degrees)
 *          SINU_LON_ERROR          : Longitude outside of valid range
 *                                      (-180 to 360 degrees)
 *          SINU_EASTING_ERROR      : Easting outside of valid range
 *                                      (False_Easting +/- ~20,000,000 m,
 *                                       depending on ellipsoid parameters)
 *          SINU_NORTHING_ERROR     : Northing outside of valid range
 *                                      (False_Northing +/- ~10,000,000 m,
 *                                       depending on ellipsoid parameters)
 *          SINU_CENT_MER_ERROR     : Origin longitude outside of valid range
 *                                      (-180 to 360 degrees)
 *          SINU_A_ERROR            : Semi-major axis less than or equal to zero
 *          SINU_INV_F_ERROR        : Inverse flattening outside of valid range
 *								  	                  (250 to 350)
 *
 * REUSE NOTES
 *
 *    SINUSOIDAL is intended for reuse by any application that performs a
 *    Sinusoid projection or its inverse.
 *    
 * REFERENCES
 *
 *    Further information on SINUSOIDAL can be found in the Reuse Manual.
 *
 *    SINUSOIDAL originated from :  U.S. Army Topographic Engineering Center
 *                                  Geospatial Information Division
 *                                  7701 Telegraph Road
 *                                  Alexandria, VA  22310-3864
 *
 * LICENSES
 *
 *    None apply to this component.
 *
 * RESTRICTIONS
 *
 *    SINUSOIDAL has no restrictions.
 *
 * ENVIRONMENT
 *
 *    SINUSOIDAL was tested and certified in the following environments:
 *
 *    1. Solaris 2.5 with GCC, version 2.8.1
 *    2. Windows 95 with MS Visual C++, version 6
 *
 * MODIFICATIONS
 *
 *    Date              Description
 *    ----              -----------
 *    7-15-99          Original Code
 *
 */


/***************************************************************************/
/*
 *                               INCLUDES
 */

#include <math.h>
#include <ossim/projection/ossimSinusoid.h>

/*
 *    math.h     - Standard C math library
 *    sinusoid.h - Is for prototype error checking
 */


/***************************************************************************/
/*
 *                               DEFINES
 */

#define PI         3.14159265358979323e0  /* PI                            */
#define PI_OVER_2  (PI / 2.0)                 
#define TWO_PI     (2.0 * PI)                  
#define SINU_COEFF_TIMES_SIN(coeff, x, latit)      (coeff * sin(x * latit))
#define FLOAT_EQ(x,v,epsilon)   (((v - epsilon) < x) && (x < (v + epsilon)))


/***************************************************************************/
/*
 *                               GLOBALS
 */

/* Ellipsoid Parameters, default to WGS 84 */
static double Sinu_a = 6378137.0;                      /* Semi-major axis of ellipsoid in meters */
static double Sinu_f = 1 / 298.257223563;              /* Flattening of ellipsoid */
static double es2 = 0.0066943799901413800;             /* Eccentricity (0.08181919084262188000) squared         */
static double es4 = 4.4814723452405e-005;              /* es2 * es2 */
static double es6 = 3.0000678794350e-007;              /* es4 * es2 */
static double c0 = .99832429845280;           /* 1 - es2 / 4.0 - 3.0 * es4 / 64.0 - 5.0 * es6 / 256.0 */
static double c1 = .0025146070605187;     /* 3.0 * es2 / 8.0 + 3.0 * es4 / 32.0 + 45.0 * es6 / 1024.0 */
static double c2 = 2.6390465943377e-006;             /* 15.0 * es4 / 256.0 + 45.0 * es6 / 1024.0 */
static double c3 = 3.4180460865959e-009;               /* 35.0 * es6 / 3072.0 */
static double a0 = .0025188265843907;                  /* 3.0 * e1 / 2.0 - 27.0 * e3 / 32.0 */
static double a1 = 3.7009490356205e-006;               /* 21.0 * e2 / 16.0 - 55.0 * e4 / 32.0 */
static double a2 = 7.4478137675038e-009;               /* 151.0 * e3 / 96.0 */
static double a3 = 1.7035993238596e-011;               /* 1097.0 * e4 / 512.0 */

/* Sinusoid projection Parameters */
static double Sinu_Origin_Long = 0.0;                  /* Longitude of origin in radians    */
static double Sinu_False_Northing = 0.0;               /* False northing in meters          */
static double Sinu_False_Easting = 0.0;                /* False easting in meters           */

/* Maximum variance for easting and northing values for WGS 84.
 */
static double Sinu_Max_Easting = 20037509.0;
static double Sinu_Min_Easting = -20037509.0;
static double Sinu_Delta_Northing = 10001966.0;
/*
 * These state variables are for optimization purposes.  The only function
 * that should modify them is Set_Sinusoidal_Parameters.
 */


/***************************************************************************/
/*
 *                              FUNCTIONS     
 */


long Set_Sinusoidal_Parameters(double a,
                               double f,
                               double Central_Meridian,
                               double False_Easting,
                               double False_Northing)
{ /* BEGIN Set_Sinusoidal_Parameters */
/*
 * The function Set_Sinusoidal_Parameters receives the ellipsoid parameters and
 * Sinusoidal projection parameters as inputs, and sets the corresponding state
 * variables.  If any errors occur, the error code(s) are returned by the function, 
 * otherwise SINU_NO_ERROR is returned.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters   (input)
 *    f                 : Flattening of ellipsoid						        (input)
 *    Central_Meridian  : Longitude in radians at the center of     (input)
 *                          the projection
 *    False_Easting     : A coordinate value in meters assigned to the
 *                          central meridian of the projection.     (input)
 *    False_Northing    : A coordinate value in meters assigned to the
 *                          origin latitude of the projection       (input)
 */

  double j;
  double One_MINUS_es2, Sqrt_One_MINUS_es2, e1, e2, e3, e4;
  double inv_f = 1 / f;
  long Error_Code = SINU_NO_ERROR;

  if (a <= 0.0)
  { /* Semi-major axis must be greater than zero */
    Error_Code |= SINU_A_ERROR;
  }
  if ((inv_f < 250) || (inv_f > 350))
  { /* Inverse flattening must be between 250 and 350 */
    Error_Code |= SINU_INV_F_ERROR;
  }
  if ((Central_Meridian < -PI) || (Central_Meridian > TWO_PI))
  { /* origin longitude out of range */
    Error_Code |= SINU_CENT_MER_ERROR;
  }
  if (!Error_Code)
  { /* no errors */
    Sinu_a = a;
    Sinu_f = f;
    es2 = 2 * Sinu_f - Sinu_f * Sinu_f;
    es4 = es2 * es2;
    es6 = es4 * es2;
    j = 45.0 * es6 / 1024.0;
    c0 = 1.0 - es2 / 4.0 - 3.0 * es4 / 64.0 - 5.0 * es6 / 256.0;
    c1 = 3.0 * es2 / 8.0 + 3.0 * es4 / 32.0 + j;
    c2 = 15.0 * es4 / 256.0 + j;
    c3 = 35.0 * es6 / 3072.0;
    One_MINUS_es2 = 1.0 - es2;
    Sqrt_One_MINUS_es2 = sqrt(One_MINUS_es2);
    e1 = (1.0 - Sqrt_One_MINUS_es2) / (1.0 + Sqrt_One_MINUS_es2);
    e2 = e1 * e1;
    e3 = e2 * e1;
    e4 = e3 * e1;
    a0 = 3.0 * e1 / 2.0 - 27.0 * e3 / 32.0 ;
    a1 = 21.0 * e2 / 16.0 - 55.0 * e4 / 32.0;
    a2 = 151.0 * e3 / 96.0;
    a3 = 1097.0 * e4 / 512.0;
    if (Central_Meridian > PI)
      Central_Meridian -= TWO_PI;
    Sinu_Origin_Long = Central_Meridian;
    Sinu_False_Northing = False_Northing;
    Sinu_False_Easting = False_Easting;

    if (Sinu_Origin_Long > 0)
    {
      Sinu_Max_Easting = 19926189.0;
      Sinu_Min_Easting = -20037509.0;
    }
    else if (Sinu_Origin_Long < 0)
    {
      Sinu_Max_Easting = 20037509.0;
      Sinu_Min_Easting = -19926189.0;
    }
    else
    {
      Sinu_Max_Easting = 20037509.0;
      Sinu_Min_Easting = -20037509.0;
    }
  } /* END OF if(!Error_Code) */
  return (Error_Code);
} /* END OF Set_Sinusoidal_Parameters */

void Get_Sinusoidal_Parameters(double *a,
                               double *f,
                               double *Central_Meridian,
                               double *False_Easting,
                               double *False_Northing)
{ /* BEGIN Get_Sinusoidal_Parameters */
/*
 * The function Get_Sinusoidal_Parameters returns the current ellipsoid
 * parameters, and Sinusoidal projection parameters.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters   (output)
 *    f                 : Flattening of ellipsoid						        (output)
 *    Central_Meridian  : Longitude in radians at the center of     (output)
 *                          the projection
 *    False_Easting     : A coordinate value in meters assigned to the
 *                          central meridian of the projection.     (output)
 *    False_Northing    : A coordinate value in meters assigned to the
 *                          origin latitude of the projection       (output)
 */

  *a = Sinu_a;
  *f = Sinu_f;
  *Central_Meridian = Sinu_Origin_Long;
  *False_Easting = Sinu_False_Easting;
  *False_Northing = Sinu_False_Northing;
  return;
} /* END OF Get_Sinusoidal_Parameters */


long Convert_Geodetic_To_Sinusoidal (double Latitude,
                                     double Longitude,
                                     double *Easting,
                                     double *Northing)
{ /* BEGIN Convert_Geodetic_To_Sinusoidal */
/*
 * The function Convert_Geodetic_To_Sinusoidal converts geodetic (latitude and
 * longitude) coordinates to Sinusoidal projection (easting and northing)
 * coordinates, according to the current ellipsoid and Sinusoidal projection
 * parameters.  If any errors occur, the error code(s) are returned by the
 * function, otherwise SINU_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi) in radians           (input)
 *    Longitude         : Longitude (lambda) in radians       (input)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */

  double slat = sin(Latitude);
  double sin2lat, sin4lat, sin6lat;
  double dlam;                      /* Longitude - Central Meridan */
  double mm;
  double MM;
  long Error_Code = SINU_NO_ERROR;

  if ((Latitude < -PI_OVER_2) || (Latitude > PI_OVER_2))
  { /* Latitude out of range */
    Error_Code |= SINU_LAT_ERROR;
  }
  if ((Longitude < -PI) || (Longitude > TWO_PI))
  { /* Longitude out of range */
    Error_Code |= SINU_LON_ERROR;
  }
  if (!Error_Code)
  { /* no errors */
    dlam = Longitude - Sinu_Origin_Long;
    if (dlam > PI)
    {
      dlam -= TWO_PI;
    }
    if (dlam < -PI)
    {
      dlam += TWO_PI;
    }
    mm = sqrt(1.0 - es2 * slat * slat);

    sin2lat = SINU_COEFF_TIMES_SIN(c1, 2.0, Latitude);
    sin4lat = SINU_COEFF_TIMES_SIN(c2, 4.0, Latitude);
    sin6lat = SINU_COEFF_TIMES_SIN(c3, 6.0, Latitude);
    MM = Sinu_a * (c0 * Latitude - sin2lat + sin4lat - sin6lat);

    *Easting = Sinu_a * dlam * cos(Latitude) / mm + Sinu_False_Easting;
    *Northing = MM + Sinu_False_Northing;
  }
  return (Error_Code);
} /* END OF Convert_Geodetic_To_Sinusoidal */


long Convert_Sinusoidal_To_Geodetic(double Easting,
                                    double Northing,
                                    double *Latitude,
                                    double *Longitude)
{ /* BEGIN Convert_Sinusoidal_To_Geodetic */
/*
 * The function Convert_Sinusoidal_To_Geodetic converts Sinusoidal projection
 * (easting and northing) coordinates to geodetic (latitude and longitude)
 * coordinates, according to the current ellipsoid and Sinusoidal projection
 * coordinates.  If any errors occur, the error code(s) are returned by the
 * function, otherwise SINU_NO_ERROR is returned.
 *
 *    Easting           : Easting (X) in meters                  (input)
 *    Northing          : Northing (Y) in meters                 (input)
 *    Latitude          : Latitude (phi) in radians              (output)
 *    Longitude         : Longitude (lambda) in radians          (output)
 */

  double dx;     /* Delta easting - Difference in easting (easting-FE)      */
  double dy;     /* Delta northing - Difference in northing (northing-FN)   */
  double mu;
  double sin2mu, sin4mu, sin6mu, sin8mu;
  double sin_lat;
  long Error_Code = SINU_NO_ERROR;

  if ((Easting < (Sinu_False_Easting + Sinu_Min_Easting))
      || (Easting > (Sinu_False_Easting + Sinu_Max_Easting)))
  { /* Easting out of range */
    Error_Code |= SINU_EASTING_ERROR;
  }
  if ((Northing < (Sinu_False_Northing - Sinu_Delta_Northing))
      || (Northing > (Sinu_False_Northing + Sinu_Delta_Northing)))
  { /* Northing out of range */
    Error_Code |= SINU_NORTHING_ERROR;
  }
  if (!Error_Code)
  { /* no errors */
    dy = Northing - Sinu_False_Northing;
    dx = Easting - Sinu_False_Easting;

    mu = dy / (Sinu_a * c0);
    sin2mu = SINU_COEFF_TIMES_SIN(a0, 2.0, mu);
    sin4mu = SINU_COEFF_TIMES_SIN(a1, 4.0, mu);
    sin6mu = SINU_COEFF_TIMES_SIN(a2, 6.0, mu);
    sin8mu = SINU_COEFF_TIMES_SIN(a3, 8.0, mu);
    *Latitude = mu + sin2mu + sin4mu + sin6mu + sin8mu;

    if (*Latitude > PI_OVER_2)  /* force distorted values to 90, -90 degrees */
      *Latitude = PI_OVER_2;
    else if (*Latitude < -PI_OVER_2)
      *Latitude = -PI_OVER_2;

    if (FLOAT_EQ(fabs(*Latitude),PI_OVER_2,1.0e-8))
      *Longitude = Sinu_Origin_Long;
    else
    {
      sin_lat = sin(*Latitude);
      *Longitude = Sinu_Origin_Long + dx * sqrt(1.0 - es2 *
                                                sin_lat * sin_lat) / (Sinu_a * cos(*Latitude));


      if (*Longitude > PI)
        *Longitude -= TWO_PI;
      if (*Longitude < -PI)
        *Longitude += TWO_PI;

      if (*Longitude > PI)  /* force distorted values to 180, -180 degrees */
        *Longitude = PI;
      else if (*Longitude < -PI)
        *Longitude = -PI;


    }
  }
  return (Error_Code);
} /* END OF Convert_Sinusoidal_To_Geodetic */
