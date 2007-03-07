#include "ossimRpfTocEntry.h"
#include "base/common/ossimCommon.h"
#include "base/common/ossimErrorCodes.h"
#ifndef NULL
#include <stddef.h>
#endif

ostream& operator <<(ostream& out,
                     const ossimRpfTocEntry& data)
{
   data.print(out);

   return out;
}


ossimRpfTocEntry::ossimRpfTocEntry()
{
   
}

ossimErrorCode ossimRpfTocEntry::parseStream(istream &in, ossimByteOrder byteOrder)
{
   ossimErrorCode result = ossimErrorCodes::OSSIM_OK;
   
   result = theBoundaryInformation.parseStream(in, byteOrder);
   if(result == ossimErrorCodes::OSSIM_OK)
   {
      allocateFrameEntryArray();
   }

   return result;
}

void ossimRpfTocEntry::print(ostream& out)const
{
   out << theBoundaryInformation << endl;

   vector< vector<ossimRpfFrameEntry> >::const_iterator frameEntry =
                                          theFrameEntryArray.begin();

   while(frameEntry != theFrameEntryArray.end())
   {
      copy((*frameEntry).begin(),
           (*frameEntry).end(),
           ostream_iterator<ossimRpfFrameEntry>(out, "\n"));
      
      ++frameEntry;
   }
}

unsigned long ossimRpfTocEntry::getNumberOfFramesHorizontal()const
{
   return theBoundaryInformation.getNumberOfFramesHorizontal();
}

unsigned long ossimRpfTocEntry::getNumberOfFramesVertical()const
{
   return theBoundaryInformation.getNumberOfFramesVertical();
}

void ossimRpfTocEntry::setEntry(const ossimRpfFrameEntry& entry,
                                long row,
                                long col)
{
   if(row < (long)theFrameEntryArray.size())
   {
      if(col < (long)theFrameEntryArray[row].size())
      {
         theFrameEntryArray[row][col] = entry;
      }
   }
}

bool ossimRpfTocEntry::getEntry(long row,
                                long col,
                                ossimRpfFrameEntry& result)const
{
   if(row < (long)theFrameEntryArray.size())
   {
      if(col < (long)theFrameEntryArray[row].size())
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
   long rows = theFrameEntryArray.size();
   long cols = 0;
   if(rows > 0)
   {
      cols = theFrameEntryArray[0].size();
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
   unsigned long horizontalSize = theBoundaryInformation.getNumberOfFramesHorizontal();

   for(unsigned long index = 0; index < theFrameEntryArray.size(); index++)
   {
      theFrameEntryArray[index].resize(horizontalSize);
   }
}
