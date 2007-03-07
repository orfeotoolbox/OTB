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
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
//*******************************************************************
//  $Id: ossimSpaceObliqueMercatorProjection.h,v 1.6 2005/09/09 19:23:58 gpotts Exp $
#ifndef ossimSpaceObliqueMercatorProjection_HEADER
#define ossimSpaceObliqueMercatorProjection_HEADER
#include "ossimMapProjection.h"

/*!
 * This class implments the formulas for the ellipsoid and
 * circular orbit.  All formulas were directly taken from
 * proj 4.  Proj 4 implementation is from the following
 * manual reference:
 *
 * Map Projections - A Working Manual
 * by John Snyder
 * Space Oblique Mercator p. 221.
 *
 * 
 */
class ossimSpaceObliqueMercatorProjection : public ossimMapProjection
{
public:
   enum ossimSatelliteType
   {
      SOM_TYPE_LANDSAT_1 = 0,
      SOM_TYPE_LANDSAT_2 = 1,
      SOM_TYPE_LANDSAT_3 = 2,
      SOM_TYPE_LANDSAT_4 = 3, 
      SOM_TYPE_LANDSAT_5 = 4,
      SOM_TYPE_LANDSAT_7 = 5
   };
   
   ossimSpaceObliqueMercatorProjection(ossimSatelliteType type=SOM_TYPE_LANDSAT_7,
                                       double pathNumber=34,
                                       const ossimEllipsoid& ellipsoid = ossimEllipsoid());

   virtual ossimObject* dup()const{return new ossimSpaceObliqueMercatorProjection(*this);}
   virtual ossimDpt forward(const ossimGpt &worldPoint)    const;
   virtual ossimGpt inverse(const ossimDpt &projectedPoint)const;

     /*!
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;

   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
  virtual void setParameters(ossimSatelliteType type,
                              double path);
   
   virtual void update();
protected:

   // proj 4 parameters that I bridge to.
   //
   double a2, a4, b, c1, c3; 
   double q, t, u, w, p22, sa, ca, xj, rlm, rlm2;

   // proj4 stuff 
   double lam0;
   double es;
   double e;
   double one_es;
   double rone_es;
   double a;
   
   void seraz0(double lam, double mult);
   
   /*!
    * Is the p variable in Map Projections a Working Manual.
    */
   double thePath;
   ossimSatelliteType theSatelliteType;
   
TYPE_DATA
};
#endif
