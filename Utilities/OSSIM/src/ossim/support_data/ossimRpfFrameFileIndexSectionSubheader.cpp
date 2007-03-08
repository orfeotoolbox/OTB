//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfFrameFileIndexSectionSubheader.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/support_data/ossimRpfFrameFileIndexSectionSubheader.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>

ostream& operator <<(ostream& out,
                     const ossimRpfFrameFileIndexSectionSubheader& data)
{
   data.print(out);
   
   return out;
}

ossimRpfFrameFileIndexSectionSubheader::ossimRpfFrameFileIndexSectionSubheader()
{
   clearFields();
}

ossimErrorCode ossimRpfFrameFileIndexSectionSubheader::parseStream(istream& in, ossimByteOrder byteOrder)
{
   if(in)
   {
      ossimEndian anEndian;

      in.read((char*)&theHighestSecurityClassification, 1);
      in.read((char*)&theIndexTableOffset, 4);
      in.read((char*)&theNumberOfIndexRecords, 4);
      in.read((char*)&theNumberOfPathnameRecords, 2);
      in.read((char*)&theIndexRecordLength, 2);

      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theIndexTableOffset);
         anEndian.swap(theNumberOfIndexRecords);
         anEndian.swap(theNumberOfPathnameRecords);
         anEndian.swap(theIndexRecordLength);
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfFrameFileIndexSectionSubheader::print(ostream& out)const
{
   out << "theHighestSecurityClassification:  " << theHighestSecurityClassification << endl
       << "theIndexTableOffset:               " << theIndexTableOffset << endl
       << "theNumberOfIndexRecords:           " << theNumberOfIndexRecords << endl
       << "theNumberOfPathnameRecords:        " << theNumberOfPathnameRecords << endl
       << "theIndexRecordLength:              " << theIndexRecordLength;
}

void ossimRpfFrameFileIndexSectionSubheader::clearFields()
{
   theHighestSecurityClassification = ' ';
   theIndexTableOffset              = 0;
   theNumberOfIndexRecords          = 0;
   theNumberOfPathnameRecords       = 0;
   theIndexRecordLength             = 0;
}
