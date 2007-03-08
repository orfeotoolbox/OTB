//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Calls Geotrans Cylinder Equal Area  projection code.  
//*******************************************************************
//  $Id: ossimCylEquAreaProjection.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <ossim/projection/ossimCylEquAreaProjection.h>
#include <ossim/base/ossimKeywordNames.h>

RTTI_DEF1(ossimCylEquAreaProjection, "ossimCylEquAreaProjection", ossimMapProjection)
   
/***************************************************************************/
/*
 *                              DEFINES
 */

#define CYEQ_NO_ERROR           0x0000
#define CYEQ_LAT_ERROR          0x0001
#define CYEQ_LON_ERROR          0x0002
#define CYEQ_EASTING_ERROR      0x0004
#define CYEQ_NORTHING_ERROR     0x0008
#define CYEQ_ORIGIN_LAT_ERROR   0x0010
#define CYEQ_CENT_MER_ERROR     0x0020
#define CYEQ_A_ERROR            0x0040
#define CYEQ_B_ERROR            0x0080
#define CYEQ_A_LESS_B_ERROR     0x0100

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
#define CYLEQAR_Q(slat, x)                (1.0-es2)*(slat/(1.0-x*x)-(1.0/(2.0*es))*      \
									          log((1.0-x)/(1.0+x))) 
#define CYEQ_COEFF_TIMES_SIN(coeff, c, Beta)      (coeff * sin(c * Beta))
#define ONE        (1.0 * M_PI / 180.0)  /* 1 degree in radians           */


ossimCylEquAreaProjection::ossimCylEquAreaProjection(const ossimEllipsoid& ellipsoid,
                                               const ossimGpt& origin)
  :ossimMapProjection(ellipsoid, origin)
{
   setDefaults();
   update();
}

ossimCylEquAreaProjection::ossimCylEquAreaProjection(const ossimEllipsoid& ellipsoid,
                                                     const ossimGpt& origin,
                                                     double falseEasting,
                                                     double falseNorthing)
  :ossimMapProjection(ellipsoid, origin)
{
   Cyeq_False_Easting  = falseEasting;
   Cyeq_False_Northing = falseNorthing;
   Cyeq_Max_Easting    = 20037509.0;
   Cyeq_Min_Easting    = -20037509.0;
   Cyeq_Delta_Northing = 6363886.0;

   update();
}

void ossimCylEquAreaProjection::setFalseEasting(double falseEasting)
{
   Cyeq_False_Easting = falseEasting;
   update();
}

void ossimCylEquAreaProjection::setFalseNorthing(double falseNorthing)
{
   Cyeq_False_Northing = falseNorthing;
   update();
}

void ossimCylEquAreaProjection::setFalseEastingNorthing(double falseEasting,
                                                        double falseNorthing)
{
   Cyeq_False_Easting = falseEasting;
   Cyeq_False_Northing = falseNorthing;
   update();
}

void ossimCylEquAreaProjection::setDefaults()
{
   Cyeq_False_Easting  = 0.0;
   Cyeq_False_Northing = 0.0;
   Cyeq_Max_Easting    = 20037509.0;
   Cyeq_Min_Easting    = -20037509.0;
   Cyeq_Delta_Northing = 6363886.0;
}

void ossimCylEquAreaProjection::update()
{   
   Cyeq_Max_Easting    = 20037509.0;
   Cyeq_Min_Easting    = -20037509.0;
   Cyeq_Delta_Northing = 6363886.0;

   Set_Cyl_Eq_Area_Parameters(theEllipsoid.getA(),
                              theEllipsoid.getFlattening(),
                              theOrigin.latr(),
                              theOrigin.lonr(),
                              Cyeq_False_Easting,
                              Cyeq_False_Northing);

   theFalseEastingNorthing.x = Cyeq_False_Easting;
   theFalseEastingNorthing.y = Cyeq_False_Northing;

   ossimMapProjection::update();
}


