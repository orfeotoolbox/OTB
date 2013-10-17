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
// Contains class definition for TiffOverviewBuilder
// 
//*******************************************************************
//  $Id: ossimTiffOverviewBuilder.cpp 22362 2013-08-07 20:23:22Z dburken $

#include <ossim/imaging/ossimTiffOverviewBuilder.h>
#include <ossim/parallel/ossimMpi.h>
#include <ossim/parallel/ossimMpiMasterOverviewSequencer.h>
#include <ossim/parallel/ossimMpiSlaveOverviewSequencer.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimStdOutProgress.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimBitMaskTileSource.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimMapProjectionInfo.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/support_data/ossimGeoTiff.h>

#include <xtiffio.h>
#include <algorithm> /* for std::fill */
#include <sstream>
using namespace std;

RTTI_DEF1(ossimTiffOverviewBuilder,
          "ossimTiffOverviewBuilder",
          ossimOverviewBuilderBase)

static ossimTrace traceDebug("ossimTiffOverviewBuilder:degug");

// Property keywords.
static const char COPY_ALL_KW[]           = "copy_all_flag";
static const char INTERNAL_OVERVIEWS_KW[] = "internal_overviews_flag";

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimTiffOverviewBuilder.cpp 22362 2013-08-07 20:23:22Z dburken $";
#endif


//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimTiffOverviewBuilder::ossimTiffOverviewBuilder()
   :
      ossimOverviewBuilderBase(),
      m_nullDataBuffer(0),
      m_bytesPerPixel(1),
      m_bitsPerSample(8),
      m_tileWidth(0),
      m_tileHeight(0),
      m_tileSizeInBytes(0),
      m_sampleFormat(0),
      m_currentTiffDir(0),
      m_tiffCompressType(COMPRESSION_NONE),
      m_jpegCompressQuality(DEFAULT_COMPRESS_QUALITY),
      m_resampleType(ossimFilterResampler::ossimFilterResampler_BOX),
      m_nullPixelValues(),
      m_copyAllFlag(false),
      m_outputTileSizeSetFlag(false),
      m_internalOverviewsFlag(false)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimTiffOverviewBuilder::ossimTiffOverviewBuilder DEBUG:\n";
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  "
         << OSSIM_ID
         << "\n";
#endif
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "overview stop dimension: " << m_overviewStopDimension
         << std::endl;
   }
}

ossimTiffOverviewBuilder::~ossimTiffOverviewBuilder()
{
}

void ossimTiffOverviewBuilder::setResampleType(
   ossimFilterResampler::ossimFilterResamplerType resampleType)
{
   m_resampleType = resampleType;
}

bool ossimTiffOverviewBuilder::buildOverview(const ossimFilename& overview_file, bool copy_all)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimTiffOverviewBuilder::buildOverview DEBUG:"
         << "\noverview file: " << overview_file.c_str()
         << "\ncopy_all flag: " << (copy_all?"true":"false")
         << std::endl;
   }


   m_outputFile  = overview_file;
   m_copyAllFlag = copy_all;

   return execute();
}


