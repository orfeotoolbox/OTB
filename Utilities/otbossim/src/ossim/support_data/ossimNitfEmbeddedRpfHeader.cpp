//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfEmbeddedRpfHeader.cpp 14241 2009-04-07 19:59:23Z dburken $

#include <istream>
#include <ostream>

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

std::ostream& ossimNitfEmbeddedRpfHeader::print(
   std::ostream& out, const std::string& prefix)const
{
   if(theRpfHeader)
   {
      theRpfHeader->print(out, prefix);
   }
   return out;
}

ossim_uint32 ossimNitfEmbeddedRpfHeader::getSizeInBytes()const
{
   return 48;
}
