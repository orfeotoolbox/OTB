//*******************************************************************
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
// Author:  Garrett Potts  (gpotts@imagelinks.com)
//
//*******************************************************************
// $Id: ossimAigHeader.h,v 1.5 2003/05/13 11:31:10 dburken Exp $

#ifndef ossimAigHeader_HEADER
#define ossimAigHeader_HEADER
#include <iostream>
#include "base/common/ossimConstants.h"
#include "string.h"

class ossimAigHeader
{
public:
   friend std::ostream& operator <<(std::ostream& out, const ossimAigHeader& header)
      {
         out << "Magic number:  " << header.theMagic << std::endl
             << "Coverage:      " << (header.theCellType==1?"Integer":"float") << std::endl
             << "Pixel size x:  " << header.thePixelSizeX << std::endl
             << "Pixel size y:  " << header.thePixelSizeY << std::endl
             << "Tiles per row: " << header.theNumberOfTilesPerRow << std::endl
             << "Tiles per col: " << header.theNumberOfTilesPerColumn << std::endl
             << "Tile width:    " << header.theWidthOfTileInPixels << std::endl
             << "Tile height:   " << header.theHeightOfTileInPixels;
         
         return out;
      }
   ossimAigHeader()
      {
         reset();
      }

   void reset()
      {
         memset(theMagic, '\0', 8);
         strcpy(theMagic, "GRID1.2");
         memset(theUnknownData, '\0', 8);
         setIntCoverage();
         memset(theUnknownAssortedData, '\0', 236);
         thePixelSizeX = 1.0;
         thePixelSizeY = 1.0;
         theUnknownGeoReferencingValues[0] = 0.0;
         theUnknownGeoReferencingValues[1] = 0.0;
         theNumberOfTilesPerRow = 0;
         theNumberOfTilesPerColumn = 0;
         theWidthOfTileInPixels = 0;
         theUnknownValue = 1;
         theHeightOfTileInPixels = 0;
      }

   bool writeStream(std::ostream& out);

   void setFloatCoverage()
      {
         theCellType = 2;
      }
   void setIntCoverage()
      {
         theCellType = 1;
      }
   char theMagic[8];
   char theUnknownData[8];
   
   /*!
    * 1 means int cover and 2 means float cover
    */
   ossim_int32   theCellType;
   
   char    theUnknownAssortedData[236];

   /*!
    * Holds the width of the pixel in geo referenced
    * coordinate.  It's 1 otherwise
    */
   double  thePixelSizeX;

   /*!
    * Holds the height of the pixel in geo referenced
    * coordinate.  It's 1 otherwise
    */
   double  thePixelSizeY;

   double  theUnknownGeoReferencingValues[2];

   /*!
    * This is the width of the file in tiles..
    */
   int     theNumberOfTilesPerRow;

   /*!
    * This is the height of the file in tiles..
    */
   int     theNumberOfTilesPerColumn;

   /*!
    * Numer of pixels wide the tile is.  Usually the value is
    * 256
    */
   int     theWidthOfTileInPixels;
   
   int     theUnknownValue;

   /*!
    * Numer of pixels high the tile is.  Usually the value is
    * 4
    */
   int     theHeightOfTileInPixels;
};

#endif
