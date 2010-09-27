//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Calls Geotrans Polar Stereographic  projection code.  
//*******************************************************************
//  $Id: ossimPolarStereoProjection.cpp 17815 2010-08-03 13:23:14Z dburken $

#include <math.h>
#include <ossim/projection/ossimPolarStereoProjection.h>
#include <ossim/base/ossimKeywordNames.h>

RTTI_DEF1(ossimPolarStereoProjection, "ossimPolarStereoProjection", ossimMapProjection)
/************************************************************************/
/*                               DEFINES
 *
 */

#ifndef PI_OVER_2
#  define PI_OVER_2  ( M_PI / 2.0)
#endif
#ifndef TWO_PI
#  define TWO_PI     (2.0 * M_PI)
#endif
#define POLAR_POW(EsSin)     pow((1.0 - EsSin) / (1.0 + EsSin), es_OVER_2)

#define POLAR_NO_ERROR                0x0000
#define POLAR_LAT_ERROR               0x0001
#define POLAR_LON_ERROR               0x0002
#define POLAR_ORIGIN_LAT_ERROR        0x0004
#define POLAR_ORIGIN_LON_ERROR        0x0008
#define POLAR_EASTING_ERROR	      0x0010
#define POLAR_NORTHING_ERROR	      0x0020
#define POLAR_A_ERROR                 0x0040
#define POLAR_B_ERROR                 0x0080
#define POLAR_A_LESS_B_ERROR          0x0100
#define POLAR_RADIUS_ERROR            0x0200

/************************************************************************/
/*                           GLOBAL DECLARATIONS
 *
 */

const double PI_Over_4 = (M_PI / 4.0);

ossimPolarStereoProjection::ossimPolarStereoProjection(const ossimEllipsoid& ellipsoid,
                                                       const ossimGpt& origin)
   :ossimMapProjection(ellipsoid, origin)
{
   setDefaults();
   update();
//    Polar_Delta_Easting  = 12713601.0;
//    Polar_Delta_Northing = 12713601.0;
}

ossimPolarStereoProjection::ossimPolarStereoProjection(const ossimEllipsoid& /* ellipsoid */,
                                                       const ossimGpt& /* origin */,
                                                       double falseEasting,
                                                       double falseNorthing)
{
   Polar_False_Easting  = falseEasting;
   Polar_False_Northing = falseNorthing;
   Polar_Delta_Easting  = 12713601.0;
   Polar_Delta_Northing = 12713601.0;

   update();
}

void ossimPolarStereoProjection::update()
{   
   Set_Polar_Stereographic_Parameters(theEllipsoid.getA(),
                                      theEllipsoid.getFlattening(),
                                      theOrigin.latr(),
                                      theOrigin.lonr(),
                                      Polar_False_Easting,
                                      Polar_False_Northing);

   theFalseEastingNorthing.x = Polar_False_Easting;
   theFalseEastingNorthing.y = Polar_False_Northing;

   ossimMapProjection::update();
}

void ossimPolarStereoProjection::setFalseEasting(double falseEasting)
{
   Polar_False_Easting = falseEasting;
   
   update();
}

void ossimPolarStereoProjection::setFalseNorthing(double falseNorthing)
{
   Polar_False_Northing = falseNorthing;
   
   update();
}

void ossimPolarStereoProjection::setDefaults()
{
   Polar_False_Easting  = 0.0;
   Polar_False_Northing = 0.0;
}

void ossimPolarStereoProjection::setFalseEastingNorthing(double falseEasting,
                                                      double falseNorthing)
{
   Polar_False_Easting  = falseEasting;
   Polar_False_Northing = falseNorthing;
   
   update();
}

ossimGpt ossimPolarStereoProjection::inverse(const ossimDpt &eastingNorthing)const
{
   double lat = 0.0;
   double lon = 0.0;
   
   Convert_Polar_Stereographic_To_Geodetic(eastingNorthing.x,
                                           eastingNorthing.y,
                                           &lat,
                                           &lon);
   
   return ossimGpt(lat*DEG_PER_RAD, lon*DEG_PER_RAD, 0.0, theDatum);
}

ossimDpt ossimPolarStereoProjection::forward(const ossimGpt &latLon)const
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

   Convert_Geodetic_To_Polar_Stereographic(gpt.latr(),
                                           gpt.lonr(),
                                           &easting,
                                           &northing);
   
   return ossimDpt(easting, northing);
}

bool ossimPolarStereoProjection::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   return ossimMapProjection::saveState(kwl, prefix);
}