ossimGpt ossimCylEquAreaProjection::inverse(const ossimDpt &eastingNorthing)const
{
   double lat = 0.0;
   double lon = 0.0;
   
   Convert_Cyl_Eq_Area_To_Geodetic(eastingNorthing.x,
                                   eastingNorthing.y,
                                   &lat,
                                   &lon);
   return ossimGpt(lat*DEG_PER_RAD, lon*DEG_PER_RAD, 0, theDatum);  
}

ossimDpt ossimCylEquAreaProjection::forward(const ossimGpt &latLon)const
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

   Convert_Geodetic_To_Cyl_Eq_Area(gpt.latr(),
                                   gpt.lonr(),
                                   &easting,
                                   &northing);

   return ossimDpt(easting, northing);

}

bool ossimCylEquAreaProjection::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   return ossimMapProjection::saveState(kwl, prefix);
}

bool ossimCylEquAreaProjection::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   bool flag = ossimMapProjection::loadState(kwl, prefix);
   
   const char* type          = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   
   setDefaults();
   
   if(ossimString(type) == STATIC_TYPE_NAME(ossimCylEquAreaProjection))
   {
      Cyeq_False_Easting  = theFalseEastingNorthing.x;
      Cyeq_False_Northing = theFalseEastingNorthing.y;
   }
   
   update();
   
   return flag;
}

/***************************************************************************/
/*
 *                              FUNCTIONS     
 */


long ossimCylEquAreaProjection::Set_Cyl_Eq_Area_Parameters(double a,
                                                           double f,
                                                           double Origin_Latitude,
                                                           double Central_Meridian,
                                                           double False_Easting,
                                                           double False_Northing)
{ /* Begin Set_Cyl_Eq_Area_Parameters */
/*
 * The function Set_Cyl_Eq_Area_Parameters receives the ellipsoid parameters and
 * Cylindrical Equal Area projcetion parameters as inputs, and sets the corresponding 
 * state variables.  If any errors occur, the error code(s) are returned by the 
 * function, otherwise CYEQ_NO_ERROR is returned.
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

  double Sin_Cyeq_Origin_Lat;
  double temp;
//  double inv_f = 1 / f;
  long Error_Code = CYEQ_NO_ERROR;

//   if (a <= 0.0)
//   { /* Semi-major axis must be greater than zero */
//     Error_Code |= CYEQ_A_ERROR;
//   }
//   if ((inv_f < 250) || (inv_f > 350))
//   { /* Inverse flattening must be between 250 and 350 */
//     Error_Code |= CYEQ_INV_F_ERROR;
//   }
//   if ((Origin_Latitude < -PI_OVER_2) || (Origin_Latitude > PI_OVER_2))
//   { /* origin latitude out of range */
//     Error_Code |= CYEQ_ORIGIN_LAT_ERROR;
//   }
//   if ((Central_Meridian < -PI) || (Central_Meridian > TWO_PI))
//   { /* origin longitude out of range */
//     Error_Code |= CYEQ_CENT_MER_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */
    Cyeq_a = a;
    Cyeq_f = f;
    Cyeq_Origin_Lat = Origin_Latitude;
    if (Central_Meridian > M_PI)
      Central_Meridian -= TWO_PI;
    Cyeq_Origin_Long = Central_Meridian;
    Cyeq_False_Northing = False_Northing;
    Cyeq_False_Easting = False_Easting;
    es2 = 2 * Cyeq_f - Cyeq_f * Cyeq_f;
    es4 = es2 * es2;
    es6 = es4 * es2;
    es = sqrt(es2);
    c0 = es2 / 3.0 + 31.0 * es4 / 180.0 + 517.0 * es6 / 5040.0; 
    c1 = 23.0 * es4 / 360.0 + 251.0 * es6 / 3780.0;
    c2 = 761.0 * es6 / 45360.0; 
    Sin_Cyeq_Origin_Lat = sin(Cyeq_Origin_Lat);
    k0 = cos(Cyeq_Origin_Lat) / sqrt(1.0 - es2 * Sin_Cyeq_Origin_Lat * Sin_Cyeq_Origin_Lat);
    Cyeq_a_k0 = Cyeq_a * k0;
    two_k0 = 2.0 * k0;

    if (Cyeq_Origin_Long > 0)
    {
      Convert_Geodetic_To_Cyl_Eq_Area(PI_OVER_2, Cyeq_Origin_Long - M_PI - ONE, &Cyeq_Max_Easting, &temp);
      Convert_Geodetic_To_Cyl_Eq_Area(PI_OVER_2, Cyeq_Origin_Long - M_PI, &Cyeq_Min_Easting, &temp);
      Convert_Geodetic_To_Cyl_Eq_Area(PI_OVER_2, M_PI, &temp, &Cyeq_Delta_Northing);
    }
    else if (Cyeq_Origin_Long < 0)
    {
      Convert_Geodetic_To_Cyl_Eq_Area(PI_OVER_2, Cyeq_Origin_Long + M_PI, &Cyeq_Max_Easting, &temp);
      Convert_Geodetic_To_Cyl_Eq_Area(PI_OVER_2, Cyeq_Origin_Long + M_PI + ONE, &Cyeq_Min_Easting, &temp);
      Convert_Geodetic_To_Cyl_Eq_Area(PI_OVER_2, M_PI, &temp, &Cyeq_Delta_Northing);  
    }
    else
    {
      Convert_Geodetic_To_Cyl_Eq_Area(PI_OVER_2, M_PI, &Cyeq_Max_Easting, &Cyeq_Delta_Northing);  
      Cyeq_Min_Easting = -Cyeq_Max_Easting;
    }

  } /* End if(!Error_Code) */
  return (Error_Code);
} /* End Set_Cyl_Eq_Area_Parameters */


