//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Oscar Kramer
//
// Description: Factory for all ossim sensor models.
// 
//*******************************************************************
//  $Id: ossimSensorModelFactory.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimSensorModelFactory_HEADER
#define ossimSensorModelFactory_HEADER
#include <ossim/projection/ossimProjectionFactoryBase.h>

class ossimProjection;
class ossimString;
class ossimFilename;

class ossimSensorModelFactory : public ossimProjectionFactoryBase
{
public:
   /*!
    * METHOD: instance()
    * For accessing static instance of concrete factory.
    */
   static ossimSensorModelFactory* instance();
   
   virtual ossimProjection* createProjection(const ossimFilename& filename,
                                             ossim_uint32 entryIdx)const;
   /*!
    * METHOD: create()
    * Attempts to create an instance of the Product given a Specifier or
    * keywordlist. Returns successfully constructed product or NULL.
    */
   virtual ossimProjection* createProjection(const ossimString& spec)     const;
   virtual ossimProjection* createProjection(const ossimKeywordlist& kwl,
                                             const char* prefix = 0)      const;

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
    * Returns name list of all products represented by this factory.
    */
   virtual std::list<ossimString> getList() const;

protected:
   ossimSensorModelFactory() {}
   
   static ossimSensorModelFactory*  theInstance;
   bool isNitf(const ossimFilename& filename)const;
   bool isLandsat(const ossimFilename& filename)const;
   bool isTileMap(const ossimFilename& filename) const;
   void findCoarseGrid(ossimFilename& result,
                       const ossimFilename& geomFile)const;
};

#endif
