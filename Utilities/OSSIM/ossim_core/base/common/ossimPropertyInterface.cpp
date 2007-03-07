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
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimPropertyInterface.cpp,v 1.10 2004/12/21 14:06:21 gpotts Exp $
#include "ossimPropertyInterface.h"
#include <base/property/ossimStringProperty.h>

RTTI_DEF(ossimPropertyInterface, "ossimPropertyInterface");

void ossimPropertyInterface::setProperty(const ossimString& name,
                                         const ossimString& value)
{
   ossimRefPtr<ossimProperty> property = new ossimStringProperty(name, value);

   setProperty(property);
}

void ossimPropertyInterface::setProperty(ossimRefPtr<ossimProperty> property)
{
}

ossimRefPtr<ossimProperty> ossimPropertyInterface::getProperty(const ossimString& name)const
{
   return (ossimProperty*)0;
}

void ossimPropertyInterface::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
}


void ossimPropertyInterface::getPropertyList(std::vector<ossimRefPtr<ossimProperty> >& propertyList)const
{
   std::vector<ossimString> propertyNames;
   int idx = 0;
   getPropertyNames(propertyNames);

   for(idx = 0; idx < (int)propertyNames.size();++idx)
   {
      ossimRefPtr<ossimProperty> prop = getProperty(propertyNames[idx]);
      if(prop.valid())
      {
         propertyList.push_back(prop);  
      }
   }
}

void ossimPropertyInterface::setProperties(std::vector<ossimRefPtr<ossimProperty> >& propertyList)
{
   ossim_uint32 idx = 0;

   for(idx = 0; idx < propertyList.size(); ++idx)
   {
      if(propertyList[idx].valid())
      {
         setProperty(propertyList[idx]);
      }
   }
}
