//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Frank Warmerdam (warmerda@home.com)
//
//*******************************************************************
//  $Id: ossimImageWriterFactoryRegistry.h 19907 2011-08-05 19:55:46Z dburken $

#ifndef ossimImageWriterFactoryRegistry_HEADER
#define ossimImageWriterFactoryRegistry_HEADER
#include <ossim/base/ossimObjectFactory.h>
#include <ossim/imaging/ossimImageWriterFactoryBase.h>
#include <ossim/base/ossimFactoryListInterface.h>
#include <vector>
#include <iosfwd>

class ossimImageFileWriter;
class ossimKeywordlist;

class OSSIMDLLEXPORT ossimImageWriterFactoryRegistry :
   public ossimObjectFactory,
   public ossimFactoryListInterface<ossimImageWriterFactoryBase, ossimImageFileWriter>
{   
public:
   static ossimImageWriterFactoryRegistry* instance();

   ossimImageFileWriter *createWriter(const ossimFilename& filename)const;
   ossimImageFileWriter *createWriterFromExtension(const ossimString& fileExtension)const;
   ossimImageFileWriter *createWriter(const ossimKeywordlist &kwl,
                                      const char *prefix=0)const;
   ossimImageFileWriter* createWriter(const ossimString& typeName)const;
   
   ossimObject* createObject(const ossimKeywordlist &kwl,
                             const char *prefix=0)const;

   ossimObject* createObject(const ossimString& typeName)const;
   
   /**
    * getTypeNameList.  This should return the class type of the object being
    * used to perform the writting.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
   
   /**
    * getImageTypeList.  This is the actual image type name.  So for
    * example, ossimTiffWriter has several image types.  Some of these
    * include TIFF_TILED, TIFF_TILED_BAND_SEPARATE ... etc.
    * The ossimGdalWriter
    * may include GDAL_IMAGINE_HFA, GDAL_RGB_NITF, GDAL_JPEG20000, ... etc
    * A writer should be able to be instantiated by this name as well as a
    * class name
    */
   virtual void getImageTypeList(std::vector<ossimString>& imageTypeList)const;

   virtual void getImageFileWritersBySuffix(ossimImageWriterFactoryBase::ImageFileWriterList& result,
                                            const ossimString& ext)const;
   virtual void getImageFileWritersByMimeType(ossimImageWriterFactoryBase::ImageFileWriterList& result,
                                              const ossimString& mimeType)const;
   /**
    * @brief Prints list of writers from getImageTypeList.
    * @param  out Stream to print to.
    * @return std::ostream&
    */
   std::ostream& printImageTypeList(std::ostream& out)const;

   /**
    * @brief Prints list of writers from getImageTypeList.
    * @param  out Stream to print to.
    * @return std::ostream&
    */
   std::ostream& printWriterProps(std::ostream& out)const;
   
protected:
   ossimImageWriterFactoryRegistry();
   ossimImageWriterFactoryRegistry(const ossimImageWriterFactoryRegistry&);
   void operator=(const ossimImageWriterFactoryRegistry&);
   
   static ossimImageWriterFactoryRegistry*    theInstance;
};

extern "C"
{
  OSSIMDLLEXPORT void* ossimImageWriterFactoryRegistryGetInstance();
}
#endif
