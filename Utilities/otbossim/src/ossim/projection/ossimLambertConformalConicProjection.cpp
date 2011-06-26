//*******************************************************************
//
// License:  See top LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Calls Lamberts Conformal Conic projection code.  
//*******************************************************************
//  $Id: ossimLambertConformalConicProjection.cpp 19640 2011-05-25 15:58:00Z oscarkramer $

#include <iostream>
#include <iomanip>
#include <ossim/projection/ossimLambertConformalConicProjection.h>
#include <ossim/base/ossimKeywordNames.h>

RTTI_DEF1(ossimLambertConformalConicProjection, "ossimLambertConformalConicProjection", ossimMapProjection)

/***************************************************************************/
/*                               DEFINES
 *
 */
#ifndef PI_OVER_2
#  define PI_OVER_2  ( M_PI / 2.0)
#endif
#ifndef TWO_PI
#  define TWO_PI     (2.0 * M_PI)
#endif
#define MAX_LAT    (( M_PI *  89.99972222222222) / 180.0)  /* 89 59 59.0 degrees in radians */
#define LAMBERT_m(clat,essin)                  (clat / sqrt(1.0 - essin * essin))
#define LAMBERT_t(lat,essin)                   tan(PI_OVER_4 - lat / 2) /				\
										            pow((1.0 - essin) / (1.0 + essin), es_OVER_2)
#define ES_SIN(sinlat)                         (es * sinlat)
/**************************************************************************/
/*                               GLOBAL DECLARATIONS
 *
 */

const double PI_OVER_4 = (M_PI / 4.0);

#define LAMBERT_NO_ERROR           0x0000
#define LAMBERT_LAT_ERROR          0x0001
#define LAMBERT_LON_ERROR          0x0002
#define LAMBERT_EASTING_ERROR      0x0004
#define LAMBERT_NORTHING_ERROR     0x0008
#define LAMBERT_FIRST_STDP_ERROR   0x0010
#define LAMBERT_SECOND_STDP_ERROR  0x0020
#define LAMBERT_ORIGIN_LAT_ERROR   0x0040
#define LAMBERT_CENT_MER_ERROR     0x0080
#define LAMBERT_A_ERROR            0x0100
#define LAMBERT_B_ERROR            0x0200
#define LAMBERT_A_LESS_B_ERROR     0x0400
#define LAMBERT_HEMISPHERE_ERROR   0x0800
#define LAMBERT_FIRST_SECOND_ERROR 0x1000

ossimLambertConformalConicProjection::ossimLambertConformalConicProjection(const ossimEllipsoid& ellipsoid,
                                                                           const ossimGpt& origin)
   :ossimMapProjection(ellipsoid, origin)
{
   setDefaults();
   update();
}
    
ossimLambertConformalConicProjection::ossimLambertConformalConicProjection(const ossimEllipsoid& ellipsoid,
                                                                           const ossimGpt& origin,
                                                                           double stdParallel1,
                                                                           double stdParallel2,
                                                                           double falseEasting,
                                                                           double falseNorthing)
   :ossimMapProjection(ellipsoid, origin)
{
   Lambert_Std_Parallel_1 = stdParallel1*RAD_PER_DEG;
   Lambert_Std_Parallel_2 = stdParallel2*RAD_PER_DEG;
   Lambert_False_Easting  = falseEasting;
   Lambert_False_Northing = falseNorthing;
   
   update();
}

ossimLambertConformalConicProjection::~ossimLambertConformalConicProjection()
{
}

ossimObject* ossimLambertConformalConicProjection::dup()const
{
   return new ossimLambertConformalConicProjection(*this);
}

void ossimLambertConformalConicProjection::update()
{   
   Set_Lambert_Parameters(theEllipsoid.getA(),
                          theEllipsoid.getFlattening(),
                          theOrigin.latr(),
                          theOrigin.lonr(),
                          Lambert_Std_Parallel_1,
                          Lambert_Std_Parallel_2,
                          Lambert_False_Easting,
                          Lambert_False_Northing);

   theFalseEastingNorthing.x = Lambert_False_Easting;
   theFalseEastingNorthing.y = Lambert_False_Northing;

   ossimMapProjection::update();
}

