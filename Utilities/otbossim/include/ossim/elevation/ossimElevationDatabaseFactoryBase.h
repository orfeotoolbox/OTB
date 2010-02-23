#ifndef ossimElevationDatabaseFactoryBase_HEADER
#define ossimElevationDatabaseFactoryBase_HEADER
#include <ossim/base/ossimObjectFactory.h>
#include <ossim/elevation/ossimElevationDatabase.h>

class OSSIM_DLL ossimElevationDatabaseFactoryBase : public ossimObjectFactory
{
public:
   /**
    * Creates an object given a type name.
    */
   virtual ossimObject* createObject(const ossimString& typeName)const
   {
      return createDatabase(typeName);
   }
   
   /**
    * Creates and object given a keyword list.
    */
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const
   {
      return createDatabase(kwl, prefix);
   }
   
   virtual ossimElevationDatabase* createDatabase(const ossimString& typeName)const=0;
   virtual ossimElevationDatabase* createDatabase(const ossimKeywordlist& kwl,
                                                  const char* prefix=0)const=0;
   virtual ossimElevationDatabase* open(const ossimString& connectionString)const=0;
};

#endif
