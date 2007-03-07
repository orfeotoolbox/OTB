//******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: This file contains the cache algorithm
//
//***********************************
// $Id: ossimTileCache.cpp,v 1.7 2003/06/15 19:00:41 dburken Exp $

#include "ossimTileCache.h"
#include "base/common/ossimDataObject.h"

void ossimTileCache::display()const
{
   ossimTileCache::Iterator tiles;

   for(long buckets = 0; buckets < theNumberOfBuckets; buckets++)
   {
      tiles = theCache[buckets].begin();
      while(tiles != theCache[buckets].end())
      {
         cout << ((*tiles).second) << endl;
         ++tiles;
      }
   }
}

ossimTileCache::ossimTileCache(long numberOfBuckets)
   : theCache(NULL),
     theNumberOfBuckets(numberOfBuckets>0?numberOfBuckets:255),
     theSizeInBytes(0)
{
   theCache = new multimap<ossim_uint32, ossimTileInformation*>[theNumberOfBuckets];
}

ossimTileCache::~ossimTileCache()
{
   deleteAll();
}

ossimDataObject* ossimTileCache::get(const ossimDpt3d &origin,
                                    unsigned long resLevel)
{
   ossimDataObject* result = NULL;
   Iterator anIterator;

   ossim_uint32 bucket = bucketHash(origin);
   
   anIterator = theCache[bucket].find(tileId(origin));
   while(anIterator != theCache[bucket].end())
   {
      CacheDataPtr info = (*anIterator).second;
      if(info)
      {
         if(info->theOrigin   == origin &&
            info->theResLevel == resLevel)
         {
            return info->theCachedTile;
         }
      }

      ++anIterator;
   }
   return result;
}

ossimDataObject* ossimTileCache::remove(const ossimDpt3d &origin,
                                  unsigned long resLevel)
{
   Iterator anIterator;
   ossimDataObject *result;
   ossim_uint32 bucket = bucketHash(origin);
   
   anIterator = theCache[bucket].find(tileId(origin));
   while(anIterator != theCache[bucket].end())
   {
      CacheDataPtr info = (*anIterator).second;
      if(info)
      {
         if(info->theOrigin   == origin &&
            info->theResLevel == resLevel)
         {
            theCache[bucket].erase(anIterator);
            theSizeInBytes -= info->theCachedTile->getDataSizeInBytes();

            result = info->theCachedTile;

            delete info;
            return result;
         }
      }

      ++anIterator;
   }
   
   return NULL;   
}

ossimDataObject* ossimTileCache::insert(const ossimDpt3d &origin,
                                       ossimDataObject* data,
                                       unsigned long resLevel)
{   
   ossim_uint32 bucket = bucketHash(origin);

   // make sure we keep up with the current size of the
   // cache in bytes.  This is only the count of the data
   // and not any overhead required by the cache.
   theSizeInBytes += data->getDataSizeInBytes();
   theCache[bucket].insert(make_pair(tileId(origin),
                                     new CacheData(data,
                                                   origin,
                                                   resLevel)));
   
   return data;
}

void ossimTileCache::invalidate()
{
   
}

void ossimTileCache::invalidate(const ossimDpt3d &origin,
                                ossim_uint32 resLevel)
{
}



ossim_uint32 ossimTileCache::tileId(const ossimDpt3d &aPt)
{
   const unsigned char *bufx = (unsigned char*)(&aPt.x);
   const unsigned char *bufy = (unsigned char*)(&aPt.y);
   const unsigned char *bufz = (unsigned char*)(&aPt.z);

   // this just multiplies each byte by some prime number
   // and add together.
   return (ossim_uint32)(bufx[0]*101 + bufx[1]*103 +
                    bufx[2]*107 + bufx[3]*109 +
                    bufx[4]*113 + bufx[5]*127 +
                    bufx[6]*131 + bufx[7]*137 +
                    bufy[0]*139 + bufy[1]*149 +
                    bufy[2]*151 + bufy[3]*157 +
                    bufy[4]*163 + bufy[5]*167 +
                    bufy[6]*173 + bufy[7]*179 +
                    bufz[0]*181 + bufz[1]*191 +
                    bufz[2]*193 + bufz[3]*197 +
                    bufz[4]*199 + bufz[5]*211 +
                    bufz[6]*223 + bufz[6]*227);       
}

ossim_uint32 ossimTileCache::bucketHash(const ossimDpt3d &aPt)
{   
   return tileId(aPt)%theNumberOfBuckets;
}

void ossimTileCache::deleteAll()
{
   Iterator anIterator;

   for(long bucket = 0; bucket < theNumberOfBuckets; bucket++)
   {
      anIterator = theCache[bucket].begin();
      while(anIterator != theCache[bucket].end())
      {
         CacheDataPtr info = (*anIterator).second;
         delete info->theCachedTile;
         delete info;
         
         ++anIterator;
      }
      theCache[bucket].clear();
   }
   delete [] theCache;
}
