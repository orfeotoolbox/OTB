//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Calls Geotrans Miller projection code.  
//*******************************************************************
//  $Id: ossimMillerProjection.cpp 17815 2010-08-03 13:23:14Z dburken $

#include <math.h>
#include <ossim/projection/ossimMillerProjection.h>
#include <ossim/base/ossimKeywordNames.h>

RTTI_DEF1(ossimMillerProjection, "ossimMillerProjection", ossimMapProjection)
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

#define MILL_NO_ERROR           0x0000
#define MILL_LAT_ERROR          0x0001
#define MILL_LON_ERROR          0x0002
#define MILL_EASTING_ERROR      0x0004
#define MILL_NORTHING_ERROR     0x0008
#define MILL_CENT_MER_ERROR     0x0020
#define MILL_A_ERROR            0x0040
#define MILL_B_ERROR            0x0080
#define MILL_A_LESS_B_ERROR     0x0100

ossimMillerProjection::ossimMillerProjection(const ossimEllipsoid& ellipsoid,
                                             const ossimGpt& origin)
   :ossimMapProjection(ellipsoid, origin)
{
   setDefaults();
   update();
//    Mill_Delta_Northing = 14675058.0;
//    Mill_Max_Easting    = 20015110.0;
//    Mill_Min_Easting    = -20015110.0;
}

ossimMillerProjection::ossimMillerProjection(const ossimEllipsoid& ellipsoid,
                                             const ossimGpt& origin,
                                             double falseEasting,
                                             double falseNorthing)
   :ossimMapProjection(ellipsoid, origin)
{

   Mill_False_Easting  = falseEasting;
   Mill_False_Northing = falseNorthing;
   Mill_Delta_Northing = 14675058.0;
   Mill_Max_Easting    = 20015110.0;
   Mill_Min_Easting    = -20015110.0;

   update();
}

void ossimMillerProjection::update()
{
   Set_Miller_Parameters(theEllipsoid.getA(),
                         theEllipsoid.getFlattening(),
                         theOrigin.lonr(),
                         Mill_False_Easting,
                         Mill_False_Northing);

   theFalseEastingNorthing.x = Mill_False_Easting;
   theFalseEastingNorthing.y = Mill_False_Northing;

   ossimMapProjection::update();
}

void ossimMillerProjection::setFalseEasting(double falseEasting)
{
   Mill_False_Easting = falseEasting;
   update();
}

void ossimMillerProjection::setFalseNorthing(double falseNorthing)
{
   Mill_False_Northing = falseNorthing;
   update();
}

void ossimMillerProjection::setDefaults()
{
   Mill_False_Easting  = 0.0;
   Mill_False_Northing = 0.0;
   Mill_Delta_Northing = 14675058.0;
   Mill_Max_Easting    = 20015110.0;
   Mill_Min_Easting    = -20015110.0;
}

void ossimMillerProjection::setCentralMeridian(double centralMeridian)
{
  Mill_Origin_Long = centralMeridian;
  update();
}

void ossimMillerProjection::setFalseEastingNorthing(double falseEasting,
                                                    double falseNorthing)
{
   Mill_False_Easting  = falseEasting;
   Mill_False_Northing = falseNorthing;
   
   update();
}

ossimGpt ossimMillerProjection::inverse(const ossimDpt &eastingNorthing)const
{
   double lat = 0.0;
   double lon = 0.0;

   Convert_Miller_To_Geodetic(eastingNorthing.x,
                              eastingNorthing.y,
                              &lat,
                              &lon);
   
   return ossimGpt(lat*DEG_PER_RAD, lon*DEG_PER_RAD, 0.0, theDatum);
}

ossimDpt ossimMillerProjection::forward(const ossimGpt &latLon)const
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

   Convert_Geodetic_To_Miller(gpt.latr(),
                              gpt.lonr(),
                              &easting,
                              &northing);
   
   return ossimDpt(easting, northing);
}

bool ossimMillerProjection::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   return ossimMapProjection::saveState(kwl, prefix);
}

bool ossimMillerProjection::loadState(const ossimKeywordlist& kwl,
                                      const char* prefix)
{
   bool flag = ossimMapProjection::loadState(kwl, prefix);

   const char* type          = kwl.find(prefix, ossimKeywordNames::TYPE_KW);

   setDefaults();

   if(ossimString(type) == STATIC_TYPE_NAME(ossimMillerProjection))
   {
      Mill_False_Easting  = theFalseEastingNorthing.x;
      Mill_False_Northing = theFalseEastingNorthing.y;
   }

   update();

   return flag;
}

/***************************************************************************/
/*
 *                              FUNCTIONS
 */

