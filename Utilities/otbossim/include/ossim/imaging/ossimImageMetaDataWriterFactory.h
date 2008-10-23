//*******************************************************************
// Copyright (C) 2003 Storage Area Networks, Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Kenneth Melero (kmelero@sanz.com)
//
//*******************************************************************
//  $Id: ossimImageMetaDataWriterFactory.h 9094 2006-06-13 19:12:40Z dburken $

#ifndef ossimImageMetaDataWriterFactory_HEADER
#define ossimImageMetaDataWriterFactory_HEADER

#include <ossim/imaging/ossimImageMetaDataWriterFactoryBase.h>
#include <ossim/base/ossimRefPtr.h>

class ossimMetadataFileWriter;

class OSSIMDLLEXPORT ossimImageMetaDataWriterFactory:
   public ossimImageMetaDataWriterFactoryBase
{
public:
   ~ossimImageMetaDataWriterFactory();

   static ossimImageMetaDataWriterFactory* instance();
   virtual ossimObject* createObject(const ossimString& typeName)const;
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
    * Satisfies pure virtual from ossimImageMetaDataWriterFactoryBase.
    * 
    * Appends the meta data writers types from each of the supported meta
    * data writers in this factor to the "metadatatypeList".
    * 
    * This is the actual metadata type name like:
    *   ossim_geometry
    *   envi_header
    *   tiff_world_file
    *   jpeg_world_file
    *   
    * @param metadatatypeList stl::vector<ossimString> List to append to.
    */
   virtual void getMetadatatypeList(
      std::vector<ossimString>& metadatatypeList) const;
 
protected:
   static ossimImageMetaDataWriterFactory* theInstance;

   ossimImageMetaDataWriterFactory();

   ossimImageMetaDataWriterFactory(const ossimImageMetaDataWriterFactory& rhs);

   const ossimImageMetaDataWriterFactory& operator =(
      const ossimImageMetaDataWriterFactory& rhs);
   
TYPE_DATA 
};

#endif
