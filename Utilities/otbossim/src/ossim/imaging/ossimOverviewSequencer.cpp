//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description: Sequencer for building overview files.
// 
//----------------------------------------------------------------------------
// $Id: ossimOverviewSequencer.cpp 16081 2009-12-10 20:56:36Z eshirschorn $

#include <ossim/imaging/ossimOverviewSequencer.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/parallel/ossimMpi.h>


#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimOverviewSequencer.cpp 16081 2009-12-10 20:56:36Z eshirschorn $";
#endif

static ossimTrace traceDebug("ossimOverviewSequencer:debug");

ossimOverviewSequencer::ossimOverviewSequencer()
   :
   ossimReferenced(),
   theImageHandler(0),
   theTile(),
   theAreaOfInterest(),
   theTileSize(OSSIM_DEFAULT_TILE_WIDTH, OSSIM_DEFAULT_TILE_HEIGHT),
   theNumberOfTilesHorizontal(0),
   theNumberOfTilesVertical(0),
   theCurrentTileNumber(0),
   theSourceResLevel(0),
   theDirtyFlag(true),
   theDecimationFactor(2),
   theResampleType(ossimFilterResampler::ossimFilterResampler_BOX)
{
   theAreaOfInterest.makeNan();

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
   theImageHandler = 0;

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
   return (theNumberOfTilesHorizontal*theNumberOfTilesVertical);
}

ossim_uint32 ossimOverviewSequencer::getNumberOfTilesHorizontal()const
{
   return theNumberOfTilesHorizontal;
}

ossim_uint32 ossimOverviewSequencer::getNumberOfTilesVertical()const
{
   return theNumberOfTilesVertical;
}

void ossimOverviewSequencer::getOutputImageRectangle(ossimIrect& rect) const
{
   if (theAreaOfInterest.hasNans())
   {
      rect.makeNan(); // not initialized...
   }
   else
   {
      ossim_int32 width  = theAreaOfInterest.width()  / theDecimationFactor;
      ossim_int32 height = theAreaOfInterest.height() / theDecimationFactor;

      //---
      // NOTE:
      // This will add a sample or line if there is a remainder like:
      // 1025 / 2 = 512 will then become 513.  It will be up to the resample
      // tile method to fill the extra sample correctly.
      //---
      if (theAreaOfInterest.width() % theDecimationFactor)
      {
         ++width;
      }
      if (theAreaOfInterest.height() % theDecimationFactor)
      {
         ++height;
      }
      
      rect = ossimIrect(0, 0, width-1, height-1);
   }
}

void ossimOverviewSequencer::setImageHandler(ossimImageHandler* input)
{
   theImageHandler = input;
   theAreaOfInterest.makeNan();
   theDirtyFlag = true;
}

void ossimOverviewSequencer::setSourceLevel(ossim_uint32 level)
{
   theSourceResLevel = level;
   theAreaOfInterest.makeNan();
   theDirtyFlag = true;
}

void ossimOverviewSequencer::initialize()
{
   if ( theDirtyFlag == false )
   {
      return; // Already initialized.
   }
   
   if ( !theImageHandler )
   {
      return;
   }

   // Check the area of interest and set from image if needed.
   if ( theAreaOfInterest.hasNans() )
   {
      theAreaOfInterest =
         theImageHandler->getImageRectangle(theSourceResLevel);
   }

   // Check the tile size and set from image if needed.
   if ( theTileSize.hasNans() )
   {
      theTileSize.x = theImageHandler->getTileWidth();
      theTileSize.y = theImageHandler->getTileHeight();
   }

   // Update theNumberOfTilesHorizontal and theNumberOfTilesVertical.
   updateTileDimensions();

   // Start on first tile.
   theCurrentTileNumber = 0;
      
   theTile  = ossimImageDataFactory::instance()->
      create( theImageHandler,
              theImageHandler->getOutputScalarType(),
              theImageHandler->getNumberOfOutputBands(),
              static_cast<ossim_uint32>(theTileSize.x),
              static_cast<ossim_uint32>(theTileSize.y) );
   if(theTile.valid())
   {
      theTile->initialize();
   }

   theDirtyFlag = false;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "aoi:                  " << theAreaOfInterest
         << "\ntile size:          " << theTileSize
         << "\ntiles wide:         " << theNumberOfTilesHorizontal
         << "\ntiles high:         " << theNumberOfTilesVertical
         << "\nsource rlevel:      " << theSourceResLevel
         << "\ndecimation factor:  " << theDecimationFactor
         << "\nresamp type:          " << theResampleType
         << std::endl;
   }
}

void ossimOverviewSequencer::setToStartOfSequence()
{
   theCurrentTileNumber = 0;
}

ossimRefPtr<ossimImageData> ossimOverviewSequencer::getNextTile()
{
   if ( theDirtyFlag )
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
   theTile->setImageRectangle(outputRect);

   // Start with a blank tile.
   theTile->makeBlank();

   // Grab the input tile.
   ossimRefPtr<ossimImageData> inputTile =
      theImageHandler->getTile(inputRect, theSourceResLevel);
   if (inputTile.valid() == false)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimOverviewSequencer::getNextTile DEBUG:"
         << "\nRequest failed for input rect: " << inputRect
         << "\nRes level:  " << theSourceResLevel << std::endl;
   }

   if ( (inputTile->getDataObjectStatus() == OSSIM_PARTIAL) ||
        (inputTile->getDataObjectStatus() == OSSIM_FULL ) )
   {
      // Resample the tile.
      resampleTile(inputTile.get());

      // Set the tile status.
      theTile->validate();
   }

   // Increment the tile index.
   ++theCurrentTileNumber;

   return theTile;
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
   return theTileSize;
}

