//*****************************************************************************
// FILE: ossimSensorModel.h
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
//   Contains declaration of class ossimSensorModel. This is the base class to
//   all sensor model-related projections including replacement models such as
//   coarse grids and polynomial models. This base class supports adjustable
//   parameters for registration adjustment.
//
//   Important note to sensor model implementors: In order to avoid a separate
//   set of "initial adjustable parameters," this design assumes ALL initial
//   values are 0. When designing the derived-class model, insure that the
//   adjustable parameters are 0-based. This applies to theAdjustableParams
//   array declared in this base class only. The derived classes can declare
//   their own adjstable params that are more descriptive and that can be
//   assigned an initial value that is non-zero, but that are linearly related
//   to the adjustable params of this class. In that case, the updateModel()
//   method will compute the derived-class's parameters based on
//   theAdjustableParams array, after an adjustment is made.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimSensorModel.h,v 1.28 2005/05/12 19:55:13 gpotts Exp $

#ifndef ossimSensorModel_HEADER
#define ossimSensorModel_HEADER

#include <projections/ossimProjection.h>
#include <base/data_types/ossimEcefRay.h>
#include <base/data_types/ossimString.h>
#include <base/data_types/ossimPolygon.h>
#include <base/data_types/ossimDrect.h>
#include <elevation/ossimElevSource.h>
#include <base/misc/ossimAdjustableParameterInterface.h>

class ossimKeywordlist;

/*!****************************************************************************
 *
 * CLASS:  ossimSensorModel
 *
 *****************************************************************************/
class OSSIMDLLEXPORT ossimSensorModel : public ossimProjection,
   public ossimAdjustableParameterInterface
{
public:
   /*!
    * CONSTRUCTORS:
    */
   ossimSensorModel();
   ossimSensorModel(const ossimSensorModel& copy_this);
   ossimSensorModel(const ossimKeywordlist& geom_kwl);

   virtual ~ossimSensorModel();

   virtual ossimObject* getBaseObject();
   virtual const ossimObject* getBaseObject()const;
   
   /*!
    * ACCESS METHODS:
    */
   virtual ossimGpt origin()            const {return theRefGndPt; }
   virtual ossimDpt getMetersPerPixel() const {return ossimDpt(fabs(theGSD.x),
                                                               fabs(theGSD.y));}
   
   /*!
    * Implementation of base-class pure virtual projection methods. These
    * methods may be overriden by derived classes if those have more efficient
    * schemes. The implementations here are iterative (relatively slow). Both
    * depend on calls to the pure virtual lineSampleHeightToWorld() method.
    */
   virtual void  lineSampleToWorld(const ossimDpt& image_point,
                                   ossimGpt&       world_point) const;
   virtual void  worldToLineSample(const ossimGpt& world_point,
                                   ossimDpt&       image_point) const;

   /*!
    * METHOD: lineSampleHeightToWorld
    * This is the pure virtual that performs the actual work of projecting
    * the image point to the given elevation above MSL. 
    */
   virtual void lineSampleHeightToWorld(const ossimDpt& lineSampPt,
                                        const double&   heightAboveMSL,
                                        ossimGpt&       worldPt) const = 0;
   /*!
    * METHOD: imagingRay(image_point, &ossimEcefRay)
    * Given an image point, returns a ray originating at some arbitrarily high
    * point (ideally at the sensor position) and pointing towards the target.
    * Implemented here but should be overriden for more efficient solution.
    */
   virtual void imagingRay(const ossimDpt& image_point,
                           ossimEcefRay&   image_ray) const;

   /*!
    * METHOD: print()
    * Fulfills base-class pure virtual. Dumps contents of object to ostream.
    */
   virtual std::ostream& print(std::ostream& out) const;

   void setImageRect(const ossimDrect& imageRect);
   void setGroundRect(const ossimGpt& ul,
                      const ossimGpt& ur,
                      const ossimGpt& lr,
                      const ossimGpt& ll);
   /*!
    * METHOD: imageSize()
    * Returns the maximum line/sample rigorously defined by this model.
    */
   ossimDpt imageSize() const { return theImageSize; }

   /*!
    * This is from the adjustable parameter interface.  It is
    * called when a paraemter adjustment is made.
    */
   virtual void adjustableParametersChanged()
      {
         updateModel();
      }
   /*!
    * VIRTUAL METHOD: updateModel()
    * Following a change to the adjustable parameter set, this virtual
    * is called to permit instances to compute derived quantities after
    * parameter change.
    */
   virtual void updateModel() {}

   /*!
    * METHODS:  saveState, loadState
    * Fulfills ossimObject base-class pure virtuals. Loads and saves geometry
    * KWL files.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /*!
    * METHOD: insideImage(image_point)
    * Returns true if the image_point lies inside the image rectangle.
    */
   virtual bool insideImage(const ossimDpt& p) const
      {
/*          return( (p.u>=(0.0-FLT_EPSILON)) && */
/*                  (p.u<=(double)(theImageSize.u-(1-FLT_EPSILON))) && */
/*                  (p.v>=(0.0-FLT_EPSILON)) && */
/*                  (p.v<=(double)(theImageSize.v-(1-FLT_EPSILON))) ); */
         return theImageClipRect.pointWithin(p);
      }

   /*!
    * STATIC METHOD: writeGeomTemplate(ostream)
    * Writes a template of keywords processed by loadState and saveState to
    * output stream.
    */
   static void writeGeomTemplate(ostream& os);

   /*!
    * OPERATORS: 
    */
   virtual bool operator==(const ossimProjection& proj) const; //inline below

protected:
   /*!
    * METHOD: extrapolate()
    * Extrapolates solutions for points outside of the image. The second
    * version accepts a height value -- if left at the default, the elevation
    * will be looked up via theElevation object.
    */
   virtual ossimDpt extrapolate (const ossimGpt& gp) const;
   virtual ossimGpt extrapolate (const ossimDpt& ip,
				 const double& height
				 =ossimElevSource::DEFAULT_NULL_HEIGHT) const;

   ossimIpt       theImageSize;       // pixels

   /*!
    * Support sub-image of larger full image by maintaining offset to UL corner
    */
   ossimDpt       theSubImageOffset;  // pixels
   
   ossimString    theImageID;
   ossimString    theSensorID;
   ossimDpt       theGSD;             // meters
   ossim_float64  theMeanGSD;         // meters
   ossimGpt       theRefGndPt;        // should be image center
   ossimDpt       theRefImgPt;        // should be image center
   ossimPolygon   theBoundGndPolygon;
   ossimDrect     theImageClipRect;
   ossim_float64  theNominalPosError; // meters
  
TYPE_DATA
};

//================= BEGIN INLINE DEFINITIONS =================================

//*****************************************************************************
//  OPERATOR == 
//*****************************************************************************
inline bool ossimSensorModel::operator==(const ossimProjection& proj) const
{
   const  ossimSensorModel* model
      = PTR_CAST(ossimSensorModel, (const ossimProjection*) &proj);
    
   if ( (!model) ||
	(theSensorID != model->theSensorID) ||
	(theImageID != model->theImageID) ||
	(theSubImageOffset != model->theSubImageOffset) )
      return false;

   return true;
}

#endif
