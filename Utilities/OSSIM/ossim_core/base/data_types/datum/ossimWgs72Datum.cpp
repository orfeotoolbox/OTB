//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// Wgs72Datum.  Special hardcoded datum.  It will create a static
// instance of a Wgs72Ellipsoid and set the initial defaults for
// that are specific to a Wgs72Datum
//*******************************************************************
//  $Id: ossimWgs72Datum.cpp,v 1.8 2004/04/02 21:17:00 gpotts Exp $

#include <iostream>
using namespace std;
#include <base/data_types/datum/ossimWgs72Datum.h>

#include <base/common/ossimConstants.h>
#include <base/factory/ossimDatumFactory.h>
#include <base/factory/ossimEllipsoidFactory.h>
#include <base/data_types/ellipse/ossimEllipsoid.h>
#include <base/data_types/ossimGpt.h>
#include <base/context/ossimNotifyContext.h>

/***************************************************************************/
/*
 *                               DEFINES FROM GEOTRANS
 */


ossimWgs72Datum::ossimWgs72Datum()
   :ossimThreeParamDatum("WGD",
                         "World Geodetic System 1972",
                         ossimEllipsoidFactory::instance()->wgs72(),
                         0.0,
                         0.0,
                         0.0,
                         -M_PI/2.0,
                         M_PI/2.0,
                         -M_PI,
                         M_PI,
                         0.0,
                         0.0,
                         0.0)
{
   if(!ellipsoid())
   {
      //ERROR
   }
}

ossimGpt ossimWgs72Datum::shift(const ossimGpt &aPt)const
{
   const ossimDatum *aDatum = aPt.datum();
   
   if( (ellipsoid()->getA()== aPt.datum()->ellipsoid()->getA())&&
       (ellipsoid()->getB()== aPt.datum()->ellipsoid()->getB()))
   {
      return ossimGpt(aPt.latd(), aPt.lond(), aPt.height(), this);
   }
   
   if(aDatum)
   {  
      return shiftFromWgs84(aDatum->shiftToWgs84(aPt));
   }

   return aPt;
}

ossimGpt ossimWgs72Datum::shiftToWgs84(const ossimGpt &aPt)const
{
/* Begin Geodetic_Shift_WGS72_To_WGS84 */
  /*
   *  This function shifts a geodetic coordinate (latitude, longitude in radians
   *  and height in meters) relative to WGS72 to a geodetic coordinate 
   *  (latitude, longitude in radians and height in meters) relative to WGS84.
   *
   *  WGS72_Lat : Latitude in radians relative to WGS72     (input)
   *  WGS72_Lon : Longitude in radians relative to WGS72    (input)
   *  WGS72_Hgt : Height in meters relative to WGS72        (input)
   *  WGS84_Lat : Latitude in radians relative to WGS84     (output)
   *  WGS84_Lon : Longitude in radians relative to WGS84    (output)
   *  WGS84_Hgt : Height in meters  relative to WGS84       (output)
   */
   double Delta_Lat;
   double Delta_Lon;
   double Delta_Hgt;
   double WGS84_a;       /* Semi-major axis of WGS84 ellipsoid               */
   double WGS84_f;       /* Flattening of WGS84 ellipsoid                    */
   double WGS72_a;       /* Semi-major axis of WGS72 ellipsoid               */
   double WGS72_f;       /* Flattening of WGS72 ellipsoid                    */
   double da;            /* WGS84_a - WGS72_a                                */
   double df;            /* WGS84_f - WGS72_f                                */
   double Q;
   double sin_Lat;
   double sin2_Lat;
   
   const ossimDatum *wgs84 = ossimDatumFactory::instance()->wgs84();
   const ossimDatum *wgs72 = ossimDatumFactory::instance()->wgs72();
   const ossimEllipsoid *wgs84Ellipsoid = ossimEllipsoidFactory::instance()->wgs84();
   const ossimEllipsoid *wgs72Ellipsoid = ossimEllipsoidFactory::instance()->wgs72();

   if(!wgs84 || !wgs72 || !wgs72Ellipsoid || !wgs84Ellipsoid)
   {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimWgs72Datum::shiftToWgs84, NULL pointer found and no shift will be performed\n";
      return (aPt);
   }
   
   WGS84_a = wgs84Ellipsoid->a();
   WGS84_f = wgs84Ellipsoid->flattening();
   WGS72_a = wgs72Ellipsoid->a();
   WGS72_f = wgs72Ellipsoid->flattening();
   da = WGS84_a - WGS72_a;
   df = WGS84_f - WGS72_f;
   Q = M_PI /  648000;
   sin_Lat = sin(aPt.latr());
   sin2_Lat = sin_Lat * sin_Lat;
   
   Delta_Lat = (4.5 * cos(aPt.latr())) / (WGS72_a*Q) + (df * sin(2*aPt.latr())) / Q;
   Delta_Lat /= SEC_PER_RAD;
   Delta_Lon = 0.554 / SEC_PER_RAD;
   Delta_Hgt = 4.5 * sin_Lat + WGS72_a * df * sin2_Lat - da + 1.4;

   if(aPt.isHgtNan())
   {
      return ossimGpt(aPt.latd() + Delta_Lat*DEG_PER_RAD,
                      aPt.lond() + Delta_Lon*DEG_PER_RAD,
                      Delta_Hgt,
                      wgs84);
   }
   return ossimGpt(aPt.latd() + Delta_Lat*DEG_PER_RAD,
                   aPt.lond() + Delta_Lon*DEG_PER_RAD,
                   aPt.height() + Delta_Hgt,
                   wgs84);
   /* End Geodetic_Shift_WGS72_To_WGS84 */
}

