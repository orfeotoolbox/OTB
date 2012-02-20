//*******************************************************************
//
// License:  See top LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Calls Geotrans Utm projection code.  
//*******************************************************************
//  $Id: ossimUtmProjection.cpp 20133 2011-10-12 19:03:47Z oscarkramer $

#include <cstdlib>
#include <cmath>
using namespace std;

#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/projection/ossimEpsgProjectionDatabase.h>

RTTI_DEF1(ossimUtmProjection, "ossimUtmProjection", ossimMapProjection)

/******************************* DEFINES *********************************/
   
#define UTM_NO_ERROR            0x0000
#define UTM_LAT_ERROR           0x0001
#define UTM_LON_ERROR           0x0002
#define UTM_EASTING_ERROR       0x0004
#define UTM_NORTHING_ERROR      0x0008
#define UTM_ZONE_ERROR          0x0010
#define UTM_HEMISPHERE_ERROR    0x0020
#define UTM_ZONE_OVERRIDE_ERROR 0x0040
#define UTM_A_ERROR             0x0080
#define UTM_B_ERROR             0x0100
#define UTM_A_LESS_B_ERROR      0x0200

#define MIN_LAT      ( (-80.5 * M_PI) / 180.0 ) /* -80.5 degrees in radians */
#define MAX_LAT      ( (84.5 * M_PI) / 180.0 )  /* 84.5 degrees in radians  */
#define MAX_DELTA_LONG  ((M_PI * 90.0)/180.0)    /* 90 degrees in radians */
#define MIN_EASTING  100000
#define MAX_EASTING  900000
#define MIN_NORTHING 0
#define MAX_NORTHING 10000000
#define MIN_SCALE_FACTOR  0.3
#define MAX_SCALE_FACTOR  3.0

#define SPHTMD(Latitude) ((double) (theTranMerc_ap * Latitude \
      - theTranMerc_bp * sin(2.e0 * Latitude) + theTranMerc_cp * sin(4.e0 * Latitude) \
      - theTranMerc_dp * sin(6.e0 * Latitude) + theTranMerc_ep * sin(8.e0 * Latitude) ) )

#define SPHSN(Latitude) ((double) (getA() / sqrt( 1.e0 - theTranMerc_es * \
      pow(sin(Latitude), 2))))

#define SPHSR(Latitude) ((double) (getA() * (1.e0 - theTranMerc_es) / \
    pow(DENOM(Latitude), 3)))

#define DENOM(Latitude) ((double) (sqrt(1.e0 - theTranMerc_es * pow(sin(Latitude),2))))


ossimUtmProjection::ossimUtmProjection(const ossimEllipsoid& ellipsoid,
                                       const ossimGpt& origin)
   :
      ossimMapProjection(ellipsoid, origin),
      theTranMerc_a(6378137.0),
      theTranMerc_f(1.0/298.257223563),
      theTranMerc_es(0.0066943799901413800),
      theTranMerc_ebs(0.0067394967565869),
      theTranMerc_Origin_Lat(0.0),
      theTranMerc_Origin_Long(0.0),
      theTranMerc_False_Northing(0.0),
      theTranMerc_False_Easting(500000.0),
      theTranMerc_Scale_Factor(0.9996),
      theTranMerc_ap(6367449.1458008),
      theTranMerc_bp(16038.508696861),
      theTranMerc_cp(16.832613334334),
      theTranMerc_dp(0.021984404273757),
      theTranMerc_Delta_Easting(40000000.0),
      theTranMerc_Delta_Northing(40000000.0),
      theZone(1),
      theHemisphere('N')
      
{
   setZone(origin);
   setHemisphere(origin);
   update();
}

ossimUtmProjection::ossimUtmProjection(const ossimEllipsoid& ellipsoid,
                                       const ossimGpt& origin,
                                       ossim_int32 zone,
                                       char hemisphere)
   :
      ossimMapProjection(ellipsoid, origin),
      theTranMerc_a(6378137.0),
      theTranMerc_f(1.0/298.257223563),
      theTranMerc_es(0.0066943799901413800),
      theTranMerc_ebs(0.0067394967565869),
      theTranMerc_Origin_Lat(0.0),
      theTranMerc_Origin_Long(0.0),
      theTranMerc_False_Northing(0.0),
      theTranMerc_False_Easting(500000.0),
      theTranMerc_Scale_Factor(0.9996),
      theTranMerc_ap(6367449.1458008),
      theTranMerc_bp(16038.508696861),
      theTranMerc_cp(16.832613334334),
      theTranMerc_dp(0.021984404273757),
      theTranMerc_Delta_Easting(40000000.0),
      theTranMerc_Delta_Northing(40000000.0),
      theZone(zone),
      theHemisphere(hemisphere)      
{
   setZone(zone);
   setHemisphere(hemisphere);
   update();
}

