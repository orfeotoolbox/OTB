//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  David Burken
//
// Description:
//
// Contains class declaration for  ossimNormalizedU16RemapTable.  Table for
// normalizing unsigned 16 bit data.
//*******************************************************************
//  $Id: ossimNormalizedU16RemapTable.h 10456 2007-02-08 14:17:50Z gpotts $

#ifndef ossimNormalizedU16RemapTable_HEADER
#define ossimNormalizedU16RemapTable_HEADER

#include <ossim/imaging/ossimNormalizedRemapTable.h>

class OSSIM_DLL ossimNormalizedU16RemapTable : public ossimNormalizedRemapTable
{
public:
   ossimNormalizedU16RemapTable();

   virtual ~ossimNormalizedU16RemapTable(){}

   enum
   {
      TABLE_ENTRIES   = 65536
   };
   
   virtual ossim_float64 normFromPix(ossim_int32 pix) const;
      //> Returns a normalized value (between '0.0' and '1.0') from an eight
      //  bit unsigned char.
      //<
   
   virtual ossim_int32 pixFromNorm(ossim_float64 normPix) const;
      //> Returns a pixel value from a normalized value.
      //  Notes:
      //  - Valid returns range from '0' to '255' so given a
      //    value greater than '1.0' method returns '255' and given a
      //    value less than '0.0' method returns '0.0'.
      //<

   virtual ossim_float64 operator[](ossim_int32 pix) const;
      //> Returns a normalized value (between '0.0' and '1.0') from an eight
      //  bit unsigned char.
      //<
   
private:
   // Forbid copy constructor and asignment operator.
   ossimNormalizedU16RemapTable(const ossimNormalizedU16RemapTable& source){}
   ossimNormalizedU16RemapTable&
   operator=(const ossimNormalizedU16RemapTable&);  

   static ossim_float64  theTable[TABLE_ENTRIES];
   static bool theTableIsInitialized;
};

inline ossim_float64 ossimNormalizedU16RemapTable::operator[](ossim_int32 pix) const
{
   return (pix < TABLE_ENTRIES ? (pix > 0 ? theTable[pix] : 0.0) : 1.0);  
}

inline ossim_float64 ossimNormalizedU16RemapTable::normFromPix(ossim_int32 pix) const
{
   return (pix < TABLE_ENTRIES ? (pix > 0 ? theTable[pix] : 0.0) : 1.0);
}

inline ossim_int32 ossimNormalizedU16RemapTable::pixFromNorm(ossim_float64 normPix) const
{
   // un-normalize...
   ossim_float64 p = normPix * 65535.0;

   // Ensure pixel is between 0 and 65535.
   p = p < 65535.0 ? (p > 0.0 ? p : 0.0) : 65535.0;

   //***
   // If p is between greater than zero and less than one, make it one as it
   // wasn't a null before.
   // Add 0.5 to p otherwise as it will be cast to an int for proper rounding.
   //***
   p = p > 1.0 ? (p+0.5) : ( !p ? 0.0 : 1.0);
       
   return static_cast<ossim_int32>(p);
}

#endif
