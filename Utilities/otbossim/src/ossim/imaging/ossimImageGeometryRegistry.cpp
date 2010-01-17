//**************************************************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description: Class implementation of ossimImageGeometryRegistry. See .h file for class documentation.
//
//**************************************************************************************************
// $Id$
#include <ossim/imaging/ossimImageGeometryRegistry.h>
#include <ossim/imaging/ossimImageGeometryFactory.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
ossimImageGeometryRegistry* ossimImageGeometryRegistry::m_instance = 0;
RTTI_DEF1(ossimImageGeometryRegistry, "ossimImageGeometryRegistry", ossimObjectFactory);
ossimImageGeometryRegistry* ossimImageGeometryRegistry::instance()
{
   if(!m_instance)
   {
      m_instance = new ossimImageGeometryRegistry();
      m_instance->registerFactory(ossimImageGeometryFactory::instance());
      ossimObjectFactoryRegistry::instance()->registerFactory(m_instance);
   }
   
   return m_instance;
}

bool ossimImageGeometryRegistry::extendGeometry(ossimImageHandler* handler)const
{
   bool result = false;
   ossim_uint32 idx = 0;
   for(;((idx < m_factoryList.size())&&!result); ++idx)
   {
      result = m_factoryList[idx]->extendGeometry(handler);
   }
   
   return result;
}
