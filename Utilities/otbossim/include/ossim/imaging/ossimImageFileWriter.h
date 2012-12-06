//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Frank Warmerdam (warmerdam@pobox.com)
//
// Description:
//
// Contains class declaration for ossimImageWriter
//*******************************************************************
//  $Id: ossimImageFileWriter.h 21963 2012-12-04 16:28:12Z dburken $

#ifndef ossimImageFileWriter_HEADER
#define ossimImageFileWriter_HEADER
#include <fstream>
#include <ossim/base/ossimIoStream.h>
#include <ossim/imaging/ossimImageWriter.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimProcessInterface.h>
#include <ossim/base/ossimObjectEvents.h>
#include <ossim/base/ossimProcessProgressEvent.h>
#include <ossim/base/ossimViewController.h>

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
    * Returns a 3-letter extension from the image type descriptor 
    * (theOutputImageType) that can be used for image file extensions.
    *
    * @param imageType string representing image type.
    *
    * @return the 3-letter string extension.
    */
   virtual ossimString getExtension() const;

   /**
    * Examples of writers that always generate internal
    * overviews are ossim_kakadu_jp2 and ossim_kakadu_nitf_j2k.
    *
    * @return true if the output of the writer will have
    * internal overviews. The default is false. 
    */
   virtual bool getOutputHasInternalOverviews( void ) const;

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
   virtual bool writeEnviHeaderFile() ;

   /**
    * Will write an ER Mapper header file.  If "theFilename" is "foo.tif"
    * then this will write out "foo.hdr".
    */
   virtual bool writeErsHeaderFile() ;

   /**
    * Will write an external geometry file.  If "theFilename" is "foo.tif"
    * then this will write out "foo.geom".
    */
   virtual bool writeExternalGeometryFile() ;

   /**
    * Will write an fgdc file.  If "theFilename" is "foo.tif"
    * then this will write out "foo.xml".
    */
   virtual bool writeFgdcFile() ;

   /**
    * Returns true on success, false on error.
    */
   virtual bool writeHistogramFile() ;

   /**
    * Will write a jpeg world file.  If "theFilename" is "foo.tif"
    * then this will write out "foo.jpw".
    */
   virtual bool writeJpegWorldFile() ;

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
                                  ossim_int32 jpeg_compress_quality = 75,
                                  bool includeR0 = false );

   /**
    * Will write a readme file.  If "theFilename" is "foo.tif"
    * then this will write out "foo_readme.txt".
    */
   virtual bool writeReadmeFile() ;

   /**
    * Will write a world file.  If "theFilename" is "foo.tif"
    * then this will write out "foo.tfw".
    */
   virtual bool writeTiffWorldFile() ;

   /**
    * Will write a world file.  The extension will be derived from the
    * image extension.
    *
    * Currently handles the following image extentions:
    * 
    * "tif" or "tiff" -> "tfw"
    * "jpg" of "jpeg" -> "jpw"
    * "png" -> "pgw"
    * "sid" -> "sdw"
    *
    * If extension does not match one of the above no file will be written
    * and false will be returned.
    *
    * @return true on success false on error.
    */
   virtual bool writeWorldFile() ;

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
   virtual bool writeMetaDataFiles() ;

   virtual void setAreaOfInterest(const ossimIrect& inputRect);

   /**
    * @brief Sets the sequencer and connects it to the input of this.
    *
    * This does not set the area of interest.  Note stored as an ossimRefPtr.
    *
    * @param sequencer Pointer to a sequencer.
    */
   virtual void changeSequencer(ossimImageSourceSequencer* sequencer);

   virtual ossimImageSourceSequencer* getSequencer();

   /**
    * @brief Initialize method.
    *
    * Calls theInputConnection->initialize() then syncs area of interest(AOI)
    * with ossimImageWriter::theAreaOfInterest taking precedence over
    * ossimImageSourceSequencer::theAreaOfInterest.
    */
   virtual void initialize();

   /**
    * Calls:
    * writeFile()
    * writeMetaDataFiles()
    *
    * @return true on success, false on error.
    */
   virtual bool execute();

   /**
    * @brief Method to write the image to a stream.
    *
    * This implementation does nothing and returns false.
    * Derived classed should implement as needed.
    *
    * @return true on success, false on error.
    */
   virtual bool writeStream();

   /**
    * @brief Sets the output stream to write to.
    *
    * This implementation in this class does nothing and returns false.
    * Derived classed should implement as needed.
    *
    * @param stream The stream to write to.
    *
    * @return true if object can write to stream, false if not.
    */
   virtual bool setOutputStream(ossimRefPtr<ossimOStream> stream);

   /** tmp drb */
   virtual bool setOutputStream(std::ostream& str);

   /**
    * @brief Method to return the stream attached to output.
    *
    * This implementation in this class does nothing and returns an empty
    * ossimRefPtr<ossimOStream>.
    * Derived classed should implement as needed.
    * 
    * @return The stream wrapped around a ref pointer.
    */
   virtual ossimRefPtr<ossimOStream> getOutputStream() const;
   
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
   virtual bool         getScaleToEightBitFlag()       const;

   virtual bool         getWriteEnviHeaderFlag()       const;
   virtual bool         getWriteErsHeaderFlag()        const;   
   virtual bool         getWriteExternalGeometryFlag() const;
   virtual bool         getWriteFgdcFlag()             const;
   virtual bool         getWriteJpegWorldFileFlag()    const;
   virtual bool         getWriteReadmeFlag()           const;
   virtual bool         getWriteTiffWorldFileFlag()    const;
   virtual bool         getWriteWorldFileFlag()        const;

   virtual void         setWriteImageFlag(bool flag);
   virtual void         setWriteOverviewFlag(bool flag);
   virtual void         setWriteHistogramFlag(bool flag);
   virtual void         setScaleToEightBitFlag(bool flag);

   virtual void         setWriteEnviHeaderFlag(bool flag);
   virtual void         setWriteErsHeaderFlag(bool flag); 
   virtual void         setWriteExternalGeometryFlag(bool flag);
   virtual void         setWriteFgdcFlag(bool flag);
   virtual void         setWriteJpegWorldFile(bool flag);
   virtual void         setWriteReadme(bool flag);
   virtual void         setWriteTiffWorldFile(bool flag);
   virtual void         setWriteWorldFile(bool flag);
   
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

   /**
    * If "point" the coordinate tie points are relative to the center of the
    * pixel.  If "area" the coordinate tie points are relative to the upper
    * left corner of the pixel.
    *
    * For image types that are keyed, tiff for instance, this will set the
    * geotiff key: GTRasterTypeGeoKey
    *
    * If meta data writers are enabled, world file writers for instance,
    * this will also be propagated to them.
    *
    * Default in this writer is:  OSSIM_PIXEL_IS_POINT
    * 
    * @param type
    * OSSIM_PIXEL_IS_POINT = center of pixel
    * OSSIM_PIXEL_IS_AREA  = upper left corner of pixel
    */
   virtual void setPixelType(ossimPixelType type);

   /**
    * If "point" the coordinate tie points are relative to the center of the
    * pixel.  If "area" the coordinate tie points are relative to the upper
    * left corner of the pixel.
    *
    * For image types that are keyed, tiff for instance, this will set the
    * geotiff key: GTRasterTypeGeoKey
    *
    * If meta data writers are enabled, world file writers for instance,
    * this will also be propagated to them.

    * Default in this writer is:  OSSIM_PIXEL_IS_POINT
    * 
    * @param type
    * "point" = center of pixel
    * "area"  = upper left corner of pixel
    *
    * @note If the downcased "type" is not equal to "point" or "area" pixel
    * type will remain unchanged.
    */
   virtual void setPixelType(const ossimString& type);

   /**
    * @return The pixel type as either OSSIM_PIXEL_IS_POINT or
    * OSSIM_PIXEL_IS_AREA.
    */
   virtual ossimPixelType getPixelType() const;
   
   /**
    * @param type ossimString to initialize which will either be "point" or
    * "area".
    */
   virtual void getPixelTypeString(ossimString& type) const;
   
