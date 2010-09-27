//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top LICENSE.txt file.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimObliqueMercatorProjection.cpp 17815 2010-08-03 13:23:14Z dburken $
#include <ossim/projection/ossimObliqueMercatorProjection.h>
#include <ossim/base/ossimKeywordNames.h>

#define OMERC_NO_ERROR                0x0000
#define OMERC_LAT_ERROR               0x0001
#define OMERC_LON_ERROR               0x0002
#define OMERC_ORIGIN_LAT_ERROR        0x0004
#define OMERC_LAT1_ERROR              0x0008
#define OMERC_LAT2_ERROR              0x0010
#define OMERC_LON1_ERROR              0x0020
#define OMERC_LON2_ERROR              0x0040
#define OMERC_LAT1_LAT2_ERROR         0x0080
#define OMERC_DIFF_HEMISPHERE_ERROR   0x0100
#define OMERC_EASTING_ERROR           0x0200
#define OMERC_NORTHING_ERROR          0x0400
#define OMERC_A_ERROR                 0x0800
#define OMERC_INV_F_ERROR             0x1000
#define OMERC_SCALE_FACTOR_ERROR      0x2000
#define OMERC_LON_WARNING             0x4000

RTTI_DEF1(ossimObliqueMercatorProjection, "ossimObliqueMercatorProjection", ossimMapProjection)

#ifndef PI_OVER_2
#  define PI_OVER_2  ( M_PI / 2.0)
#endif
#ifndef PI_OVER_4
#  define PI_OVER_4  ( M_PI / 4.0)
#endif
#ifndef TWO_PI
#  define TWO_PI     (2.0 * M_PI)
#endif
#define MIN_SCALE_FACTOR  0.3
#define MAX_SCALE_FACTOR  3.0
   
#define OMERC_t(lat, e_sinlat, e_over_2)  (tan(PI_OVER_4 - lat / 2.0)) /         \
                      (pow((1 - e_sinlat) / (1 + e_sinlat), e_over_2))


ossimObliqueMercatorProjection::ossimObliqueMercatorProjection(const ossimEllipsoid& ellipsoid,
                                                               const ossimGpt& origin)
   : ossimMapProjection(ellipsoid, origin)
{
   setDefaults();
   update();
}

ossimObliqueMercatorProjection::ossimObliqueMercatorProjection(const ossimEllipsoid& ellipsoid,
                                                               const ossimGpt& origin,
                                                               const ossimGpt& point1,
                                                               const ossimGpt& point2,
                                                               double falseEasting,
                                                               double falseNorthing,
                                                               double scaleFactor)
   : ossimMapProjection(ellipsoid, origin)
{
   setParameters(point1, point2, falseEasting, falseNorthing, scaleFactor);
}

void ossimObliqueMercatorProjection::update()
{
   Set_Oblique_Mercator_Parameters(theEllipsoid.getA(),
                                   theEllipsoid.getFlattening(),
                                   theOrigin.latr(),
                                   theCentralPoint1.latr(),
                                   theCentralPoint1.lonr(),
                                   theCentralPoint2.latr(),
                                   theCentralPoint2.lonr(),
                                   OMerc_False_Easting,
                                   OMerc_False_Northing,
                                   OMerc_Scale_Factor);

   theFalseEastingNorthing.x = OMerc_False_Easting;
   theFalseEastingNorthing.y = OMerc_False_Northing;

   ossimMapProjection::update();
}

void ossimObliqueMercatorProjection::setFalseEasting(double falseEasting)
{
   OMerc_False_Easting = falseEasting;
   update();
}

void ossimObliqueMercatorProjection::setFalseNorthing(double falseNorthing)
{
   OMerc_False_Northing = falseNorthing;
   update();  
}

void ossimObliqueMercatorProjection::setFalseEastingNorthing(double falseEasting, double falseNorthing)
{
   OMerc_False_Easting = falseEasting;
   OMerc_False_Northing = falseNorthing;
   update();  
   
}

void ossimObliqueMercatorProjection::setCentralPoint1(const ossimGpt& point)
{
   theCentralPoint1 = point;

   update();
}

void ossimObliqueMercatorProjection::setCentralPoint2(const ossimGpt& point)
{
   theCentralPoint2 = point;
   update();
}

