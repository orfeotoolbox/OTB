//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author:  David Burken (dburken@imagelinks.com)
//
// Description:
//
// Contains class declaration for  ossimNormalizedU16RemapTable.  Table for
// normalizing unsigned 16 bit data.
//*******************************************************************
//  $Id: ossimNormalizedU16RemapTable.h,v 1.3 2003/05/13 11:31:06 dburken Exp $

#ifndef ossimNormalizedU16RemapTable_HEADER
#define ossimNormalizedU16RemapTable_HEADER

#include "ossimNormalizedRemapTable.h"

class ossimNormalizedU16RemapTable : public ossimNormalizedRemapTable
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
