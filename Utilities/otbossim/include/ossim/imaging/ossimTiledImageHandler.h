//**************************************************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Oscar Kramer
// 
// Description:
//
// Image handler used  when the full image is distributed across multiple files,
// As is the case for tiled Quickbird imagery. This is not for use with band-separate formats,
// but only "spatially-separate" schemes. Throughout, MFT = multiple file tiles, the disk storage
// scheme handled by this class.
//
//**************************************************************************************************
// $Id: ossimTiledImageHandler.h 2644 2011-05-26 15:20:11Z oscar.kramer $
#ifndef ossimTiledImageHandler_HEADER
#define ossimTiledImageHandler_HEADER

#include <ossim/imaging/ossimImageHandler.h>

// Enable this flag to utilize the system of separate overviews for each tile-file
#define USING_SUB_OVRS 0

/**
 *  This class defines an abstract Handler which all image handlers(loaders)
 *  should derive from.
 */
class OSSIMDLLEXPORT ossimTiledImageHandler : public ossimImageHandler
{
public:
   //!  Constructor (default):
   ossimTiledImageHandler();
   
   //! Destructor:
   virtual ~ossimTiledImageHandler();
   
   //! @return Returns true on success, false on error.
   //! @note This method relies on the data member ossimImageData::theImageFile
   //! being set.  
   virtual bool open() = 0;

   //! Deletes the overview and clears the valid image vertices.  Derived
   //! classes should implement. */
   virtual void close();

   //! Derived classes must implement this method to be concrete.
   //! @return true if open, false if not.
   virtual bool isOpen()const;

   virtual ossim_uint32 getNumberOfInputBands() const;
   
   //! @param resLevel Reduced resolution level to return lines of.
   //! Default = 0
   //! @return The number of lines for specified reduced resolution level.
   virtual ossim_uint32 getNumberOfLines(ossim_uint32 resLevel = 0) const;

   //! @param resLevel Reduced resolution level to return samples of.
   //! Default = 0
   //! @return The number of samples for specified reduced resolution level.
   virtual ossim_uint32 getNumberOfSamples(ossim_uint32 resLevel = 0) const;
   
   //! Method to save the state of an object to a keyword list.
   //! Return true if ok or false on error.
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0)const;

   //! Method to the load (recreate) the state of an object from a keyword
   //! list.  Return true if ok or false on error.
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);

   //! Indicates whether or not the image is tiled internally.
   //! This implementation returns true if (getImageTileWidth() &&
   //! getImageTileHeight()) are both non zero.
   //! Override in the image handler if something else is desired.
   //! Returns true if tiled, false if not.
   virtual bool isImageTiled() const;

   //! Returns the tile width of the image or 0 if the image is not tiled.
   //! Note: this is not the same as the ossimImageSource::getTileWidth which
   //! returns the output tile width which can be different than the internal
   //! image tile width on disk.
   virtual ossim_uint32 getImageTileWidth() const;

   //! Returns the tile height of the image or 0 if the image is not tiled.
   //! Note: this is not the same as the ossimImageSource::getTileHeight which
   //! returns the output tile height which can be different than the internal
   //! image tile height on disk.
   virtual ossim_uint32 getImageTileHeight() const;

   //! Fills the requested tile by pulling pixels from multiple file tiles as needed.
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect, ossim_uint32 resLevel);

   //! Returns scalar type of first tile (should be the same for all tiles)
   virtual ossimScalarType getOutputScalarType() const;

   //! Returns overall bounding rect in image space.
   virtual ossimIrect getImageRectangle(ossim_uint32 resLevel = 0) const;

protected:

   //! Class for record of one tile file:
   class ossimTileFile
   {
   public:
      ossimTileFile() : overviewIsOpen(false) {}

      ossimFilename tileFilename;
      vector<ossimIrect>  subImageRects; // one rect per resolution level
      ossimRefPtr<ossimImageHandler> imageHandler;
      bool overviewIsOpen;
   };

   //!  Initialize tile buffer to match image datatype.
   void allocate();

  vector<ossimTileFile> m_tileFiles;
  ossimRefPtr<ossimImageData> m_tile;
  ossimRefPtr<ossimImageData> m_blankTile;
  ossimIrect  m_fullImgRect;
  ossim_uint32 m_startOvrResLevel;

#if USING_SUB_OVRS
public:
   //! Will build over files, one for each tile file.
   //! @param includeFullResFlag if true the full resolution layer will also
   //! be put in the overview format.  This is handy for inefficient formats.
   virtual bool buildOverview(
      ossimImageHandlerOverviewCompressionType compressionType = OSSIM_OVERVIEW_COMPRESSION_NONE,
      ossim_uint32 quality = 75,
      ossimFilterResampler::ossimFilterResamplerType resampleType = ossimFilterResampler::ossimFilterResampler_BOX,
      bool includeFullResFlag=false);

   virtual bool openOverview();
   virtual void closeOverview();

   //! This is not an allowed operation since there are many overviews (one per tile) and the
   //! precise overview being requested is ambiguous:
   virtual const ossimImageHandler* getOverview() const { return NULL; }

   //!  @return true if getNumberOfReducedResSets > 1, false if not.
   //!  @see getNumberOfReducedResSets()
   virtual bool hasOverviews() const;
   
   //! Returns the number of decimation (reduced resolution) levels. This is the minimum number
   //! among all sub-image tile-files.
   virtual ossim_uint32 getNumberOfDecimationLevels() const;

protected:
   //! Will complete the opening process. If there are overviews it will open them. 
   virtual void completeOpen();
  
   bool m_lockSubOvrs;

#endif /* #if USING_SUB_OVRS */

TYPE_DATA
};

#endif /* #ifndef ossimTiledImageHandler_HEADER */
