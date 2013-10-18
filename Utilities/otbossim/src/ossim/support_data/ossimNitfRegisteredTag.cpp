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
// $Id: ossimNitfRegisteredTag.cpp 18413 2010-11-11 19:56:22Z gpotts $

#include <iostream>
#include <sstream>
#include <string>
#include <sstream>
#include <ossim/support_data/ossimNitfRegisteredTag.h>

RTTI_DEF2(ossimNitfRegisteredTag, "ossimNitfRegisteredTag", ossimObject, ossimPropertyInterface)

ossimNitfRegisteredTag::ossimNitfRegisteredTag()
   : ossimObject(),
     ossimPropertyInterface(),
     theRegisteredTagName()
{}

ossimNitfRegisteredTag::~ossimNitfRegisteredTag()
{}

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

