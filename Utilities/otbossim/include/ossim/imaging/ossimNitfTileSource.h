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
// Contains class declaration for NitfTileSource.
//
//*******************************************************************
//  $Id: ossimNitfTileSource.h 21445 2012-08-03 20:30:10Z dburken $

#ifndef ossimNitfTileSource_HEADER
#define ossimNitfTileSource_HEADER 1

#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimAppFixedTileCache.h>
#include <ossim/support_data/ossimNitfFile.h>
#include <ossim/support_data/ossimNitfFileHeader.h>
#include <ossim/support_data/ossimNitfImageHeader.h>
#include <fstream>

struct jpeg_decompress_struct;

class OSSIM_DLL ossimNitfTileSource : public ossimImageHandler
{
public:

   enum ReadMode
   {
      READ_MODE_UNKNOWN = 0,
      
      /** IMODE = B "Band Interleaved By Block or a single band" */
      READ_BIB_BLOCK = 1,

      /** IMODE = P "Band Interleaved By Pixel" */
      READ_BIP_BLOCK = 2,

      /** IMODE = R "Band Interleaved By Row" */
      READ_BIR_BLOCK = 3,

      /** IMODE = S "Band Sequential" */
      READ_BSQ_BLOCK = 4,

      /** IMODE = B of S "single block or one block for each band" */
      READ_BIB       = 5,

      /** IMODE = P "single block Band Interleaved By Pixel" */
      READ_BIP       = 6,

      /** IMODE = R "single block Band Interleaved By Row" */
      READ_BIR       = 7,

      /** IMODE = B, IC = C3 "JPEG compressed blocks" */
      READ_JPEG_BLOCK = 8
   };

   ossimNitfTileSource();
   
   
   virtual ossimString getShortName() const;
   virtual ossimString getLongName()  const;

   /**
    *  Returns true if the image_file can be opened and is a valid nitf file.
    */
   virtual bool open();

   /**
    * Closes file and destroys all memory allocated.
    */
   virtual void close();

   /**
    * @param tileRect Requested rectangle.
    *
    * @param resLevel Reduced resolution level to grab tileRect from.
    * Default = 0 or the full resolution data set.
    *
    * @return Returns an image data object with requested rectangle from the
    * image.  Depending on the overlap of tileRect with respect to the image
    * rectangle, the returned tile could be full, partial, or an empty(blank)
    * tile.
    */
   virtual ossimRefPtr<ossimImageData> getTile(const  ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);

    /**
     * @return Returns the number of bands in the image.
     * Satisfies pure virtual from ImageHandler class.
     */
   virtual ossim_uint32 getNumberOfInputBands() const;

   /**
    * @return Number of output bands.
    */
   virtual ossim_uint32 getNumberOfOutputBands() const;

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
    * Returns the tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileWidth which
    * returns the output tile width which can be different than the internal
    * image tile width on disk.
    */
   virtual ossim_uint32 getImageTileWidth() const;

   /**
    * Returns the tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileWidth which
    * returns the output tile width which can be different than the internal
    * image tile width on disk.
    */
   virtual ossim_uint32 getImageTileHeight() const;   

   virtual bool isOpen()const;

   /**
    * @return The current entry number.
    *
    * @note NITF's can contain multiple images in a single file.  This returns
    * the index of the current image being read.
    */
   virtual ossim_uint32 getCurrentEntry() const;

   /**
    * @param entryList This is the list to initialize with entry indexes.
    */
   virtual void getEntryList(std::vector<ossim_uint32>& entryList) const;

   /**
    * @return Returns the number of entries (or images) within the nitf
    * file.
    */
   ossim_uint32 getNumberOfEntries() const;
   
   /**
    * @param entryIdx Zero base entry number to select.  Sets data member
    * "theCurrentEntry".
    *
    * @note "theCurrentEntry" will not be set if "entryIdx" is out of range.
    */
   virtual bool setCurrentEntry(ossim_uint32 entryIdx);

   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   ossimString getSecurityClassification()const;
   
   /**
    * @return Returns theCacheEnabledFlag.
    */
   bool getCacheEnabledFlag() const;

   /**
    * @param flag Sets theCacheEnabledFlag and disables/enables caching
    * accordingly.  If cache is disabled it is also flushed at the same time.
    * If cache is enabled, blocks read from the image will be cached.
    */
   void setCacheEnabledFlag(bool flag);

   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;
   virtual double getNullPixelValue(ossim_uint32 band=0)const;

   const ossimNitfFileHeader* getFileHeader()const;
   ossimNitfFileHeader* getFileHeader();
   /**
    * @return The image header for the current entry.
    */
   const ossimNitfImageHeader* getCurrentImageHeader() const;
   ossimNitfImageHeader* getCurrentImageHeader();

protected:
   virtual ~ossimNitfTileSource();

