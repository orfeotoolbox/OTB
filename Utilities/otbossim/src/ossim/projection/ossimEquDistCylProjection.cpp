//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Calls Geotrans Equidistant Cylinder projection code.  
//*******************************************************************
//  $Id: ossimEquDistCylProjection.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimNotifyContext.h>

static ossimTrace traceDebug("ossimEquDistCylProjection:debug");

RTTI_DEF1(ossimEquDistCylProjection,
          "ossimEquDistCylProjection",
	  ossimMapProjection);
	  //          ossimLlxyProjection)
   
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
#define ONE	   (1.0 * M_PI / 180.0)       /* 1 degree in radians*/

#define EQCY_NO_ERROR           0x0000
#define EQCY_LAT_ERROR          0x0001
#define EQCY_LON_ERROR          0x0002
#define EQCY_EASTING_ERROR      0x0004
#define EQCY_NORTHING_ERROR     0x0008
#define EQCY_ORIGIN_LAT_ERROR   0x0010
#define EQCY_CENT_MER_ERROR     0x0020
#define EQCY_A_ERROR            0x0040
#define EQCY_B_ERROR            0x0080
#define EQCY_A_LESS_B_ERROR     0x0100


ossimEquDistCylProjection::ossimEquDistCylProjection(const ossimEllipsoid& ellipsoid,
                                                     const ossimGpt& origin)
//  :ossimLlxyProjection(ellipsoid, origin)
  :ossimMapProjection(ellipsoid, origin)
{
   setDefaults();
   update();
}

ossimEquDistCylProjection::ossimEquDistCylProjection(const ossimEllipsoid& ellipsoid,
                                                     const ossimGpt& origin,
                                                     double falseEasting,
                                                     double falseNorthing)
//  :ossimLlxyProjection(ellipsoid, origin)
  :ossimMapProjection(ellipsoid, origin)
{
   Eqcy_False_Easting  = falseEasting;
   Eqcy_False_Northing = falseNorthing;

   Eqcy_Delta_Northing = 10007555.0;
   Eqcy_Max_Easting    = 20015110.0;
   Eqcy_Min_Easting    = -20015110.0;
   
   update();
}

void ossimEquDistCylProjection::update()
{
   Set_Equidistant_Cyl_Parameters(theEllipsoid.getA(),
                                  theEllipsoid.getFlattening(),
                                  theOrigin.latr(),
                                  theOrigin.lonr(),
                                  Eqcy_False_Easting,
                                  Eqcy_False_Northing);

   theFalseEastingNorthing.x = Eqcy_False_Easting;
   theFalseEastingNorthing.y = Eqcy_False_Northing;

   ossimMapProjection::update();
}

void ossimEquDistCylProjection::setFalseEasting(double falseEasting)
{
   Eqcy_False_Easting = falseEasting;
   update();
}

void ossimEquDistCylProjection::setFalseNorthing(double falseNorthing)
{
   Eqcy_False_Northing = falseNorthing;
   update();
}

void ossimEquDistCylProjection::setFalseEastingNorthing(double falseEasting,
                                                        double falseNorthing)
{
   Eqcy_False_Easting  = falseEasting;
   Eqcy_False_Northing = falseNorthing;
   
   update();
}

void ossimEquDistCylProjection::setDefaults()
{
   Eqcy_False_Easting  = 0.0;
   Eqcy_False_Northing = 0.0;
   Eqcy_Delta_Northing = 10007555.0;
   Eqcy_Max_Easting    = 20015110.0;
   Eqcy_Min_Easting    = -20015110.0;
}

ossimGpt ossimEquDistCylProjection::inverse(const ossimDpt &eastingNorthing)const
{
   double lat = 0.0;
   double lon = 0.0;

   Convert_Equidistant_Cyl_To_Geodetic(eastingNorthing.x,
                                       eastingNorthing.y,
                                       &lat,
                                       &lon);

   return ossimGpt(lat*DEG_PER_RAD, lon*DEG_PER_RAD, 0.0, theDatum);
}

ossimDpt ossimEquDistCylProjection::forward(const ossimGpt &latLon)const
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

   Convert_Geodetic_To_Equidistant_Cyl(gpt.latr(),
                                       gpt.lonr(),
                                       &easting,
                                       &northing);
   
   return ossimDpt(easting, northing);
}



bool ossimEquDistCylProjection::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   return ossimMapProjection::saveState(kwl, prefix);
}

bool ossimEquDistCylProjection::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimEquDistCylProjection::loadState: Input keyword list is \n" << kwl << endl;
   }

   //   ossimLlxyProjection::loadState(kwl, prefix);
   ossimMapProjection::loadState(kwl, prefix);

   const char* type = kwl.find(prefix, ossimKeywordNames::TYPE_KW);

   setDefaults();
   // make sure we are of the same type.  If we are then the easting
   // northing values will make since
   //
   if(ossimString(type) == STATIC_TYPE_NAME(ossimEquDistCylProjection))
   {
      Eqcy_False_Easting  = theFalseEastingNorthing.x;
      Eqcy_False_Northing = theFalseEastingNorthing.y;
   }
   else
   {
      theUlEastingNorthing.makeNan();
   }
   // finalize the initialization.
   update();
            
   return true;
}