bool ossimTiffOverviewBuilder::execute()
{
   static const char MODULE[] = "ossimTiffOverviewBuilder::execute";
   
   if (theErrorStatus == ossimErrorCodes::OSSIM_ERROR)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nError status has been previously set!  Returning..."
         << std::endl;
      return false;
   }

   if ( !m_imageHandler || m_imageHandler->hasError() )
   {
      setErrorStatus();
      return false;
   }

   m_outputFile = getOutputFile();
   if (m_outputFile == ossimFilename::NIL)
   {
      return false;
   }

   // Check the file.  Disallow same file overview building.
   if ( !buildInternalOverviews() && (m_imageHandler->getFilename() == m_outputFile) )
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "Source image file and overview file cannot be the same!"
         << std::endl;
      return false;
   }

   ossimFilename outputFileTemp = m_outputFile;
   
   if ( !buildInternalOverviews() )
   {
      // Add .tmp in case process gets aborted to avoid leaving bad .ovr file.
      outputFileTemp += ".tmp";
   }

   // Required number of levels needed including r0.
   ossim_uint32 requiedResLevels = getRequiredResLevels(m_imageHandler.get());

   // Zero based starting resLevel.
   ossim_uint32 startingResLevel  = 0;
   if ( !copyR0() )
   {
      startingResLevel = m_imageHandler->getNumberOfDecimationLevels();
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE
         << "\nCurrent number of reduced res sets: "
         << m_imageHandler->getNumberOfDecimationLevels()
         << "\nNumber of required reduced res sets:  " << requiedResLevels
         << "\nStarting reduced res set:    " << startingResLevel
         << "\nResampling type:  " << getOverviewType().c_str()
         << std::endl;
   }

   if ( startingResLevel >= requiedResLevels )
   {
      ossimNotify(ossimNotifyLevel_INFO)
         << MODULE << " NOTICE:"
         << "\nImage has required reduced resolution data sets.\nReturning..."
         << std::endl;
      return true;
   }

   //---
   // If image handler is band selector, start with all bands.
   // Some sources, e.g. ossimEnviTileSource can pick up default
   // bands and filter out all other bands.
   //---
   m_imageHandler->setOutputToInputBandList();  
   
   //---
   // If alpha bit mask generation was requested, then need to instantiate the mask writer object.
   // This is simply a "transparent" tile source placed after to the right of the image handler
   // that scans the pixels being pulled and accumulates alpha bit mask for writing at the end.
   //---
   if (m_bitMaskSpec.getSize() > 0)
   {
      m_maskWriter = new ossimBitMaskWriter();
      m_maskWriter->loadState(m_bitMaskSpec);
      m_maskWriter->setStartingResLevel(1);
      ossimRefPtr<ossimBitMaskTileSource> bmts = new ossimBitMaskTileSource;
      bmts->setAssociatedMaskWriter(m_maskWriter.get());
      m_maskFilter = new ossimMaskFilter();
      m_maskFilter->connectMyInputTo(m_imageHandler.get());
      m_maskFilter->setMaskSource((ossimImageSource*)bmts.get());
   }

   ossimStdOutProgress* progressListener = 0; // Only used on master.
   TIFF* tif = 0;                             // Only used on master.

   setProcessStatus(ossimProcessInterface::PROCESS_STATUS_EXECUTING);
   setPercentComplete(0.0);

   if (ossimMpi::instance()->getRank() == 0 )
   {
      //---
      // See if the file can be opened for writing.
      // Note:  If this file existed previously it will be overwritten.
      //---
      tif = openTiff(outputFileTemp);
      if (!tif)
      {
         // Set the error...
         setErrorStatus();
         ossimNotify(ossimNotifyLevel_WARN)
            << __FILE__ << " " << __LINE__ << " " << MODULE
            << "\nCannot open file: " << outputFileTemp << std::endl;
         return false;
      } 

      //---
      // Check for a listeners.  If the list is empty, add a standard out
      // listener so that command line apps like img2rr will get some progress.
      //---
      ossimStdOutProgress* progressListener = 0;
      if (theListenerList.empty())
      {
         progressListener = new ossimStdOutProgress(0, true);
         addListener(progressListener);
      }

      if (startingResLevel == 0)
      {       
         if (!writeR0(tif))
         {
            // Set the error...
            setErrorStatus();
            ossimNotify(ossimNotifyLevel_WARN)
               << __FILE__ << " " << __LINE__
               << "\nError copying image!" << std::endl;

            closeTiff(tif);
            if (progressListener)
            {
               removeListener(progressListener);
               delete progressListener;
               progressListener = 0;
            }
            
            if ( outputFileTemp.exists() && !buildInternalOverviews() )
            {
               ossimFilename::remove( outputFileTemp );
            }
            return false;
         }

         ++startingResLevel; // Go to r1.
      }

      if (needsAborting())
      {
         closeTiff(tif);
         if (progressListener)
         {
            removeListener(progressListener);
            delete progressListener;
            progressListener = 0;
         }
         return false;
      }
      
      TIFFFlush(tif);
      
   } // End of master only write of r0.
        
   for (ossim_uint32 i = startingResLevel; i < requiedResLevels; ++i)
   {
      if (ossimMpi::instance()->getRank() == 0 )
      {
         TIFFFlush(tif);
      }

      // Sync all processes...
      ossimMpi::instance()->barrier();
      
      ossimRefPtr<ossimImageHandler> ih = 0;

      //---
      // If we copied r0 to the overview file use it instead of the
      // original image handler as it is probably faster.
      //---
      if ( !copyR0() && (i <= m_imageHandler->getNumberOfDecimationLevels()) ) 
      {
         ih = m_imageHandler;
      }
      else
      {
         // We know we're a tiff so don't use the factory.
         ih = new ossimTiffTileSource;
         if ( ih->open(outputFileTemp) == false )
         {
            ih = 0;
            
            // Set the error...
            setErrorStatus();
            ossimNotify(ossimNotifyLevel_WARN)
               << __FILE__ << " " << __LINE__ << " " << MODULE
               << "\nCannot open file: " << outputFileTemp << std::endl;
            
            return false;
         }

         //---
         // Since the overview file is being opened here, need to set its handler's starting res
         // level where the original image file left off. This is usually R1 since the original
         // file only has R0, but the original file may have more than R0:
         //---
         if ( !copyR0() &&  !buildInternalOverviews() )
         {
            ih->setStartingResLevel( m_imageHandler->getNumberOfDecimationLevels());
         }
      }
      
      // If mask is to be generated, need to notify both the writer and the reader of new 
      // input source:
      if (m_bitMaskSpec.getSize() > 0)
      {
         m_maskFilter->connectMyInputTo(0, ih.get());
         m_maskWriter->connectMyInputTo(ih.get());
      }

      if ( !writeRn( ih.get(), tif, i, (i==startingResLevel) && !copyR0() ) )
      {
         // Set the error...
         setErrorStatus();
         ossimNotify(ossimNotifyLevel_WARN)
            << __FILE__ << " " << __LINE__ << " " << MODULE
            << "\nError creating reduced res set: " << i << std::endl;

         ih->disconnect();
         ih = 0;
         if (tif)
         {
            closeTiff(tif);
            tif = 0;
         }

         if (progressListener)
         {
            removeListener(progressListener);
            delete progressListener;
            progressListener = 0;
         }

         if ( outputFileTemp.exists() && !buildInternalOverviews() )
         {
            ossimFilename::remove( outputFileTemp );
         }

         return false;
      }
      
      if (needsAborting())
      {
         ih->disconnect();
         ih = 0;
         if (tif)
         {
            closeTiff(tif);
            tif = 0;
         }
         if (progressListener)
         {
            removeListener(progressListener);
            delete progressListener;
            progressListener = 0;
         }
         return false;
      }
      
      if (m_bitMaskSpec.getSize() > 0)
      {
         m_maskFilter->disconnectMyInput(0);
         m_maskWriter->disconnectAllInputs();
      }
      ih = 0;
   }

   if (ossimMpi::instance()->getRank() == 0 )
   {
      if (tif)
      {
         closeTiff(tif);
         tif = 0;
      }

      // Write out the alpha bit mask if one was enabled:
      if (m_maskWriter.valid())
      {
         ossimNotify(ossimNotifyLevel_INFO) << "Writing alpha bit mask file..." << std::endl;
         m_maskWriter->close();
      }

      // Remove the listener if we had one.
      if (progressListener)
      {
         removeListener(progressListener);
         delete progressListener;
         progressListener = 0;
      }

      if ( !buildInternalOverviews() )
      {
         outputFileTemp.rename(m_outputFile);
      }
      
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_INFO)
            << "Wrote file:  " << m_outputFile.c_str() << std::endl;
      }
     
      setCurrentMessage(ossimString("Finished..."));
   }
   
   finalize();  // Reset band list if a band selector.

   return true;
}

