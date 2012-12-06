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
   m_factoryList.clear();
}

ossimInfoFactoryRegistry* ossimInfoFactoryRegistry::instance()
{
   if ( !m_instance )
   {
      m_instance = new ossimInfoFactoryRegistry;
   }
   return m_instance;
}

void ossimInfoFactoryRegistry::registerFactory(
   ossimInfoFactoryInterface* factory)
{
   if (factory)
   {
      m_mutex.lock();
      m_factoryList.push_back(factory);
      m_mutex.unlock();
   }
}

void ossimInfoFactoryRegistry::registerFactoryToFront(
   ossimInfoFactoryInterface* factory)
{
   if (factory)
   {
      m_mutex.lock();
      m_factoryList.insert( m_factoryList.begin(), factory );
      m_mutex.unlock();
   }
}

void ossimInfoFactoryRegistry::unregisterFactory(
   ossimInfoFactoryInterface* factory)
{
   m_mutex.lock();
   std::vector<ossimInfoFactoryInterface*>::iterator i =
      std::find(m_factoryList.begin(), m_factoryList.end(), factory);
   
   if( i != m_factoryList.end() )
   {
      m_factoryList.erase(i);
   }
   m_mutex.unlock();
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
   : m_factoryList(),
     m_mutex()
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
