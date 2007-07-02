//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfTocEntry.h 9967 2006-11-29 02:01:23Z gpotts $
#ifndef ossimRpfTocEntry_HEADER
#define ossimRpfTocEntry_HEADER
#include <vector>
#include <iostream>
#include <iterator>
using namespace std;

#include <ossim/support_data/ossimRpfBoundaryRectRecord.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/support_data/ossimRpfFrameEntry.h>

class ossimRpfTocEntry
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfTocEntry& data);
   ossimRpfTocEntry();
   ossimErrorCode parseStream(istream &in, ossimByteOrder byteOrder);
   void print(ostream& out)const;
   void setEntry(const ossimRpfFrameEntry& entry,
                 long row,
                 long col);

   bool getEntry(long row,
                 long col,
                 ossimRpfFrameEntry& result)const;

   ossimString getProductType()const{return theBoundaryInformation.getProductType();}
   /*!
    * returns how many subimges or frames exist in the horizontal
    * direction.
    */
   unsigned long getNumberOfFramesHorizontal()const;

   /*!
    * returns how many subimges or frames exist in the vertical
    * direction.
    */
   unsigned long getNumberOfFramesVertical()const;

   
   const ossimRpfBoundaryRectRecord& getBoundaryInformation()const
      {
         return theBoundaryInformation;
      }
   /*!
    * If there is an entry and all the files don't exist we will return
    * true.
    */
   bool isEmpty()const;
   
private:
   void allocateFrameEntryArray();
   
   ossimRpfBoundaryRectRecord           theBoundaryInformation;
   vector< vector<ossimRpfFrameEntry> > theFrameEntryArray;
};

#endif
