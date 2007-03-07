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
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfImageLutV2_0.cpp,v 1.2 2004/11/11 14:23:34 gpotts Exp $
#include "ossimNitfImageLutV2_0.h"
#include <string.h> // memcpy

#ifndef NULL
#include <stddef.h>
#endif

ossimNitfImageLutV2_0::ossimNitfImageLutV2_0()
{
}

ossimNitfImageLutV2_0::~ossimNitfImageLutV2_0()
{
}

void ossimNitfImageLutV2_0::parseStream(std::istream &in)
{
   if(in)
   {
      ossim_uint32 count = 0;
      while(count < theNumberOfEntries)
      {
         in.read((char*)&theData[count], 1);

         ++count;
      }
   }
}

std::ostream& ossimNitfImageLutV2_0::print(ostream& out)const
{
   out << "theNumberOfEntries:          " << theNumberOfEntries;

   return out;
}

ossim_uint32 ossimNitfImageLutV2_0::getNumberOfEntries()const
{
   return theNumberOfEntries;
}

void ossimNitfImageLutV2_0::setNumberOfEntries(ossim_uint32 numberOfEntries)
{
   theData.resize(numberOfEntries);
   theNumberOfEntries = numberOfEntries;
}

ossim_uint8 ossimNitfImageLutV2_0::getValue(ossim_uint32 index)const
{
   return theData[index];
}
