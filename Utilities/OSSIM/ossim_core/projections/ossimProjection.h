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
// Description: Base class for all projections (2D-to-3D transform)
//
//*******************************************************************
//  $Id: ossimProjection.h,v 1.20 2005/09/28 19:23:17 gpotts Exp $
#ifndef ossimProjection_HEADER
#define ossimProjection_HEADER
#include <iostream>

#include <base/common/ossimObject.h>
#include <base/common/ossimErrorStatusInterface.h>
#include <base/data_types/ossimString.h>
#include <base/data_types/ossimKeyword.h>
#include <base/data_types/ossimGpt.h>
#include <base/data_types/ossimDpt.h>
#include <base/data_types/ossimGeoPolygon.h>
#include <elevation/ossimElevManager.h>

class OSSIMDLLEXPORT ossimProjection : public ossimObject,
   public ossimErrorStatusInterface
{
public:
   /*!
    * Constructors, Destructor:
    */
   ossimProjection();
   virtual ~ossimProjection() {}

   virtual ossimObject *dup()const=0;

   /*!
    * METHOD: origin()
    * Returns projection's ground point origin. That is the GP corresponding
    * to line=0, sample=0.
    */
   virtual ossimGpt origin()const=0;

   /*!
    * METHODS: forward(), reverse() 
    * OBSOLETE -- provided for existing GUI code only. Bogus return value.
    */
   virtual ossimDpt forward(const ossimGpt &wp) const;  //inline below
   virtual ossimGpt inverse(const ossimDpt &pp) const;  //inline below

   /*!
    * METHOD: worldToLineSample()
    * Performs the forward projection from ground point to line, sample.
    */
   virtual void worldToLineSample(const ossimGpt& worldPoint,
                                  ossimDpt&       lineSampPt) const = 0;

   /*!
    * METHOD: lineSampleToWorld()
    * Performs the inverse projection from line, sample to ground (world):
    */
   virtual void lineSampleToWorld(const ossimDpt& lineSampPt,
                                  ossimGpt&       worldPt) const = 0;
   
   /*!
    * METHOD: lineSampleHeightToWorld
    * This is the pure virtual that projects the image point to the given
    * elevation above ellipsoid, thereby bypassing reference to a DEM. Useful
    * for projections that are sensitive to elevation (such as sensor models).
    */
   virtual void lineSampleHeightToWorld(const ossimDpt& lineSampPt,
                                        const double&   heightAboveEllipsoid,
                                        ossimGpt&       worldPt) const = 0;

   virtual void getRoundTripError(const ossimDpt& imagePoint,
                                  ossimDpt& errorResult)const;

   virtual void getRoundTripError(const ossimGpt& groundPoint,
                                  ossimDpt& errorResult)const;
   
   virtual std::ostream& print(std::ostream& out) const;

   virtual void getGroundClipPoints(ossimGeoPolygon& gpts)const;
   /*!
    * METHODS:  saveState, loadState
    * Fulfills ossimObject base-class pure virtuals.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /*!
    * OPERATOR: ==
    * Compares this instance with arg projection. NOT IMPLEMENTED.
    */
   virtual bool operator==(const ossimProjection& projection) const=0;

   /*!
    * ACCESS METHODS: 
    */
   virtual ossimDpt getMetersPerPixel() const=0;
   
protected:
   

   TYPE_DATA
};

//====================== BEGIN INLINE DEFINITIONS ===========================

//*****************************************************************************
//  INLINE METHOD: ossimProjection::forward()
//  Projects ground point to 2D plane.
//*****************************************************************************
inline ossimDpt ossimProjection::forward(const ossimGpt &wp) const
{
   ossimDpt p;
   worldToLineSample(wp, p);
   return p;
}

//*****************************************************************************
//  INLINE METHOD: ossimProjection::inverse()
//  Inverse projection from 2D plane to ground point.
//*****************************************************************************
inline ossimGpt ossimProjection::inverse(const ossimDpt &pp) const
{
   ossimGpt g;
   lineSampleToWorld(pp, g);
   return g;
}

#endif
