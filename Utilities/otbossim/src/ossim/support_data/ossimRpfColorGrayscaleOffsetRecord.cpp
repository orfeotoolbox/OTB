#include <ossim/support_data/ossimRpfColorGrayscaleOffsetRecord.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>

ostream& operator <<(ostream& out,
                     const ossimRpfColorGrayscaleOffsetRecord& data)
{
   data.print(out);
   
   return out;
}
   
ossimRpfColorGrayscaleOffsetRecord::ossimRpfColorGrayscaleOffsetRecord()
{
   clearFields();
}

ossimErrorCode ossimRpfColorGrayscaleOffsetRecord::parseStream(istream& in,
                                                               ossimByteOrder byteOrder)
{
   if(in)
   {
      clearFields();
      
      in.read((char*)&theColorGrayscaleTableId, 2);
      in.read((char*)&theNumberOfColorGrayscaleRecords, 4);
      in.read((char*)&theColorGrayscaleElementLength, 1);
      in.read((char*)&theHistogramRecordLength, 2);
      in.read((char*)&theColorGrayscaleTableOffset, 4);
      in.read((char*)&theHistogramTableOffset, 4);

      ossimEndian anEndian;

      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theColorGrayscaleTableId);
         anEndian.swap(theNumberOfColorGrayscaleRecords);
         anEndian.swap(theHistogramRecordLength);
         anEndian.swap(theColorGrayscaleTableOffset);
         anEndian.swap(theHistogramTableOffset);
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }

   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfColorGrayscaleOffsetRecord::print(ostream& out)const
{
   out << "theColorGrayscaleTableId:                 " << theColorGrayscaleTableId << endl
       << "theNumberOfColorGrayscaleRecords:         " << theNumberOfColorGrayscaleRecords << endl
       << "theColorGrayscaleElementLength:           " << (unsigned long)theColorGrayscaleElementLength << endl
       << "theHistogramRecordLength:                 " << theHistogramRecordLength << endl
       << "theColorGrayscaleTableOffset:             " << theColorGrayscaleTableOffset << endl
       << "theHistogramTableOffset:                  " << theHistogramTableOffset;
}

void ossimRpfColorGrayscaleOffsetRecord::clearFields()
{
   theColorGrayscaleTableId         = 0;
   theNumberOfColorGrayscaleRecords = 0;
   theColorGrayscaleElementLength   = 0;
   theHistogramRecordLength         = 0;
   theColorGrayscaleTableOffset     = 0;
   theHistogramTableOffset          = 0;
}
