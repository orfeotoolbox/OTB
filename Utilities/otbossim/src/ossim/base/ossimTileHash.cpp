//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken Copied from TiledImageHash.
//
// Description: Hashing function for tiled rectangles.  WIll hash a
//              dpt or a to a single index value.
//
// NOTE:  This works with rectangles that are positive in the line up
//        (y) direction.
//              
//*******************************************************************
//  $Id: ossimTileHash.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <cfloat> // for FLT_EPSILON

#include <ossim/base/ossimTileHash.h>

ossimTileHash::ossimTileHash(const ossimDrect& imageRect,
                             double tileWidth,
                             double tileHeight)
   :
      ossimPointHash(),
      theImageRect(imageRect)
{
   // make sure that the width of the tile is not larger than
   // the width of the image rectangle.
   if(theImageRect.width() <= tileWidth)
   {
      theTileWidth = theImageRect.width();
      theNumberOfHorizTiles = 1;
   }
   else
   {
      theTileWidth          = tileWidth;
      double tempDiv        = theImageRect.width()/theTileWidth;
      double overFlow       = tempDiv  - static_cast<long>(tempDiv);
      theNumberOfHorizTiles = static_cast<long>(tempDiv);
      
      if(fabs(overFlow) > FLT_EPSILON) // if the extent went beyond a tile
      {
         theNumberOfHorizTiles ++;      // we must say it has another tile
      }
   }

   // make sure the height of the tile is not larger than the
   // height of the image rectangle.
   if(theImageRect.height() <= tileHeight)
   {
      theTileHeight = theImageRect.height();
      theNumberOfVertTiles = 1;
   }
   else
   {
      theTileHeight         = tileHeight;
      double tempDiv        = theImageRect.height()/theTileHeight;
      double overFlow       = tempDiv  - static_cast<long>(tempDiv);
      theNumberOfVertTiles = static_cast<long>(tempDiv);

      if(fabs(overFlow) > FLT_EPSILON) // if the extent went beyond a tile
      {
         theNumberOfVertTiles ++;      // we must say it has another tile
      }
   }
}

ossimTileHash::~ossimTileHash()
{
}

long ossimTileHash::operator()(const ossimFpt& aPoint)
{
   if ( (aPoint.x >= theImageRect.ul().x) &&
        (aPoint.x <= theImageRect.lr().x) &&
        (aPoint.y <= theImageRect.ul().y) &&
        (aPoint.y >= theImageRect.lr().y) )
   {
      // how far is the point horizontally  from the upper left corner
      double deltaWidth  = aPoint.x - theImageRect.ul().x;

      // how far is the point vertically from the upper left point
      double deltaHeight = theImageRect.ul().y - aPoint.y;

      // solve the horizontal and vertical index numbers
      long indexWidth  = static_cast<long>(deltaWidth  / theTileWidth);
      long indexHeight = static_cast<long>(deltaHeight / theTileHeight);

      // Map to a linear array.
      // Just like you would index a 2-D array in memory
      return static_cast<long>(theNumberOfHorizTiles*indexHeight + indexWidth);
   }

   return -1;
}

long ossimTileHash::operator()(const ossimDpt& aPoint)
{
   if ( (aPoint.x >= theImageRect.ul().x) &&
        (aPoint.x <= theImageRect.lr().x) &&
        (aPoint.y <= theImageRect.ul().y) &&
        (aPoint.y >= theImageRect.lr().y) )
   {
      // how far is the point horizontally  from the upper left corner
      double deltaWidth  = aPoint.x - theImageRect.ul().x;

      // how far is the point vertically from the upper left point
      double deltaHeight = theImageRect.ul().y - aPoint.y;

      // solve the horizontal and vertical index numbers
      long indexWidth  = static_cast<long>(deltaWidth  / theTileWidth);
      long indexHeight = static_cast<long>(deltaHeight / theTileHeight);

      // Map to a linear array.
      // Just like you would index a 2-D array in memory
      return static_cast<long>(theNumberOfHorizTiles*indexHeight + indexWidth);
   }

   return -1;
}