bool ossimTiffOverviewBuilder::writeR0(TIFF* tif)
{
   static const char MODULE[] = "ossimTiffOverviewBuilder::writeR0";

   ossimIrect rect = m_imageHandler->getImageRectangle();

   if (!setTags(tif, rect, 0))
   {
      closeTiff(tif);
      ossimNotify(ossimNotifyLevel_WARN) << MODULE << " Error writing tags!" << std::endl;
      return false;
   }
   
   // Set the geotiff tags.
   if ( setGeotiffTags(m_imageHandler->getImageGeometry().get(),
                       m_imageHandler->getBoundingRect(),
                       0,
                       tif) == false )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_NOTICE)
            << MODULE << " NOTICE: geotiff tags not set." << std::endl;
      } 
   }
   
   ossim_int32 samples         = m_imageHandler->getNumberOfSamples();
   ossim_int32 lines           = m_imageHandler->getNumberOfLines();
   ossim_int32 tilesWide       = samples % m_tileWidth ?
                           samples / m_tileWidth + 1 : samples / m_tileWidth;
   ossim_int32 tilesHigh       = lines % m_tileHeight ?
                           lines / m_tileHeight + 1 : lines / m_tileHeight;
   ossim_int32 numberOfTiles   = tilesWide * tilesHigh;

   int tileNumber = 0;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimTiffOverviewBuilder::writeR0 DEBUG:"
         << "\nsamples:        " << samples
         << "\nlines:          " << lines
         << "\ntilesWide:      " << tilesWide
         << "\ntilesHigh:      " << tilesHigh
         << "\nnumberOfTiles:  " << numberOfTiles
         << std::endl;
   }

   setCurrentMessage(ossimString("Copying r0..."));
   
   //***
   // Tile loop in the line direction.
   //***
   for(int i = 0; i < tilesHigh; ++i)
   {
      ossimIpt origin(0, 0);
      origin.y = i * m_tileHeight;
      
      //***
      // Tile loop in the sample (width) direction.
      //***
      for(int j = 0; j < tilesWide; ++j)
      {
         origin.x = j * m_tileWidth;

         ossimRefPtr<ossimImageData> t =
            m_imageHandler->getTile(ossimIrect(origin.x,
                                                origin.y,
                                                origin.x +(m_tileWidth-1),
                                                origin.y +(m_tileHeight-1)));

         // Check for errors reading tile:
         if ( m_imageHandler->hasError() )
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " ERROR: reading tile:  " << i << std::endl;
            return false;
         }

         // If masking was enabled, pass the tile onto that object for processing:
         if (m_maskWriter.valid())
            m_maskWriter->generateMask(t, 0);

         //***
         // Band loop.
         //***
         for (uint32 band=0;
              band < m_imageHandler->getNumberOfOutputBands();
              ++band)
         {
            tdata_t data;
            
            if ( t.valid() && (t->getDataObjectStatus() != OSSIM_NULL) )
            {
               // Grab a pointer to the tile for the band.
               data = static_cast<tdata_t>(t->getBuf(band));
            }
            else
            {
               data = static_cast<tdata_t>(&(m_nullDataBuffer.front()));
            }

            // Write the tile.
            int bytesWritten = 0;
            bytesWritten = TIFFWriteTile(tif,
                                         data,
                                         origin.x,
                                         origin.y,
                                         0,        // z
                                         band);    // sample

            if (bytesWritten != m_tileSizeInBytes)
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << MODULE << " ERROR:"
                  << "Error returned writing tiff tile:  " << i
                  << "\nExpected bytes written:  " << m_tileSizeInBytes
                  << "\nBytes written:  " << bytesWritten
                  << std::endl;
               theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
               return false;
            }

         } // End of band loop.  
        
         ++tileNumber;

      } // End of tile loop in the sample (width) direction.

      if (needsAborting())
      {
         setPercentComplete(100.0);
         break;
      }
      else
      {
         double tile = tileNumber;
         double numTiles = numberOfTiles;
         setPercentComplete(tile / numTiles * 100.0);
      }

   } // End of tile loop in the line (height) direction.

   //***
   // Write the current dirctory.
   //***
   if (!TIFFWriteDirectory(tif))
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Error writing directory!" << std::endl;
      return false;
   }

   ++m_currentTiffDir;

   return true;
}

