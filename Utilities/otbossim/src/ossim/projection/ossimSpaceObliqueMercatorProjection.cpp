//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top LICENSE.txt file.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimSpaceObliqueMercatorProjection.cpp 17815 2010-08-03 13:23:14Z dburken $
#include <ossim/projection/ossimSpaceObliqueMercatorProjection.h>
#include <math.h>
#include <ossim/base/ossimConstants.h>

#define PI_OVER_2 (M_PI/2)
#define PI (M_PI)
#define TWOPI (2*M_PI)
#define HALFPI (M_PI/2.0)
#define TOL 1e-7
#define PI_HALFPI 4.71238898038468985766
#define TWOPI_HALFPI 7.85398163397448309610
#define ONE_TOL	 1.00000000000001
#define ATOL 1e-50
#define FORTPI		0.78539816339744833

double aasin(double v)
{
   double av;

   if ((av = fabs(v)) >= 1.)
   {
      return (v < 0. ? -HALFPI : HALFPI);
   }
   return asin(v);
}
double aacos(double v)
{
	double av;

	if ((av = fabs(v)) >= 1.)
        {
           return (v < 0. ? PI : 0.);
	}
	return acos(v);
}
double asqrt(double v)
{
   return ((v <= 0) ? 0. : sqrt(v));
}
double aatan2(double n, double d)
{
   return ((fabs(n) < ATOL && fabs(d) < ATOL) ? 0. : atan2(n,d));
}

static const char* PATH_KW                     = "path";
static const char* SATELLITE_TYPE_KW           = "satellite_type";

#if 0
static const char* ANGLE_OF_INCLINATION_KW     = "angle_of_inclination";
static const char* REVOLUTION_TIME_KW          = "earth_rev_time";
static const char* EARTH_ROTATION_LENGTH_KW    = "earth_rot_length";
static const char* LONGITUDE_ASCENDING_NODE_KW = "lon_ascending_node";
static const char* J_COEFFICIENT_KW  = "j_coeff";
static const char* W_COEFFICIENT_KW  = "w_coeff";
static const char* Q_COEFFICIENT_KW  = "q_coeff";
static const char* T_COEFFICIENT_KW  = "t_coeff";
static const char* A2_COEFFICIENT_KW = "a2_coeff";
static const char* A4_COEFFICIENT_KW = "a4_coeff";
static const char* B_COEFFICIENT_KW  = "b_coeff";
static const char* C1_COEFFICIENT_KW = "c1_coeff";
static const char* C3_COEFFICIENT_KW = "c3_coeff";
static const char* ECCENTRICITY_KW   = "eccentricity";
#endif

RTTI_DEF1(ossimSpaceObliqueMercatorProjection, "ossimSpaceObliqueMercatorProjection", ossimMapProjection);


//*****************************************************************************
//  CONSTRUCTOR
//*****************************************************************************
ossimSpaceObliqueMercatorProjection::ossimSpaceObliqueMercatorProjection
   (ossimSatelliteType type,
    double pathNumber,
    const ossimEllipsoid& ellipsoid)
      :
      ossimMapProjection(ellipsoid)
{
   setParameters(type, pathNumber);
}

//*****************************************************************************
//  METHOD
//*****************************************************************************
void ossimSpaceObliqueMercatorProjection::setParameters(ossimSatelliteType type,
                                                        double path)
{
   thePath = path;
   theSatelliteType = type;
   double lam = 0.0;
   double alf = 0.0;
   double esc = 0.0;
   double ess = 0.0;
   
   e  = theEllipsoid.eccentricity();
   es = e*e;
   one_es = 1- es;
   rone_es = 1.0/one_es;
   a   = theEllipsoid.a();
   
   switch(theSatelliteType)
   {
   case SOM_TYPE_LANDSAT_1:
   case SOM_TYPE_LANDSAT_2:
   case SOM_TYPE_LANDSAT_3:
   {
      lam0 = RAD_PER_DEG*(128.87 - (360.0/251.0)*path);
      p22 = 103.2669323;
      alf = RAD_PER_DEG * 99.092;
      break;
   }
   case SOM_TYPE_LANDSAT_4:
   case SOM_TYPE_LANDSAT_5:
   case SOM_TYPE_LANDSAT_7:
   {
      
      lam0 = RAD_PER_DEG*(129.305582487 - (360.0/233.0)*path);
      p22 = 98.8841202;
      alf = RAD_PER_DEG * 98.2;
      break;
   }
   }
   p22 /= 1440.;
   sa = sin(alf);
   ca = cos(alf);
   if (fabs(ca) < 1e-9)
      ca = 1e-9;
   esc = es * ca * ca;
   ess = es * sa * sa;
   w = (1. - esc) * rone_es;
   w = w * w - 1.;
   q = ess * rone_es;
   t = ess * (2. - es) * rone_es * rone_es;
   u = esc * rone_es;
   xj = one_es * one_es * one_es;
   rlm = PI * (1. / 248. + .5161290322580645);
   rlm2 = rlm + TWOPI;
   a2 = a4 = b = c1 = c3 = 0.;
   seraz0(0.0, 1.0);
   for (lam = 9.; lam <= 81.0001; lam += 18.)
      seraz0(lam, 4.0);
   for (lam = 18; lam <= 72.0001; lam += 18.)
      seraz0(lam, 2.0);
   seraz0(90.0, 1.0);
   a2 /= 30.0;
   a4 /= 60.0;
   b /= 30.0;
   c1 /= 15.0;
   c3 /= 45.0;

   update();
}