/***************************************************************************/
/*
 *                              FUNCTIONS
 */


long ossimEquDistCylProjection::Set_Equidistant_Cyl_Parameters(double a,
                                                               double f,
                                                               double Std_Parallel,
                                                               double Central_Meridian,
                                                               double False_Easting,
                                                               double False_Northing)
{ /* Begin Set_Equidistant_Cyl_Parameters */
/*
 * The function Set_Equidistant_Cyl_Parameters receives the ellipsoid parameters and
 * projection parameters as inputs, and sets the corresponding state
 * variables.  It also calculates the spherical radius of the sphere having 
 * the same area as the ellipsoid.  If any errors occur, the error code(s) 
 * are returned by the function, otherwise EQCY_NO_ERROR is returned.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters   (input)
 *    f                 : Flattening of ellipsoid						        (input)
 *    Std_Parallel      : Latitude in radians at which the          (input)
 *                          point scale factor is 1.0
 *    Central_Meridian  : Longitude in radians at the center of     (input)
 *                          the projection
 *    False_Easting     : A coordinate value in meters assigned to the
 *                          central meridian of the projection.     (input)
 *    False_Northing    : A coordinate value in meters assigned to the
 *                          standard parallel of the projection     (input)
 */

  double temp;
//  double inv_f = 1 / f;
  long Error_Code = EQCY_NO_ERROR;

//   if (a <= 0.0)
//   { /* Semi-major axis must be greater than zero */
//     Error_Code |= EQCY_A_ERROR;
//   }
//   if ((inv_f < 250) || (inv_f > 350))
//   { /* Inverse flattening must be between 250 and 350 */
//     Error_Code |= EQCY_INV_F_ERROR;
//   }
//   if ((Std_Parallel < -PI_OVER_2) || (Std_Parallel > PI_OVER_2))
//   { /* standard parallel out of range */
//     Error_Code |= EQCY_STDP_ERROR;
//   }
//   if ((Central_Meridian < -PI) || (Central_Meridian > TWO_PI))
//   { /* origin longitude out of range */
//     Error_Code |= EQCY_CENT_MER_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */
    Eqcy_a = a;
    Eqcy_f = f;
    es2 = 2 * Eqcy_f - Eqcy_f * Eqcy_f;
    es4 = es2 * es2;
    es6 = es4 * es2;
    /* spherical radius */
    Ra = Eqcy_a * (1.0 - es2 / 6.0 - 17.0 * es4 / 360.0 - 67.0 * es6 /3024.0);
    Eqcy_Std_Parallel = Std_Parallel;
    Cos_Eqcy_Std_Parallel = cos(Eqcy_Std_Parallel);
    Ra_Cos_Eqcy_Std_Parallel = Ra * Cos_Eqcy_Std_Parallel;
//    if (Central_Meridian > M_PI)
//      Central_Meridian -= TWO_PI;
    Eqcy_Origin_Long = Central_Meridian;
    Eqcy_False_Easting = False_Easting;
    Eqcy_False_Northing = False_Northing;
    if (Eqcy_Origin_Long > 0)
    {
      Convert_Geodetic_To_Equidistant_Cyl(PI_OVER_2, Eqcy_Origin_Long - M_PI - ONE, &Eqcy_Max_Easting, &temp);
      Convert_Geodetic_To_Equidistant_Cyl(PI_OVER_2, Eqcy_Origin_Long - M_PI, &Eqcy_Min_Easting, &temp);
    }
    else if (Eqcy_Origin_Long < 0)
    {
      Convert_Geodetic_To_Equidistant_Cyl(PI_OVER_2, Eqcy_Origin_Long + M_PI, &Eqcy_Max_Easting, &temp);
      Convert_Geodetic_To_Equidistant_Cyl(PI_OVER_2, Eqcy_Origin_Long + M_PI + ONE, &Eqcy_Min_Easting, &temp);
    }
    else
    {
      Convert_Geodetic_To_Equidistant_Cyl(PI_OVER_2, M_PI, &Eqcy_Max_Easting, &temp);
      Eqcy_Min_Easting = -Eqcy_Max_Easting;
    }
  } /* End if(!Error_Code) */
  return (Error_Code);
} /* End Set_Equidistant_Cyl_Parameters */


void ossimEquDistCylProjection::Get_Equidistant_Cyl_Parameters(double *a,
                                                               double *f,
                                                               double *Std_Parallel,
                                                               double *Central_Meridian,
                                                               double *False_Easting,
                                                               double *False_Northing)const
{ /* Begin Get_Equidistant_Cyl_Parameters */
/*
 * The function Get_Equidistant_Cyl_Parameters returns the current ellipsoid
 * parameters and Equidistant Cylindrical projection parameters.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters   (output)
 *    f                 : Flattening of ellipsoid						        (output)
 *    Std_Parallel      : Latitude in radians at which the          (output)
 *                          point scale factor is 1.0
 *    Central_Meridian  : Longitude in radians at the center of     (output)
 *                          the projection
 *    False_Easting     : A coordinate value in meters assigned to the
 *                          central meridian of the projection.     (output)
 *    False_Northing    : A coordinate value in meters assigned to the
 *                          standard parallel of the projection     (output)
 */

  *a = Eqcy_a;
  *f = Eqcy_f;
  *Std_Parallel = Eqcy_Std_Parallel;
  *Central_Meridian = Eqcy_Origin_Long;
  *False_Easting = Eqcy_False_Easting;
  *False_Northing = Eqcy_False_Northing;
  return;
} /* End Get_Equidistant_Cyl_Parameters */