ossimUtmProjection::ossimUtmProjection(ossim_int32 zone)
   :
      ossimMapProjection(),
      theTranMerc_a(6378137.0),
      theTranMerc_f(1.0/298.257223563),
      theTranMerc_es(0.0066943799901413800),
      theTranMerc_ebs(0.0067394967565869),
      theTranMerc_Origin_Lat(0.0),
      theTranMerc_Origin_Long(0.0),
      theTranMerc_False_Northing(0.0),
      theTranMerc_False_Easting(500000.0),
      theTranMerc_Scale_Factor(0.9996),
      theTranMerc_ap(6367449.1458008),
      theTranMerc_bp(16038.508696861),
      theTranMerc_cp(16.832613334334),
      theTranMerc_dp(0.021984404273757),
      theTranMerc_Delta_Easting(40000000.0),
      theTranMerc_Delta_Northing(40000000.0),
      theZone(zone),
      theHemisphere('N')      
{
   setZone(zone);
   update();
}

ossimUtmProjection::ossimUtmProjection(const ossimUtmProjection& src)
      :ossimMapProjection(src),
      theTranMerc_a(src.theTranMerc_a),
      theTranMerc_f(src.theTranMerc_f),
      theTranMerc_es(src.theTranMerc_es),
      theTranMerc_ebs(src.theTranMerc_ebs),
      theTranMerc_Origin_Lat(src.theTranMerc_Origin_Lat),
      theTranMerc_Origin_Long(src.theTranMerc_Origin_Long),
      theTranMerc_False_Northing(src.theTranMerc_False_Northing),
      theTranMerc_False_Easting(src.theTranMerc_False_Easting),
      theTranMerc_Scale_Factor(src.theTranMerc_Scale_Factor),
      theTranMerc_ap(src.theTranMerc_ap),
      theTranMerc_bp(src.theTranMerc_bp),
      theTranMerc_cp(src.theTranMerc_cp),
      theTranMerc_dp(src.theTranMerc_dp),
      theTranMerc_Delta_Easting(src.theTranMerc_Delta_Easting),
      theTranMerc_Delta_Northing(src.theTranMerc_Delta_Northing),
      theZone(src.theZone),
      theHemisphere(src.theHemisphere)      
{
   setZone(theZone);
   setHemisphere(theHemisphere);
   update();
}

void ossimUtmProjection::update()
{
   ossimGpt origin = theOrigin;
   origin.lond(computeZoneMeridian(theZone));
   origin.latd(0.0);
   double falseNorthing = 10000000.0;
   if (theHemisphere == 'N')
   {
      falseNorthing = 0.0;
   }
   Set_Transverse_Mercator_Parameters(theEllipsoid.getA(),
                                      theEllipsoid.getFlattening(),
                                      origin.latr(),
                                      origin.lonr(),
                                      theTranMerc_False_Easting,
                                      falseNorthing,
                                      theTranMerc_Scale_Factor);

   theFalseEastingNorthing.x = theTranMerc_False_Easting;
   theFalseEastingNorthing.y = theTranMerc_False_Northing;

   ossimMapProjection::update();
}

ossimGpt ossimUtmProjection::inverse(const ossimDpt &eastingNorthing)const
{
   double lat = 0.0;
   double lon = 0.0;
   
   Convert_Transverse_Mercator_To_Geodetic(eastingNorthing.x,
                                           eastingNorthing.y,
                                           &lat,
                                           &lon);
   
   return ossimGpt(lat*DEG_PER_RAD, lon*DEG_PER_RAD, 0.0, theDatum);
}

ossimDpt ossimUtmProjection::forward(const ossimGpt &latLon)const
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

   Convert_Geodetic_To_Transverse_Mercator(gpt.latr(),
                                           gpt.lonr(),
                                           &easting,
                                           &northing);
   
   return ossimDpt(easting, northing);
}

ossimObject* ossimUtmProjection::dup()const
{
   ossimUtmProjection* proj = new ossimUtmProjection(*this);
   return proj;
}

ossim_int32 ossimUtmProjection::getZone() const
{
   return theZone;
}

char ossimUtmProjection::getHemisphere() const
{
   return theHemisphere;
}

void ossimUtmProjection::setOrigin(const ossimGpt& origin)
{
   setZone(origin);
   // NOTE: We will not set the hemisphere if the origin latitude is 0.0.
   if (origin.latd() != 0.0)
   {
      setHemisphere(origin);
   }
   ossimMapProjection::setOrigin(origin);
}

