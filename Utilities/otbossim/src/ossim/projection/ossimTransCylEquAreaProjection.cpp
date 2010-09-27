//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Calls Geotrans Transverse Cylindrical Equal Area
// projection code.  
//*******************************************************************
//  $Id: ossimTransCylEquAreaProjection.cpp 17815 2010-08-03 13:23:14Z dburken $

#include <math.h>
#include <ossim/projection/ossimTransCylEquAreaProjection.h>
#include <ossim/base/ossimKeywordNames.h>

RTTI_DEF1(ossimTransCylEquAreaProjection, "ossimTransCylEquAreaProjection", ossimMapProjection)
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

#define TCEA_Q(sinlat,x)                (One_MINUS_es2*(sinlat/(1.0-es2*sinlat*sinlat)-One_OVER_2es*log((1-x)/(1+x))))
#define TCEA_COEFF_TIMES_SIN(coeff,x,latit)         (coeff * sin(x*latit))
#define TCEA_M(c0lat,c1lat,c2lat,c3lat)             (Tcea_a * (c0lat - c1lat + c2lat - c3lat))
#define TCEA_L(Beta,c0lat,c1lat,c2lat)              (Beta + c0lat + c1lat + c2lat)
#define MIN_SCALE_FACTOR  0.3
#define MAX_SCALE_FACTOR  3.0

#define TCEA_NO_ERROR           0x0000
#define TCEA_LAT_ERROR          0x0001
#define TCEA_LON_ERROR          0x0002
#define TCEA_EASTING_ERROR      0x0004
#define TCEA_NORTHING_ERROR     0x0008
#define TCEA_ORIGIN_LAT_ERROR   0x0010
#define TCEA_CENT_MER_ERROR     0x0020
#define TCEA_A_ERROR            0x0040
#define TCEA_INV_F_ERROR        0x0080
#define TCEA_SCALE_FACTOR_ERROR 0x0100
#define TCEA_LON_WARNING        0x0200

ossimTransCylEquAreaProjection::ossimTransCylEquAreaProjection(const ossimEllipsoid& ellipsoid,
                                                               const ossimGpt& origin)
   :ossimMapProjection(ellipsoid, origin)
{
   setDefaults();
   update();
}

ossimTransCylEquAreaProjection::ossimTransCylEquAreaProjection(const ossimEllipsoid& ellipsoid,
                                                               const ossimGpt& origin,
                                                               double falseEasting,
                                                               double falseNorthing,
                                                               double scaleFactor)
   :ossimMapProjection(ellipsoid, origin)
{
   Tcea_False_Easting  = falseEasting;
   Tcea_False_Northing = falseNorthing;
   Tcea_Scale_Factor   = scaleFactor;
   Tcea_Min_Easting  = -6398628.0;
   Tcea_Max_Easting  = 6398628.0;
   Tcea_Min_Northing = -20003931.0;
   Tcea_Max_Northing = 20003931.0;

   update();
}

void ossimTransCylEquAreaProjection::setFalseEasting(double falseEasting)
{
   Tcea_False_Easting = falseEasting;
   
   update();
}


void ossimTransCylEquAreaProjection::setFalseNorthing(double falseNorthing)
{
   Tcea_False_Northing = falseNorthing;
   
   update();
}

void ossimTransCylEquAreaProjection::setFalseEastingNorthing(double falseEasting,
                                                      double falseNorthing)
{
   Tcea_False_Easting  = falseEasting;
   Tcea_False_Northing = falseNorthing;
   
   update();
}

void ossimTransCylEquAreaProjection::setScaleFactor(double scaleFactor)
{
   Tcea_Scale_Factor = scaleFactor;
   
   update();
}

void ossimTransCylEquAreaProjection::setParameters(double falseEasting,
                                                   double falseNorthing,
                                                   double scaleFactor)
{
   Tcea_False_Easting  = falseEasting;
   Tcea_False_Northing = falseNorthing;
   Tcea_Scale_Factor = scaleFactor;
   
   update();
}

void ossimTransCylEquAreaProjection::setDefaults()
{
   Tcea_Scale_Factor   = 1.0;
   Tcea_False_Easting  = 0.0;
   Tcea_False_Northing = 0.0;
   Tcea_Min_Easting    = -6398628.0;
   Tcea_Max_Easting    = 6398628.0;
   Tcea_Min_Northing   = -20003931.0;
   Tcea_Max_Northing   = 20003931.0;
}

