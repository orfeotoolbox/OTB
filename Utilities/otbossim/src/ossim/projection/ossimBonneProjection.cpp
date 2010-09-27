//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Calls Geotrans Bonne projection code.  
//*******************************************************************
//  $Id: ossimBonneProjection.cpp 17815 2010-08-03 13:23:14Z dburken $
#include <ossim/projection/ossimBonneProjection.h>
#include <ossim/projection/ossimSinusoid.h>
#include <ossim/base/ossimKeywordNames.h>

RTTI_DEF1(ossimBonneProjection, "ossimBonneProjection", ossimMapProjection)


/***************************************************************************/
/*
 *                               DEFINES
 */
#define BONN_NO_ERROR           0x0000
#define BONN_LAT_ERROR          0x0001
#define BONN_LON_ERROR          0x0002
#define BONN_EASTING_ERROR      0x0004
#define BONN_NORTHING_ERROR     0x0008
#define BONN_ORIGIN_LAT_ERROR   0x0010
#define BONN_CENT_MER_ERROR     0x0020
#define BONN_A_ERROR            0x0040
#define BONN_B_ERROR            0x0080
#define BONN_A_LESS_B_ERROR     0x0100

#define PI_OVER_2  (M_PI / 2.0)                 
#define BONN_m(coslat,sinlat)                   (coslat/sqrt(1.0 - es2*sinlat*sinlat))
#define BONN_M(c0lat,c1s2lat,c2s4lat,c3s6lat)   (Bonn_a*(c0lat-c1s2lat+c2s4lat-c3s6lat))
#define COEFF_TIMES_BONN_SIN(coeff,x,latit)	    (coeff*(sin(x * latit)))
#define FLOAT_EQ(x,v,epsilon)   (((v - epsilon) < x) && (x < (v + epsilon)))

ossimBonneProjection::ossimBonneProjection(const ossimEllipsoid& ellipsoid,
                                           const ossimGpt& origin)
   :ossimMapProjection(ellipsoid, origin)
{
   setDefaults();   
   update();
}

ossimBonneProjection::ossimBonneProjection(const ossimEllipsoid& ellipsoid,
                                           const ossimGpt& origin,
                                           const double falseEasting,
                                           const double falseNorthing)
   :ossimMapProjection(ellipsoid, origin)
{
   Bonn_False_Easting  = falseEasting;
   Bonn_False_Northing = falseNorthing;
   Bonn_Delta_Northing = 25000000.0;

   update();
}

void ossimBonneProjection::update()
{
   Set_Bonne_Parameters(theEllipsoid.getA(),
                        theEllipsoid.getFlattening(),
                        theOrigin.latr(),
                        theOrigin.lonr(),
                        Bonn_False_Easting,
                        Bonn_False_Northing);

   theFalseEastingNorthing.x = Bonn_False_Easting;
   theFalseEastingNorthing.y = Bonn_False_Northing;

   ossimMapProjection::update();
}

void ossimBonneProjection::setFalseEasting(double falseEasting)
{
   Bonn_False_Easting = falseEasting;
   update();
}

void ossimBonneProjection::setFalseNorthing(double falseNorthing)
{
   Bonn_False_Northing = falseNorthing;
   update();
}

void ossimBonneProjection::setFalseEastingNorthing(double falseEasting,
                                                   double falseNorthing)
{
   Bonn_False_Easting = falseEasting;
   Bonn_False_Northing = falseNorthing;  
   update();
}

void ossimBonneProjection::setDefaults()
{

   Bonn_False_Easting  = 0.0;
   Bonn_False_Northing = 0.0;  
   Bonn_Delta_Northing = 25000000.0;
   if(theOrigin.latd() == 0.0)
   {
      // we can't have the origin of lat 0 for Bonne
      // so bump it up an arc second.
      //
      theOrigin.latd(1.0/3600.0);
   }
}

ossimGpt ossimBonneProjection::inverse(const ossimDpt &eastingNorthing)const
{
   double lat, lon;

   Convert_Bonne_To_Geodetic(eastingNorthing.x,
                             eastingNorthing.y,
                             &lat,
                             &lon);
   return ossimGpt(lat*DEG_PER_RAD, lon*DEG_PER_RAD, 0, theDatum);  
}

ossimDpt ossimBonneProjection::forward(const ossimGpt &latLon)const
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

   Convert_Geodetic_To_Bonne(gpt.latr(),
                             gpt.lonr(),
                             &easting,
                             &northing);
   return ossimDpt(easting, northing);
}

bool ossimBonneProjection::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   return ossimMapProjection::saveState(kwl, prefix);
}

