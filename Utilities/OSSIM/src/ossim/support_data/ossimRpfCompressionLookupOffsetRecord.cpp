#include <ossim/support_data/ossimRpfCompressionLookupOffsetRecord.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>

ostream& operator <<(ostream &out,
                     const ossimRpfCompressionLookupOffsetRecord& data)
{
   data.print(out);
   
   return out;
}

ossimRpfCompressionLookupOffsetRecord::ossimRpfCompressionLookupOffsetRecord()
{
   clearFields();
}

ossimErrorCode ossimRpfCompressionLookupOffsetRecord::parseStream(istream& in,
                                                                  ossimByteOrder byteOrder)
{
   if(in)
   {
      ossimEndian anEndian;

      in.read((char*)&theCompressionLookupTableId, 2);
      in.read((char*)&theNumberOfCompressionLookupRecords, 4);
      in.read((char*)&theNumberOfValuesPerCompressionLookupRecord, 2);
      in.read((char*)&theCompressionLookupValueBitLength, 2);
      in.read((char*)&theCompressionLookupTableOffset, 4);
      
      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theCompressionLookupTableId);
         anEndian.swap(theNumberOfCompressionLookupRecords);
         anEndian.swap(theNumberOfValuesPerCompressionLookupRecord);
         anEndian.swap(theCompressionLookupValueBitLength);
         anEndian.swap(theCompressionLookupTableOffset);
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfCompressionLookupOffsetRecord::print(ostream& out)const
{
   out << "theCompressionLookupTableId:                 " << theCompressionLookupTableId << endl
       << "theNumberOfCompressionLookupRecords:         " << theNumberOfCompressionLookupRecords << endl
       << "theNumberOfValuesPerCompressionLookupRecord: " << theNumberOfValuesPerCompressionLookupRecord << endl
       << "theCompressionLookupValueBitLength:          " << theCompressionLookupValueBitLength << endl
       << "theCompressionLookupTableOffset:             " << theCompressionLookupTableOffset;
}

void ossimRpfCompressionLookupOffsetRecord::clearFields()
{
   theCompressionLookupTableId                  = 0;
   theNumberOfCompressionLookupRecords          = 0;
   theNumberOfValuesPerCompressionLookupRecord  = 0;
   theCompressionLookupValueBitLength           = 0;
   theCompressionLookupTableOffset              = 0;
}
