//*******************************************************************
// Copyright (C) 2005 Garrett Potts. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimClosestToCenterCombiner.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimClosestToCenterCombiner_HEADER
#define ossimClosestToCenterCombiner_HEADER
#include <vector>
#include <ossim/imaging/ossimImageMosaic.h>
/**
  * This implements a closest to center combiner.
  *
  * It will first grab all images overlapping a given rectangle query.  The first non-null
  * closest pixel to the center of the image is used.  This basically uses only pixels along
  * the "sweet" spot of the image.  This combiner is typically used when there is overlapping
  * sensor data.
  * 
  */
class ossimClosestToCenterCombiner : public ossimImageMosaic
{
public:
   ossimClosestToCenterCombiner();

   /**
    * Executes the ossimClosestToCenterCombiner algorithm. sets the pixels to the closest
    * Valid value.  This simulates a sweet spot cutter.
    * 
    * @param rect The region of interest.
    * @param resLevel.  For this combiner this is assumed to always be 0
    */
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32 resLevel=0);
protected:
   class ossimClosestToCenterCombinerInfo
   {
   public:
      ossimClosestToCenterCombinerInfo(ossimRefPtr<ossimImageData> tile,
                                       ossim_int32 idx)
         :theTile(tile),
         theIndex(idx)
         {
         }
      ossimRefPtr<ossimImageData> theTile;
      ossim_int32                 theIndex;
   };

   
   ossim_int32 findIdx(const std::vector<ossimClosestToCenterCombinerInfo >& normTileList,
                       const ossimIpt& pt, ossim_uint32 offset)const;

TYPE_DATA
};

#endif