   /**
    * @param imageRect The full resolution image rectangle.
    *
    * @note Should contain offsets if there are any.
    */
   void setBoundingRectangle(const ossimIrect& imageRect);
   
   /** Copy constructor, disallow... */
   ossimNitfTileSource(const ossimNitfTileSource& obj);

   /** Operator=, disallow... */
   ossimNitfTileSource& operator=(const ossimNitfTileSource& rhs); 

   /**
    *  Returns true on success, false on error.
    */
   bool loadTile(const ossimIrect& clipRect);

   /**
    * @return Returns the block number given an origin.
    */
   ossim_uint32 getBlockNumber(const ossimIpt& block_origin) const;

   /**
    * Deletes all memory allocated by this object.
    */
   void destroy();

   /**
    * @brief Parses "theImageFile" and initializes all nitf headers.
    * @return true on success, false on error.
    */
   virtual bool parseFile();

   /**
    * @brief Allocates everything for current entry.
    *
    * This is called on an open() or a entry change to an already open nitf
    * file.
    *
    * This does not allocate buffers and tiles to keep open and
    * setCurrentEntry times to a minimum.  Buffers are allocated on first
    * grab of pixel data by allocatBuffers method.
    * 
    * @return True on success, false on error.
    */
   virtual bool allocate();

   /**
    * @brief Allocates buffers for current entry.
    *
    * This is called on first getTile().
    * 
    * @return True on success, false on error.
    */
   virtual bool allocateBuffers();

   /**
    * @param hdr Pointer to image header.
    * @return true if reader can uncompress nitf.
    * */
   virtual bool canUncompress(const ossimNitfImageHeader* hdr) const;

   /**
    * Initializes the data member "theScalarType" from the current entry.
    */
   virtual void initializeScalarType();

   /**
    * Initializes the data member "theSwapBytesFlag" from the current entry.
    */
   virtual void initializeSwapBytesFlag();
   
   /**
    * Initializes the data member "theReadMode" from the current entry.
    */
   virtual void initializeReadMode();

   /**
    * Initializes the data member "theNumberOfBands" from the current entry.
    */
   void initializeBandCount();

   /**
    * Initializes the data member "theBlockSize" from the current entry.
    *
    * @note This should be performed after setting the read mode and
    * band count.
    *
    * @note This is the size of a single block read.  So if the bands are
    * separated by block, a read of this size will get one block.
    *
    * @return true on success, false on error.
    */
   bool initializeBlockSize();

   /**
    * Initializes the data members "theImageRect" and "theBlockRect"
    * from the current entry.
    *
    * @return true on success, false on error.
    */
   virtual bool initializeImageRect();

   /**
    * Initializes the data member "theCacheSize".
    */
   void initializeCacheSize();

   /**
    * Initializes the data member "theCacheTileInterLeaveType".
    */
   virtual void initializeCacheTileInterLeaveType();
   
   /**
    * Initializes the cache tile size(width and height).  For block images
    * this will be the size of one block.  For images that are a single block,
    * this will be the image width by the height of one tile.
    */
   void initializeCacheTile();

   /**
    * Initializes the data member theCompressedBuf.
    */
   virtual void initializeCompressedBuf();

   /**
    * Initializes the output tile size(width and height).
    */
   virtual void initializeOutputTile();

   /**
    * @brief Initializes "theLut" if applicable.
    */
   void initializeLut();

   /**
    * Loads a block of data to theCacheTile.
    * 
    * @param x Starting x position of block to load.
    *
    * @param y Starting y position of block to load.
    *
    * @return true on success, false on error.
    *
    * @note x and y are zero based relative to the upper left corner so any
    * sub image offset should be subtracted off.
    */
   bool loadBlockFromCache(ossim_uint32 x, ossim_uint32 y,
                           const ossimIrect& clipRect);

   /**
    * Loads a block of data to theCacheTile.
    * 
    * @param x Starting x position of block to load.
    *
    * @param y Starting y position of block to load.
    *
    * @return true on success, false on error.
    *
    * @note x and y are zero based relative to the upper left corner so any
    * sub image offset should be subtracted off.
    */
   virtual bool loadBlock(ossim_uint32 x, ossim_uint32 y);

   /**
    * @param x Horizontal upper left pixel position of the requested block.
    *
    * @param y Vertical upper left pixel position of the requested block.
    *
    * @param band Band of block.  Only relative with IMODES that have bands
    * in separate blocks.
    *
    * @return true if the stream offset was settable and false otherwise.
    */
   bool getPosition(std::streamoff& position,
                    ossim_uint32 x,
                    ossim_uint32 y,
                    ossim_uint32 band) const;

