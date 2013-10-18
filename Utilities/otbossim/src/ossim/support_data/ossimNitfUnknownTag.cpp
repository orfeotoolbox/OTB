//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Unknown tag class declaration.
//
// Note: By "unknown" this means that the tag name was not found in any of
// the tag factories.
//
//----------------------------------------------------------------------------
// $Id: ossimNitfUnknownTag.cpp 19682 2011-05-31 14:21:20Z dburken $

#include <ostream>
#include <iomanip>
#include <cctype> /* for isascii */

#include <ossim/support_data/ossimNitfUnknownTag.h>
#include <ossim/support_data/ossimNitfCommon.h>

RTTI_DEF1(ossimNitfUnknownTag, "ossimNitfUnknownTag", ossimNitfRegisteredTag);

ossimNitfUnknownTag::ossimNitfUnknownTag()
   : theTagName(),
     theTagLength(0),
     theTagData(NULL)
   
{
}

ossimNitfUnknownTag::~ossimNitfUnknownTag()
{
   if (theTagData)
   {
      delete [] theTagData;
      theTagData = NULL;
   }
}

std::string ossimNitfUnknownTag::getRegisterTagName() const
{
   return theTagName;
}

void ossimNitfUnknownTag::parseStream(std::istream& in)
{
   if (theTagLength)
   {
      if (theTagData)
      {
         delete [] theTagData;
      }

      theTagData = new char[theTagLength+1];
      
      in.read(theTagData, theTagLength);

      theTagData[theTagLength] = '\0';
   }
}

void ossimNitfUnknownTag::writeStream(std::ostream& out)
{
   if (theTagLength && theTagData)
   {
      out.write(theTagData, theTagLength);
   }
}

ossim_uint32 ossimNitfUnknownTag::getSizeInBytes()const
{
   return theTagLength;
}

void ossimNitfUnknownTag::clearFields()
{
   if (theTagData)
   {
      delete [] theTagData;
      theTagData = NULL;
   }
}

std::ostream& ossimNitfUnknownTag::print(std::ostream& out,
                                         const std::string& prefix) const
{
   std::string pfx = prefix;
   pfx += getRegisterTagName();
   pfx += ".";

   out << setiosflags(std::ios::left)
       << pfx << std::setw(24) << "CETAG:" << getRegisterTagName() << "\n"
       << pfx << std::setw(24) << "CEL:"   << getSizeInBytes() << "\n"
       << pfx << std::setw(24) << "unformatted_tag_data: ";
   
   if (tagDataIsAscii())
   {
      out << theTagData << "\n";
   }
   else
   {
      out << "binary not displayed\n";
   }
   
   return out;
}

void ossimNitfUnknownTag::setTagName(const ossimString& tagName)
{
   theTagName = tagName.string();
}


void ossimNitfUnknownTag::setTagLength(ossim_uint32 length)
{
   if (theTagData)
   {
      delete [] theTagData;
      theTagData = NULL;
   }
   theTagLength = length;
}

bool ossimNitfUnknownTag::tagDataIsAscii() const
{
   if ( (theTagLength == 0) || !theTagData )
   {
      return false;
   }

   for (ossim_uint32 i = 0; i < theTagLength; ++i)
   {
      int c = theTagData[i];
      if (isascii(c) == false)
      {
         return false;
      }
   }

   return true;
}
