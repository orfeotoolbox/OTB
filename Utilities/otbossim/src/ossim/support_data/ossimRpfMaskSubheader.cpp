#include <ossim/support_data/ossimRpfMaskSubheader.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>
#include <cmath>
ostream& operator <<(ostream& out,
                     const ossimRpfMaskSubheader& data)
{
   data.print(out);

   return out;
}

ossimRpfMaskSubheader::ossimRpfMaskSubheader()
   :theOutputPixelCodeBitString(NULL)
{
   clearFields();
}

ossimErrorCode ossimRpfMaskSubheader::parseStream(istream& in,
                                        ossimByteOrder byteOrder)
{
   if(in)
   {
      clearFields();

      in.read((char*)&theSubframeSequenceRecordLength, 2);
      in.read((char*)&theTransparencySequenceRecordLength, 2);
      in.read((char*)&theTransparentOutputPixelCodeLength, 2);
      ossimEndian anEndian;

      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theSubframeSequenceRecordLength);
         anEndian.swap(theTransparencySequenceRecordLength);
         anEndian.swap(theTransparentOutputPixelCodeLength);

      }
      if(theTransparentOutputPixelCodeLength>0)
      {
         theNumberOfBytes = (long)std::ceil(theTransparentOutputPixelCodeLength/8.0);
         theOutputPixelCodeBitString = new unsigned char[theNumberOfBytes];
         in.read((char*)theOutputPixelCodeBitString, theNumberOfBytes);
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }

   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfMaskSubheader::print(ostream& out)const
{
   out << "theSubframeSequenceRecordLength:     " << theSubframeSequenceRecordLength << endl
       << "theTransparencySequenceRecordLength: " << theTransparencySequenceRecordLength << endl
       << "theTransparentOutputPixelCodeLength: " << theTransparentOutputPixelCodeLength << endl
       << "theNumberOfBytes:                    " << theNumberOfBytes << endl
       << "theOutputPixelCodeBitString:         ";

   // display the pixel code as a bit pattern.
   for(long index = theNumberOfBytes-1;
       index >=0;
       --index)
   {
      unsigned long temp = theOutputPixelCodeBitString[index];
      out << ((temp>>7)&1)
          << ((temp>>6)&1)
          << ((temp>>5)&1)
          << ((temp>>4)&1)
          << ((temp>>3)&1)
          << ((temp>>2)&1)
          << ((temp>>1)&1)
          << (temp&1);
   }
}

void ossimRpfMaskSubheader::clearFields()
{
   theSubframeSequenceRecordLength     = 0;
   theTransparencySequenceRecordLength = 0;
   theTransparentOutputPixelCodeLength = 0;
   theNumberOfBytes                    = 0;

   if(theOutputPixelCodeBitString)
   {
      delete [] theOutputPixelCodeBitString;
      theOutputPixelCodeBitString = NULL;
   }
}
