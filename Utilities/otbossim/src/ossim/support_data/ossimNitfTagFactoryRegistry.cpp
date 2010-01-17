//*******************************************************************
//
// LICENSE: See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfTagFactoryRegistry.cpp 9212 2006-06-28 23:28:53Z dburken $

#include <cstddef>   /* for NULL      */
#include <algorithm> /* for std::find */

#include <ossim/support_data/ossimNitfTagFactoryRegistry.h>
#include <ossim/support_data/ossimNitfRpfTagFactory.h>
#include <ossim/support_data/ossimNitfRegisteredTagFactory.h>
#include <ossim/support_data/ossimNitfUnknownTag.h>

ossimNitfTagFactoryRegistry* ossimNitfTagFactoryRegistry::theInstance=NULL;

ossimNitfTagFactoryRegistry::ossimNitfTagFactoryRegistry()
{
}

ossimNitfTagFactoryRegistry::~ossimNitfTagFactoryRegistry()
{
   theInstance = NULL;
}

void ossimNitfTagFactoryRegistry::registerFactory(ossimNitfTagFactory* aFactory)
{
   if(aFactory&&!exists(aFactory))
   {
      theFactoryList.push_back(aFactory);
   }
}

void ossimNitfTagFactoryRegistry::unregisterFactory
(ossimNitfTagFactory* aFactory)
{
   std::vector<ossimNitfTagFactory*>::iterator iter =
      std::find(theFactoryList.begin(),
                theFactoryList.end(),
                aFactory);
   if(iter != theFactoryList.end())
   {
      theFactoryList.erase(iter);
   }
}

ossimNitfTagFactoryRegistry* ossimNitfTagFactoryRegistry::instance()
{
   if(theInstance)
   {
      return theInstance;
   }
   else
   {
      theInstance = new ossimNitfTagFactoryRegistry;
      theInstance->initializeDefaults();
   }
   
   return theInstance;
}

ossimRefPtr<ossimNitfRegisteredTag> ossimNitfTagFactoryRegistry::create(const ossimString &tagName)const
{
   std::vector<ossimNitfTagFactory*>::const_iterator factory;
   ossimRefPtr<ossimNitfRegisteredTag> result = NULL;

   factory = theFactoryList.begin();
   while(factory != theFactoryList.end())
   {
      result = (*factory)->create(tagName);

      if(result.valid())
      {
         return result;
      }
      ++factory;
   }

   ossimRefPtr<ossimNitfRegisteredTag> tag = new ossimNitfUnknownTag;
   return tag;
}

void ossimNitfTagFactoryRegistry::initializeDefaults()
{
   theFactoryList.push_back(ossimNitfRegisteredTagFactory::instance());
   theFactoryList.push_back(ossimNitfRpfTagFactory::instance());
}

bool ossimNitfTagFactoryRegistry::exists(ossimNitfTagFactory* factory)const
{
   std::vector<ossimNitfTagFactory*>::const_iterator iter = std::find(theFactoryList.begin(),
                                                                      theFactoryList.end(),
                                                                      factory);

   return (iter != theFactoryList.end());
   
}

ossimNitfTagFactoryRegistry::ossimNitfTagFactoryRegistry(
   const ossimNitfTagFactoryRegistry&)
{
}

const ossimNitfTagFactoryRegistry& ossimNitfTagFactoryRegistry::operator=(
   const ossimNitfTagFactoryRegistry&)
{
   return *this;
}
