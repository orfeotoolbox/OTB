//*******************************************************************
// Copyright (C) 2004 Garrett Potts.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description:
//
// Contains class definition for ossimNitfTileSource.
//*******************************************************************
//  $Id: ossimNitfPackedBits.cpp,v 1.4 2004/11/11 20:17:25 gpotts Exp $
#include "ossimNitfPackedBits.h"
#include <iostream>

ossimNitfPackedBits::ossimNitfPackedBits(ossim_uint8* packedBits, ossim_uint16 bits)
      :thePackedBits(packedBits),
       theBits(bits)
{
   theBytes    = theBits>>3;
   theBitsLeft = theBits%8;
}

ossim_uint32 ossimNitfPackedBits::getValueAsUint32(ossim_uint32 idx)const
{
   ossim_uint32 value              = 0;
   ossim_uint32 tempBits     = (theBitsLeft*idx);
   ossim_uint32 tempBitShift = tempBits%8;
   ossim_uint32 byteOffset = ( (idx*theBytes) +
                               (tempBits>>3) );
   ossim_uint32 bitCount = 0;
   ossim_uint32 bitsSeen = tempBitShift;
   ossim_uint8 bitmask = 1 << (7-tempBitShift);
   while(bitCount < theBits)
   {
      value |= (ossim_uint8)((bool)(thePackedBits[byteOffset]&bitmask));
      ++bitCount;
      ++bitsSeen;
      if(bitsSeen >= 8)
      {
         bitsSeen = 0;
         bitmask = 0x80; // binary pattern 10000000
         ++byteOffset;
      }
      else
      {
         bitmask>>=1;
      }
      if(bitCount < theBits)
      {
         value <<=1;
      }
   }
   
   return value;
}
