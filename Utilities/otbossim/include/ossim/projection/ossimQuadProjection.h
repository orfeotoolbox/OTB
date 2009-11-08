//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// 
//********************************************************************
// $Id: ossimQuadProjection.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimQuadProjection_HEADER
#define ossimQuadProjection_HEADER

#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimDblGrid.h>

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

   /**
    * @brief Implementation of pure virtual
    * ossimProjection::isAffectedByElevation method.
    * @return false.
    */
   virtual bool isAffectedByElevation() const { return false; }

 protected:
   virtual ~ossimQuadProjection();
   
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
