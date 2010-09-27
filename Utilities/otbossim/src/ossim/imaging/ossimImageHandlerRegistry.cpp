//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts
//
// Description:
//
// Contains class definition for ImageHandlerRegistry.
//
//*******************************************************************
//  $Id: ossimImageHandlerRegistry.cpp 18002 2010-08-30 18:01:10Z gpotts $
#include <algorithm>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageHandlerFactory.h>
#include <ossim/imaging/ossimImageHandlerFactoryBase.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>

using namespace std;

RTTI_DEF1(ossimImageHandlerRegistry, "ossimImageHandlerRegistry", ossimObjectFactory);

//ossimImageHandlerRegistry* ossimImageHandlerRegistry::theInstance = 0;

ossimImageHandlerRegistry::ossimImageHandlerRegistry()
{
   ossimObjectFactoryRegistry::instance()->registerFactory(this);
   registerFactory(ossimImageHandlerFactory::instance());
}

ossimImageHandlerRegistry* ossimImageHandlerRegistry::instance()
{
   static ossimImageHandlerRegistry sharedInstance;
   
   return &sharedInstance;
}

ossimImageHandlerRegistry::~ossimImageHandlerRegistry()
{
   clear();
}

void ossimImageHandlerRegistry::addFactory(
   ossimImageHandlerFactoryBase* factory)
{
   registerFactory(factory);
}

void ossimImageHandlerRegistry::registerFactory(
   ossimImageHandlerFactoryBase* factory, bool pushToFrontFlag)
{
   if( factory && !findFactory(factory) )
   {
      if (pushToFrontFlag)
      {
         theFactoryList.insert(theFactoryList.begin(), factory);
      }
      else
      {
         theFactoryList.push_back(factory);
      }
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

ossimRefPtr<ossimImageHandler> ossimImageHandlerRegistry::openBySuffix(const ossimFilename& file)const
{
   std::vector<ossimRefPtr<ossimImageHandler> > handlers;
   
   getImageHandlersBySuffix(handlers, file.ext());
   ossim_uint32 idx = 0;
   ossim_uint32 size = handlers.size();
   
   for(idx = 0; idx < size; ++idx)
   {
      if(handlers[idx]->open(file))
      {
         return handlers[idx];
      }
   }
   
   return ossimRefPtr<ossimImageHandler>(0);
}

void ossimImageHandlerRegistry::getImageHandlersBySuffix(ossimImageHandlerFactoryBase::ImageHandlerList& result,
                                                         const ossimString& ext)const
{
   vector<ossimImageHandlerFactoryBase*>::const_iterator iter = theFactoryList.begin();
   ossimImageHandlerFactoryBase::ImageHandlerList temp;
   while(iter != theFactoryList.end())
   {
      temp.clear();
      (*iter)->getImageHandlersBySuffix(temp, ext);
      
      if(!temp.empty())
      {
         
         // now append to the end of the typeList.
         result.insert(result.end(),
                       temp.begin(),
                       temp.end());
      }
      ++iter;
   }
}

void ossimImageHandlerRegistry::getImageHandlersByMimeType(ossimImageHandlerFactoryBase::ImageHandlerList& result,
                                                           const ossimString& mimeType)const
{
   vector<ossimImageHandlerFactoryBase*>::const_iterator iter = theFactoryList.begin();
   ossimImageHandlerFactoryBase::ImageHandlerList temp;
   while(iter != theFactoryList.end())
   {
      temp.clear();
      (*iter)->getImageHandlersByMimeType(temp, mimeType);
      
      if(!temp.empty())
      {
         
         // now append to the end of the typeList.
         result.insert(result.end(),
                       temp.begin(),
                       temp.end());
      }
      ++iter;
   }
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

void ossimImageHandlerRegistry::getSupportedExtensions(ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const
{
   vector<ossimString> result;
   vector<ossimImageHandlerFactoryBase*>::const_iterator iter = theFactoryList.begin();

   while(iter != theFactoryList.end())
   {
      (*iter)->getSupportedExtensions(extensionList);

      ++iter;
   }
   
}

ossimImageHandler* ossimImageHandlerRegistry::open(const ossimFilename& fileName, bool trySuffixFirst)const
{
   if(trySuffixFirst)
   {
      ossimRefPtr<ossimImageHandler> h = openBySuffix(fileName);
      if(h.valid())
      {
         return h.release();
      }
   }
   
   // now try magic number opens
   //
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

ossimImageHandlerRegistry::ossimImageHandlerRegistry(const ossimImageHandlerRegistry& /* rhs */)
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
