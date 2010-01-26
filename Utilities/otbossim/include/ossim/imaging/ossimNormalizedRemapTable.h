//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Description:
//
// Contains class declaration for base class ossimNormalizedRemapTable.
// 
//*******************************************************************
//  $Id: ossimNormalizedRemapTable.h 15742 2009-10-17 12:59:43Z dburken $


#ifndef ossimNormalizedRemapTable_HEADER
#define ossimNormalizedRemapTable_HEADER

#include <ossim/base/ossimConstants.h>

/**
 * @class ossimNormalizedRemapTable
 *
 * @brief Base class implemetation of normalized remap tables to go to/from
 * normalized value to pixel value.
 */
class OSSIM_DLL ossimNormalizedRemapTable
{
public:

   /** default constructor */
   ossimNormalizedRemapTable();

   /** virtual destructor */
   virtual ~ossimNormalizedRemapTable();

   /**
    *  @brief Gets a normalized value (between '0.0' and '1.0') from
    *  a pixel value.
    *
    *  @return Value between 0.0 and 1.0.
    */
   virtual ossim_float64 operator[](ossim_int32 pix) const = 0;

   /**
    *  @brief Gets a normalized value (between '0.0' and '1.0') from
    *  a pixel value.
    *
    *  @return Value between 0.0 and 1.0.
    */
   virtual ossim_float64 normFromPix(ossim_int32 pix) const = 0;

   /**
    * @brief Returns an pixel value as an int from a normalized value.
    *
    * @return Value between scalar range of remap table.
    */
   virtual ossim_int32 pixFromNorm(ossim_float64 normPix) const = 0;

   /**
    * @brief Pure virtual method to get the number of table entries.
    * @return The number of entries in a table.
    */
   virtual ossim_int32 getEntries() const = 0;

   /**
    * @brief Get the value used to normalize and un-normalize table entries.
    * @return Value used to normalize and un-normalize table entries.
    */
   virtual ossim_float64 getNormalizer() const;

private:

   // Forbid copy constructor and asignment operator.
   ossimNormalizedRemapTable(const ossimNormalizedRemapTable& source);
   ossimNormalizedRemapTable& operator=(const ossimNormalizedRemapTable&);  

 };

inline ossim_float64 ossimNormalizedRemapTable::getNormalizer() const
{
   return static_cast<ossim_float64>(getEntries()-1);
}

#endif
