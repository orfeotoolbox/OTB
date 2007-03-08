//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimMultiBandHistogramTileSource.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimMultiBandHistogramTileSource_HEADER
#define ossimMultiBandHistogramTileSource_HEADER
#include <ossim/imaging/ossimImageSourceHistogramFilter.h>
#include <vector>
class OSSIMDLLEXPORT ossimMultiBandHistogramTileSource : public ossimImageSourceHistogramFilter
{
public:
   enum ossimMultiBandHistogramAlgorithm
   {
      OSSIM_HISTOGRAM_NO_ALGORITHM             = 0,
      OSSIM_HISTOGRAM_LINEAR_STRETCH_ALGORITHM = 1
   };
   ossimMultiBandHistogramTileSource();
   ossimMultiBandHistogramTileSource(double minValuePercent,
                                  double maxValuePercent,
                                  ossimImageSource* inputSource,
                                  ossimMultiResLevelHistogram* histogram);
   virtual ~ossimMultiBandHistogramTileSource();
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);

   virtual void setMinValuePercent(const std::vector<double>& arrayPercents)
      {
         theMinValuePercentArray = arrayPercents;
      }
   virtual void setMaxValuePercent(const std::vector<double>& arrayPercents)
      {
         theMaxValuePercentArray = arrayPercents;
      }
   virtual void setMinValuePercent(ossim_uint32 arrayIndex,
                                   double minValue)
      {
         if(arrayIndex < theMinValuePercentArray.size())
         {
            theMinValuePercentArray[arrayIndex] = minValue;
         }
      }
   virtual void setMaxValuePercent(ossim_uint32 arrayIndex,
                                   double maxValue)
      {
         if(arrayIndex < theMaxValuePercentArray.size())
         {
            theMaxValuePercentArray[arrayIndex] = maxValue;
         }
      }
   virtual double getMinValuePercent(ossim_uint32 arrayIndex)const
      {
         if(arrayIndex < theMinValuePercentArray.size())
         {
            return theMinValuePercentArray[arrayIndex];
         }
         return OSSIM_DBL_NAN;
      }
   virtual double getMaxValuePercent(ossim_uint32 arrayIndex)const
      {
         if(arrayIndex < theMaxValuePercentArray.size())
         {
            return theMaxValuePercentArray[arrayIndex];
         }
         return OSSIM_DBL_NAN;
      }
   virtual ossim_uint32 getNumberOfMinValues()const
      {
         return theMinValuePercentArray.size();
      }
   virtual ossim_uint32 getNumberOfMaxValues()const
      {
         return theMinValuePercentArray.size();
      }
   virtual void initialize();
      
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
protected:

   void allocate();
   
   ossimRefPtr<ossimImageData>      theTile;
   ossimMultiBandHistogramAlgorithm theAlgorithm;
   std::vector<double>              theMinValuePercentArray;
   std::vector<double>              theMaxValuePercentArray;
   
   template <class T> ossimRefPtr<ossimImageData> runLinearStretchAlgorithm(
      T dummyVariable,
      ossimRefPtr<ossimImageData>& tile);

TYPE_DATA
};
#endif
