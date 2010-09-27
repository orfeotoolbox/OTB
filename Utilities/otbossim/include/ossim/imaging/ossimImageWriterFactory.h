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
//  $Id: ossimImageWriterFactory.h 18002 2010-08-30 18:01:10Z gpotts $

#ifndef ossimImageWriterFactory_HEADER
#define ossimImageWriterFactory_HEADER
#include <ossim/imaging/ossimImageWriterFactoryBase.h>

class ossimImageWriter;
class ossimKeywordlist;

class ossimImageWriterFactory: public ossimImageWriterFactoryBase
{   
public:
   virtual ~ossimImageWriterFactory();
   static ossimImageWriterFactory* instance();
   
   ossimImageFileWriter *createWriterFromExtension(const ossimString& fileExtension)const;
   virtual ossimImageFileWriter* createWriter(const ossimKeywordlist& kwl,
                                              const char *prefix=0)const;
   virtual ossimImageFileWriter* createWriter(const ossimString& typeName)const;
   
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char *prefix=0)const;
   virtual ossimObject* createObject(const ossimString& typeName)const;
   
   virtual void getExtensions(std::vector<ossimString>& result)const;
   
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
   
   /*!
    * getImageTypeList.  This is the actual image type name.  So for
    * example, ossimTiffWriter has several image types.  Some of these
    * include TIFF_TILED, TIFF_TILED_BAND_SEPARATE ... etc.  The
    * ossimGdalWriter
    * may include GDAL_IMAGINE_HFA, GDAL_RGB_NITF, GDAL_JPEG20000, ... etc
    * A writer should be able to be instantiated by this name as well as a
    * class name
    */
   virtual void getImageTypeList(std::vector<ossimString>& imageTypeList)const;
   
   virtual void getImageFileWritersBySuffix(ossimImageWriterFactoryBase::ImageFileWriterList& result,
                                            const ossimString& ext)const;
   virtual void getImageFileWritersByMimeType(ossimImageWriterFactoryBase::ImageFileWriterList& result,
                                              const ossimString& mimeType)const;
protected:
   ossimImageWriterFactory() {}

   static ossimImageWriterFactory* theInstance;

	ossimImageFileWriter* createFromMimeType(const ossimString& mimeType)const;
   bool isImageTypeName(const ossimString& name)const;
};

#endif
