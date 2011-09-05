//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimBooleanProperty.cpp 19888 2011-08-03 11:30:22Z gpotts $
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
   valueResult = theValue?"true":"false";
}

bool ossimBooleanProperty::getBoolean()const
{
   return theValue;
}
