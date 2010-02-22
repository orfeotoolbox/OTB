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

class ossimImageData;

// Helper struct for organizing frame handlers.
struct FrameHandlerInfo
{
   ossimString ext;
   ossimImageHandler* pHandler;
};

class OSSIMDLLEXPORT ossimVirtualImageHandler : public ossimImageHandler
{
public:

   /**
    * Constructor for the virtual image handler.   
    *
    * @param srcResLevel The resolution level to access from the
    * frame images. If not zero, this assumes that overviews are
    * available to access.
    */
   ossimVirtualImageHandler( ossim_uint32 srcResLevel = 0 );
	
   virtual ~ossimVirtualImageHandler();

   virtual ossimString getLongName()  const;
   virtual ossimString getShortName() const;

   /**
    *  Returns true if the virtual image can be opened and is valid.
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
    * Returns the width of the frame files.
    */
   virtual ossim_uint32 getFrameWidth() const;
   
   /**
    * Returns the height of the frame files.
    */
   virtual ossim_uint32 getFrameHeight() const;

   /**
    * Returns the number of frames in the X-direction
    * that were in the original source data.
    */
   virtual ossim_uint32 getNumberInputFramesX() const;
   
   /**
    * Returns the number of frames in the Y-direction
    * that were in the original source data.
    */
   virtual ossim_uint32 getNumberInputFramesY() const;

   /**
    *  Returns true if the virtual image has a copy of the 
    *  highest resolution imagery from the source data.
    */
   bool hasR0() const;

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

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

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
   void loadMetaData( const ossimKeywordlist& kwl,
                      const char* prefix );

   /**
    *  Opens an image frame file for reading.
    *
    *  @param resLevel The zero-based resolution level of the frame
    *  @param row The zero-based row at which the frame is located
    *  @param col The zero-based column at which the frame is located
    *  @return the image handler that was used to successfully
    *  open the file.
    */
   ossimImageHandler* openFrame( int resLevel, int row, int col );

   /**
    *  Returns an instance of a FrameHandlerInfo containing an image handler 
    *  and the respective file extension if the given file can be successfully 
    *  opened. 
    *  The list of frame handlers (m_FrameHandlerInfoList) is tried first, and 
    *  if that fails all handler-types available in OSSIM are tried. If a new 
    *  image handler is created, it is added to the internal list.
    *
    *  @return a FrameHandlerInfo containing the image handler that was used 
    *  to successfully open the file.
    */
   FrameHandlerInfo* resolveFrameHandler( const ossimFilename& f );

   ossim_uint8*                    m_Buffer;
   ossim_uint32                    m_BufferSize;
   ossimIrect                      m_BufferRect;
   ossim_uint8*                    m_NullBuffer;
   ossim_uint16                    m_SampleFormatUnit;
   ossim_uint16                    m_BitsPerSample;
   ossim_uint32                    m_BytesPerPixel;
   ossimFilename                   m_ImageSubdirectory;
   ossimString                     m_VirtualWriterType;
   ossimString                     m_MajorVersion;
   ossimString                     m_MinorVersion;
   ossim_uint16                    m_CompressType;
   ossim_int32                     m_CompressQuality;
   bool                            m_OverviewFlag;
   bool                            m_OpenedFlag;
   bool                            m_R0isFullRes;
   ossim_int16                     m_EntryIndex;
   ossim_uint16                    m_ResLevelStart;
   ossim_uint16                    m_ResLevelEnd;
   ossim_uint16                    m_SamplesPerPixel;
   ossim_uint16                    m_NumberOfResLevels;
   ossimInterleaveType             m_InterleaveType;
   ossimScalarType                 m_ScalarType;
   ossim_int32                     m_ImageFrameWidth;
   ossim_int32                     m_ImageFrameLength;
   ossim_int32                     m_R0NumberOfLines;
   ossim_int32                     m_R0NumberOfSamples;
   ossimRefPtr<ossimImageData>     m_Tile;
   vector<ossim_uint32>            m_ImageWidth;
   vector<ossim_uint32>            m_ImageLength;
   ossimString                     m_CurrentFrameExt;
   std::vector<FrameHandlerInfo*>  m_FrameHandlerInfoList;
   ossim_uint32                    m_CurrentTileWidth;
   ossim_uint32                    m_CurrentTileHeight;
   ossim_int32                     m_CurrentResLevel;
   ossim_int32                     m_CurrentRow;
   ossim_int32                     m_CurrentCol;
   ossimImageHandler*              m_CurrentImageHandler;
   ossim_uint32                    m_SourceResLevel;
   ossim_uint32                    m_NumberInputFramesX;
   ossim_uint32                    m_NumberInputFramesY;

  TYPE_DATA
};

#endif