bool ossimBonneProjection::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   // Must do this first.
   bool flag = ossimMapProjection::loadState(kwl, prefix);
   
   const char* type          = kwl.find(prefix, ossimKeywordNames::TYPE_KW);

   setDefaults();

   if(ossimString(type) == STATIC_TYPE_NAME(ossimBonneProjection))
   {
      Bonn_False_Easting  = theFalseEastingNorthing.x;
      Bonn_False_Northing = theFalseEastingNorthing.y;
   }
   
   update();
   return flag;
}


/*
 * These state variables are for optimization purposes.  The only function
 * that should modify them is Set_Bonne_Parameters.
 */


/***************************************************************************/
/*
 *                              FUNCTIONS     
 */


long ossimBonneProjection::Set_Bonne_Parameters(double a,
                                                double f,
                                                double Origin_Latitude,
                                                double Central_Meridian,
                                                double False_Easting,
                                                double False_Northing)
{ /* Begin Set_Bonne_Parameters */
/*
 * The function Set_Bonne_Parameters receives the ellipsoid parameters and
 * Bonne projection parameters as inputs, and sets the corresponding state
 * variables.  If any errors occur, the error code(s) are returned by the 
 * function, otherwise BONN_NO_ERROR is returned.
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

  double j, three_es4;
  double x,e1,e2,e3,e4;
  double clat; 
  double sin2lat, sin4lat, sin6lat, lat;
//  double inv_f = 1 / f;
  long Error_Code = BONN_NO_ERROR;

//   if (a <= 0.0)
//   { /* Semi-major axis must be greater than zero */
//     Error_Code |= BONN_A_ERROR;
//   }
//   if ((inv_f < 250) || (inv_f > 350))
//   { /* Inverse flattening must be between 250 and 350 */
// 	 Error_Code |= BONN_INV_F_ERROR;
//   } 
//   if ((Origin_Latitude < -PI_OVER_2) || (Origin_Latitude > PI_OVER_2))
//   { /* origin latitude out of range */
//     Error_Code |= BONN_ORIGIN_LAT_ERROR;
//   }
//   if ((Central_Meridian < -PI) || (Central_Meridian > TWO_PI))
//   { /* origin longitude out of range */
//     Error_Code |= BONN_CENT_MER_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */
    Bonn_a = a;
    Bonn_f = f;
    Bonn_Origin_Lat = Origin_Latitude;
//     if (Central_Meridian > PI)
//       Central_Meridian -= TWO_PI;
    Bonn_Origin_Long = Central_Meridian;
    Bonn_False_Northing = False_Northing;
    Bonn_False_Easting = False_Easting;
    if (Bonn_Origin_Lat == 0.0)
    {
       if (Bonn_Origin_Long > 0)
       {
          Bonn_Max_Easting = 19926189.0;
          Bonn_Min_Easting = -20037509.0;
       }
       else if (Bonn_Origin_Long < 0)
       {
          Bonn_Max_Easting = 20037509.0;
          Bonn_Min_Easting = -19926189.0;
       }
       else
       {
          Bonn_Max_Easting = 20037509.0;
          Bonn_Min_Easting = -20037509.0;
       }
       Bonn_Delta_Northing = 10001966.0;
       Set_Sinusoidal_Parameters(Bonn_a, Bonn_f, Bonn_Origin_Long, Bonn_False_Easting, Bonn_False_Northing);
    }
    else
    {
       Sin_Bonn_Origin_Lat = sin(Bonn_Origin_Lat);
       
       es2 = 2 * Bonn_f - Bonn_f * Bonn_f;
       es4 = es2 * es2;
       es6 = es4 * es2;
       j = 45.0 * es6 / 1024.0;
       three_es4 = 3.0 * es4;
       c0 = 1 - es2 / 4.0 - three_es4 / 64.0 - 5.0 * es6 / 256.0;
       c1 = 3.0 * es2 / 8.0 + three_es4 / 32.0 + j;
       c2 = 15.0 * es4 / 256.0 + j;
       c3 = 35.0 * es6 / 3072.0;
       
       clat = cos(Bonn_Origin_Lat);
       m1 = BONN_m(clat, Sin_Bonn_Origin_Lat);
       
       lat = c0 * Bonn_Origin_Lat;
       sin2lat = COEFF_TIMES_BONN_SIN(c1, 2.0, Bonn_Origin_Lat);
       sin4lat = COEFF_TIMES_BONN_SIN(c2, 4.0, Bonn_Origin_Lat);
       sin6lat = COEFF_TIMES_BONN_SIN(c3, 6.0, Bonn_Origin_Lat);
       M1 = BONN_M(lat, sin2lat, sin4lat, sin6lat);
       
       x = sqrt (1.0 - es2);
       e1 = (1.0 - x) / (1.0 + x);
       e2 = e1 * e1;
       e3 = e2 * e1;
       e4 = e3 * e1;
       a0 = 3.0 * e1 / 2.0 - 27.0 * e3 / 32.0;
       a1 = 21.0 * e2 / 16.0 - 55.0 * e4 / 32.0;
       a2 = 151.0 * e3 / 96.0;
       a3 = 1097.0 * e4 / 512.0;
       if (Sin_Bonn_Origin_Lat == 0.0)
          Bonn_am1sin = 0.0;
       else
          Bonn_am1sin = Bonn_a * m1 / Sin_Bonn_Origin_Lat;
       
       Bonn_Max_Easting = 20027474.0;
       Bonn_Min_Easting = -20027474.0;
       Bonn_Delta_Northing = 20003932.0;
       
    }
    
  } /* End if(!Error_Code) */
  return (Error_Code);
} /* End Set_Bonne_Parameters */


