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
// $Id: ossimBandClipFilter.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimBandClipFilter_HEADER
#define ossimBandClipFilter_HEADER
#include <vector>
using namespace std;

#include <ossim/imaging/ossimImageSourceFilter.h>

class ossimBandClipFilter : public ossimImageSourceFilter
{
public:
   /*!
    * Specifies the type of clipping.  The first will
    * just clip and 
    */
   enum ossimBandClipType
   {
      ossimBandClipType_NONE             = 0,
      ossimBandClipType_CLIP             = 1,
      ossimBandClipType_CLAMP            = 2,
      ossimBandClipType_LINEAR_STRETCH   = 3,
      ossimBandClipType_MEDIAN_STRETCH   = 4 
   };

   ossimBandClipFilter();
   ossimBandClipFilter(ossimImageSource*,
                        const vector<double>& minPix,
                        const vector<double>& maxPix,
                        ossimBandClipType clipType=ossimBandClipType_NONE);
   
   ossimBandClipFilter(ossimImageSource*,
                        double minPix,
                        double maxPix,
                        ossimBandClipType clipType=ossimBandClipType_NONE);


   void setClipType(ossimBandClipType clipType);

   ossimBandClipType getClipType()const;

   virtual ossim_uint32 getNumberOfValues()const;

   void setNumberOfValues(ossim_uint32 size);
   
   void setMinMaxPix(const vector<double>& minPix,
                     const vector<double>& maxPix);

   const std::vector<double>& getMinPixList()const;
   const std::vector<double>  getMaxPixList()const;

   double getMinPix(ossim_uint32 index)const;
   double getMaxPix(ossim_uint32 index)const;

   ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                       ossim_uint32 resLevel=0);
   
   virtual void initialize();
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = NULL);

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix = NULL)const;
protected:
   virtual ~ossimBandClipFilter();

   void runClip();
   void runClamp();
   void runLinearStretch();
   void runMedianStretch();

   std::vector<double> theMinPix; // normalized min
   std::vector<double> theMaxPix; // normalized max
   std::vector<double> theMedian; // normalized median.

   ossimBandClipType           theClipType;
   ossimRefPtr<ossimImageData> theTile;

TYPE_DATA
};

#endif
