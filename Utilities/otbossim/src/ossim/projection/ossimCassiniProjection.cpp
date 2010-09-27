//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Calls Geotrans Cassini projection code.  
//*******************************************************************
//  $Id: ossimCassiniProjection.cpp 17815 2010-08-03 13:23:14Z dburken $

#include <math.h>
#include <ossim/projection/ossimCassiniProjection.h>
#include <ossim/base/ossimKeywordNames.h>

RTTI_DEF1(ossimCassiniProjection, "ossimCassiniProjection", ossimMapProjection)



/***************************************************************************/
/*
 *                               DEFINES
 */

#ifdef PI_OVER_2
#  undef PI_OVER_2
#endif

#define CASS_NO_ERROR           0x0000
#define CASS_LAT_ERROR          0x0001
#define CASS_LON_ERROR          0x0002
#define CASS_EASTING_ERROR      0x0004
#define CASS_NORTHING_ERROR     0x0008
#define CASS_ORIGIN_LAT_ERROR   0x0010
#define CASS_CENT_MER_ERROR     0x0020
#define CASS_A_ERROR            0x0040
#define CASS_B_ERROR            0x0080
#define CASS_A_LESS_B_ERROR     0x0100
#define CASS_LON_WARNING        0x0200

#define PI_OVER_2  ( M_PI / 2.0)                    
#define CASS_M(c0lat, c1s2lat, c2s4lat, c3s6lat)  (Cass_a*(c0lat-c1s2lat+c2s4lat-c3s6lat))
#define CASS_RD(sinlat)                           (sqrt(1.0 - es2 * (sinlat * sinlat)))
#define CASS_COEFF_TIMES_SIN(coeff, x, latit)	  (coeff * (sin (x * latit)))
#define FLOAT_EQ(x,v,epsilon)   (((v - epsilon) < x) && (x < (v + epsilon)))
#define THIRTY_ONE  (31.0 * M_PI / 180.0)	  /* 31 degrees in radians */



ossimCassiniProjection::ossimCassiniProjection(const ossimEllipsoid& ellipsoid,
                                               const ossimGpt& origin)
   :ossimMapProjection(ellipsoid, origin)
{
   setDefaults();
   update();
}

ossimCassiniProjection::ossimCassiniProjection(const ossimEllipsoid& ellipsoid,
                                               const ossimGpt& origin,
                                               const double falseEasting,
                                               const double falseNorthing)
   :ossimMapProjection(ellipsoid, origin)
{
   Cass_False_Easting  = falseEasting;
   Cass_False_Northing = falseNorthing;
   Cass_Min_Easting    = -20037508.4;
   Cass_Max_Easting    = 20037508.4;
   Cass_Min_Northing   = -56575846.0;
   Cass_Max_Northing   = 56575846.0;
   
   update();
}

void ossimCassiniProjection::update()
{      
   Set_Cassini_Parameters(theEllipsoid.getA(),
                          theEllipsoid.getFlattening(),
                          theOrigin.latr(),
                          theOrigin.lonr(),
                          Cass_False_Easting,
                          Cass_False_Northing);

   theFalseEastingNorthing.x = Cass_False_Easting;
   theFalseEastingNorthing.y = Cass_False_Northing;


   ossimMapProjection::update();
}

void ossimCassiniProjection::setFalseEasting(double falseEasting)
{
   Cass_False_Easting = falseEasting;
   update();
}

void ossimCassiniProjection::setFalseNorthing(double falseNorthing)
{
   Cass_False_Northing = falseNorthing;
   update();
}

void ossimCassiniProjection::setFalseEastingNorthing(double falseEasting,
                                                   double falseNorthing)
{
   Cass_False_Easting  = falseEasting;
   Cass_False_Northing = falseNorthing;  
   update();
}

void ossimCassiniProjection::setDefaults()
{

   Cass_Min_Easting    = -20037508.4;
   Cass_Max_Easting    = 20037508.4;
   Cass_Min_Northing   = -56575846.0;
   Cass_Max_Northing   = 56575846.0;
   Cass_False_Easting  = 0.0;
   Cass_False_Northing = 0.0;
}

