//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Frank Warmerdam (warmerda@home.com)
//
//*******************************************************************
//  $Id: ossimImageWriterFactoryRegistry.cpp,v 1.11 2005/05/09 12:39:38 dburken Exp $
#include <algorithm>
#include <imaging/factory/ossimImageWriterFactory.h>
#include <imaging/factory/ossimImageWriterFactoryRegistry.h>
#include <imaging/formats/ossimImageFileWriter.h>
#include <base/factory/ossimObjectFactoryRegistry.h>
#include <base/common/ossimKeywordNames.h>

ossimImageWriterFactoryRegistry* ossimImageWriterFactoryRegistry::theInstance = NULL;

ossimImageWriterFactoryRegistry::ossimImageWriterFactoryRegistry()
{
}

ossimImageWriterFactoryRegistry* ossimImageWriterFactoryRegistry::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimImageWriterFactoryRegistry;
      ossimObjectFactoryRegistry::instance()->registerFactory(theInstance);
      ossimImageWriterFactory::instance();
   }

   return theInstance;
}

//*******************************************************************
// Public Method:
//*******************************************************************
bool ossimImageWriterFactoryRegistry::addFactory(ossimImageWriterFactoryBase* factory)
{
   return registerFactory(factory);
}

bool ossimImageWriterFactoryRegistry::registerFactory(ossimImageWriterFactoryBase* factory)
{
   bool result = false;
   if(factory&&!findFactory(factory))
   {
      theFactoryList.push_back(factory);
      result = true;
   }

   return result;
}

void ossimImageWriterFactoryRegistry::unregisterFactory(ossimImageWriterFactoryBase* factory)
{
   std::vector<ossimImageWriterFactoryBase*>::iterator iter =  std::find(theFactoryList.begin(),
                                                                          theFactoryList.end(),
                                                                          factory);
   if(iter != theFactoryList.end())
   {
      theFactoryList.erase(iter);
   }
}

bool ossimImageWriterFactoryRegistry::findFactory(ossimImageWriterFactoryBase* factory)const
{
   return (std::find(theFactoryList.begin(),
                     theFactoryList.end(),
                     factory)!=theFactoryList.end());
}

ossimImageFileWriter *ossimImageWriterFactoryRegistry::createWriterFromExtension(const ossimString& fileExtension)const
{
   vector<ossimImageWriterFactoryBase*>::const_iterator factories;
   ossimImageFileWriter *result = NULL;

   factories = theFactoryList.begin();
   while(factories != theFactoryList.end())
   {
      result = (*factories)->createWriterFromExtension(fileExtension);
      if(result)
      {
         return result;
      }
      ++factories;
   }

   return result;
   
}

ossimImageFileWriter *ossimImageWriterFactoryRegistry::createWriter(const ossimKeywordlist &kwl,
                                                                const char *prefix)const
{
   // let's see if we ned to return an object based on extension.
   // this is specified by the type to be a generic
   // ossimImageFileWriter
   //
   ossimString type = kwl.find(prefix, ossimKeywordNames::TYPE_KW);

   if(type == "ossimImageFileWriter")
   {
      ossimFilename filename = kwl.find(prefix, ossimKeywordNames::FILENAME_KW);

      if((filename != "")&&
         (filename.ext() != ""))
      {
         ossimImageFileWriter* writer = createWriterFromExtension(filename.ext());

         if(writer)
         {
            writer->setFilename(filename);
         }
         return writer;
      }
   }
   
   vector<ossimImageWriterFactoryBase*>::const_iterator factories;
   ossimImageFileWriter *result = NULL;

   factories = theFactoryList.begin();
   while(factories != theFactoryList.end())
   {
      result = (*factories)->createWriter(kwl, prefix);
      if(result)
      {
         return result;
      }
      ++factories;
   }

   return result;
}

ossimImageFileWriter *ossimImageWriterFactoryRegistry::createWriter(const ossimString& typeName)const
{
   vector<ossimImageWriterFactoryBase*>::const_iterator factories;
   ossimImageFileWriter *result = NULL;

   factories = theFactoryList.begin();
   while(factories != theFactoryList.end())
   {
      result = (*factories)->createWriter(typeName);
      if(result)
      {
         return result;
      }
      ++factories;
   }

   return result;
}

ossimObject* ossimImageWriterFactoryRegistry::createObject(const ossimKeywordlist &kwl,
                                                           const char *prefix)const
{
   return createWriter(kwl, prefix);
}

ossimObject* ossimImageWriterFactoryRegistry::createObject(const ossimString& typeName)const
{
   return createWriter(typeName);
}

void ossimImageWriterFactoryRegistry::getTypeNameList(std::vector<ossimString>& typeList)const
{
   vector<ossimString> result;
   vector<ossimImageWriterFactoryBase*>::const_iterator iter = theFactoryList.begin();
   
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

void ossimImageWriterFactoryRegistry::getImageTypeList(std::vector<ossimString>& typeList)const
{
   vector<ossimString> result;
   vector<ossimImageWriterFactoryBase*>::const_iterator iter = theFactoryList.begin();
   
   while(iter != theFactoryList.end())
   {
      result.clear();
      (*iter)->getImageTypeList(result);
      
      // now append to the end of the typeList.
      typeList.insert(typeList.end(),
                      result.begin(),
                      result.end());
      ++iter;
   }  
}

extern "C"
{
  void* ossimImageWriterFactoryRegistryGetInstance()
  {
    return ossimImageWriterFactoryRegistry::instance();
  }
}

ossimImageWriterFactoryRegistry::ossimImageWriterFactoryRegistry(const ossimImageWriterFactoryRegistry&)
   :
      ossimObjectFactory()
{
}

void ossimImageWriterFactoryRegistry::operator=(const ossimImageWriterFactoryRegistry&)
{
}