//*****************************************************************************
//  METHOD
//*****************************************************************************
void ossimSpaceObliqueMercatorProjection::seraz0(double lam,
                                                 double mult)
{
    double sdsq, h, s, fc, sd, sq, d__1;

    lam *= RAD_PER_DEG;
    sd = sin(lam);
    sdsq = sd * sd;
    s = p22 * sa * cos(lam) * sqrt((1. + t * sdsq) / ((
	    1. + w * sdsq) * (1. + q * sdsq)));
    d__1 = 1. + q * sdsq;
    h = sqrt((1. + q * sdsq) / (1. + w * sdsq)) * ((1. + 
	    w * sdsq) / (d__1 * d__1) - p22 * ca);
    sq = sqrt(xj * xj + s * s);
    b += fc = mult * (h * xj - s * s) / sq;
    a2 += fc * cos(lam + lam);
    a4 += fc * cos(lam * 4.);
    fc = mult * s * (h + xj) / sq;
    c1 += fc * cos(lam);
    c3 += fc * cos(lam * 3.);
   
}

//*****************************************************************************
//  METHOD
//*****************************************************************************
ossimDpt
ossimSpaceObliqueMercatorProjection::forward(const ossimGpt &worldPoint) const
{
   ossimGpt gpt = worldPoint;
   
   if (theDatum)
   {
      if (theDatum->code() != worldPoint.datum()->code())
      {
         gpt.changeDatum(theDatum); // Shift to our datum.
      }
   }

   ossimDpt xy;
   
   int l, nn;
   double lamt, xlam, sdsq, c, d, s, lamdp, phidp, lampp, tanph,
      lamtp, cl, sd, sp, fac, sav, tanphi;
   double phi = gpt.latr();
   double lam = gpt.lonr() - lam0;
   
   if (phi > HALFPI)
      phi = HALFPI;
   else if (phi < -HALFPI)
      phi = -HALFPI;
   lampp = phi >= 0. ? HALFPI : PI_HALFPI;
   tanphi = tan(phi);
   for (nn = 0;;)
   {
      sav = lampp;
      lamtp = lam + p22 * lampp;
      cl = cos(lamtp);
      if (fabs(cl) < TOL)
         lamtp -= TOL;
      fac = lampp - sin(lampp) * (cl < 0. ? -HALFPI : HALFPI);
      for (l = 50; l; --l) {
         lamt = lam + p22 * sav;
         if (fabs(c = cos(lamt)) < TOL)
            lamt -= TOL;
         xlam = (one_es * tanphi * sa + sin(lamt) * ca) / c;
         lamdp = atan(xlam) + fac;
         if (fabs(fabs(sav) - fabs(lamdp)) < TOL)
            break;
         sav = lamdp;
      }
      if (!l || ++nn >= 3 || (lamdp > rlm && lamdp < rlm2))
         break;
      if (lamdp <= rlm)
         lampp = TWOPI_HALFPI;
      else if (lamdp >= rlm2)
         lampp = HALFPI;
   }
   if (l) {
      sp = sin(phi);
      phidp = aasin((one_es * ca * sp - sa * cos(phi) * 
                     sin(lamt)) / sqrt(1. - es * sp * sp));
      tanph = log(tan(FORTPI + .5 * phidp));
      sd = sin(lamdp);
      sdsq = sd * sd;
      s = p22 * sa * cos(lamdp) * sqrt((1. + t * sdsq)
                                       / ((1. + w * sdsq) * (1. + q * sdsq)));
      d = sqrt(xj * xj + s * s);
      xy.x = b * lamdp + a2 * sin(2. * lamdp) + a4 *
             sin(lamdp * 4.) - tanph * s / d;
      xy.y = c1 * sd + c3 * sin(lamdp * 3.) + tanph * xj / d;
   }
   else
   {
      xy.makeNan();
      return xy;
//       xy.x = xy.y = 1.0/DBL_EPSILON; // st to large number
   }

   xy.x*=a;
   xy.y*=a;
   return xy;
}

