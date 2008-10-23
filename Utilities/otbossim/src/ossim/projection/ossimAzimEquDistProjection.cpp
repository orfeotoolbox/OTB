//*******************************************************************
// License:  See top LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Calls Geotrans Utm projection code.  
//*******************************************************************
// $Id: ossimAzimEquDistProjection.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <ossim/projection/ossimAzimEquDistProjection.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimTrace.h>


#define PI_OVER_2  ( M_PI / 2.0)                 
#define ONE        (1.0 * M_PI / 180.0)       /* 1 degree in radians           */

#define AZEQ_NO_ERROR           0x0000
#define AZEQ_LAT_ERROR          0x0001
#define AZEQ_LON_ERROR          0x0002
#define AZEQ_EASTING_ERROR      0x0004
#define AZEQ_NORTHING_ERROR     0x0008
#define AZEQ_ORIGIN_LAT_ERROR   0x0010
#define AZEQ_CENT_MER_ERROR     0x0020
#define AZEQ_A_ERROR            0x0040
#define AZEQ_INV_F_ERROR        0x0080
#define AZEQ_PROJECTION_ERROR   0x0100

RTTI_DEF1(ossimAzimEquDistProjection,"ossimAzimEquDistProjection", ossimMapProjection)
   
ossimAzimEquDistProjection::ossimAzimEquDistProjection(const ossimEllipsoid& ellipsoid,
                                                       const ossimGpt& origin)
   :ossimMapProjection(ellipsoid,
                       origin)
{
   setDefaults();
}

ossimAzimEquDistProjection::ossimAzimEquDistProjection(const ossimEllipsoid& ellipsoid,
                                                       const ossimGpt& origin,
                                                       double falseEasting,
                                                       double falseNorthing)
   :ossimMapProjection(ellipsoid,
                       origin)
{
   setFalseEastingNorthing(falseEasting, falseNorthing);
}

void ossimAzimEquDistProjection::update()
{
   Set_Azimuthal_Equidistant_Parameters(theEllipsoid.getA(),
                                        theEllipsoid.getFlattening(),
                                        theOrigin.latr(),
                                        theOrigin.lonr(),
                                        Azeq_False_Easting,
                                        Azeq_False_Northing);

   theFalseEastingNorthing.x = Azeq_False_Easting;
   theFalseEastingNorthing.y = Azeq_False_Northing;

   ossimMapProjection::update();                                     
}

ossimGpt ossimAzimEquDistProjection::inverse(const ossimDpt &eastingNorthing)const
{
   double lat, lon;

   Convert_Azimuthal_Equidistant_To_Geodetic(eastingNorthing.x,
                                             eastingNorthing.y,
                                             &lat,
                                             &lon);
   
   return ossimGpt(lat*DEG_PER_RAD, lon*DEG_PER_RAD, 0.0, theDatum);  
}

ossimDpt ossimAzimEquDistProjection::forward(const ossimGpt &latLon)const
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

   Convert_Geodetic_To_Azimuthal_Equidistant(gpt.latr(),
                                             gpt.lonr(),
                                             &easting,
                                             &northing);
   return ossimDpt(easting, northing);
}

void ossimAzimEquDistProjection::setFalseEasting(double falseEasting)
{
   Azeq_False_Easting = falseEasting;

   update();
}

void ossimAzimEquDistProjection::setFalseNorthing(double falseNorthing)
{
   Azeq_False_Northing = falseNorthing;
   
   update();
}

void ossimAzimEquDistProjection::setFalseEastingNorthing(double falseEasting,
                                                         double falseNorthing)
{
   Azeq_False_Easting = falseEasting;
   Azeq_False_Northing = falseNorthing;
   
   update();   
}

void ossimAzimEquDistProjection::setDefaults()
{
   Azeq_False_Easting  = 0.0;
   Azeq_False_Northing = 0.0;
   update();
}

bool ossimAzimEquDistProjection::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   return ossimMapProjection::saveState(kwl, prefix);
}

bool ossimAzimEquDistProjection::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   // Must do first...
   bool flag = ossimMapProjection::loadState(kwl, prefix);

   const char* type          = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   setDefaults();
   if(ossimString(type) == STATIC_TYPE_NAME(ossimAzimEquDistProjection))
   {
      Azeq_False_Easting  = theFalseEastingNorthing.x;
      Azeq_False_Northing = theFalseEastingNorthing.y;
   }

   update();
   
   return flag;
}