void ossimTransCylEquAreaProjection::update()
{  
   Set_Trans_Cyl_Eq_Area_Parameters(theEllipsoid.getA(),
                                    theEllipsoid.getFlattening(),
                                    theOrigin.latr(),
                                    theOrigin.lonr(),
                                    Tcea_False_Easting,
                                    Tcea_False_Northing,
                                    Tcea_Scale_Factor);

   theFalseEastingNorthing.x = Tcea_False_Easting;
   theFalseEastingNorthing.y = Tcea_False_Northing;
   
   ossimMapProjection::update();
}
   

ossimGpt ossimTransCylEquAreaProjection::inverse(const ossimDpt &eastingNorthing)const
{
   double lat = 0.0;
   double lon = 0.0;
   
   Convert_Trans_Cyl_Eq_Area_To_Geodetic(eastingNorthing.x,
                                         eastingNorthing.y,
                                         &lat,
                                         &lon);
   
   return ossimGpt(lat*DEG_PER_RAD, lon*DEG_PER_RAD, 0.0, theDatum);  
}

ossimDpt ossimTransCylEquAreaProjection::forward(const ossimGpt &latLon)const
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

   Convert_Geodetic_To_Trans_Cyl_Eq_Area(gpt.latr(),
                                         gpt.lonr(),
                                         &easting,
                                         &northing);
   
   return ossimDpt(easting, northing);
}

bool ossimTransCylEquAreaProjection::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   kwl.add(prefix,
           ossimKeywordNames::SCALE_FACTOR_KW,
           Tcea_Scale_Factor,
           true);
   
   return ossimMapProjection::saveState(kwl, prefix);
}

bool ossimTransCylEquAreaProjection::loadState(const ossimKeywordlist& kwl,
                                               const char* prefix)
{
   bool flag = ossimMapProjection::loadState(kwl, prefix);
   const char* type          = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   const char* scaleFactor   = kwl.find(prefix, ossimKeywordNames::SCALE_FACTOR_KW);
   
   setDefaults();

   if(ossimString(type) == STATIC_TYPE_NAME(ossimTransCylEquAreaProjection))
   {
      Tcea_False_Easting  = theFalseEastingNorthing.x;
      Tcea_False_Northing = theFalseEastingNorthing.y;

      if(scaleFactor)
      {
         Tcea_Scale_Factor = ossimString(scaleFactor).toDouble();
      }
   }
   
   update();

   return flag;
}

