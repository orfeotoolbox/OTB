//**************************************************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description: Implementation of ossim2dTo2dTransformFactory.
//
//**************************************************************************************************
// $Id$
#include <ossim/base/ossim2dTo2dTransformFactory.h>
#include <ossim/base/ossim2dBilinearTransform.h>
#include <ossim/base/ossim2dTo2dShiftTransform.h>
#include <ossim/base/ossim2dTo2dIdentityTransform.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimRefPtr.h>

ossim2dTo2dTransformFactory* ossim2dTo2dTransformFactory::m_instance = 0;
ossim2dTo2dTransformFactory* ossim2dTo2dTransformFactory::instance()
{
   if(!m_instance)
   {
      m_instance = new ossim2dTo2dTransformFactory();
   }
   return m_instance;
}

ossim2dTo2dTransform* ossim2dTo2dTransformFactory::createTransform(const ossimString& name)const
{
   ossim2dTo2dTransform* result = 0;
   if(name == STATIC_TYPE_NAME(ossim2dBilinearTransform))
   {
      result = new ossim2dBilinearTransform();
   }
   else if(name == STATIC_TYPE_NAME(ossim2dTo2dShiftTransform))
   {
      result = new ossim2dTo2dShiftTransform();
   }
   else if(name == STATIC_TYPE_NAME(ossim2dTo2dIdentityTransform))
   {
      result = new ossim2dTo2dIdentityTransform();
   }
   
   return result;
}

ossim2dTo2dTransform* ossim2dTo2dTransformFactory::createTransform(const ossimKeywordlist& kwl,
                                                                   const char* prefix)const
{
   ossimString type = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   ossimRefPtr<ossim2dTo2dTransform> result =  createTransform(type);
   if(result.valid()&&!result->loadState(kwl, prefix))
   {
      result = 0;
   }
   
   return result.release();
}

void ossim2dTo2dTransformFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back(STATIC_TYPE_NAME(ossim2dBilinearTransform));
   typeList.push_back(STATIC_TYPE_NAME(ossim2dTo2dShiftTransform));
   typeList.push_back(STATIC_TYPE_NAME(ossim2dTo2dIdentityTransform));
}
