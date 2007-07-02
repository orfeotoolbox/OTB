//----------------------------------------------------------------------------
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//----------------------------------------------------------------------------
// $Id: ossimImageMetaDataWriterFactoryBase.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimImageMetaDataWriterFactoryBase_HEADER
#define ossimImageMetaDataWriterFactoryBase_HEADER
#include <ossim/base/ossimObjectFactory.h>
#include <ossim/base/ossimRefPtr.h>

class ossimMetadataFileWriter;

class ossimImageMetaDataWriterFactoryBase : public ossimObjectFactory
{
public:
   /**
    * Pure virtual.  All ossimImageMetaDataWriterFactories must implement.
    * 
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
      const ossimString& type)const=0;

   /**
    * Pure virtual.  All ossimImageMetaDataWriterFactories must implement.
    * 
    * Appends the meta data writers types to the "metadatatypeList".
    * 
    * This is the actual metadata type name like:
    *   ossim_geometry
    *   envi_header
    *   tiff_world_file
    *   jpeg_world_file
    *   
    * @param metadatatypeList stl::vector<ossimString> List to append to.
    *
    * @note All writers should append to the list, not, clear it and then add
    * their types.
    */
   virtual void getMetadatatypeList(
      std::vector<ossimString>& metadatatypeList) const=0;

protected:
   ossimImageMetaDataWriterFactoryBase();
   
   ossimImageMetaDataWriterFactoryBase(
      const ossimImageMetaDataWriterFactoryBase&);
   
   const ossimImageMetaDataWriterFactoryBase& operator=(
      const ossimImageMetaDataWriterFactoryBase&);

TYPE_DATA
};
#endif
