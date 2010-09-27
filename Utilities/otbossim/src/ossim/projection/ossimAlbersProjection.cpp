//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Calls Geotrans Albers projection code.  
//*******************************************************************
//  $Id: ossimAlbersProjection.cpp 17815 2010-08-03 13:23:14Z dburken $

#include <ossim/projection/ossimAlbersProjection.h>

#include <ossim/base/ossimKeywordNames.h>

RTTI_DEF1(ossimAlbersProjection, "ossimAlbersProjection", ossimMapProjection)

#ifdef PI_OVER_2
#  undef PI_OVER_2
#endif   

#define ALBERS_NO_ERROR           0x0000
#define ALBERS_LAT_ERROR          0x0001
#define ALBERS_LON_ERROR          0x0002
#define ALBERS_EASTING_ERROR      0x0004
#define ALBERS_NORTHING_ERROR     0x0008
#define ALBERS_ORIGIN_LAT_ERROR   0x0010
#define ALBERS_CENT_MER_ERROR     0x0020
#define ALBERS_A_ERROR            0x0040
#define ALBERS_INV_F_ERROR        0x0080
#define ALBERS_FIRST_STDP_ERROR   0x0100
#define ALBERS_SECOND_STDP_ERROR  0x0200
#define ALBERS_FIRST_SECOND_ERROR 0x0400
#define ALBERS_HEMISPHERE_ERROR   0x0800

#define PI_OVER_2  ( M_PI / 2.0)                 
#define ES_SIN(sinlat)          (es * sinlat)
#define ONE_MINUS_SQR(x)        (1.0 - x * x)
#define ALBERS_M(clat,oneminussqressin)   (clat / sqrt(oneminussqressin))
#define ALBERS_Q(slat,oneminussqressin,essin) (one_MINUS_es2)*(slat / (oneminussqressin)-    \
											  (1 / (two_es)) *log((1 - essin) / (1 + essin)))


ossimAlbersProjection::ossimAlbersProjection(const ossimEllipsoid& ellipsoid,
                                             const ossimGpt& origin)
   :ossimMapProjection(ellipsoid, origin)
{
   Albers_Delta_Northing = 40000000.0;
   Albers_Delta_Easting  = 40000000.0;
   setDefaults();
   update();
}
    
ossimAlbersProjection::ossimAlbersProjection(const ossimEllipsoid& ellipsoid,
                                             const ossimGpt& origin,
                                             double stdParallel1,
                                             double stdParallel2,
                                             double falseEasting,
                                             double falseNorthing)
  :ossimMapProjection(ellipsoid, origin)
{
   Albers_Std_Parallel_1 = stdParallel1*RAD_PER_DEG;
   Albers_Std_Parallel_2 = stdParallel2*RAD_PER_DEG;
   Albers_Delta_Northing = 40000000.0;
   Albers_Delta_Easting  = 40000000.0;
   Albers_False_Easting  = falseEasting;
   Albers_False_Northing = falseNorthing;
   
   update();
   
}

ossimObject* ossimAlbersProjection::dup() const
{
   return new ossimAlbersProjection(*this);
}

ossimAlbersProjection::~ossimAlbersProjection()
{
}

void ossimAlbersProjection::update()
{
   Set_Albers_Parameters(theEllipsoid.getA(),
                         theEllipsoid.getFlattening(),
                         theOrigin.latr(),
                         theOrigin.lonr(),
                         Albers_Std_Parallel_1,
                         Albers_Std_Parallel_2,
                         Albers_False_Easting,
                         Albers_False_Northing);
   
   theFalseEastingNorthing.x = Albers_False_Easting;
   theFalseEastingNorthing.y = Albers_False_Northing;

   ossimMapProjection::update();
}


ossimGpt ossimAlbersProjection::inverse(const ossimDpt &eastingNorthing)const
{
   double lat, lon;
   
   
   Convert_Albers_To_Geodetic(eastingNorthing.x,
                              eastingNorthing.y,
                              &lat,
                              &lon);
   
   return ossimGpt(lat*DEG_PER_RAD, lon*DEG_PER_RAD, 0, theDatum);
}

