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

ossimInfoFactoryRegistry* ossimInfoFactoryRegistry::m_instance = 0;

ossimInfoFactoryRegistry::~ossimInfoFactoryRegistry()
{
}

ossimInfoFactoryRegistry* ossimInfoFactoryRegistry::instance()
{
   if ( !m_instance )
   {
      m_instance = new ossimInfoFactoryRegistry;
   }
   return m_instance;
}

ossimInfoBase* ossimInfoFactoryRegistry::create(
   const ossimFilename& file) const
{
   ossimInfoBase* result = 0;
   
   std::vector<ossimInfoFactoryInterface*>::const_iterator i =
      m_factoryList.begin();

   while ( i != m_factoryList.end() )
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
