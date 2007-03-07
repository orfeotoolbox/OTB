//*******************************************************************
// Copyright (C) 2002 ImageLinks Inc. 
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
// Author:  David Burken <dburken@imagelinks.com>
// 
// Description: Nitf support class.  Copied from ossimNitfImageLutV2_0.
// 
//********************************************************************
// $Id: ossimNitfImageLutV2_1.cpp,v 1.2 2004/11/11 14:23:34 gpotts Exp $

#include "ossimNitfImageLutV2_1.h"
#include <string.h> // memcpy

#ifndef NULL
#include <stddef.h>
#endif

ossimNitfImageLutV2_1::ossimNitfImageLutV2_1()
{
}

ossimNitfImageLutV2_1::~ossimNitfImageLutV2_1()
{
}

// void ossimNitfImageLutV2_1::parseStream(istream &in)
// {
//    if(in)
//    {
//       long count = 0;
//       while(count < theNumberOfEntries)
//       {
//          in.read((char*)&theData[count], 1);

//          ++count;
//       }
//    }
// }

// void ossimNitfImageLutV2_1::print(ostream& out)const
// {
//    out << "theNumberOfEntries:          " << theNumberOfEntries;
// }

// long ossimNitfImageLutV2_1::getNumberOfEntries()const
// {
//    return theNumberOfEntries;
// }

// void ossimNitfImageLutV2_1::setNumberOfEntries(long numberOfEntries)
// {
//    theData.resize(numberOfEntries);
//    theNumberOfEntries = numberOfEntries;
// }

// unsigned char ossimNitfImageLutV2_1::getValue(long index)
// {
//    return theData[index];
// }