bool ossimPolarStereoProjection::loadState(const ossimKeywordlist& kwl,
                                           const char* prefix)
{
   bool flag = ossimMapProjection::loadState(kwl, prefix);
   const char* type          = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   
   setDefaults();

   if(ossimString(type) == STATIC_TYPE_NAME(ossimPolarStereoProjection))
   {
      Polar_False_Easting  = theFalseEastingNorthing.x;
      Polar_False_Northing = theFalseEastingNorthing.y;
   }
   
   update();

   return flag;
}


/************************************************************************/
/*                              FUNCTIONS
 *
 */


long ossimPolarStereoProjection::Set_Polar_Stereographic_Parameters (double a,
                                                                     double f,
                                                                     double Latitude_of_True_Scale,
                                                                     double Longitude_Down_from_Pole,
                                                                     double False_Easting,
                                                                     double False_Northing)

{  /* BEGIN Set_Polar_Stereographic_Parameters   */
/*  
 *  The function Set_Polar_Stereographic_Parameters receives the ellipsoid
 *  parameters and Polar Stereograpic projection parameters as inputs, and
 *  sets the corresponding state variables.  If any errors occur, error
 *  code(s) are returned by the function, otherwise POLAR_NO_ERROR is returned.
 *
 *  a                : Semi-major axis of ellipsoid, in meters         (input)
 *  f                : Flattening of ellipsoid					               (input)
 *  Latitude_of_True_Scale  : Latitude of true scale, in radians       (input)
 *  Longitude_Down_from_Pole : Longitude down from pole, in radians    (input)
 *  False_Easting    : Easting (X) at center of projection, in meters  (input)
 *  False_Northing   : Northing (Y) at center of projection, in meters (input)
 */

  double es2;
  double slat, clat;
  double essin;
  double one_PLUS_es, one_MINUS_es;
  double pow_es;
  double temp;
//  double inv_f = 1 / f;
  const double  epsilon = 1.0e-2;
  long Error_Code = POLAR_NO_ERROR;

//   if (a <= 0.0)
//   { /* Semi-major axis must be greater than zero */
//     Error_Code |= POLAR_A_ERROR;
//   }
//   if ((inv_f < 250) || (inv_f > 350))
//   { /* Inverse flattening must be between 250 and 350 */
//     Error_Code |= POLAR_INV_F_ERROR;
//   }
//   if ((Latitude_of_True_Scale < -PI_OVER_2) || (Latitude_of_True_Scale > PI_OVER_2))
//   { /* Origin Latitude out of range */
//     Error_Code |= POLAR_ORIGIN_LAT_ERROR;
//   }
//   if ((Longitude_Down_from_Pole < -PI) || (Longitude_Down_from_Pole > TWO_PI))
//   { /* Origin Longitude out of range */
//     Error_Code |= POLAR_ORIGIN_LON_ERROR;
//   }

  if (!Error_Code)
  { /* no errors */

    Polar_a = a;
    two_Polar_a = 2.0 * Polar_a;
    Polar_f = f;

    if (Longitude_Down_from_Pole > M_PI)
      Longitude_Down_from_Pole -= TWO_PI;
    if (Latitude_of_True_Scale < 0)
    {
      Southern_Hemisphere = 1;
      Polar_Origin_Lat = -Latitude_of_True_Scale;
      Polar_Origin_Long = -Longitude_Down_from_Pole;
    }
    else
    {
      Southern_Hemisphere = 0;
      Polar_Origin_Lat = Latitude_of_True_Scale;
      Polar_Origin_Long = Longitude_Down_from_Pole;
    }
    Polar_False_Easting = False_Easting;
    Polar_False_Northing = False_Northing;

    es2 = 2 * Polar_f - Polar_f * Polar_f;
    es = sqrt(es2);
    es_OVER_2 = es / 2.0;

    if (fabs(fabs(Polar_Origin_Lat) - PI_OVER_2) > 1.0e-10)
    {
      slat = sin(Polar_Origin_Lat);
      essin = es * slat;
      pow_es = POLAR_POW(essin);
      clat = cos(Polar_Origin_Lat);
      mc = clat / sqrt(1.0 - essin * essin);
      Polar_a_mc = Polar_a * mc;
      tc = tan(PI_Over_4 - Polar_Origin_Lat / 2.0) / pow_es;
    }
    else
    {
      one_PLUS_es = 1.0 + es;
      one_MINUS_es = 1.0 - es;
      e4 = sqrt(pow(one_PLUS_es, one_PLUS_es) * pow(one_MINUS_es, one_MINUS_es));
    }
  }
  /* Calculate Radius */
  Convert_Geodetic_To_Polar_Stereographic(0, Polar_Origin_Long, 
                                          &temp, &Polar_Delta_Northing);
  Polar_Delta_Northing = fabs(Polar_Delta_Northing) + epsilon;
  Polar_Delta_Easting = Polar_Delta_Northing;


  return (Error_Code);
} /* END OF Set_Polar_Stereographic_Parameters */



