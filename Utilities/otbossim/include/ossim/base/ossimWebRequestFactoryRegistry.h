#ifndef ossimWebFactoryRegistry_HEADER
#define ossimWebFactoryRegistry_HEADER
#include <ossim/base/ossimFactoryListInterface.h>
#include <ossim/base/ossimWebRequestFactoryBase.h>
#include <ossim/base/ossimWebRequest.h>


/**
 * This will later be changed fro ossimHttpRequest to ossimWebRequest so we have 
 * a base class for general schema access such as ftp, sftp,smtp, ... etc and not
 * just restricted to http and https schema definitions.
 */
class OSSIM_DLL ossimWebRequestFactoryRegistry : public ossimWebRequestFactoryBase,
public ossimFactoryListInterface<ossimWebRequestFactoryBase, ossimWebRequest>

{
public:
   static ossimWebRequestFactoryRegistry* instance();
 
   virtual ossimWebRequest* create(const ossimUrl& url);
   virtual ossimHttpRequest* createHttp(const ossimUrl& url);
   
   /*!
    * Creates an object given a type name.
    */
   virtual ossimObject* createObject(const ossimString& typeName)const
   {
      return createObjectFromRegistry(typeName);
   }
   
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
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const
   {
      getAllTypeNamesFromRegistry(typeList);
   }
protected:
   ossimWebRequestFactoryRegistry();
   static ossimWebRequestFactoryRegistry* m_instance;
};
#endif
