//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author:  David Burken
//
// Description:
//
// Contains class definition for Keyword.h
//*******************************************************************
//  $Id: ossimKeyword.cpp,v 1.3 2005/10/17 18:37:16 gpotts Exp $

#include <base/data_types/ossimKeyword.h>

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