ossimDpt ossimAlbersProjection::forward(const ossimGpt &latLon)const
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


   Convert_Geodetic_To_Albers(gpt.latr(),
                              gpt.lonr(),
                              &easting,
                              &northing);
   
   return ossimDpt(easting, northing);
}

void ossimAlbersProjection::setStandardParallel1(double degree)
{
   Albers_Std_Parallel_1 = degree*RAD_PER_DEG;
   update();
}

void ossimAlbersProjection::setStandardParallel2(double degree)
{
   Albers_Std_Parallel_2 = degree*RAD_PER_DEG;
   update();
}

void ossimAlbersProjection::setStandardParallels(double parallel1Degree,
                                                 double parallel2Degree)
{
   Albers_Std_Parallel_1 = parallel1Degree*RAD_PER_DEG;
   Albers_Std_Parallel_2 = parallel2Degree*RAD_PER_DEG;
   update();
   
}

void ossimAlbersProjection::setFalseEasting(double falseEasting)
{
   Albers_False_Easting = falseEasting;
   update();
}

void ossimAlbersProjection::setFalseNorthing(double falseNorthing)
{
   Albers_False_Northing = falseNorthing;
   update();
}

void ossimAlbersProjection::setFalseEastingNorthing(double falseEasting,
                                                    double falseNorthing)
{
   Albers_False_Easting = falseEasting;
   Albers_False_Northing = falseNorthing;  
   update();
}

void ossimAlbersProjection::setParameters(double parallel1,
                                          double parallel2,
                                          double falseEasting,
                                          double falseNorthing)
{
   Albers_False_Easting = falseEasting;
   Albers_False_Northing = falseNorthing;  
   Albers_Std_Parallel_1 = parallel1*RAD_PER_DEG;
   Albers_Std_Parallel_2 = parallel2*RAD_PER_DEG;
   update(); 
}

double ossimAlbersProjection::getStandardParallel1()const
{
   return  Albers_Std_Parallel_1/RAD_PER_DEG;
}

double ossimAlbersProjection::getStandardParallel2()const
{
   return  Albers_Std_Parallel_2/RAD_PER_DEG;
}

void ossimAlbersProjection::setDefaults()
{
   // initialize to some default
   Albers_Std_Parallel_1 = 29.5*RAD_PER_DEG;
   Albers_Std_Parallel_2 = 45.5*RAD_PER_DEG;
   Albers_False_Easting  = 0.0;
   Albers_False_Northing = 0.0;  
}

bool ossimAlbersProjection::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   kwl.add(prefix,
           ossimKeywordNames::STD_PARALLEL_1_KW,
           Albers_Std_Parallel_1*DEG_PER_RAD,
           true);

   kwl.add(prefix,
           ossimKeywordNames::STD_PARALLEL_2_KW,
           Albers_Std_Parallel_2*DEG_PER_RAD,
           true);
   
   return ossimMapProjection::saveState(kwl, prefix);
}

bool ossimAlbersProjection::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   // Must do this first...
   bool flag = ossimMapProjection::loadState(kwl, prefix);

   const char* type          = kwl.find(prefix, ossimKeywordNames::TYPE_KW);

   const char* stdParallel1  = kwl.find(prefix, ossimKeywordNames::STD_PARALLEL_1_KW);
   const char* stdParallel2  = kwl.find(prefix, ossimKeywordNames::STD_PARALLEL_2_KW);

   
   if(ossimString(type) == STATIC_TYPE_NAME(ossimAlbersProjection))
   {
      Albers_False_Easting  = theFalseEastingNorthing.x;
      Albers_False_Northing = theFalseEastingNorthing.y;

      if(stdParallel1)
      {
         Albers_Std_Parallel_1 = ossimString(stdParallel1).toDouble()*RAD_PER_DEG;
      }
      else
      {
         Albers_Std_Parallel_1 = 29.5*RAD_PER_DEG;         
      }
      if(stdParallel2)
      {
         Albers_Std_Parallel_2 = ossimString(stdParallel2).toDouble()*RAD_PER_DEG;
      }
      else
      {
         Albers_Std_Parallel_2 = 45.5*RAD_PER_DEG;
      }
   }

   update();
   
   return flag;
}


