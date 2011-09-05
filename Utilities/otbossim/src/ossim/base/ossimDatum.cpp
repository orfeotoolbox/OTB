//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
//*******************************************************************
//  $Id: ossimDatum.cpp 19795 2011-06-30 15:04:48Z gpotts $
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimEpsgDatumFactory.h> // For accessing the EPSG codes

RTTI_DEF1(ossimDatum, "ossimDatum", ossimObject);

ossimDatum::ossimDatum(const ossimString &alpha_code, const ossimString &name,
                       const ossimEllipsoid* anEllipsoid,
                       ossim_float64 sigmaX, ossim_float64 sigmaY, ossim_float64 sigmaZ,
                       ossim_float64 westLongitude, ossim_float64 eastLongitude,
                       ossim_float64 southLatitude, ossim_float64 northLatitude)
:
theCode(alpha_code),
theName(name),
theEllipsoid(anEllipsoid),
theSigmaX(sigmaX),
theSigmaY(sigmaY),
theSigmaZ(sigmaZ),
theWestLongitude(westLongitude),
theEastLongitude(eastLongitude),
theSouthLatitude(southLatitude),
theNorthLatitude(northLatitude)
{
   theEpsgCode = ossimEpsgDatumFactory::instance()->findEpsgCode(alpha_code);
};

void ossimDatum::molodenskyShift(double a,
                                 double da,
                                 double f,
                                 double df,
                                 double dx,
                                 double dy,
                                 double dz,
                                 double Lat_in,
                                 double Lon_in,
                                 double Hgt_in,
                                 double &Lat_out,
                                 double &Lon_out,
                                 double &Hgt_out
)const
{
  /* Begin Molodensky_Shift */
  /*
   *  This function shifts geodetic coordinates using the Molodensky method.
   *
   *    a         : Semi-major axis of source ellipsoid in meters  (input)
   *    da        : Destination a minus source a                   (input)
   *    f         : Flattening of source ellipsoid                 (input)
   *    df        : Destination f minus source f                   (input)
   *    dx        : X coordinate shift in meters                   (input)
   *    dy        : Y coordinate shift in meters                   (input)
   *    dz        : Z coordinate shift in meters                   (input)
   *    Lat_in    : Latitude in radians.                           (input)
   *    Lon_in    : Longitude in radians.                          (input)
   *    Hgt_in    : Height in meters.                              (input)
   *    Lat_out   : Calculated latitude in radians.                (output)
   *    Lon_out   : Calculated longitude in radians.               (output)
   *    Hgt_out   : Calculated height in meters.                   (output)
   */
  double tLon_in;   /* temp longitude                                   */
  double e2;        /* Intermediate calculations for dp, dl               */
  double ep2;       /* Intermediate calculations for dp, dl               */
  double sin_Lat;   /* sin(Latitude_1)                                    */
  double sin2_Lat;  /* (sin(Latitude_1))^2                                */
  double sin_Lon;   /* sin(Longitude_1)                                   */
  double cos_Lat;   /* cos(Latitude_1)                                    */
  double cos_Lon;   /* cos(Longitude_1)                                   */
  double w2;        /* Intermediate calculations for dp, dl               */
  double w;         /* Intermediate calculations for dp, dl               */
  double w3;        /* Intermediate calculations for dp, dl               */
  double m;         /* Intermediate calculations for dp, dl               */
  double n;         /* Intermediate calculations for dp, dl               */
  double dp;        /* Delta phi                                          */
  double dp1;       /* Delta phi calculations                             */
  double dp2;       /* Delta phi calculations                             */
  double dp3;       /* Delta phi calculations                             */
  double dl;        /* Delta lambda                                       */
  double dh;        /* Delta height                                       */
  double dh1;       /* Delta height calculations                          */
  double dh2;       /* Delta height calculations                          */

  if(ossim::isnan(Hgt_in))
  {
     Hgt_in = 0.0;
  }
  
  if (Lon_in > M_PI)
    tLon_in = Lon_in - (2*M_PI);
  else
     tLon_in = Lon_in;
  
  e2 = 2 * f - f * f;
  ep2 = e2 / (1 - e2);
  sin_Lat = sin(Lat_in);
  cos_Lat = cos(Lat_in);
  sin_Lon = sin(tLon_in);
  cos_Lon = cos(tLon_in);
  sin2_Lat = sin_Lat * sin_Lat;
  w2 = 1.0 - e2 * sin2_Lat;
  w = sqrt(w2);
  w3 = w * w2;
  m = (a * (1.0 - e2)) / w3;
  n = a / w;
  dp1 = cos_Lat * dz - sin_Lat * cos_Lon * dx - sin_Lat * sin_Lon * dy;
  dp2 = ((e2 * sin_Lat * cos_Lat) / w) * da;
  dp3 = sin_Lat * cos_Lat * (2.0 * n + ep2 * m * sin2_Lat) * (1.0 - f) * df;
  dp = (dp1 + dp2 + dp3) / (m + Hgt_in);
  dl = (-sin_Lon * dx + cos_Lon * dy) / ((n + Hgt_in) * cos_Lat);
  dh1 = (cos_Lat * cos_Lon * dx) + (cos_Lat * sin_Lon * dy) + (sin_Lat * dz);
  dh2 = -(w * da) + ((a * (1 - f)) / w) * sin2_Lat * df;
  dh = dh1 + dh2;

  Lat_out = Lat_in + dp;
  Lon_out = Lon_in + dl;
  Hgt_out = Hgt_in + dh;

  if (Lon_out > (M_PI * 2))
     Lon_out -= 2*M_PI;
  if (Lon_out < (- M_PI))
     Lon_out += 2*M_PI;
}