long ossimTransCylEquAreaProjection::Set_Trans_Cyl_Eq_Area_Parameters(double a,
                                                                      double f,
                                                                      double Origin_Latitude,
                                                                      double Central_Meridian,
                                                                      double False_Easting,
                                                                      double False_Northing,
                                                                      double Scale_Factor)
{ /* BEGIN Set_Trans_Cyl_Eq_Area_Parameters */
/*
 * The function Set_Trans_Cyl_Eq_Area_Parameters receives the ellipsoid parameters and
 * Transverse Cylindrical Equal Area projection parameters as inputs, and sets the 
 * corresponding state variables.  If any errors occur, the error code(s) are returned 
 * by the function, otherwise TCEA_NO_ERROR is returned.
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
 *                          ellipsoid                               (input)
 */

  double sin_lat_90 = sin(PI_OVER_2);
  double x, j, three_es4;
  double Sqrt_One_MINUS_es2;
  double e1, e2, e3, e4;
  double lat, sin2lat, sin4lat, sin6lat;
  double temp, temp_northing;
//  double inv_f = 1 / f;
  long Error_Code = TCEA_NO_ERROR;

//   if (a <= 0.0)
//   { /* Semi-major axis must be greater than zero */
//     Error_Code |= TCEA_A_ERROR;
//   }
//   if ((inv_f < 250) || (inv_f > 350))
//   { /* Inverse flattening must be between 250 and 350 */
//     Error_Code |= TCEA_INV_F_ERROR;
//   }
//   if ((Origin_Latitude < -PI_OVER_2) || (Origin_Latitude > PI_OVER_2))
//   { /* origin latitude out of range */
//     Error_Code |= TCEA_ORIGIN_LAT_ERROR;
//   }
//   if ((Central_Meridian < -M_PI) || (Central_Meridian > TWO_PI))
//   { /* origin longitude out of range */
//     Error_Code |= TCEA_CENT_MER_ERROR;
//   }
//   if ((Scale_Factor < MIN_SCALE_FACTOR) || (Scale_Factor > MAX_SCALE_FACTOR))
//   {
//     Error_Code |= TCEA_SCALE_FACTOR_ERROR;
//   }

  if (!Error_Code)
  { /* no errors */
    Tcea_a = a;
    Tcea_f = f;
    Tcea_Origin_Lat = Origin_Latitude;
    if (Central_Meridian > M_PI)
      Central_Meridian -= TWO_PI;
    Tcea_Origin_Long = Central_Meridian;
    Tcea_False_Northing = False_Northing;
    Tcea_False_Easting = False_Easting;
    Tcea_Scale_Factor = Scale_Factor;

    es2 = 2 * Tcea_f - Tcea_f * Tcea_f;
    es = sqrt(es2);
    One_MINUS_es2 = 1.0 - es2;
    Sqrt_One_MINUS_es2 = sqrt(One_MINUS_es2);
    One_OVER_2es = 1.0 / (2.0 * es);
    es4 = es2 * es2;
    es6 = es4 * es2;
    x = es * sin_lat_90;
    qp = TCEA_Q(sin_lat_90,x);

    a0 = es2 / 3.0 + 31.0 * es4 / 180.0 + 517.0 * es6 / 5040.0;
    a1 = 23.0 * es4 / 360.0 + 251.0 * es6 / 3780.0;
    a2 = 761.0 * es6 / 45360.0;

    e1 = (1.0 - Sqrt_One_MINUS_es2) / (1.0 + Sqrt_One_MINUS_es2);
    e2 = e1 * e1;
    e3 = e2 * e1;
    e4 = e3 * e1;
    b0 = 3.0 * e1 / 2.0 - 27.0 * e3 / 32.0;
    b1 = 21.0 * e2 / 16.0 - 55.0 * e4 / 32.0;
    b2 = 151.0 * e3 / 96.0;
    b3 = 1097.0 * e4 / 512.0;

    j = 45.0 * es6 / 1024.0;
    three_es4 = 3.0 * es4;
    c0 = 1.0 - es2 / 4.0 - three_es4 / 64.0 - 5.0 * es6 / 256.0;
    c1 = 3.0 * es2 / 8.0 + three_es4 / 32.0 + j;
    c2 = 15.0 * es4 / 256.0 + j;
    c3 = 35.0 * es6 / 3072.0;
    lat = c0 * Tcea_Origin_Lat;
    sin2lat = TCEA_COEFF_TIMES_SIN(c1, 2.0, Tcea_Origin_Lat);
    sin4lat = TCEA_COEFF_TIMES_SIN(c2, 4.0, Tcea_Origin_Lat);
    sin6lat = TCEA_COEFF_TIMES_SIN(c3, 6.0, Tcea_Origin_Lat);
    M0 = TCEA_M(lat, sin2lat, sin4lat, sin6lat);
    Convert_Geodetic_To_Trans_Cyl_Eq_Area(PI_OVER_2, M_PI, &temp, &temp_northing);
    if (temp_northing > 0)
    {
      Tcea_Min_Northing = temp_northing - 20003931.458986;
      Tcea_Max_Northing = temp_northing;
    }
    else if (temp_northing < 0)
    {
      Tcea_Max_Northing = temp_northing + 20003931.458986;
      Tcea_Min_Northing = temp_northing;
    }
  } /* END OF if(!Error_Code) */
  return (Error_Code);
} /* END OF Set_Trans_Cyl_Eq_Area_Parameters */

