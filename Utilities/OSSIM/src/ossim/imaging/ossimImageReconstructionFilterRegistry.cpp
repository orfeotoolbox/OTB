//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimImageReconstructionFilterRegistry.cpp 9963 2006-11-28 21:11:01Z gpotts $

#include <algorithm>
#include <vector>
#include <ossim/imaging/ossimImageReconstructionFilterRegistry.h>
#include <ossim/imaging/ossimImageReconstructionFilterFactory.h>
#include <ossim/base/ossimString.h>

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
