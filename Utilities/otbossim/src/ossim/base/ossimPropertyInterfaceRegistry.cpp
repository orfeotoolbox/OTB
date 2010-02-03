

#include <ossim/base/ossimPropertyInterfaceRegistry.h>
#include <ossim/base/ossimPropertyInterfaceFactory.h>
#include <ossim/base/ossimString.h>
using namespace std;

RTTI_DEF1( ossimPropertyInterfaceRegistry, "ossimPropertyInterfaceRegistry", ossimObject );

ossimPropertyInterfaceRegistry* ossimPropertyInterfaceRegistry::theInstance = NULL;

ossimPropertyInterfaceRegistry::~ossimPropertyInterfaceRegistry()
{
   theInstance = NULL;
}

ossimPropertyInterface* ossimPropertyInterfaceRegistry::createInterface( const ossimObject* obj ) const
{
   vector<ossimPropertyInterfaceFactory*>::const_iterator i = theFactoryList.begin();
   ossimPropertyInterface* result = NULL;
   
   while ( ( i != theFactoryList.end() ) && ( ! result ) )
   {
      result = (*i)->createInterface( obj );

      ++i;
   }

   return result;
}

ossimPropertyInterfaceRegistry* ossimPropertyInterfaceRegistry::instance()
{
   if ( !theInstance)
   {
      theInstance = new ossimPropertyInterfaceRegistry;
   }

   return theInstance;
}

ossimObject* ossimPropertyInterfaceRegistry::createObject(const ossimString& typeName)const
{
   ossimObject* result = (ossimObject*)NULL;
   vector<ossimPropertyInterfaceFactory*>::const_iterator i = theFactoryList.begin();

   while ( ( i != theFactoryList.end() ) && ( ! result ) )
   {
      result = (*i)->createObject(typeName);

      ++i;
   }
   
   return result;
}

ossimObject* ossimPropertyInterfaceRegistry::createObject(const ossimKeywordlist& kwl,
                                                          const char* prefix)const
{
   ossimObject* result = (ossimObject*)NULL;
   vector<ossimPropertyInterfaceFactory*>::const_iterator i = theFactoryList.begin();

   while ( ( i != theFactoryList.end() ) && ( ! result ) )
   {
      result = (*i)->createObject(kwl, prefix);

      ++i;
   }
   
   return result;
}

void ossimPropertyInterfaceRegistry::getTypeNameList(std::vector<ossimString>& typeList)const
{
   vector<ossimPropertyInterfaceFactory*>::const_iterator i = theFactoryList.begin();

   while ( i != theFactoryList.end() )
   {
      std::vector<ossimString> tempTypeList;
      
      (*i)->getTypeNameList(tempTypeList);
      typeList.insert(typeList.end(),
                      tempTypeList.begin(),
                      tempTypeList.end());
      ++i;
   }
   
}

bool ossimPropertyInterfaceRegistry::addFactory( ossimPropertyInterfaceFactory* factory )
{
   return registerFactory(factory);
}

bool ossimPropertyInterfaceRegistry::registerFactory(ossimPropertyInterfaceFactory* factory)
{
   bool result = false;
   
   if ( factory )
   {
      theFactoryList.push_back( factory );
      result = true;
   }

   return result;
}

extern "C"
{
  void* ossimPropertyInterfaceRegistryGetInstance()
  {
    return ossimPropertyInterfaceRegistry::instance();
  }
}