protected:

   /**
    * Common world file writer method.
    *
    * @param file The file to write like "foo.tfw" or "foo.jpw".
    *
    * @return true on success, false on error.
    */
   bool writeWorldFile(const ossimFilename& file);
   
   /**
    * Write out the file.
    * @return true on success, false on error.
    */
   virtual bool writeFile() = 0;
   
   ossimRefPtr<ossimImageSourceSequencer> theInputConnection;
   ossimRefPtr<ossimViewController>       theViewController;
   ossimListener*             theProgressListener;
   ossimFilename              theFilename;
   ossimString                theOutputImageType;
   bool                       theWriteImageFlag;
   bool                       theWriteOverviewFlag;
   bool                       theWriteHistogramFlag;
   bool                       theScaleToEightBitFlag;
   ossim_uint16               theOverviewCompressType;
   ossim_int32                theOverviewJpegCompressQuality;

   /**
    * External meta data writer flags.  Not really part of an image writer, put
    * here for convenience.  If true the writer will do the connection work
    * for you.
    */
   bool                       theWriteEnviHeaderFlag;
   bool                       theWriteErsHeaderFlag; 
   bool                       theWriteExternalGeometryFlag;
   bool                       theWriteFgdcFlag;
   bool                       theWriteJpegWorldFileFlag;
   bool                       theWriteReadmeFlag;
   bool                       theWriteTiffWorldFileFlag;
   bool                       theWriteWorldFileFlag;
   bool                       theAutoCreateDirectoryFlag;
   
   /**
    * If not a geographic projection this allows the user to set the linear
    * units of the output tag to be:
    * 
    * meters (default)
    * us_survey_feet
    * feet
    *
    * This will be propagated to writers that can switch types and to the
    * associated metadata writers.
    */
   ossimUnitType              theLinearUnits;

   /** OSSIM_PIXEL_IS_POINT = 0, OSSIM_PIXEL_IS_AREA  = 1 */
   ossimPixelType             thePixelType;
   
TYPE_DATA
};

#endif
