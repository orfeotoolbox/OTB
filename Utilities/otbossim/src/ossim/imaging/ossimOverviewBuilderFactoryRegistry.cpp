//----------------------------------------------------------------------------
// 
// See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: The factory registry for overview builders.
//
//----------------------------------------------------------------------------
// $Id: ossimOverviewBuilderFactoryRegistry.cpp 19907 2011-08-05 19:55:46Z dburken $

#include <ossim/imaging/ossimOverviewBuilderFactoryRegistry.h>
#include <ossim/imaging/ossimOverviewBuilderFactoryBase.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <algorithm> /* for std::find */

ossimOverviewBuilderFactoryRegistry* ossimOverviewBuilderFactoryRegistry::m_instance = 0;

ossimOverviewBuilderFactoryRegistry* ossimOverviewBuilderFactoryRegistry::instance()
{
   if ( m_instance == 0 )
   {
      m_instance = new ossimOverviewBuilderFactoryRegistry();
      ossimObjectFactoryRegistry::instance()->registerFactory(m_instance);
   }
   return m_instance;
}

ossimObject* ossimOverviewBuilderFactoryRegistry::createObject(const ossimString& typeName)const
{
   return createObjectFromRegistry(typeName);
}

ossimObject* ossimOverviewBuilderFactoryRegistry::createObject(const ossimKeywordlist& kwl,
                                                               const char* prefix)const
{
   ossimRefPtr<NativeReturnType> result = 0;

   ossimString type = kwl.find(prefix, "type");
   if(!type.empty())
   {
      result = createBuilder(type);
      if(result.valid())
      {
         if(!result->loadState(kwl, prefix))
         {
            result = 0;
         }
      }
   }
   
   return result.release();
}

ossimOverviewBuilderBase*
ossimOverviewBuilderFactoryRegistry::createBuilder(
   const ossimString& typeName) const
{
   FactoryListType::const_iterator iter = m_factoryList.begin();
   NativeReturnType* result = 0;
   
   while(iter != m_factoryList.end())
   {
      result = (*iter)->createBuilder(typeName);
      if (result)
      {
         break;
      }
      ++iter;
   }
   
   return result;
}

void ossimOverviewBuilderFactoryRegistry::getTypeNameList(std::vector<ossimString>& typeList)const
{
   getAllTypeNamesFromRegistry(typeList);
}

ossimOverviewBuilderFactoryRegistry::ossimOverviewBuilderFactoryRegistry()
{
   m_instance = this;
}

ossimOverviewBuilderFactoryRegistry::ossimOverviewBuilderFactoryRegistry(
   const ossimOverviewBuilderFactoryRegistry& /* obj */ )
{
   m_instance = this;
}

void ossimOverviewBuilderFactoryRegistry::operator=(
   const ossimOverviewBuilderFactoryRegistry& /* rhs */ )
{
   
}