void ossimOverviewSequencer::setTileSize(const ossimIpt& tileSize)
{
   theTileSize = tileSize;
   updateTileDimensions();
   theDirtyFlag = true;
}

void ossimOverviewSequencer::setResampleType(
   ossimFilterResampler::ossimFilterResamplerType resampleType)
{
   theResampleType = resampleType;
}

void ossimOverviewSequencer::getInputTileRectangle(ossimIrect& inputRect) const
{
   if (!theImageHandler) return;
   
   getOutputTileRectangle(inputRect);
   inputRect = inputRect * theDecimationFactor;
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
   ossim_int32 row = theCurrentTileNumber / theNumberOfTilesHorizontal;
   ossim_int32 col = theCurrentTileNumber % theNumberOfTilesHorizontal;

   ossimIpt pt;

   // Set the upper left.
   pt.y = row * theTileSize.y;
   pt.x = col * theTileSize.x;
   outputRect.set_ul(pt);

   // Set the lower right.
   pt.y = pt.y + theTileSize.y - 1;
   pt.x = pt.x + theTileSize.x - 1;   
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
   if( theAreaOfInterest.hasNans() || theTileSize.hasNans() )
   {
      theNumberOfTilesHorizontal = 0;
      theNumberOfTilesVertical   = 0;
      theDirtyFlag = true;
   }
   else
   {
      // Get the output rectangle.
      ossimIrect rect;
      getOutputImageRectangle(rect);
      
      theNumberOfTilesHorizontal =
         static_cast<ossim_uint32>( rect.width()  / theTileSize.x );
      theNumberOfTilesVertical =
         static_cast<ossim_uint32>( rect.height() / theTileSize.y );

      if (rect.width()  % theTileSize.x)
      {
         ++theNumberOfTilesHorizontal;
      }
      if (rect.height() % theTileSize.y)
      {
         ++theNumberOfTilesVertical;
      }
   }
}

void ossimOverviewSequencer::resampleTile(const ossimImageData* inputTile)
{
   switch(theImageHandler->getOutputScalarType())
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
            << "ossimOverviewSequencer::resampleTile Unkown pixel type!"
            << std::endl;
         return;
         
   } // End of "switch(theImageHandler->getOutputScalarType())"
}

template <class T>
void  ossimOverviewSequencer::resampleTile(const ossimImageData* inputTile, T  /* dummy */ )
{
#if 0
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimOverviewSequencer::resampleTile DEBUG: "
         << "\ncurrent tile: " << theCurrentTileNumber
         << "\ninput tile:\n" << *inputTile
         << "output tile:\n" << *(theTile.get())
         << endl;
   }
#endif
   
   const ossim_uint32 BANDS = theTile->getNumberOfBands();
   const ossim_uint32 LINES = theTile->getHeight();
   const ossim_uint32 SAMPS = theTile->getWidth();
   const ossim_uint32 INPUT_WIDTH = theDecimationFactor*theTileSize.x;
   
   T nullPixel              = 0;
   ossim_float64 weight     = 0.0;
   ossim_float64 value      = 0.0;
   ossim_uint32 sampOffset  = 0;
   
   if (theResampleType ==
       ossimFilterResampler::ossimFilterResampler_NEAREST_NEIGHBOR)
   {
      for (ossim_uint32 band=0; band<BANDS; ++band)
      {
         const T* s = static_cast<const T*>(inputTile->getBuf(band)); // source
         T*       d = static_cast<T*>(theTile->getBuf(band)); // destination
         
         nullPixel = static_cast<T>(inputTile->getNullPix(band));
         weight = 0.0;
         value  = 0.0;
         
         for (ossim_uint32 i=0; i<LINES; ++i)
         {
            for (ossim_uint32 j=0; j<SAMPS; ++j)
            {
               sampOffset = j*theDecimationFactor;
               
               weight = 1.0;
               value  = *(s + i*theDecimationFactor*INPUT_WIDTH + sampOffset);
               
               if(weight)
               {
                  d[j] = static_cast<T>( value/weight );
               }
               else
               {
                  d[j] = nullPixel;
               }
               
            } // End of sample loop.
            
            d += theTileSize.x;
            
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
         T*       d = static_cast<T*>(theTile->getBuf(band)); // destination

         nullPixel = static_cast<T>(inputTile->getNullPix(band));
         weight = 0.0;
         value  = 0.0;
         
         for (ossim_uint32 i=0; i<LINES; ++i)
         {
            lineOffset1 = i*theDecimationFactor*INPUT_WIDTH;
            lineOffset2 = (i*theDecimationFactor+1)*INPUT_WIDTH;
            
            for (ossim_uint32 j=0; j<SAMPS; ++j)
            {
               sampOffset = j*theDecimationFactor;
               
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
            
            d += theTileSize.x;
            
         } // End of line loop.
         
      } // End of band loop.
   }
}
