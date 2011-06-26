//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimProperty.cpp 18406 2010-11-10 20:45:37Z gpotts $
#include <ossim/base/ossimProperty.h>
#include <ossim/base/ossimContainerProperty.h>
#include <ossim/base/ossimKeywordlist.h>

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

const ossimContainerProperty* ossimProperty::asContainer()const
{
   return 0;
}

ossimContainerProperty* ossimProperty::asContainer()
{
   return 0;
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

ossimRefPtr<ossimXmlNode> ossimProperty::toXml()const
{
   ossimRefPtr<ossimXmlNode> result = new ossimXmlNode();
   
   result->setTag(getName());
   result->setText(valueToString());
   
   return result;
}

void ossimProperty::saveState(ossimKeywordlist& kwl, const ossimString& prefix)const
{
   const ossimContainerProperty* container = asContainer();
   if(container)
   {
      ossim_uint32 nproperties     = container->getNumberOfProperties();
      ossim_uint32 propertiesIndex = 0;
      for(propertiesIndex = 0; propertiesIndex < nproperties; ++propertiesIndex)
      {
         ossimString newPrefix = prefix + container->getName() + ".";
         ossimRefPtr<ossimProperty> prop = container->theChildPropertyList[propertiesIndex];
         if(prop.valid())
         {
            prop->saveState(kwl, newPrefix);
         }
      }
   }
   else 
   {
      kwl.add(prefix + getName(), valueToString(), true);
   }
}

