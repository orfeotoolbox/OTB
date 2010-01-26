//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Calls Geotrans OrthoGraphic projection code.  
//*******************************************************************
//  $Id: ossimOrthoGraphicProjection.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <math.h>
#include <ossim/projection/ossimOrthoGraphicProjection.h>
#include <ossim/base/ossimKeywordNames.h>

RTTI_DEF1(ossimOrthoGraphicProjection, "ossimOrthoGraphicProjection", ossimMapProjection)
/***************************************************************************/
/*
 *                              DEFINES
 */
#ifndef PI_OVER_2
#  define PI_OVER_2  ( M_PI / 2.0)
#endif
#ifndef TWO_PI
#  define TWO_PI     (2.0 * M_PI)
#endif
#define MAX_LAT    ( (M_PI * 90) / 180.0 )  /* 90 degrees in radians   */


#define ORTH_NO_ERROR           0x0000
#define ORTH_LAT_ERROR          0x0001
#define ORTH_LON_ERROR          0x0002
#define ORTH_EASTING_ERROR      0x0004
#define ORTH_NORTHING_ERROR     0x0008
#define ORTH_ORIGIN_LAT_ERROR   0x0010
#define ORTH_CENT_MER_ERROR     0x0020
#define ORTH_A_ERROR            0x0040
#define ORTH_B_ERROR            0x0080
#define ORTH_A_LESS_B_ERROR     0x0100
#define ORTH_RADIUS_ERROR	     0x0200


ossimOrthoGraphicProjection::ossimOrthoGraphicProjection(const ossimEllipsoid& ellipsoid,
                                                         const ossimGpt& origin)
   :ossimMapProjection(ellipsoid, origin)
{
   setDefaults();
   update();
}

ossimOrthoGraphicProjection::ossimOrthoGraphicProjection(const ossimEllipsoid& ellipsoid,
                                                 const ossimGpt& origin,
                                                 double falseEasting,
                                                 double falseNorthing)
   :ossimMapProjection(ellipsoid, origin)
{   
   Orth_False_Easting  = falseEasting;
   Orth_False_Northing = falseNorthing;

   update();
}

void ossimOrthoGraphicProjection::update()
{
   Set_Orthographic_Parameters(theEllipsoid.getA(),
                               theEllipsoid.getFlattening(),
                               theOrigin.latr(),
                               theOrigin.lonr(),
                               Orth_False_Easting,
                               Orth_False_Northing);

   theFalseEastingNorthing.x = Orth_False_Easting;
   theFalseEastingNorthing.y = Orth_False_Northing;

   ossimMapProjection::update();
}

void ossimOrthoGraphicProjection::setFalseEasting(double falseEasting)
{
   Orth_False_Easting = falseEasting;
   update();
}

void ossimOrthoGraphicProjection::setFalseNorthing(double falseNorthing)
{
   Orth_False_Northing = falseNorthing;
   update();
}

void ossimOrthoGraphicProjection::setDefaults()
{
   Orth_False_Easting  = 0.0;
   Orth_False_Northing = 0.0;
}

void ossimOrthoGraphicProjection::setFalseEastingNorthing(double falseEasting,
                                                      double falseNorthing)
{
   Orth_False_Easting  = falseEasting;
   Orth_False_Northing = falseNorthing;
   
   update();
}

void ossimOrthoGraphicProjection::getGroundClipPoints(ossimGeoPolygon& gpts)const
{
   
   gpts.addPoint(ossimGpt(theOrigin.latd(),
                          theOrigin.lond()-90.0,
                          0.0,
                          theDatum));
   gpts.addPoint(ossimGpt(theOrigin.latd()+90.0,
                          theOrigin.lond(),
                          0.0,
                          theDatum));
   gpts.addPoint(ossimGpt(theOrigin.latd(),
                          theOrigin.lond()+90.0,
                          0.0,
                          theDatum));
   gpts.addPoint(ossimGpt(theOrigin.latd()-90.0,
                          theOrigin.lond(),
                          0.0,
                          theDatum));

   
}

