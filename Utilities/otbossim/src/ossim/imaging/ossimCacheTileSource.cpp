//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts 
//
// Description:  ossimCacheTileSource
// 
//*******************************************************************
//  $Id: ossimCacheTileSource.cpp 20459 2012-01-17 01:30:36Z gpotts $

#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/imaging/ossimCacheTileSource.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>

static ossimTrace traceDebug("ossimCacheTileSource:debug");

static const ossimString TILE_SIZE_XY_KW("tile_size_xy");
static const ossimString USE_INPUT_TILE_SIZE_KW("use_input_tile_size");

RTTI_DEF1(ossimCacheTileSource, "ossimCacheTileSource", ossimImageSourceFilter);
   
ossimCacheTileSource::ossimCacheTileSource()
   : ossimImageSourceFilter(),
     //theCacheId(ossimAppFixedTileCache::instance()->newTileCache()),
     theTile(0),
     theFixedTileSize(),
     theCachingEnabled(true),
     theEventProgressFlag(false),
     theUseInputTileSizeFlag(false)
{
   ossim::defaultTileSize(theFixedTileSize);
}

ossimCacheTileSource::~ossimCacheTileSource()
{
   deleteRlevelCache();
   //ossimAppFixedTileCache::instance()->deleteCache(theCacheId);
   
   //theCacheId = -1;
  // theBoundingRect.makeNan();
}

void ossimCacheTileSource::flush()
{
   //ossimAppFixedTileCache::instance()->flush(theCacheId);
   ossim_uint32 idx = 0;
   for(idx = 0; idx < theRLevelCacheList.size();++idx)
   {
      ossimAppFixedTileCache::instance()->flush(theRLevelCacheList[idx]);
   }

}