bool ossimTiffOverviewBuilder::writeRn( ossimImageHandler* imageHandler,
                                        TIFF* tif,
                                        ossim_uint32 resLevel,
                                        bool firstResLevel )
{
   if ( tif ) //  && buildInternalOverviews() )
   {
      // Create an empty directory to start with.
      TIFFCreateDirectory( tif );
   }
   else
   {
      return false;
   }
   
   //---
   // Set up the sequencer.  This will be one of three depending on if we're
   // running mpi and if we are a master process or a slave process.
   //---
   ossimRefPtr<ossimOverviewSequencer> sequencer;
   
   if(ossimMpi::instance()->getNumberOfProcessors() > 1)
   {
      if ( ossimMpi::instance()->getRank() == 0 )
      {
         sequencer = new ossimMpiMasterOverviewSequencer();
      }
      else
      {
         sequencer = new ossimMpiSlaveOverviewSequencer();
      }
   }
   else
   {
      sequencer = new ossimOverviewSequencer();
   }
   
   sequencer->setImageHandler(imageHandler);

   if (m_maskWriter.valid() && m_maskFilter.valid())
   {
      sequencer->setBitMaskObjects(m_maskWriter.get(), m_maskFilter.get());
   }

   // sourceResLevel: This is the res level to pull data from:
   ossim_uint32 sourceResLevel = imageHandler->getNumberOfDecimationLevels() +
      imageHandler->getStartingResLevel() - 1;

   sequencer->setSourceLevel(sourceResLevel);
   sequencer->setResampleType(m_resampleType);
   sequencer->setTileSize( ossimIpt(m_tileWidth, m_tileHeight) );
   
   if ( firstResLevel )
   {
      // Set up things that are only performed on first scan through tiles.
      
      if ( getHistogramMode() != OSSIM_HISTO_MODE_UNKNOWN )
      {
         // Accumulate a histogram.  Can't do with mpi/multi-process.
         if(ossimMpi::instance()->getNumberOfProcessors() == 1)
         {
            sequencer->setHistogramMode(getHistogramMode());
         }
         //---
         // else{} Not sure if we want an error thrown here.  For now will handle at the
         // application level.
         //---
      }
      if ( getScanForMinMaxNull() == true )
      {
         sequencer->setScanForMinMaxNull(true);
      }
      else if ( getScanForMinMax() == true )
      {
         sequencer->setScanForMinMax(true);
      }
   }

   // Note sequence setup must be performed before intialize. 
   sequencer->initialize();

   // If we are a slave process start the resampling of tiles.
   if (ossimMpi::instance()->getRank() != 0 )
   {
      sequencer->slaveProcessTiles();
      return true;
   }

   //---
   // The rest of the method on master node only.
   //---
   static const char MODULE[] = "ossimTiffOverviewBuilder::writeRn";

   ostringstream os;
   os << "creating r" << resLevel << "...";
   setCurrentMessage(os.str());

   if (resLevel == 0)
   {
      return false;
   }

   ossimIrect rect;
   sequencer->getOutputImageRectangle(rect);

   if (!setTags(tif, rect, resLevel))
   {
      setErrorStatus();
      closeTiff(tif);
      tif = 0;
      ossimNotify(ossimNotifyLevel_WARN) << MODULE << " Error writing tags!" << std::endl;
      return false;
   }

   if ( !buildInternalOverviews() && !copyR0() && (resLevel == 1) )
   {
      //---
      // Set the geotif tags for the first layer.
      // Note this is done in writeR0 method if m_copyAllFlag is set.
      //---
      if ( setGeotiffTags(m_imageHandler->getImageGeometry().get(),
                          ossimDrect(rect),
                          resLevel,
                          tif) == false )
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_NOTICE)
               << MODULE << " NOTICE: geotiff tags not set." << std::endl;
         } 
      }
   }

   ossim_uint32 outputTilesWide = sequencer->getNumberOfTilesHorizontal();
   ossim_uint32 outputTilesHigh = sequencer->getNumberOfTilesVertical();
   ossim_uint32 numberOfTiles   = sequencer->getNumberOfTiles();
   ossim_uint32 tileNumber      = 0;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimTiffOverviewBuilder::writeRn DEBUG:"
         << "\noutputTilesWide:  " << outputTilesWide
         << "\noutputTilesHigh:  " << outputTilesHigh
         << "\nnumberOfTiles:    " << numberOfTiles
         << std::endl;
   }
 
   // Tile loop in the line direction.
   ossim_uint32 y = 0;

   for(ossim_uint32 i = 0; i < outputTilesHigh; ++i)
   {
      // Tile loop in the sample (width) direction.
      ossim_uint32 x = 0;
      for(ossim_uint32 j = 0; j < outputTilesWide; ++j)
      {
         // Grab the resampled tile.
         ossimRefPtr<ossimImageData> t = sequencer->getNextTile();

         // Check for errors reading tile:
         if ( sequencer->hasError() )
         {
            setErrorStatus();
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " ERROR: reading tile:  " << i << std::endl;
            return false;
         }
         
         if ( t.valid() && ( t->getDataObjectStatus() != OSSIM_NULL ) )
         {
            // Write it to the tiff.
            for (ossim_uint32 band = 0; band < t->getNumberOfBands(); ++band)
            {
               // Write the tile.
               int bytesWritten = 0;
               bytesWritten = TIFFWriteTile(tif,
                                            t->getBuf(band),
                                            x,
                                            y,
                                            0,        // z
                                            band);    // sample
               
               if (bytesWritten != m_tileSizeInBytes)
               {
                  ossimNotify(ossimNotifyLevel_WARN)
                     << MODULE << " ERROR:"
                     << "Error returned writing tiff tile:  " << i
                     << "\nExpected bytes written:  " << m_tileSizeInBytes
                     << "\nBytes written:  " << bytesWritten
                     << std::endl;
                  theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
                  
                  return false;
               }
            }
         }
         x += m_tileWidth; // Increment x for next TIFFWriteTile.
         ++tileNumber;      // Increment tile number for percent complete.

      } // End of tile loop in the sample (width) direction.

      if (needsAborting())
      {
         setPercentComplete(100.0);
         break;
      }
      else
      {
         double tile = tileNumber;
         double numTiles = numberOfTiles;
         setPercentComplete(tile / numTiles * 100.0);
      }

      y += m_tileHeight; // Increment y for next TIFFWriteTile.

   } // End of tile loop in the line (height) direction.

   //---
   // Write the current dirctory.
   //---
   if (!TIFFFlush(tif))
   {
      setErrorStatus();
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Error writing to TIF file!" << std::endl;
      return false;
   }

   if ( firstResLevel )
   {
      if ( ossimMpi::instance()->getNumberOfProcessors() == 1 )
      {
         if ( getHistogramMode() != OSSIM_HISTO_MODE_UNKNOWN )
         {
            // Write the histogram.
            ossimFilename histoFilename = getOutputFile();
            histoFilename.setExtension("his");
            sequencer->writeHistogram(histoFilename);
         }

         if ( ( getScanForMinMaxNull() == true ) || ( getScanForMinMax() == true ) )
         {
            // Write the omd file:
            ossimFilename file = getOutputFile();
            file = file.setExtension("omd");
            sequencer->writeOmdFile(file);
         }
      }
   }

   ++m_currentTiffDir;

   return true;
}

