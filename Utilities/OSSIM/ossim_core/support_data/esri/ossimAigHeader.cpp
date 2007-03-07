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
// $Id: ossimAigHeader.cpp,v 1.3 2003/05/13 11:31:10 dburken Exp $
#include "support_data/esri/ossimAigHeader.h"
#include "base/misc/ossimEndian.h"

bool ossimAigHeader::writeStream(std::ostream& out)
{
   ossimEndian endian;
   double          tempDouble;
   ossim_uint32    tempInt;
   
   if(endian.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
   {
      out.write((char*)theMagic, 8);
      out.write((char*)theUnknownData, 8);
      tempInt = theCellType;
      endian.swap(tempInt);
      out.write((char*)&tempInt, 4);
      
      out.write((char*)theUnknownAssortedData, 236);
      tempDouble = thePixelSizeX;
      endian.swap(tempDouble);
      out.write((char*)(&tempDouble), 8);
      tempDouble = thePixelSizeY;
      endian.swap(tempDouble);
      out.write((char*)(&tempDouble), 8);
      tempDouble = 0.0;
      endian.swap(tempDouble);
      out.write((char*)(&tempDouble), 8);
      out.write((char*)(&tempDouble), 8);

      tempInt = theNumberOfTilesPerRow;
      endian.swap(tempInt);
      out.write((char*)&tempInt, 4);
      
      tempInt = theNumberOfTilesPerColumn;
      endian.swap(tempInt);
      out.write((char*)&tempInt, 4);

      tempInt = theWidthOfTileInPixels;
      endian.swap(tempInt);
      out.write((char*)&tempInt, 4);

      tempInt = theUnknownValue;
      endian.swap(tempInt);
      out.write((char*)&tempInt, 4);

      tempInt = theHeightOfTileInPixels;
      endian.swap(tempInt);
      out.write((char*)&tempInt, 4);
   }
   else
   {
      out.write((char*)theMagic, 8);
      out.write((char*)theUnknownData, 8);
      out.write((char*)&theCellType, 4);
      out.write((char*)theUnknownAssortedData, 236);
      out.write((char*)(&thePixelSizeX), 8);
      out.write((char*)(&thePixelSizeY), 8);
      tempDouble = 0.0;
      out.write((char*)(&tempDouble), 8);
      out.write((char*)(&tempDouble), 8);
      out.write((char*)&theNumberOfTilesPerRow, 4);
      out.write((char*)&theNumberOfTilesPerColumn, 4);
      out.write((char*)&theWidthOfTileInPixels, 4);
      out.write((char*)&theUnknownValue, 4);
      out.write((char*)&theHeightOfTileInPixels, 4);
   }

   return true;
}
