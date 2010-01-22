//**************************************************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description: Implementation of ossim2dTo2dTransformRegistry.
//
//**************************************************************************************************
// $Id$
#include <ossim/base/ossim2dTo2dTransformRegistry.h>
#include <ossim/base/ossim2dTo2dTransformFactory.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
ossim2dTo2dTransformRegistry* ossim2dTo2dTransformRegistry::m_instance = 0;
RTTI_DEF1(ossim2dTo2dTransformRegistry, "ossim2dTo2dTransformRegistry", ossimObjectFactory);
ossim2dTo2dTransformRegistry* ossim2dTo2dTransformRegistry::instance()
{
   if(!m_instance)
   {
      m_instance = new ossim2dTo2dTransformRegistry();
      m_instance->registerFactory(ossim2dTo2dTransformFactory::instance());
      ossimObjectFactoryRegistry::instance()->registerFactory(m_instance);
   }
   
   return m_instance;
}