/***************************************************************************/
/*
 *                              FUNCTIONS
 */
long ossimAlbersProjection::Set_Albers_Parameters(double a,
                                                  double f,
                                                  double Origin_Latitude,
                                                  double Central_Meridian,
                                                  double Std_Parallel_1,
                                                  double Std_Parallel_2,
                                                  double False_Easting,
                                                  double False_Northing)

{
/*
 * The function Set_Albers_Parameters receives the ellipsoid parameters and
 * projection parameters as inputs, and sets the corresponding state
 * variables.  If any errors occur, the error code(s) are returned by the function, 
 * otherwise ALBERS_NO_ERROR is returned.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters   (input)
 *    f                 : Flattening of ellipsoid                   (input)
 *    Origin_Latitude   : Latitude in radians at which the          (input)
 *                          point scale factor is 1.0
 *    Central_Meridian  : Longitude in radians at the center of     (input)
 *                          the projection
 *    Std_Parallel_1    : First standard parallel                   (input)
 *    Std_Parallel_2    : Second standard parallel                  (input)
 *    False_Easting     : A coordinate value in meters assigned to the
 *                          central meridian of the projection.     (input)
 *    False_Northing    : A coordinate value in meters assigned to the
 *                          origin latitude of the projection       (input)
 */

  double sin_lat, sin_lat_1, cos_lat;
  double m1, m2, SQRm1;
  double q0, q1, q2;
  double es_sin, one_MINUS_SQRes_sin;
  double nq0;
  double inv_f = 1 / f;
  long Error_Code = ALBERS_NO_ERROR;

  if (a <= 0.0)
  { /* Semi-major axis must be greater than zero */
    Error_Code |= ALBERS_A_ERROR;
  }
  if ((inv_f < 250) || (inv_f > 350))
  { /* Inverse flattening must be between 250 and 350 */
    Error_Code |= ALBERS_INV_F_ERROR;
  }
  if ((Origin_Latitude < -PI_OVER_2) || (Origin_Latitude > PI_OVER_2))
  { /* origin latitude out of range */
    Error_Code |= ALBERS_ORIGIN_LAT_ERROR;
  }
  if ((Central_Meridian < -M_PI) || (Central_Meridian > TWO_PI))
  { /* origin longitude out of range */
    Error_Code |= ALBERS_CENT_MER_ERROR;
  }
  if ((Std_Parallel_1 < -PI_OVER_2) || (Std_Parallel_1 > PI_OVER_2))
  { /* First Standard Parallel out of range */
    Error_Code |= ALBERS_FIRST_STDP_ERROR;
  }
  if ((Std_Parallel_2 < -PI_OVER_2) || (Std_Parallel_2 > PI_OVER_2))
  { /* Second Standard Parallel out of range */
    Error_Code |= ALBERS_SECOND_STDP_ERROR;
  }
  if ((Std_Parallel_1 == 0.0) && (Std_Parallel_2 == 0.0))
  { /* First & Second Standard Parallels equal 0 */
    Error_Code |= ALBERS_FIRST_SECOND_ERROR;
  }
  if (Std_Parallel_1 == -Std_Parallel_2)
  { /* Parallels are opposite latitudes */
    Error_Code |= ALBERS_HEMISPHERE_ERROR;
  }

  if (!Error_Code)
  { /* no errors */
    Albers_a = a;
    Albers_f = f;
    Albers_Origin_Lat = Origin_Latitude;
    Albers_Std_Parallel_1 = Std_Parallel_1;
    Albers_Std_Parallel_2 = Std_Parallel_2;
    if (Central_Meridian > M_PI)
      Central_Meridian -= TWO_PI;
    Albers_Origin_Long = Central_Meridian;
    Albers_False_Easting = False_Easting;
    Albers_False_Northing = False_Northing;

    es2 = 2 * Albers_f - Albers_f * Albers_f;
    es = sqrt(es2);
    one_MINUS_es2 = 1 - es2;
    two_es = 2 * es;

    sin_lat = sin(Albers_Origin_Lat);
    es_sin = ES_SIN(sin_lat);
    one_MINUS_SQRes_sin = ONE_MINUS_SQR(es_sin);
    q0 = ALBERS_Q(sin_lat, one_MINUS_SQRes_sin, es_sin);

    sin_lat_1 = sin(Albers_Std_Parallel_1);
    cos_lat = cos(Albers_Std_Parallel_1);
    es_sin = ES_SIN(sin_lat_1);
    one_MINUS_SQRes_sin = ONE_MINUS_SQR(es_sin);
    m1 = ALBERS_M(cos_lat, one_MINUS_SQRes_sin);
    q1 = ALBERS_Q(sin_lat_1, one_MINUS_SQRes_sin, es_sin);

    SQRm1 = m1 * m1;
    if (fabs(Albers_Std_Parallel_1 - Albers_Std_Parallel_2) > 1.0e-10)
    {
      sin_lat = sin(Albers_Std_Parallel_2);
      cos_lat = cos(Albers_Std_Parallel_2);
      es_sin = ES_SIN(sin_lat);
      one_MINUS_SQRes_sin = ONE_MINUS_SQR(es_sin);
      m2 = ALBERS_M(cos_lat, one_MINUS_SQRes_sin);
      q2 = ALBERS_Q(sin_lat, one_MINUS_SQRes_sin, es_sin);
      n = (SQRm1 - m2 * m2) / (q2 - q1);
    }
    else
      n = sin_lat_1;

    C = SQRm1 + n * q1;
    Albers_a_OVER_n = Albers_a / n;
    nq0 = n * q0;
    if (C < nq0)
      rho0 = 0;
    else
      rho0 = Albers_a_OVER_n * sqrt(C - nq0);


  } /* END OF if(!Error_Code) */
  return (Error_Code);
} /* END OF Set_Albers_Parameters */


