//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Calls Geotrans Mercator projection code.  
//*******************************************************************
//  $Id: ossimMercatorProjection.cpp 17815 2010-08-03 13:23:14Z dburken $

#include <math.h>
#include <ossim/projection/ossimMercatorProjection.h>
#include <ossim/base/ossimKeywordNames.h>

RTTI_DEF1(ossimMercatorProjection, "ossimMercatorProjection", ossimMapProjection)
/***************************************************************************/
/*
 *                               DEFINES
 */

#define PI         3.14159265358979323e0  /* PI                            */
#define PI_OVER_2  ( PI / 2.0e0)  
#define MAX_LAT    ( (PI * 89.5) / 180.0 )  /* 89.5 degrees in radians         */

#define MERC_NO_ERROR           0x0000
#define MERC_LAT_ERROR          0x0001
#define MERC_LON_ERROR          0x0002
#define MERC_EASTING_ERROR      0x0004
#define MERC_NORTHING_ERROR     0x0008
#define MERC_ORIGIN_LAT_ERROR   0x0010
#define MERC_CENT_MER_ERROR     0x0020
#define MERC_A_ERROR            0x0040
#define MERC_B_ERROR            0x0080
#define MERC_A_LESS_B_ERROR     0x0100

ossimMercatorProjection::ossimMercatorProjection(const ossimEllipsoid& ellipsoid,
                                                 const ossimGpt& origin)
   :ossimMapProjection(ellipsoid, origin)
{
   setDefaults();
   update();
}

ossimMercatorProjection::ossimMercatorProjection(const ossimEllipsoid& ellipsoid,
                                                 const ossimGpt& origin,
                                                 double falseEasting,
                                                 double falseNorthing,
                                                 double scaleFactor)
   :ossimMapProjection(ellipsoid, origin)
{
   setDefaults();
   Merc_False_Easting  = falseEasting;
   Merc_False_Northing = falseNorthing;
   Merc_Scale_Factor   = scaleFactor;

   update();
}

void ossimMercatorProjection::update()
{
   
   Set_Mercator_Parameters(theEllipsoid.getA(),
                           theEllipsoid.getFlattening(),
                           theOrigin.latr(),
                           theOrigin.lonr(),
                           Merc_False_Easting,
                           Merc_False_Northing,
                           &Merc_Scale_Factor);

   theFalseEastingNorthing.x = Merc_False_Easting;
   theFalseEastingNorthing.y = Merc_False_Northing;

   ossimMapProjection::update();
}

void ossimMercatorProjection::setFalseEasting(double falseEasting)
{
   Merc_False_Easting = falseEasting;
   update();
}

void ossimMercatorProjection::setFalseNorthing(double falseNorthing)
{
   Merc_False_Northing = falseNorthing;
   update();
}

void ossimMercatorProjection::setScaleFactor(double scaleFactor)
{
   Merc_Scale_Factor = scaleFactor;
   update();
}

void ossimMercatorProjection::setFalseEastingNorthing(double falseEasting,
                                                      double falseNorthing)
{
   Merc_False_Easting = falseEasting;
   Merc_False_Northing = falseNorthing;
   update();
}

void ossimMercatorProjection::setParameters(double falseEasting,
                                            double falseNorthing,
                                            double scaleFactor)
{
   Merc_False_Easting  = falseEasting;
   Merc_False_Northing = falseNorthing;
   Merc_Scale_Factor   = scaleFactor;
   
   update(); 
}

void ossimMercatorProjection::setDefaults()
{
   Merc_False_Easting  = 0.0;
   Merc_False_Northing = 0.0;
   Merc_Delta_Easting  = 20237883.0;
   Merc_Delta_Northing = 23421740.0;
   Merc_Scale_Factor   = 1.0;
}

ossimGpt ossimMercatorProjection::inverse(const ossimDpt &eastingNorthing)const
{
   double lat = 0.0;
   double lon = 0.0;

   if(theSphericalFlag)
   {
      double shift = M_PI * 6378137.0;
      lon = (eastingNorthing.x / shift) * 180.0;
      lat = (eastingNorthing.y / shift) * 180.0;
      
      lat = 180 / M_PI * (2 * atan( exp( lat * M_PI / 180.0)) - M_PI / 2.0);   
   }
   else 
   {
      Convert_Mercator_To_Geodetic(eastingNorthing.x,
                                   eastingNorthing.y,
                                   &lat,
                                   &lon);
      lat = ossim::radiansToDegrees(lat);
      lon = ossim::radiansToDegrees(lon);
   }

   
   
   return ossimGpt(lat, lon, 0.0, theDatum);  
}

