/***************************************************************************/
/* RSC IDENTIFIER:  GEOCENTRIC
 *
 * ABSTRACT
 *
 *    This component provides conversions between Geodetic coordinates (latitude,
 *    longitude in radians and height in meters) and Geocentric coordinates
 *    (X, Y, Z) in meters.
 *
 * ERROR HANDLING
 *
 *    This component checks parameters for valid values.  If an invalid value
 *    is found, the error code is combined with the current error code using 
 *    the bitwise or.  This combining allows multiple error codes to be
 *    returned. The possible error codes are:
 *
 *      GEOCENT_NO_ERROR        : No errors occurred in function
 *      GEOCENT_LAT_ERROR       : Latitude out of valid range
 *                                 (-90 to 90 degrees)
 *      GEOCENT_LON_ERROR       : Longitude out of valid range
 *                                 (-180 to 360 degrees)
 *      GEOCENT_A_ERROR         : Semi-major axis less than or equal to zero
 *      GEOCENT_INV_F_ERROR     : Inverse flattening outside of valid range
 *								                 (250 to 350)
 *
 *
 * REUSE NOTES
 *
 *    GEOCENTRIC is intended for reuse by any application that performs
 *    coordinate conversions between geodetic coordinates and geocentric
 *    coordinates.
 *    
 *
 * REFERENCES
 *    
 *    An Improved Algorithm for Geocentric to Geodetic Coordinate Conversion,
 *    Ralph Toms, February 1996  UCRL-JC-123138.
 *    
 *    Further information on GEOCENTRIC can be found in the Reuse Manual.
 *
 *    GEOCENTRIC originated from : U.S. Army Topographic Engineering Center
 *                                 Geospatial Information Division
 *                                 7701 Telegraph Road
 *                                 Alexandria, VA  22310-3864
 *
 * LICENSES
 *
 *    None apply to this component.
 *
 * RESTRICTIONS
 *
 *    GEOCENTRIC has no restrictions.
 *
 * ENVIRONMENT
 *
 *    GEOCENTRIC was tested and certified in the following environments:
 *
 *    1. Solaris 2.5 with GCC version 2.8.1
 *    2. Windows 95 with MS Visual C++ version 6
 *
 * MODIFICATIONS
 *
 *    Date              Description
 *    ----              -----------
 *    25-02-97          Original Code
 *
 */


/***************************************************************************/
/*
 *                               INCLUDES
 */
#include <float.h>
#include <math.h>
#include <ossim/base/ossimGeocent.h>
/*
 *    math.h     - is needed for calls to sin, cos, tan and sqrt.
 *    geocent.h  - is needed for Error codes and prototype error checking.
 */


/***************************************************************************/
/*
 *                               DEFINES
 */
#define PI         3.14159265358979323e0
#define PI_OVER_2  (PI / 2.0e0)
#define FALSE      0
#define TRUE       1
#define COS_67P5   0.38268343236508977  /* cosine of 67.5 degrees */
#define AD_C       1.0026000            /* Toms region 1 constant */


/***************************************************************************/
/*
 *                              GLOBAL DECLARATIONS
 */
/* Ellipsoid parameters, default to WGS 84 */
double Geocent_a = 6378137.0;          /* Semi-major axis of ellipsoid in meters */
double Geocent_f = 1 / 298.257223563;  /* Flattening of ellipsoid           */

double Geocent_e2 = 0.0066943799901413800;   /* Eccentricity squared  */
double Geocent_ep2 = 0.00673949675658690300; /* 2nd eccentricity squared */
/*
 * These state variables are for optimization purposes.  The only function
 * that should modify them is Set_Geocentric_Parameters.
 */


/***************************************************************************/
/*
 *                              FUNCTIONS     
 */


long Set_Geocentric_Parameters (double a, 
                                double f) 
{ /* BEGIN Set_Geocentric_Parameters */
/*
 * The function Set_Geocentric_Parameters receives the ellipsoid parameters
 * as inputs and sets the corresponding state variables.
 *
 *    a  : Semi-major axis of ellipsoid, in meters.          (input)
 *    f  : Flattening of ellipsoid.						               (input)
 */

/*  double inv_f = 1 / f; */
  long Error_Code = GEOCENT_NO_ERROR;

#if 0
  if (a <= 0.0)
    Error_Code |= GEOCENT_A_ERROR;
  if ((inv_f < 250) || (inv_f > 350))
  { /* Inverse flattening must be between 250 and 350 */
    Error_Code |= GEOCENT_INV_F_ERROR;
  }
#endif
  if (!Error_Code)
  {
    Geocent_a = a;
    Geocent_f = f;
    Geocent_e2 = 2 * Geocent_f - Geocent_f * Geocent_f;
    Geocent_ep2 = (1 / (1 - Geocent_e2)) - 1;
  }
  return (Error_Code);
} /* END OF Set_Geocentric_Parameters */


