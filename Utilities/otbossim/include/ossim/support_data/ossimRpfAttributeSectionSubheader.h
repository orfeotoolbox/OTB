//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfAttributeSectionSubheader.h 9967 2006-11-29 02:01:23Z gpotts $
#ifndef ossimRpfAttributeSectionSubheader_HEADER
#define ossimRpfAttributeSectionSubheader_HEADER
#include <iostream>
using namespace std;

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorContext.h>

class ossimRpfAttributeSectionSubheader
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfAttributeSectionSubheader& data);
   ossimRpfAttributeSectionSubheader();

   virtual ~ossimRpfAttributeSectionSubheader(){}
   ossimErrorCode parseStream(istream& in, ossimByteOrder byteOrder);
   virtual void print(ostream& out)const;
   ossim_uint16 getNumberOfAttributeOffsetRecords()const
      {
         return theNumberOfAttributeOffsetRecords;
      }
   ossim_uint16 getAttributeOffsetTableOffset()const
      {
         return theAttributeOffsetTableOffset;
      }

   ossim_uint64 getSubheaderStart()const;
   ossim_uint64 getSubheaderEnd()const;
   
private:
   void clearFields();
   
   ossim_uint16 theNumberOfAttributeOffsetRecords;
   ossim_uint16 theNumberOfExplicitArealCoverageRecords;
   ossim_uint32 theAttributeOffsetTableOffset;
   ossim_uint16 theAttribteOffsetRecordLength;


   mutable ossim_uint64 theAttributeSectionSubheaderStart;
   mutable ossim_uint64 theAttributeSectionSubheaderEnd;
};

#endif