//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimTiffOverviewBuilder::setTags(TIFF* tif,
                                       const ossimIrect& outputRect,
                                       ossim_int32 resLevel) const
{
   if (outputRect.hasNans())
   {
      return false;
   }
   
   ossim_int32   imageWidth      = outputRect.width();
   ossim_int32   imageHeight     = outputRect.height();
   int16         samplesPerPixel = m_imageHandler->getNumberOfOutputBands();
   ossim_float64 minSampleValue  = m_imageHandler->getMinPixelValue();
   ossim_float64 maxSampleValue  = m_imageHandler->getMaxPixelValue();

   if (resLevel)
   {
      TIFFSetField( tif, TIFFTAG_SUBFILETYPE, FILETYPE_REDUCEDIMAGE );
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimTiffOverviewBuilder::setTags DEBUG:"
         << "\nrrds_level:      " << resLevel
         << "\nimageWidth:      " << imageWidth
         << "\nimageHeight:     " << imageHeight
         << "\nminSampleValue:  " << minSampleValue
         << "\nmaxSampleValue:  " << maxSampleValue
         << std::endl;
   }
   TIFFSetField( tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_SEPARATE );
   TIFFSetField( tif, TIFFTAG_IMAGEWIDTH, imageWidth);
   TIFFSetField( tif, TIFFTAG_IMAGELENGTH, imageHeight);
   TIFFSetField( tif, TIFFTAG_BITSPERSAMPLE, m_bitsPerSample );
   TIFFSetField( tif, TIFFTAG_SAMPLEFORMAT, m_sampleFormat );
   TIFFSetField( tif, TIFFTAG_SAMPLESPERPIXEL, samplesPerPixel );

   if( m_imageHandler->getNumberOfInputBands() == 3 )
      TIFFSetField( tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB );
   else
      TIFFSetField( tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK );
   
   TIFFSetField( tif, TIFFTAG_TILEWIDTH,  m_tileWidth  );
   TIFFSetField( tif, TIFFTAG_TILELENGTH, m_tileHeight );

   //---
   // Only turn on compression for 8 bit, one or three band data.  Not sure what compression
   // types can handle what but this was crashing ossim-prepoc on a directory walk with jpeg
   // compression.
   //---
   if ( ( m_imageHandler->getOutputScalarType() == OSSIM_UINT8 ) &&
        ( ( m_imageHandler->getNumberOfInputBands() == 3 ) ||
          ( m_imageHandler->getNumberOfInputBands() == 1 ) ) )
   {
      // Set the compression related tags...
      TIFFSetField( tif, TIFFTAG_COMPRESSION, m_tiffCompressType );
      if (m_tiffCompressType == COMPRESSION_JPEG)
      {
         TIFFSetField( tif, TIFFTAG_JPEGQUALITY,  m_jpegCompressQuality);
      }
   }
   else
   {
      if ( traceDebug() && (m_tiffCompressType != COMPRESSION_NONE ) )
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimTiffOverviewBuilder::setTags WARNING:\n"
            << "Compression not set for this data type:\n"
            << "scalar type: "
            << ossimScalarTypeLut::instance()->getEntryString(
               m_imageHandler->getOutputScalarType())
            << "\nband count: " << m_imageHandler->getNumberOfInputBands()
            << std::endl;
      }
      TIFFSetField( tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE );
   }
   
   // Set the min/max values.
   switch( m_imageHandler->getOutputScalarType() )
   {
      case OSSIM_SINT16:
      case OSSIM_FLOAT32:
      case OSSIM_FLOAT64:
      case OSSIM_NORMALIZED_DOUBLE:
         TIFFSetField( tif, TIFFTAG_SMINSAMPLEVALUE, minSampleValue );
         TIFFSetField( tif, TIFFTAG_SMAXSAMPLEVALUE, maxSampleValue );
         break;

      case OSSIM_UINT8:
      case OSSIM_USHORT11:
      case OSSIM_UINT16:
      case OSSIM_UINT32:
      default:
         TIFFSetField( tif, TIFFTAG_MINSAMPLEVALUE,
                       static_cast<int>(minSampleValue) );
         TIFFSetField( tif, TIFFTAG_MAXSAMPLEVALUE,
                       static_cast<int>(maxSampleValue) );
         break;
   }
   
    return true;
}

