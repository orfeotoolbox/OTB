//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimProjectionFactoryRegistry.h,v 1.13 2005/09/08 17:30:52 gpotts Exp $
#ifndef ossimProjectionFactoryRegistry_HEADER
#define ossimProjectionFactoryRegistry_HEADER
#include <base/data_types/ossimString.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/factory/ossimObjectFactory.h>

#include <vector>
#include <list>
class ossimProjectionFactoryBase;
class ossimProjection;

class OSSIMDLLEXPORT ossimProjectionFactoryRegistry : public ossimObjectFactory
{
public:
   static ossimProjectionFactoryRegistry* instance();

   ossimProjection* createProjection(const ossimFilename& filename,
                                     ossim_uint32 entryIdx)const;
   ossimProjection* createProjection(const ossimString& name)const;
   ossimProjection* createProjection(const ossimKeywordlist& kwl,
                                     const char* prefix=NULL)const;

   virtual ossimObject* createObject(const ossimString& typeName)const;

   /**
    * Creates and object given a keyword list.
    */
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   
   /**
    * This should return the type name of all objects in all factories.
    * This is the name used to construct the objects dynamially and this
    * name must be unique.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;

   bool registerFactory(ossimProjectionFactoryBase* factory);
   void unregisterFactory(ossimProjectionFactoryBase* factory);
   bool findFactory(ossimProjectionFactoryBase* factory)const;
   
   std::list<ossimString> getList()const;
   
protected:
   ossimProjectionFactoryRegistry(){}
   ossimProjectionFactoryRegistry(const ossimProjectionFactoryRegistry& rhs):ossimObjectFactory(rhs){}
   void operator = (const ossimProjectionFactoryRegistry&){}

   static ossimProjectionFactoryRegistry*   theInstance;
   std::vector<ossimProjectionFactoryBase*> theFactoryList;

   void initializeDefaults();
};

extern "C"
{
  OSSIMDLLEXPORT void* ossimProjectionFactoryRegistryGetInstance();
}

#endif