void ossimObliqueMercatorProjection::setScaleFactor(double scaleFactor)
{
   OMerc_Scale_Factor = scaleFactor;
   update();
}

void ossimObliqueMercatorProjection::setParameters(const ossimGpt& point1,
                                                   const ossimGpt& point2,
                                                   double falseEasting,
                                                   double falseNorthing,
                                                   double scaleFactor)
{
   theCentralPoint1 = point1;
   theCentralPoint2 = point2;
   OMerc_False_Easting = falseEasting;
   OMerc_False_Northing = falseNorthing;
   OMerc_Scale_Factor = scaleFactor;

   update();
}

void ossimObliqueMercatorProjection::setDefaults()
{
   // initialize the central points to be 5 degrees about the origin.
   OMerc_Delta_Northing = 40000000.0;
   OMerc_Delta_Easting  = 40000000.0;

   theCentralPoint1.latd(theOrigin.latd() - 5.0);
   theCentralPoint1.lond(theOrigin.lond() - 5.0);
   theCentralPoint2.latd(theOrigin.latd() + 5.0);
   theCentralPoint2.lond(theOrigin.lond() + 5.0);
   
   theCentralPoint1.clampLat(-90.0, 90.0);
   theCentralPoint1.clampLon(-180.0, 180.0);
   theCentralPoint2.clampLat(-90.0, 90.0);
   theCentralPoint2.clampLon(-180.0, 180.0);
   
   OMerc_False_Easting  = 0.0;
   OMerc_False_Northing = 0.0;

   OMerc_Scale_Factor   = 1.0;
}


ossimGpt ossimObliqueMercatorProjection::inverse(const ossimDpt &eastingNorthing)const
{
   double lat = 0.0;
   double lon = 0.0;
   
   Convert_Oblique_Mercator_To_Geodetic(eastingNorthing.x,
                                        eastingNorthing.y,
                                        &lat,
                                        &lon);
   
   return ossimGpt(lat*DEG_PER_RAD, lon*DEG_PER_RAD, 0.0, theDatum);  
}

ossimDpt ossimObliqueMercatorProjection::forward(const ossimGpt &latLon)const
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

   Convert_Geodetic_To_Oblique_Mercator(gpt.latr(),
                                        gpt.lonr(),
                                        &easting,
                                        &northing);
   return ossimDpt(easting, northing);
}

bool ossimObliqueMercatorProjection::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   kwl.add(prefix,
           ossimKeywordNames::SCALE_FACTOR_KW,
           OMerc_Scale_Factor,
           true);

   kwl.add(prefix,
           ossimKeywordNames::CENTRAL_POINT1_LAT_KW,
           theCentralPoint1.latd(),
           true);

   kwl.add(prefix,
           ossimKeywordNames::CENTRAL_POINT1_LON_KW,
           theCentralPoint1.lond(),
           true);

   kwl.add(prefix,
           ossimKeywordNames::CENTRAL_POINT2_LAT_KW,
           theCentralPoint2.latd(),
           true);

   kwl.add(prefix,
           ossimKeywordNames::CENTRAL_POINT2_LON_KW,
           theCentralPoint2.lond(),
           true);
           
   
   return ossimMapProjection::saveState(kwl, prefix);   
}

bool ossimObliqueMercatorProjection::loadState(const ossimKeywordlist& kwl,
                                             const char* prefix)
{
   bool flag = ossimMapProjection::loadState(kwl, prefix);
   const char* type          = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   const char* point1Lat     = kwl.find(prefix, ossimKeywordNames::CENTRAL_POINT1_LAT_KW);
   const char* point1Lon     = kwl.find(prefix, ossimKeywordNames::CENTRAL_POINT1_LON_KW);
   const char* point2Lat     = kwl.find(prefix, ossimKeywordNames::CENTRAL_POINT2_LAT_KW);
   const char* point2Lon     = kwl.find(prefix, ossimKeywordNames::CENTRAL_POINT2_LON_KW);
   const char* scale         = kwl.find(prefix, ossimKeywordNames::SCALE_FACTOR_KW);
   
   setDefaults();

   if(ossimString(type) == STATIC_TYPE_NAME(ossimObliqueMercatorProjection))
   {
      OMerc_False_Easting  = theFalseEastingNorthing.x;
      OMerc_False_Northing = theFalseEastingNorthing.y;

      if(point1Lat&&point1Lon&&point2Lat&&point2Lon)
      {
         theCentralPoint1.latd(ossimString(point1Lat).toDouble());
         theCentralPoint1.lond(ossimString(point1Lon).toDouble());
         theCentralPoint2.latd(ossimString(point2Lat).toDouble());
         theCentralPoint2.lond(ossimString(point2Lon).toDouble());
      }
      if(scale)
      {
         OMerc_Scale_Factor = ossimString(scale).toDouble();
      }
   }
   
   update();

   return flag;

}

