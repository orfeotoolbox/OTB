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
// $Id: ossimNitfUnknownTag.cpp 22013 2012-12-19 17:37:20Z dburken $

#include <ostream>
#include <iomanip>
#include <cctype> /* for isascii */

#include <ossim/support_data/ossimNitfUnknownTag.h>
#include <ossim/support_data/ossimNitfCommon.h>

RTTI_DEF1(ossimNitfUnknownTag, "ossimNitfUnknownTag", ossimNitfRegisteredTag);

ossimNitfUnknownTag::ossimNitfUnknownTag()
   : m_tagData(0)
{
}

ossimNitfUnknownTag::~ossimNitfUnknownTag()
{
   if (m_tagData)
   {
      delete [] m_tagData;
      m_tagData = 0;
   }
}

void ossimNitfUnknownTag::parseStream(std::istream& in)
{
   if (m_tagLength)
   {
      if (m_tagData)
      {
         delete [] m_tagData;
      }

      m_tagData = new char[m_tagLength+1];
      
      in.read(m_tagData, m_tagLength);

      m_tagData[m_tagLength] = '\0';
   }
}

void ossimNitfUnknownTag::writeStream(std::ostream& out)
{
   if (m_tagLength && m_tagData)
   {
      out.write(m_tagData, m_tagLength);
   }
}

void ossimNitfUnknownTag::clearFields()
{
   if (m_tagData)
   {
      delete [] m_tagData;
      m_tagData = 0;
   }
}

std::ostream& ossimNitfUnknownTag::print(std::ostream& out,
                                         const std::string& prefix) const
{
   std::string pfx = prefix;
   pfx += getTagName();
   pfx += ".";

   out << setiosflags(std::ios::left)
       << pfx << std::setw(24) << "CETAG:" << getTagName() << "\n"
       << pfx << std::setw(24) << "CEL:"   << getTagLength() << "\n"
       << pfx << std::setw(24) << "unformatted_tag_data: ";
   
   if (tagDataIsAscii())
   {
      out << m_tagData << "\n";
   }
   else
   {
      out << "binary not displayed\n";
   }
   
   return out;
}

void ossimNitfUnknownTag::setTagLength(ossim_uint32 length)
{
   if (m_tagData)
   {
      delete [] m_tagData;
      m_tagData = 0;
   }
   m_tagLength = length;
}

bool ossimNitfUnknownTag::tagDataIsAscii() const
{
   if ( (m_tagLength == 0) || !m_tagData )
   {
      return false;
   }

   for (ossim_uint32 i = 0; i < m_tagLength; ++i)
   {
      int c = m_tagData[i];
      if (isascii(c) == false)
      {
         return false;
      }
   }

   return true;
}