ossimGpt ossimOrthoGraphicProjection::inverse(const ossimDpt &eastingNorthing)const
{
   double lat = 0.0;
   double lon = 0.0;
   ossimGpt result;
   if(Convert_Orthographic_To_Geodetic(eastingNorthing.x,
                                       eastingNorthing.y,
                                       &lat,
                                       &lon)==ORTH_NO_ERROR)
   {
      result.datum(theDatum);
      result.latr(lat);
      result.lonr(lon);
   }
   else
   {
      result.makeNan();
   }

   return result;
}

ossimDpt ossimOrthoGraphicProjection::forward(const ossimGpt &latLon)const
{

   double easting  = 0.0;
   double northing = 0.0;
   ossimGpt gpt = latLon;
   ossimDpt result;
   
   if (theDatum)
   {
      if (theDatum->code() != latLon.datum()->code())
      {
         gpt.changeDatum(theDatum); // Shift to our datum.
      }
   }

   if(Convert_Geodetic_To_Orthographic(gpt.latr(),
                                       gpt.lonr(),
                                       &easting,
                                       &northing) == ORTH_NO_ERROR)
   {
      result = ossimDpt(easting, northing);
   }
   else
   {
      result.makeNan();
   }
   
   return result;
}

bool ossimOrthoGraphicProjection::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   return ossimMapProjection::saveState(kwl, prefix);
}

bool ossimOrthoGraphicProjection::loadState(const ossimKeywordlist& kwl,
                                            const char* prefix)
{
   bool flag = ossimMapProjection::loadState(kwl, prefix);
   const char* type          = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   
   setDefaults();

   if(ossimString(type) == STATIC_TYPE_NAME(ossimOrthoGraphicProjection))
   {
      Orth_False_Easting  = theFalseEastingNorthing.x;
      Orth_False_Northing = theFalseEastingNorthing.y;
   }

   update();

   return flag;
}

/***************************************************************************/
/*
 *                              FUNCTIONS
 */


long ossimOrthoGraphicProjection::Set_Orthographic_Parameters(double a,
                                                              double f,
                                                              double Origin_Latitude,
                                                              double Central_Meridian,
                                                              double False_Easting,
                                                              double False_Northing)
{ /* BEGIN Set_Orthographic_Parameters */
/*
 * The function Set_Orthographic_Parameters receives the ellipsoid parameters and
 * projection parameters as inputs, and sets the corresponding state
 * variables.  If any errors occur, the error code(s) are returned by the function, 
 * otherwise ORTH_NO_ERROR is returned.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters   (input)
 *    f                 : Flattening of ellipsoid						        (input)
 *    Origin_Latitude   : Latitude in radians at which the          (input)
 *                          point scale factor is 1.0
 *    Central_Meridian  : Longitude in radians at the center of     (input)
 *                          the projection
 *    False_Easting     : A coordinate value in meters assigned to the
 *                          central meridian of the projection.     (input)
 *    False_Northing    : A coordinate value in meters assigned to the
 *                          origin latitude of the projection       (input)                          
 */

//  double inv_f = 1 / f;
  long Error_Code = ORTH_NO_ERROR;

  if (a <= 0.0)
  { /* Semi-major axis must be greater than zero */
    Error_Code |= ORTH_A_ERROR;
  }
//   if ((inv_f < 250) || (inv_f > 350))
//   { /* Inverse flattening must be between 250 and 350 */
//     Error_Code |= ORTH_INV_F_ERROR;
//   }
  if ((Origin_Latitude < -PI_OVER_2) || (Origin_Latitude > PI_OVER_2))
  { /* origin latitude out of range */
    Error_Code |= ORTH_ORIGIN_LAT_ERROR;
  }
  if ((Central_Meridian < -M_PI) || (Central_Meridian > TWO_PI))
  { /* origin longitude out of range */
    Error_Code |= ORTH_CENT_MER_ERROR;
  }
  if (!Error_Code)
  { /* no errors */
    Orth_a = a;
    Orth_f = f;
    es2 = 2 * Orth_f - Orth_f * Orth_f;
    es4 = es2 * es2;
    es6 = es4 * es2;
    Ra = Orth_a * (1.0 - es2 / 6.0 - 17.0 * es4 / 360.0 - 67.0 * es6 /3024.0);
    Orth_Origin_Lat = Origin_Latitude;
    Sin_Orth_Origin_Lat = sin(Orth_Origin_Lat);
    Cos_Orth_Origin_Lat = cos(Orth_Origin_Lat);
//     if (Central_Meridian > M_PI)
//       Central_Meridian -= TWO_PI;
    Orth_Origin_Long = Central_Meridian;
    Orth_False_Easting = False_Easting;
    Orth_False_Northing = False_Northing;

  } /* END OF if(!Error_Code) */
  
  return (Error_Code);
} /* END OF Set_Orthographic_Parameters */