long ossimAzimEquDistProjection::Set_Azimuthal_Equidistant_Parameters(double a,
                                                                      double f,
                                                                      double Origin_Latitude,
                                                                      double Central_Meridian,
                                                                      double False_Easting,
                                                                      double False_Northing)
{
/* BEGIN Set_Azimuthal_Equidistant_Parameters */
/*
 * The function Set_Azimuthal_Equidistant_Parameters receives the ellipsoid parameters and
 * projection parameters as inputs, and sets the corresponding state
 * variables.  If any errors occur, the error code(s) are returned by the function, 
 * otherwise AZEQ_NO_ERROR is returned.
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

  double es2, es4, es6;
//  double inv_f = 1 / f;
  double temp_Northing = 0.0;
  long Error_Code = AZEQ_NO_ERROR;

//   if (a <= 0.0)
//   { /* Semi-major axis must be greater than zero */
//     Error_Code |= AZEQ_A_ERROR;
//   }
//   if ((inv_f < 250) || (inv_f > 350))
//   { /* Inverse flattening must be between 250 and 350 */
//     Error_Code |= AZEQ_INV_F_ERROR;
//   }
//   if ((Origin_Latitude < -PI_OVER_2) || (Origin_Latitude > PI_OVER_2))
//   { /* origin latitude out of range */
//     Error_Code |= AZEQ_ORIGIN_LAT_ERROR;
//   }
//   if ((Central_Meridian < -PI) || (Central_Meridian > TWO_PI))
//   { /* origin longitude out of range */
//     Error_Code |= AZEQ_CENT_MER_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */
    Azeq_a = a;
    Azeq_f = f;
    es2 = 2 * Azeq_f - Azeq_f * Azeq_f;
    es4 = es2 * es2;
    es6 = es4 * es2;
    /* spherical radius */
    Ra = Azeq_a * (1.0 - es2 / 6.0 - 17.0 * es4 / 360.0 - 67.0 * es6 / 3024.0);
    Azeq_Origin_Lat = Origin_Latitude;
    Sin_Azeq_Origin_Lat = sin(Azeq_Origin_Lat);
    Cos_Azeq_Origin_Lat = cos(Azeq_Origin_Lat);
    abs_Azeq_Origin_Lat = fabs(Azeq_Origin_Lat);
//     if (Central_Meridian > M_PI)
//       Central_Meridian -= TWO_PI;
    Azeq_Origin_Long = Central_Meridian;
    Azeq_False_Northing = False_Northing;
    Azeq_False_Easting = False_Easting;

    if (fabs(abs_Azeq_Origin_Lat - PI_OVER_2) < 1.0e-10)
    {
      Azeq_Delta_Northing = 20015110.0;
      Azeq_Delta_Easting = 20015110.0;
    }
    else if (abs_Azeq_Origin_Lat >= 1.0e-10)
    {
      if (Azeq_Origin_Long > 0.0)
      {
        Convert_Geodetic_To_Azimuthal_Equidistant(-Azeq_Origin_Lat, 
                                                  (Azeq_Origin_Long - M_PI - ONE), &Azeq_Delta_Easting, &temp_Northing);
      }
      else
      {
        Convert_Geodetic_To_Azimuthal_Equidistant(-Azeq_Origin_Lat, 
                                                  (Azeq_Origin_Long + M_PI - ONE), &Azeq_Delta_Easting, &temp_Northing);
      }
      Azeq_Delta_Northing = 19903915.0;
    }
    else
    {
      Azeq_Delta_Northing = 19903915.0;
      Azeq_Delta_Easting = 19903915.0;
    }
  } /* End if(!Error_Code) */
  return (Error_Code);
} /* End Set_Azimuthal_Equidistant_Parameters */


