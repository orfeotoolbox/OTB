#ifndef ossimRpfMaskSubheader_HEADER
#define ossimRpfMaskSubheader_HEADER
#include <iostream>
using namespace std;

#include "base/common/ossimConstants.h" // ossiByteOrder and RPF constants
#include "base/context/ossimErrorContext.h" // for ossimErrorCode
#include "base/data_types/ossimString.h"

class ossimRpfMaskSubheader
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfMaskSubheader& data);
   ossimRpfMaskSubheader();
   virtual ~ossimRpfMaskSubheader(){}

   ossimErrorCode parseStream(istream& in,
                              ossimByteOrder byteOrder);
   void print(ostream& out)const;
   
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
