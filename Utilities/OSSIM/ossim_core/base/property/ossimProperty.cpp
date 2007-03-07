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
// $Id: ossimProperty.cpp,v 1.14 2005/06/09 11:25:35 gpotts Exp $
#include "ossimProperty.h"


RTTI_DEF1(ossimProperty, "ossimProperty", ossimObject);

const ossimProperty& ossimProperty::operator = (const ossimProperty& rhs)
{
   this->assign(rhs);
   
   return *this;
}

ossimProperty::ossimProperty(const ossimString& name)
   :theName(name),
    theReadOnlyFlag(false),
    theChangeType(ossimPropertyChangeType_NOTSPECIFIED),
    theModifiedFlag(false)
{
}

ossimProperty::ossimProperty(const ossimProperty& rhs)
   :theName(rhs.theName),
    theReadOnlyFlag(rhs.theReadOnlyFlag),
    theChangeType(rhs.theChangeType),
    theModifiedFlag(rhs.theModifiedFlag)
{
}

ossimProperty::~ossimProperty()
{
}

const ossimString& ossimProperty::getName()const
{
   return theName;
}

void ossimProperty::setName(const ossimString& name)
{
   theName = name;
}

const ossimProperty& ossimProperty::assign(const ossimProperty& rhs)
{
   theName         = rhs.theName;
   theReadOnlyFlag = rhs.theReadOnlyFlag;
   theChangeType   = rhs.theChangeType;
   theModifiedFlag = rhs.theModifiedFlag;
   
   return *this;
}

ossimString ossimProperty::valueToString()const
{
   ossimString result;

   valueToString(result);
   
   return result;
   
}

void ossimProperty::clearChangeType()
{
   theChangeType = ossimPropertyChangeType_NOTSPECIFIED;
}

void ossimProperty::setChangeType(ossimPropertyChangeType type)
{
   theChangeType = type;
}

void ossimProperty::setFullRefreshBit()
{
   theChangeType = (ossimPropertyChangeType)(theChangeType | ossimPropertyChangeType_FULL_REFRESH);
}

void ossimProperty::setCacheRefreshBit()
{
   theChangeType = (ossimPropertyChangeType)(theChangeType | ossimPropertyChangeType_CACHE_REFRESH);
}

ossimProperty::ossimPropertyChangeType ossimProperty::getChangeType()const
{
   return theChangeType;
}

bool ossimProperty::isFullRefresh()const
{
   return ((theChangeType & ossimPropertyChangeType_FULL_REFRESH) != 0);
}

bool ossimProperty::isCacheRefresh()const
{
   return ((theChangeType & ossimPropertyChangeType_CACHE_REFRESH) != 0);
}

bool ossimProperty::isChangeTypeSpecified()const
{
   return (theChangeType != ossimPropertyChangeType_NOTSPECIFIED);
}

void ossimProperty::setReadOnlyFlag(bool flag)
{
   theReadOnlyFlag = flag;
}

bool ossimProperty::getReadOnlyFlag()const
{
   return theReadOnlyFlag;
}

bool ossimProperty::isReadOnly()const
{
   return (getReadOnlyFlag());
}

void ossimProperty::setModifiedFlag(bool flag)
{
   theModifiedFlag = flag;
}

bool ossimProperty::getModifiedFlag()const
{
   return theModifiedFlag;
}

bool ossimProperty::isModified()const
{
   return (theModifiedFlag == true);
}

void ossimProperty::clearModifiedFlag()
{
   theModifiedFlag = false;
}

void ossimProperty::setDescription(const ossimString& description)
{
   theDescription = description;
}

ossimString ossimProperty::getDescription()const
{
   return theDescription;
}