void ossimAlbersProjection::Get_Albers_Parameters(double *a,
                                                  double *f,
                                                  double *Origin_Latitude,
                                                  double *Central_Meridian,
                                                  double *Std_Parallel_1,
                                                  double *Std_Parallel_2,
                                                  double *False_Easting,
                                                  double *False_Northing)const

{ /* BEGIN Get_Albers_Parameters */
/*
 * The function Get_Albers_Parameters returns the current ellipsoid
 * parameters, and Albers projection parameters.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters   (output)
 *    f                 : Flattening of ellipsoid										(output)
 *    Origin_Latitude   : Latitude in radians at which the          (output)
 *                          point scale factor is 1.0
 *    Origin_Longitude  : Longitude in radians at the center of     (output)
 *                          the projection
 *    Std_Parallel_1    : First standard parallel                   (output)
 *    Std_Parallel_2    : Second standard parallel                  (output)
 *    False_Easting     : A coordinate value in meters assigned to the
 *                          central meridian of the projection.     (output)
 *    False_Northing    : A coordinate value in meters assigned to the
 *                          origin latitude of the projection       (output)
 */

  *a = Albers_a;
  *f = Albers_f;
  *Origin_Latitude = Albers_Origin_Lat;
  *Std_Parallel_1 = Albers_Std_Parallel_1;
  *Std_Parallel_2 = Albers_Std_Parallel_2;
  *Central_Meridian = Albers_Origin_Long;
  *False_Easting = Albers_False_Easting;
  *False_Northing = Albers_False_Northing;
  return;
} /* END OF Get_Albers_Parameters */


