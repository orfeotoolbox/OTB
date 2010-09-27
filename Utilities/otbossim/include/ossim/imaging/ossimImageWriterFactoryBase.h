//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimImageWriterFactoryBase.h 18011 2010-08-31 12:48:56Z dburken $
#ifndef ossimImageWriterFactoryBase_HEADER
#define ossimImageWriterFactoryBase_HEADER
#include <ossim/base/ossimObjectFactory.h>
#include <ossim/base/ossimString.h>
#include <ossim/imaging/ossimImageFileWriter.h>
#include <vector>
class ossimKeywordlist;

class OSSIM_DLL ossimImageWriterFactoryBase : public ossimObjectFactory
{   
public:
   typedef std::vector<ossimRefPtr<ossimImageFileWriter> > ImageFileWriterList;
   
   virtual ~ossimImageWriterFactoryBase() {}
   virtual ossimImageFileWriter* createWriter(const ossimKeywordlist& kwl,
                                          const char *prefix=0)const=0;
   virtual ossimImageFileWriter* createWriter(const ossimString& typeName)const=0;

   virtual void getExtensions(std::vector<ossimString>& result)const=0;
   
   /*!
    * getImageTypeList.  This is the actual image type name.  So for
    * example, ossimTiffWriter has several image types.  Some of these
    * include tiff_tiled, tiff_tiled_band_separate ... etc.  The ossimGdalWriter
    * may include gdal_imagine_hfa,gdal_nitf_rgb_band_separate, ... etc
    * A writer should be able to be instantiated by this name as well as a class name
    */
   virtual void getImageTypeList(std::vector<ossimString>& imageTypeList)const=0;
   
   virtual void getImageFileWritersBySuffix(ossimImageWriterFactoryBase::ImageFileWriterList& result,
                                            const ossimString& ext)const;

   virtual void getImageFileWritersByMimeType(ossimImageWriterFactoryBase::ImageFileWriterList& result,
                                              const ossimString& mimeType)const;
   
TYPE_DATA
};

#endif