/*
 *                              FUNCTIONS     
 */

long ossimObliqueMercatorProjection::Set_Oblique_Mercator_Parameters(double a,
                                                                     double f,
                                                                     double Origin_Latitude,
                                                                     double Latitude_1,
                                                                     double Longitude_1,
                                                                     double Latitude_2,
                                                                     double Longitude_2,
                                                                     double False_Easting,
                                                                     double False_Northing,
                                                                     double Scale_Factor)
{ /* BEGIN Set_Oblique_Mercator_Parameters */
/*
 * The function Set_Oblique_Mercator_Parameters receives the ellipsoid parameters and
 * projection parameters as inputs, and sets the corresponding state
 * variables.  If any errors occur, the error code(s) are returned by the function, 
 * otherwise OMERC_NO_ERROR is returned.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters  (input)
 *    f                 : Flattening of ellipsoid                  (input)
 *    Origin_Latitude   : Latitude, in radians, at which the       (input)
 *                          point scale factor is 1.0
 *    Latitude_1        : Latitude, in radians, of first point lying on
 *                          central line                           (input)
 *    Longitude_1       : Longitude, in radians, of first point lying on
 *                          central line                           (input)
 *    Latitude_2        : Latitude, in radians, of second point lying on
 *                          central line                           (input)
 *    Longitude_2       : Longitude, in radians, of second point lying on
 *                          central line                           (input)
 *    False_Easting     : A coordinate value, in meters, assigned to the
 *                          central meridian of the projection     (input)
 *    False_Northing    : A coordinate value, in meters, assigned to the
 *                          origin latitude of the projection      (input)
 *    Scale_Factor      : Multiplier which reduces distances in the
 *                          projection to the actual distance on the
 *                          ellipsoid                              (input)
 */

//  double inv_f = 1 / f;
  double es2, one_MINUS_es2;
  double cos_olat, cos_olat2;
  double sin_olat, sin_olat2, es2_sin_olat2;
  double t0, t1, t2;
  double D, D2, D2_MINUS_1, sqrt_D2_MINUS_1;
  double H, L, LH;
  double E2;
  double F, G, J, P;
  double dlon;
  long Error_Code = OMERC_NO_ERROR;

//   if (a <= 0.0)
//   { /* Semi-major axis must be greater than zero */
//     Error_Code |= OMERC_A_ERROR;
//   }
//   if ((inv_f < 250) || (inv_f > 350))
//   { /* Inverse flattening must be between 250 and 350 */
//     Error_Code |= OMERC_INV_F_ERROR;
//   }
//   if ((Origin_Latitude <= -PI_OVER_2) || (Origin_Latitude >= PI_OVER_2))
//   { /* origin latitude out of range -  can not be at a pole */
//     Error_Code |= OMERC_ORIGIN_LAT_ERROR;
//   }
//   if ((Latitude_1 <= -PI_OVER_2) || (Latitude_1 >= PI_OVER_2))
//   { /* first latitude out of range -  can not be at a pole */
//     Error_Code |= OMERC_LAT1_ERROR;
//   }
//   if ((Latitude_2 <= -PI_OVER_2) || (Latitude_2 >= PI_OVER_2))
//   { /* second latitude out of range -  can not be at a pole */
//     Error_Code |= OMERC_LAT2_ERROR;
//   }
//   if (Latitude_1 == 0.0)
//   { /* first latitude can not be at the equator */
//     Error_Code |= OMERC_LAT1_ERROR;
//   }
//   if (Latitude_1 == Latitude_2)
//   { /* first and second latitudes can not be equal */
//     Error_Code |= OMERC_LAT1_LAT2_ERROR;
//   }
//   if (((Latitude_1 < 0.0) && (Latitude_2 > 0.0)) ||
//       ((Latitude_1 > 0.0) && (Latitude_2 < 0.0)))
//   { /*first and second points can not be in different hemispheres */
//     Error_Code |= OMERC_DIFF_HEMISPHERE_ERROR;
//   }
//   if ((Longitude_1 < -PI) || (Longitude_1 > TWO_PI))
//   { /* first longitude out of range */
//     Error_Code |= OMERC_LON1_ERROR;
//   }
//   if ((Longitude_2 < -PI) || (Longitude_2 > TWO_PI))
//   { /* first longitude out of range */
//     Error_Code |= OMERC_LON2_ERROR;
//   }
//   if ((Scale_Factor < MIN_SCALE_FACTOR) || (Scale_Factor > MAX_SCALE_FACTOR))
//   { /* scale factor out of range */
//     Error_Code |= OMERC_SCALE_FACTOR_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */

    OMerc_a = a;
    OMerc_f = f;
    OMerc_Origin_Lat = Origin_Latitude;
    OMerc_Lat_1 = Latitude_1;
    OMerc_Lat_2 = Latitude_2;
    OMerc_Lon_1 = Longitude_1;
    OMerc_Lon_2 = Longitude_2;
    OMerc_Scale_Factor = Scale_Factor;
    OMerc_False_Northing = False_Northing;
    OMerc_False_Easting = False_Easting;

    es2 = 2 * OMerc_f - OMerc_f * OMerc_f;
    es = sqrt(es2);
    one_MINUS_es2 = 1 - es2;
    es_OVER_2 = es / 2.0;

    cos_olat = cos(OMerc_Origin_Lat);
    cos_olat2 = cos_olat * cos_olat;
    sin_olat = sin(OMerc_Origin_Lat);
    sin_olat2 = sin_olat * sin_olat;
    es2_sin_olat2 = es2 * sin_olat2;

    OMerc_B = sqrt(1 + (es2 * cos_olat2 * cos_olat2) / one_MINUS_es2);
    OMerc_A = (OMerc_a * OMerc_B * OMerc_Scale_Factor * sqrt(one_MINUS_es2)) / (1.0 - es2_sin_olat2);  
    A_over_B = OMerc_A / OMerc_B;
    B_over_A = OMerc_B / OMerc_A;

    t0 = OMERC_t(OMerc_Origin_Lat, es * sin_olat, es_OVER_2);
    t1 = OMERC_t(OMerc_Lat_1, es * sin(OMerc_Lat_1), es_OVER_2);  
    t2 = OMERC_t(OMerc_Lat_2, es * sin(OMerc_Lat_2), es_OVER_2);  

    D = (OMerc_B * sqrt(one_MINUS_es2)) / (cos_olat * sqrt(1.0 - es2_sin_olat2)); 
    D2 = D * D;
    if (D2 < 1.0)
      D2 = 1.0;
    D2_MINUS_1 = D2 - 1.0;
    sqrt_D2_MINUS_1 = sqrt(D2_MINUS_1);
    if (D2_MINUS_1 > 1.0e-10)
    {
      if (OMerc_Origin_Lat >= 0.0)
        OMerc_E = (D + sqrt_D2_MINUS_1) * pow(t0, OMerc_B);
      else
        OMerc_E = (D - sqrt_D2_MINUS_1) * pow(t0, OMerc_B);
    }
    else
      OMerc_E = D * pow(t0, OMerc_B);
    H = pow(t1, OMerc_B);
    L = pow(t2, OMerc_B);
    F = OMerc_E / H;
    G = (F - 1.0 / F) / 2.0;
    E2 = OMerc_E * OMerc_E;
    LH = L * H;
    J = (E2 - LH) / (E2 + LH);
    P = (L - H) / (L + H);

    dlon = OMerc_Lon_1 - OMerc_Lon_2;
    if (dlon < -M_PI )
      OMerc_Lon_2 -= TWO_PI;
    if (dlon > M_PI)
      OMerc_Lon_2 += TWO_PI;
    dlon = OMerc_Lon_1 - OMerc_Lon_2;
    OMerc_Origin_Long = (OMerc_Lon_1 + OMerc_Lon_2) / 2.0 - (atan(J * tan(OMerc_B * dlon / 2.0) / P)) / OMerc_B;

    dlon = OMerc_Lon_1 - OMerc_Origin_Long;
//     if (dlon < -M_PI )
//       OMerc_Origin_Long -= TWO_PI;
//     if (dlon > M_PI)
//       OMerc_Origin_Long += TWO_PI;
   
    dlon = OMerc_Lon_1 - OMerc_Origin_Long;
    OMerc_gamma = atan(sin(OMerc_B * dlon) / G);
    cos_gamma = cos(OMerc_gamma);
    sin_gamma = sin(OMerc_gamma);
  
    OMerc_azimuth = asin(D * sin_gamma);
    cos_azimuth = cos(OMerc_azimuth);
    sin_azimuth = sin(OMerc_azimuth);

   if (OMerc_Origin_Lat >= 0)
      OMerc_u =  A_over_B * atan(sqrt_D2_MINUS_1/cos_azimuth);
   else
      OMerc_u = -A_over_B * atan(sqrt_D2_MINUS_1/cos_azimuth);

  } /* End if(!Error_Code) */
  return (Error_Code);
} /* End Set_Oblique_Mercator_Parameters */

