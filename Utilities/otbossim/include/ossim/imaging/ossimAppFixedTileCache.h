//******************************************************************
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
// $Id: ossimAppFixedTileCache.h 20127 2011-10-12 11:27:10Z gpotts $
#ifndef ossimAppFixedTileCache_HEADER
#define ossimAppFixedTileCache_HEADER
#include <map>
#include <list>
#include <iostream>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimIrect.h>
#include <OpenThreads/ReadWriteMutex>

class ossimFixedTileCache;
class ossimImageData;

class OSSIM_DLL ossimAppFixedTileCache
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                                    const ossimAppFixedTileCache& rhs);
   static const ossim_uint32 DEFAULT_SIZE;
   typedef ossim_int32 ossimAppFixedCacheId;
   static ossimAppFixedTileCache *instance(ossim_uint32  maxSize   = 0);
   virtual ~ossimAppFixedTileCache();
   
   /*!
    * Will flush all cache registered
    */
   virtual void flush();
   virtual void flush(ossimAppFixedCacheId cacheId);
   virtual void deleteCache(ossimAppFixedCacheId cacheId);
   /*!
    * Will create a new Tile cache for this application if the tile size is 0,0 it will
    * use the default tile size.  Will
    * return 0 if not successful.
    */
   ossimAppFixedCacheId newTileCache(const ossimIrect& tileBoundaryRect,
                                     const ossimIpt& tileSize=ossimIpt(0,0));
   ossimAppFixedCacheId newTileCache();

   virtual void setRect(ossimAppFixedCacheId cacheId,
                        const ossimIrect& boundaryTileRect);
   virtual void setTileSize(ossimAppFixedCacheId cacheId,
                            const ossimIpt& tileSize);
   
   ossimRefPtr<ossimImageData> getTile(ossimAppFixedCacheId cacheId,
                           const ossimIpt& origin);
   ossimRefPtr<ossimImageData> addTile(ossimAppFixedCacheId cacheId,
                                       ossimRefPtr<ossimImageData> data,
                                       bool duplicateData=true);
   
   ossimRefPtr<ossimImageData> removeTile(ossimAppFixedCacheId cacheId,
                                          const ossimIpt& origin);
   void deleteTile(ossimAppFixedCacheId cacheId,
                   const ossimIpt& origin);
   
   const ossimIpt& getTileSize(ossimAppFixedCacheId cacheId);
   
   virtual void setMaxCacheSize(ossim_uint32 cacheSize);
   
protected:
//    struct ossimAppFixedCacheTileInfo
//    {
//    public:
//       ossimAppFixedCacheTileInfo(ossimAppFixedCacheId appId,
//                                  const ossimIpt& origin)
//          :theAppCacheId(appId),
//           theOrigin(origin)
//          {}
//       ossimAppFixedCacheId  theAppCacheId;
//       ossimIpt       theOrigin;
      
//       bool operator ==(const ossimAppFixedCacheTileInfo &rhs)const
//          {
//             return (theAppCacheId == rhs.theAppCacheId &&
//                     theOrigin     == rhs.theOrigin);
//          } 
//    };
   
   ossimAppFixedTileCache();
   
   ossimFixedTileCache* getCache(ossimAppFixedCacheId cacheId);

   void shrinkGlobalCacheSize(ossim_int32 byteCount);
   void shrinkCacheSize(ossimAppFixedCacheId id,
                        ossim_int32 byteCount);
   void shrinkCacheSize(ossimFixedTileCache* cache,
                        ossim_int32 byteCount);
   void deleteAll();
   
   static ossimAppFixedTileCache *theInstance;
   
   /*!
    * Will hold the current unique Application id.
    */
   static ossimAppFixedCacheId    theUniqueAppIdCounter;
   ossimIpt                       theTileSize;
   ossim_uint32                   theMaxCacheSize;
   ossim_uint32                   theMaxGlobalCacheSize;
   ossim_uint32                   theCurrentCacheSize;

   std::map<ossimAppFixedCacheId, ossimFixedTileCache*> theAppCacheMap;

   OpenThreads::Mutex theMutex;
};

#endif
