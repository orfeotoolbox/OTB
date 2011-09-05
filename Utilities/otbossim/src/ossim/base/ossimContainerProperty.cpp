//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimContainerProperty.cpp 19920 2011-08-09 12:04:27Z gpotts $
#include <ossim/base/ossimContainerProperty.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimVisitor.h>

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
         theChildPropertyList.push_back((ossimProperty*)0);
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

void ossimContainerProperty::addStringProperty(const ossimString& name,
                                               const ossimString& value,
                                               bool readOnlyFlag)
{
   ossimProperty* prop = new ossimStringProperty(name, value);
   
   prop->setReadOnlyFlag(readOnlyFlag);
   addChild(prop);
}

const ossimContainerProperty* ossimContainerProperty::asContainer()const
{
   return this;
}

ossimContainerProperty* ossimContainerProperty::asContainer()
{
   return this;
}

ossimRefPtr<ossimProperty> ossimContainerProperty::getProperty(const ossimString& name,
                                                               bool recurse)
{
   ossim_uint32 idx = 0;
   std::vector<ossimRefPtr<ossimContainerProperty> > containers;
   if(name == getName())
   {
      return this;
   }
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
   
   return (ossimProperty*)0;
}

void ossimContainerProperty::deleteChildren()
{
//    for(ossim_uint32 idx = 0; idx < theChildPropertyList.size(); ++idx)
//    {
//       if(theChildPropertyList[idx])
//       {
//          delete theChildPropertyList[idx];
//          theChildPropertyList[idx] = (ossimProperty*)0;
//       }
//    }
   theChildPropertyList.clear();
}


bool ossimContainerProperty::setValue(const ossimString& /* value */ )
{
   return false;
}

void ossimContainerProperty::valueToString(ossimString& /* valueResult */ )const
{
}

ossim_uint32 ossimContainerProperty::getNumberOfProperties()const
{
   return (ossim_uint32)theChildPropertyList.size();
}

ossimRefPtr<ossimProperty> ossimContainerProperty::getProperty(ossim_uint32 idx)
{
   if(idx < theChildPropertyList.size())
   {
      return theChildPropertyList[idx];
   }

   return (ossimProperty*)0;
}

void ossimContainerProperty::getPropertyList(
   std::vector<ossimRefPtr<ossimProperty> >& children) const
{
   children = theChildPropertyList;
}

ossimRefPtr<ossimXmlNode> ossimContainerProperty::toXml()const
{
   ossimXmlNode* result = new ossimXmlNode;

   result->setTag(getName());
   
   ossim_uint32 idx = 0;
   for(idx = 0; idx < theChildPropertyList.size(); ++idx)
   {
      ossimRefPtr<ossimXmlNode> child = theChildPropertyList[idx]->toXml();

      result->addChildNode(child.get());
   }

   return result;
}

void ossimContainerProperty::setReadOnlyFlag(bool flag)
{
   ossimProperty::setReadOnlyFlag(flag);
   ossim_uint32 idx = 0;
   for(idx = 0; idx < theChildPropertyList.size(); ++idx)
   {
      theChildPropertyList[idx]->setReadOnlyFlag(flag);
   }
}

void ossimContainerProperty::accept(ossimVisitor& visitor)
{
   ossim_uint32 idx = 0;
   if(!visitor.hasVisited(this))
   {
      ossimProperty::accept(visitor);
      if(visitor.getVisitorType()&ossimVisitor::VISIT_CHILDREN)
      {
         for(idx = 0; idx < theChildPropertyList.size(); ++idx)
         {
            theChildPropertyList[idx]->accept(visitor);
         }
      }
   }
}