void ossimUtmProjection::setZone(const ossimGpt& ground)
{
   setZone(computeZone(ground));
}

void ossimUtmProjection::setZone(ossim_int32 zone)
{
   if( (zone < 1) || (zone > 60) )
   {
      theZone = computeZone(theOrigin);
   }
   else
   {
      theZone = zone;
   }
   theOrigin.lond(computeZoneMeridian(theZone));
   theOrigin.latd(0);
   theOrigin.datum(theDatum);
   theTranMerc_Origin_Long = theOrigin.lonr();
}

void ossimUtmProjection::setHemisphere(const ossimGpt& ground)
{
   char hemisphere = ground.latd()<0.0?'S':'N';
   setHemisphere(hemisphere);
}

void ossimUtmProjection::setHemisphere(char hemisphere)
{
   hemisphere = toupper(hemisphere);
   if((hemisphere != 'N') &&
      (hemisphere != 'S'))
   {
      theHemisphere = theOrigin.latd() < 0?'S':'N';
   }
   else
   {
      theHemisphere = hemisphere;
   }

   if (theHemisphere == 'N')
   {
      theTranMerc_False_Northing = 0.0;
   }
   else
   {
      theTranMerc_False_Northing  = 10000000.0;
   }
}

ossim_int32 ossimUtmProjection::computeZone(const ossimGpt& ground)
{
   ossim_int32 result = 0;

   double longitude = ground.lonr();
   double lat_Degrees  = (ossim_int32)( (ground.latd()) + 0.00000005);
   double long_Degrees = (ossim_int32)( (ground.lond()) + 0.00000005);
   
   if (longitude < M_PI)
      result = (ossim_int32)( (31 + ((180 * longitude) / (6 * M_PI)) ) + 0.00000005);
   else
      result = (ossim_int32)( (((180 * longitude) / (6 * M_PI)) - 29) + 0.00000005);
   if (result > 60)
      result = 1;
    /* UTM special cases */
   if ((lat_Degrees > 55) && (lat_Degrees < 64) && (long_Degrees > -1)
       && (long_Degrees < 3))
      result = 31;
   if ((lat_Degrees > 55) && (lat_Degrees < 64) && (long_Degrees > 2)
       && (long_Degrees < 12))
      result = 32;
   if ((lat_Degrees > 71) && (long_Degrees > -1) && (long_Degrees < 9))
      result = 31;
   if ((lat_Degrees > 71) && (long_Degrees > 8) && (long_Degrees < 21))
      result = 33;
   if ((lat_Degrees > 71) && (long_Degrees > 20) && (long_Degrees < 33))
      result = 35;
   if ((lat_Degrees > 71) && (long_Degrees > 32) && (long_Degrees < 42))
      result = 37;

   return result;
}

double ossimUtmProjection::computeZoneMeridian(ossim_int32 zone)
{
   return (6.0 * zone - 183.0);;
}

bool ossimUtmProjection::loadState(const ossimKeywordlist& kwl,
                                   const char* prefix)
{
   const char* zone       = kwl.find(prefix, ossimKeywordNames::ZONE_KW);
   const char* hemisphere = kwl.find(prefix, ossimKeywordNames::HEMISPHERE_KW);

   ossimMapProjection::loadState(kwl, prefix);

   // initialize zone to a dummy value.
   //
   theZone = 0;
//   if(ossimString(type) == STATIC_TYPE_NAME(ossimUtmProjection))
   {
      if(!zone)
      {
         theZone = computeZone(theOrigin);
      }
      else if (zone) 
      {
         theZone = atoi(zone);

         if( (theZone  < 1) || (theZone > 60) )
         {
            theZone = computeZone(theOrigin);
         }
         else
         {
//            if(!kwl.find(prefix, ossimKeywordNames::CENTRAL_MERIDIAN_KW))
//            {
               theOrigin.lond(computeZoneMeridian(theZone));
//            }
//            if(!kwl.find(prefix, ossimKeywordNames::ORIGIN_LATITUDE_KW))
//            {
               theOrigin.latd(0);
//            }
         }
      }
      if (hemisphere)
      {
         ossimString s = hemisphere;
         s = s.trim();
         s = s.upcase();
         
         setHemisphere(*s.c_str());
      }
      else
      {
         char hemisphere = theOrigin.latd()<0?'S':'N';
         setHemisphere(hemisphere);
      }
   }

   update();
   
   return true;
}

bool ossimUtmProjection::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   kwl.add(prefix,
           ossimKeywordNames::ZONE_KW,
           theZone,
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::HEMISPHERE_KW,
           theHemisphere,
           true);
   
   return ossimMapProjection::saveState(kwl, prefix);
}
   
