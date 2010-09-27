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
//  $Id: ossimImageHandlerFactory.h 18002 2010-08-30 18:01:10Z gpotts $

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
   virtual void getImageHandlersBySuffix(ossimImageHandlerFactoryBase::ImageHandlerList& result, const ossimString& ext)const;
   virtual void getImageHandlersByMimeType(ossimImageHandlerFactoryBase::ImageHandlerList& result, const ossimString& mimeType)const;
   
protected:

   /**
    * @brief Open method that looks at extension e.g. "tif", "jpg" to select
    * class to open.
    *
    * This is to be called by standard open before just going down the list
    * of classes the brute force hard way.
    * 
    * @param fileName The filename to open.
    * 
    * @return Pointer to image handler or null if nothing opened.
    */
//   virtual ossimImageHandler* openFromExtension(
//      const ossimFilename& fileName) const;
  
   ossimImageHandlerFactory(){}
   ossimImageHandlerFactory(const ossimImageHandlerFactory&){}
   void operator = (const ossimImageHandlerFactory&){}

   static ossimImageHandlerFactory* theInstance;

TYPE_DATA
};

#endif