void ossimOrthoGraphicProjection::Get_Orthographic_Parameters(double *a,
                                                              double *f,
                                                              double *Origin_Latitude,
                                                              double *Central_Meridian,
                                                              double *False_Easting,
                                                              double *False_Northing)const
{ /* BEGIN Get_Orthographic_Parameters */
/*
 * The function Get_Orthographic_Parameters returns the current ellipsoid
 * parameters and Orthographic projection parameters.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters   (output)
 *    f                 : Flattening of ellipsoid						        (output)
 *    Origin_Latitude   : Latitude in radians at which the          (output)
 *                          point scale factor is 1.0
 *    Central_Meridian  : Longitude in radians at the center of     (output)
 *                          the projection
 *    False_Easting     : A coordinate value in meters assigned to the
 *                          central meridian of the projection.     (output)
 *    False_Northing    : A coordinate value in meters assigned to the
 *                          origin latitude of the projection       (output) 
 */

  *a = Orth_a;
  *f = Orth_f;
  *Origin_Latitude = Orth_Origin_Lat;
  *Central_Meridian = Orth_Origin_Long;
  *False_Easting = Orth_False_Easting;
  *False_Northing = Orth_False_Northing;
  
  return;
} /* END OF Get_Orthographic_Parameters */


long ossimOrthoGraphicProjection::Convert_Geodetic_To_Orthographic (double Latitude,
                                                                    double Longitude,
                                                                    double *Easting,
                                                                    double *Northing)const

{ /* BEGIN Convert_Geodetic_To_Orthographic */
/*
 * The function Convert_Geodetic_To_Orthographic converts geodetic (latitude and
 * longitude) coordinates to Orthographic projection (easting and northing)
 * coordinates, according to the current ellipsoid and Orthographic projection
 * parameters.  If any errors occur, the error code(s) are returned by the
 * function, otherwise ORTH_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi) in radians           (input)
 *    Longitude         : Longitude (lambda) in radians       (input)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */

  double slat = sin(Latitude);
  double clat = cos(Latitude);
  double dlam;                        /* Longitude - Central Meridan */
  double clat_cdlam;
  double cos_c;                       /* Value used to determine whether the point is beyond
                                   viewing.  If zero or positive, the point is within view.  */
  long   Error_Code = ORTH_NO_ERROR;

  if ((Latitude < -PI_OVER_2) || (Latitude > PI_OVER_2))
  {  /* Latitude out of range */
     Error_Code |= ORTH_LAT_ERROR;
  }
  if ((Longitude < -M_PI) || (Longitude > TWO_PI))
  {  /* Longitude out of range */
     Error_Code|= ORTH_LON_ERROR;
  }
  dlam = Longitude - Orth_Origin_Long;
  clat_cdlam = clat * cos(dlam);
  cos_c = Sin_Orth_Origin_Lat * slat + Cos_Orth_Origin_Lat * clat_cdlam;
  if (cos_c < 0.0)
  {  /* Point is out of view.  Will return longitude out of range message
        since no point out of view is implemented.  */
     Error_Code |= ORTH_LON_ERROR;
  }

  if (!Error_Code)
  { /* no errors */
     if (dlam > M_PI)
     {
        dlam -= TWO_PI;
     }
     if (dlam < -M_PI)
     {
        dlam += TWO_PI;
     }
     *Easting = Ra * clat * sin(dlam) + Orth_False_Easting;
     *Northing = Ra * (Cos_Orth_Origin_Lat * slat - Sin_Orth_Origin_Lat * clat_cdlam) +
        Orth_False_Northing;
  }
  return (Error_Code);

} /* END OF Convert_Geodetic_To_Orthographic */