ossim_int32 ossimUtmProjection::Set_Transverse_Mercator_Parameters(double a,
                                                                   double f,
                                                                   double Origin_Latitude,
                                                                   double Central_Meridian,
                                                                   double False_Easting,
                                                                   double False_Northing,
                                                                   double /* Scale_Factor */)

{ /* BEGIN Set_Tranverse_Mercator_Parameters */
  /*
   * The function Set_Tranverse_Mercator_Parameters receives the ellipsoid
   * parameters and Tranverse Mercator projection parameters as inputs, and
   * sets the corresponding state variables. If any errors occur, the error
   * code(s) are returned by the function, otherwise UTM_NO_ERROR is
   * returned.
   *
   *    a                 : Semi-major axis of ellipsoid, in meters    (input)
   *    f                 : Flattening of ellipsoid						         (input)
   *    Origin_Latitude   : Latitude in radians at the origin of the   (input)
   *                         projection
   *    Central_Meridian  : Longitude in radians at the center of the  (input)
   *                         projection
   *    False_Easting     : Easting/X at the center of the projection  (input)
   *    False_Northing    : Northing/Y at the center of the projection (input)
   *    Scale_Factor      : Projection scale factor                    (input) 
   */

  double tn;        /* True Meridianal distance constant  */
  double tn2;
  double tn3;
  double tn4;
  double tn5;
  double dummy_northing;
  double TranMerc_b; /* Semi-minor axis of ellipsoid, in meters */
//  double inv_f = 1 / f;
  ossim_int32 Error_Code = UTM_NO_ERROR;

//   if (a <= 0.0)
//   { /* Semi-major axis must be greater than zero */
//     Error_Code |= UTM_A_ERROR;
//   }
//   if ((inv_f < 250) || (inv_f > 350))
//   { /* Inverse flattening must be between 250 and 350 */
//     Error_Code |= UTM_INV_F_ERROR;
//   }
//   if ((Origin_Latitude < -MAX_LAT) || (Origin_Latitude > MAX_LAT))
//   { /* origin latitude out of range */
//     Error_Code |= UTM_ORIGIN_LAT_ERROR;
//   }
//   if ((Central_Meridian < -M_PI) || (Central_Meridian > TWO_PI))
//   { /* origin longitude out of range */
//     Error_Code |= UTM_CENT_MER_ERROR;
//   }
//   if ((Scale_Factor < MIN_SCALE_FACTOR) || (Scale_Factor > MAX_SCALE_FACTOR))
//   {
//     Error_Code |= UTM_SCALE_FACTOR_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */
    theTranMerc_a = a;
    theTranMerc_f = f;
    theTranMerc_Origin_Lat = 0;
    theTranMerc_Origin_Long = 0;
    theTranMerc_False_Northing = 0;
    theTranMerc_False_Easting = 0; 
    // theTranMerc_Scale_Factor = 1;

    /* Eccentricity Squared */
    theTranMerc_es = 2 * theTranMerc_f - theTranMerc_f * theTranMerc_f;
    /* Second Eccentricity Squared */
    theTranMerc_ebs = (1 / (1 - theTranMerc_es)) - 1;

    TranMerc_b = theTranMerc_a * (1 - theTranMerc_f);    
    /*True meridianal constants  */
    tn = (theTranMerc_a - TranMerc_b) / (theTranMerc_a + TranMerc_b);
    tn2 = tn * tn;
    tn3 = tn2 * tn;
    tn4 = tn3 * tn;
    tn5 = tn4 * tn;

    theTranMerc_ap = theTranMerc_a * (1.e0 - tn + 5.e0 * (tn2 - tn3)/4.e0
                                + 81.e0 * (tn4 - tn5)/64.e0 );
    theTranMerc_bp = 3.e0 * theTranMerc_a * (tn - tn2 + 7.e0 * (tn3 - tn4)
                                       /8.e0 + 55.e0 * tn5/64.e0 )/2.e0;
    theTranMerc_cp = 15.e0 * theTranMerc_a * (tn2 - tn3 + 3.e0 * (tn4 - tn5 )/4.e0) /16.0;
    theTranMerc_dp = 35.e0 * theTranMerc_a * (tn3 - tn4 + 11.e0 * tn5 / 16.e0) / 48.e0;
    theTranMerc_ep = 315.e0 * theTranMerc_a * (tn4 - tn5) / 512.e0;
    Convert_Geodetic_To_Transverse_Mercator(MAX_LAT,
                                            MAX_DELTA_LONG,
                                            &theTranMerc_Delta_Easting,
                                            &theTranMerc_Delta_Northing);
    Convert_Geodetic_To_Transverse_Mercator(0,
                                            MAX_DELTA_LONG,
                                            &theTranMerc_Delta_Easting,
                                            &dummy_northing);
    theTranMerc_Origin_Lat = Origin_Latitude;
    if (Central_Meridian > M_PI)
      Central_Meridian -= TWO_PI;
    theTranMerc_Origin_Long = Central_Meridian;
    theTranMerc_False_Northing = False_Northing;
    theTranMerc_False_Easting = False_Easting; 
    // theTranMerc_Scale_Factor = Scale_Factor;
  } /* END OF if(!Error_Code) */
  return (Error_Code);
}  /* END of Set_Transverse_Mercator_Parameters  */


