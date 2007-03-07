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
// Description:
//
//*************************************************************************
// $Id: ossimObjectFactoryRegistry.cpp,v 1.10 2003/03/27 18:31:43 gpotts Exp $

#include "ossimObjectFactoryRegistry.h"

RTTI_DEF1(ossimObjectFactoryRegistry, "ossimObjectFactoryRegistry", ossimObject);
#include "ossimObjectFactoryRegistry.h"
#include "ossimObjectFactory.h"
#include "ossimBaseObjectFactory.h"
using namespace std;

ossimObjectFactoryRegistry* ossimObjectFactoryRegistry::theInstance=NULL;

ossimObjectFactoryRegistry::~ossimObjectFactoryRegistry()
{
   theInstance = NULL;
}

ossimObjectFactoryRegistry* ossimObjectFactoryRegistry::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimObjectFactoryRegistry;
   }
   
   return theInstance;
}

bool ossimObjectFactoryRegistry::addFactory(ossimObjectFactory* factory)
{
   return registerFactory(factory);
}

bool ossimObjectFactoryRegistry::registerFactory(ossimObjectFactory* factory)
{
   if(factory)
   {
      if(!findFactory(factory))
      {
         theFactoryList.push_back(factory);
         return true;
      }
      else
      {
         return true;
      }
   }
   
   return false;
}

void ossimObjectFactoryRegistry::unregisterFactory(ossimObjectFactory* factory)
{
   vector<ossimObjectFactory*>::iterator iter = theFactoryList.begin();

   while(iter != theFactoryList.end())
   {
      if( (*iter) == factory)
      {
         theFactoryList.erase(iter);
         return;
      }
      ++iter;
   }
}

ossimObject* ossimObjectFactoryRegistry::createObject(const ossimString& name)const
{
   ossimObject* result = NULL;
   unsigned long index = 0;
   
   while((index < theFactoryList.size()) &&(!result))
   {
      result = theFactoryList[index]->createObject(name);
      ++index;
   }
   return result;
}

ossimObject* ossimObjectFactoryRegistry::createObject(const ossimKeywordlist& kwl,
                                                      const char* prefix)const
{
   ossimObject* result = NULL;
   unsigned long index = 0;

   while((index < theFactoryList.size()) &&(!result))
   {
      result = theFactoryList[index]->createObject(kwl,
                                                   prefix);
      ++index;
   }

   return result;
}

void ossimObjectFactoryRegistry::getTypeNameList(std::vector<ossimString>& typeList)const
{
   vector<ossimString> result;
   vector<ossimObjectFactory*>::const_iterator iter = theFactoryList.begin();

   while(iter != theFactoryList.end())
   {
      result.clear();
      (*iter)->getTypeNameList(result);

      typeList.insert(typeList.end(),
                      result.begin(),
                      result.end());
      ++iter;
   }
}

ossimObjectFactory* ossimObjectFactoryRegistry::findFactory(ossimObjectFactory* factory)const
{
   vector<ossimObjectFactory*>::const_iterator iter = theFactoryList.begin();

   while(iter != theFactoryList.end())
   {
      if( (*iter) == factory)
      {
         return factory;
      }
      ++iter;
   }

   return NULL;
}

void ossimObjectFactoryRegistry::getTypeNameList(std::vector<ossimString>& typeList,
                                                 const ossimString& baseType)const
{
   vector<ossimString> allTypeList;

   getTypeNameList(allTypeList);

   if(baseType == "")
   {
      typeList.insert(typeList.end(),
                      allTypeList.begin(),
                      allTypeList.end());
   }
   else
   {
      int i = 0;
      ossimObject* obj = (ossimObject*)NULL;
      for(i = 0; i < (int)allTypeList.size(); ++i)
      {
         obj = createObject(allTypeList[i]);
         if(obj)
         {
            if(obj->canCastTo(baseType))
            {
               typeList.push_back(allTypeList[i]);
            }
            delete obj;
            obj = (ossimObject*)NULL;
         }
      }
   }
}


void* ossimObjectFactoryRegistryGetInstance()
{
  return ossimObjectFactoryRegistry::instance();
}
