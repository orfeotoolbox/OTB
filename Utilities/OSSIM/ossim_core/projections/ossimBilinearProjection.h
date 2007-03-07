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
// $Id: ossimBilinearProjection.h,v 1.2 2004/07/01 21:02:30 dburken Exp $
#ifndef ossimBilinearProjection_HEADER
#define ossimBilinearProjection_HEADER

#include <vector>
#include <base/data_types/ossimDpt.h>
#include <base/data_types/ossimGpt.h>
#include <projections/ossimProjection.h>
#include <base/common/ossimLeastSquaresBilin.h>

class ossimBilinearProjection : public ossimProjection
{
public:
   ossimBilinearProjection();
   ossimBilinearProjection(const ossimBilinearProjection& rhs);
   ossimBilinearProjection(const ossimDpt& ul,
                           const ossimDpt& ur,
                           const ossimDpt& lr,
                           const ossimDpt& ll,
                           const ossimGpt& ulg,
                           const ossimGpt& urg,
                           const ossimGpt& lrg,
                           const ossimGpt& llg);
   virtual ~ossimBilinearProjection();
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

   virtual std::ostream& print(std::ostream& out) const;

protected:
   void initializeBilinear();

   /**
    * Checks theLineSamplePt for nans.
    * @return true if any point has a nan.
    * @note If theLineSamplePt size is 0 this returns false.
    */
   bool dPtsHaveNan() const;
   
   /**
    * Checks theGeographicPt for nans.
    * @return true if any point has a nan.
    * @note If theLineSamplePt size is 0 this returns false.
    */
   bool gPtsHaveNan() const;

   /**
    * @return The mid point of theLineSamplePts.
    */
   ossimDpt midLineSamplePt() const;
   
   std::vector<ossimDpt>  theLineSamplePt;
   std::vector<ossimGpt>  theGeographicPt;
   ossimLeastSquaresBilin theLatFit;
   ossimLeastSquaresBilin theLonFit;

TYPE_DATA   
};

#endif /* #ifndef ossimBilinearProjection_HEADER */