void ossimLambertConformalConicProjection::setStandardParallel1(double degree)
{
   Lambert_Std_Parallel_1 = degree*RAD_PER_DEG;
   update();
}

void ossimLambertConformalConicProjection::setStandardParallel2(double degree)
{
   Lambert_Std_Parallel_2 = degree*RAD_PER_DEG;
   update();
}

void ossimLambertConformalConicProjection::setStandardParallels(double parallel1Degree,
                                                                double parallel2Degree)
{
   Lambert_Std_Parallel_1 = parallel1Degree*RAD_PER_DEG;
   Lambert_Std_Parallel_2 = parallel2Degree*RAD_PER_DEG;
   update();
   
}

void ossimLambertConformalConicProjection::setFalseEasting(double falseEasting)
{
   Lambert_False_Easting = falseEasting;
   update();
}

void ossimLambertConformalConicProjection::setFalseNorthing(double falseNorthing)
{
   Lambert_False_Northing = falseNorthing;
   update();
}

void ossimLambertConformalConicProjection::setFalseEastingNorthing(double falseEasting,
                                                                   double falseNorthing)
{
   Lambert_False_Easting = falseEasting;
   Lambert_False_Northing = falseNorthing;  
   update();
}

void ossimLambertConformalConicProjection::setParameters(double parallel1,
                                                         double parallel2,
                                                         double falseEasting,
                                                         double falseNorthing)
{
   Lambert_False_Easting = falseEasting;
   Lambert_False_Northing = falseNorthing;  
   Lambert_Std_Parallel_1 = parallel1*RAD_PER_DEG;
   Lambert_Std_Parallel_2 = parallel2*RAD_PER_DEG;
   update(); 
}


void ossimLambertConformalConicProjection::setDefaults()
{
   Lambert_Std_Parallel_1 = 40*RAD_PER_DEG;
   Lambert_Std_Parallel_2 = 50*RAD_PER_DEG;
   Lambert_False_Northing = 0.0;
   Lambert_False_Easting  = 0.0;
}

ossimGpt ossimLambertConformalConicProjection::inverse(const ossimDpt &eastingNorthing)const
{
   double lat = 0.0;
   double lon = 0.0;
  
   Convert_Lambert_To_Geodetic(eastingNorthing.x,
                               eastingNorthing.y,
                               &lat,
                               &lon);
   
   return ossimGpt(lat*DEG_PER_RAD, lon*DEG_PER_RAD, 0.0, theDatum);
}

ossimDpt ossimLambertConformalConicProjection::forward(const ossimGpt &latLon)const
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

   Convert_Geodetic_To_Lambert(gpt.latr(),
                               gpt.lonr(),
                               &easting,
                               &northing);

   return ossimDpt(easting, northing);
}

bool ossimLambertConformalConicProjection::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   kwl.add(prefix,
           ossimKeywordNames::STD_PARALLEL_1_KW,
           Lambert_Std_Parallel_1*DEG_PER_RAD,
           true);

   kwl.add(prefix,
           ossimKeywordNames::STD_PARALLEL_2_KW,
           Lambert_Std_Parallel_2*DEG_PER_RAD,
           true);
   
   return ossimMapProjection::saveState(kwl, prefix);
}

bool ossimLambertConformalConicProjection::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   bool flag = ossimMapProjection::loadState(kwl, prefix);

   const char* type          = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   const char* stdParallel1  = kwl.find(prefix, ossimKeywordNames::STD_PARALLEL_1_KW);
   const char* stdParallel2  = kwl.find(prefix, ossimKeywordNames::STD_PARALLEL_2_KW);

   setDefaults();
   
   if(ossimString(type) == STATIC_TYPE_NAME(ossimLambertConformalConicProjection))
   {
      Lambert_False_Easting  = theFalseEastingNorthing.x;
      Lambert_False_Northing = theFalseEastingNorthing.y;

      if(stdParallel1)
      {
         Lambert_Std_Parallel_1 = ossimString(stdParallel1).toDouble()*RAD_PER_DEG;
      }
      if(stdParallel2)
      {
         Lambert_Std_Parallel_2 = ossimString(stdParallel2).toDouble()*RAD_PER_DEG;
      }
   }
   update();
   return flag;
}