void ossimCylEquAreaProjection::Get_Cyl_Eq_Area_Parameters(double *a,
                                                           double *f,
                                                           double *Origin_Latitude,
                                                           double *Central_Meridian,
                                                           double *False_Easting,
                                                           double *False_Northing)const
{ /* Begin Get_Cyl_Eq_Area_Parameters */
/*
 * The function Get_Cyl_Eq_Area_Parameters returns the current ellipsoid
 * parameters, and Cylindrical Equal Area projection parameters.
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

  *a = Cyeq_a;
  *f = Cyeq_f;
  *Origin_Latitude = Cyeq_Origin_Lat;
  *Central_Meridian = Cyeq_Origin_Long;
  *False_Easting = Cyeq_False_Easting;
  *False_Northing = Cyeq_False_Northing;
  return;
} /* End Get_Cyl_Eq_Area_Parameters */


long ossimCylEquAreaProjection::Convert_Geodetic_To_Cyl_Eq_Area (double Latitude,
                                                                 double Longitude,
                                                                 double *Easting,
                                                                 double *Northing)const
{ /* Begin Convert_Geodetic_To_Cyl_Eq_Area */
/*
 * The function Convert_Geodetic_To_Cyl_Eq_Area converts geodetic (latitude and
 * longitude) coordinates to Cylindrical Equal Area projection (easting and northing)
 * coordinates, according to the current ellipsoid and Cylindrical Equal Area projection
 * parameters.  If any errors occur, the error code(s) are returned by the
 * function, otherwise CYEQ_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi) in radians           (input)
 *    Longitude         : Longitude (lambda) in radians       (input)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */

  double dlam;                      /* Longitude - Central Meridan */
  double qq;
  double x;
  double sin_lat = sin(Latitude);
  long Error_Code = CYEQ_NO_ERROR;

//   if ((Latitude < -PI_OVER_2) || (Latitude > PI_OVER_2))
//   { /* Latitude out of range */
//     Error_Code |= CYEQ_LAT_ERROR;
//   }
//   if ((Longitude < -PI) || (Longitude > TWO_PI))
//   { /* Longitude out of range */
//     Error_Code |= CYEQ_LON_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */
    dlam = Longitude - Cyeq_Origin_Long;
//     if (dlam > PI)
//     {
//       dlam -= TWO_PI;
//     }
//     if (dlam < -PI)
//     {
//       dlam += TWO_PI;
//     }
    x = es * sin_lat;
    qq = CYLEQAR_Q(sin_lat,x);
    *Easting = Cyeq_a_k0 * dlam + Cyeq_False_Easting;
    *Northing = Cyeq_a * qq / two_k0 + Cyeq_False_Northing;
  }
  return (Error_Code);
} /* End Convert_Geodetic_To_Cyl_Eq_Area */


