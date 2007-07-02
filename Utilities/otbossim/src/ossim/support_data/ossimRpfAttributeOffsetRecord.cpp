#include <ossim/support_data/ossimRpfAttributeOffsetRecord.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>

ostream& operator <<(ostream& out,
                     const ossimRpfAttributeOffsetRecord& data)
{
   data.print(out);
   
   return out;
}

ossimRpfAttributeOffsetRecord::ossimRpfAttributeOffsetRecord()
{
   clearFields();
}

ossimErrorCode ossimRpfAttributeOffsetRecord::parseStream(istream& in,
                                                          ossimByteOrder byteOrder)
{
   if(in)
   {
      in.read((char*)&theAttributeId, 2);
      in.read((char*)&theParameterId, 1);
      in.read((char*)&theArealCoverageSequenceNumber, 1);
      in.read((char*)&theAttributeRecordOffset, 4);

      ossimEndian anEndian;

      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theAttributeId);
         anEndian.swap(theAttributeRecordOffset);
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }

   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfAttributeOffsetRecord::print(ostream& out)const
{
   out << "theAttributeId:                  " << theAttributeId << endl
       << "theParameterId:                  " << theParameterId << endl
       << "theArealCoverageSequenceNumber:  " << theArealCoverageSequenceNumber << endl
       << "theAttributeRecordOffset:        " << theAttributeRecordOffset;
}

void ossimRpfAttributeOffsetRecord::clearFields()
{
   theAttributeId                 = 0;
   theParameterId                 = 0;
   theArealCoverageSequenceNumber = 0;
   theAttributeRecordOffset       = 0;
}
