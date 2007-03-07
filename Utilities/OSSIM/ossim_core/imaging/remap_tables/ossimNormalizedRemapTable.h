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
// Contains class declaration for  NormalizedRemapTable.  Table for
// normalizing unsigned 8 bit data.
//*******************************************************************
//  $Id: ossimNormalizedRemapTable.h,v 1.3 2003/05/13 11:31:06 dburken Exp $


#ifndef ossimNormalizedRemapTable_HEADER
#define ossimNormalizedRemapTable_HEADER

#include "base/common/ossimConstants.h"

//*******************************************************************
// CLASS:  ossimNormalizedRemapTable
//*******************************************************************
class ossimNormalizedRemapTable
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