void ossimPolarStereoProjection::Get_Polar_Stereographic_Parameters (double *a,
                                                                     double *f,
                                                                     double *Latitude_of_True_Scale,
                                                                     double *Longitude_Down_from_Pole,
                                                                     double *False_Easting,
                                                                     double *False_Northing)const

{ /* BEGIN Get_Polar_Stereographic_Parameters  */
/*
 * The function Get_Polar_Stereographic_Parameters returns the current
 * ellipsoid parameters and Polar projection parameters.
 *
 *  a                : Semi-major axis of ellipsoid, in meters         (output)
 *  f                : Flattening of ellipsoid					               (output)
 *  Latitude_of_True_Scale  : Latitude of true scale, in radians       (output)
 *  Longitude_Down_from_Pole : Longitude down from pole, in radians    (output)
 *  False_Easting    : Easting (X) at center of projection, in meters  (output)
 *  False_Northing   : Northing (Y) at center of projection, in meters (output)
 */

  *a = Polar_a;
  *f = Polar_f;
  *Latitude_of_True_Scale = Polar_Origin_Lat;
  *Longitude_Down_from_Pole = Polar_Origin_Long;
  *False_Easting = Polar_False_Easting;
  *False_Northing = Polar_False_Northing;
  
  return;
} /* END OF Get_Polar_Stereographic_Parameters */


long ossimPolarStereoProjection::Convert_Geodetic_To_Polar_Stereographic (double Latitude,
                                                                          double Longitude,
                                                                          double *Easting,
                                                                          double *Northing)const

{  /* BEGIN Convert_Geodetic_To_Polar_Stereographic */

/*
 * The function Convert_Geodetic_To_Polar_Stereographic converts geodetic
 * coordinates (latitude and longitude) to Polar Stereographic coordinates
 * (easting and northing), according to the current ellipsoid
 * and Polar Stereographic projection parameters. If any errors occur, error
 * code(s) are returned by the function, otherwise POLAR_NO_ERROR is returned.
 *
 *    Latitude   :  Latitude, in radians                      (input)
 *    Longitude  :  Longitude, in radians                     (input)
 *    Easting    :  Easting (X), in meters                    (output)
 *    Northing   :  Northing (Y), in meters                   (output)
 */

  double dlam;
  double slat;
  double essin;
  double t;
  double rho;
  double pow_es;
  long Error_Code = POLAR_NO_ERROR;

//   if ((Latitude < -PI_OVER_2) || (Latitude > PI_OVER_2))
//   {   /* Latitude out of range */
//     Error_Code |= POLAR_LAT_ERROR;
//   }
//   if ((Latitude < 0) && (Southern_Hemisphere == 0))
//   {   /* Latitude and Origin Latitude in different hemispheres */
//     Error_Code |= POLAR_LAT_ERROR;
//   }
//   if ((Latitude > 0) && (Southern_Hemisphere == 1))
//   {   /* Latitude and Origin Latitude in different hemispheres */
//     Error_Code |= POLAR_LAT_ERROR;
//   }
//   if ((Longitude < -PI) || (Longitude > TWO_PI))
//   {  /* Longitude out of range */
//     Error_Code |= POLAR_LON_ERROR;
//   }


  if (!Error_Code)
  {  /* no errors */

    if (fabs(fabs(Latitude) - PI_OVER_2) < 1.0e-10)
    {
      *Easting = 0.0;
      *Northing = 0.0;
    }
    else
    {
      if (Southern_Hemisphere != 0)
      {
        Longitude *= -1.0;
        Latitude *= -1.0;
      }
      dlam = Longitude - Polar_Origin_Long;
//       if (dlam > M_PI)
//       {
//         dlam -= TWO_PI;
//       }
//       if (dlam < -M_PI)
//       {
//         dlam += TWO_PI;
//       }
      slat = sin(Latitude);
      essin = es * slat;
      pow_es = POLAR_POW(essin);
      t = tan(PI_Over_4 - Latitude / 2.0) / pow_es;

      if (fabs(fabs(Polar_Origin_Lat) - PI_OVER_2) > 1.0e-10)
        rho = Polar_a_mc * t / tc;
      else
        rho = two_Polar_a * t / e4;

      *Easting = rho * sin(dlam) + Polar_False_Easting;

      if (Southern_Hemisphere != 0)
      {
        *Easting *= -1.0;
        *Northing = rho * cos(dlam) + Polar_False_Northing;
      }
      else
        *Northing = -rho * cos(dlam) + Polar_False_Northing;

    }
  }
  return (Error_Code);
} /* END OF Convert_Geodetic_To_Polar_Stereographic */