ossimGpt ossimCassiniProjection::inverse(const ossimDpt &eastingNorthing)const
{
   double lat = 0.0;
   double lon = 0.0;
   
   Convert_Cassini_To_Geodetic(eastingNorthing.x,
                               eastingNorthing.y,
                               &lat,
                               &lon);
   
   return ossimGpt(lat*DEG_PER_RAD, lon*DEG_PER_RAD, 0, theDatum);  
}

ossimDpt ossimCassiniProjection::forward(const ossimGpt &latLon)const
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

   Convert_Geodetic_To_Cassini(gpt.latr(),
                               gpt.lonr(),
                               &easting,
                               &northing);
   return ossimDpt(easting, northing);

}

bool ossimCassiniProjection::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   return ossimMapProjection::saveState(kwl, prefix);
}

bool ossimCassiniProjection::loadState(const ossimKeywordlist& kwl,
                                       const char* prefix)
{
   // Must do this first.
   bool flag = ossimMapProjection::loadState(kwl, prefix);
   
   const char* type          = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   
   setDefaults();
   
   if(ossimString(type) == STATIC_TYPE_NAME(ossimCassiniProjection))
   {
      Cass_False_Easting  = theFalseEastingNorthing.x;
      Cass_False_Northing = theFalseEastingNorthing.y;
   }
   
   update();
   
   return flag;
}

/***************************************************************************/
/*
 *                              FUNCTIONS     
 */


