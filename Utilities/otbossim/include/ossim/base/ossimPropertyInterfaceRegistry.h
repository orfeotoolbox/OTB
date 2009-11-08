#ifndef ossimPropertyInterfaceRegistry_HEADER
#define ossimPropertyInterfaceRegistry_HEADER
#include <ossim/base/ossimObjectFactory.h>

class ossimPropertyInterfaceFactory;
class ossimPropertyInterface;

#include <ossim/base/ossimRtti.h>

#include <vector>
using namespace std;

class OSSIMDLLEXPORT ossimPropertyInterfaceRegistry : public ossimObjectFactory
{
public:
   virtual ~ossimPropertyInterfaceRegistry();
   
   static ossimPropertyInterfaceRegistry* instance();
   
   virtual ossimPropertyInterface* createInterface( const ossimObject* obj ) const;

   /*!
    * Creates an object given a type name.
    */
   virtual ossimObject* createObject(const ossimString& typeName)const;

   /*!
    * Creates and object given a keyword list.
    */
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   
   /*!
    * This should return the type name of all objects in all factories.
    * This is the name used to construct the objects dynamially and this
    * name must be unique.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
   
   bool addFactory( ossimPropertyInterfaceFactory* factory );
   bool registerFactory(ossimPropertyInterfaceFactory* factory);
protected:
   ossimPropertyInterfaceRegistry()
      :ossimObjectFactory()
      {}

   ossimPropertyInterfaceRegistry( const ossimPropertyInterfaceRegistry& rhs )
      :ossimObjectFactory(rhs)
      {}
   void operator =(const ossimPropertyInterfaceRegistry&){}
   static ossimPropertyInterfaceRegistry* theInstance;
   vector<ossimPropertyInterfaceFactory*> theFactoryList;

TYPE_DATA
};

extern "C"
{
  OSSIMDLLEXPORT void* ossimPropertyInterfaceRegistryGetInstance();
}
#endif