bool ossimDatum::operator==(const ossimDatum& rhs) const
{
   // This method is complicated by the fact that some datums are represented by a precomputed
   // grid version of the parametric datum.cpp Need to consider these cases. (OLK 02/11)
   if (theCode == "NAR") // This is the code for gridded NADCON datum
   {
      if (!rhs.theCode.contains("NAR"))
         return false;
   }
   else if (rhs.theCode == "NAR")
   {
      if (!theCode.contains("NAR"))
         return false;
   }
   else if (theCode == "NAS")
   {
      if (!rhs.theCode.contains("NAS"))
         return false;
   }
   else if (rhs.theCode == "NAS")
   {
      if (!theCode.contains("NAS"))
         return false;
   }

   // Not a grid, so codes must match:
   else if (theCode != rhs.theCode)
      return false;

   // Additional EPSG code check after HARN v. parametric caused false positives (OLK 05/11):
   if ((theEpsgCode != 0) && (rhs.theEpsgCode !=0) && (theEpsgCode != rhs.theEpsgCode))
      return false;

   // Don't need to check the name since 1:1 relationship with code, code check is sufficient.
   // This saves the tedium of filtering for NADCON datums as done above.
   return ((*theEllipsoid == *rhs.theEllipsoid)&&
      (theSigmaX == rhs.theSigmaX)&&
      (theSigmaY == rhs.theSigmaY)&&
      (theSigmaZ == rhs.theSigmaZ)&&
      (theWestLongitude == rhs.theWestLongitude)&&
      (theEastLongitude == rhs.theEastLongitude)&&
      (theSouthLatitude == rhs.theSouthLatitude)&&
      (theNorthLatitude == rhs.theNorthLatitude));
}

bool ossimDatum::isEqualTo(const ossimObject& obj, ossimCompareType compareType)const
{
   const ossimDatum* rhs = dynamic_cast<const ossimDatum*> (&obj);
   bool result = rhs&&ossimObject::isEqualTo(obj, compareType);
   if(result)
   {
      result = ((theCode == rhs->theCode)&&
                (theEpsgCode == rhs->theEpsgCode)&&
                (theName == rhs->theName)&&
                (ossim::almostEqual(theSigmaX, rhs->theSigmaX))&&
                (ossim::almostEqual(theSigmaY, rhs->theSigmaY))&&
                (ossim::almostEqual(theSigmaZ, rhs->theSigmaZ))&&
                (ossim::almostEqual(theWestLongitude, rhs->theWestLongitude))&&
                (ossim::almostEqual(theEastLongitude, rhs->theEastLongitude))&&
                (ossim::almostEqual(theSouthLatitude, rhs->theSouthLatitude))&&
                (ossim::almostEqual(theNorthLatitude, rhs->theNorthLatitude)));
      
      if(result)
      {
         if(theEllipsoid&&rhs->theEllipsoid)
         {
            if(compareType == OSSIM_COMPARE_FULL)
            {
               result = theEllipsoid->isEqualTo(*rhs->theEllipsoid, compareType);
            }
            else 
            {
               result = theEllipsoid == rhs->theEllipsoid;
            }
         }
         else if(reinterpret_cast<ossim_uint64>(theEllipsoid) | reinterpret_cast<ossim_uint64>(rhs->theEllipsoid)) // one is null
         {
            result = false;
         }
      }
   }
   return result;
}
