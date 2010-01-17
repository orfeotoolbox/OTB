//----------------------------------------------------------------------------
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//----------------------------------------------------------------------------
// $Id: ossimImageMetaDataWriterRegistry.h 13508 2008-08-27 15:51:38Z gpotts $

#ifndef ossimImageMetaDataWriterRegistry_HEADER
#define ossimImageMetaDataWriterRegistry_HEADER
#include <ossim/imaging/ossimImageMetaDataWriterFactoryBase.h>
#include <ossim/base/ossimRefPtr.h>

class ossimMetadataFileWriter;

class OSSIMDLLEXPORT ossimImageMetaDataWriterRegistry :
   public ossimImageMetaDataWriterFactoryBase
{
public:
   ~ossimImageMetaDataWriterRegistry();
   
   static ossimImageMetaDataWriterRegistry* instance();
   
   void registerFactory(ossimImageMetaDataWriterFactoryBase* factory);
   void unregisterFactory(ossimImageMetaDataWriterFactoryBase* factory);
   bool findFactory(ossimImageMetaDataWriterFactoryBase* factory)const;
   /**
    * Creates an object given a type name.
    */
   virtual ossimObject* createObject(const ossimString& typeName)const;

   /**
    * Creates and object given a keyword list.
    */
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;

   /**
    * Creates a writer from either a class name or a string representing the
    * meta data type like "tiff_world_file".
    *
    * @return ossimRefPtr<ossimMetadataFileWriter>
    *
    * @note Return can have a null pointer if type is not found in a factory so
    * the caller should always check the pointer like:
    * ossimRefPtr<ossimMetadataFileWriter> mw =
    *    ossimImageMetaDataWriterRegistry::instance("ossim_readme");
    * if (!mw.valid())
    * {
    *    // not in factory
    * }
    */
   virtual ossimRefPtr<ossimMetadataFileWriter> createWriter(
      const ossimString& type)const;
   
   /**
    * This should return the type name of all objects in all factories.
    * This is the name used to construct the objects dynamially and this
    * name must be unique.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;

   /**
    * Initializes list with meta data types from all registered
    * metadata factories.
    * 
    * This is the actual image type name like:
    *   ossim_geometry
    *   envi_header
    *   tiff_world_file
    *   jpeg_world_file
    *   
    * @param metadatatypeList stl::vector<ossimString> list to append to.
    *
    * @note Since this is the master registry for all meta data factories
    * this method starts by clearing the metadatatypeList.
    */
   virtual void getMetadatatypeList(
      std::vector<ossimString>& metadatatypeList)const;
   
protected:
   ossimImageMetaDataWriterRegistry();

   ossimImageMetaDataWriterRegistry(
      const ossimImageMetaDataWriterRegistry& rhs);
   
   const ossimImageMetaDataWriterRegistry& operator=(
      const ossimImageMetaDataWriterRegistry &rhs);

  // static ossimImageMetaDataWriterRegistry* theInstance;
   std::vector<ossimImageMetaDataWriterFactoryBase*> theFactoryList;
};


extern "C"
{
   OSSIMDLLEXPORT void* ossimImageMetaDataWriterRegistryGetInstance();
}
#endif