bool ossimTiffOverviewBuilder::setGeotiffTags(const ossimImageGeometry* geom,
                                              const ossimDrect& boundingRect,
                                              ossim_uint32 resLevel,
                                              TIFF* tif)
{
   bool result = false;

   if ( geom && tif )
   {
      const ossimProjection* proj = geom->getProjection();
      if (proj)
      {
         // Must duplicate if changing scale.
         ossimObject* obj = proj->dup();
         ossimMapProjection* mapProj = PTR_CAST(ossimMapProjection, obj);
         if ( mapProj )
         {
            if ( mapProj->hasModelTransform() == false )
            {
               if (resLevel)
               {
                  ossim_float64 factor = (ossim_float64)(1 << resLevel);
                  mapProj->applyScale(ossimDpt(factor, factor), true);
               }
               ossimRefPtr<ossimMapProjectionInfo> projInfo =
                  new ossimMapProjectionInfo(mapProj, boundingRect);
               result = ossimGeoTiff::writeTags(tif, projInfo);
            }
         }
         delete obj; // Cleanup from dup.
         obj = 0;
      }
   }
   
   return result;
}

TIFF* ossimTiffOverviewBuilder::openTiff(const ossimString& filename) const
{
   ossimString openMode;
   if ( !buildInternalOverviews() )
   {
      openMode = "w";

      ossim_uint64 fourGigs = (static_cast<ossim_uint64>(1024)*
                               static_cast<ossim_uint64>(1024)*
                               static_cast<ossim_uint64>(1024)*
                               static_cast<ossim_uint64>(4));
      ossimIrect bounds = m_imageHandler->getBoundingRect();
      ossim_uint64 byteCheck =
         (static_cast<ossim_uint64>(bounds.width())*
          static_cast<ossim_uint64>(bounds.height())*
          static_cast<ossim_uint64>(m_imageHandler->getNumberOfOutputBands())*
          static_cast<ossim_uint64>(ossim::scalarSizeInBytes(m_imageHandler->
                                                             getOutputScalarType())));
         
      if((byteCheck*static_cast<ossim_uint64>(2))>fourGigs)
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << " Big tiff activated\n";
         }
         openMode += "8";
      }
      else
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << " No big tiff activated\n";
         }
      }
   }
   else
   {
      openMode = "r+"; // Append to existing file...
   }

   // Open:
   return XTIFFOpen( filename.c_str(), openMode.c_str() );
}

void ossimTiffOverviewBuilder::closeTiff(TIFF* tif)
{
   XTIFFClose( tif );
}

void ossimTiffOverviewBuilder::setCompressionType(ossim_uint16 compression_type)
{
   switch (compression_type)
   {
   case COMPRESSION_JPEG:
   case COMPRESSION_LZW:
   case COMPRESSION_DEFLATE:
   case COMPRESSION_PACKBITS:
      m_tiffCompressType = compression_type;
      break;
   default:
      m_tiffCompressType = COMPRESSION_NONE;
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << __FILE__ << " " << __LINE__
            << "\nossimTiffOverviewBuilder::setCompressionType Unsupported compression type: "
            << compression_type << "\nDefaulting to none."
            << std::endl;
      }
      break;
   }
}

