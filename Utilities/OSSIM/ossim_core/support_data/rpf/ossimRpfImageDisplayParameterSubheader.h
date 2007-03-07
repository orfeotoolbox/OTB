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
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfImageDisplayParameterSubheader.h,v 1.2 2003/06/15 19:01:34 dburken Exp $
#ifndef ossimRpfImageDisplayParameterSubheader_HEADER
#define ossimRpfImageDisplayParameterSubheader_HEADER
#include <iostream>
using namespace std;

#include "base/common/ossimConstants.h" // ossiByteOrder and RPF constants
#include "base/context/ossimErrorContext.h" // for ossimErrorCode

class ossimRpfImageDisplayParameterSubheader
{
public:
   friend ostream& operator<<(ostream& out,
                              const ossimRpfImageDisplayParameterSubheader& data);
   ossimRpfImageDisplayParameterSubheader();
   ossimErrorCode parseStream(istream& in, ossimByteOrder byteOrder);
   
   void print(ostream& out)const;

   ossim_uint32 getStartOffset()const
      {
         return theStartOffset;
      }
   ossim_uint32 getEndOffset()const
      {
         return theEndOffset;
      }
   ossim_uint32 getNumberOfImageRows()const
      {
         return theNumberOfImageRows;
      }
   ossim_uint32 getNumberOfImageCodesPerRow()const
      {
         return theNumberOfImageCodesPerRow;
      }
   ossim_uint32 getImageCodeLength()const
      {
         return theImageCodeBitLength;
      }
   
private:
   void clearFields();

   ossim_uint32 theStartOffset;
   ossim_uint32 theEndOffset;
   
   ossim_uint32 theNumberOfImageRows;
   ossim_uint32 theNumberOfImageCodesPerRow;
   ossim_uint8  theImageCodeBitLength;
};

#endif
