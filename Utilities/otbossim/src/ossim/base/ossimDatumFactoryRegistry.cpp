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
#include <ossim/base/ossimDatumFactoryInterface.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimString.h>

//ossimDatumFactoryRegistry* ossimDatumFactoryRegistry::theInstance = 0;

ossimDatumFactoryRegistry::~ossimDatumFactoryRegistry()
{
}

ossimDatumFactoryRegistry* ossimDatumFactoryRegistry::instance()
{
   static ossimDatumFactoryRegistry sharedInstance;

   return &sharedInstance;
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
         return result;
      }
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
}

ossimDatumFactoryRegistry::ossimDatumFactoryRegistry(
   const ossimDatumFactoryRegistry& /*obj*/)
   : theFactoryList()
{
   // hidden, never called.
}

const ossimDatumFactoryRegistry& ossimDatumFactoryRegistry::operator=(
   const ossimDatumFactoryRegistry& /* rhs */)
{
   // hidden, never called.
   return *this;
}
