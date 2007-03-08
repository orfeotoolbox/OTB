//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  David Burken
//
// Description:
//
// Contains class declaration for  NormalizedRemapTable.  Table for
// normalizing unsigned 8 bit data.
//*******************************************************************
//  $Id: ossimNormalizedRemapTable.h 10456 2007-02-08 14:17:50Z gpotts $


#ifndef ossimNormalizedRemapTable_HEADER
#define ossimNormalizedRemapTable_HEADER

#include <ossim/base/ossimConstants.h>

//*******************************************************************
// CLASS:  ossimNormalizedRemapTable
//*******************************************************************
class OSSIM_DLL ossimNormalizedRemapTable
{
public:
   ossimNormalizedRemapTable();

   virtual ~ossimNormalizedRemapTable(){}

   /*!
    *  Returns a normalized value (between '0.0' and '1.0') from
    *  a pixel value.
    */
   virtual ossim_float64 normFromPix(ossim_int32 pix) const = 0; 

   /*!
    *  Returns an pixel value as an int from a normalized value.
    */
   virtual ossim_int32 pixFromNorm(ossim_float64 normPix) const = 0;

   /*!
    *  Returns a normalized value (between '0.0' and '1.0') from a
    *  pixel value.
    */
   virtual ossim_float64 operator[](ossim_int32 pix) const = 0;
   
private:
   // Forbid copy constructor and asignment operator.
   ossimNormalizedRemapTable(const ossimNormalizedRemapTable& source);
   ossimNormalizedRemapTable& operator=(const ossimNormalizedRemapTable&);  

 };

#endif
