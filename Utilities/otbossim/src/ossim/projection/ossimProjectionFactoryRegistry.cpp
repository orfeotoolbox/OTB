//*******************************************************************
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimProjectionFactoryRegistry.cpp 10225 2007-01-10 19:16:12Z dburken $
#include <algorithm>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimProjectionFactoryBase.h>
#include <ossim/projection/ossimSrsProjectionFactory.h>
#include <ossim/projection/ossimMapProjectionFactory.h>
#include <ossim/projection/ossimTiffProjectionFactory.h>
#include <ossim/projection/ossimNitfProjectionFactory.h>
#include <ossim/projection/ossimPcsCodeProjectionFactory.h>
#include <ossim/projection/ossimStatePlaneProjectionFactory.h>
#include <ossim/projection/ossimSensorModelFactory.h>
#include <ossim/projection/ossimMiscProjectionFactory.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>

ossimProjectionFactoryRegistry* ossimProjectionFactoryRegistry::theInstance=0;

ossimProjectionFactoryRegistry::~ossimProjectionFactoryRegistry()
{
   theFactoryList.clear();
   theInstance = 0;
}

ossimProjectionFactoryRegistry* ossimProjectionFactoryRegistry::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimProjectionFactoryRegistry;
      theInstance->initializeDefaults();
      ossimObjectFactoryRegistry::instance()->registerFactory(theInstance);
   }

   return theInstance;
}

ossimProjection*
ossimProjectionFactoryRegistry::createProjection(const ossimFilename& name,
                                                 ossim_uint32 entryIdx)const
{
   ossimProjection* result = 0;
   std::vector<ossimProjectionFactoryBase*>::const_iterator factory = theFactoryList.begin();

   while( (factory != theFactoryList.end()) && !result )
   {
      if(*factory)
      {
         result = (*factory)->createProjection(name, entryIdx);
      }
      
      ++factory;
   }
   
   return result;
}

ossimProjection* ossimProjectionFactoryRegistry::createProjection(const ossimString& name)const
{
   ossimProjection* result = 0;
   std::vector<ossimProjectionFactoryBase*>::const_iterator factory = theFactoryList.begin();

   while((factory != theFactoryList.end()&&!
          result))
   {
      if(*factory)
      {
         result = (*factory)->createProjection(name);
      }
      
      ++factory;
   }
   
   return result;
}

ossimProjection* ossimProjectionFactoryRegistry::createProjection(
   const ossimKeywordlist& kwl, const char* prefix)const
{
   ossimProjection* result = 0;
   std::vector<ossimProjectionFactoryBase*>::const_iterator factory = theFactoryList.begin();

   while( (factory != theFactoryList.end()) && !result )
   {
      if(*factory)
      {
         result = (*factory)->createProjection(kwl, prefix);
      }
      
      ++factory;
   }

   if ( (result == 0) && (prefix == 0) )
   {
      //---
      // Check for an embedded prefix like "image0."
      //
      // This is a workaround for people who use the "image_info" application
      // to create a geometry file.
      //
      // If the image has an external geometry file and it has a prefix like
      // "image0." and someone does:
      //
      // theInputConnection->getImageGeometry(kwl);
      // ossimRefPtr<ossimProjection> proj =
      // ossimProjectionFactoryRegistry::instance()->createProjection(kwl);
      //
      // It will fail because the factory doesn't know it has a prefix.
      //---
      ossimKeywordlist::KeywordMap::const_iterator i = kwl.getMap().begin();
      if ( i != kwl.getMap().end() )
      {
         ossimString s1 = (*i).first;
         if ( s1.size() )
         {
            vector<ossimString> v;
            s1.split(v, ossimString("."));
            if ( v.size() )
            {
               if ( v[0].contains("image") )
               {
                  ossimString s2 = v[0];
                  s2 += ".";
                  factory = theFactoryList.begin();
                  while( (factory != theFactoryList.end()) && !result )
                  {
                     if(*factory)
                     {
                        result = (*factory)->createProjection(kwl, s2.c_str());
                     }
                     
                     ++factory;
                  }
               }
            }
         }
      }
   }

   return result;
}

bool ossimProjectionFactoryRegistry::registerFactory(
   ossimProjectionFactoryBase* factory, bool pushToFrontFlag)
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
      return true;
   }

   return false;
}

void ossimProjectionFactoryRegistry::unregisterFactory(ossimProjectionFactoryBase* factory)
{
   std::vector<ossimProjectionFactoryBase*>::iterator iter =  std::find(theFactoryList.begin(),
                                                                        theFactoryList.end(),
                                                                        factory);
   if(iter != theFactoryList.end())
   {
      theFactoryList.erase(iter);
   }
}

bool ossimProjectionFactoryRegistry::findFactory(ossimProjectionFactoryBase* factory)const
{
   return (std::find(theFactoryList.begin(),
                     theFactoryList.end(),
                     factory)!=theFactoryList.end());
}

std::list<ossimString> ossimProjectionFactoryRegistry::getList()const
{
   std::list<ossimString> result;
   std::vector<ossimProjectionFactoryBase*>::const_iterator factory = theFactoryList.begin();

   while(factory != theFactoryList.end())
   {
      if(*factory)
      {
         std::list<ossimString> temp = (*factory)->getList();
         result.insert(result.end(),
                       temp.begin(),
                       temp.end());
      }
      
      ++factory;
   }
   
   return result;
   
}

ossimObject* ossimProjectionFactoryRegistry::createObject(const ossimString& typeName)const
{
   return createProjection(typeName);
}

ossimObject* ossimProjectionFactoryRegistry::createObject(const ossimKeywordlist& kwl,
                                                          const char* prefix)const
{
   return createProjection(kwl, prefix);
}

void ossimProjectionFactoryRegistry::getTypeNameList(std::vector<ossimString>& typeList)const
{
   std::vector<ossimString> result;
   std::vector<ossimProjectionFactoryBase*>::const_iterator factory = theFactoryList.begin();

   while(factory != theFactoryList.end())
   {
      if(*factory)
      {
         result.clear();
         (*factory)->getTypeNameList(result);
         typeList.insert(typeList.end(),
                         result.begin(),
                         result.end());
      }
      
      ++factory;
   }   
}

ossimProjectionFactoryRegistry::ossimProjectionFactoryRegistry()
{}

ossimProjectionFactoryRegistry::ossimProjectionFactoryRegistry(
   const ossimProjectionFactoryRegistry& rhs)
   :
   ossimObjectFactory(rhs)
{}

void ossimProjectionFactoryRegistry::operator=(
   const ossimProjectionFactoryRegistry&)
{}

void ossimProjectionFactoryRegistry::initializeDefaults()
{
   theInstance->registerFactory(ossimSensorModelFactory::instance());
   theInstance->registerFactory(ossimMapProjectionFactory::instance());
   theInstance->registerFactory(ossimSrsProjectionFactory::instance());
   theInstance->registerFactory(ossimTiffProjectionFactory::instance());
   theInstance->registerFactory(ossimPcsCodeProjectionFactory::instance());   
   theInstance->registerFactory(ossimStatePlaneProjectionFactory::instance());
   theInstance->registerFactory(ossimNitfProjectionFactory::instance());   
   theInstance->registerFactory(ossimMiscProjectionFactory::instance());
}

extern "C"
{
  void* ossimProjectionFactoryRegistryGetInstance()
  {
    return ossimProjectionFactoryRegistry::instance();
  }

}
