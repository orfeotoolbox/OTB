//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimObjectFactoryRegistry.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimObjectFactoryRegistry_HEADER
#define ossimObjectFactoryRegistry_HEADER
#include <vector>

#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimObjectFactory.h>
#include <ossim/base/ossimFactoryListInterface.h>

class ossimKeywordlist;
class ossimString;
class OSSIMDLLEXPORT ossimObjectFactoryRegistry : public ossimObject,
                                                  public ossimFactoryListInterface<ossimObjectFactory, ossimObject>
{
public:
   virtual ~ossimObjectFactoryRegistry();
   static ossimObjectFactoryRegistry* instance();
   
   virtual ossimObject* createObject(const ossimString& name)const
   {
      return createObjectFromRegistry(name);
   }
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const
   {
      return createObjectFromRegistry(kwl, prefix);
   }
   
   /*!
    * Returns a type list of all objects that can be instantiated
    * through the createObjectMethods above.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const
   {
      getAllTypeNamesFromRegistry(typeList);
   }

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
   
//   std::vector<ossimObjectFactory*>   theFactoryList;

//   ossimObjectFactory* findFactory(ossimObjectFactory* factory)const;

TYPE_DATA
};

extern "C"
{
  OSSIMDLLEXPORT void* ossimObjectFactoryRegistryGetInstance();
}
#endif
