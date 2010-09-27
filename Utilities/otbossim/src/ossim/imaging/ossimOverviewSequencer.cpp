//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description: Sequencer for building overview files.
// 
//----------------------------------------------------------------------------
// $Id: ossimOverviewSequencer.cpp 17928 2010-08-19 18:43:53Z gpotts $

#include <ossim/imaging/ossimOverviewSequencer.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimMultiResLevelHistogram.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/parallel/ossimMpi.h>


#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimOverviewSequencer.cpp 17928 2010-08-19 18:43:53Z gpotts $";
#endif

static ossimTrace traceDebug("ossimOverviewSequencer:debug");

ossimOverviewSequencer::ossimOverviewSequencer()
   :
   ossimReferenced(),
   m_imageHandler(0),
   m_tile(0),
   m_areaOfInterest(),
   m_tileSize(OSSIM_DEFAULT_TILE_WIDTH, OSSIM_DEFAULT_TILE_HEIGHT),
   m_numberOfTilesHorizontal(0),
   m_numberOfTilesVertical(0),
   m_currentTileNumber(0),
   m_sourceResLevel(0),
   m_dirtyFlag(true),
   m_decimationFactor(2),
   m_resampleType(ossimFilterResampler::ossimFilterResampler_BOX),
   m_histogram(0),
   m_histoMode(OSSIM_HISTO_MODE_UNKNOWN),
   m_histoTileIndex(1)
{
   m_areaOfInterest.makeNan();

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimOverviewSequencer::ossimOverviewSequencer entered..."
         << "\nmpi rank:  "<< ossimMpi::instance()->getRank()
         << std::endl;
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)<< "OSSIM_ID:  " << OSSIM_ID
                                         << std::endl;
#endif
   }
}

ossimOverviewSequencer::~ossimOverviewSequencer()
{
   m_imageHandler = 0;
   m_tile         = 0;
   m_histogram    = 0;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimOverviewSequencer::~ossimOverviewSequencer entered..."
         << "\nmpi rank:  " << ossimMpi::instance()->getRank()
         << std::endl;
   }
}

ossim_uint32 ossimOverviewSequencer::getNumberOfTiles() const
{
   return (m_numberOfTilesHorizontal*m_numberOfTilesVertical);
}

ossim_uint32 ossimOverviewSequencer::getNumberOfTilesHorizontal()const
{
   return m_numberOfTilesHorizontal;
}

ossim_uint32 ossimOverviewSequencer::getNumberOfTilesVertical()const
{
   return m_numberOfTilesVertical;
}

void ossimOverviewSequencer::getOutputImageRectangle(ossimIrect& rect) const
{
   if (m_areaOfInterest.hasNans())
   {
      rect.makeNan(); // not initialized...
   }
   else
   {
      ossim_int32 width  = m_areaOfInterest.width()  / m_decimationFactor;
      ossim_int32 height = m_areaOfInterest.height() / m_decimationFactor;

      //---
      // NOTE:
      // This will add a sample or line if there is a remainder like:
      // 1025 / 2 = 512 will then become 513.  It will be up to the resample
      // tile method to fill the extra sample correctly.
      //---
      if (m_areaOfInterest.width() % m_decimationFactor)
      {
         ++width;
      }
      if (m_areaOfInterest.height() % m_decimationFactor)
      {
         ++height;
      }
      
      rect = ossimIrect(0, 0, width-1, height-1);
   }
}

void ossimOverviewSequencer::setImageHandler(ossimImageHandler* input)
{
   m_imageHandler = input;
   m_areaOfInterest.makeNan();
   m_dirtyFlag = true;
}

void ossimOverviewSequencer::setSourceLevel(ossim_uint32 level)
{
   m_sourceResLevel = level;
   m_areaOfInterest.makeNan();
   m_dirtyFlag = true;
}

ossimHistogramMode ossimOverviewSequencer::getHistogramMode() const
{
   return m_histoMode;
}

void ossimOverviewSequencer::setHistogramMode(ossimHistogramMode mode)
{
   m_histoMode = mode;
   m_dirtyFlag = true;
}

