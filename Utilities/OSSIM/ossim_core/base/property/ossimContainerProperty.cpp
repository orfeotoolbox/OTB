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
// $Id: ossimContainerProperty.cpp,v 1.6 2004/09/23 17:18:27 gpotts Exp $
#include "ossimContainerProperty.h"

RTTI_DEF1(ossimContainerProperty, "ossimContainerProperty", ossimProperty);

ossimContainerProperty::ossimContainerProperty(const ossimString& name)
   :ossimProperty(name)
{
}

ossimContainerProperty::ossimContainerProperty(const ossimContainerProperty& rhs)
   :ossimProperty(rhs)
{
   copyList(rhs);
}

ossimContainerProperty::~ossimContainerProperty()
{
   deleteChildren();
}

ossimObject* ossimContainerProperty::dup()const
{
   return new ossimContainerProperty(*this);
}

const ossimProperty& ossimContainerProperty::assign(const ossimProperty& rhs)
{
   ossimContainerProperty* rhsContainer = PTR_CAST(ossimContainerProperty,
                                                   &rhs);
   ossimProperty::assign(rhs);

   if(rhsContainer)
   {
      copyList(*rhsContainer);
   }

   return *this;
}

void ossimContainerProperty::copyList(const ossimContainerProperty& rhs)
{
   deleteChildren();

   for(ossim_uint32 idx = 0; idx < rhs.theChildPropertyList.size();++idx)
   {
      if(rhs.theChildPropertyList[idx].valid())
      {
         theChildPropertyList.push_back((ossimProperty*)(rhs.theChildPropertyList[idx]->dup()));
      }
      else
      {
         theChildPropertyList.push_back((ossimProperty*)NULL);
      }
   }
}

void ossimContainerProperty::addChildren(std::vector<ossimRefPtr<ossimProperty> >& propertyList)
{
   for(ossim_uint32 idx = 0; idx < propertyList.size(); ++idx)
   {
      if(propertyList[idx].valid())
      {
         theChildPropertyList.push_back(propertyList[idx]);
      }
   }
}

void ossimContainerProperty::addChild(ossimProperty* property)
{
   theChildPropertyList.push_back(property);
}

ossimRefPtr<ossimProperty> ossimContainerProperty::getProperty(const ossimString& name,
                                                   bool recurse)
{
   ossim_uint32 idx = 0;
   std::vector<ossimRefPtr<ossimContainerProperty> > containers;
   
   for(idx = 0; idx < theChildPropertyList.size(); ++idx)
   {
      if(theChildPropertyList[idx].valid())
      {
         if(theChildPropertyList[idx]->getName() == name)
         {
            return theChildPropertyList[idx];
         }
         if(PTR_CAST(ossimContainerProperty, theChildPropertyList[idx].get())&&recurse)
         {
            containers.push_back(PTR_CAST(ossimContainerProperty, theChildPropertyList[idx].get()));
         }
      }
   }

   if(containers.size())
   {
      for(idx = 0; idx < containers.size();++idx)
      {
         ossimRefPtr<ossimProperty> prop = containers[idx]->getProperty(name, recurse);
         if(prop.valid())
         {
            return prop;
         }
      }
   }
   
   return (ossimProperty*)NULL;
}

void ossimContainerProperty::deleteChildren()
{
//    for(ossim_uint32 idx = 0; idx < theChildPropertyList.size(); ++idx)
//    {
//       if(theChildPropertyList[idx])
//       {
//          delete theChildPropertyList[idx];
//          theChildPropertyList[idx] = (ossimProperty*)NULL;
//       }
//    }
   theChildPropertyList.clear();
}


bool ossimContainerProperty::setValue(const ossimString& value)
{
   return false;
}

void ossimContainerProperty::valueToString(ossimString& valueResult)const
{
}

ossim_uint32 ossimContainerProperty::getNumberOfProperties()const
{
   return theChildPropertyList.size();
}

ossimRefPtr<ossimProperty> ossimContainerProperty::getProperty(ossim_uint32 idx)
{
   if(idx < theChildPropertyList.size())
   {
      return theChildPropertyList[idx];
   }

   return (ossimProperty*)NULL;
}

void ossimContainerProperty::getPropertyList(std::vector<ossimRefPtr<ossimProperty> >& children)
{
   children = theChildPropertyList;
}