ossimDpt ossimMercatorProjection::forward(const ossimGpt &latLon)const
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
   if(theSphericalFlag)
   {
      double lat = latLon.latd();
      double lon = latLon.lond();
      double shift = M_PI * Merc_a;
      easting = lon * shift / 180.0;
      northing = log( tan((90 + lat) * M_PI / 360.0 )) / (M_PI / 180.0);
      
      northing = northing * shift / 180.0;
   }
   else 
   {
      Convert_Geodetic_To_Mercator(gpt.latr(),
                                   gpt.lonr(),
                                   &easting,
                                   &northing);
   }      

   
   return ossimDpt(easting, northing);
}


bool ossimMercatorProjection::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   kwl.add(prefix,
           ossimKeywordNames::SCALE_FACTOR_KW,
           Merc_Scale_Factor,
           true);

   return ossimMapProjection::saveState(kwl, prefix);
}

bool ossimMercatorProjection::loadState(const ossimKeywordlist& kwl,
                                        const char* prefix)
{
   bool flag = ossimMapProjection::loadState(kwl, prefix);

   const char* type          = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   const char* scaleFactor   = kwl.find(prefix, ossimKeywordNames::SCALE_FACTOR_KW);
   
   setDefaults();

   if(ossimString(type) == STATIC_TYPE_NAME(ossimMercatorProjection))
   {
      Merc_False_Easting  = theFalseEastingNorthing.x;
      Merc_False_Northing = theFalseEastingNorthing.y;

      if(scaleFactor)
      {
         Merc_Scale_Factor = ossimString(scaleFactor).toDouble();
      }
   }
   update();
   
   return flag;
}


/***************************************************************************/
/*
 *                              FUNCTIONS     
 */


long ossimMercatorProjection::Set_Mercator_Parameters(double a,      
                                                      double f,
                                                      double Origin_Latitude,
                                                      double Central_Meridian,
                                                      double False_Easting,
                                                      double False_Northing,
                                                      double *Scale_Factor)
{ /* BEGIN Set_Mercator_Parameters */
/*
 * The function Set_Mercator_Parameters receives the ellipsoid parameters and
 * Mercator projection parameters as inputs, and sets the corresponding state 
 * variables.  It calculates and returns the scale factor.  If any errors
 * occur, the error code(s) are returned by the function, otherwise Merc_NO_ERROR
 * is returned.
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
 *    Scale_Factor      : Multiplier which reduces distances in the 
 *                          projection to the actual distance on the
 *                          ellipsoid                               (output)
 */

  double es2;   /* Eccentricity squared of ellipsoid to the second power    */
  double es3;   /* Eccentricity squared of ellipsoid to the third power     */
  double es4;   /* Eccentricity squared of ellipsoid to the fourth power    */
  double sin_olat; /* sin(Origin_Latitude), temp variable */
//  double inv_f = 1 / f;
  long Error_Code = MERC_NO_ERROR;

   theSphericalFlag = ossim::almostEqual(f, 0.0);

//   if (a <= 0.0)
//   { /* Semi-major axis must be greater than zero */
//     Error_Code |= MERC_A_ERROR;
//   }
//   if ((inv_f < 250) || (inv_f > 350))
//   { /* Inverse flattening must be between 250 and 350 */
//     Error_Code |= MERC_INV_F_ERROR;
//   }
//   if ((Origin_Latitude < -MAX_LAT) || (Origin_Latitude > MAX_LAT))
//   { /* origin latitude out of range */
//     Error_Code |= MERC_ORIGIN_LAT_ERROR;
//   }
//   if ((Central_Meridian < -PI) || (Central_Meridian > (2*PI)))
//   { /* origin longitude out of range */
//     Error_Code |= MERC_CENT_MER_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */
    Merc_a = a;
    Merc_f = f;
    Merc_Origin_Lat = Origin_Latitude;
//     if (Central_Meridian > PI)
//       Central_Meridian -= (2*PI);
    Merc_Origin_Long = Central_Meridian;
    Merc_False_Northing = False_Northing;
    Merc_False_Easting = False_Easting;
    Merc_es = 2 * Merc_f - Merc_f * Merc_f;
    Merc_e = sqrt(Merc_es);
    sin_olat = sin(Origin_Latitude);
    Merc_Scale_Factor = 1.0 / ( sqrt(1.e0 - Merc_es * sin_olat * sin_olat) 
                                / cos(Origin_Latitude) );
    es2 = Merc_es * Merc_es;
    es3 = es2 * Merc_es;
    es4 = es3 * Merc_es;
    Merc_ab = Merc_es / 2.e0 + 5.e0 * es2 / 24.e0 + es3 / 12.e0
              + 13.e0 * es4 / 360.e0;
    Merc_bb = 7.e0 * es2 / 48.e0 + 29.e0 * es3 / 240.e0 
              + 811.e0 * es4 / 11520.e0;
    Merc_cb = 7.e0 * es3 / 120.e0 + 81.e0 * es4 / 1120.e0;
    Merc_db = 4279.e0 * es4 / 161280.e0;
    *Scale_Factor = Merc_Scale_Factor;
    Convert_Geodetic_To_Mercator( MAX_LAT, (Merc_Origin_Long + PI),                                           
                                  &Merc_Delta_Easting, &Merc_Delta_Northing);
    if (Merc_Delta_Easting < 0)
      Merc_Delta_Easting = -Merc_Delta_Easting;
    Merc_Delta_Easting *= 1.01;
    Merc_Delta_Easting -= Merc_False_Easting;
    Merc_Delta_Northing *= 1.01;
    Merc_Delta_Northing -= Merc_False_Northing;
  } /* END OF if(!Error_Code) */
  return (Error_Code);
} /* END OF Set_Mercator_Parameters */


