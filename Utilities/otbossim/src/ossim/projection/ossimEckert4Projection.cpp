//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Calls Geotrans Eckert4 projection code.  
//*******************************************************************
//  $Id: ossimEckert4Projection.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <ossim/projection/ossimEckert4Projection.h>

#include <ossim/base/ossimKeywordNames.h>

RTTI_DEF1(ossimEckert4Projection, "ossimEckert4Projection", ossimMapProjection)


/***************************************************************************/
/*
 *                               DEFINES
 */

#ifndef PI_OVER_2
#  define PI_OVER_2  ( M_PI / 2.0)
#endif
#ifndef TWO_PI
#  define TWO_PI     (2.0 * M_PI)
#endif
#define NUM(Theta, SinTheta, CosTheta)   (Theta + SinTheta * CosTheta + 2.0 * SinTheta)


#define ECK4_NO_ERROR           0x0000
#define ECK4_LAT_ERROR          0x0001
#define ECK4_LON_ERROR          0x0002
#define ECK4_EASTING_ERROR      0x0004
#define ECK4_NORTHING_ERROR     0x0008
#define ECK4_CENT_MER_ERROR     0x0020
#define ECK4_A_ERROR            0x0040
#define ECK4_B_ERROR            0x0080
#define ECK4_A_LESS_B_ERROR     0x0100

/***************************************************************************/
/*
 *                               GLOBALS
 */

const double two_PLUS_PI_OVER_2 = (2.0 + M_PI / 2.0);

ossimEckert4Projection::ossimEckert4Projection(const ossimEllipsoid& ellipsoid,
                                               const ossimGpt& origin)
   :ossimMapProjection(ellipsoid, origin)
{
   setDefaults();
   update();
}

ossimEckert4Projection::ossimEckert4Projection(const ossimEllipsoid& ellipsoid,
                                               const ossimGpt& origin,
                                               double falseEasting,
                                               double falseNorthing)
   :ossimMapProjection(ellipsoid, origin)
{
   
   Eck4_False_Easting  = falseEasting;
   Eck4_False_Northing = falseNorthing;
   Eck4_Delta_Northing = 8451144.0;
   Eck4_Max_Easting    = 16902288.0;
   Eck4_Min_Easting    = -16902288.0;

   update();
}


void ossimEckert4Projection::update()
{   
   
   Set_Eckert4_Parameters(theEllipsoid.getA(),
                          theEllipsoid.getFlattening(),
                          theOrigin.lonr(),
                          Eck4_False_Easting,
                          Eck4_False_Northing);

   theFalseEastingNorthing.x = Eck4_False_Easting;
   theFalseEastingNorthing.y = Eck4_False_Northing;

   ossimMapProjection::update();
}

ossimGpt ossimEckert4Projection::inverse(const ossimDpt &eastingNorthing)const
{
   double lat  = 0.0;
   double lon = 0.0;
   
   Convert_Eckert4_To_Geodetic(eastingNorthing.x,
                               eastingNorthing.y,
                               &lat,
                               &lon);
   
   return ossimGpt(lat*DEG_PER_RAD, lon*DEG_PER_RAD, 0.0, theDatum);  
}

ossimDpt ossimEckert4Projection::forward(const ossimGpt &latLon)const
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

   Convert_Geodetic_To_Eckert4(gpt.latr(),
                               gpt.lonr(),
                               &easting,
                               &northing);

   return ossimDpt(easting, northing);
}

void ossimEckert4Projection::setFalseEasting(double falseEasting)
{
   Eck4_False_Easting = falseEasting;
   update();
}

void ossimEckert4Projection::setFalseNorthing(double falseNorthing)
{
   Eck4_False_Northing = falseNorthing;
   update();
}

void ossimEckert4Projection::setFalseEastingNorthing(double falseEasting,
                                                        double falseNorthing)
{
   Eck4_False_Easting = falseEasting;
   Eck4_False_Northing = falseNorthing;
   update();
}

void ossimEckert4Projection::setDefaults()
{
   Eck4_Delta_Northing  = 8451144.0;
   Eck4_Max_Easting     = 16902288.0;
   Eck4_Min_Easting     = -16902288.0;
   Eck4_False_Easting   = 0.0;
   Eck4_False_Northing  = 0.0;
}

