//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file
//
// Author:  David Burken
//
// Description: Class definition of Registry (singleton) for datum factories.
//
//----------------------------------------------------------------------------
// $Id$

#include <ossim/base/ossimDatumFactoryRegistry.h>
#include <ossim/base/ossimDatumFactory.h>
#include <ossim/base/ossimEpsgDatumFactory.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimString.h>

ossimDatumFactoryRegistry* ossimDatumFactoryRegistry::theInstance = 0;

ossimDatumFactoryRegistry::~ossimDatumFactoryRegistry()
{
}

ossimDatumFactoryRegistry* ossimDatumFactoryRegistry::instance()
{
   if (!theInstance)
   {
      theInstance =  new ossimDatumFactoryRegistry;
   }
   return theInstance;
}

void ossimDatumFactoryRegistry::registerFactory(
   ossimDatumFactoryInterface* factory)
{
   if (factory)
   {
      theFactoryList.push_back(factory);
   }
}

const ossimDatum* ossimDatumFactoryRegistry::create(
   const ossimString &code)const
{
   const ossimDatum* result = 0;

   std::vector<ossimDatumFactoryInterface*>::const_iterator i =
      theFactoryList.begin();

   while (i != theFactoryList.end())
   {
      result = (*i)->create(code);
      if (result)
      {
         break;
      }
      ++i; // go to next factory
   }
   return result;
}

const ossimDatum* ossimDatumFactoryRegistry::create(const ossimKeywordlist& kwl, 
                                                    const char *prefix)const
{
   const ossimDatum* result = 0;
   std::vector<ossimDatumFactoryInterface*>::const_iterator i = theFactoryList.begin();

   while (i != theFactoryList.end())
   {
      result = (*i)->create(kwl, prefix);
      if (result)
         return result;

      ++i; // go to next factory
   }
   return result;
}

void ossimDatumFactoryRegistry::getList(std::vector<ossimString>& list) const
{
   std::vector<ossimDatumFactoryInterface*>::const_iterator i =
      theFactoryList.begin();
   while (i != theFactoryList.end())
   {
      (*i)->getList(list);
      ++i;
   }
}

ossimDatumFactoryRegistry::ossimDatumFactoryRegistry()
   : theFactoryList()
{
   registerFactory(ossimDatumFactory::instance());
   registerFactory(ossimEpsgDatumFactory::instance());
}
