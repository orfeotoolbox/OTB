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

#ifndef ossimNitfImageDataMaskV2_1_HEADER
#define ossimNitfImageDataMaskV2_1_HEADER

#include <ossim/base/ossimConstants.h>
#include <vector>
#include <ostream>

class OSSIMDLLEXPORT ossimNitfImageDataMaskV2_1
{
public:
   ossimNitfImageDataMaskV2_1();

   void setBlockCount(ossim_uint32 blockCount);
   void setBlockLengthInBytes(ossim_uint32 blockLength);

   void setIncludeBlock(ossim_uint32 blockNumber, bool included);
   
   void writeStream(std::ostream &out);

private:
   std::vector<bool> theIncludedBlocks;
   ossim_uint32 theBlockLength;
};

#endif