std::ostream& ossimLambertConformalConicProjection::print(
   std::ostream& out) const
{
   // Capture the original flags.
   std::ios_base::fmtflags f = out.flags();

   out << setiosflags(ios::fixed) << setprecision(15);

   out << "// ossimLambertConformalConicProjection::print\n"
       << ossimKeywordNames::STD_PARALLEL_1_KW << ": "
       << Lambert_Std_Parallel_1*DEG_PER_RAD << "\n"
       << ossimKeywordNames::STD_PARALLEL_2_KW << ": "
      << Lambert_Std_Parallel_2*DEG_PER_RAD << std::endl;

   // Reset flags.
   out.setf(f);
   
   return ossimMapProjection::print(out);
}

/************************************************************************/
/*                              FUNCTIONS
 *
 */

long ossimLambertConformalConicProjection::Set_Lambert_Parameters(double a,
                                                                  double f,
                                                                  double Origin_Latitude,
                                                                  double Central_Meridian,
                                                                  double Std_Parallel_1,
                                                                  double Std_Parallel_2,
                                                                  double False_Easting,
                                                                  double False_Northing)

{ /* BEGIN Set_Lambert_Parameters */
/*
 * The function Set_Lambert_Parameters receives the ellipsoid parameters and
 * Lambert Conformal Conic projection parameters as inputs, and sets the
 * corresponding state variables.  If any errors occur, the error code(s)
 * are returned by the function, otherwise LAMBERT_NO_ERROR is returned.
 *
 *   a                   : Semi-major axis of ellipsoid, in meters   (input)
 *   f                   : Flattening of ellipsoid						       (input)
 *   Origin_Latitude     : Latitude of origin, in radians            (input)
 *   Central_Meridian    : Longitude of origin, in radians           (input)
 *   Std_Parallel_1      : First standard parallel, in radians       (input)
 *   Std_Parallel_2      : Second standard parallel, in radians      (input)
 *   False_Easting       : False easting, in meters                  (input)
 *   False_Northing      : False northing, in meters                 (input)
 */

  double slat, slat1, clat;
  double es_sin;
  double t0, t1, t2;
  double m1, m2;
//  double inv_f = 1 / f;
  long Error_Code = LAMBERT_NO_ERROR;

//   if (a <= 0.0)
//   { /* Semi-major axis must be greater than zero */
//     Error_Code |= LAMBERT_A_ERROR;
//   }
//   if ((inv_f < 250) || (inv_f > 350))
//   { /* Inverse flattening must be between 250 and 350 */
//     Error_Code |= LAMBERT_INV_F_ERROR;
//   }
//   if ((Origin_Latitude < -MAX_LAT) || (Origin_Latitude > MAX_LAT))
//   { /* Origin Latitude out of range */
//     Error_Code |= LAMBERT_ORIGIN_LAT_ERROR;
//   }
//   if ((Std_Parallel_1 < -MAX_LAT) || (Std_Parallel_1 > MAX_LAT))
//   { /* First Standard Parallel out of range */
//     Error_Code |= LAMBERT_FIRST_STDP_ERROR;
//   }
//   if ((Std_Parallel_2 < -MAX_LAT) || (Std_Parallel_2 > MAX_LAT))
//   { /* Second Standard Parallel out of range */
//     Error_Code |= LAMBERT_SECOND_STDP_ERROR;
//   }
//   if ((Std_Parallel_1 == 0) && (Std_Parallel_2 == 0))
//   { /* First & Second Standard Parallels are both 0 */
//     Error_Code |= LAMBERT_FIRST_SECOND_ERROR;
//   }
//   if (Std_Parallel_1 == -Std_Parallel_2)
//   { /* Parallels are the negation of each other */
//     Error_Code |= LAMBERT_HEMISPHERE_ERROR;
//   }
//   if ((Central_Meridian < -PI) || (Central_Meridian > TWO_PI))
//   { /* Origin Longitude out of range */
//     Error_Code |= LAMBERT_CENT_MER_ERROR;
//   }

  if (!Error_Code)
  { /* no errors */

    Lambert_a = a;
    Lambert_f = f;
    Lambert_Origin_Lat = Origin_Latitude;
    Lambert_Std_Parallel_1 = Std_Parallel_1;
    Lambert_Std_Parallel_2 = Std_Parallel_2;
//     if (Central_Meridian > PI)
//       Central_Meridian -= TWO_PI;
    Lambert_Origin_Long = Central_Meridian;
    Lambert_False_Easting = False_Easting;
    Lambert_False_Northing = False_Northing;

    es2 = 2 * Lambert_f - Lambert_f * Lambert_f;
    es = sqrt(es2);
    es_OVER_2 = es / 2.0;

    slat = sin(Lambert_Origin_Lat);
    es_sin = ES_SIN(slat);
    t0 = LAMBERT_t(Lambert_Origin_Lat, es_sin);

    slat1 = sin(Lambert_Std_Parallel_1);
    clat = cos(Lambert_Std_Parallel_1);
    es_sin = ES_SIN(slat1);
    m1 = LAMBERT_m(clat, es_sin);
    t1 = LAMBERT_t(Lambert_Std_Parallel_1, es_sin);


    if (fabs(Lambert_Std_Parallel_1 - Lambert_Std_Parallel_2) > 1.0e-10)
    {
      slat = sin(Lambert_Std_Parallel_2);
      clat = cos(Lambert_Std_Parallel_2);
      es_sin = ES_SIN(slat);
      m2 = LAMBERT_m(clat, es_sin);
      t2 = LAMBERT_t(Lambert_Std_Parallel_2, es_sin);
      n = log(m1 / m2) / log(t1 / t2);
    }
    else
      n = slat1;
    F = m1 / (n * pow(t1, n));
    Lambert_aF = Lambert_a * F;
    if ((t0 == 0) && (n < 0))
      rho0 = 0.0;
    else
      rho0 = Lambert_aF * pow(t0, n);

  }
  return (Error_Code);
} /* END OF Set_Lambert_Parameters */


