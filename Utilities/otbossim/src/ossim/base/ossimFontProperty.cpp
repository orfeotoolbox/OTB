//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimFontProperty.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <sstream>
#include <ossim/base/ossimFontProperty.h>
#include <ossim/base/ossimKeywordlist.h>


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

   std::istringstream in(value);

   result = kwl.parseStream(in);
   if(result)
   {
      theValue.loadState(kwl);
   }

   return result;
}

void ossimFontProperty::valueToString(ossimString& valueResult)const
{
   std::ostringstream out;
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