long ossimCylEquAreaProjection::Convert_Cyl_Eq_Area_To_Geodetic(double Easting,
                                                                double Northing,
                                                                double *Latitude,
                                                                double *Longitude)const
{ /* Begin Convert_Cyl_Eq_Area_To_Geodetic */
/*
 * The function Convert_Cyl_Eq_Area_To_Geodetic converts
 * Cylindrical Equal Area projection (easting and northing) coordinates
 * to geodetic (latitude and longitude) coordinates, according to the
 * current ellipsoid and Cylindrical Equal Area projection
 * coordinates.  If any errors occur, the error code(s) are returned by the
 * function, otherwise CYEQ_NO_ERROR is returned.
 *
 *    Easting           : Easting (X) in meters                  (input)
 *    Northing          : Northing (Y) in meters                 (input)
 *    Latitude          : Latitude (phi) in radians              (output)
 *    Longitude         : Longitude (lambda) in radians          (output)
 */

  double sin2beta, sin4beta, sin6beta;
  double dx;     /* Delta easting - Difference in easting (easting-FE)      */
  double dy;     /* Delta northing - Difference in northing (northing-FN)   */
  double qp;
  double beta;
  double sin_lat = sin(PI_OVER_2);
  double i;
  double x;
  long Error_Code = CYEQ_NO_ERROR;

//   if ((Easting < (Cyeq_False_Easting + Cyeq_Min_Easting))
//       || (Easting > (Cyeq_False_Easting + Cyeq_Max_Easting)))
//   { /* Easting out of range */
//     Error_Code |= CYEQ_EASTING_ERROR;
//   }
//   if ((Northing < (Cyeq_False_Northing - fabs(Cyeq_Delta_Northing)))
//       || (Northing > (Cyeq_False_Northing + fabs(Cyeq_Delta_Northing))))
//   { /* Northing out of range */
//     Error_Code |= CYEQ_NORTHING_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */
    dy = Northing - Cyeq_False_Northing;
    dx = Easting - Cyeq_False_Easting;
    x = es * sin_lat;
    qp = CYLEQAR_Q(sin_lat,x);
    i = two_k0 * dy / (Cyeq_a * qp);
    if (i > 1.0)
      i = 1.0;
    else if (i < -1.0)
      i = -1.0;
    beta = asin(i);
    sin2beta = CYEQ_COEFF_TIMES_SIN(c0, 2.0, beta);
    sin4beta = CYEQ_COEFF_TIMES_SIN(c1, 4.0, beta);
    sin6beta = CYEQ_COEFF_TIMES_SIN(c2, 6.0, beta);
    *Latitude = beta + sin2beta + sin4beta + sin6beta;
    *Longitude = Cyeq_Origin_Long + dx / Cyeq_a_k0;

//     if (*Latitude > PI_OVER_2)  /* force distorted values to 90, -90 degrees */
//       *Latitude = PI_OVER_2;
//     else if (*Latitude < -PI_OVER_2)
//       *Latitude = -PI_OVER_2;

//     if (*Longitude > PI)
//       *Longitude -= TWO_PI;
//     if (*Longitude < -PI)
//       *Longitude += TWO_PI;

//     if (*Longitude > PI) /* force distorted values to 180, -180 degrees */
//       *Longitude = PI;
//     else if (*Longitude < -PI)
//       *Longitude = -PI;


  }
  return (Error_Code);
} /* End Convert_Cyl_Eq_Area_To_Geodetic */
