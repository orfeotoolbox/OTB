//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimProjectionFactoryRegistry.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimProjectionFactoryRegistry_HEADER
#define ossimProjectionFactoryRegistry_HEADER
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimObjectFactory.h>
#include <ossim/base/ossimFactoryListInterface.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimProjectionFactoryBase.h>

class ossimImageHandler;
class OSSIM_DLL ossimProjectionFactoryRegistry : public ossimObjectFactory,
                                                 public ossimFactoryListInterface<ossimProjectionFactoryBase,
                                                                                 ossimProjection>
{
public:

   virtual ~ossimProjectionFactoryRegistry();
   
   static ossimProjectionFactoryRegistry* instance();

   
   ossimProjection* createProjection(const ossimFilename& filename,
                                     ossim_uint32 entryIdx)const;
   ossimProjection* createProjection(const ossimString& name)const
   {
      return createNativeObjectFromRegistry(name);
   }
   ossimProjection* createProjection(const ossimKeywordlist& kwl,
                                     const char* prefix=NULL)const;
   
   virtual ossimProjection* createProjection(ossimImageHandler* handler)const;
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
                                     const char* prefix=0)const
   {
      return createObjectFromRegistry(kwl, prefix);
   }
   
   /**
    * This should return the type name of all objects in all factories.
    * This is the name used to construct the objects dynamially and this
    * name must be unique.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const
   {
      getAllTypeNamesFromRegistry(typeList);
   }
      
protected:
   ossimProjectionFactoryRegistry();
   ossimProjectionFactoryRegistry(const ossimProjectionFactoryRegistry& rhs);
   void operator = (const ossimProjectionFactoryRegistry&);

   static ossimProjectionFactoryRegistry* m_instance;
   void initializeDefaults();
};

extern "C"
{
  OSSIMDLLEXPORT void* ossimProjectionFactoryRegistryGetInstance();
}

#endif
