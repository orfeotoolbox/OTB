#ifndef ossimElevationDatabaseRegistry_HEADER
#define ossimElevationDatabaseRegistry_HEADER
#include <ossim/elevation/ossimElevationDatabaseFactoryBase.h>
#include <ossim/elevation/ossimElevationDatabase.h>
#include <ossim/base/ossimFactoryListInterface.h>
class OSSIM_DLL ossimElevationDatabaseRegistry : public ossimFactoryListInterface<ossimElevationDatabaseFactoryBase, ossimElevationDatabase>

{
public:
   ossimElevationDatabaseRegistry()
   {
      m_instance = 0;
   }
   static ossimElevationDatabaseRegistry* instance();
   void getTypeNameList(std::vector<ossimString>& typeList) const 
   {
      getAllTypeNamesFromRegistry(typeList);
   }
   ossimElevationDatabase* createDatabase(const ossimString& typeName)const;
   ossimElevationDatabase* createDatabase(const ossimKeywordlist& kwl,
                                          const char* prefix=0)const;
   ossimElevationDatabase* open(const ossimString& connectionString);

protected:
   static ossimElevationDatabaseRegistry* m_instance;
};
#endif
