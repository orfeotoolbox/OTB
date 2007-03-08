//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  David Burken
//
// Description:
//
// Contains class declaration for ossimImageHandlerFactoryMaker.
//*******************************************************************
//  $Id: ossimImageHandlerFactory.h 10103 2006-12-14 16:12:19Z gpotts $

#ifndef ossimImageHandlerFactory_HEADER
#define ossimImageHandlerFactory_HEADER
#include <ossim/imaging/ossimImageHandlerFactoryBase.h>

#include <ossim/base/ossimString.h>

class ossimImageHandler;
class ossimFilename;
class ossimKeywordlist;

//*******************************************************************
// CLASS:  ossimImageHandlerFactory
//*******************************************************************
class OSSIMDLLEXPORT ossimImageHandlerFactory : public ossimImageHandlerFactoryBase
{
public:
   virtual ~ossimImageHandlerFactory();
   static ossimImageHandlerFactory* instance();
   
   virtual ossimImageHandler* open(const ossimFilename& fileName)const;
   virtual ossimImageHandler* open(const ossimKeywordlist& kwl,
                                   const char* prefix=0)const;

   
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
   virtual void getSupportedExtensions(ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const;
  
protected:
   ossimImageHandlerFactory(){}
   ossimImageHandlerFactory(const ossimImageHandlerFactory&){}
   void operator = (const ossimImageHandlerFactory&){}

   static ossimImageHandlerFactory* theInstance;

TYPE_DATA
};

#endif
