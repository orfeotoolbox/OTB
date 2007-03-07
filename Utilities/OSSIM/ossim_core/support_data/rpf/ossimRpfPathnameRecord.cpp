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
// $Id: ossimRpfPathnameRecord.cpp,v 1.2 2004/03/19 18:31:47 gpotts Exp $
#include "ossimRpfPathnameRecord.h"
#include "base/misc/ossimEndian.h"
#include "base/common/ossimErrorCodes.h"

ostream& operator <<(ostream& out,
                     const ossimRpfPathnameRecord& data)
{
   data.print(out);
   
   return out;
}


ossimRpfPathnameRecord::ossimRpfPathnameRecord()
{   
   clearFields();
}

void ossimRpfPathnameRecord::clearFields()
{
   theLength   = 0;
   thePathname = "";
}

ossimErrorCode ossimRpfPathnameRecord::parseStream(istream& in, ossimByteOrder byteOrder)
{
   if(in)
   {
      ossimEndian anEndian;
      
      clearFields();
      in.read((char*)&theLength, 2);

      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theLength);
      }

      char *temp = new char[theLength+1];
      in.read((char*)temp, theLength);
      temp[theLength] = '\0';
      thePathname = temp;
      
      delete [] temp;
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }

   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfPathnameRecord::print(ostream& out)const
{
   out << "theLength:        " << theLength   << endl
       << "thePathname:      " << thePathname;
}