void ossimOverviewSequencer::writeHistogram()
{
   if ( m_histogram.valid() && m_imageHandler.valid() )
   {
      writeHistogram( m_imageHandler->getFilenameWithThisExtension("his") );
   }
}

void ossimOverviewSequencer::writeHistogram(const ossimFilename& file)
{
   if ( m_histogram.valid() )
   {
      ossimRefPtr<ossimMultiResLevelHistogram> histo = new ossimMultiResLevelHistogram;
      histo->addHistogram( m_histogram.get() );
      ossimKeywordlist kwl;
      histo->saveState(kwl);
      kwl.write(file.c_str());
   }
}

void ossimOverviewSequencer::initialize()
{
   if ( m_dirtyFlag == false )
   {
      return; // Already initialized.
   }
   
   if ( !m_imageHandler )
   {
      return;
   }

   // Check the area of interest and set from image if needed.
   if ( m_areaOfInterest.hasNans() )
   {
      m_areaOfInterest = m_imageHandler->getImageRectangle(m_sourceResLevel);
   }

   // Check the tile size and set from image if needed.
   if ( m_tileSize.hasNans() )
   {
      m_tileSize.x = m_imageHandler->getTileWidth();
      m_tileSize.y = m_imageHandler->getTileHeight();
   }

   // Update m_numberOfTilesHorizontal and m_numberOfTilesVertical.
   updateTileDimensions();

   // Start on first tile.
   m_currentTileNumber = 0;

   // Use this factory constructor as it copies the min/max/nulls from the image handler.
   m_tile = ossimImageDataFactory::instance()->
      create( 0, m_imageHandler->getNumberOfOutputBands(), m_imageHandler.get());
   
   if(m_tile.valid())
   {
      // Set the width and height.
      m_tile->setWidthHeight(static_cast<ossim_uint32>(m_tileSize.x),
                             static_cast<ossim_uint32>(m_tileSize.y) );
      // Initialize tile buffer.
      m_tile->initialize();
   }

   if (m_histoMode != OSSIM_HISTO_MODE_UNKNOWN)
   {
      m_histogram = new ossimMultiBandHistogram;
      
      m_histogram->create(m_imageHandler.get());

      if (m_histoMode == OSSIM_HISTO_MODE_NORMAL)
      {
         m_histoTileIndex = 1; // Sample every tile.
      }
      else
      {
         const ossim_float64 PIXEL_TO_SAMPLE = 100.0 * 256.0 * 256.0; // 100 256x256 tiles.
         ossim_float64 pixels = m_tileSize.x * m_tileSize.y * getNumberOfTiles();

         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "pixels: " << pixels << "\n"
               << "PIXEL_TO_SAMPLE: " << PIXEL_TO_SAMPLE << "\n";
         }
         
         if (pixels > PIXEL_TO_SAMPLE)
         {
            m_histoTileIndex = ossim::round<ossim_uint32>(pixels/PIXEL_TO_SAMPLE);
         }
         else
         {
            m_histoTileIndex = 1; // Sample every tile.
         }
      }
   }
   else
   {
      m_histogram = 0;
   }
   
   m_dirtyFlag = false;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "aoi:                  " << m_areaOfInterest
         << "\ntile size:          " << m_tileSize
         << "\ntiles wide:         " << m_numberOfTilesHorizontal
         << "\ntiles high:         " << m_numberOfTilesVertical
         << "\nsource rlevel:      " << m_sourceResLevel
         << "\ndecimation factor:  " << m_decimationFactor
         << "\nresamp type:        " << m_resampleType
         << "\nhisto mode:         " << m_histoMode << "\n";
      if (m_histoMode != OSSIM_HISTO_MODE_UNKNOWN)
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "Sampling every " << m_histoTileIndex << " tile(s) for histogram.\n";
      }
   }
}

void ossimOverviewSequencer::setToStartOfSequence()
{
   m_currentTileNumber = 0;
}

