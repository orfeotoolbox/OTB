//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts 
//
// Description:  ossimCacheTileSource
// 
//*******************************************************************
//  $Id: ossimCacheTileSource.cpp 12623 2008-04-07 14:10:08Z gpotts $

#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/imaging/ossimCacheTileSource.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>

static ossimTrace traceDebug("ossimCacheTileSource:debug");

static const ossimString TILE_SIZE_XY_KW("tile_size_xy");

RTTI_DEF1(ossimCacheTileSource, "ossimCacheTileSource", ossimImageSourceFilter);
   
ossimCacheTileSource::ossimCacheTileSource()
   : ossimImageSourceFilter(),
     theCacheId(ossimAppFixedTileCache::instance()->newTileCache()),
     theTile(0),
     theFixedTileSize(),
     theCachingEnabled(true),
     theEventProgressFlag(false),
     theCacheRLevel(0),
     theBoundingRect()     
{
   theBoundingRect.makeNan();
   theFixedTileSize = ossimAppFixedTileCache::instance()->getTileSize(theCacheId);
}

// ossimCacheTileSource::ossimCacheTileSource(ossimImageSource* inputSource)
//    : ossimImageSourceFilter(inputSource),
//      theCacheId(ossimAppFixedTileCache::instance()->newTileCache()),
//      theTile(0),
//      theCachingEnabled(true),
//      theEventProgressFlag(false)
// {
//    theBoundingRect.makeNan();
//    theFixedTileSize = ossimAppFixedTileCache::instance()->getTileSize(theCacheId);
//    theCacheRLevel = 0;
// }

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
   theTile = 0;
}
   
void ossimCacheTileSource::allocate()
{
   theTile = 0;
   if(theInputConnection)
   {
      theTile = ossimImageDataFactory::instance()->create(this, this);
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
      allocate();  // Do we need this??? (drb)
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
   ossimRefPtr<ossimImageData> tempTile = 0;
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
	OpenThreads::ScopedLock<OpenThreads::Mutex> scopeLock(theMutex);
   return theFixedTileSize.x;
}

ossim_uint32 ossimCacheTileSource::getTileHeight() const
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> scopeLock(theMutex);
   return theFixedTileSize.y;  
}

bool ossimCacheTileSource::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   ossimAppFixedTileCache::instance()->deleteCache(theCacheId);
   
   theCacheId = ossimAppFixedTileCache::instance()->newTileCache();
   
   const char* lookup = kwl.find(prefix, ossimKeywordNames::ENABLE_CACHE_KW);
   if(lookup)
   {
      theCachingEnabled = ossimString(lookup).toBool();
   }

   lookup = kwl.find(prefix, TILE_SIZE_XY_KW);
   if (lookup)
   {
      ossimIpt pt;
      pt.toPoint(std::string(lookup));
      setTileSize(pt);
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
           theCachingEnabled,
           true);
   
   kwl.add(prefix,
           TILE_SIZE_XY_KW,
           theFixedTileSize.toString().c_str());
   
   return ossimImageSourceFilter::saveState(kwl, prefix);
}

ossimRefPtr<ossimProperty> ossimCacheTileSource::getProperty(
   const ossimString& name)const
{
   // Lock for the length of this method.
	OpenThreads::ScopedLock<OpenThreads::Mutex> scopeLock(theMutex);
   
   if (name == TILE_SIZE_XY_KW)
   {
      ossimRefPtr<ossimProperty> result =
         new ossimStringProperty(name, theFixedTileSize.toString());
      result->setCacheRefreshBit();
      return result;
   }

   return ossimImageSourceFilter::getProperty(name);
}

void ossimCacheTileSource::setProperty(ossimRefPtr<ossimProperty> property)
{
   if (!property) return;

   ossimString name = property->getName();
   if (name == TILE_SIZE_XY_KW)
   {
      ossimIpt pt;
      pt.toPoint(property->valueToString());

      // Rule: Must be positive and at least 32.
      if ( (pt.x > 31) && (pt.y > 31) )
      {
         setTileSize(pt);
      }
      else
      {
         ossimNotify(ossimNotifyLevel_NOTICE)
            << "ossimCacheTileSource::setProperty NOTICE:"
            << "\nTile dimensions must be at least 32!"
            << "\nFormat = ( x, y )"
            << std::endl;
      }
   }

   ossimImageSourceFilter::setProperty(property);
}

void ossimCacheTileSource::getPropertyNames(
   std::vector<ossimString>& propertyNames)const
{
   propertyNames.push_back(TILE_SIZE_XY_KW);
   
   ossimImageSourceFilter::getPropertyNames(propertyNames);
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

void ossimCacheTileSource::getTileSize(ossimIpt& size) const
{
   size = theFixedTileSize;
}

void ossimCacheTileSource::setTileSize(const ossimIpt& size)
{
   if (size != theFixedTileSize)
   {
      theTile = 0; // Force an allocate of new tile.
      theFixedTileSize = size;
      ossimAppFixedTileCache::instance()->setTileSize(theCacheId, size);
   }
}

void ossimCacheTileSource::fireProgressEvent(double percentComplete)
{
   if(theEventProgressFlag)
   {
      ossimProcessProgressEvent event(this, percentComplete);
      fireEvent(event);
   }
}