void ossimAzimEquDistProjection::Get_Azimuthal_Equidistant_Parameters(double *a,
                                                                      double *f,
                                                                      double *Origin_Latitude,
                                                                      double *Central_Meridian,
                                                                      double *False_Easting,
                                                                      double *False_Northing)const
{
/* Begin Get_Azimuthal_Equidistant_Parameters */
/*
 * The function Get_Azimuthal_Equidistant_Parameters returns the current ellipsoid
 * parameters and Azimuthal Equidistant projection parameters.
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

  *a = Azeq_a;
  *f = Azeq_f;
  *Origin_Latitude = Azeq_Origin_Lat;
  *Central_Meridian = Azeq_Origin_Long;
  *False_Easting = Azeq_False_Easting;
  *False_Northing = Azeq_False_Northing;
  return;
} /* End Get_Azimuthal_Equidistant_Parameters */


long ossimAzimEquDistProjection::Convert_Geodetic_To_Azimuthal_Equidistant (double Latitude,
                                                                            double Longitude,
                                                                            double *Easting,
                                                                            double *Northing)const
{
/* Begin Convert_Geodetic_To_Azimuthal_Equidistant */
/*
 * The function Convert_Geodetic_To_Azimuthal_Equidistant converts geodetic (latitude and
 * longitude) coordinates to Azimuthal Equidistant projection (easting and northing)
 * coordinates, according to the current ellipsoid and Azimuthal Equidistant projection
 * parameters.  If any errors occur, the error code(s) are returned by the
 * function, otherwise AZEQ_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi) in radians           (input)
 *    Longitude         : Longitude (lambda) in radians       (input)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */

  double dlam;       /* Longitude - Central Meridan */
  double k_prime;    /* scale factor */
  double c;          /* angular distance from center */
  double slat = sin(Latitude);
  double clat = cos(Latitude);
  double cos_c;
  double sin_dlam, cos_dlam;
  double Ra_kprime;
  double Ra_PI_OVER_2_Lat;
  long Error_Code = AZEQ_NO_ERROR;

//   if ((Latitude < -PI_OVER_2) || (Latitude > PI_OVER_2))
//   { /* Latitude out of range */
//     Error_Code |= AZEQ_LAT_ERROR;
//   }
//   if ((Longitude < -M_PI) || (Longitude > TWO_PI))
//   { /* Longitude out of range */
//     Error_Code |= AZEQ_LON_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */
    dlam = Longitude - Azeq_Origin_Long;
    if (dlam > M_PI)
    {
      dlam -= TWO_PI;
    }
    if (dlam < -M_PI)
    {
      dlam += TWO_PI;
    }

    sin_dlam = sin(dlam);
    cos_dlam = cos(dlam);
    if (fabs(abs_Azeq_Origin_Lat - PI_OVER_2) < 1.0e-10)
    {
      if (Azeq_Origin_Lat >= 0.0)
      {
        Ra_PI_OVER_2_Lat = Ra * (PI_OVER_2 - Latitude);
        *Easting = Ra_PI_OVER_2_Lat * sin_dlam + Azeq_False_Easting;
        *Northing = -1.0 * (Ra_PI_OVER_2_Lat * cos_dlam) + Azeq_False_Northing;
      }
      else
      {
        Ra_PI_OVER_2_Lat = Ra * (PI_OVER_2 + Latitude);
        *Easting = Ra_PI_OVER_2_Lat * sin_dlam + Azeq_False_Easting;
        *Northing = Ra_PI_OVER_2_Lat * cos_dlam + Azeq_False_Northing;
      }
    }
    else if (abs_Azeq_Origin_Lat <= 1.0e-10)
    {
      cos_c = clat * cos_dlam;
      if (fabs(fabs(cos_c) - 1.0) < 1.0e-14)
      {
        if (cos_c >= 0.0)
        {
          *Easting = Azeq_False_Easting;
          *Northing = Azeq_False_Northing;
        }
        else
        {
          /* if cos_c == -1 */
          Error_Code |= AZEQ_PROJECTION_ERROR;
        }
      }
      else
      {
        c = acos(cos_c);
        k_prime = c / sin(c);
        Ra_kprime = Ra * k_prime;
        *Easting = Ra_kprime * clat * sin_dlam + Azeq_False_Easting;
        *Northing = Ra_kprime * slat + Azeq_False_Northing;
      }
    }
    else
    {
      cos_c = (Sin_Azeq_Origin_Lat * slat) + (Cos_Azeq_Origin_Lat * clat * cos_dlam);
      if (fabs(fabs(cos_c) - 1.0) < 1.0e-14)
      {
        if (cos_c >= 0.0)
        {
          *Easting = Azeq_False_Easting;
          *Northing = Azeq_False_Northing;
        }
        else
        {
          /* if cos_c == -1 */
          Error_Code |= AZEQ_PROJECTION_ERROR;
        }
      }
      else
      {
        c = acos(cos_c);
        k_prime = c / sin(c);
        Ra_kprime = Ra * k_prime;
        *Easting = Ra_kprime * clat * sin_dlam + Azeq_False_Easting;
        *Northing = Ra_kprime * (Cos_Azeq_Origin_Lat * slat - Sin_Azeq_Origin_Lat * clat * cos_dlam) + Azeq_False_Northing;
      }
    }
  }
  return (Error_Code);
} /* End Convert_Geodetic_To_Azimuthal_Equidistant */


