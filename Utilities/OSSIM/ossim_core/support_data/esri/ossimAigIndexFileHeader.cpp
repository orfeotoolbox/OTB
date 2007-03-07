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
// Author:  Garrett Potts  (gpotts@imagelinks.com)
//
//*******************************************************************
// $Id: ossimAigIndexFileHeader.cpp,v 1.3 2003/05/13 11:31:10 dburken Exp $
#include "support_data/esri/ossimAigIndexFileHeader.h"
#include "base/misc/ossimEndian.h"

bool ossimAigIndexFileHeader::writeStream(std::ostream& out)
{
   ossimEndian endian;
   ossim_uint32 tempInt;
   
   if(endian.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
   {
      out.write((char*)theMagicNumber, 8);
      out.write((char*)theZeroFill1, 16);
      tempInt = theFileSize;
      endian.swap(tempInt);
      out.write((char*)(&tempInt), 4);
      out.write((char*)theZeroFill2, 72);
   }
   else
   {
      out.write((char*)(&theFileSize), 8);
   }
   
   return out.good();
}