void ossimObliqueMercatorProjection::Get_Oblique_Mercator_Parameters(double *a,
                                                                     double *f,
                                                                     double *Origin_Latitude,
                                                                     double *Latitude_1,
                                                                     double *Longitude_1,
                                                                     double *Latitude_2,
                                                                     double *Longitude_2,
                                                                     double *False_Easting,
                                                                     double *False_Northing,
                                                                     double *Scale_Factor)const
{ /* Begin Get_Oblique_Mercator_Parameters */
/*
 * The function Get_Oblique_Mercator_Parameters returns the current ellipsoid
 * parameters and Oblique Mercator projection parameters.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters  (output)
 *    f                 : Flattening of ellipsoid                  (output)
 *    Origin_Latitude   : Latitude, in radians, at which the       (output)
 *                          point scale factor is 1.0
 *    Latitude_1        : Latitude, in radians, of first point lying on
 *                          central line                           (output)
 *    Longitude_1       : Longitude, in radians, of first point lying on
 *                          central line                           (output)
 *    Latitude_2        : Latitude, in radians, of second point lying on
 *                          central line                           (output)
 *    Longitude_2       : Longitude, in radians, of second point lying on
 *                          central line                           (output)
 *    False_Easting     : A coordinate value, in meters, assigned to the
 *                          central meridian of the projection     (output)
 *    False_Northing    : A coordinate value, in meters, assigned to the
 *                          origin latitude of the projection      (output)
 *    Scale_Factor      : Multiplier which reduces distances in the
 *                          projection to the actual distance on the
 *                          ellipsoid                              (output)
 */

  *a = OMerc_a;
  *f = OMerc_f;
  *Origin_Latitude = OMerc_Origin_Lat;
  *Latitude_1 = OMerc_Lat_1;
  *Longitude_1 = OMerc_Lon_1;
  *Latitude_2 = OMerc_Lat_2;
  *Longitude_2 = OMerc_Lon_2;
  *Scale_Factor = OMerc_Scale_Factor;
  *False_Easting = OMerc_False_Easting;
  *False_Northing = OMerc_False_Northing;
  
  return;
} /* End Get_Azimuthal_Equidistant_Parameters */

