//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top LICENSE.txt file.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimUpsProjection.cpp 17815 2010-08-03 13:23:14Z dburken $
#include <ossim/projection/ossimUpsProjection.h>
#include <ossim/projection/ossimPolarst.h>

#define PI       3.14159265358979323e0  /* PI     */
#define PI_OVER    (PI/2.0e0)           /* PI over 2 */
#define MAX_LAT    ((PI * 90)/180.0)    /* 90 degrees in radians */
#define MAX_ORIGIN_LAT ((81.114528 * PI) / 180.0)
#define MIN_NORTH_LAT (83.5*PI/180.0)
#define MIN_SOUTH_LAT (-79.5*PI/180.0)
#define MIN_EAST_NORTH 0
#define MAX_EAST_NORTH 4000000

#define UPS_NO_ERROR                0x0000
#define UPS_LAT_ERROR               0x0001
#define UPS_LON_ERROR               0x0002
#define UPS_HEMISPHERE_ERROR        0x0004
#define UPS_EASTING_ERROR           0x0008
#define UPS_NORTHING_ERROR          0x0010
#define UPS_A_ERROR                 0x0020
#define UPS_INV_F_ERROR             0x0040

RTTI_DEF1(ossimUpsProjection, "ossimUpsProjection", ossimMapProjection);

ossimUpsProjection::ossimUpsProjection(const ossimEllipsoid& ellipsoid,
                                       const ossimGpt& origin)
   :ossimMapProjection(ellipsoid, origin)
{
   setDefaults();
   update();
}
    

void ossimUpsProjection::setDefaults()
{
   if(theOrigin.latd() >= -FLT_EPSILON)
   {
      UPS_Origin_Latitude  =  MAX_ORIGIN_LAT;
      theHemisphere        = 'N';
   }
   else
   {
      UPS_Origin_Latitude  =  -MAX_ORIGIN_LAT;
      theHemisphere = 'S';
   }
   UPS_False_Easting    = 2000000;
   UPS_False_Northing   = 2000000;
   false_easting        = 0.0;
   false_northing       = 0.0;
   UPS_Easting          = 0.0;
   UPS_Northing         = 0.0;
   UPS_Origin_Longitude = 0.0;
   theOrigin            = ossimGpt(UPS_Origin_Latitude*DEG_PER_RAD,
                                   0.0,0.0);
}

void ossimUpsProjection::setHemisphere(char hemisphere)
{ 
  theHemisphere = hemisphere; 
  update();
}

ossimGpt ossimUpsProjection::inverse(const ossimDpt &eastingNorthing)const
{
   double lat = 0.0;
   double lon = 0.0;
   
   Convert_UPS_To_Geodetic(theHemisphere,
                           eastingNorthing.x,
                           eastingNorthing.y,
                           &lat,
                           &lon);
   
   return ossimGpt(lat*DEG_PER_RAD, lon*DEG_PER_RAD, 0.0, theDatum);
}

ossimDpt ossimUpsProjection::forward(const ossimGpt &latLon)const
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
   
   Convert_Geodetic_To_UPS(gpt.latr(),
                           gpt.lonr(),
                           &theHemisphere,
                           &easting,
                           &northing);
   
   return ossimDpt(easting, northing);
}

void ossimUpsProjection::update()
{
   Set_UPS_Parameters(theEllipsoid.getA(),
                      theEllipsoid.getFlattening());

   theFalseEastingNorthing.x = UPS_False_Easting;
   theFalseEastingNorthing.y = UPS_False_Northing;

   ossimMapProjection::update();
}

/*!
 * Method to save the state of an object to a keyword list.
 * Return true if ok or false on error.
 */
bool ossimUpsProjection::saveState(ossimKeywordlist& kwl,
                                   const char* prefix)const
{
   return ossimMapProjection::saveState(kwl, prefix);
}

/*!
 * Method to the load (recreate) the state of an object from a keyword
 * list.  Return true if ok or false on error.
 */
bool ossimUpsProjection::loadState(const ossimKeywordlist& kwl,
                                   const char* prefix)
{
   setDefaults();

   return ossimMapProjection::loadState(kwl, prefix);
}

/************************************************************************/
/*                              FUNCTIONS
 *
 */


long ossimUpsProjection::Set_UPS_Parameters( double a,
                                             double f)
{
/*
 * The function SET_UPS_PARAMETERS receives the ellipsoid parameters and sets
 * the corresponding state variables. If any errors occur, the error code(s)
 * are returned by the function, otherwise UPS_NO_ERROR is returned.
 *
 *   a     : Semi-major axis of ellipsoid in meters (input)
 *   f     : Flattening of ellipsoid					      (input)
 */

//  double inv_f = 1 / f;
  long Error_Code = UPS_NO_ERROR;

//   if (a <= 0.0)
//   { /* Semi-major axis must be greater than zero */
//     Error_Code |= UPS_A_ERROR;
//   }
//   if ((inv_f < 250) || (inv_f > 350))
//   { /* Inverse flattening must be between 250 and 350 */
//     Error_Code |= UPS_INV_F_ERROR;
//   }

  if (!Error_Code)
  { /* no errors */
    UPS_a = a;
    UPS_f = f;
  }
  return (Error_Code);
}  /* END of Set_UPS_Parameters  */


void ossimUpsProjection::Get_UPS_Parameters( double *a,
                                             double *f)const
{
/*
 * The function Get_UPS_Parameters returns the current ellipsoid parameters.
 *
 *  a      : Semi-major axis of ellipsoid, in meters (output)
 *  f      : Flattening of ellipsoid					       (output)
 */

  *a = UPS_a;
  *f = UPS_f;
  
  return;
} /* END OF Get_UPS_Parameters */