void ossimTiffOverviewBuilder::setJpegCompressionQuality(ossim_int32 quality)
{
   if (quality > 1 && quality < 101)
   {
      m_jpegCompressQuality = quality;
   }
   else
   {
      m_jpegCompressQuality = DEFAULT_COMPRESS_QUALITY;

      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimTiffOverviewBuilder::setJpegCompressionQuality\n"
         << "\nCompression quality of " << quality << " is out of range!"
         << "\nRange is 100 to 1.  Current quality set to default of 75."
         << std::endl;
   }
}

bool ossimTiffOverviewBuilder::getCopyAllFlag() const
{
   return m_copyAllFlag;
}

void ossimTiffOverviewBuilder::setCopyAllFlag(bool flag)
{
   m_copyAllFlag = flag;
}

void ossimTiffOverviewBuilder::setInternalOverviewsFlag( bool flag )
{
   m_internalOverviewsFlag = flag;
}

bool ossimTiffOverviewBuilder::getInternalOverviewsFlag() const
{
   return m_internalOverviewsFlag;
}

ossimObject* ossimTiffOverviewBuilder::getObject()
{
   return this;
}

const ossimObject* ossimTiffOverviewBuilder::getObject() const
{
   return this;
}

void ossimTiffOverviewBuilder::setOutputFile(const ossimFilename& file)
{
   m_outputFile = file;
}

ossimFilename ossimTiffOverviewBuilder::getOutputFile() const
{
   ossimFilename result;

   if ( buildInternalOverviews() )
   {
      // m_imageHandler pointer good if buildInternalOverviews() returns true.
      result = m_imageHandler->getFilename();
   }
   else
   {
      result = m_outputFile;
   }

   if (m_outputFile == ossimFilename::NIL)
   {
      if ( m_imageHandler.valid() )
      {
         bool usePrefix = (m_imageHandler->getNumberOfEntries()>1?true:false);
         result = m_imageHandler->
            getFilenameWithThisExtension(ossimString("ovr"), usePrefix);
      }
   }
   
   return result;
}

void ossimTiffOverviewBuilder::setOutputTileSize(const ossimIpt& tileSize)
{
   m_tileWidth  = tileSize.x;
   m_tileHeight = tileSize.y;
   m_outputTileSizeSetFlag = true;
}

bool ossimTiffOverviewBuilder::setInputSource(ossimImageHandler* imageSource)
{
   static const char MODULE[] = "ossimTiffOverviewBuilder::initializeFromHandler";

   bool result = ossimOverviewBuilderBase::setInputSource( imageSource );
   
   if ( result )
   {
      if(!m_outputTileSizeSetFlag)
      {
         ossimIpt tileSize;
         ossim::defaultTileSize(tileSize);
         m_tileWidth  = tileSize.x;
         m_tileHeight = tileSize.y;
      }
      
      if (traceDebug())
      {
         CLOG << "DEBUG:"
              << "\nm_tileWidth:   " << m_tileWidth
              << "\nm_tileHeight:  " << m_tileHeight
              << "\nSource image is tiled:  "
              << (m_imageHandler->isImageTiled()?"true":"false")
              << "\nm_imageHandler->getTileWidth():  "
              << m_imageHandler->getTileWidth()
              << "\nm_imageHandler->getTileHeight():  "
              << m_imageHandler->getTileHeight()
              << "\nm_imageHandler->getImageTileWidth():  "
              << m_imageHandler->getImageTileWidth()
              << "\nm_imageHandler->getImageTileHeight():  "
              << m_imageHandler->getImageTileHeight()
              << std::endl;
      }

      switch(m_imageHandler->getOutputScalarType())
      {
         case OSSIM_UINT8:
            m_bitsPerSample = 8;
            m_bytesPerPixel = 1;
            m_sampleFormat  = SAMPLEFORMAT_UINT;
            break;
            
         case OSSIM_USHORT11:
         case OSSIM_UINT16:
            m_bitsPerSample = 16;
            m_bytesPerPixel = 2;
            m_sampleFormat  = SAMPLEFORMAT_UINT;
            break;
            
         case OSSIM_SINT16:
            m_bitsPerSample = 16;
            m_bytesPerPixel = 2;
            m_sampleFormat  = SAMPLEFORMAT_INT;
            break;
            
         case OSSIM_SINT32:
            m_bitsPerSample = 32;
            m_bytesPerPixel = 4;
            m_sampleFormat  = SAMPLEFORMAT_INT;
            break;
            
         case OSSIM_UINT32:
            m_bitsPerSample = 32;
            m_bytesPerPixel = 4;
            m_sampleFormat  = SAMPLEFORMAT_UINT;
            break;
            
         case OSSIM_FLOAT32:
            m_bitsPerSample = 32;
            m_bytesPerPixel = 4;
            m_sampleFormat  = SAMPLEFORMAT_IEEEFP;
            break;
            
         case OSSIM_NORMALIZED_DOUBLE:
         case OSSIM_FLOAT64:
            m_bitsPerSample = 64;
            m_bytesPerPixel = 8;
            m_sampleFormat  = SAMPLEFORMAT_IEEEFP;
            break;
            
         default:
            // Set the error...
            setErrorStatus();
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " ERROR:"
               << "\nUnknow pixel type:  "
               << (ossimScalarTypeLut::instance()->
                   getEntryString(m_imageHandler->getOutputScalarType()))
               << std::endl;
            result = false;
      }

      if ( result )
      {
         m_tileSizeInBytes = m_tileWidth * m_tileHeight * m_bytesPerPixel;
         
         //---
         // Make a buffer to pass to pass to the write tile methods when an image
         // handler returns a null tile.
         //---
         m_nullDataBuffer.resize(m_tileSizeInBytes);
      
         // Fill it with zeroes.
         std::fill(m_nullDataBuffer.begin(), m_nullDataBuffer.end(), 0);
      }
   }
   else
   {
      // Set the error...
      setErrorStatus();
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nSetting image handler as input failed!"
         << std::endl;
   }

   return result;

} // End: ossimTiffOverviewBuilder::setInputSource(ossimImageHandler* imageSource)

