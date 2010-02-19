#include <ossim/elevation/ossimElevationDatabaseRegistry.h>
#include <ossim/elevation/ossimElevationDatabaseFactory.h>

ossimElevationDatabaseRegistry* ossimElevationDatabaseRegistry::m_instance = 0;
ossimElevationDatabaseRegistry* ossimElevationDatabaseRegistry::instance()
{
   if(!m_instance)
   {
      m_instance = new ossimElevationDatabaseRegistry;
      
      m_instance->registerFactory(ossimElevationDatabaseFactory::instance());
   }
   
   return m_instance;
}

ossimElevationDatabase* ossimElevationDatabaseRegistry::createDatabase(const ossimString& typeName)const
{
   ossimElevationDatabase* result = 0;
   ossim_uint32 idx = 0;
   for(;((idx < m_factoryList.size())&&!result); ++idx)
   {
      result = m_factoryList[idx]->createDatabase(typeName);
   }
   
   return result;
   
}
ossimElevationDatabase* ossimElevationDatabaseRegistry::createDatabase(const ossimKeywordlist& kwl,
                                                                       const char* prefix)const
{
   ossimElevationDatabase* result = 0;
   ossim_uint32 idx = 0;
   for(;((idx < m_factoryList.size())&&!result); ++idx)
   {
      result = m_factoryList[idx]->createDatabase(kwl, prefix);
   }
   
   return result;
}

ossimElevationDatabase* ossimElevationDatabaseRegistry::open(const ossimString& connectionString)
{
   ossimElevationDatabase* result = 0;
   ossim_uint32 idx = 0;
   for(;((idx < m_factoryList.size())&&!result); ++idx)
   {
      result = m_factoryList[idx]->open(connectionString);
   }
   
   return result;
}

