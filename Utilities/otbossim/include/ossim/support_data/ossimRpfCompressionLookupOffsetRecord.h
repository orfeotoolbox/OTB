#ifndef ossimRpfCompressionLookupOffsetRecord_HEADER
#define ossimRpfCompressionLookupOffsetRecord_HEADER
#include <iostream>
using namespace std;
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorContext.h>

class ossimRpfCompressionLookupOffsetRecord
{
public:
   friend ostream& operator <<(ostream &out,
                               const ossimRpfCompressionLookupOffsetRecord& data);
   ossimRpfCompressionLookupOffsetRecord();
   ossimErrorCode parseStream(istream& in,
                              ossimByteOrder byteOrder);
   void print(ostream& out)const;
   ossim_uint16 getCompressionLookupTableId()const
      {
         return theCompressionLookupTableId;
      }
   ossim_uint32 getNumberOfCompressionLookupRecords()const
      {
         return theNumberOfCompressionLookupRecords;
      }
   ossim_uint16 getNumberOfValuesPerCompressionLookupRecord()const
      {
         return theNumberOfValuesPerCompressionLookupRecord;
      }
   ossim_uint16 getCompressionLookupValueBitLength()const
      {
         return theCompressionLookupValueBitLength;
      }
   ossim_uint32 getCompressionLookupTableOffset()const
      {
         return theCompressionLookupTableOffset;
      }
   
private:
   void clearFields();
   
   ossim_uint16 theCompressionLookupTableId;
   ossim_uint32 theNumberOfCompressionLookupRecords;
   ossim_uint16 theNumberOfValuesPerCompressionLookupRecord;
   ossim_uint16 theCompressionLookupValueBitLength;
   ossim_uint32 theCompressionLookupTableOffset;
};

#endif
