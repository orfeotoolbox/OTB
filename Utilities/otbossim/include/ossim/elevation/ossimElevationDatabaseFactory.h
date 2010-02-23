#ifndef ossimElevationDatabaseFactory_HEADER
#define ossimElevationDatabaseFactory_HEADER
#include "ossimElevationDatabaseFactoryBase.h"

class OSSIM_DLL ossimElevationDatabaseFactory : public ossimElevationDatabaseFactoryBase
{
public:
   ossimElevationDatabaseFactory()
   {
      m_instance = this;
   }
   static ossimElevationDatabaseFactory* instance();
   
   ossimElevationDatabase* createDatabase(const ossimString& typeName)const;
   ossimElevationDatabase* createDatabase(const ossimKeywordlist& kwl,
                                          const char* prefix=0)const;
   virtual ossimElevationDatabase* open(const ossimString& connectionString)const;
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
protected:
   static ossimElevationDatabaseFactory* m_instance;
};
#endif
