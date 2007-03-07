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
// $Id: ossimRpfBoundaryRectTable.cpp,v 1.1 2003/04/02 20:45:00 gpotts Exp $
#include "ossimRpfBoundaryRectTable.h"
#include <iomanip>
using namespace std;

#include <string.h> // for memset
#include "base/misc/ossimEndian.h"
#include "base/common/ossimErrorCodes.h"

ostream& operator <<(ostream& out,
                     const ossimRpfBoundaryRectTable& data)
{
   data.print(out);

   return out;
}

ossimRpfBoundaryRectTable::ossimRpfBoundaryRectTable(unsigned long numberOfEntries)
{
   theTable.resize(numberOfEntries);  
}

ossimErrorCode ossimRpfBoundaryRectTable::parseStream(istream& in,
                                                      ossimByteOrder byteOrder)
{
   if(in)
   {
      vector<ossimRpfBoundaryRectRecord>::iterator entry = theTable.begin();
      
      while(entry != theTable.end())
      {
         (*entry).parseStream(in, byteOrder);
         ++entry;
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }

   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfBoundaryRectTable::setNumberOfEntries(unsigned long numberOfEntries)
{
   theTable.resize(numberOfEntries);
}

void ossimRpfBoundaryRectTable::print(ostream& out)const
{
   copy(theTable.begin(),
        theTable.end(),
        ostream_iterator<ossimRpfBoundaryRectRecord>(out, "\n"));
}