void ossimMercatorProjection::Get_Mercator_Parameters(double *a,
                                                      double *f,
                                                      double *Origin_Latitude,
                                                      double *Central_Meridian,
                                                      double *False_Easting,
                                                      double *False_Northing,
                                                      double *Scale_Factor)const
{ /* BEGIN Get_Mercator_Parameters */
/*
 * The function Get_Mercator_Parameters returns the current ellipsoid
 * parameters, Mercator projection parameters, and scale factor.
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
 *    Scale_Factor      : Multiplier which reduces distances in the 
 *                          projection to the actual distance on the
 *                          ellipsoid                               (output)
 */

  *a = Merc_a;
  *f = Merc_f;
  *Origin_Latitude = Merc_Origin_Lat;
  *Central_Meridian = Merc_Origin_Long;
  *False_Easting = Merc_False_Easting;
  *False_Northing = Merc_False_Northing;
  *Scale_Factor = Merc_Scale_Factor;
  
  return;
} /* END OF Get_Mercator_Parameters */


long ossimMercatorProjection::Convert_Geodetic_To_Mercator (double Latitude,
                                                            double Longitude,
                                                            double *Easting,
                                                            double *Northing)const
{ /* BEGIN Convert_Geodetic_To_Mercator */
   long Error_Code = MERC_NO_ERROR;
/*
 * The function Convert_Geodetic_To_Mercator converts geodetic (latitude and
 * longitude) coordinates to Mercator projection (easting and northing)
 * coordinates, according to the current ellipsoid and Mercator projection
 * parameters.  If any errors occur, the error code(s) are returned by the
 * function, otherwise MERC_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi) in radians           (input)
 *    Longitude         : Longitude (lambda) in radians       (input)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */

  double ctanz2;        /* Cotangent of z/2 - z - Isometric colatitude     */
  double e_x_sinlat;    /* e * sin(Latitude)                               */
  double Delta_Long;    /* Difference in origin longitude and longitude    */
  double tan_temp;
  double pow_temp;


//   if ((Latitude < -MAX_LAT) || (Latitude > MAX_LAT))
//   { /* Latitude out of range */
//     Error_Code |= MERC_LAT_ERROR;
//   }
//   if ((Longitude < -PI) || (Longitude > (2*PI)))
//   { /* Longitude out of range */
//     Error_Code |= MERC_LON_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */
    if (Longitude > PI)
      Longitude -= (2*PI);
    e_x_sinlat = Merc_e * sin(Latitude);
    tan_temp = tan(PI / 4.e0 + Latitude / 2.e0);
    pow_temp = pow( ((1.e0 - e_x_sinlat) / (1.e0 + e_x_sinlat)),
                    (Merc_e / 2.e0) );
    ctanz2 = tan_temp * pow_temp;
    *Northing = Merc_Scale_Factor * Merc_a * log(ctanz2) + Merc_False_Northing;
    Delta_Long = Longitude - Merc_Origin_Long;
//     if (Delta_Long > PI)
//       Delta_Long -= (2 * PI);
//     if (Delta_Long < -PI)
//       Delta_Long += (2 * PI);
    *Easting = Merc_Scale_Factor * Merc_a * Delta_Long
               + Merc_False_Easting;
  }
  return (Error_Code);
} /* END OF Convert_Geodetic_To_Mercator */