void ossimBonneProjection::Get_Bonne_Parameters(double *a,
                                                double *f,
                                                double *Origin_Latitude,
                                                double *Central_Meridian,
                                                double *False_Easting,
                                                double *False_Northing)const
{ /* Begin Get_Bonne_Parameters */
/*
 * The function Get_Bonne_Parameters returns the current ellipsoid
 * parameters and Bonne projection parameters.
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

  *a = Bonn_a;
  *f = Bonn_f;
  *Origin_Latitude = Bonn_Origin_Lat;
  *Central_Meridian = Bonn_Origin_Long;
  *False_Easting = Bonn_False_Easting;
  *False_Northing = Bonn_False_Northing;
  return;
} /* End Get_Bonne_Parameters */


long ossimBonneProjection::Convert_Geodetic_To_Bonne (double Latitude,
                                                      double Longitude,
                                                      double *Easting,
                                                      double *Northing)const
{ /* Begin Convert_Geodetic_To_Bonne */
/*
 * The function Convert_Geodetic_To_Bonne converts geodetic (latitude and
 * longitude) coordinates to Bonne projection (easting and northing)
 * coordinates, according to the current ellipsoid and Bonne projection
 * parameters.  If any errors occur, the error code(s) are returned by the
 * function, otherwise BONN_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi) in radians           (input)
 *    Longitude         : Longitude (lambda) in radians       (input)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */

  double dlam; /* Longitude - Central Meridan */
  double mm;
  double MM;
  double rho;
  double EE;
  double clat = cos(Latitude);
  double slat = sin(Latitude);
  double lat, sin2lat, sin4lat, sin6lat;
  long Error_Code = BONN_NO_ERROR;

//   if ((Latitude < -PI_OVER_2) || (Latitude > PI_OVER_2))
//   { /* Latitude out of range */
//     Error_Code |= BONN_LAT_ERROR;
//   }
//   if ((Longitude < -PI) || (Longitude > TWO_PI))
//   { /* Longitude out of range */
//     Error_Code |= BONN_LON_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */
     if (Bonn_Origin_Lat == 0.0)
        Convert_Geodetic_To_Sinusoidal(Latitude, Longitude, Easting, Northing);
     else
     {
        dlam = Longitude - Bonn_Origin_Long;
        if (dlam > M_PI)
        {
           dlam -= TWO_PI;
        }
        if (dlam < -M_PI)
        {
           dlam += TWO_PI;
        }
        if ((Latitude - Bonn_Origin_Lat) == 0.0 && FLOAT_EQ(fabs(Latitude),PI_OVER_2,.00001))
        {
           *Easting = 0.0;
           *Northing = 0.0;
        }
        else
        {
           mm = BONN_m(clat, slat);
           
           lat = c0 * Latitude;
           sin2lat = COEFF_TIMES_BONN_SIN(c1, 2.0, Latitude);
           sin4lat = COEFF_TIMES_BONN_SIN(c2, 4.0, Latitude);
           sin6lat = COEFF_TIMES_BONN_SIN(c3, 6.0, Latitude);
           MM = BONN_M(lat, sin2lat, sin4lat, sin6lat);         
           
           rho = Bonn_am1sin + M1 - MM;
           if (rho == 0)
              EE = 0;
           else
              EE = Bonn_a * mm * dlam / rho;
           *Easting = rho * sin(EE) + Bonn_False_Easting;
           *Northing = Bonn_am1sin - rho * cos(EE) + Bonn_False_Northing;
        }
     }
  }
  return (Error_Code);
} /* End Convert_Geodetic_To_Bonne */