//*****************************************************************************
//  METHOD
//*****************************************************************************
ossimGpt
ossimSpaceObliqueMercatorProjection::inverse(const ossimDpt &projectedPoint)
   const
{
   int nn;
   double lamt, sdsq, s, lamdp, phidp, sppsq, dd, sd, sl, fac, scl, sav, spp;
   double lam, phi;
   
   ossimDpt xy = projectedPoint;
   xy.x/=a;
   xy.y/=a;
   
   lamdp = xy.x / b;
   nn = 50;
   do {
      sav = lamdp;
      sd = sin(lamdp);
      sdsq = sd * sd;
      s = p22 * sa * cos(lamdp) * sqrt((1. + t * sdsq)
                                       / ((1. + w * sdsq) * (1. + q * sdsq)));
      lamdp = xy.x + xy.y * s / xj - a2 * sin(
         2. * lamdp) - a4 * sin(lamdp * 4.) - s / xj * (
            c1 * sin(lamdp) + c3 * sin(lamdp * 3.));
      lamdp /= b;
   } while (fabs(lamdp - sav) >= TOL && --nn);
   sl = sin(lamdp);
   fac = exp(sqrt(1. + s * s / xj / xj) * (xy.y - 
                                           c1 * sl - c3 * sin(lamdp * 3.)));
   phidp = 2. * (atan(fac) - FORTPI);
   dd = sl * sl;
   if (fabs(cos(lamdp)) < TOL)
      lamdp -= TOL;
   spp = sin(phidp);
   sppsq = spp * spp;
   lamt = atan(((1. - sppsq * rone_es) * tan(lamdp) * ca -
                spp * sa * sqrt((1. + q * dd) * (1. - sppsq) - sppsq * u) /
                cos(lamdp)) / (1. - sppsq * (1. + u)));
   sl = lamt >= 0. ? 1. : -1.;
   scl = cos(lamdp) >= 0. ? 1. : -1;
   lamt -= HALFPI * (1. - scl) * sl;
   
   lam = lamt - p22 * lamdp;
   if (fabs(sa) < TOL)
      phi = aasin(spp / sqrt(one_es * one_es + es * sppsq));
   else
      phi = atan((tan(lamdp) * cos(lamt) - ca * sin(lamt)) /
                    (one_es * sa));
   return ossimGpt(phi*DEG_PER_RAD,
                   (lam+lam0)*DEG_PER_RAD, 0.0, theDatum);
   
}                        

//*****************************************************************************
//  METHOD
//*****************************************************************************
void ossimSpaceObliqueMercatorProjection::update()
{
//   theMetersPerPixel.x    = 1.0;
//   theMetersPerPixel.y    = 1.0;
   
//   theUlEastingNorthing.x = 0.0;
//   theUlEastingNorthing.y = 0.0;
}


//*****************************************************************************
//  METHOD
//*****************************************************************************
bool ossimSpaceObliqueMercatorProjection::saveState(ossimKeywordlist& kwl,
                                                    const char* prefix) const
{
   kwl.add(prefix,
           PATH_KW,
           thePath,
           true);
   
   kwl.add(prefix,
           SATELLITE_TYPE_KW,
           (int)theSatelliteType,
           true);
   return ossimMapProjection::saveState(kwl, prefix);
}

//*****************************************************************************
//  METHOD
//*****************************************************************************
bool ossimSpaceObliqueMercatorProjection::loadState(const ossimKeywordlist& kwl,
                                                    const char* prefix)
{
   bool result = true;

   const char* path = kwl.find(prefix, PATH_KW);
   const char* type = kwl.find(prefix,  SATELLITE_TYPE_KW);

   if(path)
   {
      thePath = ossimString(path).toDouble();
   }
   else
   {
      result = false;
   }
   if(type)
   {
      theSatelliteType = (ossimSatelliteType)ossimString(type).toInt();
   }
   else
   {
      result = false;
   }

   setParameters(theSatelliteType, thePath);

   if(result)
   {
      result = ossimMapProjection::loadState(kwl, prefix);
   }
   else
   {
      ossimMapProjection::loadState(kwl, prefix);
   }

   return result;
}

//*************************************************************************************************
//! Returns TRUE if principal parameters are within epsilon tolerance.
//*************************************************************************************************
bool ossimSpaceObliqueMercatorProjection::operator==(const ossimProjection& proj) const
{
   if (!ossimMapProjection::operator==(proj))
      return false;

   ossimSpaceObliqueMercatorProjection* p = PTR_CAST(ossimSpaceObliqueMercatorProjection, &proj);
   if (!p) return false;

   if (theSatelliteType != p->theSatelliteType) return false;
   if (!ossim::almostEqual(thePath,p->thePath)) return false;

   return true;
}