void ossimTransCylEquAreaProjection::Get_Trans_Cyl_Eq_Area_Parameters(double *a,
                                                                      double *f,
                                                                      double *Origin_Latitude,
                                                                      double *Central_Meridian,
                                                                      double *False_Easting,
                                                                      double *False_Northing,
                                                                      double *Scale_Factor)const
{ /* BEGIN Get_Trans_Cyl_Eq_Area_Parameters */
/*
 * The function Get_Trans_Cyl_Eq_Area_Parameters returns the current ellipsoid
 * parameters, Transverse Cylindrical Equal Area projection parameters, and scale factor.
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

  *a = Tcea_a;
  *f = Tcea_f;
  *Origin_Latitude = Tcea_Origin_Lat;
  *Central_Meridian = Tcea_Origin_Long;
  *False_Easting = Tcea_False_Easting;
  *False_Northing = Tcea_False_Northing;
  *Scale_Factor = Tcea_Scale_Factor;
  
  return;
} /* END OF Get_Trans_Cyl_Eq_Area_Parameters */


long ossimTransCylEquAreaProjection::Convert_Geodetic_To_Trans_Cyl_Eq_Area (double Latitude,
                                                                            double Longitude,
                                                                            double *Easting,
                                                                            double *Northing)const
{ /* BEGIN Convert_Geodetic_To_Trans_Cyl_Eq_Area */
/*
 * The function Convert_Geodetic_To_Trans_Cyl_Eq_Area converts geodetic (latitude and
 * longitude) coordinates to Transverse Cylindrical Equal Area projection (easting and
 * northing) coordinates, according to the current ellipsoid and Transverse Cylindrical 
 * Equal Area projection parameters.  If any errors occur, the error code(s) are returned 
 * by the function, otherwise TCEA_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi) in radians           (input)
 *    Longitude         : Longitude (lambda) in radians       (input)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */

  double x;
  double dlam;                      /* Longitude - Central Meridan */
  double sin_lat = sin(Latitude);
  double qq, qq_OVER_qp;
  double beta, betac;
  double sin2betac, sin4betac, sin6betac;
  double PHIc;
  double phi, sin2phi, sin4phi, sin6phi;
  double sinPHIc;
  double Mc;
  long Error_Code = TCEA_NO_ERROR;

//   if ((Latitude < -PI_OVER_2) || (Latitude > PI_OVER_2))
//   { /* Latitude out of range */
//     Error_Code |= TCEA_LAT_ERROR;
//   }
//   if ((Longitude < -M_PI) || (Longitude > TWO_PI))
//   { /* Longitude out of range */
//     Error_Code |= TCEA_LON_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */
    dlam = Longitude - Tcea_Origin_Long;
    if (fabs(dlam) >= (M_PI / 2.0))
    { /* Distortion will result if Longitude is more than 90 degrees from the Central Meridian */
      Error_Code |= TCEA_LON_WARNING;
    }

    if (dlam > M_PI)
    {
      dlam -= TWO_PI;
    }
    if (dlam < -M_PI)
    {
      dlam += TWO_PI;
    }
    if (Latitude == PI_OVER_2)
    {
      qq = qp;
      qq_OVER_qp = 1.0;
    }
    else
    {
      x = es * sin_lat;
      qq = TCEA_Q(sin_lat, x);
      qq_OVER_qp = qq / qp;
    }


    if (qq_OVER_qp > 1.0)
      qq_OVER_qp = 1.0;
    else if (qq_OVER_qp < -1.0)
      qq_OVER_qp = -1.0;

    beta = asin(qq_OVER_qp);
    betac = atan(tan(beta) / cos(dlam));

    if ((fabs(betac) - PI_OVER_2) > 1.0e-8)
      PHIc = betac;
    else
    {
      sin2betac = TCEA_COEFF_TIMES_SIN(a0, 2.0, betac);
      sin4betac = TCEA_COEFF_TIMES_SIN(a1, 4.0, betac);
      sin6betac = TCEA_COEFF_TIMES_SIN(a2, 6.0, betac);
      PHIc = TCEA_L(betac, sin2betac, sin4betac, sin6betac);
    }

    sinPHIc = sin(PHIc);
    *Easting = Tcea_a * cos(beta) * cos(PHIc) * sin(dlam) /
               (Tcea_Scale_Factor * cos(betac) * sqrt(1.0 - es2 *
                                                      sinPHIc * sinPHIc)) + Tcea_False_Easting;

    phi = c0 * PHIc;
    sin2phi = TCEA_COEFF_TIMES_SIN(c1, 2.0, PHIc);
    sin4phi = TCEA_COEFF_TIMES_SIN(c2, 4.0, PHIc);
    sin6phi = TCEA_COEFF_TIMES_SIN(c3, 6.0, PHIc);
    Mc = TCEA_M(phi, sin2phi, sin4phi, sin6phi);

    *Northing = Tcea_Scale_Factor * (Mc - M0) + Tcea_False_Northing;
  }
  return (Error_Code);
} /* END OF Convert_Geodetic_To_Trans_Cyl_Eq_Area */


