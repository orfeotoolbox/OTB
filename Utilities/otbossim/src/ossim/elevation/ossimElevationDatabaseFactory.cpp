#include <ossim/elevation/ossimElevationDatabaseFactory.h>
#include <ossim/elevation/ossimDtedElevationDatabase.h>
#include <ossim/elevation/ossimSrtmElevationDatabase.h>
#include <ossim/elevation/ossimGeneralRasterElevationDatabase.h>
#include <ossim/base/ossimKeywordNames.h>

ossimElevationDatabaseFactory* ossimElevationDatabaseFactory::m_instance = 0;
ossimElevationDatabaseFactory* ossimElevationDatabaseFactory::instance()
{
   if(!m_instance)
   {
      m_instance = new ossimElevationDatabaseFactory();
   }
   
   return m_instance;
}

ossimElevationDatabase* ossimElevationDatabaseFactory::createDatabase(const ossimString& typeName)const
{
   if((typeName == STATIC_TYPE_NAME(ossimDtedElevationDatabase)) ||
      (typeName == "dted")||
      (typeName == "dted_directory"))
      
   {
      return new ossimDtedElevationDatabase();
   }
   else if((typeName == STATIC_TYPE_NAME(ossimSrtmElevationDatabase)) ||
           (typeName == "srtm")||
           (typeName == "srtm_directory"))
      
   {
      return new ossimSrtmElevationDatabase();
   }
   else if((typeName == STATIC_TYPE_NAME(ossimGeneralRasterElevationDatabase)) ||
           (typeName == "general_raster")||
           (typeName == "general_raster_directory"))
      
   {
      return new ossimGeneralRasterElevationDatabase();
   }
   
   return 0;
}

ossimElevationDatabase* ossimElevationDatabaseFactory::createDatabase(const ossimKeywordlist& kwl,
                                                                      const char* prefix)const
{
   ossimRefPtr<ossimElevationDatabase> result = 0;
   ossimString type = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   if(!type.empty())
   {
      result = createDatabase(type);
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

ossimElevationDatabase* ossimElevationDatabaseFactory::open(const ossimString& connectionString)const
{
   ossimRefPtr<ossimElevationDatabase> result = new ossimDtedElevationDatabase();
   
   if(!result->open(connectionString))
   {
      result = new ossimSrtmElevationDatabase;
      if(!result->open(connectionString))
      {
         result = new ossimGeneralRasterElevationDatabase;
         if(!result->open(connectionString))
         {
            result = 0;
         }
      }
   }
   
   return result.release();
}

void ossimElevationDatabaseFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back(STATIC_TYPE_NAME(ossimDtedElevationDatabase));
   typeList.push_back(STATIC_TYPE_NAME(ossimSrtmElevationDatabase));
   typeList.push_back(STATIC_TYPE_NAME(ossimGeneralRasterElevationDatabase));
}
