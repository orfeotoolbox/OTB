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
// $Id: ossimRpfBoundaryRectSectionSubheader.cpp,v 1.1 2003/04/02 20:45:00 gpotts Exp $
#include "ossimRpfBoundaryRectSectionSubheader.h"
#include "base/misc/ossimEndian.h"
#include "base/common/ossimErrorCodes.h"

ostream& operator <<(ostream& out,
                     const ossimRpfBoundaryRectSectionSubheader &data)
{
   data.print(out);

   return out;
}

ossimRpfBoundaryRectSectionSubheader::ossimRpfBoundaryRectSectionSubheader()
{
   clearFields();
}

ossimErrorCode ossimRpfBoundaryRectSectionSubheader::parseStream(istream& in,
                                                                 ossimByteOrder byteOrder)
{
   if(in)
   {
      ossimEndian anEndian;

      clearFields();
      
      in.read((char*)&theRectangleTableOffset, 4);
      in.read((char*)&theNumberOfEntries, 2);
      in.read((char*)&theLengthOfEachEntry, 2);

      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theRectangleTableOffset);
         anEndian.swap(theNumberOfEntries);
         anEndian.swap(theLengthOfEachEntry);
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfBoundaryRectSectionSubheader::print(ostream& out)const
{
   out << "theRectangleTableOffset:      " << theRectangleTableOffset << endl
       << "theNumberOfEntries:           " << theNumberOfEntries << endl
       << "theLengthOfEachEntry:         " << theLengthOfEachEntry;
}

void ossimRpfBoundaryRectSectionSubheader::clearFields()
{
   theRectangleTableOffset = 0;
   theNumberOfEntries      = 0;
   theLengthOfEachEntry    = 0;
}