long ossimPolarStereoProjection::Convert_Polar_Stereographic_To_Geodetic (double Easting,
                                                                          double Northing,
                                                                          double *Latitude,
                                                                          double *Longitude)const

{ /*  BEGIN Convert_Polar_Stereographic_To_Geodetic  */
/*
 *  The function Convert_Polar_Stereographic_To_Geodetic converts Polar
 *  Stereographic coordinates (easting and northing) to geodetic
 *  coordinates (latitude and longitude) according to the current ellipsoid
 *  and Polar Stereographic projection Parameters. If any errors occur, the
 *  code(s) are returned by the function, otherwise POLAR_NO_ERROR
 *  is returned.
 *
 *  Easting          : Easting (X), in meters                   (input)
 *  Northing         : Northing (Y), in meters                  (input)
 *  Latitude         : Latitude, in radians                     (output)
 *  Longitude        : Longitude, in radians                    (output)
 *
 */

  double dy, dx;
  double rho;
  double t;
  double PHI, sin_PHI;
  double tempPHI = 0.0;
  double essin;
  double pow_es;
//  double temp;
  long Error_Code = POLAR_NO_ERROR;

//   if ((Easting > (Polar_False_Easting + Polar_Delta_Easting)) ||
//       (Easting < (Polar_False_Easting - Polar_Delta_Easting)))
//   { /* Easting out of range */
//     Error_Code |= POLAR_EASTING_ERROR;
//   }
//   if ((Northing > (Polar_False_Northing + Polar_Delta_Northing)) ||
//       (Northing < (Polar_False_Northing - Polar_Delta_Northing)))
//   { /* Northing out of range */
//     Error_Code |= POLAR_NORTHING_ERROR;
//   }
//   if (!Error_Code)
//   {
//     temp = sqrt(Easting * Easting + Northing * Northing);     

//     if ((temp > (Polar_False_Easting + Polar_Delta_Easting)) || 
//         (temp > (Polar_False_Northing + Polar_Delta_Northing)) ||
//         (temp < (Polar_False_Easting - Polar_Delta_Easting)) || 
//         (temp < (Polar_False_Northing - Polar_Delta_Northing)))
//     { /* Point is outside of projection area */
//       Error_Code |= POLAR_RADIUS_ERROR;
//     }
//   }

  if (!Error_Code)
  { /* no errors */

    dy = Northing - Polar_False_Northing;
    dx = Easting - Polar_False_Easting;
    if ((dy == 0.0) && (dx == 0.0))
    {
      *Latitude = PI_OVER_2;
      *Longitude = Polar_Origin_Long;

    }
    else
    {
      if (Southern_Hemisphere != 0)
      {
        dy *= -1.0;
        dx *= -1.0;
      }

      rho = sqrt(dx * dx + dy * dy);
      if (fabs(fabs(Polar_Origin_Lat) - PI_OVER_2) > 1.0e-10)
        t = rho * tc / (Polar_a_mc);
      else
        t = rho * e4 / (two_Polar_a);
      PHI = PI_OVER_2 - 2.0 * atan(t);
      while (fabs(PHI - tempPHI) > 1.0e-10)
      {
        tempPHI = PHI;
        sin_PHI = sin(PHI);
        essin =  es * sin_PHI;
        pow_es = POLAR_POW(essin);
        PHI = PI_OVER_2 - 2.0 * atan(t * pow_es);
      }
      *Latitude = PHI;
      *Longitude = Polar_Origin_Long + atan2(dx, -dy);

//       if (*Longitude > M_PI)
//         *Longitude -= TWO_PI;
//       else if (*Longitude < -M_PI)
//         *Longitude += TWO_PI;


//       if (*Latitude > PI_OVER_2)  /* force distorted values to 90, -90 degrees */
//         *Latitude = PI_OVER_2;
//       else if (*Latitude < -PI_OVER_2)
//         *Latitude = -PI_OVER_2;

//       if (*Longitude > M_PI)  /* force distorted values to 180, -180 degrees */
//         *Longitude = M_PI;
//       else if (*Longitude < -M_PI)
//         *Longitude = -M_PI;

    }
    if (Southern_Hemisphere != 0)
    {
      *Latitude *= -1.0;
      *Longitude *= -1.0;
    }

  }
  return (Error_Code);
} /* END OF Convert_Polar_Stereographic_To_Geodetic */


