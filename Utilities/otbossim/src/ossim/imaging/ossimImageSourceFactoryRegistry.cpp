//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimImageSourceFactoryRegistry.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <algorithm>
#include <ossim/imaging/ossimImageSourceFactoryRegistry.h>
#include <ossim/imaging/ossimImageSourceFactory.h>
#include <ossim/imaging/ossimImageReconstructionFilterRegistry.h>
#include <ossim/base/ossimString.h>

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
