//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Frank Warmerdam (warmerdam@pobox.com)
//
// Description:
//
// Contains class declaration for ossimImageWriter
//*******************************************************************
//  $Id: ossimImageFileWriter.h,v 1.39 2006/01/06 16:46:46 dburken Exp $

#ifndef ossimImageFileWriter_HEADER
#define ossimImageFileWriter_HEADER
#include <fstream>
#include <imaging/ossimImageWriter.h>
#include <base/data_types/ossimFilename.h>
#include <base/common/events/ossimConnectableObjectListener.h>
#include <base/common/ossimProcessInterface.h>
#include <base/common/events/ossimObjectEvents.h>
#include <base/common/events/ossimProcessProgressEvent.h>
#include <base/common/ossimViewController.h>

/**
 * Pure virtual base class for image file writers.
 *
 * There is normally one subclass of this class for each format supported
 * for writing.  This class roughly corresponds to the ImageHandler class
 * used for reading images.  Format specific ossimImageWriter's are normally
 * instantiated by the ossimImageWriterFactory::createWriterInstance() method.
 * ossimImageWriters should not be directly instantiated by application code.
 */

class OSSIMDLLEXPORT ossimImageFileWriter
   :  public ossimImageWriter,
      public ossimProcessInterface,
      public ossimConnectableObjectListener
{   
public:

   /**
    * The writer will own the sequencer.
    */
   ossimImageFileWriter(const ossimFilename& filename = ossimFilename(),
                        ossimImageSource* inputSource=0,
                        ossimObject* owner=0);
   
   virtual ~ossimImageFileWriter();
   
   virtual ossimObject* getObject();

   virtual const ossimObject* getObject()const;

   /**
    * void getImageTypeList(std::vector<ossimString>& imageTypeList)const
    *
    * pure virtual
    * 
    * Appends the writers image types to the "imageTypeList".
    * 
    * This is the actual image type name.  So for
    * example, ossimTiffWriter has several image types.  Some of these
    * include TIFF_TILED, TIFF_TILED_BAND_SEPARATE ... etc.
    * The ossimGdalWriter
    * may include GDAL_IMAGINE_HFA, GDAL_RGB_NITF, GDAL_JPEG20000, ... etc.
    *
    * @param imageTypeList stl::vector<ossimString> list to append to.
    *
    * @note All writers should append to the list, not, clear it and then add
    * their types.
    */
   virtual void getImageTypeList(std::vector<ossimString>& imageTypeList)const=0;

   /**
    * bool hasImageType(const ossimString& imageType) const
    *
    * @param imageType string representing image type.
    *
    * @return true if "imageType" is supported by writer.
    */
   virtual bool hasImageType(const ossimString& imageType) const;

   /**
    * Sets the output image tiling size if supported by the writer.  If not
    * supported this simply sets the sequencer(input) tile size.
    */
   virtual void setTileSize(const ossimIpt& tileSize);

   /**
    * Will write an envi header file.  If "theFilename" is "foo.tif"
    * then this will write out "foo.hdr".
    */
   virtual bool writeEnviHeaderFile() const;

   /**
    * Will write an external geometry file.  If "theFilename" is "foo.tif"
    * then this will write out "foo.geom".
    */
   virtual bool writeExternalGeometryFile() const;

   /**
    * Will write an fgdc file.  If "theFilename" is "foo.tif"
    * then this will write out "foo.xml".
    */
   virtual bool writeFgdcFile() const;

   /**
    * Returns true on success, false on error.
    */
   virtual bool writeHistogramFile() const;

   /**
    * Will write a jpeg world file.  If "theFilename" is "foo.tif"
    * then this will write out "foo.jpw".
    */
   virtual bool writeJpegWorldFile() const;

   /**
    * Write out an ossim overview file from the source_file.
    * If source file is "foo.tif", this will create a "foo.ovr" in
    * the same directory.
    *
    * @param source_file The file to create overview from.
    *
    * @param tiff_compress_type valid types:
    * - COMPRESSION_JPEG
    * - COMPRESSION_DEFLATE
    * - COMPRESSION_PACKBITS
    * - COMPRESSION_NONE=1 (default)
    *
    * @param jpeg_compress_quality Compression quality.  Only valid with
    * COMPRESSION_JPEG. Range 1 to 100 with 100 being the best.
    * default = 75
    *
    * @return true on success, false on error.
    */
   virtual bool writeOverviewFile(ossim_uint16 tiff_compress_type = 1,
                                  ossim_int32 jpeg_compress_quality = 75)const;

   /**
    * Will write a readme file.  If "theFilename" is "foo.tif"
    * then this will write out "foo_readme.txt".
    */
   virtual bool writeReadmeFile() const;

   /**
    * Will write a readme file.  If "theFilename" is "foo.tif"
    * then this will write out "foo.tfw".
    */
   virtual bool writeTiffWorldFile() const;

   /**
    * Convenience method that calls meta data write methods that are flagged
    * to be called.  Attempts to write all files flagged to be written even
    * if one file writer returns false.
    *
    * Currently calls these methods if flagged:
    * writeEnviHeaderFile
    * writeExternalGeometryFile
    * writeFgdcFile
    * writeJpegWorldFile
    * writeReadmeFile
    * writeTiffWorldFile
    * writeOverviewFile
    * writeHistogramFile
    *
    * @return true if all files flagged are written, false if not.
    */
   virtual bool writeMetaDataFiles() const;

   virtual void setAreaOfInterest(const ossimIrect& inputRect);

   virtual void changeSequencer(ossimImageSourceSequencer* sequencer);

   virtual ossimImageSourceSequencer* getSequencer();

   virtual void initialize();

   /**
    * Calls:
    * writeFile()
    * writeMetaDataFiles()
    *
    * @return true on success, false on error.
    */
   virtual bool execute();

   virtual void setPercentComplete(double percentComplete);

   virtual void  setOutputImageType(ossim_int32 type);
   virtual void  setOutputImageType(const ossimString& type);
   virtual ossim_int32 getOutputImageType() const;
   virtual ossimString getOutputImageTypeString() const;
   
   virtual void setOutputName(const ossimString& outputName);

   virtual void setFilename(const ossimFilename& file);

   virtual const ossimFilename& getFilename()const;

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   

   virtual bool canConnectMyInputTo(ossim_int32 inputIndex,
                                    const ossimConnectableObject* object)const;
   
   virtual void disconnectInputEvent(ossimConnectionEvent& event);
   virtual void connectInputEvent(ossimConnectionEvent& event);
   virtual void propertyEvent(ossimPropertyEvent& event);

   /**
   * Control flags...
   */

   virtual bool         getWriteImageFlag()            const;
   virtual bool         getWriteHistogramFlag()        const;
   virtual bool         getWriteOverviewFlag()         const;

   virtual bool         getWriteEnviHeaderFlag()       const;
   virtual bool         getWriteExternalGeometryFlag() const;
   virtual bool         getWriteFgdcFlag()             const;
   virtual bool         getWriteJpegWorldFileFlag()    const;
   virtual bool         getWriteReadmeFlag()           const;
   virtual bool         getWriteTiffWorldFileFlag()    const;

   virtual void         setWriteImageFlag(bool flag);
   virtual void         setWriteOverviewFlag(bool flag);
   virtual void         setWriteHistogramFlag(bool flag);

   virtual void         setWriteEnviHeaderFlag(bool flag);
   virtual void         setWriteExternalGeometryFlag(bool flag);
   virtual void         setWriteFgdcFlag(bool flag);
   virtual void         setWriteJpegWorldFile(bool flag);
   virtual void         setWriteReadme(bool flag);
   virtual void         setWriteTiffWorldFile(bool flag);
   
   virtual ossim_uint16 getOverviewCompressType() const;
   virtual ossim_int32  getOverviewJpegCompressQuality() const;

   virtual void         setOverviewCompressType(ossim_uint16 type);
   virtual void         setOverviewJpegCompressQuality(ossim_int32 quality);

   
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   /**
    * Overrides base "addListener" this will capture the pointer and then call
    * the base class "addListener".  If overview building is enabled, the
    * listener will be passed to the overview builder::addListener, and
    * subsequently removed from the overview builder after the build overview
    * call is complete.  This it transparent to the caller of this method.
    *
    * @param listener The listener to add.
    *
    * @return Currently always returns true.
    *
    * @note Callers still should call "removeListener" after the writing
    * process is complete.
    *
    * @note If called more than once, theProgressListener will contain
    * the latest listener.
    */
   virtual bool addListener(ossimListener* listener);

   /**
    * Overrides base "removeListener".  Calls base "removeListener which will
    * find and removes the listener.
    * If "theProgressListener" pointer matches listener "theProgressListener"
    * will be set to NULL.
    *
    * @param listener The listener to remove.
    *
    * @return Currently always returns true.
    */
   virtual bool removeListener(ossimListener* listener);
   
protected:
   /**
    * Write out the file.
    * @return true on success, false on error.
    */
   virtual bool writeFile() = 0;
   
   ossimImageSourceSequencer* theInputConnection;
   ossimViewController*       theViewController;
   ossimListener*             theProgressListener;
   ossimFilename              theFilename;
   ossimString                theOutputImageType;
   bool                       theWriteImageFlag;
   bool                       theWriteOverviewFlag;
   bool                       theWriteHistogramFlag;
   ossim_uint16               theOverviewCompressType;
   ossim_int32                theOverviewJpegCompressQuality;

   /**
    * External meta data writer flags.  Not really part of an image writer, put
    * here for convenience.  If true the writer will do the connection work
    * for you.
    */
   bool                       theWriteEnviHeaderFlag;
   bool                       theWriteExternalGeometryFlag;
   bool                       theWriteFgdcFlag;
   bool                       theWriteJpegWorldFileFlag;
   bool                       theWriteReadmeFlag;
   bool                       theWriteTiffWorldFileFlag;

TYPE_DATA
};

#endif
