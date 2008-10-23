#include <ossim/support_data/ossimRpfCompressionSectionSubheader.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>

ossimRpfCompressionSectionSubheader::ossimRpfCompressionSectionSubheader()
{
   clearFields();
}

ostream& operator<<(ostream& out,
                    const ossimRpfCompressionSectionSubheader& data)
{
   data.print(out);
   
   return out;
}

ossimErrorCode ossimRpfCompressionSectionSubheader::parseStream(istream& in,
                                                                ossimByteOrder byteOrder)
{
   if(in)
   {
      theStartOffset = in.tellg();
      
      in.read((char*)&theCompressionAlgorithmId, 2);
      in.read((char*)&theNumberOfCompressionLookupOffsetRecords, 2);
      in.read((char*)&theNumberOfCompressionParameterOffsetRecords, 2);

      theEndOffset = in.tellg();
      
      ossimEndian anEndian;

      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theCompressionAlgorithmId);
         anEndian.swap(theNumberOfCompressionLookupOffsetRecords);
         anEndian.swap(theNumberOfCompressionParameterOffsetRecords);
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }

   return ossimErrorCodes::OSSIM_OK;
}
   
void ossimRpfCompressionSectionSubheader::print(ostream& out)const
{
   out << "theCompressionAlgorithmId:                    " << theCompressionAlgorithmId << endl
       << "theNumberOfCompressionLookupOffsetRecords:    " << theNumberOfCompressionLookupOffsetRecords << endl
       << "theNumberOfCompressionParameterOffsetRecords: " << theNumberOfCompressionParameterOffsetRecords;
}

void ossimRpfCompressionSectionSubheader::clearFields()
{
   theStartOffset                               = 0;
   theEndOffset                                 = 0;
   theCompressionAlgorithmId                    = 0;
   theNumberOfCompressionLookupOffsetRecords    = 0;
   theNumberOfCompressionParameterOffsetRecords = 0;
}
