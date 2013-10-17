//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//          Frank Warmerdam (warmerdam@pobox.com)
//
// Description:
//
// Contains class declaration for ossimTiffTileSource.
// ossimTiffTileSource  is derived from ImageHandler which is derived from
// TileSource.
//*******************************************************************
//  $Id: ossimTiffTileSource.h 22243 2013-04-20 15:27:22Z dburken $

#ifndef ossimTiffTileSource_HEADER
#define ossimTiffTileSource_HEADER 1

#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimIrect.h>
#include <tiffio.h>
#include <vector>

class ossimImageData;
class ossimTiffOverviewTileSource;

class OSSIMDLLEXPORT ossimTiffTileSource : public ossimImageHandler
{
public:

   enum ReadMethod
   {
      UNKNOWN,
      READ_RGBA_U8_TILE,
      READ_RGBA_U8_STRIP,
      READ_RGBA_U8A_STRIP,
      READ_SCAN_LINE,
      READ_TILE,
      READ_U16_STRIP
   };

   ossimTiffTileSource();

   virtual ossimString getLongName()  const;
   virtual ossimString getShortName() const;

   /**
    *  Returns true if the image_file can be opened and is a valid tiff file.
    */
   virtual bool open(const ossimFilename& image_file);
   virtual void close();
   
   /**
    *  Returns a pointer to a tile given an origin representing the upper left
    *  corner of the tile to grab from the image.
    *  Satisfies pure virtual from TileSource class.
    */
   virtual ossimRefPtr<ossimImageData> getTile(const  ossimIrect& rect,
                                               ossim_uint32 resLevel=0);
   
   /**
    * Method to get a tile.   
    *
    * @param result The tile to stuff.  Note The requested rectangle in full
    * image space and bands should be set in the result tile prior to
    * passing.  It will be an error if:
    * result.getNumberOfBands() != this->getNumberOfOutputBands()
    *
    * @return true on success false on error.  If return is false, result
    *  is undefined so caller should handle appropriately with makeBlank or
    * whatever.
    */
   virtual bool getTile(ossimImageData* result, ossim_uint32 resLevel=0);
   
   /**
    *  Returns the number of bands in the image.
    *  Satisfies pure virtual from ImageHandler class.
    */
   virtual ossim_uint32 getNumberOfInputBands() const;
   virtual ossim_uint32 getNumberOfOutputBands () const;

   /**
    * @brief Indicates whether or not the image handler can control output
    * band selection via the setOutputBandList method.
    *
    * Overrides: ossimImageHandler::isBandSelector
    *
    * @return true
    */
   virtual bool isBandSelector() const;

   /**
    * @brief If the image handler "isBandSeletor()" then the band selection
    * of the output chip can be controlled.
    *
    * Overrides: ossimImageHandler::setOutputBandList
    * 
    * @return true on success, false on error.
    */
   virtual bool setOutputBandList(const std::vector<ossim_uint32>& band_list);

   /** @brief Initializes bandList to the zero based order of output bands. */
   virtual void getOutputBandList(std::vector<ossim_uint32>& bandList) const;
   
   /**
    *  Returns the number of lines in the image.
    *  Satisfies pure virtual from ImageHandler class.
    */
   virtual ossim_uint32 getNumberOfLines(ossim_uint32 resLevel = 0) const;
   
   /**
    *  Returns the number of samples in the image.
    *  Satisfies pure virtual from ImageHandler class.
    */
   virtual ossim_uint32 getNumberOfSamples(ossim_uint32 resLevel = 0) const;

   /**
    *  Returns the number of image file directories in the tiff image.
    */
   virtual ossim_uint32 getNumberOfDirectories() const;
   
   /**
    * Returns the number of reduced resolution data sets (rrds).
    * Notes:
    *
    * - The full res image is counted as a data set so an image with no
    *   reduced resolution data set will have a count of one.
    * - This method counts R0 as a res set even if it does not have one.
    *   This was done deliberately so as to not screw up code down the
    *   line.
    */
   virtual ossim_uint32 getNumberOfDecimationLevels() const;
   
   /**
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   virtual bool isOpen()const;

   /**
    * Returns the output pixel type of the tile source.
    */
   virtual ossimScalarType getOutputScalarType() const;

   /**
    * Returns the width of the output tile.
    */
   virtual ossim_uint32 getTileWidth() const;
   
   /**
    * Returns the height of the output tile.
    */
   virtual ossim_uint32 getTileHeight() const;

   /**
    *  Returns true if the first directory of the tiff image did not have
    *  the reduced resolution file type set.
    */
   bool hasR0() const;
  
   virtual ossim_float64 getMinPixelValue(ossim_uint32 band=0)const;
   virtual ossim_float64 getMaxPixelValue(ossim_uint32 band=0)const;
   virtual ossim_float64 getNullPixelValue(ossim_uint32 band=0)const;
   
   virtual bool isValidRLevel(ossim_uint32 resLevel) const;

   /**
    * @return The tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileWidth which
    * returns the output tile width, which can be different than the
    * internal image tile width on disk.
    */
   virtual ossim_uint32 getImageTileWidth() const;

   /**
    * @return The tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileHeight which
    * returns the output tile width which can be different than the internal
    * image tile width on disk.
    */
   virtual ossim_uint32 getImageTileHeight() const;

