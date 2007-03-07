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
// $Id: ossimBooleanProperty.cpp,v 1.6 2004/01/29 13:29:31 gpotts Exp $
#include "ossimBooleanProperty.h"

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
					   ossimString& msg)
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
