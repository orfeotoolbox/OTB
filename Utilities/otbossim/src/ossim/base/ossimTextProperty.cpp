//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimTextProperty.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/base/ossimTextProperty.h>

RTTI_DEF1(ossimTextProperty, "ossimTextProperty", ossimProperty);

ossimTextProperty::ossimTextProperty(const ossimString& name,
                                     const ossimString& value,
                                     bool multiLineFlag)
   :ossimProperty(name),
    theValue(value),
    theMultiLineFlag(multiLineFlag)
{
}

ossimTextProperty::ossimTextProperty(const ossimTextProperty& rhs)
   :ossimProperty(rhs),
    theValue(rhs.theValue),
    theMultiLineFlag(rhs.theMultiLineFlag)
{
}

ossimObject* ossimTextProperty::dup()const
{
   return new ossimTextProperty(*this);
}

bool ossimTextProperty::isMulitLine()const
{
   return theMultiLineFlag;
}

void ossimTextProperty::setMultiLineFlag(bool flag)
{
   theMultiLineFlag = flag;
}

bool ossimTextProperty::setValue(const ossimString& value)
{
   theValue = value;
   
   return true;
}

void ossimTextProperty::valueToString(ossimString& valueResult)const
{
   valueResult = theValue;
}

const ossimProperty& ossimTextProperty::assign(const ossimProperty& rhs)
{
   theValue = rhs.valueToString();

   return *this;
}
   
