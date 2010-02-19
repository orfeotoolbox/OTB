//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  Eric Hirschorn
//
// Description: Class definition for sequencer for building virtual overviews.
// 
//----------------------------------------------------------------------------
// $Id: ossimVirtualOverviewSequencer.h 15833 2009-10-29 01:41:53Z eshirschorn $
#ifndef ossimVirtualOverviewSequencer_HEADER
#define ossimVirtualOverviewSequencer_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimFilterResampler.h>
#include <ossim/imaging/ossimImageSourceSequencer.h>

#define OSSIM_DEFAULT_FRAME_HEIGHT  ((ossim_int32)128)
#define OSSIM_DEFAULT_FRAME_WIDTH   ((ossim_int32)128)

class ossimImageHandler;

/**
 * @class Sequencer for building virtual overviews.
 */
class OSSIM_DLL ossimVirtualOverviewSequencer : public ossimImageSourceSequencer
{
public:
   
   /** default constructor */
   ossimVirtualOverviewSequencer( ossimImageSource* inputSource=NULL,
                                  ossimObject* owner=NULL );

   /**
    * @brief Gets the zero-based image rectangle for the output reduced
    * resolution data set (rrds).
    *
    * @param rect The rectangle to initialize.
    */
   virtual void getOutputImageRectangle( ossimIrect& rect ) const; 

   /**
    * @brief Sets the input source resolution to decimate from.
    * @param level The level to decimate.
    */
   void setSourceLevel( ossim_uint32 level );

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
   virtual ossimRefPtr<ossimImageData> getNextTile( ossim_uint32 resLevel=0 );

   /**
    * @brief updated the tile size.  This will update the number of horizontal
    * and vertical tiles.
    * @param pt The tile size.  pt.x = width, pt.y = height
    */
   virtual void setTileSize( const ossimIpt& pt );

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
      ossimFilterResampler::ossimFilterResamplerType resampleType );

   /**
    * @return the image frame size generated. 
    */
   ossimIpt getFrameSize()const;

   /**
    * @brief updated the image frame size generated. 
    * @param pt The frame size.  pt.x = width, pt.y = height
    */
   void setFrameSize( const ossimIpt& pt );

   /**
    * @return The index location of the current tile.
    */ 
   ossim_uint32 getCurrentTileNumber() const;

   /**
    * @brief Will set the internal pointers to the specified
    * tile number.  To get the data for this tile number and then 
    * to set up to the next tile in the sequence just call 
    * getNextTile.
    * @param tileNumber The index location of the desired tile.
    */
   void setCurrentTileNumber( ossim_uint32 tileNumber );

   /**
    * @brief Use this routine if decimation resampling isn't required.
    * @param flag If set to true, the data is not resampled.
    */
   void setDisableResamplingFlag( bool flag = true );

   /**
    * @return The boolean flag for disabling resampling.
    */ 
   bool getDisableResamplingFlag( void ) const;

protected:
   /** virtual destructor */
   virtual ~ossimVirtualOverviewSequencer();

   /**
    * @brief Gets the image rectangle for the input tile for
    * theCurrentTileNumber.
    * @param inputRect The rectangle to initialize.
    */
   void getInputTileRectangle( ossimIrect& inputRect ) const;

   /**
    * @brief Gets the image rectangle for the output tile for
    * theCurrentTileNumber.
    * @param outputRect The rectangle to initialize.
    */
   void getOutputTileRectangle( ossimIrect& outputRect ) const;

   /**
    *  Resamples a patch of data.
    */
   void resampleTile( const ossimImageData* inputTile );

   template <class T> void resampleTile( const ossimImageData* inputTile,
                                         T dummy );

   ossimRefPtr<ossimImageData> m_Tile;

   /** This is the resolution level to build overviews from. */
   ossim_uint32                m_SourceResLevel;

   /** If true, the instance has been initialized. */
   bool                        m_Initialized;

   /** Currently only handles NEAREST_NEIGHBOR and BOX (default = BOX) */
   ossimFilterResampler::ossimFilterResamplerType m_ResampleType;
   
   /** Size of the image frame being generated by the virtual writer. */
   ossimIpt                    m_FrameSize;

   /** Set this to true if resampling isn't necessary. Default=false */
   bool                        m_bDisableResampling;
};

#endif /* #ifndef ossimVirtualOverviewSequencer_HEADER */
