//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimObjectFactoryRegistry.h 13508 2008-08-27 15:51:38Z gpotts $
#ifndef ossimObjectFactoryRegistry_HEADER
#define ossimObjectFactoryRegistry_HEADER
#include <vector>

#include <ossim/base/ossimObject.h>

class OSSIMDLLEXPORT ossimObjectFactory;
class OSSIMDLLEXPORT ossimKeywordlist;
class OSSIMDLLEXPORT ossimString;

class OSSIMDLLEXPORT ossimObjectFactoryRegistry : public ossimObject
{
public:
   virtual ~ossimObjectFactoryRegistry();
   static ossimObjectFactoryRegistry* instance();
   /*!
    * Should use the registerFactory to add a factory to the list.
    * This method will call registerFactory.
    */
   bool                addFactory(ossimObjectFactory* factory);

   /*!
    * Registers a factory to the list.
    */
   bool                registerFactory(ossimObjectFactory* factory);
   void                unregisterFactory(ossimObjectFactory* factory);
   
   virtual ossimObject* createObject(const ossimString& name)const;
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   
   /*!
    * Returns a type list of all objects that can be instantiated
    * through the createObjectMethods above.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;

   /*!
    * returns a list of objects that are of the passed in
    * base type.  for example:
    *
    *  If I want all ossimImageSource objects then I call:
    *
    *  ossimObjectFactoryRegistry::instance()->getTypeNameList(l,
    *                                                          "ossimImageSource");
    */ 
   virtual void getTypeNameList(std::vector<ossimString>& typeList,
                                const ossimString& baseType)const;
   
protected:
   ossimObjectFactoryRegistry();//hide
   ossimObjectFactoryRegistry(const ossimObjectFactoryRegistry&):ossimObject(){}//hide
   void operator =(const ossimObjectFactoryRegistry&){}//hide
   
   std::vector<ossimObjectFactory*>   theFactoryList;

   ossimObjectFactory* findFactory(ossimObjectFactory* factory)const;

TYPE_DATA
};

extern "C"
{
  OSSIMDLLEXPORT void* ossimObjectFactoryRegistryGetInstance();
}
#endif
