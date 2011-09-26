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
// $Id: ossimObject.cpp 20070 2011-09-07 18:48:35Z dburken $
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimVisitor.h>

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

bool ossimObject::isEqualTo(const ossimObject& obj, ossimCompareType /* compareType */)const
{
   return (getClassName() == obj.getClassName());
}

void ossimObject::accept(ossimVisitor& visitor)
{
   if(!visitor.stopTraversal()&&!visitor.hasVisited(this))
   {
      visitor.visit(this);
   }
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