long ossimCassiniProjection::Set_Cassini_Parameters(double a,
                                                    double f,
                                                    double Origin_Latitude,
                                                    double Central_Meridian,
                                                    double False_Easting,
                                                    double False_Northing)
{ /* Begin Set_Cassini_Parameters */
/*
 * The function Set_Cassini_Parameters receives the ellipsoid parameters and
 * Cassini projection parameters as inputs, and sets the corresponding state
 * variables.  If any errors occur, the error code(s) are returned by the 
 * function, otherwise CASS_NO_ERROR is returned.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters   (input)
 *    f                 : Flattening of ellipsoid                   (input)
 *    Origin_Latitude   : Latitude in radians at which the          (input)
 *                          point scale factor is 1.0
 *    Central_Meridian  : Longitude in radians at the center of     (input)
 *                          the projection
 *    False_Easting     : A coordinate value in meters assigned to the
 *                          central meridian of the projection.     (input)
 *    False_Northing    : A coordinate value in meters assigned to the
 *                          origin latitude of the projection       (input)
 */

  double j,three_es4;
  double x, e1, e2, e3, e4;
  double lat, sin2lat, sin4lat, sin6lat;
  double temp;
//  double inv_f = 1 / f;
  long Error_Code = CASS_NO_ERROR;

//   if (a <= 0.0)
//   { /* Semi-major axis must be greater than zero */
//     Error_Code |= CASS_A_ERROR;
//   }
//   if ((inv_f < 250) || (inv_f > 350))
//   { /* Inverse flattening must be between 250 and 350 */
//     Error_Code |= CASS_INV_F_ERROR;
//   }
//   if ((Origin_Latitude < -PI_OVER_2) || (Origin_Latitude > PI_OVER_2))
//   { /* origin latitude out of range */
//     Error_Code |= CASS_ORIGIN_LAT_ERROR;
//   }
//   if ((Central_Meridian < -PI) || (Central_Meridian > TWO_PI))
//   { /* origin longitude out of range */
//     Error_Code |= CASS_CENT_MER_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */
    Cass_a = a;
    Cass_f = f;
    Cass_Origin_Lat = Origin_Latitude;
//     if (Central_Meridian > PI)
//       Central_Meridian -= TWO_PI;
    Cass_Origin_Long = Central_Meridian;
    Cass_False_Northing = False_Northing;
    Cass_False_Easting = False_Easting;
    es2 = 2 * Cass_f - Cass_f * Cass_f;
    es4 = es2 * es2;
    es6 = es4 * es2;
    j = 45.0 * es6 / 1024.0;
    three_es4 = 3.0 * es4;
    c0 = 1 - es2 / 4.0 - three_es4 / 64.0 - 5.0 * es6 / 256.0; 
    c1 = 3.0 * es2 /8.0 + three_es4 / 32.0 + j;
    c2 = 15.0 * es4 / 256.0 + j;
    c3 = 35.0 * es6 / 3072.0;
    lat = c0 * Cass_Origin_Lat;
    sin2lat = CASS_COEFF_TIMES_SIN(c1, 2.0, Cass_Origin_Lat);
    sin4lat = CASS_COEFF_TIMES_SIN(c2, 4.0, Cass_Origin_Lat);
    sin6lat = CASS_COEFF_TIMES_SIN(c3, 6.0, Cass_Origin_Lat);
    M0 = CASS_M(lat, sin2lat, sin4lat, sin6lat);

    One_Minus_es2 = 1.0 - es2;
    x = sqrt (One_Minus_es2);
    e1 = (1 - x) / (1 + x);
    e2 = e1 * e1;
    e3 = e2 * e1;
    e4 = e3 * e1;
    a0 = 3.0 * e1 / 2.0 - 27.0 * e3 / 32.0;
    a1 = 21.0 * e2 / 16.0 - 55.0 * e4 / 32.0;
    a2 = 151.0 * e3 / 96.0;
    a3 = 1097.0 * e4 /512.0;

    if (Cass_Origin_Long > 0)
    {
      Convert_Geodetic_To_Cassini(THIRTY_ONE, Cass_Origin_Long - M_PI, &temp, &Cass_Max_Northing);
      Convert_Geodetic_To_Cassini(-THIRTY_ONE, Cass_Origin_Long - M_PI, &temp, &Cass_Min_Northing); 
      Cass_Max_Easting = 19926188.9;
      Cass_Min_Easting = -20037508.4;
    }
    else if (Cass_Origin_Long < 0)
    {
      Convert_Geodetic_To_Cassini(THIRTY_ONE, M_PI + Cass_Origin_Long, &temp, &Cass_Max_Northing); 
      Convert_Geodetic_To_Cassini(-THIRTY_ONE, M_PI + Cass_Origin_Long, &temp, &Cass_Min_Northing);
      Cass_Max_Easting = 20037508.4;
      Cass_Min_Easting = -19926188.9;
    }
    else
    {
      Convert_Geodetic_To_Cassini(THIRTY_ONE, M_PI, &temp, &Cass_Max_Northing); 
      Convert_Geodetic_To_Cassini(-THIRTY_ONE, M_PI , &temp, &Cass_Min_Northing);
      Cass_Max_Easting = 20037508.4;
      Cass_Min_Easting = -20037508.4;
    }

  } /* End if(!Error_Code) */
  return (Error_Code);
} /* End Set_Cassini_Parameters */


void ossimCassiniProjection::Get_Cassini_Parameters(double *a,
                                                    double *f,
                                                    double *Origin_Latitude,
                                                    double *Central_Meridian,
                                                    double *False_Easting,
                                                    double *False_Northing)const
{ /* Begin Get_Cassini_Parameters */
/*
 * The function Get_Cassini_Parameters returns the current ellipsoid
 * parameters, Cassini projection parameters.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters   (output)
 *    f                 : Flattening of ellipsoid                   (output)
 *    Origin_Latitude   : Latitude in radians at which the          (output)
 *                          point scale factor is 1.0
 *    Central_Meridian  : Longitude in radians at the center of     (output)
 *                          the projection
 *    False_Easting     : A coordinate value in meters assigned to the
 *                          central meridian of the projection.     (output)
 *    False_Northing    : A coordinate value in meters assigned to the
 *                          origin latitude of the projection       (output)
 */

  *a = Cass_a;
  *f = Cass_f;
  *Origin_Latitude = Cass_Origin_Lat;
  *Central_Meridian = Cass_Origin_Long;
  *False_Easting = Cass_False_Easting;
  *False_Northing = Cass_False_Northing;
  
  return;
} /* End Get_Cassini_Parameters */


