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
//  $Id: ossimImageHandlerRegistry.h 19551 2011-05-07 16:36:34Z dburken $

#ifndef ossimImageHandlerRegistry_HEADER
#define ossimImageHandlerRegistry_HEADER 1

#include <ossim/base/ossimObjectFactory.h>
#include <ossim/base/ossimRtti.h>
#include <ossim/imaging/ossimImageHandlerFactoryBase.h>
#include <ossim/base/ossimFactoryListInterface.h>
#include <vector>

class ossimImageHandler;
class ossimFilename;
class ossimKeywordlist;

class OSSIMDLLEXPORT ossimImageHandlerRegistry : public ossimObjectFactory,
                                                public ossimFactoryListInterface<ossimImageHandlerFactoryBase, ossimImageHandler>
{
public:
   virtual ~ossimImageHandlerRegistry();
   
   static ossimImageHandlerRegistry* instance();
   

   /**
    * @brief open that takes a filename.
    * @param fileName File to open.
    * @param trySuffixFirst If true calls code to try to open by suffix first,
    * then goes through the list of available handlers. default=true.
    * @param openOverview If true image handler will attempt to open overview.
    * default = true
    * @return Pointer to image handler or null if cannot open.
    */
   ossimImageHandler* open(const ossimFilename& fileName,
                           bool trySuffixFirst=true,
                           bool openOverview=true)const;
   
   /*!
    *  Given a keyword list return a pointer to an ImageHandler.  Returns
    *  null if a valid handler cannot be found.
    */
   ossimImageHandler* open(const ossimKeywordlist& kwl, const char* prefix=0)const;

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
                                     const char* prefix=0)const;

   /**
    * openBySuffix will call the mthod getImageHandlersBySuffix and go through each handler to try and open the file.
    * This should be a faster open for we do not have to do a magic number compare on all prior files and keep opening and
    * closing files.
    * @param openOverview If true image handler will attempt to open overview.
    * default = true
    */
   virtual ossimRefPtr<ossimImageHandler> openBySuffix(const ossimFilename& file,
                                                       bool openOverview=true)const; 
   
   /**
    *
    * Will add to the result list any handler that supports the passed in extensions
    *
    */
   virtual void getImageHandlersBySuffix(ossimImageHandlerFactoryBase::ImageHandlerList& result,
                                         const ossimString& ext)const;
   /**
    *
    * Will add to the result list and handler that supports the passed in mime type
    *
    */
   virtual void getImageHandlersByMimeType(ossimImageHandlerFactoryBase::ImageHandlerList& result,
                                           const ossimString& mimeType)const;
   
   /*!
    * This should return the type name of all objects in all factories.
    * This is the name used to construct the objects dynamially and this
    * name must be unique.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const
   {
      getAllTypeNamesFromRegistry(typeList);
   }

   virtual void getSupportedExtensions(ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const;

   /**
    * @brief Prints list of readers and properties.
    * @param  out Stream to print to.
    * @return std::ostream&
    */
   std::ostream& printReaderProps(std::ostream& out) const;
   
protected:
   ossimImageHandlerRegistry();
   ossimImageHandlerRegistry(const ossimImageHandlerRegistry& rhs);
   const ossimImageHandlerRegistry&
      operator=(const ossimImageHandlerRegistry& rhs);

   //static ossimImageHandlerRegistry*            theInstance;

TYPE_DATA
};

extern "C"
{
 OSSIM_DLL  void* ossimImageHandlerRegistryGetInstance();
}
#endif
