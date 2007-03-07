//----------------------------------------------------------------------------
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//----------------------------------------------------------------------------
// $Id: ossimImageMetaDataWriterRegistry.cpp,v 1.10 2005/08/08 22:01:32 dburken Exp $


#include <imaging/factory/ossimImageMetaDataWriterRegistry.h>
#include <imaging/factory/ossimImageMetaDataWriterFactory.h>
#include <imaging/metadata/ossimMetadataFileWriter.h>
#include <base/factory/ossimObjectFactoryRegistry.h>
#include <algorithm>

ossimImageMetaDataWriterRegistry*
ossimImageMetaDataWriterRegistry::theInstance= NULL;

ossimImageMetaDataWriterRegistry::ossimImageMetaDataWriterRegistry()
{
   theInstance = this;
}

ossimImageMetaDataWriterRegistry::~ossimImageMetaDataWriterRegistry()
{
   ossimObjectFactoryRegistry::instance()->unregisterFactory(theInstance);
   theInstance = NULL;
}

ossimImageMetaDataWriterRegistry* ossimImageMetaDataWriterRegistry::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimImageMetaDataWriterRegistry;
      theInstance->registerFactory(ossimImageMetaDataWriterFactory::instance());
      ossimObjectFactoryRegistry::instance()->registerFactory(theInstance);
   }

   return theInstance;
}

void ossimImageMetaDataWriterRegistry::registerFactory(
   ossimImageMetaDataWriterFactoryBase* factory)
{
   if(factory&&!findFactory(factory))
   {
      theFactoryList.push_back(factory);
   }
}

void ossimImageMetaDataWriterRegistry::unregisterFactory(
   ossimImageMetaDataWriterFactoryBase* factory)
{
   std::vector<ossimImageMetaDataWriterFactoryBase*>::iterator iter =
      std::find(theFactoryList.begin(),
                theFactoryList.end(),
                factory);
   if(iter != theFactoryList.end())
   {
      theFactoryList.erase(iter);
   }
}

bool ossimImageMetaDataWriterRegistry::findFactory(
   ossimImageMetaDataWriterFactoryBase* factory)const
{
   return (std::find(theFactoryList.begin(),
                     theFactoryList.end(),
                     factory)!=theFactoryList.end());
}


ossimObject* ossimImageMetaDataWriterRegistry::createObject(
   const ossimString& typeName) const
{
   ossimObject* result = NULL;
   vector<ossimImageMetaDataWriterFactoryBase*>::const_iterator factory;
   
   factory = theFactoryList.begin();
   while((factory != theFactoryList.end()) && !result)
   {
      result = (*factory)->createObject(typeName);
      ++factory;
   }
   
   return result;
}

ossimObject* ossimImageMetaDataWriterRegistry::createObject(
   const ossimKeywordlist& kwl,
   const char* prefix) const
{
   ossimObject* result = NULL;
   vector<ossimImageMetaDataWriterFactoryBase*>::const_iterator factory;

   factory = theFactoryList.begin();
   while((factory != theFactoryList.end()) && !result)
   {
     result = (*factory)->createObject(kwl, prefix);
     ++factory;
   }
   return result;
}

ossimRefPtr<ossimMetadataFileWriter>
ossimImageMetaDataWriterRegistry::createWriter(const ossimString& type) const
{
   ossimRefPtr<ossimMetadataFileWriter> result = NULL;
   
   vector<ossimImageMetaDataWriterFactoryBase*>::const_iterator factory;
   factory = theFactoryList.begin();
   while((factory != theFactoryList.end()) && !result)
   {
     result = (*factory)->createWriter(type);
     ++factory;
   }
   return result;
}

void ossimImageMetaDataWriterRegistry::getTypeNameList(
   std::vector<ossimString>& typeList) const
{
   vector<ossimString> result;
   vector<ossimImageMetaDataWriterFactoryBase*>::const_iterator iter =
      theFactoryList.begin();

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

void ossimImageMetaDataWriterRegistry::getMetadatatypeList(
   std::vector<ossimString>& metadatatypeList) const
{
   //---
   // Since this is the master registry for all meta data writer factories
   // we will start with a cleared list.
   //---
   metadatatypeList.clear();
   
   vector<ossimImageMetaDataWriterFactoryBase*>::const_iterator iter =
      theFactoryList.begin();
   
   while(iter != theFactoryList.end())
   {
      (*iter)->getMetadatatypeList(metadatatypeList);
      ++iter;
   } 
}

ossimImageMetaDataWriterRegistry::ossimImageMetaDataWriterRegistry(
   const ossimImageMetaDataWriterRegistry& rhs)
{
}

const ossimImageMetaDataWriterRegistry&
ossimImageMetaDataWriterRegistry::operator=(
   const ossimImageMetaDataWriterRegistry &rhs)
{
   return *this;
}

extern "C"
{
   void* ossimImageMetaDataWriterRegistryGetInstance()
   {
      return ossimImageMetaDataWriterRegistry::instance();
   }
}