void ossimLambertConformalConicProjection::Get_Lambert_Parameters(double *a,
                                                                  double *f,
                                                                  double *Origin_Latitude,
                                                                  double *Central_Meridian,
                                                                  double *Std_Parallel_1,
                                                                  double *Std_Parallel_2,
                                                                  double *False_Easting,
                                                                  double *False_Northing)const

{ /* BEGIN Get_Lambert_Parameters */
/*                         
 * The function Get_Lambert_Parameters returns the current ellipsoid
 * parameters and Lambert Conformal Conic projection parameters.
 *
 *   a                   : Semi-major axis of ellipsoid, in meters   (output)
 *   f                   : Flattening of ellipsoid					         (output)
 *   Origin_Latitude     : Latitude of origin, in radians            (output)
 *   Central_Meridian    : Longitude of origin, in radians           (output)
 *   Std_Parallel_1      : First standard parallel, in radians       (output)
 *   Std_Parallel_2      : Second standard parallel, in radians      (output)
 *   False_Easting       : False easting, in meters                  (output)
 *   False_Northing      : False northing, in meters                 (output)
 */


  *a = Lambert_a;
  *f = Lambert_f;
  *Std_Parallel_1 = Lambert_Std_Parallel_1;
  *Std_Parallel_2 = Lambert_Std_Parallel_2;
  *Origin_Latitude = Lambert_Origin_Lat;
  *Central_Meridian = Lambert_Origin_Long;
  *False_Easting = Lambert_False_Easting;
  *False_Northing = Lambert_False_Northing;
  
  return;
} /* END OF Get_Lambert_Parameters */


