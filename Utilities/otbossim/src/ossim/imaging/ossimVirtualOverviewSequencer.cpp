//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description: Sequencer for building virtual overview.
// 
//----------------------------------------------------------------------------
// $Id: ossimVirtualOverviewSequencer.cpp 15833 2009-10-29 01:41:53Z eshirschorn $

#include <ossim/imaging/ossimVirtualOverviewSequencer.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/parallel/ossimMpi.h>

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimVirtualOverviewSequencer.cpp 15833 2009-10-29 01:41:53Z eshirschorn $";
#endif

#define DECIMATION_FACTOR 2

static ossimTrace traceDebug("ossimVirtualOverviewSequencer:debug");

ossimVirtualOverviewSequencer::ossimVirtualOverviewSequencer(
   ossimImageSource* inputSource,
   ossimObject* owner)
   :
   ossimImageSourceSequencer(inputSource,owner),
   m_Tile(),
   m_SourceResLevel(0),
   m_Initialized(false),
   m_ResampleType(ossimFilterResampler::ossimFilterResampler_BOX),
   m_FrameSize(OSSIM_DEFAULT_FRAME_WIDTH, OSSIM_DEFAULT_FRAME_HEIGHT),
   m_bDisableResampling(false)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimVirtualOverviewSequencer::ossimVirtualOverviewSequencer entered..."
         << "\nmpi rank:  "<< ossimMpi::instance()->getRank()
         << std::endl;
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)<< "OSSIM_ID:  " << OSSIM_ID
                                         << std::endl;
#endif
   }
}

ossimVirtualOverviewSequencer::~ossimVirtualOverviewSequencer()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimVirtualOverviewSequencer::~ossimVirtualOverviewSequencer entered..."
         << "\nmpi rank:  " << ossimMpi::instance()->getRank()
         << std::endl;
   }
}

void ossimVirtualOverviewSequencer::setDisableResamplingFlag( bool flag )
{
   m_bDisableResampling = flag;

   // Typically, if we are here when m_Initialized is already true, then
   // someone is making an override call. In that case, we've got to
   // ensure that initialize() is called again.
   if ( m_Initialized == true )
   {
      initialize();
   }
}

bool ossimVirtualOverviewSequencer::getDisableResamplingFlag( void ) const
{ return m_bDisableResampling; }

void ossimVirtualOverviewSequencer::getOutputImageRectangle(ossimIrect& rect) const
{
   if (theAreaOfInterest.hasNans())
   {
      rect.makeNan(); // not initialized...
   }
   else
   {
      ossim_int32 width  = theAreaOfInterest.width()  / DECIMATION_FACTOR;
      ossim_int32 height = theAreaOfInterest.height() / DECIMATION_FACTOR;

      //---
      // NOTE:
      // This will add a sample or line if there is a remainder like:
      // 1025 / 2 = 512 will then become 513.  It will be up to the resample
      // tile method to fill the extra sample correctly.
      //---
      if (theAreaOfInterest.width() % DECIMATION_FACTOR)
      {
         ++width;
      }
      if (theAreaOfInterest.height() % DECIMATION_FACTOR)
      {
         ++height;
      }
      
      rect = ossimIrect(0, 0, width-1, height-1);
   }
}

void ossimVirtualOverviewSequencer::setSourceLevel(ossim_uint32 level)
{
   m_SourceResLevel = level;
   theAreaOfInterest.makeNan();

   // Typically, if we are here when m_Initialized is already true, then
   // someone is making an override call. In that case, we've got to
   // ensure that initialize() is called again.
   if ( m_Initialized == true )
   {
      initialize();
   }
}

void ossimVirtualOverviewSequencer::initialize()
{
   if ( !theInputConnection )
   {
      return;
   }

   // Check the tile size and set from image if needed.
   if ( theTileSize.hasNans() )
   {
      theTileSize.x = theInputConnection->getTileWidth();
      theTileSize.y = theInputConnection->getTileHeight();
   }

   // Update theNumberOfTilesHorizontal and theNumberOfTilesVertical.
   updateTileDimensions();

   // Start on first tile.
   theCurrentTileNumber = 0;
      
   m_Tile  = ossimImageDataFactory::instance()->
      create( theInputConnection,
              theInputConnection->getOutputScalarType(),
              theInputConnection->getNumberOfOutputBands(),
              static_cast<ossim_uint32>(theTileSize.x),
              static_cast<ossim_uint32>(theTileSize.y) );
   if(m_Tile.valid())
   {
      m_Tile->initialize();
   }

   m_Initialized = true;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "aoi:                  " << theAreaOfInterest
         << "\ntile size:          " << theTileSize
         << "\ntiles wide:         " << theNumberOfTilesHorizontal
         << "\ntiles high:         " << theNumberOfTilesVertical
         << "\nsource rlevel:      " << m_SourceResLevel
         << "\nresamp type:        " << m_ResampleType
         << std::endl;
   }
}

ossimIpt ossimVirtualOverviewSequencer::getFrameSize()const
{
   return m_FrameSize;
}

