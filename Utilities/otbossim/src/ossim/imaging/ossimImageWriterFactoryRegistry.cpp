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
//  $Id: ossimImageWriterFactoryRegistry.cpp 18004 2010-08-30 18:11:59Z gpotts $
#include <algorithm>
#include <iterator>
#include <ostream>
#include <ossim/imaging/ossimImageWriterFactory.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <ossim/imaging/ossimImageFileWriter.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimString.h>

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

ossimImageFileWriter* ossimImageWriterFactoryRegistry::createWriter(const ossimFilename& filename)const
{
   ossimImageFileWriter * writer = createWriterFromExtension(filename.ext().downcase());
   if(writer)
   {
      writer->setFilename(filename);
   }
   
   return writer;
}

ossimImageFileWriter *ossimImageWriterFactoryRegistry::createWriterFromExtension(const ossimString& fileExtension)const
{
   ossimImageFileWriter *writer = NULL;
   ossimImageWriterFactoryBase::ImageFileWriterList result;
   getImageFileWritersBySuffix(result, fileExtension);
   if(!result.empty())
   {
      writer = result[0].release();
      result.clear();
   }
   return writer;
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

void ossimImageWriterFactoryRegistry::getImageFileWritersBySuffix(ossimImageWriterFactoryBase::ImageFileWriterList& result,
                                                                  const ossimString& ext)const
{
   ossimImageWriterFactoryBase::ImageFileWriterList tempResult;
   vector<ossimImageWriterFactoryBase*>::const_iterator iter = theFactoryList.begin();
   
   while(iter != theFactoryList.end())
   {
      result.clear();
      (*iter)->getImageFileWritersBySuffix(tempResult, ext);
      
      // now append to the end of the typeList.
      result.insert(result.end(),
                      tempResult.begin(),
                      tempResult.end());
      ++iter;
   }  
   
}

void ossimImageWriterFactoryRegistry::getImageFileWritersByMimeType(ossimImageWriterFactoryBase::ImageFileWriterList& result,
                                                                    const ossimString& mimeType)const
{
   ossimImageWriterFactoryBase::ImageFileWriterList tempResult;
   vector<ossimImageWriterFactoryBase*>::const_iterator iter = theFactoryList.begin();
   
   while(iter != theFactoryList.end())
   {
      result.clear();
      (*iter)->getImageFileWritersByMimeType(tempResult, mimeType);
      
      // now append to the end of the typeList.
      result.insert(result.end(),
                    tempResult.begin(),
                    tempResult.end());
      ++iter;
   }  
}

std::ostream& ossimImageWriterFactoryRegistry::printImageTypeList(
   std::ostream& out)const
{
   std::vector<ossimString> outputType;
   
   this->getImageTypeList(outputType);
   std::copy(outputType.begin(),
             outputType.end(),
             std::ostream_iterator<ossimString>(out, "\n"));
   out << std::endl;
   return out;
}

std::ostream& ossimImageWriterFactoryRegistry::printWriterProps(std::ostream& out)const
{
   // Loop through factories:
   vector<ossimImageWriterFactoryBase*>::const_iterator factoryIter = theFactoryList.begin();
   while( factoryIter != theFactoryList.end() )
   {
      out << "factory: " << (*factoryIter)->getClassName() << "\n\n";

      // Loop through writer classes in factory.
      std::vector<ossimString> typeNames;
      (*factoryIter)->getTypeNameList(typeNames);
      std::vector<ossimString>::const_iterator typeNamesIter = typeNames.begin();
      while (typeNamesIter != typeNames.end())
      {
         ossimRefPtr<ossimImageFileWriter> writer = (*factoryIter)->createWriter(*typeNamesIter);
         if ( writer.valid() )
         {
            out << "writer:\n" << writer->getClassName() << "\n";

            // Loop through writer types, e.g. tiff_tiled_band_separate
            std::vector<ossimString> imageTypeList;
            writer->getImageTypeList(imageTypeList);
            std::vector<ossimString>::const_iterator imageTypeListIter = imageTypeList.begin();
            out << "\ntypes:\n";
            while ( imageTypeListIter != imageTypeList.end() )
            {
               out << (*imageTypeListIter) << "\n";
               ++imageTypeListIter;
            }

            // Loop through writer properties, e.g. compression_quality.
            out << "\nproperties:\n";
            std::vector<ossimString> propNames;
            writer->getPropertyNames(propNames);
            std::vector<ossimString>::const_iterator propNamesIter = propNames.begin();
            while ( propNamesIter != propNames.end() )
            {
               out << (*propNamesIter) << "\n";
               
               ++propNamesIter;
            }
            out << "\n";
         }

         ++typeNamesIter;
      }
      
      ++factoryIter;
   }
   return out;
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
