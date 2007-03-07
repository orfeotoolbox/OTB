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
// $Id: ossimNitfEmbeddedRpfHeader.cpp,v 1.7 2004/09/28 15:35:30 gpotts Exp $
#include "ossimNitfEmbeddedRpfHeader.h"
#include "support_data/rpf/ossimRpfHeader.h"

RTTI_DEF1(ossimNitfEmbeddedRpfHeader, "ossimNitfEmbeddedRpfHeader", ossimNitfRegisteredTag)
   
ossimNitfEmbeddedRpfHeader::ossimNitfEmbeddedRpfHeader()
   : theRpfHeader(NULL)
{
   theRpfHeader = new ossimRpfHeader;
}

ossimNitfEmbeddedRpfHeader::~ossimNitfEmbeddedRpfHeader()
{
   if(theRpfHeader)
   {
      delete theRpfHeader;
      theRpfHeader = NULL;
   }
}

ossimString ossimNitfEmbeddedRpfHeader::getRegisterTagName()const
{
   return ossimString("RPFHDR");
}

const ossimRpfHeader* ossimNitfEmbeddedRpfHeader::getRpfHeader()const
{
   return theRpfHeader;
}

void ossimNitfEmbeddedRpfHeader::parseStream(std::istream& in)
{
   if(in&&theRpfHeader)
   {
      theRpfHeader->parseStream(in);
   }
}

void ossimNitfEmbeddedRpfHeader::writeStream(std::ostream& out)
{
}

std::ostream& ossimNitfEmbeddedRpfHeader::print(std::ostream& out)const
{
   if(theRpfHeader)
   {
      theRpfHeader->print(out);
   }
   return out;
}

ossim_uint32 ossimNitfEmbeddedRpfHeader::getSizeInBytes()const
{
   return 48;
}
