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
// $Id: ossimClosestToCenterCombiner.cpp 11955 2007-10-31 16:10:22Z gpotts $
#include <ossim/imaging/ossimClosestToCenterCombiner.h>
#include <ossim/imaging/ossimImageDataFactory.h>

RTTI_DEF1(ossimClosestToCenterCombiner, "ossimClosestToCenterCombiner", ossimImageMosaic);

ossimClosestToCenterCombiner::ossimClosestToCenterCombiner()
   :ossimImageMosaic()
{
}

ossimRefPtr<ossimImageData> ossimClosestToCenterCombiner::getTile(const ossimIrect& rect,
                                                                  ossim_uint32 resLevel)
{
   ossim_uint32 layerIdx = 0;
   if(!isSourceEnabled())
   {
      return ossimImageMosaic::getTile(rect, resLevel);
   }
   if(!theTile.valid())
   {
      allocate();
      if(!theTile.valid())
      {
         return 0;
      }
   }
   theTile->setImageRectangle(rect);
   theTile->makeBlank();
   std::vector<ossimClosestToCenterCombinerInfo > normTileList;
   ossimRefPtr<ossimImageData> currentTile = getNextNormTile(layerIdx, 0, rect);
   while(currentTile.valid())
   {
      normTileList.push_back(ossimClosestToCenterCombinerInfo((ossimImageData*)currentTile->dup(),
                                                              layerIdx));
      currentTile = getNextNormTile(layerIdx, rect, resLevel);
   }

   
   if(normTileList.size() == 1)
   {
      theTile->copyNormalizedBufferToTile((ossim_float32*)normTileList[0].theTile->getBuf());
   }
   else if(normTileList.size() > 1)
   {
      ossimRefPtr<ossimImageData> copyTile    = ossimImageDataFactory::instance()->create(0,
                                                                                          OSSIM_NORMALIZED_FLOAT);
      copyTile->setImageRectangleAndBands(rect,
                                          getNumberOfOutputBands());
      copyTile->initialize();
                                                                                          
      ossim_int32 idx   = 0;
      ossim_uint32 w     = rect.width();
      ossim_uint32 h     = rect.height();
      ossim_uint32 idxW  = 0;
      ossim_uint32 idxH  = 0;
      ossimIpt origin    = rect.ul();
      ossimIpt ulPt      = rect.ul();
      ossim_uint32 band  = 0;
      ossim_uint32 bands = copyTile->getNumberOfBands();
      ossim_uint32 srcBandIdx = 0;
      std::vector<ossim_float32*> bandList(bands);

      for(band = 0; band < bands; ++band)
      {
         bandList[band] = (ossim_float32*)copyTile->getBuf(band);
      }
      ossim_uint32 offset   = 0;
      origin.y = ulPt.y;
      for(idxH = 0; idxH < h; ++idxH)
      {
         origin.x = ulPt.x;
         for(idxW = 0; idxW < w;++idxW)
         {
            idx = findIdx(normTileList, origin, offset);

            if(idx >=0)
            {
               for(band = 0; band < bands; ++band)
               {
                  srcBandIdx = ossim::min(normTileList[idx].theTile->getNumberOfBands(), band);
                  
                  bandList[band][offset] = *(((ossim_float32*)normTileList[idx].theTile->getBuf(srcBandIdx))+offset);
               }
            }
            ++offset;
            ++origin.x;
         }
         ++origin.y;
      }
      theTile->copyNormalizedBufferToTile((ossim_float32*)copyTile->getBuf());
   }

   theTile->validate();
   
   return theTile;

}

ossim_int32 ossimClosestToCenterCombiner::findIdx(const std::vector<ossimClosestToCenterCombinerInfo >& normTileList,
                                                  const ossimIpt& pt, ossim_uint32 offset)const
{
   ossim_float32 distance = 1000000;
   ossim_int32 returnIdx = -1;
   ossim_float32 tempDistance;
   ossim_int32 idx = 0;
   ossim_int32 maxIdx = (ossim_int32)normTileList.size();
   ossimIpt midPt;
   ossimIrect rect;
   for(idx = 0; idx < maxIdx; ++ idx)
   {
      rect = theFullResBounds[ normTileList[idx].theIndex ];
      midPt = rect.midPoint();
      
      tempDistance = (pt-midPt).length();
      if(tempDistance < distance)
      {
         if(normTileList[idx].theTile->getDataObjectStatus() == OSSIM_FULL)
         {
            distance = tempDistance;
            returnIdx = idx;
         }
         else if(!normTileList[idx].theTile->isNull(offset))
         {
            distance = tempDistance;
            returnIdx = idx;
         }
      }
   }

   return returnIdx;
}
