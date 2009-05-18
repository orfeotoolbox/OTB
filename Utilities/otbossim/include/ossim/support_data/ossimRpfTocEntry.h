//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Garrett Potts
// 
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfTocEntry.h 14241 2009-04-07 19:59:23Z dburken $
#ifndef ossimRpfTocEntry_HEADER
#define ossimRpfTocEntry_HEADER

#include <vector>
#include <iosfwd>

#include <ossim/support_data/ossimRpfBoundaryRectRecord.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/support_data/ossimRpfFrameEntry.h>

class ossimRpfTocEntry
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                                    const ossimRpfTocEntry& data);
   ossimRpfTocEntry();
   ossimErrorCode parseStream(std::istream &in, ossimByteOrder byteOrder);

   /**
    * @brief print method that outputs a key/value type format adding prefix
    * to keys.
    * @param out String to output to.
    * @param prefix This will be prepended to key.
    * e.g. Where prefix = "nitf." and key is "file_name" key becomes:
    * "nitf.file_name:"
    * @return output stream.
    */
   std::ostream& print(std::ostream& out,
                       const std::string& prefix=std::string()) const;
   
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
   ossim_uint32 getNumberOfFramesHorizontal()const;

   /*!
    * returns how many subimges or frames exist in the vertical
    * direction.
    */
   ossim_uint32 getNumberOfFramesVertical()const;

   
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
   
   ossimRpfBoundaryRectRecord theBoundaryInformation;
   std::vector< std::vector<ossimRpfFrameEntry> > theFrameEntryArray;
};

#endif
