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
// $Id: ossimAppTileCache.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimAppTileCache_HEADER
#define ossimAppTileCache_HEADER
#include <map>
#include <list>
using namespace std;

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimRefPtr.h>
// class ossimTile;
class ossimTileCache;
class ossimDataObject;

class ossimAppTileCache
{
public:
   static const ossim_uint32 DEFAULT_SIZE;

   static const ossim_uint32 DEFAULT_BUCKET_SIZE;

   typedef ossim_int32 ossimAppCacheId;
   typedef ossim_int32 ossimTileId;
   typedef map<ossimAppCacheId, ossimTileCache*>::iterator AppIdIterator;

   ~ossimAppTileCache();

   /*!
    *  We will force a singleton on this class.
    */
   static ossimAppTileCache *instance(ossim_uint32  maxSize   = 0);

   /*!
    * Will create a new Tile cache for this application.  Will
    * return 0 if not successful.
    */
   ossimAppCacheId newTileCache(ossim_uint32 bucketSize=DEFAULT_BUCKET_SIZE);

   /*!
    * Will retrieve a tile from the cache.  Will return NULL
    * if not found.
    */
   ossimDataObject *get(ossimAppCacheId id,
                        const ossimDpt3d &origin,
                        ossim_uint32 resLevel=0);

   /*!
    * Will remove a tile completly from the cache.  The
    * returned tile is no longer owned by the cache.
    */
   ossimRefPtr<ossimDataObject> removeTile(ossimAppCacheId id,
                                           const ossimDpt3d &origin,
                                           unsigned long resLevel);
   /*!
    * Will insert a tile into the cache.
    */
   ossimRefPtr<ossimDataObject> insert(ossimAppCacheId id,
                                       const ossimDpt3d &origin,
                                       const ossimDataObject* data,
                                       ossim_uint32 resLevel=0);
   
   /*!
    * This will delete the specified cache. The LRU queue will
    * be updated accordingly
    */
   void deleteCache(ossimAppCacheId appId);
protected:

   struct ossimAppCacheTileInfo
   {
   public:
      ossimAppCacheTileInfo(ossimAppCacheId appId,
                       const ossimDpt3d& origin,
                       ossim_uint32    resLevel)
         :theAppCacheId(appId),
          theOrigin(origin),
          theResLevel(resLevel)
         {}
      ossimAppCacheId  theAppCacheId;
      ossimDpt3d         theOrigin;
      ossim_uint32     theResLevel;

      bool operator ==(const ossimAppCacheTileInfo &rhs)
         {
            return (theAppCacheId == rhs.theAppCacheId &&
                    theOrigin     == rhs.theOrigin &&
                    theResLevel   == rhs.theResLevel);
         } 
  
   };

   ossimAppTileCache(ossim_uint32  maxSize   = DEFAULT_SIZE)
      :
         theMaxCacheSize(maxSize),
         theCurrentCacheSize(0)
      {}

   void deleteAll();
   /*!
    * Used to get access to the App cache.
    */
  ossimTileCache* get(ossimAppCacheId id);

   /*!
    * will remove all occurances of the appId from the
    * queue
    */
   void deleteAppCacheFromQueue(ossimAppCacheId appId);

   /*!
    * Will remove a single instance of a tile from the queue.
    */
   void removeTileFromQueue(ossimAppCacheId appId,
                            const ossimDpt3d &origin,
                            ossim_uint32 resLevel);

   void adjustQueue(ossimAppCacheId id,
                    const ossimDpt3d &origin,
                    ossim_uint32 resLevel);
   /*!
    * will pop the queue and remove that tile from its cache
    * and return it to the caller. This is used by the insert
    * when the max cache size is exceeded.
    */
   ossimRefPtr<ossimDataObject> removeTile();


   static ossimAppTileCache *theInstance;
   /*!
    * Will hold the current unique Application id.
    */
   static ossimAppCacheId                   theUniqueAppIdCounter;
   /*!
    * Will hold the list of application caches
    */
   map<ossimAppCacheId, ossimTileCache*>  theAppCache;

   /*!
    * Is the maximum size of the cache.
    */
   ossim_uint32                      theMaxGlobalCacheSize;
   ossim_uint32                      theMaxCacheSize;
   

   /*!
    * This holds the current cache size.
    */
   ossim_uint32                      theCurrentCacheSize;
   

   /*!
    * Is used in an Least recently used algorithm
    */
   list<ossimAppCacheTileInfo>      theUsedQueue;
   
};

#endif