void ossimVirtualOverviewSequencer::setFrameSize(const ossimIpt& frameSize)
{
   m_FrameSize = frameSize;

   // Typically, if we are here when m_Initialized is already true, then
   // someone is making an override call. In that case, we've got to
   // ensure that initialize() is called again.
   if ( m_Initialized == true )
   {
      initialize();
   }
}

// ESH 08/2009: Adding support for non-sequential tile access
ossim_uint32 ossimVirtualOverviewSequencer::getCurrentTileNumber() const
{
   return theCurrentTileNumber;
}

// ESH 08/2009: Adding support for non-sequential tile access
void ossimVirtualOverviewSequencer::setCurrentTileNumber( ossim_uint32 tileNumber )
{
   theCurrentTileNumber = tileNumber;
}

ossimRefPtr<ossimImageData> ossimVirtualOverviewSequencer::getNextTile( ossim_uint32 resLevel )
{
   // Note that 'resLevel' is not used in this routine, since m_SourceResLevel
   // is used instead.

   // Get the output rectangle.
   ossimIrect outputRect;
   getOutputTileRectangle(outputRect);

   if ( m_bDisableResampling == false )
   {
      // Get the rectangle to grab from the image.
      ossimIrect inputRect;
      getInputTileRectangle(inputRect);

      // Capture the output rectangle.
      m_Tile->setImageRectangle(outputRect);

      // Start with a blank tile.
      m_Tile->makeBlank();

      // Grab the input tile.
      ossimRefPtr<ossimImageData> inputTile =
         theInputConnection->getTile(inputRect, m_SourceResLevel);
      if (inputTile.valid() == false)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimVirtualOverviewSequencer::getNextTile DEBUG:"
            << "\nRequest failed for input rect: " << inputRect
            << "\nRes level:  " << m_SourceResLevel << std::endl;
      }

      if ( (inputTile->getDataObjectStatus() == OSSIM_PARTIAL) ||
           (inputTile->getDataObjectStatus() == OSSIM_FULL ) )
      {
         // Resample the tile.
         resampleTile(inputTile.get());

         // Set the tile status.
         m_Tile->validate();
      }

      // Increment the tile index.
      ++theCurrentTileNumber;

      return m_Tile;
   }

   // Grab the input tile but do not resample.
   ossimRefPtr<ossimImageData> outputTile =
      theInputConnection->getTile( outputRect, m_SourceResLevel );
   if ( outputTile.valid() == false )
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimVirtualOverviewSequencer::getNextTile DEBUG:"
         << "\nRequest failed for input rect: " << outputRect
         << "\nRes level:  " << m_SourceResLevel << std::endl;
   }

   // Increment the tile index.
   ++theCurrentTileNumber;

   return outputTile;
}

void ossimVirtualOverviewSequencer::setTileSize(const ossimIpt& tileSize)
{
   theTileSize = tileSize;
   updateTileDimensions();

   // Typically, if we are here when m_Initialized is already true, then
   // someone is making an override call. In that case, we've got to
   // ensure that initialize() is called again.
   if ( m_Initialized == true )
   {
      initialize();
   }
}

void ossimVirtualOverviewSequencer::setResampleType(
   ossimFilterResampler::ossimFilterResamplerType resampleType)
{
   m_ResampleType = resampleType;
}

void ossimVirtualOverviewSequencer::getInputTileRectangle( ossimIrect& inputRect ) const
{
   if (!theInputConnection) return;
   
   getOutputTileRectangle(inputRect);
   inputRect = inputRect * DECIMATION_FACTOR;
}

void ossimVirtualOverviewSequencer::getOutputTileRectangle(
   ossimIrect& outputRect) const
{
   ossim_int32 virtual_minx, virtual_miny, virtual_maxx, virtual_maxy;
   theAreaOfInterest.getBounds( virtual_minx, virtual_miny, 
                                virtual_maxx, virtual_maxy );

   ossimIpt pt_ul;
   ossimIpt pt_lr;

   if ( m_FrameSize.y >= theTileSize.y )
   {
      // Get the relative row with respect to the frame origin.
      ossim_int32 delta_row = theCurrentTileNumber / theNumberOfTilesHorizontal;
      // Set the upper left.
      pt_ul.y = virtual_miny + delta_row * theTileSize.y;
      // Set the lower right.
      pt_lr.y = pt_ul.y + theTileSize.y - 1;
   }
   else
   {
      // Set the upper left.
      pt_ul.y = virtual_miny;
      // Set the lower right.
      pt_lr.y = virtual_maxy;
   }

   if ( m_FrameSize.x >= theTileSize.x )
   {
      // Get the relative column with respect to the frame origin.
      ossim_int32 delta_col = theCurrentTileNumber % theNumberOfTilesHorizontal;
      // Set the upper left.
      pt_ul.x = virtual_minx + delta_col * theTileSize.x;
      // Set the lower right.
      pt_lr.x = pt_ul.x + theTileSize.x - 1;
   }
   else
   {
      // Set the upper left.
      pt_ul.x = virtual_minx;
      // Set the lower right.
      pt_lr.x = virtual_maxx;
   }

   outputRect.set_ul(pt_ul);
   outputRect.set_lr(pt_lr);
}

