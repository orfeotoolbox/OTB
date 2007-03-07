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
//  $Id: ossimTiledImageHash.h,v 1.3 2005/07/15 19:16:59 dburken Exp $
#ifndef ossimTiledImageHash_HEADER
#define ossimTiledImageHash_HEADER
#include "base/data_types/ossimDrect.h"
#include "ossimPointHash.h"

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
