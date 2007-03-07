//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: LGPL see LICENSE.txt
//
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfTagFactoryRegistry.cpp,v 1.2 2004/09/28 15:35:30 gpotts Exp $
#include <support_data/nitf/ossimNitfTagFactoryRegistry.h>
#include <support_data/nitf/ossimNitfRpfTagFactory.h>
#include <support_data/nitf/ossimNitfRegisteredTagFactory.h>
#ifndef NULL
#include <stddef.h>
#endif
#include <algorithm>

ossimNitfTagFactoryRegistry* ossimNitfTagFactoryRegistry::theInstance=NULL;

ossimNitfTagFactoryRegistry::ossimNitfTagFactoryRegistry()
{
}

ossimNitfTagFactoryRegistry::~ossimNitfTagFactoryRegistry()
{
   theInstance = NULL;
}


void ossimNitfTagFactoryRegistry::registerFactory(ossimNitfTagFactory* aFactory)
{
   if(aFactory&&!exists(aFactory))
   {
      theFactoryList.push_back(aFactory);
   }
}


ossimNitfTagFactoryRegistry* ossimNitfTagFactoryRegistry::instance()
{
   if(theInstance)
   {
      return theInstance;
   }
   else
   {
      theInstance = new ossimNitfTagFactoryRegistry;
      theInstance->initializeDefaults();
   }
   
   return theInstance;
}

ossimRefPtr<ossimNitfRegisteredTag> ossimNitfTagFactoryRegistry::create(const ossimString &tagName)const
{
   std::vector<ossimNitfTagFactory*>::const_iterator factory;
   ossimRefPtr<ossimNitfRegisteredTag> result = NULL;

   factory = theFactoryList.begin();
   while(factory != theFactoryList.end())
   {
      result = (*factory)->create(tagName);

      if(result.valid())
      {
         return result;
      }
      ++factory;
   }
   
   return NULL;
}

void ossimNitfTagFactoryRegistry::initializeDefaults()
{
   theFactoryList.push_back(ossimNitfRegisteredTagFactory::instance());
   theFactoryList.push_back(ossimNitfRpfTagFactory::instance());
}

bool ossimNitfTagFactoryRegistry::exists(ossimNitfTagFactory* factory)const
{
   std::vector<ossimNitfTagFactory*>::const_iterator iter = std::find(theFactoryList.begin(),
                                                                      theFactoryList.end(),
                                                                      factory);

   return (iter != theFactoryList.end());
   
}
