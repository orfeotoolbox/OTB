#ifndef ossimRpfCompressionSection_HEADER
#define ossimRpfCompressionSection_HEADER
#include <iostream>
#include <vector>
#include <iterator>
using namespace std;
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimConstants.h>

class ossimRpfCompressionSectionSubheader;

struct ossimRpfCompressionOffsetTableData
{
   friend ostream& operator<<(ostream& out,
                              const ossimRpfCompressionOffsetTableData& data);
   ossimRpfCompressionOffsetTableData();
   ossimRpfCompressionOffsetTableData(const ossimRpfCompressionOffsetTableData& rhs);
   ~ossimRpfCompressionOffsetTableData();
   const ossimRpfCompressionOffsetTableData& operator =(const ossimRpfCompressionOffsetTableData& rhs);
   
   ossim_uint16 theTableId;
   ossim_uint32 theNumberOfLookupValues;
   ossim_uint16 theCompressionLookupValueBitLength;
   ossim_uint16 theNumberOfValuesPerLookup;
   ossim_uint8* theData;
};

class ossimRpfCompressionSection
{
public:
   friend ostream& operator << (ostream& out,
                                const ossimRpfCompressionSection& data);
   ossimRpfCompressionSection();
   virtual ~ossimRpfCompressionSection();
   ossimErrorCode parseStream(istream& in,
                              ossimByteOrder byteOrder);
   void print(ostream& out)const;
   const vector<ossimRpfCompressionOffsetTableData>& getTable()const
      {
         return theTable;
      }
   const ossimRpfCompressionSectionSubheader* getSubheader()const
   {
      return theSubheader;
   }
private:
   void clearTable();
   
   ossimRpfCompressionSectionSubheader* theSubheader;
   
   ossim_uint32 theCompressionLookupOffsetTableOffset;
   ossim_uint16 theCompressionLookupTableOffsetRecordLength;

   vector<ossimRpfCompressionOffsetTableData> theTable;
};

#endif
