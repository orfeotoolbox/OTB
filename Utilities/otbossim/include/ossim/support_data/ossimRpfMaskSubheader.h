#ifndef ossimRpfMaskSubheader_HEADER
#define ossimRpfMaskSubheader_HEADER
#include <iostream>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimString.h>

class ossimRpfMaskSubheader
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                                    const ossimRpfMaskSubheader& data);
   ossimRpfMaskSubheader();

   virtual ~ossimRpfMaskSubheader();

   ossimErrorCode parseStream(std::istream& in,
                              ossimByteOrder byteOrder);
   void print(std::ostream& out)const;
   
private:
   void clearFields();

   unsigned short theSubframeSequenceRecordLength;
   unsigned short theTransparencySequenceRecordLength;
   unsigned short theTransparentOutputPixelCodeLength;
   unsigned char* theOutputPixelCodeBitString;

   /*!
    * This is just a working variable and is not
    * part of the Rpf fields. this is the number of bytes
    * of the output pixel code length.  It divides by
    * 8 and then does the ceiling of the
    * theTransparentOutputPixelCodeLength.
    */
   long theNumberOfBytes;
};
#endif