void ossimVirtualOverviewSequencer::resampleTile(const ossimImageData* inputTile)
{
   switch(theInputConnection->getOutputScalarType())
   {
      case OSSIM_UINT8:
      {
         resampleTile(inputTile, ossim_uint8(0));
         break;
      }

      case OSSIM_USHORT11:
      case OSSIM_UINT16:
      {
         resampleTile(inputTile, ossim_uint16(0));
         break;
      }
      case OSSIM_SINT16:
      {
         resampleTile(inputTile, ossim_sint16(0));
         break;
      }

      case OSSIM_UINT32:
      {
         resampleTile(inputTile, ossim_uint32(0));
         break;
      }
      
      case OSSIM_FLOAT32:
      {
         resampleTile(inputTile, ossim_float32(0.0));
         break;
      }
         
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
      {
         resampleTile(inputTile, ossim_float64(0.0));
         break;
      }
      default:
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimVirtualOverviewSequencer::resampleTile Unknown pixel type!"
            << std::endl;
         return;
         
   } // End of "switch(theImageHandler->getOutputScalarType())"
}

template <class T>
void  ossimVirtualOverviewSequencer::resampleTile(const ossimImageData* inputTile, T  /* dummy */ )
{
   const ossim_uint32 BANDS = m_Tile->getNumberOfBands();
   const ossim_uint32 LINES = m_Tile->getHeight();
   const ossim_uint32 SAMPS = m_Tile->getWidth();

   const ossim_uint32 OUTPUT_WIDTH = (m_FrameSize.x>=theTileSize.x) ? 
                                       theTileSize.x : SAMPS;
   const ossim_uint32 INPUT_WIDTH  = DECIMATION_FACTOR * OUTPUT_WIDTH;

   T nullPixel             = 0;
   ossim_float64 weight    = 0.0;
   ossim_float64 val       = 0.0;
   ossim_uint32 sampOffset = 0;

   if (m_ResampleType ==
       ossimFilterResampler::ossimFilterResampler_NEAREST_NEIGHBOR)
   {
      for (ossim_uint32 band=0; band<BANDS; ++band)
      {
         const T* s = static_cast<const T*>(inputTile->getBuf(band)); // source
         T*       d = static_cast<T*>(m_Tile->getBuf(band)); // destination

         nullPixel = static_cast<T>(inputTile->getNullPix(band));
         weight = 0.0;
         val    = 0.0;

         for (ossim_uint32 i=0; i<LINES; ++i)
         {
            for (ossim_uint32 j=0; j<SAMPS; ++j)
            {
               sampOffset = j*DECIMATION_FACTOR;

               weight = 1.0;
               val    = *(s + i*DECIMATION_FACTOR*INPUT_WIDTH + sampOffset);

               if(weight)
               {
                  d[j] = static_cast<T>( val/weight );
               }
               else
               {
                  d[j] = nullPixel;
               }

            } // End of sample loop.

            d += SAMPS; //theTileSize.x;

         } // End of line loop.

      } // End of band loop.

   }
   else // ossimFilterResampler::ossimFilterResampler_BOX
   {
      ossim_uint32 lineOffset1 = 0;
      ossim_uint32 lineOffset2 = 0;
      ossim_float64 ul = 0.0;
      ossim_float64 ur = 0.0;
      ossim_float64 ll = 0.0;
      ossim_float64 lr = 0.0;

      for (ossim_uint32 band=0; band<BANDS; ++band)
      {
         const T* s = static_cast<const T*>(inputTile->getBuf(band)); // source
         T*       d = static_cast<T*>(m_Tile->getBuf(band)); // destination

         nullPixel = static_cast<T>(inputTile->getNullPix(band));
         weight = 0.0;
         val    = 0.0;

         for (ossim_uint32 i=0; i<LINES; ++i)
         {
            lineOffset1 = i*DECIMATION_FACTOR*INPUT_WIDTH;
            lineOffset2 = (i*DECIMATION_FACTOR+1)*INPUT_WIDTH;

            for (ossim_uint32 j=0; j<SAMPS; ++j)
            {
               sampOffset = j*DECIMATION_FACTOR;

               weight = 0.0;
               val    = 0.0;

               //---
               // Grab four pixels from the source, average, and assign
               // to output.
               //---
               ul = *(s + lineOffset1 + sampOffset);
               ur = *(s + lineOffset1 + sampOffset + 1);
               ll = *(s + lineOffset2 + sampOffset);
               lr = *(s + lineOffset2 + sampOffset + 1);

               if(ul != nullPixel)
               {
                  ++weight;
                  val += ul;
               }
               if(ur != nullPixel)
               {
                  ++weight;
                  val += ur;
               }
               if(ll != nullPixel)
               {
                  ++weight;
                  val += ll;
               }
               if(lr != nullPixel)
               {
                  ++weight;
                  val += lr;
               }

               if(weight)
               {
                  d[j] = static_cast<T>( val/weight );
               }
               else
               {
                  d[j] = nullPixel;
               }

            } // End of sample loop.

            d += SAMPS; //theTileSize.x;

         } // End of line loop.

      } // End of band loop.
   }
}
