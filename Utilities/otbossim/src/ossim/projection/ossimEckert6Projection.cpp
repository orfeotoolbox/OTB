//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Calls Geotrans Eckert6 projection code.  
//*******************************************************************
//  $Id: ossimEckert6Projection.cpp 17815 2010-08-03 13:23:14Z dburken $

/***************************************************************************/
/*
 *                               DEFINES
 */
#include <ossim/projection/ossimEckert6Projection.h>

#include <ossim/base/ossimKeywordNames.h>

RTTI_DEF1(ossimEckert6Projection, "ossimEckert6Projection", ossimMapProjection)

#define ECK6_NO_ERROR           0x0000
#define ECK6_LAT_ERROR          0x0001
#define ECK6_LON_ERROR          0x0002
#define ECK6_EASTING_ERROR      0x0004
#define ECK6_NORTHING_ERROR     0x0008
#define ECK6_CENT_MER_ERROR     0x0020
#define ECK6_A_ERROR            0x0040
#define ECK6_B_ERROR            0x0080
#define ECK6_A_LESS_B_ERROR     0x0100

#ifndef PI_OVER_2
#  define PI_OVER_2  ( M_PI / 2.0)
#endif
#ifndef TWO_PI
#  define TWO_PI     (2.0 * M_PI)
#endif

#define MAX_LAT    ( (M_PI * 90.0) / 180.0 )  /* 90 degrees in radians    */

/***************************************************************************/
/*
 *                               GLOBALS
 */

const double one_PLUS_PI_OVER_2 = (1.0 + M_PI / 2.0);

ossimEckert6Projection::ossimEckert6Projection(const ossimEllipsoid& ellipsoid,
                                               const ossimGpt& origin)
   :ossimMapProjection(ellipsoid, origin)
{
   setDefaults();
}

ossimEckert6Projection::ossimEckert6Projection(const ossimEllipsoid& ellipsoid,
                                               const ossimGpt& origin,
                                               double falseEasting,
                                               double falseNorthing)
   :ossimMapProjection(ellipsoid, origin)
{
   Eck6_False_Easting  = falseEasting;
   Eck6_False_Northing = falseNorthing;
   Eck6_Delta_Northing  = 8451144.0;
   Eck6_Max_Easting     = 16902288.0;
   Eck6_Min_Easting     = -16902288.0;
   
   update();
}

void ossimEckert6Projection::update()
{
   Set_Eckert6_Parameters(theEllipsoid.getA(),
                          theEllipsoid.getFlattening(),
                          theOrigin.lonr(),
                          Eck6_False_Easting,
                          Eck6_False_Northing);

   theFalseEastingNorthing.x = Eck6_False_Easting;
   theFalseEastingNorthing.y = Eck6_False_Northing;

   ossimMapProjection::update();
}

ossimGpt ossimEckert6Projection::inverse(const ossimDpt &eastingNorthing)const
{
   double lat = 0.0;
   double lon = 0.0;
   
   Convert_Eckert6_To_Geodetic(eastingNorthing.x,
                               eastingNorthing.y,
                               &lat,
                               &lon);
   
   return ossimGpt(lat*DEG_PER_RAD, lon*DEG_PER_RAD, 0.0, theDatum);  
}

ossimDpt ossimEckert6Projection::forward(const ossimGpt &latLon)const
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

   Convert_Geodetic_To_Eckert6(gpt.latr(),
                               gpt.lonr(),
                               &easting,
                               &northing);
   
   return ossimDpt(easting, northing);
}

void ossimEckert6Projection::setFalseEasting(double falseEasting)
{
   Eck6_False_Easting = falseEasting;
   update();
}

void ossimEckert6Projection::setFalseNorthing(double falseNorthing)
{
   Eck6_False_Northing = falseNorthing;
   update();
}

void ossimEckert6Projection::setFalseEastingNorthing(double falseEasting,
                                                        double falseNorthing)
{
   Eck6_False_Easting = falseEasting;
   Eck6_False_Northing = falseNorthing;
   update();
}

void ossimEckert6Projection::setDefaults()
{
   Eck6_Delta_Northing  = 8451144.0;
   Eck6_Max_Easting     = 16902288.0;
   Eck6_Min_Easting     = -16902288.0;
   Eck6_False_Easting   = 0.0;
   Eck6_False_Northing  = 0.0;
   
   update();
}

void ossimEckert6Projection::setCentralMeridian(double centralMeridian)
{
  Eck6_Origin_Long = centralMeridian;
  update();
}

