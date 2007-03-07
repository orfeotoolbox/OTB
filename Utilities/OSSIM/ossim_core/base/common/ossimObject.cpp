//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimObject.cpp,v 1.18 2005/10/14 13:26:01 dburken Exp $
#include <base/common/ossimObject.h>
#include <base/common/ossimKeywordNames.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/factory/ossimObjectFactoryRegistry.h>
#include <base/context/ossimNotifyContext.h>

RTTI_DEF(ossimObject, "ossimObject")

ossimObject::ossimObject()
{}

ossimObject::~ossimObject()
{
}
   
ossimObject* ossimObject::dup()const
{
   ossimObject* result = NULL;
   ossimKeywordlist kwl;

   saveState(kwl);
   
   result = ossimObjectFactoryRegistry::instance()->createObject(kwl);

   if(!result)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "dup() not implemented for object = "
         << getClassName() << ". Please implement!\n";
   }
   
   return result;
}

ossimString ossimObject::getShortName()const
{
   return getClassName();
}

ossimString ossimObject::getLongName()const
{
   return getClassName();
}

ossimString ossimObject::getDescription()const
{
   return getLongName();
}


ossimString ossimObject::getClassName()const
{
   return TYPE_NAME(this);
}

RTTItypeid ossimObject::getType()const
{
   return TYPE_INFO(this);
}

bool ossimObject::canCastTo(ossimObject* obj)const
{
   if(obj)
   {
      return obj->getType().can_cast(this->getType());
   }
   
   return false;
}

bool ossimObject::canCastTo(const ossimString& parentClassName)const
{
   return (getType().find_baseclass(parentClassName.c_str()) !=
           getType().null_type());
}

bool ossimObject::canCastTo(const RTTItypeid& id)const
{
   return id.can_cast(this->getType());
}

bool ossimObject::saveState(ossimKeywordlist& kwl,
                            const char* prefix)const
{
   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           getClassName(),
           true);

   return true;
}

bool ossimObject::loadState(const ossimKeywordlist&, const char*)
{
   return true;
}

std::ostream& ossimObject::print(std::ostream& out) const
{
   return out;
}

std::ostream& operator<<(std::ostream& out, const ossimObject& obj)
{
   return obj.print(out);
}
