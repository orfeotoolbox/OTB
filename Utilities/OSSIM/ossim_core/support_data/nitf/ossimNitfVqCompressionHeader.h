//*******************************************************************
// Copyright (C) 2004 Garrett Potts
//
// LICENSE: LGPL see top level LICENSE.txt for more details
// 
// Author: Garrett Potts
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfVqCompressionHeader.h,v 1.3 2005/10/17 18:42:27 gpotts Exp $
#ifndef ossimNitfVqCompressionHeader_HEADER
#define ossimNitfVqCompressionHeader_HEADER
#include "ossimNitfCompressionHeader.h"

class OSSIM_DLL ossimNitfVqCompressionOffsetTableData
{
public:
   friend OSSIM_DLL std::ostream& operator<<(std::ostream& out,
                              const ossimNitfVqCompressionOffsetTableData& data);
   ossimNitfVqCompressionOffsetTableData();
   ossimNitfVqCompressionOffsetTableData(const ossimNitfVqCompressionOffsetTableData& rhs);
   ~ossimNitfVqCompressionOffsetTableData();
   const ossimNitfVqCompressionOffsetTableData& operator =(const ossimNitfVqCompressionOffsetTableData& rhs);
   ossim_uint32 getDataLengthInBytes()const;
   void clearFields();
   void parseStream(std::istream& in);
   
   ossim_uint16 theTableId;
   ossim_uint32 theNumberOfCompressionLookupRecords;
   ossim_uint16 theNumberOfValuesPerCompressionLookup;
   ossim_uint16 theCompressionLookupValueBitLength;
   ossim_uint32 theCompressionLookupTableOffset;
   ossim_uint8* theData;
};

class OSSIM_DLL ossimNitfVqCompressionHeader : public ossimNitfCompressionHeader
{
public:
   ossimNitfVqCompressionHeader();
   virtual void parseStream(std::istream &in);
   virtual std::ostream& print(std::ostream& out) const;

   virtual ossim_uint32 getBlockSizeInBytes()const;
   virtual ossim_uint32 getNumberOfImageRows()const;
   virtual ossim_uint32 getNumberOfImageCodesPerRow()const;
   virtual ossim_uint32 getCompressionAlgorithmId()const;
   virtual ossim_uint32 getImageCodeBitLength()const;
   virtual ossim_uint32 getNumberOfTables()const;
   const std::vector<ossimNitfVqCompressionOffsetTableData>& getTable()const;
   
protected:
   ossim_uint32 theNumberOfImageRows;
   ossim_uint32 theNumberOfImageCodesPerRow;
   ossim_uint8  theImageCodeBitLength;

   ossim_uint16 theCompressionAlgorithmId;
   ossim_uint16 theNumberOfCompressionLookupOffsetRecords;
   ossim_uint16 theNumberOfCompressionParameterOffsetRecords;

   ossim_uint32 theCompressionLookupOffsetTableOffset;
   ossim_uint16 theCompressionLookupTableOffsetRecordLength;

   std::vector<ossimNitfVqCompressionOffsetTableData> theTable;
   void clearFields();

TYPE_DATA   
};

#endif
