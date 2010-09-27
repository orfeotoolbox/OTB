//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Calls Geotrans Sinusoidal projection code.  
//*******************************************************************
//  $Id: ossimSinusoidalProjection.cpp 17815 2010-08-03 13:23:14Z dburken $

#include <math.h>
#include <ossim/projection/ossimSinusoidalProjection.h>
#include <ossim/base/ossimKeywordNames.h>

RTTI_DEF1(ossimSinusoidalProjection, "ossimSinusoidalProjection", ossimMapProjection)
/***************************************************************************/
/*
 *                              DEFINES
 */

#define SINU_NO_ERROR           0x0000
#define SINU_LAT_ERROR          0x0001
#define SINU_LON_ERROR          0x0002
#define SINU_EASTING_ERROR      0x0004
#define SINU_NORTHING_ERROR     0x0008
#define SINU_CENT_MER_ERROR     0x0020
#define SINU_A_ERROR            0x0040
#define SINU_B_ERROR            0x0080
#define SINU_A_LESS_B_ERROR     0x0100

#ifndef PI_OVER_2
#  define PI_OVER_2  ( M_PI / 2.0)
#endif
#ifndef TWO_PI
#  define TWO_PI     (2.0 * M_PI)
#endif

#define SINU_COEFF_TIMES_SIN(coeff, x, latit)      (coeff * sin(x * latit))
#define FLOAT_EQ(x,v,epsilon)   (((v - epsilon) < x) && (x < (v + epsilon)))

/***************************************************************************/
/*
 *                              FUNCTIONS     
 */

ossimSinusoidalProjection::ossimSinusoidalProjection(const ossimEllipsoid& ellipsoid,
                                                     const ossimGpt& origin)
   :ossimMapProjection(ellipsoid, origin)
{
   setDefaults();
   update();
}

ossimSinusoidalProjection::ossimSinusoidalProjection(const ossimEllipsoid& ellipsoid,
                                                     const ossimGpt& origin,
                                                     double falseEasting,
                                                     double falseNorthing)
   :ossimMapProjection(ellipsoid, origin)
{
   Sinu_False_Easting  = falseEasting;
   Sinu_False_Northing = falseNorthing;
   Sinu_Max_Easting    = 20037509.0;
   Sinu_Min_Easting    = -20037509.0;
   Sinu_Delta_Northing = 10001966.0;

   update();
}

void ossimSinusoidalProjection::update()
{   
   Set_Sinusoidal_Parameters(theEllipsoid.getA(),
                             theEllipsoid.getFlattening(),
                             theOrigin.lonr(),
                             Sinu_False_Easting,
                             Sinu_False_Northing);

   theFalseEastingNorthing.x = Sinu_False_Easting;
   theFalseEastingNorthing.y = Sinu_False_Northing;
 
   ossimMapProjection::update();
}

void ossimSinusoidalProjection::setFalseEasting(double falseEasting)
{
   Sinu_False_Easting = falseEasting;
   
   update();
}

void ossimSinusoidalProjection::setFalseNorthing(double falseNorthing)
{
   Sinu_False_Northing = falseNorthing;
   
   update();
}

void ossimSinusoidalProjection::setDefaults()
{
   Sinu_Max_Easting    = 20037509.0;
   Sinu_Min_Easting    = -20037509.0;
   Sinu_Delta_Northing = 10001966.0;
   Sinu_False_Easting  = 0.0;
   Sinu_False_Northing = 0.0;
}

void ossimSinusoidalProjection::setCentralMeridian(double centralMeridian)
{
  Sinu_Origin_Long = centralMeridian;
  update();
}

void ossimSinusoidalProjection::setFalseEastingNorthing(double falseEasting,
                                                        double falseNorthing)
{
   Sinu_False_Easting  = falseEasting;
   Sinu_False_Northing = falseNorthing;
   
   update();
}

ossimGpt ossimSinusoidalProjection::inverse(const ossimDpt &eastingNorthing)const
{
   double lat = 0.0;
   double lon = 0.0;
   
   Convert_Sinusoidal_To_Geodetic(eastingNorthing.x,
                                  eastingNorthing.y,
                                  &lat,
                                  &lon);   
   return ossimGpt(lat*DEG_PER_RAD, lon*DEG_PER_RAD, 0.0, theDatum);  
}

ossimDpt ossimSinusoidalProjection::forward(const ossimGpt &latLon)const
{
   double easting  = 0.0;
   double northing = 0.0;
   ossimGpt gpt = latLon;
   
   if (theDatum)
   {
      if (theDatum->code() != latLon.datum()->code())
      {
         gpt.changeDatum(theDatum); // Shift to our datum.
      }
   }

   Convert_Geodetic_To_Sinusoidal(gpt.latr(),
                                  gpt.lonr(),
                                  &easting,
                                  &northing);   
   return ossimDpt(easting, northing);
}