long ossimMercatorProjection::Convert_Mercator_To_Geodetic(double Easting,
                                                           double Northing,
                                                           double *Latitude,
                                                           double *Longitude)const
{ /* BEGIN Convert_Mercator_To_Geodetic */
/*
 * The function Convert_Mercator_To_Geodetic converts Mercator projection
 * (easting and northing) coordinates to geodetic (latitude and longitude)
 * coordinates, according to the current ellipsoid and Mercator projection
 * coordinates.  If any errors occur, the error code(s) are returned by the
 * function, otherwise MERC_NO_ERROR is returned.
 *
 *    Easting           : Easting (X) in meters                  (input)
 *    Northing          : Northing (Y) in meters                 (input)
 *    Latitude          : Latitude (phi) in radians              (output)
 *    Longitude         : Longitude (lambda) in radians          (output)
 */

  double dx;     /* Delta easting - Difference in easting (easting-FE)      */
  double dy;     /* Delta northing - Difference in northing (northing-FN)   */
  double xphi;   /* Isometric latitude                                      */
   long Error_Code = MERC_NO_ERROR;

#if 0
   if(theSphericalFlag)
   {
      *Latitude = M_PI*.5 - 2.0 * atan(exp(-Easting / Merc_a));
      *Longitude = Easting/Merc_a;
      
      return Error_Code;
   }
#endif
//   if ((Easting < (Merc_False_Easting - Merc_Delta_Easting))
//       || (Easting > (Merc_False_Easting + Merc_Delta_Easting)))
//   { /* Easting out of range */
//     Error_Code |= MERC_EASTING_ERROR;
//   }
//   if ((Northing < (Merc_False_Northing - Merc_Delta_Northing))
//       || (Northing > (Merc_False_Northing + Merc_Delta_Northing)))
//   { /* Northing out of range */
//     Error_Code |= MERC_NORTHING_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */
    dy = Northing - Merc_False_Northing;
    dx = Easting - Merc_False_Easting;
    *Longitude = Merc_Origin_Long + dx / (Merc_Scale_Factor * Merc_a);
    xphi = PI / 2.e0 
           - 2.e0 * atan(1.e0 / exp(dy / (Merc_Scale_Factor * Merc_a)));
    *Latitude = xphi + Merc_ab * sin(2.e0 * xphi) + Merc_bb * sin(4.e0 * xphi)
                + Merc_cb * sin(6.e0 * xphi) + Merc_db * sin(8.e0 * xphi);
//     if (*Longitude > PI)
//       *Longitude -= (2 * PI);
//     if (*Longitude < -PI)
//       *Longitude += (2 * PI);
  }
  return (Error_Code);
} /* END OF Convert_Mercator_To_Geodetic */

//*************************************************************************************************
//! Returns TRUE if principal parameters are within epsilon tolerance.
//*************************************************************************************************
bool ossimMercatorProjection::operator==(const ossimProjection& proj) const
{
   if (!ossimMapProjection::operator==(proj))
      return false;

   ossimMercatorProjection* p = PTR_CAST(ossimMercatorProjection, &proj);
   if (!p) return false;

   if (!ossim::almostEqual(Merc_Scale_Factor,p->Merc_Scale_Factor)) return false;

   return true;
}
