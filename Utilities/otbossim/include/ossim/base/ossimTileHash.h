//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken Copied from TiledImageHash.
//
// Description: Hashing function for tiled rectangles.  Will hash a
//              dpt or fpt to a single index value.
//
// NOTE:  Works on rectangles that are positive in the line up (y)
//        direction.  Use TiledImageHash for rectangles that are positive
//        in the line down direction.
//              
//*******************************************************************
//  $Id: ossimTileHash.h 9094 2006-06-13 19:12:40Z dburken $

#ifndef TileHash_HEADER
#define TileHash_HEADER

#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimPointHash.h>

class OSSIMDLLEXPORT ossimTileHash : public ossimPointHash
{
public:
   ossimTileHash(const ossimDrect &imageRect,
                 double tileWidth,
                 double tileHeight);

   virtual ~ossimTileHash();

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
