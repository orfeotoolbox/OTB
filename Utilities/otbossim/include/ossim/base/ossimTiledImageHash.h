//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description: Hashing function for tiled rectangles.  Will hash a
//              dpt to a single index value.
//              
//*******************************************************************
//  $Id: ossimTiledImageHash.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimTiledImageHash_HEADER
#define ossimTiledImageHash_HEADER
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimPointHash.h>

class OSSIMDLLEXPORT ossimTiledImageHash : public ossimPointHash
{
public:
   ossimTiledImageHash(const ossimDrect &imageRect,
                       double tileWidth,
                       double tileHeight);

   virtual ~ossimTiledImageHash();

   virtual long operator()(const ossimDpt &aPoint);
   virtual long operator()(const ossimFpt &aPoint);
   
private:
   ossimDrect   theImageRect;
   double  theTileWidth;
   double  theTileHeight;
   long    theNumberOfHorizTiles;
   long    theNumberOfVertTiles;
};

#endif
