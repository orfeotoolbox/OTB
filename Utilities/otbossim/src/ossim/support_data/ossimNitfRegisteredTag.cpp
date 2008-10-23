//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfRegisteredTag.cpp 11256 2007-06-21 20:53:58Z dburken $
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
