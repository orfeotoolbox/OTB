//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts 
//
// Description:
//
// ossimCacheTileSource.
//*******************************************************************
//  $Id: ossimCacheTileSource.cpp,v 1.55 2005/08/17 19:18:12 gpotts Exp $
#include <base/common/ossimTrace.h>
#include <imaging/tile_sources/ossimCacheTileSource.h>
#include <imaging/ossimImageData.h>
#include <imaging/factory/ossimImageDataFactory.h>
#include <base/common/ossimKeywordNames.h>
#include <base/data_types/ossimKeywordlist.h>

#ifndef NULL
#  include <stddef.h>
#endif
static ossimTrace traceDebug("ossimCacheTileSource:debug");

RTTI_DEF1(ossimCacheTileSource, "ossimCacheTileSource", ossimImageSourceFilter);
   
ossimCacheTileSource::ossimCacheTileSource()
   : ossimImageSourceFilter(),
     theCacheId(ossimAppFixedTileCache::instance()->newTileCache()),
     theTile(0),
     theCachingEnabled(true),
     theEventProgressFlag(false)
{
   theBoundingRect.makeNan();
   theFixedTileSize = ossimAppFixedTileCache::instance()->getTileSize(theCacheId);
   theCacheRLevel = 0;
}

ossimCacheTileSource::ossimCacheTileSource(ossimImageSource* inputSource)
   : ossimImageSourceFilter(inputSource),
     theCacheId(ossimAppFixedTileCache::instance()->newTileCache()),
     theTile(0),
     theCachingEnabled(true),
     theEventProgressFlag(false)
{
   theBoundingRect.makeNan();
   theFixedTileSize = ossimAppFixedTileCache::instance()->getTileSize(theCacheId);
   theCacheRLevel = 0;
}

ossimCacheTileSource::~ossimCacheTileSource()
{
   ossimAppFixedTileCache::instance()->deleteCache(theCacheId);
   
   theCacheId = -1;
   theBoundingRect.makeNan();
}

void ossimCacheTileSource::flush()
{
   ossimAppFixedTileCache::instance()->flush(theCacheId);
}

void ossimCacheTileSource::initialize()
{
   ossimImageSourceFilter::initialize();
   flush();
   theCacheRLevel = 999999999;

   theTile = NULL;
}
   
void ossimCacheTileSource::allocate()
{
   theTile = NULL;
   if(theInputConnection)
   {
      theTile = ossimImageDataFactory::instance()->create(this,
                                                          this);
      theTile->initialize();
   }
}

ossimRefPtr<ossimImageData> ossimCacheTileSource::getTile(const ossimIrect& tileRect,
                                              ossim_uint32 resLevel)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimCacheTileSource::getTile DEBUG: Entered ..." << std::endl;
   }
   if(!theInputConnection)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "ossimCacheTileSource::getTile DEBUG: Leaving with theInputConnection NULL ..." << std::endl;
      }
      return 0;
   }
   
   if(!isSourceEnabled())
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "ossimCacheTileSource::getTile DEBUG: Source not enabled so leaving ..." << std::endl;
      }
      return theInputConnection->getTile(tileRect, resLevel);
   }
   
   if(!theTile.get())
   {
      allocate();
   }

   if(!theTile.get())
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "ossimCacheTileSource::getTile DEBUG: Unable to allocate tile so leaving ..." << std::endl;
      }
      return 0;
   }

   if(resLevel != theCacheRLevel)
   {
      flush();
      theCacheRLevel = 9999999;
      allocate();
      theBoundingRect = getBoundingRect(resLevel);
      if(theBoundingRect.hasNans())
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << "ossimCacheTileSource::getTile DEBUG: bounding rect has nans so leaving ..." << std::endl;
         }
         return 0;
      }
      theFixedTileSize = ossimAppFixedTileCache::instance()->getTileSize(theCacheId);
      theBoundingRect.stretchToTileBoundary(theFixedTileSize);
      ossimAppFixedTileCache::instance()->setRect(theCacheId,
                                                  theBoundingRect);
      theCacheRLevel = resLevel;
   }

   theTile->setImageRectangle(tileRect);
   theTile->makeBlank();

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimCacheTileSource::getTile DEBUG: filling tile and leaving ..." << std::endl;
   }
   return fillTile(resLevel);
}

