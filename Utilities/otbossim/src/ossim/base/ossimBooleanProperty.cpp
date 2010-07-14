//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimBooleanProperty.cpp 17195 2010-04-23 17:32:18Z dburken $
#include <ossim/base/ossimBooleanProperty.h>

RTTI_DEF1(ossimBooleanProperty, "ossimBooleanProperty", ossimProperty);

ossimBooleanProperty::ossimBooleanProperty(const ossimString& name,
                                           bool value)
   :ossimProperty(name),
    theValue(value)
{
}

ossimBooleanProperty::ossimBooleanProperty(const ossimBooleanProperty& rhs)
   :ossimProperty(rhs),
    theValue(rhs.theValue)
{
}

ossimObject* ossimBooleanProperty::dup()const
{
   return new ossimBooleanProperty(*this);
}

const ossimProperty& ossimBooleanProperty::assign(const ossimProperty& rhs)
{
   ossimProperty::assign(rhs);

   ossimBooleanProperty* property = PTR_CAST(ossimBooleanProperty,
                                             &rhs);
   if(property)
   {
      theValue = property->theValue;
   }
   
   return *this;
}

bool ossimBooleanProperty::setValue(const ossimString& value)
{
   theValue = value.toBool();
   
   return true;
}

bool ossimBooleanProperty::setBooleanValue(bool value,
					   ossimString& /* msg */)
{
   theValue = value;

   return true;
}

void ossimBooleanProperty::valueToString(ossimString& valueResult)const
{
   valueResult = theValue?"1":"0";
}

bool ossimBooleanProperty::getBoolean()const
{
   return theValue;
}
