//******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: This file contains the Application cache algorithm
//
//***********************************
// $Id: ossimFixedTileCache.cpp 16276 2010-01-06 01:54:47Z gpotts $
#include <ossim/imaging/ossimFixedTileCache.h>
#include <algorithm>

ossimFixedTileCache::ossimFixedTileCache()
   : theTileBoundaryRect(),
     theTileSize(),
     theBoundaryWidthHeight(),
     theTilesHorizontal(0),
     theTilesVertical(0),
     theCacheSize(0),
     theMaxCacheSize(0),
     theTileMap(),
     theLruQueue(),
     theUseLruFlag(true)
{
   ossim::defaultTileSize(theTileSize);

   ossimIrect tempRect;
   tempRect.makeNan();

   setRect(tempRect);
   
   // theCacheSize    = 0;
   // theMaxCacheSize = 0;
   // theUseLruFlag = true;
}

ossimFixedTileCache::~ossimFixedTileCache()
{
   flush();
}

void ossimFixedTileCache::setRect(const ossimIrect& rect)
{
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
      ossim::defaultTileSize(theTileSize);
      theTileBoundaryRect      = rect;
      theTileBoundaryRect.stretchToTileBoundary(theTileSize);
      theBoundaryWidthHeight.x = theTileBoundaryRect.width();
      theBoundaryWidthHeight.y = theTileBoundaryRect.height();
      theTilesHorizontal       = theBoundaryWidthHeight.x/theTileSize.x;
      theTilesVertical         = theBoundaryWidthHeight.y/theTileSize.y;
   }
   flush();
}

void ossimFixedTileCache::setRect(const ossimIrect& rect,
                                  const ossimIpt& tileSize)
{
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
      theTileBoundaryRect      = rect;
      theTileSize              = tileSize;
      theTileBoundaryRect.stretchToTileBoundary(theTileSize);
      theBoundaryWidthHeight.x = theTileBoundaryRect.width();
      theBoundaryWidthHeight.y = theTileBoundaryRect.height();
      theTilesHorizontal       = theBoundaryWidthHeight.x/theTileSize.x;
      theTilesVertical         = theBoundaryWidthHeight.y/theTileSize.y;
   }
   flush();
}


void ossimFixedTileCache::keepTilesWithinRect(const ossimIrect& rect)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   std::map<ossim_int32, ossimFixedTileCacheInfo>::iterator tileIter = theTileMap.begin();

   while(tileIter != theTileMap.end())
   {
      ossimIrect tileRect = (*tileIter).second.theTile->getImageRectangle();
      if(!tileRect.intersects(rect))
      {
         eraseFromLru(computeId((*tileIter).second.theTile->getOrigin()));
         theCacheSize -= (*tileIter).second.theTile->getDataSizeInBytes();
         (*tileIter).second.theTile = NULL;
         theTileMap.erase(tileIter);
      }
      ++tileIter;
   }
}

ossimRefPtr<ossimImageData> ossimFixedTileCache::addTile(ossimRefPtr<ossimImageData> imageData,
                                                         bool duplicateData)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   ossimRefPtr<ossimImageData> result = NULL;
   if(!imageData.valid())
   {
      return result;
   }
   if(!imageData->getBuf())
   {
      return result;
   }
   
   ossim_int32 id = computeId(imageData->getOrigin());
   if(id < 0)
   {
      return result;
   }
   
   std::map<ossim_int32, ossimFixedTileCacheInfo>::iterator tileIter =
      theTileMap.find(id);

   if(tileIter==theTileMap.end())
   {
      if(duplicateData)
      {
         result = (ossimImageData*)imageData->dup();
      }
      else
      {
         result = imageData;
      }
      ossimFixedTileCacheInfo cacheInfo(result, id);
       
      theCacheSize += imageData->getDataSizeInBytes();
      theTileMap.insert(make_pair(id, cacheInfo));
      if(theUseLruFlag)
      {
         theLruQueue.push_back(id);
      }
   }
   
   return result;
}

ossimRefPtr<ossimImageData> ossimFixedTileCache::getTile(ossim_int32 id)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   ossimRefPtr<ossimImageData> result = NULL;

   std::map<ossim_int32, ossimFixedTileCacheInfo>::iterator tileIter =
      theTileMap.find(id);
   if(tileIter!=theTileMap.end())
   {
      result = (*tileIter).second.theTile;
      adjustLru(id);
   }

   return result;
}

