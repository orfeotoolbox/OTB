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
// $Id: ossimNitfRegisteredTag.cpp 14241 2009-04-07 19:59:23Z dburken $

#include <ostream>
#include <string>
#include <ossim/support_data/ossimNitfRegisteredTag.h>
#include <ossim/base/ossimKeywordlist.h>

RTTI_DEF2(ossimNitfRegisteredTag, "ossimNitfRegisteredTag", ossimObject, ossimPropertyInterface)

ossimNitfRegisteredTag::ossimNitfRegisteredTag()
   : ossimObject(),
     ossimPropertyInterface(),
     theRegisteredTagName()
{}

ossimNitfRegisteredTag::~ossimNitfRegisteredTag()
{}

void ossimNitfRegisteredTag::setProperty(ossimRefPtr<ossimProperty> property)
{
}

ossimRefPtr<ossimProperty> ossimNitfRegisteredTag::getProperty(const ossimString& name)const
{
   return 0;
}

void ossimNitfRegisteredTag::getPropertyNames(std::vector<ossimString>& propertyNames)const
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