bool ossimEckert6Projection::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   return ossimMapProjection::saveState(kwl, prefix);
}

bool ossimEckert6Projection::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   bool flag = ossimMapProjection::loadState(kwl, prefix);
   
   const char* type          = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   
   setDefaults();
   
   if(ossimString(type) == STATIC_TYPE_NAME(ossimEckert6Projection))
   {
      Eck6_False_Easting  = theFalseEastingNorthing.x;
      Eck6_False_Northing = theFalseEastingNorthing.y;
   }
   
   update();
   
   return flag;
}

/***************************************************************************/
/*
 *                              FUNCTIONS
 */


long ossimEckert6Projection::Set_Eckert6_Parameters(double a,
                                                    double f,
                                                    double Central_Meridian,
                                                    double False_Easting,
                                                    double False_Northing)
{ /* Begin Set_Eckert6_Parameters */
/*
 * The function Set_Eckert6_Parameters receives the ellipsoid parameters and
 * Eckert VI projection parameters as inputs, and sets the corresponding state
 * variables.  If any errors occur, the error code(s) are returned by the 
 * function, otherwise ECK6_NO_ERROR is returned.
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

  double Ra;                      /* Spherical radius */
//  double inv_f = 1 / f;
  long Error_Code = ECK6_NO_ERROR;

//   if (a <= 0.0)
//   { /* Semi-major axis must be greater than zero */
//     Error_Code |= ECK6_A_ERROR;
//   }
//   if ((inv_f < 250) || (inv_f > 350))
//   { /* Inverse flattening must be between 250 and 350 */
//     Error_Code |= ECK6_INV_F_ERROR;
//   }
//   if ((Central_Meridian < -PI) || (Central_Meridian > TWO_PI))
//   { /* origin longitude out of range */
//     Error_Code |= ECK6_CENT_MER_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */
    Eck6_a = a;
    Eck6_f = f;
    es2 = 2 * Eck6_f - Eck6_f * Eck6_f;
    es4 = es2 * es2;
    es6 = es4 * es2;
    /* spherical radius */
    Ra = Eck6_a * (1.0 - es2 / 6.0 - 17.0 * es4 / 360.0 - 67.0 * es6 /3024.0);
    Ra_Over_Sqrt_Two_Plus_PI = Ra / (sqrt(2.0 + M_PI));
    Inv_Ra_Over_Sqrt_Two_Plus_PI = 1 / Ra_Over_Sqrt_Two_Plus_PI;
//     if (Central_Meridian > PI)
//       Central_Meridian -= TWO_PI;
    Eck6_Origin_Long = Central_Meridian;
    Eck6_False_Easting = False_Easting;
    Eck6_False_Northing = False_Northing;
    if (Eck6_Origin_Long > 0)
    {
      Eck6_Max_Easting = 17555761.0;
      Eck6_Min_Easting = -17653839.0;
    }
    else if (Eck6_Origin_Long < 0)
    {
      Eck6_Max_Easting = 17653838.0;
      Eck6_Min_Easting = -17555761.0;
    }
    else
    {
      Eck6_Max_Easting = 17653838.0;
      Eck6_Min_Easting = -17653838.0;
    }

  } /* End if(!Error_Code) */
  return (Error_Code);
} /* End Set_Eckert6_Parameters */


