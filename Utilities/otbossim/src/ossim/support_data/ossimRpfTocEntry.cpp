//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimRpfTocEntry.cpp 15833 2009-10-29 01:41:53Z eshirschorn $

#include <istream>
#include <ostream>
#include <iterator>
#include <ossim/support_data/ossimRpfTocEntry.h>
#include <ossim/support_data/ossimRpfFrameEntry.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimErrorCodes.h>

std::ostream& operator <<(std::ostream& out,
                          const ossimRpfTocEntry& data)
{
   data.print(out);
   return out;
}

ossimRpfTocEntry::ossimRpfTocEntry()
{
   
}

ossimErrorCode ossimRpfTocEntry::parseStream(
   std::istream &in, ossimByteOrder byteOrder)
{
   ossimErrorCode result = ossimErrorCodes::OSSIM_OK;
   
   result = theBoundaryInformation.parseStream(in, byteOrder);
   if(result == ossimErrorCodes::OSSIM_OK)
   {
      allocateFrameEntryArray();
   }

   return result;
}

std::ostream& ossimRpfTocEntry::print(std::ostream& out,
                                      const std::string& prefix) const
{
   theBoundaryInformation.print(out, prefix);

   vector< vector<ossimRpfFrameEntry> >::const_iterator frameEntry =
      theFrameEntryArray.begin();

   while( frameEntry != theFrameEntryArray.end() )
   {
      std::vector<ossimRpfFrameEntry>::const_iterator i =
         (*frameEntry).begin();
      while ( i != (*frameEntry).end() ) 
      {
         (*i).print(out, prefix);
         ++i;
      }
      ++frameEntry;
   }
   return out;
}

ossim_uint32 ossimRpfTocEntry::getNumberOfFramesHorizontal()const
{
   return theBoundaryInformation.getNumberOfFramesHorizontal();
}

ossim_uint32 ossimRpfTocEntry::getNumberOfFramesVertical()const
{
   return theBoundaryInformation.getNumberOfFramesVertical();
}

void ossimRpfTocEntry::setEntry(const ossimRpfFrameEntry& entry,
                                long row,
                                long col)
{
   if(row < (long)theFrameEntryArray.size() && row >= 0)
   {
      if(col < (long)theFrameEntryArray[row].size() && col >= 0)
      {
         theFrameEntryArray[row][col] = entry;
      }
   }
}

bool ossimRpfTocEntry::getEntry(long row,
                                long col,
                                ossimRpfFrameEntry& result)const
{
   if(row < (long)theFrameEntryArray.size() && row >= 0)
   {
      if(col < (long)theFrameEntryArray[row].size() && col >= 0)
      {
         result = theFrameEntryArray[row][col];
      }
      else
      {
         return false;
      }
   }
   else
   {
      return false;
   }
   
   return true;
}

/*!
 * If there is an entry and all the files don't exist we will return
 * true.
 */
bool ossimRpfTocEntry::isEmpty()const
{
   long rows = (long)theFrameEntryArray.size();
   long cols = 0;
   if(rows > 0)
   {
      cols = (long)theFrameEntryArray[0].size();
      for(long rowIndex = 0; rowIndex < rows; ++ rowIndex)
      {
         for(long colIndex = 0; colIndex < cols; ++colIndex)
         {
            if(theFrameEntryArray[rowIndex][colIndex].exists())
            {
               return false;
            }
         }
      }
   }

   return true;
}

void ossimRpfTocEntry::allocateFrameEntryArray()
{
   theFrameEntryArray.resize(theBoundaryInformation.getNumberOfFramesVertical());
   ossim_uint32 horizontalSize = theBoundaryInformation.getNumberOfFramesHorizontal();

   for(ossim_uint32 index = 0; index < theFrameEntryArray.size(); index++)
   {
      theFrameEntryArray[index].resize(horizontalSize);
   }
}