bool ossimEckert4Projection::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   return ossimMapProjection::saveState(kwl, prefix);
}

bool ossimEckert4Projection::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   bool flag = ossimMapProjection::loadState(kwl, prefix);
   
   const char* type          = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   
   setDefaults();
   
   if(ossimString(type) == STATIC_TYPE_NAME(ossimEckert4Projection))
   {
      Eck4_False_Easting  = theFalseEastingNorthing.x;
      Eck4_False_Northing = theFalseEastingNorthing.y;
   }
   
   update();
   
   return flag;
}

/***************************************************************************/
/*
 *                              FUNCTIONS
 */


long ossimEckert4Projection::Set_Eckert4_Parameters(double a,
                                                    double f,                
                                                    double Central_Meridian,
                                                    double False_Easting,
                                                    double False_Northing)

{ /* Begin Set_Eckert4_Parameters */
/*
 * The function Set_Eckert4_Parameters receives the ellipsoid parameters and
 * projection parameters as inputs, and sets the corresponding state
 * variables.  If any errors occur, the error code(s) are returned by the 
 * function, otherwise ECK4_NO_ERROR is returned.
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

  double Ra;              /* Spherical radius */
//  double inv_f = 1 / f;
  long Error_Code = ECK4_NO_ERROR;

//   if (a <= 0.0)
//   { /* Semi-major axis must be greater than zero */
//     Error_Code |= ECK4_A_ERROR;
//   }
//   if ((inv_f < 250) || (inv_f > 350))
//   { /* Inverse flattening must be between 250 and 350 */
//     Error_Code |= ECK4_INV_F_ERROR;
//   }
//   if ((Central_Meridian < -PI) || (Central_Meridian > TWO_PI))
//   { /* origin longitude out of range */
//     Error_Code |= ECK4_CENT_MER_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */
    Eck4_a = a;
    Eck4_f = f;
    es2 = 2 * Eck4_f - Eck4_f * Eck4_f;
    es4 = es2 * es2;
    es6 = es4 * es2;
    /* spherical radius */
    Ra = Eck4_a * (1.0 - es2 / 6.0 - 17.0 * es4 / 360.0 - 67.0 * es6 / 3024.0);
    Ra0 = 0.4222382 * Ra;
    Ra1 = 1.3265004 * Ra;
//     if (Central_Meridian > PI)
//       Central_Meridian -= TWO_PI;
    Eck4_Origin_Long = Central_Meridian;
    Eck4_False_Easting = False_Easting;
    Eck4_False_Northing = False_Northing;
    if (Eck4_Origin_Long > 0)
    {
      Eck4_Max_Easting = 16808386.0;
      Eck4_Min_Easting = -16902288.0;
    }
    else if (Eck4_Origin_Long < 0)
    {
      Eck4_Max_Easting = 16902288.0;
      Eck4_Min_Easting = -16808386.0;
    }
    else
    {
      Eck4_Max_Easting = 16902288.0;
      Eck4_Min_Easting = -16902288.0;
    }

  } /* End if(!Error_Code) */
  return (Error_Code);
} /* End Set_Eckert4_Parameters */


void ossimEckert4Projection::Get_Eckert4_Parameters(double *a,
                                                    double *f,
                                                    double *Central_Meridian,
                                                    double *False_Easting,
                                                    double *False_Northing)const
{ /* Begin Get_Eckert4_Parameters */
/*
 * The function Get_Eckert4_Parameters returns the current ellipsoid
 * parameters and Eckert IV projection parameters.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters   (output)
 *    f                 : Flattening of ellipsoid		    (output)
 *    Central_Meridian  : Longitude in radians at the center of     (output)
 *                          the projection
 *    False_Easting     : A coordinate value in meters assigned to the
 *                          central meridian of the projection.     (output)
 *    False_Northing    : A coordinate value in meters assigned to the
 *                          origin latitude of the projection       (output)
 */

  *a = Eck4_a;
  *f = Eck4_f;
  *Central_Meridian = Eck4_Origin_Long;
  *False_Easting = Eck4_False_Easting;
  *False_Northing = Eck4_False_Northing;
  return;
} /* End Get_Eckert4_Parameters */


long ossimEckert4Projection::Convert_Geodetic_To_Eckert4 (double Latitude,
                                                          double Longitude,
                                                          double *Easting,
                                                          double *Northing)const

