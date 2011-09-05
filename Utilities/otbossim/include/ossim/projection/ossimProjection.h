//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description: Base class for all projections (2D-to-3D transform)
//
//*******************************************************************
//  $Id: ossimProjection.h 19793 2011-06-30 13:26:56Z gpotts $
#ifndef ossimProjection_HEADER
#define ossimProjection_HEADER
#include <iostream>

#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimKeyword.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimGeoPolygon.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimRefPtr.h>
class OSSIMDLLEXPORT ossimProjection : public ossimObject, public ossimErrorStatusInterface
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

   virtual bool isEqualTo(const ossimObject& obj, ossimCompareType compareType = OSSIM_COMPARE_FULL)const
   {
      return ossimObject::isEqualTo(obj, compareType);
   }
   
   /*!
    * OPERATOR: ==
    * Compares this instance with arg projection.
    */
   virtual bool operator==(const ossimProjection& projection) const=0;
   virtual bool operator!=(const ossimProjection& projection) const {return !(*this == projection);}

   /*!
    * ACCESS METHODS: 
    */
   virtual ossimDpt getMetersPerPixel() const=0;

   /**
    * @brief Pure virtual method to query if projection is affected by
    * elevation.
    * @return true if affected, false if not.
    */
   virtual bool isAffectedByElevation() const=0;
   
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