void Get_Geocentric_Parameters (double *a, 
                                double *f)
{ /* BEGIN Get_Geocentric_Parameters */
/*
 * The function Get_Geocentric_Parameters returns the ellipsoid parameters
 * to be used in geocentric coordinate conversions.
 *
 *    a  : Semi-major axis of ellipsoid, in meters.          (output)
 *    f  : Flattening of ellipsoid.						               (output)
 */

  *a = Geocent_a;
  *f = Geocent_f;
} /* END OF Get_Geocentric_Parameters */


long Convert_Geodetic_To_Geocentric (double Latitude,
                                     double Longitude,
                                     double Height,
                                     double *X,
                                     double *Y,
                                     double *Z) 
{ /* BEGIN Convert_Geodetic_To_Geocentric */
/*
 * The function Convert_Geodetic_To_Geocentric converts geodetic coordinates
 * (latitude, longitude, and height) to geocentric coordinates (X, Y, Z),
 * according to the current ellipsoid parameters.
 *
 *    Latitude  : Geodetic latitude in radians                     (input)
 *    Longitude : Geodetic longitude in radians                    (input)
 *    Height    : Geodetic height, in meters                       (input)
 *    X         : Calculated Geocentric X coordinate, in meters    (output)
 *    Y         : Calculated Geocentric Y coordinate, in meters    (output)
 *    Z         : Calculated Geocentric Z coordinate, in meters    (output)
 *
 */
  long Error_Code = GEOCENT_NO_ERROR;
  double Rn;            /*  Earth radius at location  */
  double Sin_Lat;       /*  sin(Latitude)  */
  double Sin2_Lat;      /*  Square of sin(Latitude)  */
  double Cos_Lat;       /*  cos(Latitude)  */

#if 0
  if ((Latitude < -PI_OVER_2) || (Latitude > PI_OVER_2))
  { /* Latitude out of range */
    Error_Code |= GEOCENT_LAT_ERROR;
  }
  if ((Longitude < -PI) || (Longitude > (2*PI)))
  { /* Longitude out of range */
    Error_Code |= GEOCENT_LON_ERROR;
  }
#endif
  if (!Error_Code)
  { /* no errors */
    if (Longitude > PI)
      Longitude -= (2*PI);
    Sin_Lat = sin(Latitude);
    Cos_Lat = cos(Latitude);
    Sin2_Lat = Sin_Lat * Sin_Lat;
    Rn = Geocent_a / (sqrt(1.0e0 - Geocent_e2 * Sin2_Lat));
    *X = (Rn + Height) * Cos_Lat * cos(Longitude);
    *Y = (Rn + Height) * Cos_Lat * sin(Longitude);
    *Z = ((Rn * (1 - Geocent_e2)) + Height) * Sin_Lat;

  }
  return (Error_Code);
} /* END OF Convert_Geodetic_To_Geocentric */

void Convert_Geocentric_To_Geodetic (double X,
                                     double Y, 
                                     double Z,
                                     double *Latitude,
                                     double *Longitude,
                                     double *Height)
{
   /* BEGIN Convert_Geocentric_To_Geodetic */
   /*
    * The function Convert_Geocentric_To_Geodetic converts geocentric
    * coordinates (X, Y, Z) to geodetic coordinates (latitude, longitude, 
    * and height), according to the current ellipsoid parameters.
    *
    *    X         : Geocentric X coordinate, in meters.         (input)
    *    Y         : Geocentric Y coordinate, in meters.         (input)
    *    Z         : Geocentric Z coordinate, in meters.         (input)
    *    Latitude  : Calculated latitude value in radians.       (output)
    *    Longitude : Calculated longitude value in radians.      (output)
    *    Height    : Calculated height value, in meters.         (output)
    *
    */

   double d = sqrt(pow(X, 2) + pow(Y, 2));

   double phi2 = Z / ((1 - Geocent_e2) * d);
   double p = 1.0;
   double phi1 = 0.0;
   double N1 = 0.0;
   double height1 = 0.0;
   int iterIdx = 0;
   const int MAX_ITER = 10;
   if (phi2 != 0.0)
   {
      while ( (p > 1e-16) && (iterIdx < MAX_ITER))
      {
         phi1 = phi2;
         N1 = Geocent_a / sqrt(1.0 - (Geocent_e2 * pow(sin(phi1), 2.0)));
         height1 = (d / cos(phi1) - N1);
         phi2 = atan((Z / d) * (1.0 + (Geocent_e2 * N1 * sin(phi1)) / Z));
         p = fabs(phi2 - phi1);
         ++iterIdx;
         /* printf("phi: %e   phi2: %e   p: %e  \n", phi1, phi2, p); */
      }                                                                                           
   }                                                                                                   
   else
   {                                                                                              
      phi1 = phi2;                                                                                
      N1 = Geocent_a / sqrt(1.0 - (Geocent_e2 * pow(sin(phi1), 2.0)));                    
      height1 = (d / cos(phi1)) - N1;                                                             
   }                                                                                                   

   /* *Latitude = phi2 * 180/PI; */
   /* *Longitude = atan2(Y, X) * 180/PI; */
   *Latitude = phi2;                                                                       
   *Longitude = atan2(Y, X);                                                                
   *Height = height1; 
} /* END OF Convert_Geocentric_To_Geodetic */