long ossimCassiniProjection::Convert_Geodetic_To_Cassini (double Latitude,
                                                          double Longitude,
                                                          double *Easting,
                                                          double *Northing)const
{ /* Begin Convert_Geodetic_To_Cassini */
/*
 * The function Convert_Geodetic_To_Cassini converts geodetic (latitude and
 * longitude) coordinates to Cassini projection (easting and northing)
 * coordinates, according to the current ellipsoid and Cassini projection
 * parameters.  If any errors occur, the error code(s) are returned by the
 * function, otherwise CASS_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi) in radians           (input)
 *    Longitude         : Longitude (lambda) in radians       (input)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */

  double lat, sin2lat, sin4lat, sin6lat;
  double RD;
  double tlat = tan(Latitude);
  double clat = cos(Latitude);
  double slat = sin(Latitude);
  double dlam;                      /* Longitude - Central Meridan */
  double NN;
  double TT;
  double AA, A2, A3, A4, A5;
  double CC;
  double MM;
  long Error_Code = CASS_NO_ERROR;

//   if ((Latitude < -PI_OVER_2) || (Latitude > PI_OVER_2))
//   { /* Latitude out of range */
//     Error_Code |= CASS_LAT_ERROR;
//   }
//   if ((Longitude < -PI) || (Longitude > TWO_PI))
//   { /* Longitude out of range */
//     Error_Code |= CASS_LON_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */
    dlam = Longitude - Cass_Origin_Long;

    if (fabs(dlam) > (4.0 * M_PI / 180.0))
    { /* Distortion will result if Longitude is more than 4 degrees from the Central Meridian */
      Error_Code |= CASS_LON_WARNING;
    }

    if (dlam > M_PI)
    {
      dlam -= TWO_PI;
    }
    if (dlam < -M_PI)
    {
      dlam += TWO_PI;
    }
    RD = CASS_RD(slat);
    NN = Cass_a / RD;
    TT = tlat * tlat;
    AA = dlam * clat;
    A2 = AA * AA;
    A3 = AA * A2;
    A4 = AA * A3;
    A5 = AA * A4;
    CC = es2 * clat * clat / One_Minus_es2;
    lat = c0 * Latitude;
    sin2lat = CASS_COEFF_TIMES_SIN(c1, 2.0, Latitude);
    sin4lat = CASS_COEFF_TIMES_SIN(c2, 4.0, Latitude);
    sin6lat = CASS_COEFF_TIMES_SIN(c3, 6.0, Latitude);
    MM = CASS_M(lat, sin2lat, sin4lat, sin6lat);

    *Easting = NN * (AA - (TT * A3 / 6.0) - (8.0 - TT + 8.0 * CC) *
                     (TT * A5 / 120.0)) + Cass_False_Easting;
    *Northing = MM - M0 + NN * tlat * ((A2 / 2.0) + (5.0 - TT +
                                                     6.0 * CC) * A4 / 24.0) + Cass_False_Northing;
  }
  return (Error_Code);
} /* End Convert_Geodetic_To_Cassini */


