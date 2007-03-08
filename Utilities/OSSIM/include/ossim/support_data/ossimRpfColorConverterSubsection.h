#ifndef ossimRpfColorConverterSubsection_HEADER
#define ossimRpfColorConverterSubsection_HEADER
#include <iostream>
#include <vector>
#include <iterator>
using namespace std;
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/support_data/ossimRpfColorConverterTable.h>

class ossimRpfColorConverterSubsection
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfColorConverterSubsection& data);
   ossimRpfColorConverterSubsection();
   virtual ~ossimRpfColorConverterSubsection(){}

   ossimErrorCode parseStream(istream& in,
                              ossimByteOrder byteOrder);
   
   ossim_uint32 getStartOffset()const{return theStartOffset;}
   ossim_uint32 getEndOffset()const{return theEndOffset;}

   const vector<ossimRpfColorConverterTable>& getColorConversionTable()const
      {
         return theTableList;
      }
   const ossimRpfColorConverterTable* getColorConversionTable(ossim_uint32 givenThisNumberOfEntires)const;
   
   void setNumberOfColorConverterOffsetRecords(ossim_uint16 numberOfRecords);
   void print(ostream& out)const;

   void clearFields();
   
private:
   ossimRpfColorConverterSubsection(const ossimRpfColorConverterSubsection&){}//hide
   void operator =(const ossimRpfColorConverterSubsection&){}//hide

   ossim_uint32  theStartOffset;
   ossim_uint32  theEndOffset;
   
   ossim_uint16  theNumberOfColorConverterOffsetRecords;
   
   ossim_uint32  theColorConverterOffsetTableOffset;
   ossim_uint16  theColorConverterOffsetRecordLength;
   ossim_uint16  theConverterRecordLength;

   vector<ossimRpfColorConverterTable> theTableList;
};

#endif
