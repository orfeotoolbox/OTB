//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimObjectFactoryRegistry.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <ossim/base/ossimObjectFactoryRegistry.h>

RTTI_DEF1(ossimObjectFactoryRegistry, "ossimObjectFactoryRegistry", ossimObject);
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/base/ossimObjectFactory.h>
#include <ossim/base/ossimBaseObjectFactory.h>
#include <ossim/base/ossimString.h>
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