long ossimCassiniProjection::Convert_Cassini_To_Geodetic(double Easting,
                                                         double Northing,
                                                         double *Latitude,
                                                         double *Longitude)const
{ /* Begin Convert_Cassini_To_Geodetic */
/*
 * The function Convert_Cassini_To_Geodetic converts Cassini projection
 * (easting and northing) coordinates to geodetic (latitude and longitude)
 * coordinates, according to the current ellipsoid and Cassini projection
 * coordinates.  If any errors occur, the error code(s) are returned by the
 * function, otherwise CASS_NO_ERROR is returned.
 *
 *    Easting           : Easting (X) in meters                  (input)
 *    Northing          : Northing (Y) in meters                 (input)
 *    Latitude          : Latitude (phi) in radians              (output)
 *    Longitude         : Longitude (lambda) in radians          (output)
 */

  double dx;     /* Delta easting - Difference in easting (easting-FE)      */
  double dy;     /* Delta northing - Difference in northing (northing-FN)   */
  double mu1;
  double sin2mu, sin4mu, sin6mu, sin8mu;
  double M1;
  double phi1;
  double tanphi1, sinphi1, cosphi1;
  double T1, T;
  double N1;
  double RD, R1;
  double DD, D2, D3, D4, D5;
//  const double epsilon = 1.0e-1;
  long Error_Code = CASS_NO_ERROR;

//   if ((Easting < (Cass_False_Easting + Cass_Min_Easting))
//       || (Easting > (Cass_False_Easting + Cass_Max_Easting)))
//   { /* Easting out of range */
//     Error_Code |= CASS_EASTING_ERROR;
//   }
//   if ((Northing < (Cass_False_Northing + Cass_Min_Northing - epsilon))
//       || (Northing > (Cass_False_Northing + Cass_Max_Northing + epsilon)))
//   { /* Northing out of range */
//     Error_Code |= CASS_NORTHING_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */
    dy = Northing - Cass_False_Northing;
    dx = Easting - Cass_False_Easting;
    M1 = M0 + dy;
    mu1 = M1 / (Cass_a * c0); 

    sin2mu = CASS_COEFF_TIMES_SIN(a0, 2.0, mu1);
    sin4mu = CASS_COEFF_TIMES_SIN(a1, 4.0, mu1);
    sin6mu = CASS_COEFF_TIMES_SIN(a2, 6.0, mu1);
    sin8mu = CASS_COEFF_TIMES_SIN(a3, 8.0, mu1);
    phi1 = mu1 + sin2mu + sin4mu + sin6mu + sin8mu;

    if (FLOAT_EQ(phi1,PI_OVER_2,.00001))
    {
      *Latitude =  PI_OVER_2;
      *Longitude = Cass_Origin_Long;
    }
    else if (FLOAT_EQ(phi1,-PI_OVER_2,.00001))
    {
      *Latitude = -PI_OVER_2;
      *Longitude = Cass_Origin_Long;
    }
    else
    {
      tanphi1 = tan(phi1);
      sinphi1 = sin(phi1);
      cosphi1 = cos(phi1);
      T1 = tanphi1 * tanphi1;
      RD = CASS_RD(sinphi1);
      N1 = Cass_a / RD;
      R1 = N1 * One_Minus_es2 / (RD * RD);
      DD = dx / N1;
      D2 = DD * DD;
      D3 = D2 * DD;
      D4 = D3 * DD;
      D5 = D4 * DD;
      T = (1.0 + 3.0 * T1);
      *Latitude = phi1 - (N1 * tanphi1 / R1) * (D2 / 2.0 - T * D4 / 24.0);

      *Longitude = Cass_Origin_Long + (DD - T1 * D3 / 3.0 + T * T1 * D5 / 15.0) / cosphi1;

      if (*Latitude > PI_OVER_2)  /* force distorted values to 90, -90 degrees */
        *Latitude = PI_OVER_2;
      else if (*Latitude < -PI_OVER_2)
        *Latitude = -PI_OVER_2;

//       if (*Longitude > PI)
//         *Longitude -= TWO_PI;
//       if (*Longitude < -PI)
//         *Longitude += TWO_PI;

      if (*Longitude > M_PI)  /* force distorted values to 180, -180 degrees */
        *Longitude = M_PI;
      else if (*Longitude < -M_PI)
        *Longitude = -M_PI;
    }
    if (fabs(*Longitude - Cass_Origin_Long) > (4.0 * M_PI / 180.0))
    { /* Distortion will result if Longitude is more than 4 degrees from the Central Meridian */
      Error_Code |= CASS_LON_WARNING;
    }
  }
  return (Error_Code);
} /* End Convert_Cassini_To_Geodetic */

