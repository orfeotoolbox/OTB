//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
// $Id: ossimIntensityAdjustmentFilter.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimIntensityAdjustmentFilter_HEADER
#define ossimIntensityAdjustmentFilter_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/base/ossimDblGrid.h>

class ossimIntensityAdjustmentFilter : public ossimImageSourceFilter
{
public:
   ossimIntensityAdjustmentFilter();
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32 resLevel=0);

   const ossimIrect& getGridBounds()const
      {
         return theGridBounds;
      }
   virtual void setIntensityGrid(const ossimDblGrid& grid)
      {
         theMeanIntensityGrid = grid;
      }
   void setMeanIntensityTarget(double targetMean)
      {
         theMeanIntensityTarget = targetMean;
      }
   double getMeanIntensityTarget()const
      {
         return theMeanIntensityTarget;
      }
   ossimDblGrid& getMeanIntensityGrid()
      {
         return theMeanIntensityGrid;
      }
   const ossimDblGrid& getMeanIntensityGrid()const
      {
         return theMeanIntensityGrid;
      }
   void createAndPopulateGrid(const ossimIpt& spacing, double targetMean = .5);
   
   virtual void initialize();
protected:
   virtual ~ossimIntensityAdjustmentFilter();

   virtual void allocate();
   
   ossimDblGrid                theMeanIntensityGrid;
   double                      theMeanIntensityTarget;
   ossimRefPtr<ossimImageData> theNormTile;
   ossimRefPtr<ossimImageData> theTile;
   ossimRefPtr<ossimImageData> theBlankTile;

   ossimIrect   theGridBounds;

   double computeMeanIntensity(ossimRefPtr<ossimImageData>& data);
   void loadNormTile(ossimRefPtr<ossimImageData>& data);
   double matchTargetMean(double inputValue,
                          double meanValue,
                          double targetMean,
                          double minValue,
                          double maxValue);
   
TYPE_DATA
};
#endif