bool ossimSinusoidalProjection::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   return ossimMapProjection::saveState(kwl, prefix);
}

bool ossimSinusoidalProjection::loadState(const ossimKeywordlist& kwl,
                                          const char* prefix)
{
   bool flag = ossimMapProjection::loadState(kwl, prefix);
   const char* type          = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   
   setDefaults();

   if(ossimString(type) == STATIC_TYPE_NAME(ossimSinusoidalProjection))
   {
      Sinu_False_Easting  = theFalseEastingNorthing.x;
      Sinu_False_Northing = theFalseEastingNorthing.y;
   }
   
   update();

   return flag;
}

/***************************************************************************/
/*
 *                              FUNCTIONS     
 */


long ossimSinusoidalProjection::Set_Sinusoidal_Parameters(double a,
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
//  double inv_f = 1 / f;
  long Error_Code = SINU_NO_ERROR;

//   if (a <= 0.0)
//   { /* Semi-major axis must be greater than zero */
//     Error_Code |= SINU_A_ERROR;
//   }
//   if ((inv_f < 250) || (inv_f > 350))
//   { /* Inverse flattening must be between 250 and 350 */
//     Error_Code |= SINU_INV_F_ERROR;
//   }
//   if ((Central_Meridian < -M_PI) || (Central_Meridian > TWO_PI))
//   { /* origin longitude out of range */
//     Error_Code |= SINU_CENT_MER_ERROR;
//   }
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
//     if (Central_Meridian > M_PI)
//       Central_Meridian -= TWO_PI;
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

void ossimSinusoidalProjection::Get_Sinusoidal_Parameters(double *a,
                                                          double *f,
                                                          double *Central_Meridian,
                                                          double *False_Easting,
                                                          double *False_Northing)const
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


long ossimSinusoidalProjection::Convert_Geodetic_To_Sinusoidal (double Latitude,
                                                                double Longitude,
                                                                double *Easting,
                                                                double *Northing)const
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

//   if ((Latitude < -PI_OVER_2) || (Latitude > PI_OVER_2))
//   { /* Latitude out of range */
//     Error_Code |= SINU_LAT_ERROR;
//   }
//   if ((Longitude < -M_PI) || (Longitude > TWO_PI))
//   { /* Longitude out of range */
//     Error_Code |= SINU_LON_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */
    dlam = Longitude - Sinu_Origin_Long;
//     if (dlam > M_PI)
//     {
//       dlam -= TWO_PI;
//     }
//     if (dlam < -M_PI)
//     {
//       dlam += TWO_PI;
//     }
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


long ossimSinusoidalProjection::Convert_Sinusoidal_To_Geodetic(double Easting,
                                                               double Northing,
                                                               double *Latitude,
                                                               double *Longitude)const
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

//   if ((Easting < (Sinu_False_Easting + Sinu_Min_Easting))
//       || (Easting > (Sinu_False_Easting + Sinu_Max_Easting)))
//   { /* Easting out of range */
//     Error_Code |= SINU_EASTING_ERROR;
//   }
//   if ((Northing < (Sinu_False_Northing - Sinu_Delta_Northing))
//       || (Northing > (Sinu_False_Northing + Sinu_Delta_Northing)))
//   { /* Northing out of range */
//     Error_Code |= SINU_NORTHING_ERROR;
//   }
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


//       if (*Longitude > M_PI)
//         *Longitude -= TWO_PI;
//       if (*Longitude < -M_PI)
//         *Longitude += TWO_PI;

//       if (*Longitude > M_PI)  /* force distorted values to 180, -180 degrees */
//         *Longitude = M_PI;
//       else if (*Longitude < -M_PI)
//         *Longitude = -M_PI;
    }
  }
  return (Error_Code);
} /* END OF Convert_Sinusoidal_To_Geodetic */

//*************************************************************************************************
//! Returns TRUE if principal parameters are within epsilon tolerance.
//*************************************************************************************************
bool ossimSinusoidalProjection::operator==(const ossimProjection& proj) const
{
   if (!ossimMapProjection::operator==(proj))
      return false;

   ossimSinusoidalProjection* p = PTR_CAST(ossimSinusoidalProjection, &proj);
   if (!p) return false;

   if (!ossim::almostEqual(Sinu_Origin_Long,p->Sinu_Origin_Long)) return false;

   return true;
}
