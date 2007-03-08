//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Unknown tag class declaration.
//
// Note: By "unknown" this means that the tag name was not found in any of
// the tag factories.
//
//----------------------------------------------------------------------------
// $Id: ossimNitfUnknownTag.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <cstring> /* for memcpy */
#include <cctype>  /* for isascii */
#include <iomanip>

#include <ossim/support_data/ossimNitfUnknownTag.h>
#include <ossim/support_data/ossimNitfCommon.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimDms.h>
#include <ossim/base/ossimDpt.h>

static const ossimTrace traceDebug(ossimString("ossimNitfUnknownTag:debug"));

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

ossimString ossimNitfUnknownTag::getRegisterTagName() const
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

std::ostream& ossimNitfUnknownTag::print(std::ostream& out) const
{
   out << setiosflags(std::ios::left)
       << "ossimNitfUnknownTag::print"
       << std::setw(24) << "\nTag name:" << theTagName
       << std::setw(24) << "\nTag length:" << theTagLength
       << "\nUnformatted tag data:" << std::endl;
   if (tagDataIsAscii())
   {
      out << theTagData << std::endl;
   }

   return out;
}


void ossimNitfUnknownTag::setTagName(const ossimString& tagName)
{
   theTagName = tagName;
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