void ossimEckert6Projection::Get_Eckert6_Parameters(double *a,
                                                    double *f,
                                                    double *Central_Meridian,
                                                    double *False_Easting,
                                                    double *False_Northing)const
{ /* Begin Get_Eckert6_Parameters */
/*
 * The function Get_Eckert6_Parameters returns the current ellipsoid
 * parameters and Eckert VI projection parameters.
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

  *a = Eck6_a;
  *f = Eck6_f;
  *Central_Meridian = Eck6_Origin_Long;
  *False_Easting = Eck6_False_Easting;
  *False_Northing = Eck6_False_Northing;
  
  return;
} /* End Get_Eckert6_Parameters */


long ossimEckert6Projection::Convert_Geodetic_To_Eckert6 (double Latitude,
                                                          double Longitude,
                                                          double *Easting,
                                                          double *Northing)const

{ /* Begin Convert_Geodetic_To_Eckert6 */
/*
 * The function Convert_Geodetic_To_Eckert6 converts geodetic (latitude and
 * longitude) coordinates to Eckert VI projection (easting and northing)
 * coordinates, according to the current ellipsoid and Eckert VI projection
 * parameters.  If any errors occur, the error code(s) are returned by the
 * function, otherwise ECK6_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi) in radians           (input)
 *    Longitude         : Longitude (lambda) in radians       (input)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */

  double slat = sin(Latitude);
  double dlam;     /* Longitude - Central Meridan */
  double theta = Latitude;
  double delta_theta = 1.0;
  double dt_tolerance = 4.85e-10;        /* approximately 1/1000th of
                                             an arc second or 1/10th meter */
  long Error_Code = ECK6_NO_ERROR;

//   if ((Latitude < -PI_OVER_2) || (Latitude > PI_OVER_2))
//   {  /* Latitude out of range */
//     Error_Code |= ECK6_LAT_ERROR;
//   }
//   if ((Longitude < -PI) || (Longitude > TWO_PI))
//   {  /* Longitude out of range */
//     Error_Code|= ECK6_LON_ERROR;
//   }

  if (!Error_Code)
  { /* no errors */

    dlam = Longitude - Eck6_Origin_Long;
//     if (dlam > PI)
//     {
//       dlam -= TWO_PI;
//     }
//     if (dlam < -PI)
//     {
//       dlam += TWO_PI;
//     }
    while (fabs(delta_theta) > dt_tolerance)
    {
      delta_theta = -(theta + sin(theta) - one_PLUS_PI_OVER_2 *
                      slat) / (1.0 + cos(theta));
      theta += delta_theta;
    }
    *Easting = Ra_Over_Sqrt_Two_Plus_PI * dlam * (1.0 + cos(theta)) +
               Eck6_False_Easting;
    *Northing = 2.0 * Ra_Over_Sqrt_Two_Plus_PI  * theta + Eck6_False_Northing;

  }
  return (Error_Code);

} /* End Convert_Geodetic_To_Eckert6 */


long ossimEckert6Projection::Convert_Eckert6_To_Geodetic(double Easting,
                                                         double Northing,
                                                         double *Latitude,
                                                         double *Longitude)const
{ /* Begin Convert_Eckert6_To_Geodetic */
/*
 * The function Convert_Eckert6_To_Geodetic converts Eckert VI projection
 * (easting and northing) coordinates to geodetic (latitude and longitude)
 * coordinates, according to the current ellipsoid and Eckert VI projection
 * coordinates.  If any errors occur, the error code(s) are returned by the
 * function, otherwise ECK6_NO_ERROR is returned.
 *
 *    Easting           : Easting (X) in meters                  (input)
 *    Northing          : Northing (Y) in meters                 (input)
 *    Latitude          : Latitude (phi) in radians              (output)
 *    Longitude         : Longitude (lambda) in radians          (output)
 */

  double dx, dy;
  double theta;
  double i;

  long Error_Code = ECK6_NO_ERROR;

//   if ((Easting < (Eck6_False_Easting + Eck6_Min_Easting))
//       || (Easting > (Eck6_False_Easting + Eck6_Max_Easting)))
//   { /* Easting out of range  */
//     Error_Code |= ECK6_EASTING_ERROR;
//   }
//   if ((Northing < (Eck6_False_Northing - Eck6_Delta_Northing))
//       || (Northing > (Eck6_False_Northing + Eck6_Delta_Northing)))
//   { /* Northing out of range */
//     Error_Code |= ECK6_NORTHING_ERROR;
//   }

  if (!Error_Code)
  {
    dy = Northing - Eck6_False_Northing;
    dx = Easting - Eck6_False_Easting;
    theta = Inv_Ra_Over_Sqrt_Two_Plus_PI * dy / 2.0;
    i = (theta + sin(theta)) / one_PLUS_PI_OVER_2;
    if (i > 1.0)
      *Latitude = MAX_LAT;
    else if (i < -1.0)
      *Latitude = -MAX_LAT;
    else
      *Latitude = asin(i);
    *Longitude = Eck6_Origin_Long + Inv_Ra_Over_Sqrt_Two_Plus_PI * dx / (1 + cos(theta));

//     if (*Latitude > PI_OVER_2)  /* force distorted values to 90, -90 degrees */
//       *Latitude = PI_OVER_2;
//     else if (*Latitude < -PI_OVER_2)
//       *Latitude = -PI_OVER_2;

//     if (*Longitude > PI)
//       *Longitude -= TWO_PI;
//     if (*Longitude < -PI)
//       *Longitude += TWO_PI;

//     if (*Longitude > PI)  /* force distorted values to 180, -180 degrees */
//       *Longitude = PI;
//     else if (*Longitude < -PI)
//       *Longitude = -PI;

  }
  return (Error_Code);

} /* End Convert_Eckert6_To_Geodetic */
