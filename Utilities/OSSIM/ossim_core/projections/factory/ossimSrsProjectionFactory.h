//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimSrsProjectionFactory.h,v 1.1 2005/09/08 16:49:27 gpotts Exp $

#ifndef ossimSrsProjectionFactory_HEADER
#define ossimSrsProjectionFactory_HEADER

#include "ossimProjectionFactoryBase.h"

class ossimProjection;
class ossimString;

class OSSIMDLLEXPORT ossimSrsProjectionFactory : public ossimProjectionFactoryBase
{
public:
   /*!
    * METHOD: instance()
    * Instantiates singleton instance of this class:
    */
   static ossimSrsProjectionFactory* instance();

   virtual ossimProjection* createProjection(const ossimFilename& filename,
                                             ossim_uint32 entryIdx)const;
   /*!
    * METHOD: create()
    * Attempts to create an instance of the projection specified by name.
    * Returns successfully constructed projection or NULL.
    */
   virtual ossimProjection* createProjection(const ossimString& name)const;
   virtual ossimProjection* createProjection(const ossimKeywordlist& kwl,
                                             const char* prefix = 0)const;

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

   /*!
    * METHOD: getList()
    * Returns list of all projections represented by this factory:
    */
   virtual list<ossimString> getList()const;

protected:
   ossimSrsProjectionFactory() {}
   static ossimSrsProjectionFactory*  theInstance;
 
};

#endif