ossimRefPtr<ossimImageData> ossimOverviewSequencer::getNextTile()
{
   if ( m_dirtyFlag )
   {
      return ossimRefPtr<ossimImageData>();
   }

   // Get the rectangle to grab from the image.
   ossimIrect inputRect;
   getInputTileRectangle(inputRect);

   // Get the output rectangle.
   ossimIrect outputRect;
   getOutputTileRectangle(outputRect);

   // Capture the output rectangle.
   m_tile->setImageRectangle(outputRect);

   // Start with a blank tile.
   m_tile->makeBlank();

   // Grab the input tile.
   ossimRefPtr<ossimImageData> inputTile =
      m_imageHandler->getTile(inputRect, m_sourceResLevel);
   if (inputTile.valid() == false)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimOverviewSequencer::getNextTile DEBUG:"
         << "\nRequest failed for input rect: " << inputRect
         << "\nRes level:  " << m_sourceResLevel << std::endl;
   }

   if ( ( m_histoMode != OSSIM_HISTO_MODE_UNKNOWN ) &&
        ( (m_currentTileNumber % m_histoTileIndex) == 0 ) )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimOverviewSequencer::getNextTile DEBUG:"
            << "\npopulating histogram for tile: " << m_currentTileNumber
            << "\n";
      }
      inputTile->populateHistogram(m_histogram);
   }

   if ( (inputTile->getDataObjectStatus() == OSSIM_PARTIAL) ||
        (inputTile->getDataObjectStatus() == OSSIM_FULL ) )
   {
      // Resample the tile.
      resampleTile(inputTile.get());

      // Set the tile status.
      m_tile->validate();
   }

   // Increment the tile index.
   ++m_currentTileNumber;

   return m_tile;
}

void ossimOverviewSequencer::slaveProcessTiles()
{
}

bool ossimOverviewSequencer::isMaster() const
{
   return true;
}

ossimIpt ossimOverviewSequencer::getTileSize() const
{
   return m_tileSize;
}

void ossimOverviewSequencer::setTileSize(const ossimIpt& tileSize)
{
   m_tileSize = tileSize;
   updateTileDimensions();
   m_dirtyFlag = true;
}

void ossimOverviewSequencer::setResampleType(
   ossimFilterResampler::ossimFilterResamplerType resampleType)
{
   m_resampleType = resampleType;
}

void ossimOverviewSequencer::getInputTileRectangle(ossimIrect& inputRect) const
{
   if (!m_imageHandler) return;
   
   getOutputTileRectangle(inputRect);
   inputRect = inputRect * m_decimationFactor;
   inputRect = inputRect;

#if 0
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimOverviewSequencer::getInputTileRectangle DEBUG:"
         << "input rect: " << inputRect << std::endl;
   }
#endif
}

void ossimOverviewSequencer::getOutputTileRectangle(
   ossimIrect& outputRect) const
{
   // Get the row and column.
   ossim_int32 row = m_currentTileNumber / m_numberOfTilesHorizontal;
   ossim_int32 col = m_currentTileNumber % m_numberOfTilesHorizontal;

   ossimIpt pt;

   // Set the upper left.
   pt.y = row * m_tileSize.y;
   pt.x = col * m_tileSize.x;
   outputRect.set_ul(pt);

   // Set the lower right.
   pt.y = pt.y + m_tileSize.y - 1;
   pt.x = pt.x + m_tileSize.x - 1;   
   outputRect.set_lr(pt);

#if 0
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimOverviewSequencer::getOutputTileRectangle DEBUG:"
         << "output rect: " << outputRect << std::endl;
   }
#endif
}

void ossimOverviewSequencer::updateTileDimensions()
{
   if( m_areaOfInterest.hasNans() || m_tileSize.hasNans() )
   {
      m_numberOfTilesHorizontal = 0;
      m_numberOfTilesVertical   = 0;
      m_dirtyFlag = true;
   }
   else
   {
      // Get the output rectangle.
      ossimIrect rect;
      getOutputImageRectangle(rect);
      
      m_numberOfTilesHorizontal =
         static_cast<ossim_uint32>( rect.width()  / m_tileSize.x );
      m_numberOfTilesVertical =
         static_cast<ossim_uint32>( rect.height() / m_tileSize.y );

      if (rect.width()  % m_tileSize.x)
      {
         ++m_numberOfTilesHorizontal;
      }
      if (rect.height() % m_tileSize.y)
      {
         ++m_numberOfTilesVertical;
      }
   }
}

