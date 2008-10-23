//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts  (gpotts@imagelinks.com)
//
//*******************************************************************
// $Id: ossimAigHeader.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/support_data/ossimAigHeader.h>
#include <ossim/base/ossimEndian.h>

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