long ossimObliqueMercatorProjection::Convert_Geodetic_To_Oblique_Mercator(double Latitude,
                                                                          double Longitude,
                                                                          double *Easting,
                                                                          double *Northing)const
{ /* BEGIN Convert_Geodetic_To_Oblique_Mercator */
/*
 * The function Convert_Geodetic_To_Oblique_Mercator converts geodetic (latitude and
 * longitude) coordinates to Oblique Mercator projection (easting and
 * northing) coordinates, according to the current ellipsoid and Oblique Mercator 
 * projection parameters.  If any errors occur, the error code(s) are returned 
 * by the function, otherwise OMERC_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi), in radians           (input)
 *    Longitude         : Longitude (lambda), in radians       (input)
 *    Easting           : Easting (X), in meters               (output)
 *    Northing          : Northing (Y), in meters              (output)
 */

  double dlam, B_dlam, cos_B_dlam;
  double t, S, T, V, U;
  double Q, Q_inv;
  /* Coordinate axes defined with respect to the azimuth of the center line */
  /* Natural origin*/
  double v = 0;
  double u = 0;
  long Error_Code = OMERC_NO_ERROR;

//   if ((Latitude < -PI_OVER_2) || (Latitude > PI_OVER_2))
//   { /* Latitude out of range */
//     Error_Code |= OMERC_LAT_ERROR;
//   }
//   if ((Longitude < -M_PI) || (Longitude > TWO_PI))
//   { /* Longitude out of range */
//     Error_Code |= OMERC_LON_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */
    dlam = Longitude - OMerc_Origin_Long;

//     if (fabs(dlam) >= PI_OVER_2)
//     { /* Distortion will result if Longitude is 90 degrees or more from the Central Meridian */
//       Error_Code |= OMERC_LON_WARNING;
//     }

//     if (dlam > M_PI)
//     {
//       dlam -= TWO_PI;
//     }
//     if (dlam < -M_PI)
//     {
//       dlam += TWO_PI;
//     }

    if (fabs(fabs(Latitude) - PI_OVER_2) > 1.0e-10)
    {
      t = OMERC_t(Latitude, es * sin(Latitude), es_OVER_2);  
      Q = OMerc_E / pow(t, OMerc_B);
      Q_inv = 1.0 / Q;
      S = (Q - Q_inv) / 2.0;
      T = (Q + Q_inv) / 2.0;
      B_dlam = OMerc_B * dlam;
      V = sin(B_dlam);
      U = ((-1.0 * V * cos_gamma) + (S * sin_gamma)) / T;
      if (fabs(fabs(U) - 1.0) < 1.0e-10)
      { /* Point projects into infinity */
        Error_Code |= OMERC_LON_ERROR;
      }
      else
      {
        v = A_over_B * log((1.0 - U) / (1.0 + U)) / 2.0;
        cos_B_dlam = cos(B_dlam);
        if (fabs(cos_B_dlam) < 1.0e-10)
          u = OMerc_A * B_dlam;
        else
          u = A_over_B * atan(((S * cos_gamma) + (V * sin_gamma)) / cos_B_dlam);
      }
    }
    else
    {
      if (Latitude > 0.0)
        v = A_over_B * log(tan(PI_OVER_4 - (OMerc_gamma / 2.0)));
      else
        v = A_over_B * log(tan(PI_OVER_4 + (OMerc_gamma / 2.0)));
      u = A_over_B * Latitude;
    }


    u = u - OMerc_u;

    *Easting  = OMerc_False_Easting + v * cos_azimuth + u * sin_azimuth;
    *Northing = OMerc_False_Northing + u * cos_azimuth - v * sin_azimuth;

  }
  return (Error_Code);
} /* End Convert_Geodetic_To_Oblique_Mercator */


