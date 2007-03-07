//*******************************************************************
// Copyright (C) 2001 ImageLinks Inc.
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
//  $Id: ossimNitfTileSource.h,v 1.42 2005/11/09 16:07:43 dburken Exp $
#ifndef ossimNitfTileSource_HEADER
#define ossimNitfTileSource_HEADER

#include <fstream>
using namespace std;

#include <imaging/formats/ossimImageHandler.h>
#include <imaging/cache/ossimAppFixedTileCache.h>

class ossimNitfFile;
class ossimNitfImageHeader;

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
      READ_BIR       = 7
   };

   ossimNitfTileSource();
   
   virtual ~ossimNitfTileSource();
   
   virtual ossimString getShortName() const;
   virtual ossimString getLongName()  const;
   virtual ossimString className()    const;

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

protected:

   /**
    * @param imageRect The full resolution image rectangle.
    *
    * @note Should contain offsets if there are any.
    */
   void setBoundingRectangle(const ossimIrect& imageRect);

   /**
    * @return The image header for the current entry.
    */
   const ossimNitfImageHeader* getCurrentImageHeader() const;

private:
   
   /** Copy constructor, disallow... */
   ossimNitfTileSource(const ossimNitfTileSource& source);

   /** Operator=, disallow... */
   ossimNitfTileSource& operator=(const ossimNitfTileSource& source); 

   /**
    *  Returns true on success, false on error.
    */
   bool loadTile(const ossimIrect& clipRect);

   /**
    * @param pt Point to adjust.  This will be modified by the method to fall
    * on an even block boundary.  Behavior is to snap to the nearest upper
    * left block corner.
    *
    * @param tileHeight Height of one tile in pixels.
    *
    * @param tileWidth Width of one tile in pixels.
    */
   void adjustToStartOfBlock(ossimIpt& pt,
                             ossim_int32 tileHeight,
                             ossim_int32 tileWidth) const;

   /**
    * @return Returns the block number given an origin.
    */
   ossim_uint32 getBlockNumber(const ossimIpt& block_origin) const;

   /**
    * Deletes all memory allocated by this object.
    */
   void destroy();

   /**
    * Parses "theImageFile" and initializes all nitf headers.
    *
    * @return true on success, false on error.
    *
    * @note parseFile() and initialize() are separated  so initialize() can
    * be called on a entry change to an already open nitf file.
    */
   bool parseFile();

   /**
    * Allocates all memory with the exception of headers.
    *
    * @note parseFile() and allocate() are separated so allocate() can
    * be called on a entry change to an already open nitf file.
    */
   bool allocate();

   /**
    * Initializes the data member "theScalarType" from the current entry.
    */
   void initializeScalarType();

   /**
    * Initializes the data member "theSwapBytesFlag" from the current entry.
    */
   void initializeSwapBytesFlag();
   
   /**
    * Initializes the data member "theReadMode" from the current entry.
    */
   void initializeReadMode();

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
    */
   void initializeBlockSize();

   /**
    * Initializes the data members "theImageRect" and "theSubImageOffset"
    * from the current entry.
    */
   void initializeImageRect();

   /**
    * Initializes the data member "theCacheSize".
    */
   void initializeCacheSize();

   /**
    * Initializes the data member "theCacheTileInterLeaveType".
    */
   void initializeCacheTileInterLeaveType();
   
   /**
    * Initializes the cache tile size(width and height).  For block images
    * this will be the size of one block.  For images that are a single block,
    * this will be the image width by the height of one tile.
    */
   void initializeCacheTile();

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
   bool loadBlock(ossim_uint32 x, ossim_uint32 y);

   /**
    * @param x Horizontal upper left pixel position of the requested block.
    *
    * @param y Vertical upper left pixel position of the requested block.
    *
    * @param band Band of block.  Only relative with IMODES that have bands
    * in separate blocks.
    *
    * @return true if the stream position was settable and false otherwise.
    */
   bool getPosition(std::streampos& position,
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
   
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;
   virtual double getNullPixelValue(ossim_uint32 band=0)const;
   ossim_uint32 getPartialReadSize(const ossimIpt& blockOrigin)const;
   bool isVqCompressed(const ossimString& compressionCode)const;
   void vqUncompress(ossimRefPtr<ossimImageData> destination,
                     ossim_uint8* source);

   void lutUncompress(ossimRefPtr<ossimImageData> destination,
                     ossim_uint8* source);
   
   ossimRefPtr<ossimImageData>   theTile;
   ossimRefPtr<ossimImageData>   theCacheTile;
   ossimNitfFile*                theNitfFile;
   vector<ossimNitfImageHeader*> theNitfImageHeader;
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
   ifstream                      theFileStr;   
   vector<ossim_uint32>          theOutputBandList;
   ossimIpt                      theCacheSize;
   ossimInterleaveType           theCacheTileInterLeaveType;
   bool                          theCacheEnabledFlag;
   std::vector<ossim_uint32>     theEntryList;
   ossimAppFixedTileCache::ossimAppFixedCacheId theCacheId;
   bool                          thePackedBitsFlag;
   ossimIrect                    theBlockImageRect;
   ossim_uint8*                  theCompressedBuf;
TYPE_DATA
};
   
#endif /* #ifndef ossimNitfTileSource_HEADER */