void ossimUtmProjection::Get_Transverse_Mercator_Parameters(double *a,
                                                                      double *f,
                                                                      double *Origin_Latitude,
                                                                      double *Central_Meridian,
                                                                      double *False_Easting,
                                                                      double *False_Northing,
                                                                      double *Scale_Factor)const

{ /* BEGIN Get_Tranverse_Mercator_Parameters  */
  /*
   * The function Get_Transverse_Mercator_Parameters returns the current
   * ellipsoid and Transverse Mercator projection parameters.
   *
   *    a                 : Semi-major axis of ellipsoid, in meters    (output)
   *    f                 : Flattening of ellipsoid						         (output)
   *    Origin_Latitude   : Latitude in radians at the origin of the   (output)
   *                         projection
   *    Central_Meridian  : Longitude in radians at the center of the  (output)
   *                         projection
   *    False_Easting     : Easting/X at the center of the projection  (output)
   *    False_Northing    : Northing/Y at the center of the projection (output)
   *    Scale_Factor      : Projection scale factor                    (output) 
   */

  *a = theTranMerc_a;
  *f = theTranMerc_f;
  *Origin_Latitude = theTranMerc_Origin_Lat;
  *Central_Meridian = theTranMerc_Origin_Long;
  *False_Easting = theTranMerc_False_Easting;
  *False_Northing = theTranMerc_False_Northing;
  *Scale_Factor = theTranMerc_Scale_Factor;
  
  return;
} /* END OF Get_Tranverse_Mercator_Parameters */



ossim_int32 ossimUtmProjection::Convert_Geodetic_To_Transverse_Mercator (double Latitude,
                                                                            double Longitude,
                                                                            double *Easting,
                                                                            double *Northing)const
   
