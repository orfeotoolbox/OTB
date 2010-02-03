//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Eric Hirschorn
//
// Description:
//
// Contains class declaration for ossimVirtualImageHandler.
// ossimVirtualImageHandler is derived from ImageHandler which is 
// derived from ImageSource.
//*******************************************************************
//  $Id: ossimVirtualImageHandler.h 14655 2009-06-05 11:58:56Z dburken $

#ifndef ossimVirtualImageHandler_HEADER
#define ossimVirtualImageHandler_HEADER

#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimIrect.h>
#include <tiffio.h>

class ossimImageData;

class OSSIMDLLEXPORT ossimVirtualImageHandler : public ossimImageHandler
{
public:

   enum ReadMethod
   {
      UNKNOWN,
      READ_RGBA_U8_TILE,
      READ_RGBA_U8_STRIP,
      READ_RGBA_U8A_STRIP,
      READ_SCAN_LINE,
      READ_TILE
   };

   ossimVirtualImageHandler();
	
   virtual ~ossimVirtualImageHandler();

   virtual ossimString getLongName()  const;
   virtual ossimString getShortName() const;

   /**
    *  Returns true if the image_file can be opened and is a valid tiff file.
    */
  virtual bool open( const ossimFilename& image_file );
  virtual void close();
  virtual bool isOpen()const;

   /**
    *  Returns a pointer to a tile given an origin representing the upper left
    *  corner of the tile to grab from the image.
    *  Satisfies pure virtual from TileSource class.
    */
   virtual ossimRefPtr<ossimImageData> getTile( const ossimIrect& rect,
                                                ossim_uint32 resLevel=0 );

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
   virtual bool getTile(ossimImageData& result, ossim_uint32 resLevel=0);
   
   /**
     *  Returns the number of bands in the image.
     *  Satisfies pure virtual from ImageHandler class.
     */
   virtual ossim_uint32 getNumberOfInputBands() const;
   virtual ossim_uint32 getNumberOfOutputBands () const;
   
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
    * Returns the zero-based (relative) image rectangle for the reduced
    * resolution data set (rrds) passed in.  Note that rrds 0 is the highest
    * resolution rrds.
    */
   virtual ossimIrect getImageRectangle(ossim_uint32 resLevel = 0) const; 

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
    * Returns the output pixel type of the tile source.
    */
   virtual ossimScalarType getOutputScalarType() const;

   /**
    * Returns the width of the tiles within a frame file.
    */
   virtual ossim_uint32 getTileWidth() const;
   
   /**
    * Returns the height of the tiles within a frame file.
    */
   virtual ossim_uint32 getTileHeight() const;

   /**
    * Returns the width of the frame files.
    */
   virtual ossim_uint32 getFrameWidth() const;
   
   /**
    * Returns the height of the frame files.
    */
   virtual ossim_uint32 getFrameHeight() const;

   /**
    *  Returns true if the virtual image has a copy of the 
    *  highest resolution imagery from the source data.
    */
   bool hasR0() const;
  
   virtual double getMinPixelValue( ossim_uint32 band=0 )const;
   virtual double getMaxPixelValue( ossim_uint32 band=0 )const;
   
   virtual bool isValidRLevel( ossim_uint32 resLevel ) const;

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

   virtual void setProperty( ossimRefPtr<ossimProperty> property );
   virtual ossimRefPtr<ossimProperty> getProperty( const ossimString& name )const;
   virtual void getPropertyNames( std::vector<ossimString>& propertyNames )const;
   
   virtual std::ostream& print( std::ostream& os ) const;

protected:

   /**
    *  Returns true if no errors.
    */
   bool open();

   bool allocateBuffer();

   bool loadTile( const ossimIrect& clip_rect,
                  ossimImageData& result,
                  ossim_uint32 resLevel );

   virtual bool initializeBuffers();

   /**
    * @brief validateMinNax Checks min and max to make sure they are not equal
    * to the scalar type nan or double nan; sets to default min max if so.
    */
   void validateMinMax();

   /**
    *  Retrieves the virtual image header info from a keywordlist.
    *
    *  @param kwl A keywordlist where the header info is stored.
    *  @return true on success, false on error.
    */
   virtual bool loadHeaderInfo( const ossimKeywordlist& kwl );

   /**
    *  Retrieves the virtual image -specific information for
    *  a single image entry from the keywordlist.
    */
   void loadNativeKeywordEntry( const ossimKeywordlist& kwl,
                                const ossimString& prefix );

   /**
    *  Retrieves the geometry information for a single image entry
    *  from the keywordlist.
    */
   void loadGeometryKeywordEntry( const ossimKeywordlist& kwl,
                                  const ossimString& prefix );

   /**
    *  Retrieves general image information for a single image entry
    *  from the keywordlist.
    */
   void loadGeneralKeywordEntry( const ossimKeywordlist& kwl, 
                                 const ossimString& prefix );

   /**
    * Grab the null, min, and max values from the input keywordlist.
    */
   void loadMetaData( const ossimKeywordlist& kwl );

   /**
    *  Opens a tiff file for a single output frame for reading.
    *
    *  @param resLevel The zero-based resolution level of the frame
    *  @param row The zero-based row at which the frame is located
    *  @param col The zero-based column at which the frame is located
    *  @return true on success, false on error.
    */
   bool openTiff( int resLevel, int row, int col );

   /**
    *  Close the currently open tiff file.
    *  @return true on success, false on error.
    */
   bool closeTiff();

   /**
    *  Calculates and returns the number of tiles in x,y that a
    *  single frame of the virtual image contain.
    *
    *  @return the number of tiles in x,y directions.
    */
   ossimIpt getNumberOfTilesPerFrame() const;

   ossim_uint8*                theBuffer;
   ossim_uint32                theBufferSize;
   ossimIrect                  theBufferRect;
   ossim_uint8*                theNullBuffer;
   ossim_uint16                theSampleFormatUnit;
   double                      theMaxSampleValue;
   double                      theMinSampleValue;
   ossim_uint16                theBitsPerSample;
   ossim_uint32                theBytesPerPixel;
   ossimFilename               theImageSubdirectory;
   ossimFilename               theCurrentFrameName;
   ossimString                 theVirtualWriterType;
   ossimString                 theMajorVersion;
   ossimString                 theMinorVersion;
   ossim_uint16                theCompressType;
   ossim_int32                 theCompressQuality;
   bool                        theOverviewFlag;
   bool                        theOpenedFlag;
   bool                        theR0isFullRes;
   ossim_int16                 theEntryIndex;
   ossim_uint16                theResLevelStart;
   ossim_uint16                theResLevelEnd;
   ossim_uint16                theSamplesPerPixel;
   ossim_uint16                theNumberOfResLevels;
   ossim_uint16                thePlanarConfig;
   ossimScalarType             theScalarType;
   vector<ossimIpt>            theNumberOfFrames;
   ReadMethod                  theReadMethod;
   ossim_int32                 theImageTileWidth;
   ossim_int32                 theImageTileLength;
   ossim_int32                 theImageFrameWidth;
   ossim_int32                 theImageFrameLength;
   ossim_int32                 theR0NumberOfLines;
   ossim_int32                 theR0NumberOfSamples;
   ossim_uint16                thePhotometric;
   TIFF*                       theTif;
   ossimRefPtr<ossimImageData> theTile;
   vector<ossim_uint32>        theImageWidth;
   vector<ossim_uint32>        theImageLength;

  TYPE_DATA
};

#endif
