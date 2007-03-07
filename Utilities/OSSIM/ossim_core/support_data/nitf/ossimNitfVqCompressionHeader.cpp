//*******************************************************************
// Copyright (C) 2004 Garrett Potts
//
// LICENSE: LGPL see top level LICENSE.txt for more details
// 
// Author: Garrett Potts
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfVqCompressionHeader.cpp,v 1.7 2005/10/05 12:50:48 gpotts Exp $
#include "ossimNitfVqCompressionHeader.h"
#include <base/misc/ossimEndian.h>
RTTI_DEF1(ossimNitfVqCompressionHeader, "ossimNitfVqCompressionHeader", ossimNitfCompressionHeader);

ostream& operator<<(ostream& out,
                    const ossimNitfVqCompressionOffsetTableData& data)
{
   out << "theTableId:                              " << data.theTableId << std::endl
       << "theNumberOfCompressionLookupRecords:     " << data.theNumberOfCompressionLookupRecords << std::endl
       << "theNumberOfValuesPerCompressionLookup:   " << data.theNumberOfValuesPerCompressionLookup << std::endl
       << "theCompressionLookupValueBitLength:      " << data.theCompressionLookupValueBitLength<< std::endl
       << "theCompressionLookupTableOffset:         " << data.theCompressionLookupTableOffset;

   return out;
}


ossimNitfVqCompressionOffsetTableData::ossimNitfVqCompressionOffsetTableData()
      :theData(NULL)
{
   clearFields();
}

ossimNitfVqCompressionOffsetTableData::ossimNitfVqCompressionOffsetTableData(const ossimNitfVqCompressionOffsetTableData& rhs)
      :theTableId(rhs.theTableId),
       theNumberOfCompressionLookupRecords(rhs.theNumberOfCompressionLookupRecords),
       theNumberOfValuesPerCompressionLookup(rhs.theNumberOfValuesPerCompressionLookup),
       theCompressionLookupValueBitLength(rhs.theCompressionLookupValueBitLength),
       theCompressionLookupTableOffset(rhs.theCompressionLookupTableOffset),
       theData(NULL)

{
   ossim_uint32 size = (theNumberOfValuesPerCompressionLookup*
                        theNumberOfCompressionLookupRecords*
                        theCompressionLookupValueBitLength)/8;
   if(size > 0)
   {
      theData = new unsigned char[size];
      if(rhs.theData)
      {
         memcpy(theData, rhs.theData, size);
      }
   }
}

ossimNitfVqCompressionOffsetTableData::~ossimNitfVqCompressionOffsetTableData()
{
   if(theData)
   {
      delete [] theData;
      theData = NULL;
   }
}

const ossimNitfVqCompressionOffsetTableData& ossimNitfVqCompressionOffsetTableData::operator =(const ossimNitfVqCompressionOffsetTableData& rhs)
{
   if(this != &rhs)
   {
      if(theData)
      {
         delete [] theData;
         theData = NULL;
      }

      theTableId                            = rhs.theTableId;
      theNumberOfCompressionLookupRecords   = rhs.theNumberOfCompressionLookupRecords;
      theCompressionLookupValueBitLength    = rhs.theCompressionLookupValueBitLength;
      theNumberOfValuesPerCompressionLookup = rhs.theNumberOfValuesPerCompressionLookup;
      theCompressionLookupTableOffset       = rhs.theCompressionLookupTableOffset;
      
      if(rhs.theData)
      {
         ossim_uint32 size = (theNumberOfValuesPerCompressionLookup*
                              theNumberOfCompressionLookupRecords*
                              theCompressionLookupValueBitLength)/8;
         if(size > 0)
         {
            theData = new unsigned char[size];
            memcpy(theData, rhs.theData, size);
         }
      }
   }
   
   return *this;
   
}
ossim_uint32 ossimNitfVqCompressionOffsetTableData::getDataLengthInBytes()const
{
   return (theNumberOfValuesPerCompressionLookup*
           theNumberOfCompressionLookupRecords*
           theCompressionLookupValueBitLength)/8;
}