void ossimOverviewSequencer::resampleTile(const ossimImageData* inputTile)
{
   switch(m_imageHandler->getOutputScalarType())
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
         
      case OSSIM_SINT32:
      {
         resampleTile(inputTile, ossim_sint32(0));
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
            << "ossimOverviewSequencer::resampleTile Unkown pixel type!"
            << std::endl;
         return;
         
   } // End of "switch(m_imageHandler->getOutputScalarType())"
}

template <class T>
void  ossimOverviewSequencer::resampleTile(const ossimImageData* inputTile, T  /* dummy */ )
{
#if 0
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimOverviewSequencer::resampleTile DEBUG: "
         << "\ncurrent tile: " << m_currentTileNumber
         << "\ninput tile:\n" << *inputTile
         << "output tile:\n" << *(m_tile.get())
         << endl;
   }
#endif
   
   const ossim_uint32 BANDS = m_tile->getNumberOfBands();
   const ossim_uint32 LINES = m_tile->getHeight();
   const ossim_uint32 SAMPS = m_tile->getWidth();
   const ossim_uint32 INPUT_WIDTH = m_decimationFactor*m_tileSize.x;
   
   T nullPixel              = 0;
   ossim_float64 weight     = 0.0;
   ossim_float64 value      = 0.0;
   ossim_uint32 sampOffset  = 0;
   
   if (m_resampleType == ossimFilterResampler::ossimFilterResampler_NEAREST_NEIGHBOR)
   {
      for (ossim_uint32 band=0; band<BANDS; ++band)
      {
         const T* s = static_cast<const T*>(inputTile->getBuf(band)); // source
         T*       d = static_cast<T*>(m_tile->getBuf(band)); // destination
         
         nullPixel = static_cast<T>(inputTile->getNullPix(band));
         weight = 0.0;
         value  = 0.0;
         
         for (ossim_uint32 i=0; i<LINES; ++i)
         {
            for (ossim_uint32 j=0; j<SAMPS; ++j)
            {
               sampOffset = j*m_decimationFactor;
               
               weight = 1.0;
               value  = *(s + i*m_decimationFactor*INPUT_WIDTH + sampOffset);
               
               if(weight)
               {
                  d[j] = static_cast<T>( value/weight );
               }
               else
               {
                  d[j] = nullPixel;
               }
               
            } // End of sample loop.
            
            d += m_tileSize.x;
            
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
         T*       d = static_cast<T*>(m_tile->getBuf(band)); // destination

         nullPixel = static_cast<T>(inputTile->getNullPix(band));
         weight = 0.0;
         value  = 0.0;
         
         for (ossim_uint32 i=0; i<LINES; ++i)
         {
            lineOffset1 = i*m_decimationFactor*INPUT_WIDTH;
            lineOffset2 = (i*m_decimationFactor+1)*INPUT_WIDTH;
            
            for (ossim_uint32 j=0; j<SAMPS; ++j)
            {
               sampOffset = j*m_decimationFactor;
               
               weight = 0.0;
               value  = 0.0;
               
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
                  value += ul;
               }
               if(ur != nullPixel)
               {
                  ++weight;
                  value += ur;
               }
               if(ll != nullPixel)
               {
                  ++weight;
                  value += ll;
               }
               if(lr != nullPixel)
               {
                  ++weight;
                  value += lr;
               }

               if(weight)
               {
                  d[j] = static_cast<T>( value/weight );
               }
               else
               {
                  d[j] = nullPixel;
               }
            
            } // End of sample loop.
            
            d += m_tileSize.x;
            
         } // End of line loop.
         
      } // End of band loop.
   }
}