long ossimEquDistCylProjection::Convert_Geodetic_To_Equidistant_Cyl (double Latitude,
                                                                     double Longitude,
                                                                     double *Easting,
                                                                     double *Northing)const
   
{ /* Begin Convert_Geodetic_To_Equidistant_Cyl */
/*
 * The function Convert_Geodetic_To_Equidistant_Cyl converts geodetic (latitude and
 * longitude) coordinates to Equidistant Cylindrical projection (easting and northing)
 * coordinates, according to the current ellipsoid, spherical radiius
 * and Equidistant Cylindrical projection parameters.
 * If any errors occur, the error code(s) are returned by the
 * function, otherwise EQCY_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi) in radians           (input)
 *    Longitude         : Longitude (lambda) in radians       (input)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */

  double dlam;     /* Longitude - Central Meridan */
  long Error_Code = EQCY_NO_ERROR;

//   if ((Latitude < -PI_OVER_2) || (Latitude > PI_OVER_2))
//   {  /* Latitude out of range */
//     Error_Code |= EQCY_LAT_ERROR;
//   }
//   if ((Longitude < -M_PI) || (Longitude > TWO_PI))
//   {  /* Longitude out of range */
//     Error_Code|= EQCY_LON_ERROR;
//   }

  if (!Error_Code)
  { /* no errors */
    dlam = Longitude - Eqcy_Origin_Long;
    if (dlam > M_PI)
    {
      dlam -= TWO_PI;
    }
    if (dlam < -M_PI)
    {
      dlam += TWO_PI;
    }

    *Easting = Ra_Cos_Eqcy_Std_Parallel * dlam + Eqcy_False_Easting;
    *Northing = Ra * Latitude + Eqcy_False_Northing;

  }
  return (Error_Code);

} /* End Convert_Geodetic_To_Equidistant_Cyl */


long ossimEquDistCylProjection::Convert_Equidistant_Cyl_To_Geodetic(double Easting,
                                                                    double Northing,
                                                                    double *Latitude,
                                                                    double *Longitude)const
{ /* Begin Convert_Equidistant_Cyl_To_Geodetic */
/*
 * The function Convert_Equidistant_Cyl_To_Geodetic converts Equidistant Cylindrical projection
 * (easting and northing) coordinates to geodetic (latitude and longitude)
 * coordinates, according to the current ellipsoid, spherical radius
 * and Equidistant Cylindrical projection coordinates.
 * If any errors occur, the error code(s) are returned by the
 * function, otherwise EQCY_NO_ERROR is returned.
 *
 *    Easting           : Easting (X) in meters                  (input)
 *    Northing          : Northing (Y) in meters                 (input)
 *    Latitude          : Latitude (phi) in radians              (output)
 *    Longitude         : Longitude (lambda) in radians          (output)
 */

  double dx, dy;
  long Error_Code = EQCY_NO_ERROR;

//   if ((Easting < (Eqcy_False_Easting + Eqcy_Min_Easting))
//       || (Easting > (Eqcy_False_Easting + Eqcy_Max_Easting)))
//   { /* Easting out of range */
//     Error_Code |= EQCY_EASTING_ERROR;
//   }
//   if ((Northing < (Eqcy_False_Northing - Eqcy_Delta_Northing))
//       || (Northing > (Eqcy_False_Northing + Eqcy_Delta_Northing)))
//   { /* Northing out of range */
//     Error_Code |= EQCY_NORTHING_ERROR;
//   }

  if (!Error_Code)
  {
    dy = Northing - Eqcy_False_Northing;
    dx = Easting - Eqcy_False_Easting;
    *Latitude = dy / Ra;

    if (Ra_Cos_Eqcy_Std_Parallel == 0)
      *Longitude = 0;
    else
      *Longitude = Eqcy_Origin_Long + dx / Ra_Cos_Eqcy_Std_Parallel;

//     if (*Latitude > PI_OVER_2)  /* force distorted values to 90, -90 degrees */
//       *Latitude = PI_OVER_2;
//     else if (*Latitude < -PI_OVER_2)
//       *Latitude = -PI_OVER_2;

//    if (*Longitude > PI)
//      *Longitude -= TWO_PI;
//    if (*Longitude < -PI)
//      *Longitude += TWO_PI;

//    if (*Longitude > PI)  /* force distorted values to 180, -180 degrees */
//      *Longitude = PI;
//    else if (*Longitude < -PI)
//      *Longitude = -PI;

  }
  return (Error_Code);

} /* End Convert_Equidistant_Cyl_To_Geodetic */