#if 0
void Convert_Geocentric_To_Geodetic (double X,
                                     double Y, 
                                     double Z,
                                     double *Latitude,
                                     double *Longitude,
                                     double *Height)
{ /* BEGIN Convert_Geocentric_To_Geodetic */
/*
 * The function Convert_Geocentric_To_Geodetic converts geocentric
 * coordinates (X, Y, Z) to geodetic coordinates (latitude, longitude, 
 * and height), according to the current ellipsoid parameters.
 *
 *    X         : Geocentric X coordinate, in meters.         (input)
 *    Y         : Geocentric Y coordinate, in meters.         (input)
 *    Z         : Geocentric Z coordinate, in meters.         (input)
 *    Latitude  : Calculated latitude value in radians.       (output)
 *    Longitude : Calculated longitude value in radians.      (output)
 *    Height    : Calculated height value, in meters.         (output)
 *
 * The method used here is derived from 'An Improved Algorithm for
 * Geocentric to Geodetic Coordinate Conversion', by Ralph Toms, Feb 1996
 */

/* Note: Variable names follow the notation used in Toms, Feb 1996 */

  double W;        /* distance from Z axis */
  double W2;       /* square of distance from Z axis */
  double T0;       /* initial estimate of vertical component */
  double T1;       /* corrected estimate of vertical component */
  double S0;       /* initial estimate of horizontal component */
  double S1;       /* corrected estimate of horizontal component */
  double Sin_B0;   /* sin(B0), B0 is estimate of Bowring aux variable */
  double Sin3_B0;  /* cube of sin(B0) */
  double Cos_B0;   /* cos(B0) */
  double Sin_p1;   /* sin(phi1), phi1 is estimated latitude */
  double Cos_p1;   /* cos(phi1) */
  double Rn;       /* Earth radius at location */
  double Sum;      /* numerator of cos(phi1) */
  int At_Pole;     /* indicates location is in polar region */
  double Geocent_b = Geocent_a * (1 - Geocent_f); /* Semi-minor axis of ellipsoid, in meters */

  At_Pole = FALSE;
  if (X != 0.0)
  {
    *Longitude = atan2(Y,X);
  }
  else
  {
    if (Y > 0)
    {
      *Longitude = PI_OVER_2;
    }
    else if (Y < 0)
    {
      *Longitude = -PI_OVER_2;
    }
    else
    {
      At_Pole = TRUE;
      *Longitude = 0.0;
      if (Z > 0.0)
      {  /* north pole */
        *Latitude = PI_OVER_2;
      }
      else if (Z < 0.0)
      {  /* south pole */
        *Latitude = -PI_OVER_2;
      }
      else
      {  /* center of earth */
        *Latitude = PI_OVER_2;
        *Height = -Geocent_b;
        return;
      } 
    }
  }
  W2 = X*X + Y*Y;
  W = sqrt(W2);
  T0 = Z * AD_C;
  S0 = sqrt(T0 * T0 + W2);
  Sin_B0 = T0 / S0;
  Cos_B0 = W / S0;
  Sin3_B0 = Sin_B0 * Sin_B0 * Sin_B0;
  T1 = Z + Geocent_b * Geocent_ep2 * Sin3_B0;
  Sum = W - Geocent_a * Geocent_e2 * Cos_B0 * Cos_B0 * Cos_B0;
  S1 = sqrt(T1*T1 + Sum * Sum);
  Sin_p1 = T1 / S1;
  Cos_p1 = Sum / S1;
  Rn = Geocent_a / sqrt(1.0 - Geocent_e2 * Sin_p1 * Sin_p1);
  if (Cos_p1 >= COS_67P5)
  {
    *Height = W / Cos_p1 - Rn;
  }
  else if (Cos_p1 <= -COS_67P5)
  {
    *Height = W / -Cos_p1 - Rn;
  }
  else
  {
    *Height = Z / Sin_p1 + Rn * (Geocent_e2 - 1.0);
  }
  if (At_Pole == FALSE)
  {
    *Latitude = atan(Sin_p1 / Cos_p1);
  }
} /* END OF Convert_Geocentric_To_Geodetic */
#endif
