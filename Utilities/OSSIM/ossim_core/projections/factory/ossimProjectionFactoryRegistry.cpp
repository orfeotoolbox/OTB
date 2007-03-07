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
// $Id: ossimProjectionFactoryRegistry.cpp,v 1.23 2005/11/25 20:31:35 dburken Exp $
#include <algorithm>
#include <projections/factory/ossimProjectionFactoryRegistry.h>
#include <projections/factory/ossimProjectionFactoryBase.h>
#include <projections/factory/ossimSrsProjectionFactory.h>
#include <projections/factory/ossimMapProjectionFactory.h>
#include <projections/factory/ossimTiffProjectionFactory.h>
#include <projections/factory/ossimNitfProjectionFactory.h>
#include <projections/factory/ossimPcsCodeProjectionFactory.h>
#include <projections/factory/ossimStatePlaneProjectionFactory.h>
#include <projections/factory/ossimSensorModelFactory.h>
#include <projections/factory/ossimMiscProjectionFactory.h>
#include <projections/ossimProjection.h>
#include <base/factory/ossimObjectFactoryRegistry.h>

ossimProjectionFactoryRegistry* ossimProjectionFactoryRegistry::theInstance=NULL;

ossimProjectionFactoryRegistry* ossimProjectionFactoryRegistry::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimProjectionFactoryRegistry;
      theInstance->initializeDefaults();
      ossimObjectFactoryRegistry::instance()->registerFactory(theInstance);
   }

   return theInstance;
}

ossimProjection*
ossimProjectionFactoryRegistry::createProjection(const ossimFilename& name,
                                                 ossim_uint32 entryIdx)const
{
   ossimProjection* result = NULL;
   vector<ossimProjectionFactoryBase*>::const_iterator factory = theFactoryList.begin();

   while( (factory != theFactoryList.end()) && !result )
   {
      if(*factory)
      {
         result = (*factory)->createProjection(name, entryIdx);
      }
      
      ++factory;
   }
   
   return result;
}

ossimProjection* ossimProjectionFactoryRegistry::createProjection(const ossimString& name)const
{
   ossimProjection* result = NULL;
   vector<ossimProjectionFactoryBase*>::const_iterator factory = theFactoryList.begin();

   while((factory != theFactoryList.end()&&!
          result))
   {
      if(*factory)
      {
         result = (*factory)->createProjection(name);
      }
      
      ++factory;
   }
   
   return result;
}

ossimProjection* ossimProjectionFactoryRegistry::createProjection(
   const ossimKeywordlist& kwl, const char* prefix)const
{
   ossimProjection* result = NULL;
   vector<ossimProjectionFactoryBase*>::const_iterator factory = theFactoryList.begin();

   while( (factory != theFactoryList.end()) && !result )
   {
      if(*factory)
      {
         result = (*factory)->createProjection(kwl, prefix);
      }
      
      ++factory;
   }
   
   return result;
}

bool ossimProjectionFactoryRegistry::registerFactory(ossimProjectionFactoryBase* factory)
{
   if(factory&&!findFactory(factory))
   {
      theFactoryList.push_back(factory);
      return true;
   }

   return false;
}

void ossimProjectionFactoryRegistry::unregisterFactory(ossimProjectionFactoryBase* factory)
{
   std::vector<ossimProjectionFactoryBase*>::iterator iter =  std::find(theFactoryList.begin(),
                                                                        theFactoryList.end(),
                                                                        factory);
   if(iter != theFactoryList.end())
   {
      theFactoryList.erase(iter);
   }
}

bool ossimProjectionFactoryRegistry::findFactory(ossimProjectionFactoryBase* factory)const
{
   return (std::find(theFactoryList.begin(),
                     theFactoryList.end(),
                     factory)!=theFactoryList.end());
}

std::list<ossimString> ossimProjectionFactoryRegistry::getList()const
{
   list<ossimString> result;
   vector<ossimProjectionFactoryBase*>::const_iterator factory = theFactoryList.begin();

   while(factory != theFactoryList.end())
   {
      if(*factory)
      {
         list<ossimString> temp = (*factory)->getList();
         result.insert(result.end(),
                       temp.begin(),
                       temp.end());
      }
      
      ++factory;
   }
   
   return result;
   
}

ossimObject* ossimProjectionFactoryRegistry::createObject(const ossimString& typeName)const
{
   return createProjection(typeName);
}

ossimObject* ossimProjectionFactoryRegistry::createObject(const ossimKeywordlist& kwl,
                                                          const char* prefix)const
{
   return createProjection(kwl, prefix);
}

void ossimProjectionFactoryRegistry::getTypeNameList(std::vector<ossimString>& typeList)const
{
   vector<ossimString> result;
   vector<ossimProjectionFactoryBase*>::const_iterator factory = theFactoryList.begin();

   while(factory != theFactoryList.end())
   {
      if(*factory)
      {
         result.clear();
         (*factory)->getTypeNameList(result);
         typeList.insert(typeList.end(),
                         result.begin(),
                         result.end());
      }
      
      ++factory;
   }   
}

void ossimProjectionFactoryRegistry::initializeDefaults()
{
   theInstance->registerFactory(ossimSensorModelFactory::instance());
   theInstance->registerFactory(ossimMapProjectionFactory::instance());
   theInstance->registerFactory(ossimSrsProjectionFactory::instance());
   theInstance->registerFactory(ossimTiffProjectionFactory::instance());
   theInstance->registerFactory(ossimPcsCodeProjectionFactory::instance());   
   theInstance->registerFactory(ossimStatePlaneProjectionFactory::instance());
   theInstance->registerFactory(ossimNitfProjectionFactory::instance());   
   theInstance->registerFactory(ossimMiscProjectionFactory::instance());
}

extern "C"
{
  void* ossimProjectionFactoryRegistryGetInstance()
  {
    return ossimProjectionFactoryRegistry::instance();
  }

}
