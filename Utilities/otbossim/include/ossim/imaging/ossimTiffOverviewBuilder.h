//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
//
// Contains class declaration for TiffOverviewBuilder.
//
//*******************************************************************
//  $Id: ossimTiffOverviewBuilder.h 11699 2007-09-10 15:23:26Z gpotts $

#ifndef ossimTiffOverviewBuilder_HEADER
#define ossimTiffOverviewBuilder_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>

#include <ossim/imaging/ossimOverviewBuilderBase.h>
#include <ossim/imaging/ossimFilterResampler.h>

#include <tiffio.h>

class ossimImageHandler;
class ossimFilename;
class ossimConnectableObject;

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
    * @brief Sets theCopyAllFlag.
    * @param flag The flag. If true all data will be written to the
    * overview including R0.
    */
   void setCopyAllFlag(bool flag);

   /**
    * @return ossimObject* to this object.
    */
   virtual ossimObject* getObject();

   /**
    * @return const ossimObject* to this object.
    */
   virtual const ossimObject* getObject() const;

   /**
    * @return The output filename.  This will be derived from the image
    * handlers filename unless the method buildOverview has been called which
    * takes a filename.
    */
   ossimFilename getOutputFile() const;

   /**
    * @return true if input is an image handler.
    */
   virtual bool canConnectMyInputTo(ossim_int32 index,
                                    const ossimConnectableObject* obj) const;

   /**
    * @brief Sets the input to the builder. Satisfies pure virtual from
    * ossimOverviewBuilderInterface.
    *
    * @param imageSource The input to the builder.
    *
    * @param bool youOwnItFlag If true this obect will delete imageSource
    * at destruction.
    *
    * @return True on successful initializion, false on error.
    */
   virtual bool setInputSource(ossimImageHandler* imageSource,
                               bool youOwnItFlag);
   
   /**
    * @brief Sets the output filename.
    * Satisfies pure virtual from ossimOverviewBuilderInterface.
    * @param file The output file name.
    */
   virtual void  setOutputFile(const ossimFilename& file);

   void setOutputTileSize(const ossimIpt& tileSize);

   /**
    * @brief Sets the overview output type.
    *
    * Satisfies pure virtual from ossimOverviewBuilderInterface.
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
    * Satisfies pure virtual from ossimOverviewBuilderInterface.
    * @return The overview output type as a string.
    */
   virtual ossimString getOverviewType() const;

   /**
    * @brief Method to populate class supported types.
    * Satisfies pure virtual from ossimOverviewBuilderInterface.
    * @param typeList List of ossimStrings to add to.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;

   /**
    * @biref Method to set properties.
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
    * @brief Method to initialize output file name from image handler.
    * @return true on success, false on error.
    */
   bool initializeOutputFilenamFromHandler();
   
   /**
    *  Returns the number of reduced resolution data sets to get the smallest
    *  dimension >= 32 && < 64.
    */
  ossim_int32 getNumberOfSets() const;

   /**
    *  Copy the full resolution image data to the output tif image.
    */
  bool writeR0(TIFF* tif);

   /**
    *  Write reduced resolution data set to the tif file.  "rrds_level" must
    *  be greater than 1 for this method as it uses subsequent rrds_levels
    *  from the tif file.
    */
   bool writeRn(ossimImageHandler* imageHandler,
                TIFF* tif,
                ossim_uint32 rrdsLevel);
   
   /**
    *  Set the tiff tags for the appropriate rrds_level.  Level zero is the
    *  full resolution image.
    *
    *  @param tif Pointer to the tif file.
    *  @param outputRect The dimensions (zero based) of res set.
    *  @param rrds_level The current reduced res level.
    */
   bool setTags(TIFF* tif,
                const ossimIrect& outputRect,
                ossim_int32 rrds_level) const;

   TIFF* openTiff(const ossimString& filename,
                  const ossimString& openMode);
   void closeTiff(TIFF* tif);
   // Disallow these...
   ossimTiffOverviewBuilder(const ossimTiffOverviewBuilder& source);
   ossimTiffOverviewBuilder& operator=(const ossimTiffOverviewBuilder& rhs); 

   ossimImageHandler* theImageHandler;
   bool               theOwnsImageHandlerFlag;
   ossimFilename      theOutputFile;
   ossimFilename      theOutputFileTmp;
   ossim_uint8*       theNullDataBuffer;
   ossim_int32        theBytesPerPixel;
   ossim_int32        theBitsPerSample;
   ossim_int32        theTileWidth;
   ossim_int32        theTileHeight;
   ossim_int32        theTileSizeInBytes;
   ossim_int32        theSampleFormat;
   ossim_int32        theCurrentTiffDir;
   ossim_uint16       theTiffCompressType;
   ossim_int32        theJpegCompressQuality;
   ossimFilterResampler::ossimFilterResamplerType theResampleType;
   vector<double>      theNullPixelValues;
   bool               theCopyAllFlag;
   bool               theOutputTileSizeSetFlag;
TYPE_DATA   
};
   
#endif
