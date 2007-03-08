//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts (gpott@imagelinks.com)
//
// Description:  Contains class definition for the class
//               ImageHandlerRegistry.
//
//*******************************************************************
//  $Id: ossimImageHandlerRegistry.h 10103 2006-12-14 16:12:19Z gpotts $

#ifndef ossimImageHandlerRegistry_HEADER
#define ossimImageHandlerRegistry_HEADER

#include <vector>
#include <ossim/base/ossimObjectFactory.h>
#include <ossim/base/ossimRtti.h>
#include <ossim/imaging/ossimImageHandlerFactoryBase.h>
class ossimImageHandler;
class ossimFilename;
class ossimKeywordlist;

class OSSIMDLLEXPORT ossimImageHandlerRegistry : public ossimObjectFactory
{
public:
   virtual ~ossimImageHandlerRegistry();
   
   static ossimImageHandlerRegistry* instance();
   
   void addFactory(ossimImageHandlerFactoryBase* factory);

   /**
    * Method to add a factory to the list of this registry.
    *
    * @param factory Factory to add.
    *
    * @param pushToFrontFlag If true the factory is added to the front of the
    * list.  If false (default behavior) the factory is added to the end of
    * the list.
    */
   void registerFactory(ossimImageHandlerFactoryBase* factory,
                        bool pushToFrontFlag=false);
   
   void unregisterFactory(ossimImageHandlerFactoryBase* factory);
   bool findFactory(ossimImageHandlerFactoryBase* factory)const;
   
   ossimImageHandler* open(const ossimFilename& fileName)const;

   
   /*!
    *  Given a keyword list return a pointer to an ImageHandler.  Returns
    *  null if a valid handler cannot be found.
    */
   ossimImageHandler* open(const ossimKeywordlist& kwl, const char* prefix=0)const;

   /*!
    * Creates an object given a type name.
    */
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
   ossimImageHandlerRegistry();
   ossimImageHandlerRegistry(const ossimImageHandlerRegistry& rhs);
   const ossimImageHandlerRegistry&
      operator=(const ossimImageHandlerRegistry& rhs);

private:
   void clear();
   std::vector<ossimImageHandlerFactoryBase*>   theFactoryList;
   static ossimImageHandlerRegistry*            theInstance;

TYPE_DATA
};

extern "C"
{
 OSSIM_DLL  void* ossimImageHandlerRegistryGetInstance();
}
#endif
