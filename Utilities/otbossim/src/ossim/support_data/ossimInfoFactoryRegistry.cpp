//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file
//
// Author:  David Burken
//
// Description: Class definition of registry (singleton) for info factories.
//
//----------------------------------------------------------------------------
// $Id$

#include <ossim/base/ossimFilename.h>
#include <ossim/support_data/ossimInfoFactoryRegistry.h>
#include <ossim/support_data/ossimInfoFactoryInterface.h>
#include <ossim/support_data/ossimInfoFactory.h>

#include <algorithm> /* for std::find */

ossimInfoFactoryRegistry* ossimInfoFactoryRegistry::theInstance = 0;

ossimInfoFactoryRegistry::~ossimInfoFactoryRegistry()
{
   theFactoryList.clear();
}

ossimInfoFactoryRegistry* ossimInfoFactoryRegistry::instance()
{
   if ( !theInstance )
   {
      theInstance = new ossimInfoFactoryRegistry;
   }
   return theInstance;
}

void ossimInfoFactoryRegistry::registerFactory(
   ossimInfoFactoryInterface* factory)
{
   if (factory)
   {
      theFactoryList.push_back(factory);
   }
}

void ossimInfoFactoryRegistry::unregisterFactory(
   ossimInfoFactoryInterface* factory)
{
   std::vector<ossimInfoFactoryInterface*>::iterator i =
      std::find(theFactoryList.begin(), theFactoryList.end(), factory);
   
   if( i != theFactoryList.end() )
   {
      theFactoryList.erase(i);
   }
}

ossimInfoBase* ossimInfoFactoryRegistry::create(
   const ossimFilename& file) const
{
   ossimInfoBase* result = 0;
   
   std::vector<ossimInfoFactoryInterface*>::const_iterator i =
      theFactoryList.begin();

   while ( i != theFactoryList.end() )
   {
      result = (*i)->create(file);
      if ( result )
      {
         break;
      }
      ++i;
   }

   return result;
}

/** hidden from use default constructor */
ossimInfoFactoryRegistry::ossimInfoFactoryRegistry()
   : theFactoryList()
{
   this->registerFactory(ossimInfoFactory::instance());
}

/** hidden from use copy constructor */
ossimInfoFactoryRegistry::ossimInfoFactoryRegistry(
   const ossimInfoFactoryRegistry& /* obj */)
{}

/** hidden from use operator = */
const ossimInfoFactoryRegistry& ossimInfoFactoryRegistry::operator=(
   const ossimInfoFactoryRegistry& /* rhs */ )
{
   return *this;
}
