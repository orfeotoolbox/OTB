#ifndef ossimRpfColorGrayscaleSubheader_HEADER
#define ossimRpfColorGrayscaleSubheader_HEADER
#include <iostream>
using namespace std;
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimConstants.h>

class ossimRpfColorGrayscaleSubheader
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfColorGrayscaleSubheader& data);
   
   ossimRpfColorGrayscaleSubheader();
   ~ossimRpfColorGrayscaleSubheader(){}
   
   ossimErrorCode parseStream(istream& in,
                            ossimByteOrder byteOrder);
   void print(ostream& out)const;
   unsigned long getStartOffset()const
      {
         return theStartOffset;
      }
   unsigned long getEndOffset()const
      {
         return theEndOffset;
      }
   unsigned long getNumberOfColorGreyscaleOffsetRecords()const
      {
         return theNumberOfColorGreyscaleOffsetRecords;
      }
   unsigned long getNumberOfColorConverterOffsetRecords()const
      {
         return theNumberOfColorConverterOffsetRecords;
      }
   
private:
   void clearFields();

   unsigned long theStartOffset;
   unsigned long theEndOffset;
   
   unsigned char theNumberOfColorGreyscaleOffsetRecords;
   unsigned char theNumberOfColorConverterOffsetRecords;

   /*!
    * 12 byte field.
    */
   ossimString         theColorGrayscaleFilename;
};

#endif