long ossimObliqueMercatorProjection::Convert_Oblique_Mercator_To_Geodetic(double Easting,
                                                                          double Northing,
                                                                          double *Latitude,
                                                                          double *Longitude)const
{ /* Begin Convert_Oblique_Mercator_To_Geodetic */
/*
 * The function Convert_Oblique_Mercator_To_Geodetic converts Oblique Mercator projection
 * (easting and northing) coordinates to geodetic (latitude and longitude)
 * coordinates, according to the current ellipsoid and Oblique Mercator projection
 * coordinates.  If any errors occur, the error code(s) are returned by the
 * function, otherwise OMERC_NO_ERROR is returned.
 *
 *    Easting           : Easting (X), in meters                  (input)
 *    Northing          : Northing (Y), in meters                 (input)
 *    Latitude          : Latitude (phi), in radians              (output)
 *    Longitude         : Longitude (lambda), in radians          (output)
 */

  double dx, dy;
  /* Coordinate axes defined with respect to the azimuth of the center line */
  /* Natural origin*/
  double u, v;
  double Q_prime, Q_prime_inv;
  double S_prime, T_prime, V_prime, U_prime;
  double t;
  double es_sin;
  double u_B_over_A;
  double phi;
  double temp_phi = 0.0;
  long Error_Code = OMERC_NO_ERROR; 

//   if ((Easting < (OMerc_False_Easting - OMerc_Delta_Easting)) 
//       || (Easting > (OMerc_False_Easting + OMerc_Delta_Easting)))
//   { /* Easting out of range  */
//     Error_Code |= OMERC_EASTING_ERROR;
//   }
//   if ((Northing < (OMerc_False_Northing - OMerc_Delta_Northing)) 
//       || (Northing > (OMerc_False_Northing + OMerc_Delta_Northing)))
//   { /* Northing out of range */
//     Error_Code |= OMERC_NORTHING_ERROR;
//   }

  if (!Error_Code)
  {
    dy = Northing - OMerc_False_Northing;
    dx = Easting - OMerc_False_Easting;
    v = dx * cos_azimuth - dy * sin_azimuth;
    u = dy * cos_azimuth + dx * sin_azimuth;
    u = u + OMerc_u;
    Q_prime = exp(-1.0 * (v * B_over_A ));
    Q_prime_inv = 1.0 / Q_prime;
    S_prime = (Q_prime - Q_prime_inv) / 2.0;
    T_prime = (Q_prime + Q_prime_inv) / 2.0;
    u_B_over_A = u * B_over_A;
    V_prime = sin(u_B_over_A);
    U_prime = (V_prime * cos_gamma + S_prime * sin_gamma) / T_prime;
    if (fabs(fabs(U_prime) - 1.0) < 1.0e-10)
    {
      if (U_prime > 0)
        *Latitude = PI_OVER_2;
      else
        *Latitude = -PI_OVER_2;
      *Longitude = OMerc_Origin_Long;
    }
    else
    {
      t = pow(OMerc_E / sqrt((1.0 + U_prime) / (1.0 - U_prime)), 1.0 / OMerc_B);
      phi = PI_OVER_2 - 2.0 * atan(t);
      while (fabs(phi - temp_phi) > 1.0e-10)
      {
        temp_phi = phi;
        es_sin = es * sin(phi);
        phi = PI_OVER_2 - 2.0 * atan(t * pow((1.0 - es_sin) / (1.0 + es_sin), es_OVER_2));
      }
      *Latitude = phi;
      *Longitude = OMerc_Origin_Long - atan2((S_prime * cos_gamma - V_prime * sin_gamma), cos(u_B_over_A)) / OMerc_B;
    }

//     if (fabs(*Latitude) < 2.0e-7)  /* force lat to 0 to avoid -0 degrees */
//       *Latitude = 0.0;
//     if (*Latitude > PI_OVER_2)  /* force distorted values to 90, -90 degrees */
//       *Latitude = PI_OVER_2;
//     else if (*Latitude < -PI_OVER_2)
//       *Latitude = -PI_OVER_2;

//     if (*Longitude > PI)
//       *Longitude -= TWO_PI;
//     if (*Longitude < -PI)
//       *Longitude += TWO_PI;

//     if (fabs(*Longitude) < 2.0e-7)  /* force lon to 0 to avoid -0 degrees */
//       *Longitude = 0.0;
//     if (*Longitude > PI)  /* force distorted values to 180, -180 degrees */
//       *Longitude = PI;
//     else if (*Longitude < -PI)
//       *Longitude = -PI;

//     if (fabs(*Longitude - OMerc_Origin_Long) >= PI_OVER_2)
//     { /* Distortion will result if Longitude is 90 degrees or more from the Central Meridian */
//       Error_Code |= OMERC_LON_WARNING;
//     }

  }
  return (Error_Code);
} /* End Convert_Oblique_Mercator_To_Geodetic */

//*************************************************************************************************
//! Returns TRUE if principal parameters are within epsilon tolerance.
//*************************************************************************************************
bool ossimObliqueMercatorProjection::operator==(const ossimProjection& proj) const
{
   if (!ossimMapProjection::operator==(proj))
      return false;

   ossimObliqueMercatorProjection* p = PTR_CAST(ossimObliqueMercatorProjection, &proj);
   if (!p) return false;

   if (theCentralPoint1 != p->theCentralPoint1) return false;
   if (theCentralPoint2 != p->theCentralPoint2) return false;
   if (!ossim::almostEqual(OMerc_Scale_Factor,p->OMerc_Scale_Factor)) return false;

   return true;
}