   void setApplyColorPaletteFlag(bool flag);
   bool getApplyColorPaletteFlag()const;

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   bool isColorMapped() const;
   
   virtual std::ostream& print(std::ostream& os) const;

   TIFF* tiffPtr()
   {
      return theTiffPtr;
   }
#if 0
   /**
    * @brief Method to get the image geometry object associated with this
    * image.
    *
    * The geometry contains full-to-local image transform as well as
    * projection (image-to-world).
    *
    * @return ossimImageGeometry* or null if not defined.
    */
   virtual ossimImageGeometry* getImageGeometry();
#endif   
protected:
   virtual ~ossimTiffTileSource();
   /**
    *  Returns true if no errors initializing object.
    *
    *  Notes:
    *  - Callers of this method must ensure "theTiffPtr" data member
    *    is initialized.
    *  - This method was added to consolidate object initialization code
    *    between constructor and public open method.
    */
   bool open();

   // Must be protected for derived ossimTerraSarTiffReader.
   TIFF* theTiffPtr; 
   
private:

   /**
    *  Adjust point to even tile boundary.  Assumes 0,0 origin.
    *  Shifts in the upper left direction.
    */
   void adjustToStartOfTile(ossimIpt& pt) const;

   /**
    *  If the tiff source has R0 then this returns the current tiff directory
    *  that the tiff pointer is pointing to; else, it returns the current
    *  directory + 1.
    */
   ossim_uint32 getCurrentTiffRLevel() const;
   
   ossimString getReadMethod(ossim_uint32 directory) const;
   
   bool loadTile(const ossimIrect& tile_rect,
                 const ossimIrect& clip_rect,
                 ossimImageData* result);
   
   bool loadFromRgbaU8Tile(const ossimIrect& tile_rect,
                           const ossimIrect& clip_rect,
                           ossimImageData* result);
   
   bool loadFromRgbaU8Strip(const ossimIrect& tile_rect,
                            const ossimIrect& clip_rect,
                            ossimImageData* result);
   
   bool loadFromRgbaU8aStrip(const ossimIrect& tile_rect,
                             const ossimIrect& clip_rect,
                             ossimImageData* result);

   bool loadFromU16Strip(const ossimIrect& clip_rect,
                         ossimImageData* result);
   
   bool loadFromScanLine(const ossimIrect& clip_rect,
                         ossimImageData* result);

   bool loadFromTile(const ossimIrect& clip_rect,
                     ossimImageData* result);
   
   void setReadMethod();
   
   /**
    * Change tiff directory and sets theCurrentDirectory.
    *
    * @return true on success, false on error.
    */
   bool setTiffDirectory(ossim_uint16 directory);

   void populateLut();

   /**
    * @brief validateMinMaxNull Checks min, max and null to make sure they are not equal
    * to the scalar type nan or double nan; sets to default min max if so.
    */
   void validateMinMaxNull();

   /**
    * @brief Checks line/samples of level for power of two decimation from the
    * previous rlevel.
    *
    * Used to weed out thumbnail directories being used as a reduced
    * resolution layer.
    *
    * Note this shuts off the thumbnail which someone may
    * want to see.  We could make this a reader prop if it becomes an issue.
    * drb - 09 Jan. 2012.
    *
    * @param dir The tiff directory index.
    * 
    * @return true is so; else, false.  If level is zero returns false.
    */
   bool isPowerOfTwoDecimation(ossim_uint32 dir) const;

   /** @brief Allocates theTile. */
   void allocateTile();

   /**
    * @brief Allocates theBuffer
    * @return true on success; false, on error.
    */
   bool allocateBuffer();  
   
   ossimRefPtr<ossimImageData> theTile;
   
   ossim_uint8*         theBuffer;
   ossim_uint32         theBufferSize;
   ossimIrect           theBufferRect;
   ossim_uint32         theBufferRLevel;
   ossim_uint32         theCurrentTileWidth;
   ossim_uint32         theCurrentTileHeight;

   ossim_uint16         theSamplesPerPixel;
   ossim_uint16         theBitsPerSample;
   ossim_uint16         theSampleFormatUnit;

   ossim_float64        theMaxSampleValue;
   ossim_float64        theMinSampleValue;
   ossim_float64        theNullSampleValue;

   ossim_uint16         theNumberOfDirectories;
   ossim_uint16         theCurrentDirectory;
   bool                 theR0isFullRes;
   ossim_uint32         theBytesPerPixel;
   ossimScalarType      theScalarType;
   bool                 theApplyColorPaletteFlag;

   // Things we need from each directory as they can be different.
   std::vector<ossim_uint32> theImageWidth;
   std::vector<ossim_uint32> theImageLength;
   std::vector<ReadMethod>   theReadMethod;
   std::vector<ossim_uint16> thePlanarConfig;
   std::vector<ossim_uint16> thePhotometric;
   std::vector<ossim_uint32> theRowsPerStrip;
   std::vector<ossim_uint32> theImageTileWidth;
   std::vector<ossim_uint32> theImageTileLength;
   std::vector<ossim_uint32> theImageDirectoryList;
   
   ossim_uint32              theCurrentTiffRlevel;
   ossim_int32               theCompressionType;
   std::vector<ossim_uint32> theOutputBandList;
   
TYPE_DATA
};

#endif /* #ifndef ossimTiffTileSource_HEADER */