bool ossimTiffOverviewBuilder::setOverviewType(const ossimString& type)
{
   bool result = true;
   if (type == "ossim_tiff_nearest")
   {
      m_resampleType =
         ossimFilterResampler::ossimFilterResampler_NEAREST_NEIGHBOR;
   }
   else if (type == "ossim_tiff_box")
   {
      m_resampleType = ossimFilterResampler::ossimFilterResampler_BOX;
   }
   else
   {
      result = false;
   }
   return result;
}

ossimString ossimTiffOverviewBuilder::getOverviewType() const
{
   ossimString type;
   if (m_resampleType == ossimFilterResampler::ossimFilterResampler_NEAREST_NEIGHBOR)
   {
      type = "ossim_tiff_nearest";
   }
   else
   {
      type = "ossim_tiff_box"; // This is default...
   }
   return type;
}

void ossimTiffOverviewBuilder::getTypeNameList(
   std::vector<ossimString>& typeList)const
{
   typeList.push_back(ossimString("ossim_tiff_box"));
   typeList.push_back(ossimString("ossim_tiff_nearest"));
}

void ossimTiffOverviewBuilder::setProperty(ossimRefPtr<ossimProperty> property)
{
   if ( property.valid() )
   {
      if(property->getName() == ossimKeywordNames::COMPRESSION_QUALITY_KW)
      {
         m_jpegCompressQuality = property->valueToString().toInt32();
      }
      else if(property->getName() == ossimKeywordNames::COMPRESSION_TYPE_KW)
      {
         ossimString value = property->valueToString();
         value = value.downcase();
         if(value == "jpeg")
         {
            m_tiffCompressType =  COMPRESSION_JPEG;
         }
         else if(value == "lzw")
         {
            m_tiffCompressType =  COMPRESSION_LZW;
            
         }
         else if(value == "deflate")
         {
            m_tiffCompressType =  COMPRESSION_DEFLATE;
         }
         else if(value == "packbits")
         {
            m_tiffCompressType =  COMPRESSION_PACKBITS;
         }
         else
         {
            m_tiffCompressType = COMPRESSION_NONE;
         }
      }
      else if(property->getName() == COPY_ALL_KW)
      {
         m_copyAllFlag = property->valueToString().toBool();
      }
      else if( property->getName() == INTERNAL_OVERVIEWS_KW )
      {
         m_internalOverviewsFlag = property->valueToString().toBool();
      }
      else if(property->getName() == ossimKeywordNames::OVERVIEW_STOP_DIMENSION_KW)
      {
         m_overviewStopDimension = property->valueToString().toUInt32();
      }
      else if(property->getName() == ossimKeywordNames::OUTPUT_TILE_SIZE_KW)
      {
         ossimIpt ipt;
         
         ipt.toPoint(property->valueToString());
         
         setOutputTileSize(ipt);
      }
   }
}

void ossimTiffOverviewBuilder::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   propertyNames.push_back(ossimKeywordNames::COMPRESSION_QUALITY_KW);
   propertyNames.push_back(ossimKeywordNames::COMPRESSION_TYPE_KW);
   propertyNames.push_back(COPY_ALL_KW);
   propertyNames.push_back(INTERNAL_OVERVIEWS_KW);
   propertyNames.push_back(ossimKeywordNames::OVERVIEW_STOP_DIMENSION_KW);
}

bool ossimTiffOverviewBuilder::canConnectMyInputTo(
   ossim_int32 index,
   const ossimConnectableObject* obj) const
{
   if ( (index == 0) &&
        PTR_CAST(ossimImageHandler, obj) )
   {
      return true;
   }

   return false;
}

bool ossimTiffOverviewBuilder::buildInternalOverviews() const
{
   bool result = false;
   if ( m_internalOverviewsFlag &&  m_imageHandler.valid() )
   {
      if ( m_imageHandler->getClassName() == "ossimTiffTileSource" )
      {
         result = true;
      }
      else
      {
         ossimNotify(ossimNotifyLevel_NOTICE)
            << "Internal overviews opton not supported for class: "
            << m_imageHandler->getClassName()
            << std::endl;
      }
   }
   return result;
}

bool ossimTiffOverviewBuilder::copyR0() const
{
   return ( m_copyAllFlag && !buildInternalOverviews());
}
