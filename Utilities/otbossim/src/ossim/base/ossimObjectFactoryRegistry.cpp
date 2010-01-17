//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimObjectFactoryRegistry.cpp 15766 2009-10-20 12:37:09Z gpotts $

#include <ossim/base/ossimObjectFactoryRegistry.h>

RTTI_DEF1(ossimObjectFactoryRegistry, "ossimObjectFactoryRegistry", ossimObject);
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/base/ossimObjectFactory.h>
#include <ossim/base/ossimBaseObjectFactory.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimRefPtr.h>
using namespace std;


ossimObjectFactoryRegistry::ossimObjectFactoryRegistry()
{
}

ossimObjectFactoryRegistry::~ossimObjectFactoryRegistry()
{
}

ossimObjectFactoryRegistry* ossimObjectFactoryRegistry::instance()
{
   static ossimObjectFactoryRegistry sharedInstance;
   
   return &sharedInstance;
}

#if 0
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
#endif

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
      ossimRefPtr<ossimObject> obj;
      for(i = 0; i < (int)allTypeList.size(); ++i)
      {
         obj = createObject(allTypeList[i]);
         if(obj.valid())
         {
            if(obj->canCastTo(baseType))
            {
               typeList.push_back(allTypeList[i]);
            }
         }
      }
   }
}


void* ossimObjectFactoryRegistryGetInstance()
{
  return ossimObjectFactoryRegistry::instance();
}
