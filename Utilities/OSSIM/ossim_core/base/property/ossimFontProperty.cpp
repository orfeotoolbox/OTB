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
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimFontProperty.cpp,v 1.4 2004/01/29 13:29:31 gpotts Exp $
#include <sstream>
#include "ossimFontProperty.h"
#include "base/data_types/ossimKeywordlist.h"


RTTI_DEF1(ossimFontProperty, "ossimFontProperty", ossimProperty);

ossimFontProperty::ossimFontProperty(const ossimString& name,
                                     const ossimFontInformation& value)
   :ossimProperty(name),
    theValue(value)
{
   
}

ossimFontProperty::ossimFontProperty(const ossimFontProperty& rhs)
   :ossimProperty(rhs),
    theValue(rhs.theValue)
{
}

ossimFontProperty::~ossimFontProperty()
{
}

ossimObject* ossimFontProperty::dup()const
{
   return new ossimFontProperty(*this);
}

const ossimProperty& ossimFontProperty::assign(const ossimProperty& rhs)
{
   ossimProperty::assign(rhs);

   ossimFontProperty* rhsPtr = PTR_CAST(ossimFontProperty,
                                        &rhs);

   if(rhsPtr)
   {
      theValue = rhsPtr->theValue;
   }
   
   return *this;
}

bool ossimFontProperty::setValue(const ossimString& value)
{
   bool result = true;
   ossimKeywordlist kwl;

   istringstream in(value);

   result = kwl.parseStream(in);
   if(result)
   {
      theValue.loadState(kwl);
   }

   return result;
}

void ossimFontProperty::valueToString(ossimString& valueResult)const
{
   ostringstream out;
   ossimKeywordlist kwl;

   theValue.saveState(kwl);
   
   kwl.writeToStream(out);
   valueResult = kwl.toString();
}

void ossimFontProperty::setFontInformation(ossimFontInformation& fontInfo)
{
   theValue = fontInfo;
}

const ossimFontInformation& ossimFontProperty::getFontInformation()const
{
   return theValue;
}