ossimRefPtr<ossimImageData> ossimCacheTileSource::fillTile(
   ossim_uint32 resLevel)
{
   ossimRefPtr<ossimImageData> tempTile = NULL;
   fireProgressEvent(0.0);
   ossimIrect boundingRect = theBoundingRect;
   ossimIrect tileRect = theTile->getImageRectangle();
   
   ossimIrect allignedRect = tileRect.clipToRect(boundingRect);
   
   if (allignedRect.hasNans())
   {
      theTile->makeBlank();
      return theTile;
   }
   
   allignedRect.stretchToTileBoundary(theFixedTileSize);
   ossimAppFixedTileCache::ossimAppFixedCacheId cacheId = theCacheId;
   
   // check to see if we need to loop
   if((allignedRect == tileRect)&&
      (static_cast<ossim_int32>(tileRect.width())  == theFixedTileSize.x)&&
      (static_cast<ossim_int32>(tileRect.height()) == theFixedTileSize.y))
   {
      ossimIpt origin = tileRect.ul();
      if(theCachingEnabled)
      {
         tempTile = ossimAppFixedTileCache::instance()->getTile(cacheId, origin);
      }
      if(!tempTile.valid())
      {
         tempTile = theInputConnection->getTile(tileRect, resLevel);
         
         if(tempTile.valid())
         {
            if((tempTile->getBuf())&&
               (tempTile->getDataObjectStatus()!=OSSIM_EMPTY)&&
               theCachingEnabled)
            {
               ossimAppFixedTileCache::instance()->addTile(cacheId, tempTile);
            }
         }
      }
      
      if(tempTile.valid())
      {
         if((tempTile->getDataObjectStatus() != OSSIM_NULL)&&
            (tempTile->getDataObjectStatus() != OSSIM_EMPTY))
         {
            
            theTile->setDataObjectStatus(tempTile->getDataObjectStatus());
            theTile->loadTile(tempTile.get());
         }
      }
      fireProgressEvent(100.0);
   }
   else
   {
      long boundaryHeight = allignedRect.height();
      long boundaryWidth  = allignedRect.width();
      ossimIpt origin(allignedRect.ul());
      long totalTiles  = (boundaryHeight/theFixedTileSize.y)*
         (boundaryWidth/theFixedTileSize.x);
      long currentTile = 0;
      for(long row = 0; row < boundaryHeight; row+=theFixedTileSize.y)
      {
         origin.x  =  allignedRect.ul().x;
         for(long col = 0; col < boundaryWidth; col+=theFixedTileSize.x)
         {
            ++currentTile;
            if(theCachingEnabled)
            {
               tempTile = ossimAppFixedTileCache::instance()->getTile(cacheId,
                                                                      origin);
            }
            else
            {
               tempTile = 0;
            }
            if(!tempTile.valid())
            {
               ossimIrect rect(origin.x,
                               origin.y,
                               origin.x + theFixedTileSize.x-1,
                               origin.y + theFixedTileSize.y-1);

               tempTile = theInputConnection->getTile(rect, resLevel);
               
               if(tempTile.valid())
               {
                  if(theTile->getBuf()&&
                     (tempTile->getDataObjectStatus()!=OSSIM_EMPTY)&&
                     theCachingEnabled)
                  {
                     ossimAppFixedTileCache::instance()->addTile(cacheId, tempTile);
                  }
               }
            }
            if(tempTile.valid())
            {
               if(tempTile->getBuf()&&
                  (tempTile->getDataObjectStatus()!=OSSIM_EMPTY))
               {
                  theTile->loadTile(tempTile.get());
               }
            }
            double percent = 100.0*((double)currentTile/(double)totalTiles);
            fireProgressEvent(percent);
            origin.x   += theFixedTileSize.x;
         }
         origin.y += theFixedTileSize.y;
      }
      fireProgressEvent(100);
   }
   
   theTile->validate();
   
   return theTile;
}

ossim_uint32 ossimCacheTileSource::getTileWidth() const
{
   return theFixedTileSize.x;
}

ossim_uint32 ossimCacheTileSource::getTileHeight() const
{
   return theFixedTileSize.y;  
}

bool ossimCacheTileSource::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   ossimAppFixedTileCache::instance()->deleteCache(theCacheId);
   theCacheId = ossimAppFixedTileCache::instance()->newTileCache();
   const char* cachingEnabled = kwl.find(prefix, ossimKeywordNames::ENABLE_CACHE_KW);
   
   if(cachingEnabled)
   {
      theCachingEnabled = ossimString(cachingEnabled).toBool();
   }
   
   bool result = ossimImageSourceFilter::loadState(kwl, prefix);
   initialize();
   return result;
}

bool ossimCacheTileSource::saveState(ossimKeywordlist& kwl,
                                     const char* prefix)const
{
   kwl.add(prefix,
           ossimKeywordNames::ENABLE_CACHE_KW,
           (long)theCachingEnabled,
           true);
   
   return ossimImageSourceFilter::saveState(kwl, prefix);
}


ossimString ossimCacheTileSource::getLongName()const
{
   return ossimString("Tile Cache , cache for ossimImageData objects.");
}

ossimString ossimCacheTileSource::getShortName()const
{
   return ossimString("Tile Cache");
}

void ossimCacheTileSource::setCachingEnabledFlag(bool value)
{
   if(!value && theCachingEnabled)
   {
      flush();
   }
   theCachingEnabled = value;
}

void ossimCacheTileSource::setEventProgressFlag(bool value)
{
   theEventProgressFlag = value;
}

ossimIpt ossimCacheTileSource::getTileSize()
{
   return theFixedTileSize;
}

void ossimCacheTileSource::setTileSize(const ossimIpt& size)
{
   theFixedTileSize = size;
}

void ossimCacheTileSource::fireProgressEvent(double percentComplete)
{
   if(theEventProgressFlag)
   {
      ossimProcessProgressEvent event(this, percentComplete);
      fireEvent(event);
   }
}
