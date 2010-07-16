//----------------------------------------------------------------------------
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//----------------------------------------------------------------------------
// $Id: ossimImageMetaDataWriterRegistry.cpp 17206 2010-04-25 23:20:40Z dburken $


#include <ossim/imaging/ossimImageMetaDataWriterRegistry.h>
#include <ossim/imaging/ossimImageMetaDataWriterFactory.h>
#include <ossim/imaging/ossimMetadataFileWriter.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <algorithm>

//ossimImageMetaDataWriterRegistry*
//ossimImageMetaDataWriterRegistry::theInstance= NULL;

ossimImageMetaDataWriterRegistry::ossimImageMetaDataWriterRegistry()
{
   registerFactory(ossimImageMetaDataWriterFactory::instance());
   ossimObjectFactoryRegistry::instance()->registerFactory(this);
}

ossimImageMetaDataWriterRegistry::~ossimImageMetaDataWriterRegistry()
{
   ossimObjectFactoryRegistry::instance()->unregisterFactory(this);
}

ossimImageMetaDataWriterRegistry* ossimImageMetaDataWriterRegistry::instance()
{
   static ossimImageMetaDataWriterRegistry sharedInstance;

   return &sharedInstance;
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
   std::vector<ossimImageMetaDataWriterFactoryBase*>::const_iterator factory;
   
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
   std::vector<ossimImageMetaDataWriterFactoryBase*>::const_iterator factory;

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
   
   std::vector<ossimImageMetaDataWriterFactoryBase*>::const_iterator factory;
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
   std::vector<ossimString> result;
   std::vector<ossimImageMetaDataWriterFactoryBase*>::const_iterator iter =
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
   
   std::vector<ossimImageMetaDataWriterFactoryBase*>::const_iterator iter =
      theFactoryList.begin();
   
   while(iter != theFactoryList.end())
   {
      (*iter)->getMetadatatypeList(metadatatypeList);
      ++iter;
   } 
}

ossimImageMetaDataWriterRegistry::ossimImageMetaDataWriterRegistry(
   const ossimImageMetaDataWriterRegistry& /* rhs */)
{
}

const ossimImageMetaDataWriterRegistry&
ossimImageMetaDataWriterRegistry::operator=(
   const ossimImageMetaDataWriterRegistry& /* rhs */)
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
