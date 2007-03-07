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
//  $Id: ossimImageWriterFactoryRegistry.h,v 1.13 2004/12/10 16:46:37 gpotts Exp $

#ifndef ossimImageWriterFactoryRegistry_HEADER
#define ossimImageWriterFactoryRegistry_HEADER
#include "base/factory/ossimObjectFactory.h"

#include <vector>

#include "base/common/ossimCommon.h"

class ossimImageFileWriter;
class ossimImageWriterFactoryBase;
class ossimKeywordlist;

class OSSIMDLLEXPORT ossimImageWriterFactoryRegistry : public ossimObjectFactory
{   
public:
   static ossimImageWriterFactoryRegistry* instance();
   bool addFactory(ossimImageWriterFactoryBase*);
   bool registerFactory(ossimImageWriterFactoryBase* factory);   
   void unregisterFactory(ossimImageWriterFactoryBase* factory);
   bool findFactory(ossimImageWriterFactoryBase* factory)const;

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
   
protected:
   ossimImageWriterFactoryRegistry();
   ossimImageWriterFactoryRegistry(const ossimImageWriterFactoryRegistry&);
   void operator=(const ossimImageWriterFactoryRegistry&);
   
   std::vector<ossimImageWriterFactoryBase*>  theFactoryList;
   static ossimImageWriterFactoryRegistry*    theInstance;
};

extern "C"
{
  OSSIMDLLEXPORT void* ossimImageWriterFactoryRegistryGetInstance();
}
#endif