long ossimLambertConformalConicProjection::Convert_Geodetic_To_Lambert (double Latitude,
                                                                        double Longitude,
                                                                        double *Easting,
                                                                        double *Northing)const
   
{ /* BEGIN Convert_Geodetic_To_Lambert */
/*
 * The function Convert_Geodetic_To_Lambert converts Geodetic (latitude and
 * longitude) coordinates to Lambert Conformal Conic projection (easting
 * and northing) coordinates, according to the current ellipsoid and
 * Lambert Conformal Conic projection parameters.  If any errors occur, the
 * error code(s) are returned by the function, otherwise LAMBERT_NO_ERROR is
 * returned.
 *
 *    Latitude         : Latitude, in radians                         (input)
 *    Longitude        : Longitude, in radians                        (input)
 *    Easting          : Easting (X), in meters                       (output)
 *    Northing         : Northing (Y), in meters                      (output)
 */

  double slat;
  double es_sin;
  double t;
  double rho;
  double dlam;
  double theta;
  long  Error_Code = LAMBERT_NO_ERROR;

//   if ((Latitude < -PI_OVER_2) || (Latitude > PI_OVER_2))
//   {  /* Latitude out of range */
//     Error_Code|= LAMBERT_LAT_ERROR;
//   }
//   if ((Longitude < -PI) || (Longitude > TWO_PI))
//   {  /* Longitude out of range */
//     Error_Code|= LAMBERT_LON_ERROR;
//   }

  if (!Error_Code)
  { /* no errors */

    if (fabs(fabs(Latitude) - PI_OVER_2) > 1.0e-10)
    {
      slat = sin(Latitude);
      es_sin = ES_SIN(slat);
      t = LAMBERT_t(Latitude, es_sin);
      rho = Lambert_aF * pow(t, n);
    }
    else
    {
      if ((Latitude * n) <= 0)
      { /* Point can not be projected */
        Error_Code |= LAMBERT_LAT_ERROR;
        return (Error_Code);
      }
      rho = 0.0;
    }

    dlam = Longitude - Lambert_Origin_Long;

//     if (dlam > PI)
//     {
//       dlam -= TWO_PI;
//     }
//     if (dlam < -PI)
//     {
//       dlam += TWO_PI;
//     }

    theta = n * dlam;

    *Easting = rho * sin(theta) + Lambert_False_Easting;
    *Northing = rho0 - rho * cos(theta) + Lambert_False_Northing;

  }
  return (Error_Code);
} /* END OF Convert_Geodetic_To_Lambert */



long ossimLambertConformalConicProjection::Convert_Lambert_To_Geodetic (double Easting,
                                                                        double Northing,
                                                                        double *Latitude,
                                                                        double *Longitude)const

