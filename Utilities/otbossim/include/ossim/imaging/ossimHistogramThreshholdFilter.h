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
// $Id: ossimHistogramThreshholdFilter.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimHistogramThreshholdFilter_HEADER
#define ossimHistogramThreshholdFilter_HEADER
#include <ossim/imaging/ossimImageSourceHistogramFilter.h>

class ossimHistogramThreshholdFilter : public ossimImageSourceHistogramFilter
{
public:
   ossimHistogramThreshholdFilter();
   ossimHistogramThreshholdFilter(double minValuePercent,
                                  double maxValuePercent,
                                  ossimImageSource* inputSource,
                                  ossimMultiResLevelHistogram* histogram);
   virtual ~ossimHistogramThreshholdFilter();
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);
   
   virtual void setMinValuePercent(double minValue);
   virtual void setMaxValuePercent(double maxValue);
   
   virtual double getMinValuePercent()const;
   virtual double getMaxValuePercent()const;

   virtual void initialize();
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
protected:
   
   double theMinValuePercent;
   double theMaxValuePercent;
   template <class T>
   ossimRefPtr<ossimImageData> runThreshholdStretchAlgorithm(
      T dummyVariable,
      ossimRefPtr<ossimImageData>& tile);
   
TYPE_DATA
};
#endif
