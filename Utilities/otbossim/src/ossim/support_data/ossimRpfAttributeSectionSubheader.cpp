//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfAttributeSectionSubheader.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/support_data/ossimRpfAttributeSectionSubheader.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>

ostream& operator <<(ostream& out,
                     const ossimRpfAttributeSectionSubheader& data)
{
   data.print(out);
   
   return out;
}

ossimRpfAttributeSectionSubheader::ossimRpfAttributeSectionSubheader()
{
   clearFields();
}

ossimErrorCode ossimRpfAttributeSectionSubheader::parseStream(istream& in,
                                                              ossimByteOrder byteOrder)
{
   theAttributeSectionSubheaderStart = 0;
   theAttributeSectionSubheaderEnd = 0;
   if(in)
   {
      theAttributeSectionSubheaderStart = in.tellg();
      in.read((char*)&theNumberOfAttributeOffsetRecords, 2);
      in.read((char*)&theNumberOfExplicitArealCoverageRecords, 2);
      in.read((char*)&theAttributeOffsetTableOffset, 4);
      in.read((char*)&theAttribteOffsetRecordLength, 2);
      ossimEndian anEndian;

      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theNumberOfAttributeOffsetRecords);
         anEndian.swap(theNumberOfExplicitArealCoverageRecords);
         anEndian.swap(theAttributeOffsetTableOffset);
         anEndian.swap(theAttribteOffsetRecordLength);
      }
      theAttributeSectionSubheaderEnd = in.tellg();
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfAttributeSectionSubheader::print(ostream& out)const
{
   out << "theNumberOfAttributeOffsetRecords:         " << theNumberOfAttributeOffsetRecords << endl
       << "theNumberOfExplicitArealCoverageRecords:   " << theNumberOfExplicitArealCoverageRecords << endl
       << "theAttributeOffsetTableOffset:             " << theAttributeOffsetTableOffset << endl
       << "theAttribteOffsetRecordLength:             " << theAttribteOffsetRecordLength;
}

void ossimRpfAttributeSectionSubheader::clearFields()
{
   theNumberOfAttributeOffsetRecords       = 0;
   theNumberOfExplicitArealCoverageRecords = 0;
   theAttributeOffsetTableOffset           = 0;
   theAttribteOffsetRecordLength           = 0;
   
   theAttributeSectionSubheaderStart       = 0;
   theAttributeSectionSubheaderEnd         = 0;
}

ossim_uint64 ossimRpfAttributeSectionSubheader::getSubheaderStart()const
{
   return theAttributeSectionSubheaderStart;
}

ossim_uint64 ossimRpfAttributeSectionSubheader::getSubheaderEnd()const
{
   return theAttributeSectionSubheaderEnd;
}