{ /* BEGIN Convert_Lambert_To_Geodetic */
/*
 * The function Convert_Lambert_To_Geodetic converts Lambert Conformal
 * Conic projection (easting and northing) coordinates to Geodetic
 * (latitude and longitude) coordinates, according to the current ellipsoid
 * and Lambert Conformal Conic projection parameters.  If any errors occur,
 * the error code(s) are returned by the function, otherwise LAMBERT_NO_ERROR
 * is returned.
 *
 *    Easting          : Easting (X), in meters                       (input)
 *    Northing         : Northing (Y), in meters                      (input)
 *    Latitude         : Latitude, in radians                         (output)
 *    Longitude        : Longitude, in radians                        (output)
 */


  double dy, dx;
  double rho, rho0_MINUS_dy;
  double t;
  double PHI;
  double tempPHI = 0.0;
  double sin_PHI;
  double es_sin;
  double theta = 0.0;
  double tolerance = 4.85e-10;
  long Error_Code = LAMBERT_NO_ERROR;

//   if ((Easting < (Lambert_False_Easting - Lambert_Delta_Easting))
//       ||(Easting > (Lambert_False_Easting + Lambert_Delta_Easting)))
//   { /* Easting out of range  */
//     Error_Code |= LAMBERT_EASTING_ERROR;
//   }
//   if ((Northing < (Lambert_False_Northing - Lambert_Delta_Northing))
//       || (Northing > (Lambert_False_Northing + Lambert_Delta_Northing)))
//   { /* Northing out of range */
//     Error_Code |= LAMBERT_NORTHING_ERROR;
//   }

  if (!Error_Code)
  { /* no errors */

    dy = Northing - Lambert_False_Northing;
    dx = Easting - Lambert_False_Easting;
    rho0_MINUS_dy = rho0 - dy;
    rho = sqrt(dx * dx + (rho0_MINUS_dy) * (rho0_MINUS_dy));

    if (n < 0.0)
    {
      rho *= -1.0;
      dy *= -1.0;
      dx *= -1.0;
      rho0_MINUS_dy *= -1.0;
    }

    if (rho != 0.0)
    {
      theta = atan2(dx, rho0_MINUS_dy);
      t = pow(rho / (Lambert_aF) , 1.0 / n);
      PHI = PI_OVER_2 - 2.0 * atan(t);
      while (fabs(PHI - tempPHI) > tolerance)
      {
        tempPHI = PHI;
        sin_PHI = sin(PHI);
        es_sin = ES_SIN(sin_PHI);
        PHI = PI_OVER_2 - 2.0 * atan(t * pow((1.0 - es_sin) / (1.0 + es_sin), es_OVER_2));
      }
      *Latitude = PHI;
      *Longitude = theta / n + Lambert_Origin_Long;

      if (fabs(*Latitude) < 2.0e-7)  /* force lat to 0 to avoid -0 degrees */
        *Latitude = 0.0;
      if (*Latitude > PI_OVER_2)  /* force distorted values to 90, -90 degrees */
        *Latitude = PI_OVER_2;
      else if (*Latitude < -PI_OVER_2)
        *Latitude = -PI_OVER_2;

      if (*Longitude > M_PI)
      {
        if (*Longitude - M_PI < 3.5e-6) 
          *Longitude = M_PI;
//         else
//           *Longitude -= TWO_PI;
      }
      if (*Longitude < -M_PI)
      {
        if (fabs(*Longitude + M_PI) < 3.5e-6)
          *Longitude = -M_PI;
//         else
//           *Longitude += TWO_PI;
      }

      if (fabs(*Longitude) < 2.0e-7)  /* force lon to 0 to avoid -0 degrees */
        *Longitude = 0.0;
      if (*Longitude > M_PI)  /* force distorted values to 180, -180 degrees */
        *Longitude = M_PI;
      else if (*Longitude < -M_PI)
        *Longitude = -M_PI;
    }
    else
    {
      if (n > 0.0)
        *Latitude = PI_OVER_2;
      else
        *Latitude = -PI_OVER_2;
      *Longitude = Lambert_Origin_Long;
    }
  }
  return (Error_Code);
} /* END OF Convert_Lambert_To_Geodetic */

double ossimLambertConformalConicProjection::getStandardParallel1()const
{
   return  Lambert_Std_Parallel_1/RAD_PER_DEG;
}

double ossimLambertConformalConicProjection::getStandardParallel2()const
{
   return  Lambert_Std_Parallel_2/RAD_PER_DEG;
}

//*************************************************************************************************
//! Returns TRUE if principal parameters are within epsilon tolerance.
//*************************************************************************************************
bool ossimLambertConformalConicProjection::operator==(const ossimProjection& proj) const
{
   if (!ossimMapProjection::operator==(proj)) 
      return false;

   ossimLambertConformalConicProjection* p = PTR_CAST(ossimLambertConformalConicProjection, &proj);
   if (!p) return false;

   if (!ossim::almostEqual(Lambert_Std_Parallel_1,p->Lambert_Std_Parallel_1)) return false;
   if (!ossim::almostEqual(Lambert_Std_Parallel_2,p->Lambert_Std_Parallel_2)) return false;

   return true;
}
