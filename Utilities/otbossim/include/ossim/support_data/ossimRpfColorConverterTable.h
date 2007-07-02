#ifndef ossimRpfColorConverterTable_HEADER
#define ossimRpfColorConverterTable_HEADER
#include <iostream>
using namespace std;

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorContext.h>

class ossimRpfColorConverterTable
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfColorConverterTable& data);
   
   ossimRpfColorConverterTable();
   virtual ~ossimRpfColorConverterTable();

   ossimErrorCode parseStream(istream& in,
                              ossimByteOrder byteOrder);
   void print(ostream& out)const;
   ossim_uint32 getEntry(ossim_uint32 entry){return theColorGrayscaleTableEntryList[entry];}
   void setNumberOfEntries(ossim_uint32 entries);
   void setTableId(ossim_uint16 id);
   const ossim_uint32* getEntryList()const{return theColorGrayscaleTableEntryList;}
   ossim_uint32 getNumberOfEntries()const{return theNumberOfEntries;}
   
private:
   /*!
    * This will not be a field read from the stream this is
    * the value in the color converter offset table.
    */
   ossim_uint16 theTableId;

   /*!
    * Holds the number of entries
    */ 
   ossim_uint32  theNumberOfEntries;

   /*!
    * This will hold the lookup table values.
    */
   ossim_uint32* theColorGrayscaleTableEntryList;
};

#endif
