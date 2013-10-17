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
// $Id: ossimNitfRegisteredTag.cpp 22013 2012-12-19 17:37:20Z dburken $

#include <iostream>
#include <sstream>
#include <string>
#include <sstream>
#include <ossim/support_data/ossimNitfRegisteredTag.h>

RTTI_DEF2(ossimNitfRegisteredTag, "ossimNitfRegisteredTag", ossimObject, ossimPropertyInterface)

ossimNitfRegisteredTag::ossimNitfRegisteredTag()
   : 
   ossimObject(),
   ossimPropertyInterface(),
   m_tagName(),
   m_tagLength(0)
{}
ossimNitfRegisteredTag::ossimNitfRegisteredTag(const std::string& tagName, ossim_uint32 tagLength)
   : 
   ossimObject(),
   ossimPropertyInterface(),
   m_tagName(tagName),
   m_tagLength(tagLength)
{}

ossimNitfRegisteredTag::~ossimNitfRegisteredTag()
{}

std::string ossimNitfRegisteredTag::getRegisterTagName() const
{
   // Deprecated:
   return m_tagName;
}

const std::string& ossimNitfRegisteredTag::getTagName() const
{
   return m_tagName;
}

void ossimNitfRegisteredTag::setTagName(const std::string& tagName)
{
   m_tagName = tagName;
}

ossim_uint32 ossimNitfRegisteredTag::getSizeInBytes() const
{
   // Deprecated:
   return getTagLength();
}

ossim_uint32 ossimNitfRegisteredTag::getTagLength() const
{
   return m_tagLength;
}

void ossimNitfRegisteredTag::setTagLength(ossim_uint32 length)
{
   m_tagLength = length;
}

void ossimNitfRegisteredTag::setProperty(ossimRefPtr<ossimProperty> /* property */)
{
}

ossimRefPtr<ossimProperty> ossimNitfRegisteredTag::getProperty(const ossimString& /* name */)const
{
   return 0;
}

void ossimNitfRegisteredTag::getPropertyNames(std::vector<ossimString>& /* propertyNames */)const
{
}

void ossimNitfRegisteredTag::getMetadata(ossimKeywordlist& /* kwl */,
                                         const char* /* prefix */ ) const
{
}

std::ostream& ossimNitfRegisteredTag::print(
   std::ostream& out, const std::string& /* prefix */) const
{
   // return print(out);
   return out;
}

bool ossimNitfRegisteredTag::saveState(ossimKeywordlist& kwl, const ossimString& prefix)const
{
   bool result = false;
   // by default we will use the already implemented print method as backup
   //
   std::ostringstream out;
   
   print(out);
   
   ossimKeywordlist kwlTemp;
   
   std::istringstream in(out.str());
   if(kwlTemp.parseStream(in))
   {
      result = true;
      kwl.add(prefix, kwlTemp);
   }
   
   return result;
}