long ossimAlbersProjection::Convert_Geodetic_To_Albers (double Latitude,
                                                        double Longitude,
                                                        double *Easting,
                                                        double *Northing)const

{ /* BEGIN Convert_Geodetic_To_Albers */
/*
 * The function Convert_Geodetic_To_Albers converts geodetic (latitude and
 * longitude) coordinates to Albers projection (easting and northing)
 * coordinates, according to the current ellipsoid and Albers projection
 * parameters.  If any errors occur, the error code(s) are returned by the
 * function, otherwise ALBERS_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi) in radians           (input)
 *    Longitude         : Longitude (lambda) in radians       (input)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */

  double dlam;                      /* Longitude - Central Meridan */
  double sin_lat, cos_lat;
  double es_sin, one_MINUS_SQRes_sin;
  double q;
  double rho;
  double theta;
  double nq;
  long Error_Code = ALBERS_NO_ERROR;

  if ((Latitude < -PI_OVER_2) || (Latitude > PI_OVER_2))
  {  /* Latitude out of range */
    Error_Code |= ALBERS_LAT_ERROR;
  }
  if ((Longitude < -M_PI) || (Longitude > TWO_PI))
  {  /* Longitude out of range */
    Error_Code|= ALBERS_LON_ERROR;
  }

  if (!Error_Code)
  { /* no errors */

    dlam = Longitude - Albers_Origin_Long;
    if (dlam > M_PI)
    {
      dlam -= TWO_PI;
    }
    if (dlam < -M_PI)
    {
      dlam += TWO_PI;
    }
    sin_lat = sin(Latitude);
    cos_lat = cos(Latitude);
    es_sin = ES_SIN(sin_lat);
    one_MINUS_SQRes_sin = ONE_MINUS_SQR(es_sin);
    q = ALBERS_Q(sin_lat, one_MINUS_SQRes_sin, es_sin);
    nq = n * q;
    if (C < nq)
      rho = 0;
    else
      rho = Albers_a_OVER_n * sqrt(C - nq);


    theta = n * dlam;
    *Easting = rho * sin(theta) + Albers_False_Easting;
    *Northing = rho0 - rho * cos(theta) + Albers_False_Northing;
  }
  return (Error_Code);
} /* END OF Convert_Geodetic_To_Albers */


