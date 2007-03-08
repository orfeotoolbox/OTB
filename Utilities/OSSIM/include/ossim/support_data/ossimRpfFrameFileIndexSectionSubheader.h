//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfFrameFileIndexSectionSubheader.h 9967 2006-11-29 02:01:23Z gpotts $
#ifndef ossimRpfFrameFileIndexSectionSubheader_HEADER
#define ossimRpfFrameFileIndexSectionSubheader_HEADER
#include <iostream>
using namespace std;

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorContext.h>

class ossimRpfFrameFileIndexSectionSubheader
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfFrameFileIndexSectionSubheader& data);
   ossimRpfFrameFileIndexSectionSubheader();
   virtual ~ossimRpfFrameFileIndexSectionSubheader(){}

   ossimErrorCode parseStream(istream& in,
                              ossimByteOrder byteOrder);
   virtual void print(ostream& out)const;
   ossim_uint32 getOffset()const{return theIndexTableOffset;}
   ossim_uint32 getNumberOfIndexRecords()const{return theNumberOfIndexRecords;}
   ossim_uint16 getNumberOfPathnameRecords()const{return theNumberOfPathnameRecords;}
   ossim_uint16 getIndexRecordLength()const{return theIndexRecordLength;}

   void clearFields();
private:
   char          theHighestSecurityClassification;
   ossim_uint32  theIndexTableOffset;
   ossim_uint32  theNumberOfIndexRecords;
   ossim_uint16  theNumberOfPathnameRecords;
   ossim_uint16  theIndexRecordLength;
};

#endif
