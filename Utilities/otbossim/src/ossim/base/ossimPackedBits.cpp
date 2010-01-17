//*******************************************************************
// Copyright (C) 2004 Garrett Potts.
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Contains class definition for ossimPackBits.
//*******************************************************************
//  $Id: ossimPackedBits.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <ossim/base/ossimPackedBits.h>
#include <iostream>

ossimPackedBits::ossimPackedBits(const ossim_uint8* packedBits,
                                 ossim_uint16 bits)
      :thePackedBits(packedBits),
       theBits(bits)
{
   theBytes    = theBits>>3;
   theBitsLeft = theBits%8;
}

ossim_uint32 ossimPackedBits::getValueAsUint32(ossim_uint32 idx)const
{
   ossim_uint32 value        = 0;
   ossim_uint32 tempBits     = (theBitsLeft*idx);
   ossim_uint32 tempBitShift = tempBits%8;
   ossim_uint32 byteOffset   = ( (idx*theBytes) + (tempBits>>3) );
   ossim_uint32 bitCount     = 0;
   ossim_uint32 bitsSeen     = tempBitShift;
   ossim_uint8 bitmask       = 1 << (7-tempBitShift);
   
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