long ossimAlbersProjection::Convert_Albers_To_Geodetic(double Easting,
                                                       double Northing,
                                                       double *Latitude,
                                                       double *Longitude)const
{ /* BEGIN Convert_Albers_To_Geodetic */
/*
 * The function Convert_Albers_To_Geodetic converts Albers projection
 * (easting and northing) coordinates to geodetic (latitude and longitude)
 * coordinates, according to the current ellipsoid and Albers projection
 * coordinates.  If any errors occur, the error code(s) are returned by the
 * function, otherwise ALBERS_NO_ERROR is returned.
 *
 *    Easting           : Easting (X) in meters                  (input)
 *    Northing          : Northing (Y) in meters                 (input)
 *    Latitude          : Latitude (phi) in radians              (output)
 *    Longitude         : Longitude (lambda) in radians          (output)
 */

  double dy, dx;
  double rho0_MINUS_dy;
  double q, qconst, q_OVER_2;
  double rho, rho_n;
  double PHI, Delta_PHI = 1.0;
  double sin_phi;
  double es_sin, one_MINUS_SQRes_sin;
  double theta = 0.0;
  double tolerance = 4.85e-10;        /* approximately 1/1000th of
                              an arc second or 1/10th meter */
  long Error_Code = ALBERS_NO_ERROR; 

  if ((Easting < (Albers_False_Easting - Albers_Delta_Easting)) 
      || (Easting > Albers_False_Easting + Albers_Delta_Easting))
  { /* Easting out of range  */
    Error_Code |= ALBERS_EASTING_ERROR;
  }
  if ((Northing < (Albers_False_Northing - Albers_Delta_Northing)) 
      || (Northing > Albers_False_Northing + Albers_Delta_Northing))
  { /* Northing out of range */
    Error_Code |= ALBERS_NORTHING_ERROR;
  }

  if (!Error_Code)
  {
    dy = Northing - Albers_False_Northing;
    dx = Easting - Albers_False_Easting;
    rho0_MINUS_dy = rho0 - dy;
    rho = sqrt(dx * dx + rho0_MINUS_dy * rho0_MINUS_dy);

    if (n < 0)
    {
      rho *= -1.0;
      dy *= -1.0;
      dx *= -1.0;
      rho0_MINUS_dy *= -1.0;
    }

    if (rho != 0.0)
      theta = atan2(dx, rho0_MINUS_dy);
    rho_n = rho * n;
    q = (C - (rho_n * rho_n) / (Albers_a * Albers_a)) / n;
    qconst = 1 - ((one_MINUS_es2) / (two_es)) * log((1.0 - es) / (1.0 + es));
    if (fabs(fabs(qconst) - fabs(q)) > 1.0e-6)
    {
      q_OVER_2 = q / 2.0;
      if (q_OVER_2 > 1.0)
        *Latitude = PI_OVER_2;
      else if (q_OVER_2 < -1.0)
        *Latitude = -PI_OVER_2;
      else
      {
        PHI = asin(q_OVER_2);
        if (es < 1.0e-10)
          *Latitude = PHI;
        else
        {
          while (fabs(Delta_PHI) > tolerance)
          {
            sin_phi = sin(PHI);
            es_sin = ES_SIN(sin_phi);
            one_MINUS_SQRes_sin = ONE_MINUS_SQR(es_sin);
            Delta_PHI = (one_MINUS_SQRes_sin * one_MINUS_SQRes_sin) / (2.0 * cos(PHI)) *
                        (q / (one_MINUS_es2) - sin_phi / one_MINUS_SQRes_sin +
                         (log((1.0 - es_sin) / (1.0 + es_sin)) / (two_es)));
            PHI += Delta_PHI;
          }
          *Latitude = PHI;
        }

        if (*Latitude > PI_OVER_2)  /* force distorted values to 90, -90 degrees */
          *Latitude = PI_OVER_2;
        else if (*Latitude < -PI_OVER_2)
          *Latitude = -PI_OVER_2;

      }
    }
    else
    {
      if (q >= 0.0)
        *Latitude = PI_OVER_2;
      else
        *Latitude = -PI_OVER_2;
    }
    *Longitude = Albers_Origin_Long + theta / n;

    if (*Longitude > M_PI)
      *Longitude -= TWO_PI;
    if (*Longitude < -M_PI)
      *Longitude += TWO_PI;

    if (*Longitude > M_PI) /* force distorted values to 180, -180 degrees */
      *Longitude = M_PI;
    else if (*Longitude < -M_PI)
      *Longitude = -M_PI;

  }
  return (Error_Code);
} /* END OF Convert_Albers_To_Geodetic */

//*************************************************************************************************
//! Returns TRUE if principal parameters are within epsilon tolerance.
//*************************************************************************************************
bool ossimAlbersProjection::operator==(const ossimProjection& proj) const
{
   if (!ossimMapProjection::operator==(proj)) return false;

   ossimAlbersProjection* p = PTR_CAST(ossimAlbersProjection, &proj);
   if (!p) return false;

   if (!ossim::almostEqual(Albers_Std_Parallel_1,p->Albers_Std_Parallel_1)) return false;
   if (!ossim::almostEqual(Albers_Std_Parallel_2,p->Albers_Std_Parallel_2)) return false;
   if (!ossim::almostEqual(Albers_Delta_Northing,p->Albers_Delta_Northing)) return false;
   if (!ossim::almostEqual(Albers_Delta_Easting, p->Albers_Delta_Easting) ) return false;

   return true;
}
