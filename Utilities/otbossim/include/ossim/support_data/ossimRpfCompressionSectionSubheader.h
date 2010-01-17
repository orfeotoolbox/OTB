#ifndef ossimRpfCompressionSectionSubheader_HEADER
#define ossimRpfCompressionSectionSubheader_HEADER
#include <iostream>
using namespace std;

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorContext.h>

class ossimRpfCompressionSectionSubheader
{
public:
   friend ostream& operator<<(ostream& out,
                              const ossimRpfCompressionSectionSubheader& data);
   ossimRpfCompressionSectionSubheader();
   virtual ~ossimRpfCompressionSectionSubheader(){}

   ossimErrorCode parseStream(istream& in, ossimByteOrder byteOrder);

   unsigned long getStartOffset()const{return theStartOffset;}
   unsigned long getEndOffset()const{return theEndOffset;}
   unsigned short getCompressionAlgorithmId()const
      {
         return theCompressionAlgorithmId;
      }
   unsigned short getNumberOfCompressionLookupOffsetRecords()const
      {
         return theNumberOfCompressionLookupOffsetRecords;
      }
   unsigned short getNumberOfCompressionParameterOffsetRecords()const
      {
         return theNumberOfCompressionParameterOffsetRecords;
      }
   void print(ostream& out)const;
   
private:
   void clearFields();

   /*!
    * helper varible that holds the starting absolute
    * offset to the start of the this data.
    */
   unsigned long theStartOffset;

   /*!
    * helper varible that holds the starting absolute
    * offset to the end of the this data.
    */
   unsigned long theEndOffset;
   /*!
    * Current ids are:
    *
    * 1  VQ
    * 2 Jpeg Lossless
    */
   unsigned short theCompressionAlgorithmId;

   unsigned short theNumberOfCompressionLookupOffsetRecords;

   unsigned short theNumberOfCompressionParameterOffsetRecords;
   
};

#endif
