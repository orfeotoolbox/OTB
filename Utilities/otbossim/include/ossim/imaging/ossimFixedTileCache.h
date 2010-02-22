//******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: This file contains the Application cache algorithm
//
//***********************************
// $Id: ossimFixedTileCache.h 16276 2010-01-06 01:54:47Z gpotts $
#ifndef ossimFixedTileCache_HEADER
#define ossimFixedTileCache_HEADER
#include <map>
#include <list>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/imaging/ossimImageData.h>
#include <OpenThreads/Mutex>

class  ossimFixedTileCacheInfo
{
public:
   ossimFixedTileCacheInfo(ossimRefPtr<ossimImageData>& tile,
                           ossim_int32 tileId=-1)
      :theTile(tile),
      theTileId(tileId)
      {
      }
   
   bool operator <(const ossimFixedTileCacheInfo& rhs)const
      {
         return (theTileId < rhs.theTileId);
      }
   bool operator <(ossim_int32 tileId)const
      {
         return (theTileId < tileId);
      }
   bool operator >(const ossimFixedTileCacheInfo& rhs)const
      {
         return (theTileId > rhs.theTileId);
      }
   bool operator >(ossim_int32 tileId)const
      {
         return (theTileId > tileId);
      }
   bool operator ==(const ossimFixedTileCacheInfo& rhs)const
      {
         return (theTileId == rhs.theTileId);
      }
   bool operator ==(ossim_int32 tileId)const
      {
         return (theTileId == tileId);
      }
   
   ossimRefPtr<ossimImageData> theTile;
   ossim_int32 theTileId;
};

class ossimFixedTileCache : public ossimReferenced
{
public:
   ossimFixedTileCache();
   virtual void setRect(const ossimIrect& rect);
   virtual void setRect(const ossimIrect& rect,
                        const ossimIpt& tileSize);
   void keepTilesWithinRect(const ossimIrect& rect);
   virtual ossimRefPtr<ossimImageData> addTile(ossimRefPtr<ossimImageData> imageData,
                                               bool duplicateData=true);
   virtual ossimRefPtr<ossimImageData> getTile(ossim_int32 id);
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIpt& origin)
      {
         return getTile(computeId(origin));
      }
   virtual void setUseLruFlag(bool flag)
      {
         theUseLruFlag = flag;
      }
   virtual bool getUseLruFlag()const
      {
         return theUseLruFlag;
      }
   virtual void flush();
   virtual void deleteTile(const ossimIpt& origin)
      {
         deleteTile(computeId(origin));
      }
   virtual void deleteTile(ossim_int32 tileId);
   virtual ossimRefPtr<ossimImageData> removeTile(const ossimIpt& origin)
      {
         return removeTile(computeId(origin));
      }
   virtual ossimRefPtr<ossimImageData> removeTile(ossim_int32 tileId);
   virtual const ossimIrect& getTileBoundaryRect()const
      {
         return theTileBoundaryRect;
      }
   virtual ossim_uint32 getNumberOfTiles()const
      {
         return (ossim_uint32)theTileMap.size();
      }
   virtual const ossimIpt& getTileSize()const
      {
         return theTileSize;
      }
   virtual ossim_uint32 getCacheSize()const
      {
         return theCacheSize;
      }
   virtual void deleteTile();
   virtual ossimRefPtr<ossimImageData> removeTile();
   
   virtual void setMaxCacheSize(ossim_uint32 cacheSize)
      {
         theMaxCacheSize = cacheSize;
      }

   ossim_uint32 getMaxCacheSize()const
      {
         return theMaxCacheSize;
      }
   
   virtual ossimIpt getTileOrigin(ossim_int32 tileId);
   virtual ossim_int32 computeId(const ossimIpt& tileOrigin)const;
   virtual void setTileSize(const ossimIpt& tileSize);
protected:
   virtual ~ossimFixedTileCache();
   OpenThreads::Mutex theMutex;
   ossimIrect   theTileBoundaryRect;
   ossimIpt     theTileSize;
   ossimIpt     theBoundaryWidthHeight;
   ossim_uint32 theTilesHorizontal;
   ossim_uint32 theTilesVertical;
   ossim_uint32 theCacheSize;
   ossim_uint32 theMaxCacheSize;
   std::map<ossim_int32, ossimFixedTileCacheInfo> theTileMap;
   std::list<ossim_int32> theLruQueue;
   bool                   theUseLruFlag;
   virtual void eraseFromLru(ossim_int32 id);
   void adjustLru(ossim_int32 id);
};

#endif
