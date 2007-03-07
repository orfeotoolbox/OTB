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
// $Id: ossimRpfImageDisplayParameterSubheader.cpp,v 1.1 2003/04/02 20:45:00 gpotts Exp $
#include "ossimRpfImageDisplayParameterSubheader.h"

#include "base/misc/ossimEndian.h"
#include "base/common/ossimErrorCodes.h"

ostream& operator<<(ostream& out,
                    const ossimRpfImageDisplayParameterSubheader& data)
{
   data.print(out);

   return out;
}

ossimRpfImageDisplayParameterSubheader::ossimRpfImageDisplayParameterSubheader()
{
   clearFields();
}

ossimErrorCode ossimRpfImageDisplayParameterSubheader::parseStream(istream& in,
                                                         ossimByteOrder byteOrder)
{
   clearFields();
   if(in)
   {
      theStartOffset = in.tellg();
      in.read((char*)&theNumberOfImageRows, 4);
      in.read((char*)&theNumberOfImageCodesPerRow, 4);
      in.read((char*)&theImageCodeBitLength, 1);
      theEndOffset = in.tellg();

      ossimEndian anEndian;
      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theNumberOfImageRows);
         anEndian.swap(theNumberOfImageCodesPerRow);
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   return ossimErrorCodes::OSSIM_OK;
}
   
void ossimRpfImageDisplayParameterSubheader::print(ostream& out)const
{
   out << "theNumberOfImageRows:          " << theNumberOfImageRows << endl
       << "theNumberOfImageCodesPerRow:   " << theNumberOfImageCodesPerRow << endl
       << "theImageCodeBitLength:         " << (unsigned long)theImageCodeBitLength;
}

void ossimRpfImageDisplayParameterSubheader::clearFields()
{
   theNumberOfImageRows        = 0;
   theNumberOfImageCodesPerRow = 0;
   theImageCodeBitLength       = 0;
}
