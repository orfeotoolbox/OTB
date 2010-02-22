//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Description:
//
// Contains class declaration for  ossimNormalizedS16RemapTable.  Table for
// normalizing signed 16 bit data.
// 
//*******************************************************************
//  $Id: ossimNormalizedS16RemapTable.h 10456 2007-02-08 14:17:50Z gpotts $

#ifndef ossimNormalizedS16RemapTable_HEADER
#define ossimNormalizedS16RemapTable_HEADER

#include <ossim/imaging/ossimNormalizedRemapTable.h>
#include <ossim/base/ossimCommon.h> /* for round */

/**
 * @class ossimNormalizedS16RemapTable
 *
 * @brief Signed 16 bit normalized remap tables to go to/from
 * normalized value to pixel value.
 */
class OSSIM_DLL ossimNormalizedS16RemapTable : public ossimNormalizedRemapTable
{
public:
   
   /** @brief default constructor */
   ossimNormalizedS16RemapTable();

   /** @brief virtual destructor */
   virtual ~ossimNormalizedS16RemapTable();

   enum
   {
      TABLE_ENTRIES  = 65536,  // 2^16 32767-(-32768)+1
      OFFSET_TO_ZERO = 32768,  // Gets -32768 to zero.
      NULL_PIX       = -32768,
      MIN_PIX        = -32767 
   };

   /**
    * @brief Gets the number of table entries.
    * @return The number of entries in a table.
    */
   virtual ossim_int32 getEntries() const;

   /**
    * @brief Gets normalized value (between '0.0' and '1.0') from an
    * int which should in scalar range of a signed 16 bit.
    *
    * @note This table is specialized to map both -32768 and -32767 to 0 since
    * dted data has a null of -32767.
    * 
    * @return Value between 0.0 and 1.0.
    */
   virtual ossim_float64 operator[](ossim_int32 pix) const;

   /**
    * @brief Gets normalized value (between '0.0' and '1.0') from an
    * int which should in scalar range of a signed 16 bit.
    *
    * @note This table is specialized to map both -32768 and -32767 to 0 since
    * dted data has a null of -32767.
    *
    * @return Value between 0.0 and 1.0.
    */
   virtual ossim_float64 normFromPix(ossim_int32 pix) const;

   /**
    * @brief Gets pixel value from normalized value.
    *
    * Valid returns range is signed 16 bit range or -32768 to 32767.
    *
    * @return Value between -32768 to 32767.
    */
   virtual ossim_int32 pixFromNorm(ossim_float64 normPix) const;

private:

   static ossim_float64  theTable[TABLE_ENTRIES];
   static bool theTableIsInitialized;

};

inline ossim_int32 ossimNormalizedS16RemapTable::getEntries() const
{
   return TABLE_ENTRIES;
}

inline ossim_float64 ossimNormalizedS16RemapTable::operator[](
   ossim_int32 pix) const
{
   ossim_float64 result = 0;

   // Move pix into table range.  This will take -32768 to 0.
   ossim_int32 p = pix + OFFSET_TO_ZERO;

   result = (p < TABLE_ENTRIES ? ( p >= 0 ? theTable[p] : 0.0) : 1.0);

   return result;
}

inline ossim_float64 ossimNormalizedS16RemapTable::normFromPix(
   ossim_int32 pix) const
{
   ossim_float64 result = 0;

   // Move pix into table range.  This will take -32768 to 0.
   ossim_int32 p = pix + OFFSET_TO_ZERO;

   result = (p < TABLE_ENTRIES ? ( p >= 0 ? theTable[p] : 0.0) : 1.0);

   return result;
}

inline ossim_int32 ossimNormalizedS16RemapTable::pixFromNorm(
   ossim_float64 normPix) const
{
   if(normPix <= 0.0) return NULL_PIX;
   
   // Clamp between 0 and 1 on the way in.
   ossim_float64 p = (normPix<1.0) ? ( (normPix>0.0) ? normPix : 0.0) : 1.0;

   // Un-normalize.
   p = p * getNormalizer(); // TABLE_ENTRIES - 1;

   //---
   // Move pixel into sign range then round it.  This will take  65535 to
   // 32767 which is the max pixel for this scalar type.
   //---
   p = ossim::round<ossim_int32>(p - OFFSET_TO_ZERO);

   if (p == NULL_PIX)
   {
      // norm pixel came in just above zero so should be at least min.
      p = MIN_PIX; 
   }

   return static_cast<ossim_int32>(p);
}

#endif