long ossimMillerProjection::Set_Miller_Parameters(double a,
                                                  double f,
                                                  double Central_Meridian ,
                                                  double False_Easting,
                                                  double False_Northing)
{ /* Begin Set_Miller_Parameters */
/*
 * The function Set_Miller_Parameters receives the ellipsoid parameters and
 * Miller Cylindrical projcetion parameters as inputs, and sets the corresponding
 * state variables.  If any errors occur, the error code(s) are returned by the 
 * function, otherwise MILL_NO_ERROR is returned.
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

//  double inv_f = 1 / f;
  long Error_Code = MILL_NO_ERROR;

//   if (a <= 0.0)
//   { /* Semi-major axis must be greater than zero */
//     Error_Code |= MILL_A_ERROR;
//   }
//   if ((inv_f < 250) || (inv_f > 350))
//   { /* Inverse flattening must be between 250 and 350 */
//     Error_Code |= MILL_INV_F_ERROR;
//   }
//   if ((Central_Meridian < -PI) || (Central_Meridian > TWO_PI))
//   { /* origin longitude out of range */
//     Error_Code |= MILL_CENT_MER_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */
    Mill_a = a;
    Mill_f = f;
    es2 = 2 * Mill_f - Mill_f * Mill_f;
    es4 = es2 * es2;
    es6 = es4 * es2;
    /* spherical radius */
    Ra = Mill_a * (1.0 - es2 / 6.0 - 17.0 * es4 / 360.0 - 67.0 * es6 /3024.0);
//     if (Central_Meridian > PI)
//       Central_Meridian -= TWO_PI;
    Mill_Origin_Long = Central_Meridian;
    Mill_False_Easting = False_Easting;
    Mill_False_Northing = False_Northing;
    if (Mill_Origin_Long > 0)
    {
      Mill_Max_Easting = 19903915.0;
      Mill_Min_Easting = -20015110.0;
    }
    else if (Mill_Origin_Long < 0)
    {
      Mill_Max_Easting = 20015110.0;
      Mill_Min_Easting = -19903915.0;
    }
    else
    {
      Mill_Max_Easting = 20015110.0;
      Mill_Min_Easting = -20015110.0;
    }
  } /* End if(!Error_Code) */
  return (Error_Code);
} /* End Set_Miller_Parameters */


void ossimMillerProjection::Get_Miller_Parameters(double *a,
                                                  double *f,
                                                  double *Central_Meridian,
                                                  double *False_Easting,
                                                  double *False_Northing)const
{ /* Begin Get_Miller_Parameters */
/*
 * The function Get_Miller_Parameters returns the current ellipsoid
 * parameters and Miller Cylindrical projection parameters.
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

  *a = Mill_a;
  *f = Mill_f;
  *Central_Meridian = Mill_Origin_Long;
  *False_Easting = Mill_False_Easting;
  *False_Northing = Mill_False_Northing;
  
  return;
} /* End Get_Miller_Parameters */


long ossimMillerProjection::Convert_Geodetic_To_Miller (double Latitude,
                                                        double Longitude,
                                                        double *Easting,
                                                        double *Northing)const

{ /* Begin Convert_Geodetic_To_Miller */
/*
 * The function Convert_Geodetic_To_Miller converts geodetic (latitude and
 * longitude) coordinates to Miller Cylindrical projection (easting and northing)
 * coordinates, according to the current ellipsoid and Miller Cylindrical projection
 * parameters.  If any errors occur, the error code(s) are returned by the
 * function, otherwise MILL_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi) in radians           (input)
 *    Longitude         : Longitude (lambda) in radians       (input)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */

  double slat = sin(0.8 * Latitude);
  double dlam;     /* Longitude - Central Meridan */

  long   Error_Code = MILL_NO_ERROR;

//   if ((Latitude < -PI_OVER_2) || (Latitude > PI_OVER_2))
//   {  /* Latitude out of range */
//     Error_Code |= MILL_LAT_ERROR;
//   }
//   if ((Longitude < -PI) || (Longitude > TWO_PI))
//   {  /* Longitude out of range */
//     Error_Code|= MILL_LON_ERROR;
//   }

  if (!Error_Code)
  { /* no errors */
    dlam = Longitude - Mill_Origin_Long;
//     if (dlam > PI)
//     {
//       dlam -= TWO_PI;
//     }
//     if (dlam < -PI)
//     {
//       dlam += TWO_PI;
//     }
    *Easting = Ra * dlam + Mill_False_Easting;
    *Northing = (Ra / 1.6) * log((1.0 + slat) /
                                 (1.0 - slat)) + Mill_False_Northing;
  }
  return (Error_Code);
} /* End Convert_Geodetic_To_Miller */


long ossimMillerProjection::Convert_Miller_To_Geodetic(double Easting,
                                                       double Northing,
                                                       double *Latitude,
                                                       double *Longitude)const
{ /* Begin Convert_Miller_To_Geodetic */
/*
 * The function Convert_Miller_To_Geodetic converts Miller Cylindrical projection
 * (easting and northing) coordinates to geodetic (latitude and longitude)
 * coordinates, according to the current ellipsoid and Miller Cylindrical projection
 * coordinates.  If any errors occur, the error code(s) are returned by the
 * function, otherwise MILL_NO_ERROR is returned.
 *
 *    Easting           : Easting (X) in meters                  (input)
 *    Northing          : Northing (Y) in meters                 (input)
 *    Latitude          : Latitude (phi) in radians              (output)
 *    Longitude         : Longitude (lambda) in radians          (output)
 */

  double dx, dy;
  long Error_Code = MILL_NO_ERROR;

//   if ((Easting < (Mill_False_Easting + Mill_Min_Easting))
//       || (Easting > (Mill_False_Easting + Mill_Max_Easting)))
//   { /* Easting out of range  */
//     Error_Code |= MILL_EASTING_ERROR;
//   }
//   if ((Northing < (Mill_False_Northing - Mill_Delta_Northing)) || 
//       (Northing > (Mill_False_Northing + Mill_Delta_Northing) ))
//   { /* Northing out of range */
//     Error_Code |= MILL_NORTHING_ERROR;
//   }

  if (!Error_Code)
  {
    dy = Northing - Mill_False_Northing;
    dx = Easting - Mill_False_Easting;
    *Latitude = atan(sinh(0.8 * dy / Ra)) / 0.8;
    *Longitude = Mill_Origin_Long + dx / Ra;

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
} /* End Convert_Miller_To_Geodetic */
