//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  David Burken
//
// Description:
//
// Contains class declaration for  ossimNormalizedU11RemapTable.  Table for
// normalizing unsigned 11 bit data.
//
//*******************************************************************
//  $Id: ossimNormalizedU11RemapTable.h 10456 2007-02-08 14:17:50Z gpotts $

#ifndef ossimNormalizedU11RemapTable_HEADER
#define ossimNormalizedU11RemapTable_HEADER

#include <ossim/imaging/ossimNormalizedRemapTable.h>

class OSSIM_DLL ossimNormalizedU11RemapTable : public ossimNormalizedRemapTable
{
public:
   ossimNormalizedU11RemapTable();

   virtual ~ossimNormalizedU11RemapTable(){}

   enum
   {
      TABLE_ENTRIES = 2048
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
   ossimNormalizedU11RemapTable(const ossimNormalizedU11RemapTable& source){}
   ossimNormalizedU11RemapTable&
   operator=(const ossimNormalizedU11RemapTable&);  

   static ossim_float64  theTable[TABLE_ENTRIES];
   static bool theTableIsInitialized;
};

inline ossim_float64 ossimNormalizedU11RemapTable::operator[](ossim_int32 pix) const
{
   return (pix < TABLE_ENTRIES ? (pix > 0 ? theTable[pix] : 0.0) : 1.0);  
}

inline ossim_float64 ossimNormalizedU11RemapTable::normFromPix(ossim_int32 pix) const
{
   return (pix < TABLE_ENTRIES ? (pix > 0 ? theTable[pix] : 0.0) : 1.0);
}

inline ossim_int32 ossimNormalizedU11RemapTable::pixFromNorm(ossim_float64 normPix) const
{
   // un-normalize...
   ossim_float64 p = normPix * 2047.0;

   // Ensure pixel is between 0 and 2047.
   p = p < 2047.0 ? (p > 0.0 ? p : 0.0) : 2047.0;

   //***
   // If p is between greater than zero and less than one, make it one as it
   // wasn't a null before.
   // Add 0.5 to p otherwise as it will be cast to an int for proper rounding.
   //***
   p = p > 1.0 ? (p+0.5) : ( !p ? 0.0 : 1.0);
       
   return static_cast<ossim_int32>(p);
}

#endif
