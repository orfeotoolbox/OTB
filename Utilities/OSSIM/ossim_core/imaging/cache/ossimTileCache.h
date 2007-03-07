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
// $Id: ossimTileCache.h,v 1.5 2003/05/15 14:34:16 dburken Exp $

#ifndef DataCache_HEADER
#define DataCache_HEADER
#include <map>
#include <stack>
using namespace std;

#include "base/common/ossimConstants.h"
#include "base/data_types/ossimDpt3d.h" 
class ossimDataObject;
class ossimTileCache
{
public:
   
   ossimTileCache(long numberOfBuckets  = 10);
   virtual ~ossimTileCache();

   /*!
    * This will return a tile if found.  Note this tile is still
    * owned by the cache.
    */
   virtual ossimDataObject* get(const ossimDpt3d &origin,
                               unsigned long resLevel=0);

   /*!
    * Will remove the tile from the cache and will not delete.  Note:
    * the cache no longer owns the tile.
    */
   virtual ossimDataObject* remove(const ossimDpt3d &origin,
                                  unsigned long resLevel=0);
   /*!
    * This will not duplicate the tile.  Note the tile is owned
    * by the cache.
    */
   virtual ossimDataObject* insert(const ossimDpt3d &origin,
                                  ossimDataObject* data,
                                  unsigned long resLevel=0);
   /*!
    * Not implemented
    */
   void invalidate();
   /*!
    * Not implemented
    */
   virtual void invalidate(const ossimDpt3d &origin,
                           ossim_uint32 resLevel);


   
   virtual long numberOfItems()const{return theCache?theCache->size():0;}
   virtual void display()const;

   virtual ossim_uint32 sizeInBytes(){return theSizeInBytes;}
   
protected:

   struct ossimTileInformation
   {
   public:
      friend ostream& operator <<(ostream &out,
                                  const ossimTileInformation &info)
         {

            return out;
         }
      ossimTileInformation(ossimDataObject* aTile,
                      const ossimDpt3d &origin = ossimDpt(0,0),
                      unsigned long resLevel = 0)
         : theCachedTile(aTile),
           theOrigin(origin),
           theResLevel(resLevel)
         {}
      ossimDataObject*    theCachedTile;
      ossimDpt3d         theOrigin;
      unsigned long      theResLevel;
   };

   typedef multimap<ossim_uint32, ossimTileInformation*>::iterator Iterator;
   typedef ossimTileInformation* CacheDataPtr;
   typedef ossimTileInformation  CacheData;
   
   void deleteAll();
   virtual ossim_uint32 bucketHash(const ossimDpt3d &aPt);
   virtual ossim_uint32 tileId(const ossimDpt3d &aPt);
   
   multimap<ossim_uint32, ossimTileInformation*>  *theCache;
   long                                            theNumberOfBuckets;
   ossim_uint32                                    theSizeInBytes;
};

#endif
