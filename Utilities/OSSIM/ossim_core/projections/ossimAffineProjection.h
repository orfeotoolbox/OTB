//*****************************************************************************
// FILE: ossimAffineProjection.h
//
// Copyright (C) 2001 ImageLinks, Inc.
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
// AUTHOR: Oscar Kramer (okramer@imagelinks.com)
//
// DESCRIPTION:
//   Contains declaration of class ossimAffineProjection. This is an
//   implementation of a warping interpolation model.
//
//*****************************************************************************
//  $Id: ossimAffineProjection.h,v 1.3 2005/09/09 19:23:58 gpotts Exp $

#ifndef ossimAffineProjection_HEADER
#define ossimAffineProjection_HEADER

#include "projections/ossimProjection.h"
#include "base/data_types/ossimIpt.h"

class ossim2dTo2dTransform;

/*!****************************************************************************
 *
 * CLASS:  ossimAffineProjection
 *
 *****************************************************************************/
class ossimAffineProjection : public ossimProjection
{
public:
   /*!
    * Default Contructor:
    */
   ossimAffineProjection();

   /*!
    * Primary constructor accepting pointer to the underlying client projection.
    */
   ossimAffineProjection(ossimProjection* client);

   /*!
    * Constructor accepts OSSIM keywordlist geometry file.
    */
   ossimAffineProjection(const ossimKeywordlist& geom_kwl,
                       const char* prefix=NULL);
   
   ~ossimAffineProjection();
   
   /*!
    * METHOD: worldToLineSample()
    * Performs the forward projection from ground point to line, sample.
    */
   virtual void worldToLineSample(const ossimGpt& worldPoint,
                                  ossimDpt&       lineSampPt) const;

   /*!
    * METHOD: lineSampleToWorld()
    * Performs the inverse projection from line, sample to ground (world):
    */
   virtual void lineSampleToWorld(const ossimDpt& lineSampPt,
                                      ossimGpt&       worldPt) const;
   
   /*!
    * Performs the inverse projection from line, sample to ground, bypassing
    * reference to elevation surface:
    */
   virtual void lineSampleHeightToWorld(const ossimDpt& lineSampPt,
                                        const double&   hgtEllipsoid,
                                        ossimGpt&       worldPt) const;
   
   /*!
    * Extends base-class implementation. Dumps contents of object to ostream.
    */
   virtual std::ostream& print(std::ostream& out) const;
   friend std::ostream& operator<<(std::ostream& os,
                                   const ossimAffineProjection& m);
   
   /*!
    * Fulfills ossimObject base-class pure virtuals. Loads and saves geometry
    * KWL files. Returns true if successful.
    */
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0) const;
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);

   /*!
    * Returns pointer to a new instance, copy of this.
    */
   virtual ossimObject* dup() const;
   
   /*!
    * Returns projection's ground point origin. That is the GP corresponding
    * to line=0, sample=0.
    */
   virtual ossimGpt origin() const;

   /*!
    * Compares this instance with arg projection. NOT IMPLEMENTED.
    */
   virtual bool operator==(const ossimProjection& projection) const
      { return false; }

   virtual ossimDpt getMetersPerPixel() const;
      
protected:
   
   /*!
    * Data Members:
    */
   ossimProjection*      theClientProjection;
   ossim2dTo2dTransform* theAffineTransform;
   
   TYPE_DATA
};

#endif