void ossimNitfVqCompressionOffsetTableData::parseStream(std::istream& in)
{
   ossimEndian endian;
   in.read((char*)(&theTableId), 2);
   in.read((char*)(&theNumberOfCompressionLookupRecords),4);
   in.read((char*)(&theNumberOfValuesPerCompressionLookup), 2);
   in.read((char*)(&theCompressionLookupValueBitLength),2);
   in.read((char*)(&theCompressionLookupTableOffset), 4);

   if(endian.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
   {
      endian.swap(theTableId);
      endian.swap(theNumberOfCompressionLookupRecords);
      endian.swap(theNumberOfValuesPerCompressionLookup);
      endian.swap(theCompressionLookupValueBitLength);
      endian.swap(theCompressionLookupTableOffset);
   }
}

void ossimNitfVqCompressionOffsetTableData::clearFields()
{
   theTableId = 0;
   theNumberOfCompressionLookupRecords = 0;
   theNumberOfValuesPerCompressionLookup = 0;
   theCompressionLookupValueBitLength = 0;
   theCompressionLookupTableOffset = 0;

   if(theData)
   {
      delete [] theData;
      theData = NULL;
   }
}

ossimNitfVqCompressionHeader::ossimNitfVqCompressionHeader()
{
   clearFields();
}

void ossimNitfVqCompressionHeader::parseStream(std::istream &in)
{
   ossimEndian endian;

   in.read((char*)(&theNumberOfImageRows), 4);
   in.read((char*)(&theNumberOfImageCodesPerRow), 4);
   in.read((char*)(&theImageCodeBitLength), 1);
   in.read((char*)(&theCompressionAlgorithmId), 2);

   in.read((char*)(&theNumberOfCompressionLookupOffsetRecords), 2);
   in.read((char*)(&theNumberOfCompressionParameterOffsetRecords), 2);
   in.read((char*)(&theCompressionLookupOffsetTableOffset), 4);
   in.read((char*)(&theCompressionLookupTableOffsetRecordLength), 2);
   
   if(endian.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
   {
      endian.swap(theNumberOfImageRows);
      endian.swap(theNumberOfImageCodesPerRow);
      endian.swap(theCompressionAlgorithmId);
      endian.swap(theNumberOfCompressionLookupOffsetRecords);
      endian.swap(theNumberOfCompressionParameterOffsetRecords);
      endian.swap(theCompressionLookupOffsetTableOffset);
      endian.swap(theCompressionLookupTableOffsetRecordLength);
   }
   
   if((theNumberOfCompressionLookupOffsetRecords > 0)&&
      (theCompressionAlgorithmId == 1)) 
   {
      theTable.clear();
      theTable.resize(theNumberOfCompressionLookupOffsetRecords);

      ossim_uint32 idx = 0;

      for(idx = 0; idx < theNumberOfCompressionLookupOffsetRecords; ++idx)
      {
         theTable[idx].parseStream(in);
      }
      for(idx = 0; idx < theTable.size(); ++idx)
      {
         if(theTable[idx].theData)
         {
            delete [] theTable[idx].theData;
            theTable[idx].theData = 0;
         }
         if(theTable[idx].getDataLengthInBytes()>0)
         {
            theTable[idx].theData = new ossim_uint8[theTable[idx].getDataLengthInBytes()];
            in.read((char*)(theTable[idx].theData), theTable[idx].getDataLengthInBytes());
         }
      }
   }
}

std::ostream& ossimNitfVqCompressionHeader::print(std::ostream& out) const
{
   out << "VQ COMPRESSION HEADER" << std::endl
       << "theNumberOfImageRows:                           " << theNumberOfImageRows << std::endl
       << "theNumberOfImageCodesPerRow:                    " << theNumberOfImageCodesPerRow << std::endl
       << "theImageCodeBitLength:                          " << (ossim_uint32)theImageCodeBitLength << std::endl
       << "theCompressionAlgorithmId:                      " << theCompressionAlgorithmId << std::endl
       << "theNumberOfCompressionLookupOffsetRecords:      " << theNumberOfCompressionLookupOffsetRecords << std::endl
       << "theCompressionLookupTableOffsetRecordLength:    " << theCompressionLookupTableOffsetRecordLength;

   if(theTable.size() > 0)
   {
      ossim_uint32 idx = 0;
      out << std::endl;

      for(idx = 0; idx < theTable.size()-1; ++idx)
      {
         out << theTable[idx] << std::endl;
      }
      out << theTable[idx];
   }

   return out;
}

ossim_uint32 ossimNitfVqCompressionHeader::getBlockSizeInBytes()const
{
   return (getNumberOfImageRows()*
           getNumberOfImageCodesPerRow()*
           getImageCodeBitLength())/8;
}

ossim_uint32 ossimNitfVqCompressionHeader::getNumberOfImageRows()const
{
   return theNumberOfImageRows;
}

ossim_uint32 ossimNitfVqCompressionHeader::getNumberOfImageCodesPerRow()const
{
   return theNumberOfImageCodesPerRow;
}

ossim_uint32 ossimNitfVqCompressionHeader::getCompressionAlgorithmId()const
{
   return theCompressionAlgorithmId;
}

ossim_uint32 ossimNitfVqCompressionHeader::getImageCodeBitLength()const
{
   return theImageCodeBitLength;
}

ossim_uint32 ossimNitfVqCompressionHeader::getNumberOfTables()const
{
   return theTable.size();
}

const std::vector<ossimNitfVqCompressionOffsetTableData>& ossimNitfVqCompressionHeader::getTable()const
{
   return theTable;
}

void ossimNitfVqCompressionHeader::clearFields()
{
   theNumberOfImageRows                        = 0;
   theNumberOfImageCodesPerRow                 = 0;
   theImageCodeBitLength                       = 0;
   theCompressionAlgorithmId                   = 0;
   theNumberOfCompressionLookupOffsetRecords   = 0;
   theCompressionLookupTableOffsetRecordLength = 0;
}

