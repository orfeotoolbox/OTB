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
// $Id: ossimTextProperty.cpp,v 1.6 2004/01/29 13:29:31 gpotts Exp $
#include "ossimTextProperty.h"

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
   
