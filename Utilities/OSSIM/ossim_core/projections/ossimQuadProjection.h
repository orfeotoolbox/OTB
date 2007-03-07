//*******************************************************************
// Copyright (C) 2004 Intelligence Data Systems, Inc.  All rights reserved.
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
// Author: Garrett Potts
// 
//********************************************************************
// $Id: ossimQuadProjection.h,v 1.3 2004/06/17 14:13:36 dburken Exp $
#ifndef ossimQuadProjection_HEADER
#define ossimQuadProjection_HEADER

#include <projections/ossimProjection.h>
#include <base/data_types/ossimIrect.h>
#include <base/data_types/ossimDblGrid.h>

class ossimQuadProjection : public ossimProjection
{
 public:
   ossimQuadProjection();
   ossimQuadProjection(const ossimQuadProjection& rhs);
   ossimQuadProjection(const ossimIrect& rect,
                       const ossimGpt& ulg,
                       const ossimGpt& urg,
                       const ossimGpt& lrg,
                       const ossimGpt& llg);
                       
   virtual ~ossimQuadProjection();
   virtual ossimObject *dup()const;
   virtual ossimGpt origin()const;
   
   virtual void worldToLineSample(const ossimGpt& worldPoint,
                                  ossimDpt&       lineSampPt) const;
   /*!
    * METHOD: lineSampleToWorld()
    * Performs the inverse projection from line, sample to ground (world):
    */
   virtual void lineSampleToWorld(const ossimDpt& lineSampPt,
                                  ossimGpt&       worldPt) const;
   
   /*!
    * METHOD: lineSampleHeightToWorld
    * This is the pure virtual that projects the image point to the given
    * elevation above ellipsoid, thereby bypassing reference to a DEM. Useful
    * for projections that are sensitive to elevation (such as sensor models).
    */
   virtual void lineSampleHeightToWorld(const ossimDpt& lineSampPt,
                                        const double&   heightAboveEllipsoid,
                                        ossimGpt&       worldPt) const;
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
   virtual bool operator==(const ossimProjection& projection) const;
   virtual ossimDpt getMetersPerPixel() const;

 protected:
   ossimIrect theInputRect;
   ossimGpt theUlg;
   ossimGpt theUrg;
   ossimGpt theLrg;
   ossimGpt theLlg;

   ossimDblGrid theLatGrid;   
   ossimDblGrid theLonGrid;

   void initializeGrids();
   ossimGpt extrapolate(const ossimDpt& imagePoint,
                        const double&   height) const;
TYPE_DATA   
};

#endif /* #ifndef ossimQuadProjection_HEADER */
