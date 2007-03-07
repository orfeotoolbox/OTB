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
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// Contains class definition for ImageHandlerRegistry.
//
//*******************************************************************
//  $Id: ossimImageHandlerRegistry.cpp,v 1.15 2004/01/13 19:24:31 gpotts Exp $
#include <algorithm>
#include "ossimImageHandlerRegistry.h"
#include "ossimImageHandlerFactory.h"
#include "ossimImageHandlerFactoryBase.h"
#include "base/data_types/ossimFilename.h"
#include "imaging/formats/ossimImageHandler.h"
#include "base/factory/ossimObjectFactoryRegistry.h"

using namespace std;

RTTI_DEF1(ossimImageHandlerRegistry, "ossimImageHandlerRegistry", ossimObjectFactory);

ossimImageHandlerRegistry* ossimImageHandlerRegistry::theInstance = 0;

ossimImageHandlerRegistry::ossimImageHandlerRegistry()
{  
}

ossimImageHandlerRegistry* ossimImageHandlerRegistry::instance()
{
   if (theInstance == 0)
   {
      theInstance = new ossimImageHandlerRegistry();
      ossimObjectFactoryRegistry::instance()->registerFactory(theInstance);
      theInstance->registerFactory(ossimImageHandlerFactory::instance());
   }
   return theInstance;
}

ossimImageHandlerRegistry::~ossimImageHandlerRegistry()
{
   clear();
   theInstance = 0;
}

void ossimImageHandlerRegistry::registerFactory(ossimImageHandlerFactoryBase* factory)
{
   if(factory&&!findFactory(factory))
   {
      theFactoryList.push_back(factory);
   }
}

void ossimImageHandlerRegistry::unregisterFactory(ossimImageHandlerFactoryBase* factory)
{
   std::vector<ossimImageHandlerFactoryBase*>::iterator iter =  std::find(theFactoryList.begin(),
                                                                          theFactoryList.end(),
                                                                          factory);
   if(iter != theFactoryList.end())
   {
      theFactoryList.erase(iter);
   }
}

bool ossimImageHandlerRegistry::findFactory(ossimImageHandlerFactoryBase* factory)const
{
   return (std::find(theFactoryList.begin(),
                     theFactoryList.end(),
                     factory)!=theFactoryList.end());
}


ossimObject* ossimImageHandlerRegistry::createObject(const ossimString& typeName)const
{
   ossimObject*                   result = NULL;
   vector<ossimImageHandlerFactoryBase*>::const_iterator factory;

   factory = theFactoryList.begin();
   while((factory != theFactoryList.end()) && !result)
   {
     result = (*factory)->createObject(typeName);
     ++factory;
   }
   
   return result;
 
}

ossimObject* ossimImageHandlerRegistry::createObject(const ossimKeywordlist& kwl,
                                                     const char* prefix)const
{
   ossimObject* result = NULL;
   vector<ossimImageHandlerFactoryBase*>::const_iterator factory;

   factory = theFactoryList.begin();
   while((factory != theFactoryList.end()) && !result)
   {
     result = (*factory)->createObject(kwl, prefix);
     ++factory;
   }
   if(!result)
   {
      result = open(kwl, prefix);
   }
   return result;
}

void ossimImageHandlerRegistry::getTypeNameList(std::vector<ossimString>& typeList)const
{
   vector<ossimString> result;
   vector<ossimImageHandlerFactoryBase*>::const_iterator iter = theFactoryList.begin();

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
 
ossimImageHandler* ossimImageHandlerRegistry::open(const ossimFilename& fileName)const
{
   ossimImageHandler*                   result = NULL;
   vector<ossimImageHandlerFactoryBase*>::const_iterator factory;

   factory = theFactoryList.begin();
   while((factory != theFactoryList.end()) && !result)
   {
      
      result = (*factory)->open(fileName);
      ++factory;
   }
   
   return result;
}

ossimImageHandler* ossimImageHandlerRegistry::open(const ossimKeywordlist& kwl,
                                                   const char* prefix)const
{
   ossimImageHandler*                   result = NULL;
   vector<ossimImageHandlerFactoryBase*>::const_iterator factory;

   factory = theFactoryList.begin();
   while((factory != theFactoryList.end()) && !result)
   {
     result = (*factory)->open(kwl, prefix);
     ++factory;
   }
   
   return result;
}

void ossimImageHandlerRegistry::clear()
{
   theFactoryList.clear();
}

ossimImageHandlerRegistry::ossimImageHandlerRegistry(const ossimImageHandlerRegistry& rhs)
   :  ossimObjectFactory()
{}

const ossimImageHandlerRegistry&
ossimImageHandlerRegistry::operator=(const ossimImageHandlerRegistry& rhs)
{ return rhs; }

extern "C"
{
  void* ossimImageHandlerRegistryGetInstance()
  {
    return ossimImageHandlerRegistry::instance();
  }
}
