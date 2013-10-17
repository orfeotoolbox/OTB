//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Contains class declaration for TiffOverviewBuilder.
//
//*******************************************************************
//  $Id: ossimTiffOverviewBuilder.h 22232 2013-04-13 20:06:19Z dburken $

#ifndef ossimTiffOverviewBuilder_HEADER
#define ossimTiffOverviewBuilder_HEADER

#include <vector>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>

#include <ossim/imaging/ossimOverviewBuilderBase.h>
#include <ossim/imaging/ossimFilterResampler.h>

#include <tiffio.h>

class ossimConnectableObject;
class ossimFilename;
class ossimImageGeometry;

class OSSIM_DLL ossimTiffOverviewBuilder
   :
      public ossimOverviewBuilderBase
{
public:

   enum
   {
      DEFAULT_COMPRESS_QUALITY=75
   };

   /** default constructor */
   ossimTiffOverviewBuilder();
   
   /** virtual destructor */
   virtual ~ossimTiffOverviewBuilder();

   /**
    * Supports BOX or NEAREST NEIGHBOR.  When indexed you should probably use nearest neighbor
    */ 
   void setResampleType(ossimFilterResampler::ossimFilterResamplerType resampleType);
   
   /**
    *  Builds overview file and sets "theOutputFile" to that of
    *  the overview_file.
    *
    *  @param overview_file The overview file name to output.
    *
    *  @param copy_all  If set to true the entire image will be
    *  copied.  This can be used to convert an image to a tiled tif.
    *  
    *  @return trueon success, false on error.
    */
   bool buildOverview(const ossimFilename& overview_file,
                      bool copy_all=false);

   /**
    * Calls buildOverview.  This method uses "theOutputFile" for the file
    * name.
    *
    * If the copy_all flag is set the entire image will be copied.  This can
    * be used to convert an image to a tiled tif.
    *
    * @return true on success, false on error.
    *
    * @note If setOutputFile was not called the output name will be derived
    * from the image name.  If image was "foo.tif" the overview file will
    * be "foo.ovr".
    */
   virtual bool execute();

   /**
    *  Sets the compression type to use when building overviews.
    *  
    *  @param compression_type Current supported types:
    * - COMPRESSION_JPEG
    * - COMPRESSION_LZW
    * - COMPRESSION_DEFLATE
    * - COMPRESSION_PACKBITS
    * - COMPRESSION_NONE (default)
    */
   void setCompressionType(ossim_uint16 compression_type);

   /**
    *  Sets the compression quality for use when using a compression type
    *  of COMPRESSION_JPEG.
    *
    *  @param quality Range 1 to 100 with 100 being best quality.
    */
   void setJpegCompressionQuality(ossim_int32 quality);

   /**
    * @brief Method to return copy all flag.
    * @return The copy all flag.  If true all data will be written to the
    * overview including R0.
    */
   bool getCopyAllFlag() const;

   /**
    * @brief Sets internal overviews flag.
    * @param flag If true, and input source is tiff format, overviews will be
    * built internally.
    */
   void setInternalOverviewsFlag(bool flag);

   /** @return The intenal overview flag. */
   bool getInternalOverviewsFlag() const;

   /**
    * @brief Sets theCopyAllFlag.
    * @param flag The flag. If true all data will be written to the
    * overview including R0.
    */
   void setCopyAllFlag(bool flag);

   /** @return ossimObject* to this object. */
   virtual ossimObject* getObject();

   /**
    * @return const ossimObject* to this object.
    */
   virtual const ossimObject* getObject() const;

   /**
    * @return true if input is an image handler.
    */
   virtual bool canConnectMyInputTo(ossim_int32 index,
                                    const ossimConnectableObject* obj) const;

   /**
    * @brief Sets the input to the builder. Satisfies pure virtual from
    * ossimOverviewBuilderBase.
    *
    * @param imageSource The input to the builder.
    *
    * @return True on successful initializion, false on error.
    */
   virtual bool setInputSource(ossimImageHandler* imageSource);
   
   /**
    * @brief Sets the output filename.
    * Satisfies pure virtual from ossimOverviewBuilderBase.
    * @param file The output file name.
    */
   virtual void  setOutputFile(const ossimFilename& file);

   /**
    * @brief Gets the output file name. (pure virtual)
    *
    * @return The output file name or ossimFilename::NIL if it was not set
    * yet and the image handle has not been initialized.
    *
    * @note This is non-const as a call to this may force initialization of
    * overview builder output file name if the it was not set already.
    *
    * @note This will return ossimFilename::NIL unless one of was called,
    * setInputSource or setOutputFile.
    */
   virtual ossimFilename getOutputFile() const;

   void setOutputTileSize(const ossimIpt& tileSize);

   /**
    * @brief Sets the overview output type.
    *
    * Satisfies pure virtual from ossimOverviewBuilderBase.
    * 
    * Currently handled types are:
    * "ossim_tiff_nearest" and "ossim_tiff_box"
    *
    * @param type This should be the string representing the type.  This method
    * will do nothing if type is not handled and return false.
    *
    * @return true if type is handled, false if not.
    */
   virtual bool setOverviewType(const ossimString& type);

   /**
    * @brief Gets the overview type.
    * Satisfies pure virtual from ossimOverviewBuilderBase.
    * @return The overview output type as a string.
    */
   virtual ossimString getOverviewType() const;

   /**
    * @brief Method to populate class supported types.
    * Satisfies pure virtual from ossimOverviewBuilderBase.
    * @param typeList List of ossimStrings to add to.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;

   /**
    * @brief Method to set properties.
    * @param property Property to set.
    *
    * @note Currently supported property:
    * name=levels, value should be list of levels separated by a comma with
    * no spaces. Example: "2,4,8,16,32,64"
    */
   virtual void setProperty(ossimRefPtr<ossimProperty> property);

   /**
    * @brief Method to populate the list of property names.
    * @param propertyNames List to populate.  This does not clear the list
    * just adds to it.
    */
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
  
private:

   /**
    *  Copy the full resolution image data to the output tif image.
    */
  bool writeR0(TIFF* tif);

   /**
    *  Write reduced resolution data set to the tif file.
    *
    *  @param firstRestLevel used to tell method that if a histogram is needed, do it on
    *  that res level.
    */
   bool writeRn(ossimImageHandler* imageHandler,
                TIFF* tif,
                ossim_uint32 resLevel,
                bool firstResLevel);
   
   /**
    *  Set the tiff tags for the appropriate resLevel.  Level zero is the
    *  full resolution image.
    *
    *  @param tif Pointer to the tif file.
    *  @param outputRect The dimensions (zero based) of res set.
    *  @param rrds_level The current reduced res level.
    */
   bool setTags(TIFF* tif,
                const ossimIrect& outputRect,
                ossim_int32 resLevel) const;

   /**
    *  Writes geotiff tags.
    *  @param geom
    *  @param boundingRect
    *  @param resLevel Zero base, 0 being full res.
    *  @param tif
    *  @return true on success, false on error.
    */
   bool setGeotiffTags(const ossimImageGeometry* geom,
                       const ossimDrect& boundingRect,
                       ossim_uint32 resLevel,
                       TIFF* tif);

   TIFF* openTiff( const ossimString& filename ) const;

   void closeTiff(TIFF* tif);

   /**
    * @returns true if m_internalOverviewsFlag and input image supports
    * building internal overviews.  Currently only tiff reader.
    */
   bool buildInternalOverviews() const;

   /**
    * @return true if m_copyAllFlag is set and build internal overviews is not
    * set.
    */
   bool copyR0() const;

   // Disallow these...
   ossimTiffOverviewBuilder(const ossimTiffOverviewBuilder& source);
   ossimTiffOverviewBuilder& operator=(const ossimTiffOverviewBuilder& rhs); 

   std::vector<ossim_uint8>                           m_nullDataBuffer;
   ossim_int32                                        m_bytesPerPixel;
   ossim_int32                                        m_bitsPerSample;
   ossim_int32                                        m_tileWidth;
   ossim_int32                                        m_tileHeight;
   ossim_int32                                        m_tileSizeInBytes;
   ossim_int32                                        m_sampleFormat;
   ossim_int32                                        m_currentTiffDir;
   ossim_uint16                                       m_tiffCompressType;
   ossim_int32                                        m_jpegCompressQuality;
   ossimFilterResampler::ossimFilterResamplerType     m_resampleType;
   std::vector<double>                                m_nullPixelValues;
   bool                                               m_copyAllFlag;
   bool                                               m_outputTileSizeSetFlag;
   bool                                               m_internalOverviewsFlag;

TYPE_DATA   
};
   
#endif
