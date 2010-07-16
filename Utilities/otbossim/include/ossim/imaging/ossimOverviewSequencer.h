//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Class definition for sequencer for building overview files.
// 
//----------------------------------------------------------------------------
// $Id: ossimOverviewSequencer.h 17194 2010-04-23 15:05:19Z dburken $
#ifndef ossimOverviewSequencer_HEADER
#define ossimOverviewSequencer_HEADER

#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimMultiBandHistogram.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimFilterResampler.h>

class ossimFilename;
class ossimImageHandler;

/**
 * @class Sequencer for building overview files.
 */
class OSSIM_DLL ossimOverviewSequencer : public ossimReferenced
{
public:

   /** default constructor */
   ossimOverviewSequencer();
   
   /**
    * @return The number of tiles within the area of interest.
    */
   ossim_uint32 getNumberOfTiles()const;

   /**
    * @return The number of tiles along the x or horizontal direction.
    */
   ossim_uint32 getNumberOfTilesHorizontal()const;

   /**
    * @return The number of tiles along the y or vertical direction.
    */
   ossim_uint32 getNumberOfTilesVertical()const;

   /**
    * @brief Gets the zero-based image rectangle for the output reduced
    * resolution data set (rrds).
    *
    * @param rect The rectangle to initialize.
    */
   virtual void getOutputImageRectangle(ossimIrect& rect) const; 

   /**
    * @brief Sets the input source or connection.
    * @param input Pointer to ossimImageHandler.
    * @note This object does not own the image handler.
    */
   void setImageHandler(ossimImageHandler* input);

   /**
    * @brief Sets the input source resolution to decimate from.
    * @param level The level to decimate.
    */
   void setSourceLevel(ossim_uint32 level);

   /**
    * @brief Gets the histogram accumulation mode.
    * @return mode UNKNOWN, NORMAL or FAST.
    */
   ossimHistogramMode getHistogramMode() const;

   /**
    * @brief Sets the histogram accumulation mode.
    * @param mode NONE, NORMAL or FAST.
    */
   void setHistogramMode(ossimHistogramMode mode);

   /**
    * @brief Write histogram method.
    */
   void writeHistogram();

   /**
    * @brief Write histogram method that takes a file name.
    * @param file File to write to.
    */
   void writeHistogram(const ossimFilename& file);

   /**
    * This must be called.  We can only initialize this
    * object completely if we know all connections
    * are valid.  Some other object drives this and so the
    * connection's initialize will be called after.  The job
    * of this connection is to set up the sequence.  It will
    * default to the bounding rect.  The area of interest can be
    * set to some other rectagle (use setAreaOfInterest).
    */
   virtual void initialize();

   /**
    * @brief Will set the internal pointers to the upperleft
    * tile number.  To go to the next tile in the sequence
    * just call getNextTile.
    */
   virtual void setToStartOfSequence();

   /**
    * Will allow you to get the next tile in the sequence.
    * Note the last tile returned will be an invalid
    * ossimRefPtr<ossimImageData>.  Callers should be able to do:
    * 
    * ossimRefPtr<ossimImageData> id = sequencer->getNextTile();
    * while (id.valid())
    * {
    *    doSomething;
    *    id = sequencer->getNextTile();
    * }
    * 
    */
   virtual ossimRefPtr<ossimImageData> getNextTile();

   /**
    * @brief This implementation does nothing.  Derived slave would use this
    * as a trigger to start resampling tiles.
    */
   virtual void slaveProcessTiles();

   /**
    * @return Always true for this implementation.
    */
   virtual bool isMaster()const;

   /** @return The tile size. */
   ossimIpt getTileSize() const;

   /**
    * @brief updated the tile size.  This will update the number of horizontal
    * and vertical tiles.
    * @param pt The tile size.  pt.x = width, pt.y = height
    * @param height The tile height.
    */
   void setTileSize(const ossimIpt& pt);

   /**
    * @brief Sets the resampling type.
    *
    * Supports BOX or NEAREST NEIGHBOR.
    * When indexed you should probably use nearest neighbor.
    * default = ossimFilterResampler::ossimFilterResampler_BOX
    *
    * @param resampleType The resampling method to use. 
    */ 
   void setResampleType(
      ossimFilterResampler::ossimFilterResamplerType resampleType);

protected:

   /** virtual destructor */
   virtual ~ossimOverviewSequencer();

   /**
    * @brief Gets the image rectangle for the input tile for
    * theCurrentTileNumber.
    * @param inputRect The rectangle to initialize.
    */
   void getInputTileRectangle(ossimIrect& inputRect) const;
   
   /**
    * @brief Gets the image rectangle for the output tile for
    * theCurrentTileNumber.
    * @param outputRect The rectangle to initialize.
    */
   void getOutputTileRectangle(ossimIrect& outputRect) const;

   /**
    * @brief Updates theNumberOfTilesHorizontal and theNumberOfTilesVertical.
    *
    * This required theAreaOfInterest and theTileSize to be set.
    */
   void updateTileDimensions();

   /**
    *  Resamples a patch of data.
    */
   void resampleTile(const ossimImageData* inputTile);

   template <class T> void  resampleTile(const ossimImageData* inputTile, T dummy);

   ossimImageHandler*          m_imageHandler;
   ossimRefPtr<ossimImageData> m_tile;
   ossimIrect                  m_areaOfInterest;
   ossimIpt                    m_tileSize;
   ossim_uint32                m_numberOfTilesHorizontal;
   ossim_uint32                m_numberOfTilesVertical;
   ossim_uint32                m_currentTileNumber;

   /** This is the resolution level to build overviews from. */
   ossim_uint32                m_sourceResLevel;

   /** Dirty flag - if true, this object is not initialized. */
   bool                        m_dirtyFlag;

   /** TODO make this handle any decimation.  Right now hard coded to two. */
   ossim_int32                 m_decimationFactor;

   /** Currently only handles NEAREST_NEIGHBOR and BOX (default = BOX) */
   ossimFilterResampler::ossimFilterResamplerType m_resampleType;

   ossimRefPtr<ossimMultiBandHistogram> m_histogram;

   ossimHistogramMode m_histoMode;

   /**
    * Used to determine which tiles to accumulate a histogram from.  If set to 1 every tile is
    * accumulated, 2 every other tile, 3 every 3rd tile, and so on.  Set in initialize method
    * based on mode and image size.
    */
   ossim_uint32 m_histoTileIndex;
};

#endif /* #ifndef ossimOverviewSequencer_HEADER */