{      /* BEGIN Convert_Geodetic_To_Transverse_Mercator */

  /*
   * The function Convert_Geodetic_To_Transverse_Mercator converts geodetic
   * (latitude and longitude) coordinates to Transverse Mercator projection
   * (easting and northing) coordinates, according to the current ellipsoid
   * and Transverse Mercator projection coordinates.  If any errors occur, the
   * error code(s) are returned by the function, otherwise UTM_NO_ERROR is
   * returned.
   *
   *    Latitude      : Latitude in radians                         (input)
   *    Longitude     : Longitude in radians                        (input)
   *    Easting       : Easting/X in meters                         (output)
   *    Northing      : Northing/Y in meters                        (output)
   */

  double c;       /* Cosine of latitude                          */
  double c2;
  double c3;
  double c5;
  double c7;
  double dlam;    /* Delta longitude - Difference in Longitude       */
  double eta;     /* constant - theTranMerc_ebs *c *c                   */
  double eta2;
  double eta3;
  double eta4;
  double s;       /* Sine of latitude                        */
  double sn;      /* Radius of curvature in the prime vertical       */
  double t;       /* Tangent of latitude                             */
  double tan2;
  double tan3;
  double tan4;
  double tan5;
  double tan6;
  double t1;      /* Term in coordinate conversion formula - GP to Y */
  double t2;      /* Term in coordinate conversion formula - GP to Y */
  double t3;      /* Term in coordinate conversion formula - GP to Y */
  double t4;      /* Term in coordinate conversion formula - GP to Y */
  double t5;      /* Term in coordinate conversion formula - GP to Y */
  double t6;      /* Term in coordinate conversion formula - GP to Y */
  double t7;      /* Term in coordinate conversion formula - GP to Y */
  double t8;      /* Term in coordinate conversion formula - GP to Y */
  double t9;      /* Term in coordinate conversion formula - GP to Y */
  double tmd;     /* True Meridional distance                        */
  double tmdo;    /* True Meridional distance for latitude of origin */
  ossim_int32    Error_Code = UTM_NO_ERROR;
//  double temp_Origin;
//  double temp_Long;

//   if ((Latitude < -MAX_LAT) || (Latitude > MAX_LAT))
//   {  /* Latitude out of range */
//     Error_Code|= UTM_LAT_ERROR;
//   }
   if (Longitude > M_PI)
     Longitude -= TWO_PI;
//   if ((Longitude < (theTranMerc_Origin_Long - MAX_DELTA_LONG))
//       || (Longitude > (theTranMerc_Origin_Long + MAX_DELTA_LONG)))
//   {
//     if (Longitude < 0)
//       temp_Long = Longitude + TWO_PI;
//     else
//       temp_Long = Longitude;
//     if (theTranMerc_Origin_Long < 0)
//       temp_Origin = theTranMerc_Origin_Long + TWO_PI;
//     else
//       temp_Origin = theTranMerc_Origin_Long;
//     if ((temp_Long < (temp_Origin - MAX_DELTA_LONG))
//         || (temp_Long > (temp_Origin + MAX_DELTA_LONG)))
//       Error_Code|= UTM_LON_ERROR;
//   }
  if (!Error_Code)
  { /* no errors */

    /* 
     *  Delta Longitude
     */
    dlam = Longitude - theTranMerc_Origin_Long;

//     if (fabs(dlam) > (9.0 * M_PI / 180))
//     { /* Distortion will result if Longitude is more than 9 degrees from the Central Meridian */
//       Error_Code |= UTM_LON_WARNING;
//     }

     if (dlam > M_PI)
       dlam -= TWO_PI;
     if (dlam < -M_PI)
       dlam += TWO_PI;
     if (fabs(dlam) < 2.e-10)
       dlam = 0.0;

    s = sin(Latitude);
    c = cos(Latitude);
    c2 = c * c;
    c3 = c2 * c;
    c5 = c3 * c2;
    c7 = c5 * c2;
    t = tan (Latitude);
    tan2 = t * t;
    tan3 = tan2 * t;
    tan4 = tan3 * t;
    tan5 = tan4 * t;
    tan6 = tan5 * t;
    eta = theTranMerc_ebs * c2;
    eta2 = eta * eta;
    eta3 = eta2 * eta;
    eta4 = eta3 * eta;

    /* radius of curvature in prime vertical */
    sn = SPHSN(Latitude);

    /* True Meridianal Distances */
    tmd = SPHTMD(Latitude);

    /*  Origin  */
    tmdo = SPHTMD (theTranMerc_Origin_Lat);

    /* northing */
    t1 = (tmd - tmdo) * theTranMerc_Scale_Factor;
    t2 = sn * s * c * theTranMerc_Scale_Factor/ 2.e0;
    t3 = sn * s * c3 * theTranMerc_Scale_Factor * (5.e0 - tan2 + 9.e0 * eta 
                                                + 4.e0 * eta2) /24.e0; 

    t4 = sn * s * c5 * theTranMerc_Scale_Factor * (61.e0 - 58.e0 * tan2
                                                + tan4 + 270.e0 * eta - 330.e0 * tan2 * eta + 445.e0 * eta2
                                                + 324.e0 * eta3 -680.e0 * tan2 * eta2 + 88.e0 * eta4 
                                                -600.e0 * tan2 * eta3 - 192.e0 * tan2 * eta4) / 720.e0;

    t5 = sn * s * c7 * theTranMerc_Scale_Factor * (1385.e0 - 3111.e0 * 
                                                tan2 + 543.e0 * tan4 - tan6) / 40320.e0;

    *Northing = theTranMerc_False_Northing + t1 + pow(dlam,2.e0) * t2
                + pow(dlam,4.e0) * t3 + pow(dlam,6.e0) * t4
                + pow(dlam,8.e0) * t5; 

    /* Easting */
    t6 = sn * c * theTranMerc_Scale_Factor;
    t7 = sn * c3 * theTranMerc_Scale_Factor * (1.e0 - tan2 + eta ) /6.e0;
    t8 = sn * c5 * theTranMerc_Scale_Factor * (5.e0 - 18.e0 * tan2 + tan4
                                            + 14.e0 * eta - 58.e0 * tan2 * eta + 13.e0 * eta2 + 4.e0 * eta3 
                                            - 64.e0 * tan2 * eta2 - 24.e0 * tan2 * eta3 )/ 120.e0;
    t9 = sn * c7 * theTranMerc_Scale_Factor * ( 61.e0 - 479.e0 * tan2
                                             + 179.e0 * tan4 - tan6 ) /5040.e0;

    *Easting = theTranMerc_False_Easting + dlam * t6 + pow(dlam,3.e0) * t7 
               + pow(dlam,5.e0) * t8 + pow(dlam,7.e0) * t9;
  }
  return (Error_Code);
} /* END OF Convert_Geodetic_To_Transverse_Mercator */


