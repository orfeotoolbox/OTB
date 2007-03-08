//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimProjectionFactoryRegistry.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimProjectionFactoryRegistry_HEADER
#define ossimProjectionFactoryRegistry_HEADER
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimObjectFactory.h>

#include <vector>
#include <list>
class ossimProjectionFactoryBase;
class ossimProjection;

class OSSIMDLLEXPORT ossimProjectionFactoryRegistry : public ossimObjectFactory
{
public:

   virtual ~ossimProjectionFactoryRegistry();
   
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

   /**
    * Method to add a factory to the list of this registry.
    *
    * @param factory Factory to add.
    *
    * @param pushToFrontFlag If true the factory is added to the front of the
    * list.  If false (default behavior) the factory is added to the end of
    * the list.
    *
    * @return true on success, false on error.
    */
   bool registerFactory(ossimProjectionFactoryBase* factory,
                        bool pushToFrontFlag=false);
   
   void unregisterFactory(ossimProjectionFactoryBase* factory);
   bool findFactory(ossimProjectionFactoryBase* factory)const;
   
   std::list<ossimString> getList()const;
   
protected:
   ossimProjectionFactoryRegistry();
   ossimProjectionFactoryRegistry(const ossimProjectionFactoryRegistry& rhs);
   void operator = (const ossimProjectionFactoryRegistry&);

   static ossimProjectionFactoryRegistry*   theInstance;
   std::vector<ossimProjectionFactoryBase*> theFactoryList;

   void initializeDefaults();
};

extern "C"
{
  OSSIMDLLEXPORT void* ossimProjectionFactoryRegistryGetInstance();
}

#endif
