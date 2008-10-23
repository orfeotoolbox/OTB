//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimSrsProjectionFactory.h 12081 2007-11-26 21:44:18Z dburken $

#ifndef ossimSrsProjectionFactory_HEADER
#define ossimSrsProjectionFactory_HEADER

#include <ossim/projection/ossimProjectionFactoryBase.h>

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

protected:
   ossimSrsProjectionFactory() {}
   static ossimSrsProjectionFactory*  theInstance;
 
};

#endif