long ossimOrthoGraphicProjection::Convert_Orthographic_To_Geodetic(double Easting,
                                                                   double Northing,
                                                                   double *Latitude,
                                                                   double *Longitude)const
{ /* BEGIN Convert_Orthographic_To_Geodetic */
/*
 * The function Convert_Orthographic_To_Geodetic converts Orthographic projection
 * (easting and northing) coordinates to geodetic (latitude and longitude)
 * coordinates, according to the current ellipsoid and Orthographic projection
 * coordinates.  If any errors occur, the error code(s) are returned by the
 * function, otherwise ORTH_NO_ERROR is returned.
 *
 *    Easting           : Easting (X) in meters                  (input)
 *    Northing          : Northing (Y) in meters                 (input)
 *    Latitude          : Latitude (phi) in radians              (output)
 *    Longitude         : Longitude (lambda) in radians          (output)
 */

  double cc;
  double cos_cc, sin_cc;
  double rho;
  double dx, dy;
  double atan_dx_over_dy;
//  double temp;
  double rho_OVER_Ra;
  long Error_Code = ORTH_NO_ERROR;

  if ((Easting > (Orth_False_Easting + Ra)) ||
      (Easting < (Orth_False_Easting - Ra)))
  { /* Easting out of range */
    Error_Code |= ORTH_EASTING_ERROR;
  }
  if ((Northing > (Orth_False_Northing + Ra)) ||
      (Northing < (Orth_False_Northing - Ra)))
  { /* Northing out of range */
    Error_Code |= ORTH_NORTHING_ERROR;
  }
  if (!Error_Code)
  {
    double temp = sqrt(Easting * Easting + Northing * Northing);     

    if ((temp > (Orth_False_Easting + Ra)) || (temp > (Orth_False_Northing + Ra)) ||
        (temp < (Orth_False_Easting - Ra)) || (temp < (Orth_False_Northing - Ra)))
    { /* Point is outside of projection area */
      Error_Code |= ORTH_RADIUS_ERROR;
    }
  }

  if (!Error_Code)
  {
    dx = Easting - Orth_False_Easting;
    dy = Northing - Orth_False_Northing;
    atan_dx_over_dy = atan(dx / dy);
    rho = sqrt(dx * dx + dy * dy);
    if (rho == 0.0)
    {
      *Latitude = Orth_Origin_Lat;
      *Longitude = Orth_Origin_Long;
    }
    else
    {
      rho_OVER_Ra = rho / Ra;

      if (rho_OVER_Ra > 1.0)
        rho_OVER_Ra = 1.0;
      else if (rho_OVER_Ra < -1.0)
        rho_OVER_Ra = -1.0;

      cc = asin(rho_OVER_Ra);
      cos_cc = cos(cc);
      sin_cc = sin(cc);
      *Latitude = asin(cos_cc * Sin_Orth_Origin_Lat + (dy * sin_cc * Cos_Orth_Origin_Lat / rho));

      if (Orth_Origin_Lat == MAX_LAT)
        *Longitude = Orth_Origin_Long - atan_dx_over_dy;
      else if (Orth_Origin_Lat == -MAX_LAT)
        *Longitude = Orth_Origin_Long + atan_dx_over_dy;
      else
        *Longitude = Orth_Origin_Long + atan(dx * sin_cc / (rho *
                                                            Cos_Orth_Origin_Lat * cos_cc - dy * Sin_Orth_Origin_Lat * sin_cc));

//       if (*Latitude > PI_OVER_2)  /* force distorted values to 90, -90 degrees */
//         *Latitude = PI_OVER_2;
//       else if (*Latitude < -PI_OVER_2)
//         *Latitude = -PI_OVER_2;

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
} /* END OF Convert_Orthographic_To_Geodetic */