long ossimUpsProjection::Convert_Geodetic_To_UPS ( double Latitude,
                                                   double Longitude,
                                                   char   * /* Hemisphere */,
                                                   double *Easting,
                                                   double *Northing)const
{
/*
 *  The function Convert_Geodetic_To_UPS converts geodetic (latitude and
 *  longitude) coordinates to UPS (hemisphere, easting, and northing)
 *  coordinates, according to the current ellipsoid parameters. If any 
 *  errors occur, the error code(s) are returned by the function, 
 *  otherwide UPS_NO_ERROR is returned.
 *
 *    Latitude      : Latitude in radians                       (input)
 *    Longitude     : Longitude in radians                      (input)
 *    Hemisphere    : Hemisphere either 'N' or 'S'              (output)
 *    Easting       : Easting/X in meters                       (output)
 *    Northing      : Northing/Y in meters                      (output)
 */

  double tempEasting, tempNorthing;
  long Error_Code = UPS_NO_ERROR;

//   if ((Latitude < -MAX_LAT) || (Latitude > MAX_LAT))
//   {   /* latitude out of range */
//     Error_Code |= UPS_LAT_ERROR;
//   }
//   if ((Latitude < 0) && (Latitude > MIN_SOUTH_LAT))
//     Error_Code |= UPS_LAT_ERROR;
//   if ((Latitude >= 0) && (Latitude < MIN_NORTH_LAT))
//     Error_Code |= UPS_LAT_ERROR;
//   if ((Longitude < -PI) || (Longitude > (2 * PI)))
//   {  /* slam out of range */
//     Error_Code |= UPS_LON_ERROR;
//   }

  if (!Error_Code)
  {  /* no errors */
//     if (Latitude < 0)
//     {
//       UPS_Origin_Latitude = -MAX_ORIGIN_LAT; 
//       *Hemisphere = 'S';
//     }
//     else
//     {
//       UPS_Origin_Latitude = MAX_ORIGIN_LAT; 
//       *Hemisphere = 'N';
//     }


    Set_Polar_Stereographic_Parameters( UPS_a,
                                        UPS_f,
                                        UPS_Origin_Latitude,
                                        UPS_Origin_Longitude,
                                        false_easting,
                                        false_northing);

    Convert_Geodetic_To_Polar_Stereographic(Latitude,
                                            Longitude,
                                            &tempEasting,
                                            &tempNorthing);

    UPS_Easting = UPS_False_Easting + tempEasting;
    UPS_Northing = UPS_False_Northing + tempNorthing;


    *Easting = UPS_Easting;
    *Northing = UPS_Northing;
  }  /*  END of if(!Error_Code)   */

  return (Error_Code);
}  /* END OF Convert_Geodetic_To_UPS  */


long ossimUpsProjection::Convert_UPS_To_Geodetic(char   Hemisphere,
                                                 double Easting,
                                                 double Northing,
                                                 double *Latitude,
                                                 double *Longitude)const
{
/*
 *  The function Convert_UPS_To_Geodetic converts UPS (hemisphere, easting, 
 *  and northing) coordinates to geodetic (latitude and longitude) coordinates
 *  according to the current ellipsoid parameters.  If any errors occur, the 
 *  error code(s) are returned by the function, otherwise UPS_NO_ERROR is 
 *  returned.
 *
 *    Hemisphere    : Hemisphere either 'N' or 'S'              (input)
 *    Easting       : Easting/X in meters                       (input)
 *    Northing      : Northing/Y in meters                      (input)
 *    Latitude      : Latitude in radians                       (output)
 *    Longitude     : Longitude in radians                      (output)
 */

  long Error_Code = UPS_NO_ERROR;

//   if ((Hemisphere != 'N') && (Hemisphere != 'S'))
//     Error_Code |= UPS_HEMISPHERE_ERROR;
//   if ((Easting < MIN_EAST_NORTH) || (Easting > MAX_EAST_NORTH))
//     Error_Code |= UPS_EASTING_ERROR;
//   if ((Northing < MIN_EAST_NORTH) || (Northing > MAX_EAST_NORTH))
//     Error_Code |= UPS_NORTHING_ERROR;

  if (Hemisphere =='N')
  {UPS_Origin_Latitude = MAX_ORIGIN_LAT;}
  if (Hemisphere =='S')
  {UPS_Origin_Latitude = -MAX_ORIGIN_LAT;}

  if (!Error_Code)
  {   /*  no errors   */
    Set_Polar_Stereographic_Parameters( UPS_a,
                                        UPS_f,
                                        UPS_Origin_Latitude,
                                        UPS_Origin_Longitude,
                                        UPS_False_Easting,
                                        UPS_False_Northing);



    Convert_Polar_Stereographic_To_Geodetic( Easting,
                                             Northing,
                                             Latitude,
                                             Longitude); 


//     if ((*Latitude < 0) && (*Latitude > MIN_SOUTH_LAT))
//       Error_Code |= UPS_LAT_ERROR;
//     if ((*Latitude >= 0) && (*Latitude < MIN_NORTH_LAT))
//       Error_Code |= UPS_LAT_ERROR;
  }  /*  END OF if(!Error_Code) */
  return (Error_Code);
}  /*  END OF Convert_UPS_To_Geodetic  */ 

//*************************************************************************************************
//! Returns TRUE if principal parameters are within epsilon tolerance.
//*************************************************************************************************
bool ossimUpsProjection::operator==(const ossimProjection& proj) const
{
   if (!ossimMapProjection::operator==(proj))
      return false;

   ossimUpsProjection* p = PTR_CAST(ossimUpsProjection, &proj);
   if (!p) return false;

   if (theHemisphere != p->theHemisphere) return false;

   return true;
}