long ossimAzimEquDistProjection::Convert_Azimuthal_Equidistant_To_Geodetic(double Easting,
                                                                           double Northing,
                                                                           double *Latitude,
                                                                           double *Longitude)const
{ /* Begin Convert_Azimuthal_Equidistant_To_Geodetic */
/*
 * The function Convert_Azimuthal_Equidistant_To_Geodetic converts Azimuthal_Equidistant projection
 * (easting and northing) coordinates to geodetic (latitude and longitude)
 * coordinates, according to the current ellipsoid and Azimuthal_Equidistant projection
 * coordinates.  If any errors occur, the error code(s) are returned by the
 * function, otherwise AZEQ_NO_ERROR is returned.
 *
 *    Easting           : Easting (X) in meters                  (input)
 *    Northing          : Northing (Y) in meters                 (input)
 *    Latitude          : Latitude (phi) in radians              (output)
 *    Longitude         : Longitude (lambda) in radians          (output)
 */

  double dx, dy;
  double rho;        /* height above ellipsoid */
  double c;          /* angular distance from center */
  double sin_c, cos_c, dy_sinc;
  long Error_Code = AZEQ_NO_ERROR; 

//   if ((Easting < (Azeq_False_Easting - Azeq_Delta_Easting)) 
//       || (Easting > (Azeq_False_Easting + Azeq_Delta_Easting)))
//   { /* Easting out of range  */
//     Error_Code |= AZEQ_EASTING_ERROR;
//   }
//   if ((Northing < (Azeq_False_Northing - Azeq_Delta_Northing)) 
//       || (Northing > (Azeq_False_Northing + Azeq_Delta_Northing)))
//   { /* Northing out of range */
//     Error_Code |= AZEQ_NORTHING_ERROR;
//   }

  if (!Error_Code)
  {
    dy = Northing - Azeq_False_Northing;
    dx = Easting - Azeq_False_Easting;
    rho = sqrt(dx * dx + dy * dy);
    if (fabs(rho) <= 1.0e-10)
    {
      *Latitude = Azeq_Origin_Lat;
      *Longitude = Azeq_Origin_Long;
    }
    else
    {
      c = rho / Ra;
      sin_c = sin(c);
      cos_c = cos(c);
      dy_sinc = dy * sin_c;
      *Latitude = asin((cos_c * Sin_Azeq_Origin_Lat) + ((dy_sinc * Cos_Azeq_Origin_Lat) / rho));
      if (fabs(abs_Azeq_Origin_Lat - PI_OVER_2) < 1.0e-10)
      {
        if (Azeq_Origin_Lat >= 0.0)
          *Longitude = Azeq_Origin_Long + atan2(dx, -dy);
        else
          *Longitude = Azeq_Origin_Long + atan2(dx, dy);
      }
      else
        *Longitude = Azeq_Origin_Long + atan2((dx * sin_c), ((rho * Cos_Azeq_Origin_Lat * cos_c) - (dy_sinc * Sin_Azeq_Origin_Lat)));
    }

    if (*Latitude > PI_OVER_2)  /* force distorted values to 90, -90 degrees */
      *Latitude = PI_OVER_2;
    else if (*Latitude < -PI_OVER_2)
      *Latitude = -PI_OVER_2;

//     if (*Longitude > M_PI)
//       *Longitude -= TWO_PI;
//     if (*Longitude < -M_PI)
//       *Longitude += TWO_PI;

    if (*Longitude > M_PI)  /* force distorted values to 180, -180 degrees */
      *Longitude = M_PI;
    else if (*Longitude < -M_PI)
      *Longitude = -M_PI;

  }
  return (Error_Code);
} /* End Convert_Azimuthal_Equidistant_To_Geodetic */
