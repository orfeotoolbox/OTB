//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//********************************************************************
// $Id: ossimRpfColorConverterSubsection.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <ossim/support_data/ossimRpfColorConverterSubsection.h>
#include <ossim/support_data/ossimRpfColorConverterOffsetRecord.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>

ostream& operator <<(ostream& out,
                     const ossimRpfColorConverterSubsection& data)
{
   data.print(out);
   
   return out;
}

ossimRpfColorConverterSubsection::ossimRpfColorConverterSubsection()
{
   theNumberOfColorConverterOffsetRecords = 0;
   clearFields();
}

void ossimRpfColorConverterSubsection::clearFields()
{
   theColorConverterOffsetTableOffset     = 0;
   theColorConverterOffsetRecordLength    = 0;
   theConverterRecordLength               = 0;

   theTableList.clear();
}

const ossimRpfColorConverterTable* ossimRpfColorConverterSubsection::getColorConversionTable(ossim_uint32 givenThisNumberOfEntires)const
{
   vector<ossimRpfColorConverterTable>::const_iterator listElement = theTableList.begin();

   while(listElement != theTableList.end())
   {
      if((*listElement).getNumberOfEntries() == givenThisNumberOfEntires)
      {
         return &(*listElement);
      }
   }

   return NULL;
}

ossimErrorCode ossimRpfColorConverterSubsection::parseStream(istream& in,
                                                             ossimByteOrder byteOrder)
{
   if(in)
   {
      clearFields();

      // this is the start of the subsection
      theStartOffset = in.tellg();
      in.read((char*)&theColorConverterOffsetTableOffset, 4);
      in.read((char*)&theColorConverterOffsetRecordLength, 2);
      in.read((char*)&theConverterRecordLength, 2);

      // this grabs the end of the subsection
      theEndOffset   = in.tellg();

      ossimEndian anEndian;

      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theColorConverterOffsetTableOffset);
         anEndian.swap(theColorConverterOffsetRecordLength);
         anEndian.swap(theConverterRecordLength);

      }
      theTableList.resize(theNumberOfColorConverterOffsetRecords);
      for(unsigned long index = 0;
          index < theNumberOfColorConverterOffsetRecords;
          ++index)
      {
         ossimRpfColorConverterOffsetRecord recordInfo;
         
         if(recordInfo.parseStream(in, byteOrder) ==
            ossimErrorCodes::OSSIM_OK)
         {
            unsigned long rememberGet = in.tellg();

            theTableList[index].setNumberOfEntries(recordInfo.theNumberOfColorConverterRecords);
            theTableList[index].setTableId(recordInfo.theColorConverterTableId);
            in.seekg(theStartOffset + recordInfo.theColorConverterTableOffset, ios::beg);
            theTableList[index].parseStream(in, byteOrder);
            
            in.seekg(rememberGet, ios::beg);
         }
         else
         {
            return ossimErrorCodes::OSSIM_ERROR;
         }
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }

   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfColorConverterSubsection::setNumberOfColorConverterOffsetRecords(ossim_uint16 numberOfRecords)
{
   theNumberOfColorConverterOffsetRecords = numberOfRecords;
}

void ossimRpfColorConverterSubsection::print(ostream& out)const
{
   out << "theColorConverterOffsetTableOffset:      "
       << theColorConverterOffsetTableOffset << endl
       << "theColorConverterOffsetRecordLength:     "
       << theColorConverterOffsetRecordLength << endl
       << "theConverterRecordLength:                "
       << theConverterRecordLength << endl;

   copy(theTableList.begin(),
        theTableList.end(),
        ostream_iterator<ossimRpfColorConverterTable>(out, "\n"));
}