   /**
    * @return the total number of blocks for the current entry.
    *
    * @note For band separated blocks, all bands will be counted as on block.
    */
   ossim_uint32 getNumberOfBlocks() const;

   /**
    * @return The number of bytes to get from one block band to the next band.
    */
   std::streampos getBandOffset() const;

   /**
    * @return The number of bytes to get from one block to the next block.
    */
   std::streampos getBlockOffset() const;
   

   void explodePackedBits(ossimRefPtr<ossimImageData> packedBuffer)const;

   void convertTransparentToNull(ossimRefPtr<ossimImageData> tile)const;
   
   ossim_uint32 getPartialReadSize(const ossimIpt& blockOrigin)const;
   bool isVqCompressed(const ossimString& compressionCode)const;

   /**
    * @brief Uncompresses Vector Quantization unmasked image data.
    * IC field = C4
    * @param destination tile to stuff.
    * @param source Pointer to compressed data.
    */
   void vqUncompressC4(ossimRefPtr<ossimImageData> destination,
                       ossim_uint8* source);
   
   /**
    * @brief Uncompresses Vector Quantization masked image data.
    * IC field = M4
    * @param destination tile to stuff.
    * @param source Pointer to compressed data.
    */
   void vqUncompressM4(ossimRefPtr<ossimImageData> destination,
                       ossim_uint8* source);

   void lutUncompress(ossimRefPtr<ossimImageData> destination,
                     ossim_uint8* source);

   /**
    * @brief scans the file storing in offsets in "theNitfBlockOffset" and
    * block sizes in "theNitfBlockSize".
    * @return true on success, false on error.  This checks for arrays being
    * the same size as number of blocks.
    */
   virtual bool scanForJpegBlockOffsets();

   /**
    * @brief Uncompresses a jpeg block using the jpeg-6b library.
    * @param x sample location in image space.
    * @param y line location in image space.
    * @return true on success, false on error.
    */
   virtual bool uncompressJpegBlock(ossim_uint32 x, ossim_uint32 y);

   /**
    * @brief Loads one of the default tables based on COMRAT value.
    *
    * @return true if comrat had valid table value(1-5) and table was loaded,
    * false if COMRAT value did not contain a valid value.  Will also return
    * false if there is already a table loaded.
    * 
    * @note COMRAT is nitf compression rate code field:
    * -  "00.2" == default compression table 2
    * -  "00.5" == default compression table 5 and so on.
    */
   bool loadJpegQuantizationTables(jpeg_decompress_struct& cinfo) const;

   /**
    * @brief Loads default huffman tables.
    *
    * @return true success, false on error.
    */
   bool loadJpegHuffmanTables(jpeg_decompress_struct& cinfo) const;
   
   /**
   * @brief Virtual method determines the decimation factors at each resolution level. 
   * This method derives the decimations from the image metadata.
   */
   virtual void establishDecimationFactors();

   ossimRefPtr<ossimImageData>   theTile;
   ossimRefPtr<ossimImageData>   theCacheTile;
   ossimRefPtr<ossimNitfFile>    theNitfFile;
   std::vector<ossimRefPtr<ossimNitfImageHeader> > theNitfImageHeader;
   ReadMode                      theReadMode;
   ossimScalarType               theScalarType;
   bool                          theSwapBytesFlag;
   ossim_uint32                  theNumberOfInputBands;
   ossim_uint32                  theNumberOfOutputBands;
   ossim_uint32                  theBlockSizeInBytes;
   ossim_uint32                  theReadBlockSizeInBytes;
   ossim_uint32                  theNumberOfImages;
   ossim_uint32                  theCurrentEntry;
   ossimIrect                    theImageRect;
   std::ifstream                 theFileStr;   
   std::vector<ossim_uint32>     theOutputBandList;
   ossimIpt                      theCacheSize;
   ossimInterleaveType           theCacheTileInterLeaveType;
   bool                          theCacheEnabledFlag;
   std::vector<ossim_uint32>     theEntryList;
   ossimAppFixedTileCache::ossimAppFixedCacheId theCacheId;
   bool                          thePackedBitsFlag;
   ossimIrect                    theBlockImageRect;
   std::vector<ossim_uint8>      theCompressedBuf;

   //---
   // Have compressed jpeg blocks of variable length so we must scan and
   // capture the offsetts and block sizes for reading.
   //---
   std::vector<std::streamoff>   theNitfBlockOffset;
   std::vector<ossim_uint32>     theNitfBlockSize;
   
   bool m_isJpeg12Bit;

   //---
   // If set to true indicates scanForJpegBlockOffsets() needs to be called
   // prior to grabbing a block.
   //---
   bool m_jpegOffsetsDirty;
   
TYPE_DATA
};
   
#endif /* #ifndef ossimNitfTileSource_HEADER */