{ /* Begin Convert_Geodetic_To_Eckert4 */
/*
 * The function Convert_Geodetic_To_Eckert4 converts geodetic (latitude and
 * longitude) coordinates to Eckert IV projection (easting and northing)
 * coordinates, according to the current ellipsoid, spherical radius and
 * Eckert IV projection parameters.
 * If any errors occur, the error code(s) are returned by the
 * function, otherwise ECK4_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi) in radians           (input)
 *    Longitude         : Longitude (lambda) in radians       (input)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */

  double slat = sin(Latitude);
  double sin_theta, cos_theta;
  double num;
  double dlam;     /* Longitude - Central Meridan */
  double theta = Latitude / 2.0;
  double delta_theta = 1.0;
  double dt_tolerance = 4.85e-10;        /* approximately 1/1000th of
                                            an arc second or 1/10th meter */
  long Error_Code = ECK4_NO_ERROR;

//   if ((Latitude < -PI_OVER_2) || (Latitude > PI_OVER_2))
//   {  /* Latitude out of range */
//     Error_Code |= ECK4_LAT_ERROR;
//   }
//   if ((Longitude < -PI) || (Longitude > TWO_PI))
//   {  /* Longitude out of range */
//     Error_Code|= ECK4_LON_ERROR;
//   }

  if (!Error_Code)
  { /* no errors */

    dlam = Longitude - Eck4_Origin_Long;
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
       sin_theta = sin(theta);
       cos_theta = cos(theta);
       num = NUM(theta, sin_theta, cos_theta);
       delta_theta = -(num - two_PLUS_PI_OVER_2 * slat) /
                     (2.0 * cos_theta * (1.0 + cos_theta));
       theta += delta_theta;
    }
    *Easting = Ra0 * dlam * (1.0 + cos(theta)) + Eck4_False_Easting;
    *Northing = Ra1 * sin(theta) + Eck4_False_Northing; 
  }
  
  return (Error_Code);

} /* End Convert_Geodetic_To_Eckert4 */


long ossimEckert4Projection::Convert_Eckert4_To_Geodetic(double Easting,
                                                         double Northing,
                                                         double *Latitude,
                                                         double *Longitude)const
{ /* Begin Convert_Eckert4_To_Geodetic */
/*
 * The function Convert_Eckert4_To_Geodetic converts Eckert IV projection
 * (easting and northing) coordinates to geodetic (latitude and longitude)
 * coordinates, according to the current ellipsoid, spherical radius and
 * Eckert IV projection coordinates.
 * If any errors occur, the error code(s) are returned by the
 * function, otherwise ECK4_NO_ERROR is returned.
 *
 *    Easting           : Easting (X) in meters                  (input)
 *    Northing          : Northing (Y) in meters                 (input)
 *    Latitude          : Latitude (phi) in radians              (output)
 *    Longitude         : Longitude (lambda) in radians          (output)
 */

  double theta;
  double sin_theta, cos_theta;
  double num;
  double dx, dy;
  double i;

  long Error_Code = ECK4_NO_ERROR;

//   if ((Easting < (Eck4_False_Easting + Eck4_Min_Easting))
//       || (Easting > (Eck4_False_Easting + Eck4_Max_Easting)))
//   { /* Easting out of range  */
//     Error_Code |= ECK4_EASTING_ERROR;
//   }
//   if ((Northing < (Eck4_False_Northing - Eck4_Delta_Northing)) 
//       || (Northing > (Eck4_False_Northing + Eck4_Delta_Northing)))
//   { /* Northing out of range */
//     Error_Code |= ECK4_NORTHING_ERROR;
//   }

  if (!Error_Code)
  {
    dy = Northing - Eck4_False_Northing;
    dx = Easting - Eck4_False_Easting;
    i = dy / Ra1;
    if (i > 1.0)
      i = 1.0;
    else if (i < -1.0)
      i = -1.0;

    theta = asin(i);
    sin_theta = sin(theta);
    cos_theta = cos(theta);
    num = NUM(theta, sin_theta, cos_theta);

    *Latitude = asin(num / two_PLUS_PI_OVER_2);
    *Longitude = Eck4_Origin_Long + dx / (Ra0 * (1 + cos_theta));

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

} /* End Convert_Eckert4_To_Geodetic */