ossim_int32 ossimUtmProjection::Convert_Transverse_Mercator_To_Geodetic (double Easting,
                                                                            double Northing,
                                                                            double *Latitude,
                                                                            double *Longitude)const
{      /* BEGIN Convert_Transverse_Mercator_To_Geodetic */

  /*
   * The function Convert_Transverse_Mercator_To_Geodetic converts Transverse
   * Mercator projection (easting and northing) coordinates to geodetic
   * (latitude and longitude) coordinates, according to the current ellipsoid
   * and Transverse Mercator projection parameters.  If any errors occur, the
   * error code(s) are returned by the function, otherwise UTM_NO_ERROR is
   * returned.
   *
   *    Easting       : Easting/X in meters                         (input)
   *    Northing      : Northing/Y in meters                        (input)
   *    Latitude      : Latitude in radians                         (output)
   *    Longitude     : Longitude in radians                        (output)
   */

  double c;       /* Cosine of latitude                          */
  double de;      /* Delta easting - Difference in Easting (Easting-Fe)    */
  double dlam;    /* Delta longitude - Difference in Longitude       */
  double eta;     /* constant - theTranMerc_ebs *c *c                   */
  double eta2;
  double eta3;
  double eta4;
  double ftphi;   /* Footpoint latitude                              */
  int    i;       /* Loop iterator                   */
  /*double s;        Sine of latitude                        */
  double sn;      /* Radius of curvature in the prime vertical       */
  double sr;      /* Radius of curvature in the meridian             */
  double t;       /* Tangent of latitude                             */
  double tan2;
  double tan4;
  double t10;     /* Term in coordinate conversion formula - GP to Y */
  double t11;     /* Term in coordinate conversion formula - GP to Y */
  double t12;     /* Term in coordinate conversion formula - GP to Y */
  double t13;     /* Term in coordinate conversion formula - GP to Y */
  double t14;     /* Term in coordinate conversion formula - GP to Y */
  double t15;     /* Term in coordinate conversion formula - GP to Y */
  double t16;     /* Term in coordinate conversion formula - GP to Y */
  double t17;     /* Term in coordinate conversion formula - GP to Y */
  double tmd;     /* True Meridional distance                        */
  double tmdo;    /* True Meridional distance for latitude of origin */
  ossim_int32 Error_Code = UTM_NO_ERROR;

//   if ((Easting < (theTranMerc_False_Easting - theTranMerc_Delta_Easting))
//       ||(Easting > (theTranMerc_False_Easting + theTranMerc_Delta_Easting)))
//   { /* Easting out of range  */
//     Error_Code |= UTM_EASTING_ERROR;
//   }
//   if ((Northing < (theTranMerc_False_Northing - theTranMerc_Delta_Northing))
//       || (Northing > (theTranMerc_False_Northing + theTranMerc_Delta_Northing)))
//   { /* Northing out of range */
//     Error_Code |= UTM_NORTHING_ERROR;
//   }

  if (!Error_Code)
  {
    /* True Meridional Distances for latitude of origin */
    tmdo = SPHTMD(theTranMerc_Origin_Lat);

    /*  Origin  */
    tmd = tmdo +  (Northing - theTranMerc_False_Northing) / theTranMerc_Scale_Factor; 

    /* First Estimate */
    sr = SPHSR(0.e0);
    ftphi = tmd/sr;

    for (i = 0; i < 5 ; i++)
    {
      t10 = SPHTMD (ftphi);
      sr = SPHSR(ftphi);
      ftphi = ftphi + (tmd - t10) / sr;
    }

    /* Radius of Curvature in the meridian */
    sr = SPHSR(ftphi);

    /* Radius of Curvature in the meridian */
    sn = SPHSN(ftphi);

    /* Sine Cosine terms */
    // s = sin(ftphi);
    c = cos(ftphi);

    /* Tangent Value  */
    t = tan(ftphi);
    tan2 = t * t;
    tan4 = tan2 * tan2;
    eta = theTranMerc_ebs * pow(c,2);
    eta2 = eta * eta;
    eta3 = eta2 * eta;
    eta4 = eta3 * eta;
    de = Easting - theTranMerc_False_Easting;
    if (fabs(de) < 0.0001)
      de = 0.0;

    /* Latitude */
    t10 = t / (2.e0 * sr * sn * pow(theTranMerc_Scale_Factor, 2));
    t11 = t * (5.e0  + 3.e0 * tan2 + eta - 4.e0 * pow(eta,2)
               - 9.e0 * tan2 * eta) / (24.e0 * sr * pow(sn,3) 
                                       * pow(theTranMerc_Scale_Factor,4));
    t12 = t * (61.e0 + 90.e0 * tan2 + 46.e0 * eta + 45.E0 * tan4
               - 252.e0 * tan2 * eta  - 3.e0 * eta2 + 100.e0 
               * eta3 - 66.e0 * tan2 * eta2 - 90.e0 * tan4
               * eta + 88.e0 * eta4 + 225.e0 * tan4 * eta2
               + 84.e0 * tan2* eta3 - 192.e0 * tan2 * eta4)
          / ( 720.e0 * sr * pow(sn,5) * pow(theTranMerc_Scale_Factor, 6) );
    t13 = t * ( 1385.e0 + 3633.e0 * tan2 + 4095.e0 * tan4 + 1575.e0 
                * pow(t,6))/ (40320.e0 * sr * pow(sn,7) * pow(theTranMerc_Scale_Factor,8));
    *Latitude = ftphi - pow(de,2) * t10 + pow(de,4) * t11 - pow(de,6) * t12 
                + pow(de,8) * t13;

    t14 = 1.e0 / (sn * c * theTranMerc_Scale_Factor);

    t15 = (1.e0 + 2.e0 * tan2 + eta) / (6.e0 * pow(sn,3) * c * 
                                        pow(theTranMerc_Scale_Factor,3));

    t16 = (5.e0 + 6.e0 * eta + 28.e0 * tan2 - 3.e0 * eta2
           + 8.e0 * tan2 * eta + 24.e0 * tan4 - 4.e0 
           * eta3 + 4.e0 * tan2 * eta2 + 24.e0 
           * tan2 * eta3) / (120.e0 * pow(sn,5) * c  
                             * pow(theTranMerc_Scale_Factor,5));

    t17 = (61.e0 +  662.e0 * tan2 + 1320.e0 * tan4 + 720.e0 
           * pow(t,6)) / (5040.e0 * pow(sn,7) * c 
                          * pow(theTranMerc_Scale_Factor,7));

    /* Difference in Longitude */
    dlam = de * t14 - pow(de,3) * t15 + pow(de,5) * t16 - pow(de,7) * t17;

    /* Longitude */
    (*Longitude) = theTranMerc_Origin_Long + dlam;
    while (*Latitude > (90.0 * RAD_PER_DEG))
    {
      *Latitude = M_PI - *Latitude;
      *Longitude += M_PI;
      if (*Longitude > M_PI)
        *Longitude -= TWO_PI;
    }

     while (*Latitude < (-90.0 * RAD_PER_DEG))
     {
       *Latitude = - (*Latitude + M_PI);
       *Longitude += M_PI;
       if (*Longitude > M_PI)
         *Longitude -= TWO_PI;
     }
     if (*Longitude > TWO_PI)
       *Longitude -= TWO_PI;
     if (*Longitude < -M_PI)
       *Longitude += TWO_PI;

//     if (fabs(dlam) > (9.0 * M_PI / 180))
//     { /* Distortion will result if Longitude is more than 9 degrees from the Central Meridian */
//       Error_Code |= UTM_LON_WARNING;
//     }
  }
  return (Error_Code);
} /* END OF Convert_Transverse_Mercator_To_Geodetic */