void ossimCacheTileSource::initialize()
{
   ossimImageSourceFilter::initialize();
   flush();
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

ossimRefPtr<ossimImageData> ossimCacheTileSource::getTile(
   const ossimIrect& tileRect, ossim_uint32 resLevel)
{
   ossimRefPtr<ossimImageData> result = 0;
   
   if ( theInputConnection )
   {
      if ( isSourceEnabled() )
      {
         if(!theTile.valid())
         {
            allocate();
         }
         
         if (theTile.valid())
         {
            theTile->setImageRectangle(tileRect);
            theTile->makeBlank();
            // see if we can get a valid cache at the given resolution level
            if(getCacheId(resLevel) < 0)
            {
               return theInputConnection->getTile(tileRect, resLevel);
            }
            result = fillTile(resLevel);
         }
      }
      else // Not enabled...
      {
         result = theInputConnection->getTile(tileRect, resLevel);
      }

   } // End:  if ( theInputConnection )

   return result;
}

ossimRefPtr<ossimImageData> ossimCacheTileSource::fillTile(
   ossim_uint32 resLevel)
{
   ossimRefPtr<ossimImageData> tempTile = 0;
   fireProgressEvent(0.0);
   ossimIrect boundingRect = getBoundingRect(resLevel);
   ossimIrect tileRect = theTile->getImageRectangle();
   ossimIrect allignedRect = tileRect.clipToRect(boundingRect);
   ossimAppFixedTileCache::ossimAppFixedCacheId cacheId = getCacheId(resLevel); 
   ossimIpt cacheTileSize =  ossimAppFixedTileCache::instance()->getTileSize(cacheId);
   if ( !allignedRect.hasNans() )
   {
      allignedRect.stretchToTileBoundary(cacheTileSize);
     // ossimAppFixedTileCache::ossimAppFixedCacheId cacheId = theCacheId;
      
      // check to see if we need to loop
      if((allignedRect == tileRect)&&
         (static_cast<ossim_int32>(tileRect.width())  == cacheTileSize.x)&&
         (static_cast<ossim_int32>(tileRect.height()) == cacheTileSize.y))
      {
         ossimIpt origin = tileRect.ul();
         if(theCachingEnabled)
         {
            tempTile = ossimAppFixedTileCache::instance()->getTile(cacheId,
                                                                   origin);
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
                  ossimAppFixedTileCache::instance()->addTile(cacheId,
                                                              tempTile);
               }
            }
         }
//         else
//         {
//            std::cout << "FOUND IN CACHE AT RES " << resLevel << "\n";
//         }
         
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
         
         ossim_int32 boundaryHeight = allignedRect.height();
         ossim_int32 boundaryWidth  = allignedRect.width();
         ossimIpt origin(allignedRect.ul());
         ossim_int32 totalTiles  = (boundaryHeight/cacheTileSize.y)*
            (boundaryWidth/theFixedTileSize.x);
         ossim_int32 currentTile = 0;
         for(ossim_int32 row = 0;
             row < boundaryHeight;
             row+=theFixedTileSize.y)
         {
            origin.x  =  allignedRect.ul().x;
            for(ossim_int32 col = 0;
                col < boundaryWidth;
                col+=theFixedTileSize.x)
            {
               ++currentTile;
               if(theCachingEnabled)
               {
                  tempTile =
                     ossimAppFixedTileCache::instance()->getTile(cacheId,
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
                                  origin.x + cacheTileSize.x-1,
                                  origin.y + cacheTileSize.y-1);
                  
                  tempTile = theInputConnection->getTile(rect, resLevel);
                  
                  if(tempTile.valid())
                  {
                     // drb if(theTile->getBuf()&&
                     if(tempTile->getBuf()&&                        
                        (tempTile->getDataObjectStatus()!=OSSIM_EMPTY)&&
                        theCachingEnabled)
                     {
                        ossimAppFixedTileCache::instance()->
                           addTile(cacheId, tempTile);
                     }
                  }
               }
             //  else
             //  {
             //     std::cout << "FOUND IN CACHE AT RES " << resLevel << "\n";
             // }
               
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
         theTile->validate();
         fireProgressEvent(100);
      }
      
   } // End of:  if ( !allignedRect.hasNans() )
   
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
   //ossimAppFixedTileCache::instance()->deleteCache(theCacheId);
   
   //theCacheId = ossimAppFixedTileCache::instance()->newTileCache();
   
   deleteRlevelCache();
   const char* lookup = kwl.find(prefix, ossimKeywordNames::ENABLE_CACHE_KW);
   if(lookup)
   {
      theCachingEnabled = ossimString(lookup).toBool();
   }
   lookup = kwl.find(prefix, USE_INPUT_TILE_SIZE_KW);
   if(lookup)
   {
      theUseInputTileSizeFlag = ossimString(lookup).toBool();
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
           USE_INPUT_TILE_SIZE_KW,
           theUseInputTileSizeFlag,
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
   
   if (name == TILE_SIZE_XY_KW)
   {
      ossimRefPtr<ossimProperty> result =
         new ossimStringProperty(name, theFixedTileSize.toString());
      result->setCacheRefreshBit();
      return result;
   }
   else if(name == ossimKeywordNames::ENABLE_CACHE_KW)
   {
      ossimRefPtr<ossimProperty> result = new ossimBooleanProperty(name,
                                                                   theCachingEnabled);
      result->setCacheRefreshBit();
      return result;
   }
   else if(name == USE_INPUT_TILE_SIZE_KW)
   {
      ossimRefPtr<ossimProperty> result = new ossimBooleanProperty(name,
                                                                   theUseInputTileSizeFlag);
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

      if ( (pt.x > 7) && (pt.y > 7) )
      {
         setTileSize(pt);
      }
      else
      {
         ossimNotify(ossimNotifyLevel_NOTICE)
            << "ossimCacheTileSource::setProperty NOTICE:"
            << "\nTile dimensions must be at least 8!"
            << "\nFormat = ( x, y )"
            << std::endl;
      }
   }
   else if(name == ossimKeywordNames::ENABLE_CACHE_KW)
   {
      setCachingEnabledFlag(property->valueToString().toBool());
   }
   else if(name == USE_INPUT_TILE_SIZE_KW)
   {
      theUseInputTileSizeFlag = property->valueToString().toBool();
      if(theUseInputTileSizeFlag)
      {
         deleteRlevelCache();
      }
     // setCachingEnabledFlag(property->valueToString().toBool());
   }
   else
   {
      ossimImageSourceFilter::setProperty(property);
   }
}

void ossimCacheTileSource::getPropertyNames(
   std::vector<ossimString>& propertyNames)const
{
   propertyNames.push_back(TILE_SIZE_XY_KW);
   propertyNames.push_back(ossimKeywordNames::ENABLE_CACHE_KW);
   propertyNames.push_back(USE_INPUT_TILE_SIZE_KW);
   
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
      initializeRlevelCache();
     // ossimAppFixedTileCache::instance()->setTileSize(theCacheId, size);
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

ossimAppFixedTileCache::ossimAppFixedCacheId ossimCacheTileSource::getCacheId(ossim_uint32 resLevel)
{
   ossimAppFixedTileCache::ossimAppFixedCacheId result = -1;
   if(theRLevelCacheList.empty())
   {
      initializeRlevelCache();
   }
   if(resLevel < theRLevelCacheList.size())
   {
      if(theRLevelCacheList[resLevel] <0)
      {
         ossimIpt cacheTileSize(theFixedTileSize);
         if(theUseInputTileSizeFlag)
         {
            cacheTileSize = ossimIpt(ossim::min(static_cast<ossim_int32>(theInputConnection->getTileWidth()),
                                                static_cast<ossim_int32>(1024)), 
                                     ossim::min(static_cast<ossim_int32>(theInputConnection->getTileHeight()),
                                                static_cast<ossim_int32>(1024)));
            
         }

         //std::cout << cacheTileSize << std::endl;
         ossimIrect rect = getBoundingRect(resLevel);
         
         if((cacheTileSize.x > static_cast<ossim_int64>(rect.width()))&&
            (cacheTileSize.y > static_cast<ossim_int64>(rect.height())))
         {
            cacheTileSize.x = ossim::max(static_cast<ossim_int64>(rect.width()), 
                                         static_cast<ossim_int64>(64));
            cacheTileSize.y = ossim::max(static_cast<ossim_int64>(rect.height()), 
                                         static_cast<ossim_int64>(64));
         }

         rect.stretchToTileBoundary(theFixedTileSize);
         theRLevelCacheList[resLevel] = ossimAppFixedTileCache::instance()->newTileCache(rect, cacheTileSize); 
      }
      result = theRLevelCacheList[resLevel];
   }
   
   return result;
}

void ossimCacheTileSource::deleteRlevelCache()
{
   ossim_uint32 idx = 0;
   for(idx = 0; idx < theRLevelCacheList.size();++idx)
   {
      ossimAppFixedTileCache::instance()->deleteCache(theRLevelCacheList[idx]);
   }
   theRLevelCacheList.clear();
}

void ossimCacheTileSource::initializeRlevelCache()
{
   ossim_uint32 nLevels = getNumberOfDecimationLevels();
   deleteRlevelCache();
   
   if(nLevels > 0)
   {
      ossim_uint32 idx = 0;
      theRLevelCacheList.resize(nLevels);
      for(idx= 0; idx < theRLevelCacheList.size(); ++idx)
      {
         theRLevelCacheList[idx] = -1;
      }
   }
}