ossimIpt ossimFixedTileCache::getTileOrigin(ossim_int32 tileId)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   ossimIpt result;
   result.makeNan();

   if(tileId < 0)
   {
      return result;
   }
   ossim_int32 ty = (tileId/theTilesHorizontal);
   ossim_int32 tx = (tileId%theTilesVertical);
   
   ossimIpt ul = theTileBoundaryRect.ul();
   
   result = ossimIpt(ul.x + tx*theTileSize.x, ul.y + ty*theTileSize.y);

   return result;
}

ossim_int32 ossimFixedTileCache::computeId(const ossimIpt& tileOrigin)const
{
   ossimIpt idDiff = tileOrigin - theTileBoundaryRect.ul();

   if((idDiff.x < 0)||
      (idDiff.y < 0)||
      (idDiff.x >= theBoundaryWidthHeight.x)||
      (idDiff.y >= theBoundaryWidthHeight.y))
     {
       return -1;
     }
   ossim_uint32 y = idDiff.y/theTileSize.y;
   y*=theTilesHorizontal;

   ossim_uint32 x = idDiff.x/theTileSize.x;
   
   
   return (y + x);
}

void ossimFixedTileCache::deleteTile(ossim_int32 tileId)
{
   std::map<ossim_int32, ossimFixedTileCacheInfo>::iterator tileIter =
      theTileMap.find(tileId);

   if(tileIter != theTileMap.end())
   {
      if((*tileIter).second.theTile.valid())
      {
         theCacheSize -= (*tileIter).second.theTile->getDataSizeInBytes();
         (*tileIter).second.theTile = NULL;
      }
      theTileMap.erase(tileIter);
      eraseFromLru(tileId);
   }
}

ossimRefPtr<ossimImageData> ossimFixedTileCache::removeTile(ossim_int32 tileId)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   ossimRefPtr<ossimImageData> result = NULL;
   
   std::map<ossim_int32, ossimFixedTileCacheInfo>::iterator tileIter =
      theTileMap.find(tileId);

   if(tileIter != theTileMap.end())
   {
      theCacheSize -= (*tileIter).second.theTile->getDataSizeInBytes();
      if((*tileIter).second.theTile.valid())
      {
         result =  (*tileIter).second.theTile;
      }
      theTileMap.erase(tileIter);
      eraseFromLru(tileId);
   }
   
   return result;
}

void ossimFixedTileCache::flush()
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   std::map<ossim_int32, ossimFixedTileCacheInfo>::iterator tileIter =
      theTileMap.begin();

   while(tileIter != theTileMap.end())
   {
      if( (*tileIter).second.theTile.valid())
      {
         (*tileIter).second.theTile = 0;
      }
      ++tileIter;
   }
   theLruQueue.clear();
   theTileMap.clear();
   theCacheSize = 0;
}

void ossimFixedTileCache::deleteTile()
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   if(theUseLruFlag)
   {
      if(theLruQueue.begin() != theLruQueue.end())
      {
         deleteTile(*(theLruQueue.begin()));
      }
   }
}

ossimRefPtr<ossimImageData> ossimFixedTileCache::removeTile()
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   if(theUseLruFlag)
   {
      if(theLruQueue.begin() != theLruQueue.end())
      {
         return removeTile(*(theLruQueue.begin()));
      }
   }

   return NULL;
}

void ossimFixedTileCache::adjustLru(ossim_int32 id)
{
   if(theUseLruFlag)
   {
      std::list<ossim_int32>::iterator iter = std::find(theLruQueue.begin(), theLruQueue.end(), id);
      
      if(iter != theLruQueue.end())
      {
         ossim_int32 value = *iter;
         theLruQueue.erase(iter);
         theLruQueue.push_back(value);
      }
   }
}

void ossimFixedTileCache::eraseFromLru(ossim_int32 id)
{
   if(theUseLruFlag)
   {
      
      std::list<ossim_int32>::iterator iter = std::find(theLruQueue.begin(), theLruQueue.end(), id);
      
      if(iter != theLruQueue.end())
      {
         theLruQueue.erase(iter);
      }
   }
}


void ossimFixedTileCache::setTileSize(const ossimIpt& tileSize)
{
   setRect(theTileBoundaryRect, tileSize);
}
