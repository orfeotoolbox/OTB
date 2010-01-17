#include <ossim/support_data/ossimRpfColorConverterOffsetRecord.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>

ostream& operator <<(ostream& out,
                     const ossimRpfColorConverterOffsetRecord& data)
{
   out << "theColorConverterTableId:                 " << data.theColorConverterTableId << endl
       << "theNumberOfColorConverterRecords:         " << data.theNumberOfColorConverterRecords << endl
       << "theColorConverterTableOffset:             " << data.theColorConverterTableOffset << endl
       << "theSourceColorGrayscaleOffsetTableOffset: " << data.theSourceColorGrayscaleOffsetTableOffset << endl
       << "theTargetColorGrayscaleOffsetTableOffset: " << data.theTargetColorGrayscaleOffsetTableOffset;

   return out;
}

ossimRpfColorConverterOffsetRecord::ossimRpfColorConverterOffsetRecord()
{
   clearFields();
}

ossimErrorCode ossimRpfColorConverterOffsetRecord::parseStream(istream& in,
                                                               ossimByteOrder byteOrder)
{
   if(in)
   {
      in.read((char*)&theColorConverterTableId, 2);
      in.read((char*)&theNumberOfColorConverterRecords, 4);
      in.read((char*)&theColorConverterTableOffset, 4);
      in.read((char*)&theSourceColorGrayscaleOffsetTableOffset, 4);
      in.read((char*)&theTargetColorGrayscaleOffsetTableOffset, 4);
      
      ossimEndian anEndian;
      
      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theColorConverterTableId);
         anEndian.swap(theNumberOfColorConverterRecords);
         anEndian.swap(theColorConverterTableOffset);
         anEndian.swap(theSourceColorGrayscaleOffsetTableOffset);
         anEndian.swap(theTargetColorGrayscaleOffsetTableOffset);
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }

   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfColorConverterOffsetRecord::clearFields()
{
   theColorConverterTableId                 = 0;
   theNumberOfColorConverterRecords         = 0;
   theColorConverterTableOffset             = 0;
   theSourceColorGrayscaleOffsetTableOffset = 0;
   theTargetColorGrayscaleOffsetTableOffset = 0;
}

