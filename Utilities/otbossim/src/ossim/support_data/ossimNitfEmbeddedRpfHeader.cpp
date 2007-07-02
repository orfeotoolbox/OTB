//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfEmbeddedRpfHeader.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <ossim/support_data/ossimNitfEmbeddedRpfHeader.h>
#include <ossim/support_data/ossimRpfHeader.h>

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
