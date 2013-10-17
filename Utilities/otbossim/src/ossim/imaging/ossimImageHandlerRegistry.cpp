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
//  $Id: ossimImageHandlerRegistry.cpp 22228 2013-04-12 14:11:45Z dburken $

#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/base/ossimString.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageHandlerFactory.h>
#include <ossim/imaging/ossimImageHandlerFactoryBase.h>
#include <algorithm>
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
   unregisterAllFactories();
}


ossimObject* ossimImageHandlerRegistry::createObject(const ossimKeywordlist& kwl,
                                                     const char* prefix)const
{
   ossimObject* result = createObjectFromRegistry(kwl, prefix);
   if(!result)
   {
      result = open(kwl, prefix);
   }
   return result;
}

ossimRefPtr<ossimImageHandler> ossimImageHandlerRegistry::openBySuffix(const ossimFilename& file,
                                                                       bool openOverview)const
{
   std::vector<ossimRefPtr<ossimImageHandler> > handlers;
   
   getImageHandlersBySuffix(handlers, file.ext());
   ossim_uint32 idx = 0;
   ossim_uint32 size = (ossim_uint32) handlers.size();
   
   for(idx = 0; idx < size; ++idx)
   {
      handlers[idx]->setOpenOverviewFlag(openOverview);
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
   vector<ossimImageHandlerFactoryBase*>::const_iterator iter = m_factoryList.begin();
   ossimImageHandlerFactoryBase::ImageHandlerList temp;
   while(iter != m_factoryList.end())
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

void ossimImageHandlerRegistry::getImageHandlersByMimeType(
   ossimImageHandlerFactoryBase::ImageHandlerList& result, const ossimString& mimeType)const
{
   vector<ossimImageHandlerFactoryBase*>::const_iterator iter = m_factoryList.begin();
   ossimImageHandlerFactoryBase::ImageHandlerList temp;
   while(iter != m_factoryList.end())
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

void ossimImageHandlerRegistry::getTypeNameList( std::vector<ossimString>& typeList ) const
{
   getAllTypeNamesFromRegistry(typeList);
}

void ossimImageHandlerRegistry::getSupportedExtensions(
   ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const
{
   vector<ossimString> result;
   vector<ossimImageHandlerFactoryBase*>::const_iterator iter = m_factoryList.begin();

   while(iter != m_factoryList.end())
   {
      (*iter)->getSupportedExtensions(extensionList);

      ++iter;
   }
   
}

ossimImageHandler* ossimImageHandlerRegistry::open(const ossimFilename& fileName,
                                                   bool trySuffixFirst,
                                                   bool openOverview)const
{
   if(trySuffixFirst)
   {
      ossimRefPtr<ossimImageHandler> h = openBySuffix(fileName, openOverview);
      if(h.valid())
      {
         return h.release();
      }
   }
   
   // now try magic number opens
   //
   ossimImageHandler*                   result = NULL;
   vector<ossimImageHandlerFactoryBase*>::const_iterator factory;

   factory = m_factoryList.begin();
   while((factory != m_factoryList.end()) && !result)
   {
      result = (*factory)->open(fileName, openOverview);
      ++factory;
   }
   
   return result;
}

ossimImageHandler* ossimImageHandlerRegistry::open(const ossimKeywordlist& kwl,
                                                   const char* prefix)const
{
   ossimImageHandler*                   result = NULL;
   vector<ossimImageHandlerFactoryBase*>::const_iterator factory;
   
   factory = m_factoryList.begin();
   while((factory != m_factoryList.end()) && !result)
   {
      result = (*factory)->open(kwl, prefix);
      ++factory;
   }
   
   return result;
}

ossimRefPtr<ossimImageHandler> ossimImageHandlerRegistry::openOverview(
   const ossimFilename& file ) const
{
   ossimRefPtr<ossimImageHandler> result = 0;
   vector<ossimImageHandlerFactoryBase*>::const_iterator factory = m_factoryList.begin();
   while( factory != m_factoryList.end() )
   {
      result = (*factory)->openOverview( file );
      if ( result.valid() )
      {
         break;
      }
      ++factory;
   }  
   return result;
}

ossimObject* ossimImageHandlerRegistry::createObject(const ossimString& typeName) const
{
   return createObjectFromRegistry(typeName);
}

std::ostream& ossimImageHandlerRegistry::printReaderProps(std::ostream& out) const
{
   // Loop through factories:
   vector<ossimImageHandlerFactoryBase*>::const_iterator factory = m_factoryList.begin();
   while( factory != m_factoryList.end() )
   {
      out << "factory: " << (*factory)->getClassName() << "\n";
      
      // Loop through factory image handlers:
      std::vector<ossimString> readerList;
      (*factory)->getTypeNameList(readerList);

      std::vector<ossimString>::const_iterator i = readerList.begin();
      while ( i != readerList.end() )
      {
         ossimRefPtr<ossimImageHandler> ih =
            dynamic_cast<ossimImageHandler*>( (*factory)->createObject( (*i) ) );
         if ( ih.valid() )
         {
            std::vector<ossimString> propertyList;
            ih->getPropertyNames(propertyList);
            out << "reader: " << ih->getClassName() << "\n";
            
            if ( propertyList.size() )
            {
               // Loop through image handler properties:
               out << "properties:\n";
               std::vector<ossimString>::const_iterator p = propertyList.begin();
               while ( p != propertyList.end() )
               {
                  out << (*p) << "\n";
                  ++p;
               }
            }
         }
         ++i;
      }
      ++factory;
   }
   out << std::endl;
   return out;
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
