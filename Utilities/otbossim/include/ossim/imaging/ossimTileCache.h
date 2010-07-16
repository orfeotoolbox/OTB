//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// Description: This file contains the cache algorithm
//
//***********************************
// $Id: ossimTileCache.h 17207 2010-04-25 23:21:14Z dburken $

#ifndef DataCache_HEADER
#define DataCache_HEADER
#include <map>
#include <stack>
using namespace std;

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimDataObject.h>
class OSSIM_DLL ossimTileCache
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


   
   virtual long numberOfItems()const{return theCache?(long)theCache->size():(long)0;}
   virtual void display()const;

   virtual ossim_uint32 sizeInBytes(){return theSizeInBytes;}
   
protected:

   struct ossimTileInformation
   {
   public:
      friend ostream& operator <<(ostream &out,
                                  const ossimTileInformation& /* info */)
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
      ossimRefPtr<ossimDataObject>    theCachedTile;
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
