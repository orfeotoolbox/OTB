//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  David Burken
//
// Description:
//
// Contains class definition for Keyword.h
//*******************************************************************
//  $Id: ossimKeyword.cpp 9963 2006-11-28 21:11:01Z gpotts $

#include <iostream>
#include <ossim/base/ossimKeyword.h>

ossimKeyword::ossimKeyword()
   :
      theKey(""),
      theDesc(""),
      theValue("")
{}

ossimKeyword::ossimKeyword(const char* key, const char* description)
   :
      theKey(key),
      theDesc(description),
      theValue("")
{}

ossimKeyword::ossimKeyword(const char* key,
                           const char* description,
                           const char* value)
   :
      theKey(key),
      theDesc(description),
      theValue(value)      
{}

ossimKeyword::ossimKeyword(const ossimKeyword& src)
   :
      theKey(src.key()),
      theDesc(src.description()),
      theValue(src.value())
{}

ossimKeyword::~ossimKeyword()
{}

ossimKeyword::operator const char*() const
{
   return theKey.c_str();
}

const char* ossimKeyword::key() const
{
   return theKey.c_str();
}

const char* ossimKeyword::description() const
{
   return theDesc.c_str();
}

const char* ossimKeyword::value() const
{
   return theValue.c_str();
}

ossimString ossimKeyword::keyString() const
{
   return theKey;
}

ossimString ossimKeyword::descriptionString() const
{
   return theDesc;
}

ossimString ossimKeyword::valueString() const
{
   return theValue;
}

void ossimKeyword::setValue(const char* val)
{
   theValue = val;
}

void ossimKeyword::setValue(const ossimString& val)
{
   theValue = val;
}

void ossimKeyword::setDescription(const char* desc)
{
   theDesc  = desc;
}

void ossimKeyword::setDescription(const ossimString& desc)
{
   theDesc  = desc;
}
   
const ossimKeyword& ossimKeyword::operator=(const ossimKeyword& kw)
{
   if (this != &kw)
   {
      theKey = kw.key();
      theDesc = kw.description();
      theValue = kw.value();
   }
   
   return *this;
}

std::ostream& operator<<(std::ostream& os, const ossimKeyword& kw)
{
   os << "\n key   = " << kw.key()
      << "\n desc  = " << kw.description()
      << "\n value = " << kw.value()
      << std::endl;
   return os;
}