long ossimBonneProjection::Convert_Bonne_To_Geodetic(double Easting,
                                                     double Northing,
                                                     double *Latitude,
                                                     double *Longitude)const
{ /* Begin Convert_Bonne_To_Geodetic */
/*
 * The function Convert_Bonne_To_Geodetic converts Bonne projection
 * (easting and northing) coordinates to geodetic (latitude and longitude)
 * coordinates, according to the current ellipsoid and Bonne projection
 * coordinates.  If any errors occur, the error code(s) are returned by the
 * function, otherwise BONN_NO_ERROR is returned.
 *
 *    Easting           : Easting (X) in meters                  (input)
 *    Northing          : Northing (Y) in meters                 (input)
 *    Latitude          : Latitude (phi) in radians              (output)
 *    Longitude         : Longitude (lambda) in radians          (output)
 */

  double dx;     /* Delta easting - Difference in easting (easting-FE)      */
  double dy;     /* Delta northing - Difference in northing (northing-FN)   */
  double mu;
  double MM;
  double mm;
  double am1sin_dy;
  double rho;
  double sin2mu, sin4mu, sin6mu, sin8mu;
  double clat, slat;
  long Error_Code = BONN_NO_ERROR;

//   if ((Easting < (Bonn_False_Easting + Bonn_Min_Easting))
//       || (Easting > (Bonn_False_Easting + Bonn_Max_Easting)))
//   { /* Easting out of range */
//     Error_Code |= BONN_EASTING_ERROR;
//   }
//   if ((Northing < (Bonn_False_Northing - Bonn_Delta_Northing))
//       || (Northing > (Bonn_False_Northing + Bonn_Delta_Northing)))
//   { /* Northing out of range */
//     Error_Code |= BONN_NORTHING_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */
     if (Bonn_Origin_Lat == 0.0)
        Convert_Sinusoidal_To_Geodetic(Easting, Northing, Latitude, Longitude);
     else
     {
        dy = Northing - Bonn_False_Northing;
        dx = Easting - Bonn_False_Easting;
        am1sin_dy = Bonn_am1sin - dy;
        rho = sqrt(dx * dx + am1sin_dy * am1sin_dy);
        if (Bonn_Origin_Lat < 0.0)
           rho = -rho;
        MM = Bonn_am1sin + M1 - rho;
        
        mu = MM / (Bonn_a * c0); 
        sin2mu = COEFF_TIMES_BONN_SIN(a0, 2.0, mu);
        sin4mu = COEFF_TIMES_BONN_SIN(a1, 4.0, mu);
        sin6mu = COEFF_TIMES_BONN_SIN(a2, 6.0, mu);
        sin8mu = COEFF_TIMES_BONN_SIN(a3, 8.0, mu);
        *Latitude = mu + sin2mu + sin4mu + sin6mu + sin8mu;
        
        if (FLOAT_EQ(fabs(*Latitude),PI_OVER_2,.00001))
        {
           *Longitude = Bonn_Origin_Long;
        }
        else
        {
           clat = cos(*Latitude);
           slat = sin(*Latitude);
           mm = BONN_m(clat, slat);
           
           if (Bonn_Origin_Lat < 0.0)
           {
              dx = -dx;
              am1sin_dy = -am1sin_dy;
           }
           *Longitude = Bonn_Origin_Long + rho * (atan2(dx, am1sin_dy)) /
                        (Bonn_a * mm);
        }
        
        if (*Latitude > PI_OVER_2)  /* force distorted values to 90, -90 degrees */
           *Latitude = PI_OVER_2;
        else if (*Latitude < -PI_OVER_2)
           *Latitude = -PI_OVER_2;
        
//         if (*Longitude > PI)
//            *Longitude -= TWO_PI;
//         if (*Longitude < -PI)
//            *Longitude += TWO_PI;
        
        if (*Longitude > M_PI)/* force distorted values to 180, -180 degrees */
           *Longitude = M_PI;
        else if (*Longitude < -M_PI)
           *Longitude = -M_PI;
     }
  }
  return (Error_Code);
} /* End Convert_Bonne_To_Geodetic */

//*************************************************************************************************
//! Returns TRUE if principal parameters are within epsilon tolerance.
//*************************************************************************************************
bool ossimBonneProjection::operator==(const ossimProjection& proj) const
{
   if (!ossimMapProjection::operator==(proj))
      return false;

   ossimBonneProjection* p = PTR_CAST(ossimBonneProjection, &proj);
   if (!p)
      return false;

   if (!ossim::almostEqual(Bonn_False_Easting, p->Bonn_False_Easting) ) return false;
   if (!ossim::almostEqual(Bonn_False_Northing,p->Bonn_False_Northing)) return false;
   if (!ossim::almostEqual(Bonn_Delta_Northing,p->Bonn_Delta_Northing)) return false;

   return true;
}