ossimGpt ossimWgs72Datum::shiftFromWgs84(const ossimGpt &aPt)const
{
 /* Begin Geodetic_Shift_WGS84_To_WGS72 */
  /*
   *  This function shifts a geodetic coordinate (latitude, longitude in radians
   *  and height in meters) relative to WGS84 to a geodetic coordinate 
   *  (latitude, longitude in radians and height in meters) relative to WGS72.
   *
   *  WGS84_Lat : Latitude in radians relative to WGS84     (input)
   *  WGS84_Lon : Longitude in radians relative to WGS84    (input)
   *  WGS84_Hgt : Height in meters  relative to WGS84       (input)
   *  WGS72_Lat : Latitude in radians relative to WGS72     (output)
   *  WGS72_Lon : Longitude in radians relative to WGS72    (output)
   *  WGS72_Hgt : Height in meters relative to WGS72        (output)
   */
   double Delta_Lat;
   double Delta_Lon;
   double Delta_Hgt;
   double WGS84_a;       /* Semi-major axis of WGS84 ellipsoid               */
   double WGS84_f;       /* Flattening of WGS84 ellipsoid                    */
   double WGS72_a;       /* Semi-major axis of WGS72 ellipsoid               */
   double WGS72_b;       /* Semi-minor axis of WGS72 ellipsoid               */
   double WGS72_f;       /* Flattening of WGS72 ellipsoid                    */
   double da;            /* WGS72_a - WGS84_a                                */
   double df;            /* WGS72_f - WGS84_f                                */
   double Q;
   double sin_Lat;
   double sin2_Lat;
   const ossimDatum *wgs84 = ossimDatumFactory::instance()->wgs84();
   const ossimDatum *wgs72 = ossimDatumFactory::instance()->wgs72();
   const ossimEllipsoid *wgs84Ellipsoid = ossimEllipsoidFactory::instance()->wgs84();
   const ossimEllipsoid *wgs72Ellipsoid = ossimEllipsoidFactory::instance()->wgs72();
   
   if(!wgs84 || !wgs72 || !wgs72Ellipsoid || !wgs84Ellipsoid)
   {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimWgs72Datum::shiftFromWgs84, NULL pointer found and no shift will be performed\n";
      
      return (aPt);
   }
   
   WGS84_a = wgs84Ellipsoid->a();
   WGS84_f = wgs84Ellipsoid->flattening();
   WGS72_a = wgs72Ellipsoid->a();
   WGS72_b = wgs72Ellipsoid->b();  
   WGS72_f = wgs72Ellipsoid->flattening();
   
   da = WGS72_a - WGS84_a;
   df = WGS72_f - WGS84_f;
   Q = M_PI / 648000;
   sin_Lat = sin(aPt.latr());
   sin2_Lat = sin_Lat * sin_Lat;

   Delta_Lat = (-4.5 * cos(aPt.latr())) / (WGS84_a*Q)
               + (df * sin(2*aPt.latr())) / Q;
   Delta_Lat /= SEC_PER_RAD;
   Delta_Lon = -0.554 / SEC_PER_RAD;
   Delta_Hgt = -4.5 * sin_Lat + WGS84_a * df * sin2_Lat - da - 1.4;

   if(aPt.isHgtNan())
   {
      
      return ossimGpt(aPt.latd() + Delta_Lat*DEG_PER_RAD,
                      aPt.lond() + Delta_Lon*DEG_PER_RAD,
                      Delta_Hgt,
                      this);
   }
   return ossimGpt(aPt.latd() + Delta_Lat*DEG_PER_RAD,
                   aPt.lond() + Delta_Lon*DEG_PER_RAD,
                   aPt.height() + Delta_Hgt,
                   this);
}
