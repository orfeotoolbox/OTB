//*******************************************************************
// Copyright (C) 2007 Ball Aerspace & Technologies Corp 
//
// LICENSE: See top level LICENSE.txt file.
// 
// Author: Lucas Eckels
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: $

#include <ossim/support_data/ossimNitfImageDataMaskV2_1.h>
#include <ossim/base/ossimEndian.h>

ossimNitfImageDataMaskV2_1::ossimNitfImageDataMaskV2_1() : theBlockLength(0)
{
}

void ossimNitfImageDataMaskV2_1::setBlockCount(ossim_uint32 blockCount)
{
   theIncludedBlocks.resize(blockCount, true);
}

void ossimNitfImageDataMaskV2_1::setBlockLengthInBytes(ossim_uint32 blockLength)
{
   theBlockLength = blockLength;
}

void ossimNitfImageDataMaskV2_1::setIncludeBlock(ossim_uint32 blockNumber, bool included)
{
   if (blockNumber < theIncludedBlocks.size())
   {
      theIncludedBlocks[blockNumber] = included;
   }
}

void ossimNitfImageDataMaskV2_1::writeStream(std::ostream &out)
{
   ossimEndian endian;

   ossim_uint32 imdatoff = 4 + 2 + 2 + 2 + 4 * theIncludedBlocks.size();
   ossim_uint16 bmrlnth = 4; // only support band mask
   ossim_uint16 tmrlnth = 0;
   ossim_uint16 tpxcdlnth = 0;

   if(endian.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
   {
      endian.swap(imdatoff);
      endian.swap(bmrlnth);
      endian.swap(tmrlnth);
      endian.swap(tpxcdlnth);
   }

   out.write(reinterpret_cast<char*>(&imdatoff), sizeof(imdatoff));
   out.write(reinterpret_cast<char*>(&bmrlnth), sizeof(bmrlnth));
   out.write(reinterpret_cast<char*>(&tmrlnth), sizeof(tmrlnth));
   out.write(reinterpret_cast<char*>(&tpxcdlnth), sizeof(tpxcdlnth));

   ossim_uint32 position = 0;
   // Band Masks
   for (std::vector<bool>::const_iterator iter = theIncludedBlocks.begin();
      iter != theIncludedBlocks.end(); ++iter)
   {
      ossim_uint32 offset = 0xFFFFFFFF;
      if (*iter)
      {
         offset = position;
         position += theBlockLength;
      }
      if (endian.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
      {
         endian.swap(offset);
      }
      out.write(reinterpret_cast<char*>(&offset), sizeof(offset));
   }

   // Pad pixels not supported, always set to length 0, so don't need to write anything
}