long ossimTransCylEquAreaProjection::Convert_Trans_Cyl_Eq_Area_To_Geodetic(double Easting,
                                                                           double Northing,
                                                                           double *Latitude,
                                                                           double *Longitude)const
{ /* BEGIN Convert_Trans_Cyl_Eq_Area_To_Geodetic */
/*
 * The function Convert_Trans_Cyl_Eq_Area_To_Geodetic converts Transverse
 * Cylindrical Equal Area projection (easting and northing) coordinates
 * to geodetic (latitude and longitude) coordinates, according to the
 * current ellipsoid and Transverse Cylindrical Equal Area projection
 * coordinates.  If any errors occur, the error code(s) are returned by the
 * function, otherwise TCEA_NO_ERROR is returned.
 *
 *    Easting           : Easting (X) in meters                  (input)
 *    Northing          : Northing (Y) in meters                 (input)
 *    Latitude          : Latitude (phi) in radians              (output)
 *    Longitude         : Longitude (lambda) in radians          (output)
 */

  double x;
  double dx;     /* Delta easting - Difference in easting (easting-FE)      */
  double dy;     /* Delta northing - Difference in northing (northing-FN)   */
  double Mc;
  double MUc;
  double sin2mu, sin4mu, sin6mu, sin8mu;
  double PHIc;
  double Qc;
  double sin_lat;
  double beta, betac, beta_prime;
  double sin2beta, sin4beta, sin6beta;
  double cosbetac;
  double Qc_OVER_qp;
  double temp;

  long Error_Code = TCEA_NO_ERROR;

//   if ((Easting < (Tcea_False_Easting + Tcea_Min_Easting))
//       || (Easting > (Tcea_False_Easting + Tcea_Max_Easting)))
//   { /* Easting out of range */
//     Error_Code |= TCEA_EASTING_ERROR;
//   }
//   if ((Northing < (Tcea_False_Northing + Tcea_Min_Northing))
//       || (Northing > (Tcea_False_Northing + Tcea_Max_Northing)))
//   { /* Northing out of range */
//     Error_Code |= TCEA_NORTHING_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */
    dy = Northing - Tcea_False_Northing;
    dx = Easting - Tcea_False_Easting;
    Mc = M0 + dy / Tcea_Scale_Factor;
    MUc = Mc / (Tcea_a * c0);

    sin2mu = TCEA_COEFF_TIMES_SIN(b0, 2.0, MUc);
    sin4mu = TCEA_COEFF_TIMES_SIN(b1, 4.0, MUc);
    sin6mu = TCEA_COEFF_TIMES_SIN(b2, 6.0, MUc);
    sin8mu = TCEA_COEFF_TIMES_SIN(b3, 8.0, MUc);
    PHIc = MUc + sin2mu + sin4mu + sin6mu + sin8mu;

    sin_lat = sin(PHIc);
    x = es * sin_lat;
    Qc = TCEA_Q(sin_lat, x);
    Qc_OVER_qp = Qc / qp;

    if (Qc_OVER_qp < -1.0)
      Qc_OVER_qp = -1.0;
    else if (Qc_OVER_qp > 1.0)
      Qc_OVER_qp = 1.0;

    betac = asin(Qc_OVER_qp);
    cosbetac = cos(betac);
    temp = Tcea_Scale_Factor * dx * cosbetac * sqrt(1.0 -
                                                    es2 * sin_lat * sin_lat) / (Tcea_a * cos(PHIc));
    if (temp > 1.0)
      temp = 1.0;
    else if (temp < -1.0)
      temp = -1.0;
    beta_prime = -asin(temp);
    beta = asin(cos(beta_prime) * sin(betac));

    sin2beta = TCEA_COEFF_TIMES_SIN(a0, 2.0, beta);
    sin4beta = TCEA_COEFF_TIMES_SIN(a1, 4.0, beta);
    sin6beta = TCEA_COEFF_TIMES_SIN(a2, 6.0, beta);
    *Latitude = TCEA_L(beta, sin2beta, sin4beta, sin6beta);

    *Longitude = Tcea_Origin_Long - atan(tan(beta_prime) / cosbetac);

//     if (*Latitude > PI_OVER_2)  /* force distorted values to 90, -90 degrees */
//       *Latitude = PI_OVER_2;
//     else if (*Latitude < -PI_OVER_2)
//       *Latitude = -PI_OVER_2;

//     if (*Longitude > M_PI)
//       *Longitude -= TWO_PI;
//     if (*Longitude < -M_PI)
//       *Longitude += TWO_PI;

//     if (*Longitude > M_PI) /* force distorted values to 180, -180 degrees */
//       *Longitude = M_PI;
//     else if (*Longitude < -M_PI)
//       *Longitude = -M_PI;
  }
  return (Error_Code);
} /* END OF Convert_Trans_Cyl_Eq_Area_To_Geodetic */

//*************************************************************************************************
//! Returns TRUE if principal parameters are within epsilon tolerance.
//*************************************************************************************************
bool ossimTransCylEquAreaProjection::operator==(const ossimProjection& proj) const
{
   if (!ossimMapProjection::operator==(proj))
      return false;

   ossimTransCylEquAreaProjection* p = PTR_CAST(ossimTransCylEquAreaProjection, &proj);
   if (!p) return false;

   if (!ossim::almostEqual(Tcea_Scale_Factor,p->Tcea_Scale_Factor)) return false;

   return true;
}
