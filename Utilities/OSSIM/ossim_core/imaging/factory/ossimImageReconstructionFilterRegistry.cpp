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
// $Id: ossimImageReconstructionFilterRegistry.cpp,v 1.3 2005/10/17 18:42:27 gpotts Exp $
#include <algorithm>
#include "ossimImageReconstructionFilterRegistry.h"
#include "ossimImageReconstructionFilterFactory.h"

ossimImageReconstructionFilterRegistry* ossimImageReconstructionFilterRegistry::theInstance = 0;

RTTI_DEF1(ossimImageReconstructionFilterRegistry, "ossimImageReconstructionFilterRegistry", ossimImageSourceFactoryBase);

ossimImageReconstructionFilterRegistry::ossimImageReconstructionFilterRegistry()
{
   theInstance = this;
}

ossimImageReconstructionFilterRegistry::~ossimImageReconstructionFilterRegistry()
{
   theInstance = NULL;
}

ossimImageReconstructionFilterRegistry* ossimImageReconstructionFilterRegistry::instance()
{
  if(theInstance == NULL)
  {
     theInstance = new ossimImageReconstructionFilterRegistry;
     theInstance->registerFactory(ossimImageReconstructionFilterFactory::instance());
  }
  return theInstance;
}

ossimObject* ossimImageReconstructionFilterRegistry::createObject(const ossimString& name)const
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

ossimObject* ossimImageReconstructionFilterRegistry::createObject(const ossimKeywordlist& kwl,
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

void ossimImageReconstructionFilterRegistry::getTypeNameList(std::vector<ossimString>& typeList)const
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

void ossimImageReconstructionFilterRegistry::registerFactory(ossimImageSourceFactoryBase* factory)
{
  if(factory&&!findFactory(factory))
  {
     theFactoryList.push_back(factory);
  }
}

void ossimImageReconstructionFilterRegistry::unregisterFactory(ossimImageSourceFactoryBase* factory)
{
   std::vector<ossimImageSourceFactoryBase*>::iterator iter =  std::find(theFactoryList.begin(),
                                                                          theFactoryList.end(),
                                                                          factory);
   if(iter != theFactoryList.end())
   {
      theFactoryList.erase(iter);
   }
}

bool ossimImageReconstructionFilterRegistry::findFactory(ossimImageSourceFactoryBase* factory)const
{
   return (std::find(theFactoryList.begin(),
                     theFactoryList.end(),
                     factory)!=theFactoryList.end());
}
