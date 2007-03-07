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
// $Id: ossimImageSourceFactoryRegistry.cpp,v 1.7 2005/10/17 18:42:27 gpotts Exp $
#include <algorithm>
#include "ossimImageSourceFactoryRegistry.h"
#include "ossimImageSourceFactory.h"
#include "ossimImageReconstructionFilterRegistry.h"

ossimImageSourceFactoryRegistry* ossimImageSourceFactoryRegistry::theInstance = NULL;

RTTI_DEF1(ossimImageSourceFactoryRegistry, "ossimImageSourceFactoryRegistry", ossimImageSourceFactoryBase);

ossimImageSourceFactoryRegistry::ossimImageSourceFactoryRegistry()
{
   theInstance = this;
}

ossimImageSourceFactoryRegistry::~ossimImageSourceFactoryRegistry()
{
   theInstance->unregisterFactory(ossimImageSourceFactory::instance());
   
   theInstance = NULL;
}

ossimImageSourceFactoryRegistry* ossimImageSourceFactoryRegistry::instance()
{
  if(theInstance == NULL)
  {
    theInstance = new ossimImageSourceFactoryRegistry;
    theInstance->registerFactory(ossimImageSourceFactory::instance());
    theInstance->registerFactory(ossimImageReconstructionFilterRegistry::instance());
  }
  return theInstance;
}

ossimObject* ossimImageSourceFactoryRegistry::createObject(const ossimString& name)const
{
   ossimObject*                   result = NULL;
   std::vector<ossimImageSourceFactoryBase*>::const_iterator factory;

   factory = theFactoryList.begin();
   while((factory != theFactoryList.end()) && !result)
   {
     result = (*factory)->createObject(name);
     ++factory;
   }
   
   return result;
}

ossimObject* ossimImageSourceFactoryRegistry::createObject(const ossimKeywordlist& kwl,
							   const char* prefix)const
{
   ossimObject*                   result = NULL;
   std::vector<ossimImageSourceFactoryBase*>::const_iterator factory;

   factory = theFactoryList.begin();
   while((factory != theFactoryList.end()) && !result)
   {
     result = (*factory)->createObject(kwl, prefix);
     ++factory;
   }
   
   return result;
 
}

void ossimImageSourceFactoryRegistry::getTypeNameList(std::vector<ossimString>& typeList)const
{
   std::vector<ossimString> result;
   std::vector<ossimImageSourceFactoryBase*>::const_iterator iter = theFactoryList.begin();

   while(iter != theFactoryList.end())
   {
      result.clear();
      (*iter)->getTypeNameList(result);

      // now append to the end of the typeList.
      typeList.insert(typeList.end(),
                      result.begin(),
                      result.end());
      ++iter;
   }
  
}

void ossimImageSourceFactoryRegistry::registerFactory(ossimImageSourceFactoryBase* factory)
{
  if(factory&&!findFactory(factory))
  {
     theFactoryList.push_back(factory);
  }
}

void ossimImageSourceFactoryRegistry::unregisterFactory(ossimImageSourceFactoryBase* factory)
{
   std::vector<ossimImageSourceFactoryBase*>::iterator iter =  std::find(theFactoryList.begin(),
                                                                          theFactoryList.end(),
                                                                          factory);
   if(iter != theFactoryList.end())
   {
      theFactoryList.erase(iter);
   }
}

bool ossimImageSourceFactoryRegistry::findFactory(ossimImageSourceFactoryBase* factory)const
{
   return (std::find(theFactoryList.begin(),
                     theFactoryList.end(),
                     factory)!=theFactoryList.end());
}

void* ossimImageSourceFactoryRegistryGetInstance()
{
  return ossimImageSourceFactoryRegistry::instance();
}

// Hidden
ossimImageSourceFactoryRegistry::ossimImageSourceFactoryRegistry(const ossimImageSourceFactoryRegistry&)
   : ossimImageSourceFactoryBase()
{}

// Hidden
void ossimImageSourceFactoryRegistry::operator=(ossimImageSourceFactoryRegistry&)
{}