std::ostream& ossimUtmProjection::print(std::ostream& out) const
{
   out << setiosflags(ios::fixed) << setprecision(15)
       << "// ossimUtmProjection::print"
       << "\ntheZone:  " << theZone
       << "\ntheHemisphere:  " << theHemisphere
       << endl;
   return ossimMapProjection::print(out);
}

double ossimUtmProjection::getFalseEasting() const
{
   return theTranMerc_False_Easting;
}

double ossimUtmProjection::getFalseNorthing() const
{
   return theTranMerc_False_Northing;
}

//*************************************************************************************************
//! Returns TRUE if principal parameters are within epsilon tolerance.
//*************************************************************************************************
bool ossimUtmProjection::operator==(const ossimProjection& proj) const
{
   bool result = false;
   if ( this == &proj )
   {
      result = true; // Pointer addresses the same.
   }
   else
   {
      //---
      // Check our stuff first.  No sense going onto ossimMapProjection::operator==
      // if we are not a utm projection.
      //---
      const ossimUtmProjection* p = dynamic_cast<const ossimUtmProjection*>(&proj);
      if ( p )
      {
         if ( theZone == p->theZone )
         {
            if ( theHemisphere == p->theHemisphere )
            {
               result = ossimMapProjection::operator==(proj);
            }
         }
      }
   }
   return result;   
}
