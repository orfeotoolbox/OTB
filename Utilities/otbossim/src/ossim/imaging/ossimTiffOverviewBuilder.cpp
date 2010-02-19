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
//  $Id: ossimTiffOverviewBuilder.cpp 15979 2009-11-21 19:23:03Z dburken $

#include <algorithm> /* for std::fill */
// #include <cstring>
#include <sstream>
using namespace std;
#include <xtiffio.h>

#include <ossim/imaging/ossimTiffOverviewBuilder.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimOverviewSequencer.h>
#include <ossim/parallel/ossimMpi.h>
#include <ossim/parallel/ossimMpiMasterOverviewSequencer.h>
#include <ossim/parallel/ossimMpiSlaveOverviewSequencer.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimStdOutProgress.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageMetaData.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimMapProjectionInfo.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/support_data/ossimGeoTiff.h>

RTTI_DEF1(ossimTiffOverviewBuilder,
          "ossimTiffOverviewBuilder",
          ossimOverviewBuilderBase)

static ossimTrace traceDebug("ossimTiffOverviewBuilder:degug");

// Property keywords.
static const char COPY_ALL_KW[] = "copy_all_flag";

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimTiffOverviewBuilder.cpp 15979 2009-11-21 19:23:03Z dburken $";
#endif


//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimTiffOverviewBuilder::ossimTiffOverviewBuilder()
   :
      ossimOverviewBuilderBase(),
      theImageHandler(0),
      theOutputFile(ossimFilename::NIL),
      theNullDataBuffer(0),
      theBytesPerPixel(1),
      theBitsPerSample(8),
      theTileWidth(0),
      theTileHeight(0),
      theTileSizeInBytes(0),
      theSampleFormat(0),
      theCurrentTiffDir(0),
      theTiffCompressType(COMPRESSION_NONE),
      theJpegCompressQuality(DEFAULT_COMPRESS_QUALITY),
      theResampleType(ossimFilterResampler::ossimFilterResampler_BOX),
      theNullPixelValues(),
      theCopyAllFlag(false),
      theOutputTileSizeSetFlag(false)
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
         << "overview stop dimension: " << theOverviewStopDimension
         << std::endl;
   }
}

ossimTiffOverviewBuilder::~ossimTiffOverviewBuilder()
{
   theImageHandler = 0;
}

void ossimTiffOverviewBuilder::setResampleType(
   ossimFilterResampler::ossimFilterResamplerType resampleType)
{
   theResampleType = resampleType;
}

bool ossimTiffOverviewBuilder::buildOverview(
   const ossimFilename& overview_file, bool copy_all)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimTiffOverviewBuilder::buildOverview DEBUG:"
         << "\noverview file: " << overview_file.c_str()
         << "\ncopy_all flag: " << (copy_all?"true":"false")
         << std::endl;
   }


   theOutputFile  = overview_file;
   theCopyAllFlag = copy_all;

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

   if ( !theImageHandler )
   {
      return false;
   }

   theOutputFile = getOutputFile();
   if (theOutputFile == ossimFilename::NIL)
   {
      return false;
   }

   // Check the file.  Disallow same file overview building.
   if (theImageHandler->getFilename() == theOutputFile)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "Source image file and overview file cannot be the same!"
         << std::endl;
      return false;
   }

   // Add .tmp in case process gets aborted to avoid leaving bad .ovr file.
   ossimFilename outputFileTemp = theOutputFile + ".tmp";

   // Required number of levels needed including r0.
   ossim_uint32 requiedResLevels = getRequiredResLevels(theImageHandler.get());

   // Zero based starting resLevel.
   ossim_uint32 startingResLevel  = 0;
   if (!theCopyAllFlag)
   {
      startingResLevel = theImageHandler->getNumberOfDecimationLevels();
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE
         << "\nCurrent number of reduced res sets: "
         << theImageHandler->getNumberOfDecimationLevels()
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
      return false;
   }
   
   ossimStdOutProgress* progressListener = 0; // Only used on master.
   TIFF* tif = 0;                             // Only used on master.

   setProcessStatus(ossimProcessInterface::PROCESS_STATUS_EXECUTING);
   setPercentComplete(0.0);

   if (ossimMpi::instance()->getRank() == 0 )
   {
      ossim_uint64 fourGigs = (static_cast<ossim_uint64>(1024)*
                               static_cast<ossim_uint64>(1024)*
                               static_cast<ossim_uint64>(1024)*
                               static_cast<ossim_uint64>(4));
      ossimIrect bounds = theImageHandler->getBoundingRect();
      ossim_uint64 byteCheck = (static_cast<ossim_uint64>(bounds.width())*
                                static_cast<ossim_uint64>(bounds.height())*
                                static_cast<ossim_uint64>(theImageHandler->getNumberOfOutputBands())*
                                static_cast<ossim_uint64>(ossim::scalarSizeInBytes(theImageHandler->getOutputScalarType())));
      ossimString openMode = "w";
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
      
      //---
      // See if the file can be opened for writing.
      // Note:  If this file existed previously it will be overwritten.
      //---
      tif = openTiff(outputFileTemp, openMode);
      if (!tif)
      {
         // Set the error...
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_OPEN_FILE_ERROR,
                       "%s file %s line %d\nCannot open file:  %s",
                       MODULE,
                       __FILE__,
                       __LINE__,
                       outputFileTemp.c_str());      
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
            ossimSetError(getClassName(),
                          ossimErrorCodes::OSSIM_ERROR,
                          "File %s line %d\nError copying image!",
                          __FILE__,
                          __LINE__);
            closeTiff(tif);
            if (progressListener)
            {
               removeListener(progressListener);
               delete progressListener;
               progressListener = 0;
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
      if ( i <= theImageHandler->getNumberOfDecimationLevels())
      {
         ih = theImageHandler;
      }
      else
      {
         ih = ossimImageHandlerRegistry::instance()->open(outputFileTemp);
         if (!ih)
         {
            // Set the error...
            ossimSetError(getClassName(),
                          ossimErrorCodes::OSSIM_OPEN_FILE_ERROR,
                          "%s file %s line %d\nCannot open file:  %s",
                          MODULE,
                          __FILE__,
                          __LINE__,
                          outputFileTemp.c_str());   
            return false;
         }
      }
      
      if ( !writeRn(ih.get(), tif, i) )
      {
         // Set the error...
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_WRITE_FILE_ERROR,
                       "\
%s file %s line %d\nError creating reduced res set!",
                       MODULE,
                       __FILE__,
                       __LINE__);
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
      
      if (needsAborting())
      {
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
      
      ih = 0;
   }

   if (ossimMpi::instance()->getRank() == 0 )
   {
      if (tif)
      {
         closeTiff(tif);
         tif = 0;
      }
      // Remove the listener if we had one.
      if (progressListener)
      {
         removeListener(progressListener);
         delete progressListener;
         progressListener = 0;
      }
      
      outputFileTemp.rename(theOutputFile);
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_INFO)
            << "Wrote file:  " << theOutputFile.c_str() << std::endl;
      }
      ossimFilename file=theOutputFile;
      file = file.setExtension("omd");
      ossimKeywordlist kwl;
      if(file.exists())
      {
         kwl.addFile(file.c_str());
         
      }
      ossimImageMetaData metaData(theImageHandler->getOutputScalarType(),
                                  theImageHandler->getNumberOfInputBands());
      
      uint32 i= 0;
      
      for(i = 0; i < metaData.getNumberOfBands(); ++i)
      {
         metaData.setMinPix(i,  theImageHandler->getMinPixelValue(i));
         metaData.setMaxPix(i,  theImageHandler->getMaxPixelValue(i));
         metaData.setNullPix(i, theImageHandler->getNullPixelValue(i));
      }
      metaData.saveState(kwl);
      kwl.write(file.c_str());
      
      setCurrentMessage(ossimString("Finished..."));
   }

   
   return true;
}

bool ossimTiffOverviewBuilder::writeR0(TIFF* tif)
{
   static const char MODULE[] = "ossimTiffOverviewBuilder::writeR0";

   ossimIrect rect = theImageHandler->getImageRectangle();

   if (!setTags(tif, rect, 0))
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Error writing tags!" << std::endl;
      return false;
   }
   // now set the geotiff tags
   //
   // Get the geometry from the input.
   const ossimMapProjection* mapProj = 0;
   const ossimImageGeometry* inputGeom = theImageHandler->getImageGeometry();
   if (inputGeom)
      mapProj = PTR_CAST(ossimMapProjection, inputGeom->getProjection());

   if(mapProj)
   {
      ossimDrect areaOfInterest = theImageHandler->getBoundingRect();
      ossimRefPtr<ossimMapProjectionInfo> projInfo = new ossimMapProjectionInfo(mapProj, areaOfInterest);
      ossimGeoTiff::writeTags(tif, projInfo);
   }
   
   ossim_int32 samples         = theImageHandler->getNumberOfSamples();
   ossim_int32 lines           = theImageHandler->getNumberOfLines();
   ossim_int32 tilesWide       = samples % theTileWidth ?
                           samples / theTileWidth + 1 : samples / theTileWidth;
   ossim_int32 tilesHigh       = lines % theTileHeight ?
                           lines / theTileHeight + 1 : lines / theTileHeight;
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
      origin.y = i * theTileHeight;
      
      //***
      // Tile loop in the sample (width) direction.
      //***
      for(int j = 0; j < tilesWide; ++j)
      {
         origin.x = j * theTileWidth;

         ossimRefPtr<ossimImageData> t =
            theImageHandler->getTile(ossimIrect(origin.x,
                                                origin.y,
                                                origin.x +(theTileWidth-1),
                                                origin.y +(theTileHeight-1)));
	 
         //***
         // Band loop.
         //***
         for (uint32 band=0;
              band<theImageHandler->getNumberOfInputBands();
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
               data = static_cast<tdata_t>(&(theNullDataBuffer.front()));
            }

            // Write the tile.
            int bytesWritten = 0;
            bytesWritten = TIFFWriteTile(tif,
                                         data,
                                         origin.x,
                                         origin.y,
                                         0,        // z
                                         band);    // sample

            if (bytesWritten != theTileSizeInBytes)
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << MODULE << " ERROR:"
                  << "Error returned writing tiff tile:  " << i
                  << "\nExpected bytes written:  " << theTileSizeInBytes
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

   ++theCurrentTiffDir;

   return true;
}

bool ossimTiffOverviewBuilder::writeRn(ossimImageHandler* imageHandler,
                                       TIFF* tif,
                                       ossim_uint32 resLevel)
{
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

   ossim_uint32 sourceResLevel =
      imageHandler->getNumberOfDecimationLevels() - 1;

   sequencer->setSourceLevel(sourceResLevel);
   sequencer->setResampleType(theResampleType);
   sequencer->setTileSize( ossimIpt(theTileWidth, theTileHeight) );
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
      closeTiff(tif);
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Error writing tags!" << std::endl;
      return false;
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
         if (t.valid() && ( t->getDataObjectStatus() != OSSIM_NULL ) )
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
               
               if (bytesWritten != theTileSizeInBytes)
               {
                  ossimNotify(ossimNotifyLevel_WARN)
                     << MODULE << " ERROR:"
                     << "Error returned writing tiff tile:  " << i
                     << "\nExpected bytes written:  " << theTileSizeInBytes
                     << "\nBytes written:  " << bytesWritten
                     << std::endl;
                  theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
                  
                  return false;
               }
            }
         }
         x += theTileWidth; // Increment x for next TIFFWriteTile.
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

      y += theTileHeight; // Increment y for next TIFFWriteTile.

   } // End of tile loop in the line (height) direction.

   //---
   // Write the current dirctory.
   //---
   if (!TIFFWriteDirectory(tif))
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Error writing directory!" << std::endl;
      return false;
   }

   ++theCurrentTiffDir;

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
   int16         samplesPerPixel = theImageHandler->getNumberOfOutputBands();
   ossim_float64 minSampleValue  = theImageHandler->getMinPixelValue();
   ossim_float64 maxSampleValue  = theImageHandler->getMaxPixelValue();

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
   TIFFSetField( tif, TIFFTAG_BITSPERSAMPLE, theBitsPerSample );
   TIFFSetField( tif, TIFFTAG_SAMPLEFORMAT, theSampleFormat );
   TIFFSetField( tif, TIFFTAG_SAMPLESPERPIXEL, samplesPerPixel );

   if( theImageHandler->getNumberOfInputBands() == 3 )
      TIFFSetField( tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB );
   else
      TIFFSetField( tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK );
   
   TIFFSetField( tif, TIFFTAG_TILEWIDTH,  theTileWidth  );
   TIFFSetField( tif, TIFFTAG_TILELENGTH, theTileHeight );
   
   // Set the compression related tags...
   TIFFSetField( tif, TIFFTAG_COMPRESSION, theTiffCompressType );
   if (theTiffCompressType == COMPRESSION_JPEG)
   {
      TIFFSetField( tif, TIFFTAG_JPEGQUALITY,  theJpegCompressQuality);
   }
   
   // Set the min/max values.
   switch( theImageHandler->getOutputScalarType() )
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

TIFF* ossimTiffOverviewBuilder::openTiff(const ossimString& filename,
                                         const ossimString& openMode)
{
// #ifdef OSSIM_HAS_GEOTIFF
// #  if OSSIM_HAS_GEOTIFF
   return XTIFFOpen( filename.c_str(), openMode.c_str() );
// #  else
//    return TIFFOpen( filename.c_str(), openMode.c_str() );
// #  endif
// #else
//    return TIFFOpen( filename.c_str(), openMode.c_str() );
// #endif
}

void ossimTiffOverviewBuilder::closeTiff(TIFF* tif)
{
// #ifdef OSSIM_HAS_GEOTIFF
// #  if OSSIM_HAS_GEOTIFF 
      XTIFFClose( tif );
// #  else
//       TIFFClose( tif );
// #  endif
// #else
//       TIFFClose( tif );   
// #endif
}


void ossimTiffOverviewBuilder::setCompressionType(ossim_uint16 compression_type)
{
   switch (compression_type)
   {
   case COMPRESSION_JPEG:
   case COMPRESSION_LZW:
   case COMPRESSION_DEFLATE:
   case COMPRESSION_PACKBITS:
      theTiffCompressType = compression_type;
      break;
   default:
      theTiffCompressType = COMPRESSION_NONE;
      if (traceDebug())
      {
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_WARNING,
                       "\
ossimTiffOverviewBuilder::setCompressionType\nfile %s line %d\n\
Unsupported compression type: %d  Defaulting to none.",
                       __FILE__,
                       __LINE__,
                       compression_type);
      }
      break;
   }
}

void ossimTiffOverviewBuilder::setJpegCompressionQuality(ossim_int32 quality)
{
   if (quality > 1 && quality < 101)
   {
      theJpegCompressQuality = quality;
   }
   else
   {
      theJpegCompressQuality = DEFAULT_COMPRESS_QUALITY;

      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_WARNING,
                    "\
ossimTiffOverviewBuilder::setJpegCompressionQuality\n%s file %s \
line %d Compression quality of %d is out of range!\nis out of range!\n\
Range is 100 to 1.  Current quality set to default of 75.",
                    __FILE__,
                    __LINE__,
                    quality);
   }
}

bool ossimTiffOverviewBuilder::getCopyAllFlag() const
{
   return theCopyAllFlag;
}

void ossimTiffOverviewBuilder::setCopyAllFlag(bool flag)
{
   theCopyAllFlag = flag;
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
   theOutputFile = file;
}

ossimFilename ossimTiffOverviewBuilder::getOutputFile() const
{
   ossimFilename result = theOutputFile;
   if (theOutputFile == ossimFilename::NIL)
   {
      if (theImageHandler.valid())
      {
         bool usePrefix = (theImageHandler->getNumberOfEntries()>1?true:false);
         result = theImageHandler->
            getFilenameWithThisExtension(ossimString("ovr"), usePrefix);
      }
   }
   return result;
}

void ossimTiffOverviewBuilder::setOutputTileSize(const ossimIpt& tileSize)
{
   theTileWidth  = tileSize.x;
   theTileHeight = tileSize.y;
   theOutputTileSizeSetFlag = true;
}

bool ossimTiffOverviewBuilder::setInputSource(ossimImageHandler* imageSource)
{
   static const char MODULE[] =
      "ossimTiffOverviewBuilder::initializeFromHandler";

   theImageHandler         = imageSource;

   if (!theImageHandler)
   {
      // Set the error...
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nNull image handler pointer passed to constructor! Returning..."
         << std::endl;
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "%s File %s line %d\nNULL pointer passed to constructor!",
                    MODULE,
                    __FILE__,
                    __LINE__);
      return false;
   }
   else if (theImageHandler->getErrorStatus() ==
            ossimErrorCodes::OSSIM_ERROR)
   {
      // Set the error...
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nError detected in image handler!  Returning..."
         << std::endl;
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "%s file %s line %d\nImageHandler error detected!",
                    MODULE,
                    __FILE__,
                    __LINE__);
      return false;
   }

   if(!theOutputTileSizeSetFlag)
   {
   // Note:  Need a default overview tile size in preferences...
      ossimIpt tileSize;
      ossim::defaultTileSize(tileSize);
//       if(theImageHandler->isImageTiled())
//       {
//          if(theImageHandler->getBoundingRect().width() != theImageHandler->getImageTileWidth())
//          {
//             tileSize = ossimIpt(theImageHandler->getImageTileWidth(),
//                                 theImageHandler->getImageTileHeight());
//          }
//       }

      theTileWidth  = tileSize.x;
      theTileHeight = tileSize.y;
   }
   if (traceDebug())
   {
      CLOG << "DEBUG:"
           << "\ntheTileWidth:   " << theTileWidth
           << "\ntheTileHeight:  " << theTileHeight
           << "\nSource image is tiled:  "
           << (theImageHandler->isImageTiled()?"true":"false")
           << "\ntheImageHandler->getTileWidth():  "
           << theImageHandler->getTileWidth()
           << "\ntheImageHandler->getTileHeight():  "
           << theImageHandler->getTileHeight()
           << "\ntheImageHandler->getImageTileWidth():  "
           << theImageHandler->getImageTileWidth()
           << "\ntheImageHandler->getImageTileHeight():  "
           << theImageHandler->getImageTileHeight()
           << std::endl;
   }

   switch(theImageHandler->getOutputScalarType())
   {
      case OSSIM_UINT8:
         theBitsPerSample = 8;
         theBytesPerPixel = 1;
         theSampleFormat  = SAMPLEFORMAT_UINT;
         break;
         
      case OSSIM_USHORT11:
      case OSSIM_UINT16:
         theBitsPerSample = 16;
         theBytesPerPixel = 2;
         theSampleFormat  = SAMPLEFORMAT_UINT;
         break;
      
      case OSSIM_SINT16:
         theBitsPerSample = 16;
         theBytesPerPixel = 2;
         theSampleFormat  = SAMPLEFORMAT_INT;
         break;

      case OSSIM_UINT32:
         theBitsPerSample = 32;
         theBytesPerPixel = 4;
         theSampleFormat  = SAMPLEFORMAT_UINT;
         break;
      
      case OSSIM_FLOAT32:
         theBitsPerSample = 32;
         theBytesPerPixel = 4;
         theSampleFormat  = SAMPLEFORMAT_IEEEFP;
         break;
         
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         theBitsPerSample = 64;
         theBytesPerPixel = 8;
         theSampleFormat  = SAMPLEFORMAT_IEEEFP;
         break;
         
      default:
         // Set the error...
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " ERROR:"
            << "\nUnknow pixel type:  "
            << (ossimScalarTypeLut::instance()->
                getEntryString(theImageHandler->getOutputScalarType()))
            << std::endl;
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_ERROR,
                       "Unknown pixel type!",
                       __FILE__,
                       __LINE__);
         return false;
   }

   theTileSizeInBytes = theTileWidth * theTileHeight * theBytesPerPixel;

   //---
   // Make a buffer to pass to pass to the write tile methods when an image
   // handler returns a null tile.
   //---
   theNullDataBuffer.resize(theTileSizeInBytes);

   // Fill it with zeroes.
   std::fill(theNullDataBuffer.begin(), theNullDataBuffer.end(), 0);

   return true;
}

bool ossimTiffOverviewBuilder::setOverviewType(const ossimString& type)
{
   bool result = true;
   if (type == "ossim_tiff_nearest")
   {
      theResampleType =
         ossimFilterResampler::ossimFilterResampler_NEAREST_NEIGHBOR;
   }
   else if (type == "ossim_tiff_box")
   {
      theResampleType = ossimFilterResampler::ossimFilterResampler_BOX;
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
   if (theResampleType ==
       ossimFilterResampler::ossimFilterResampler_NEAREST_NEIGHBOR)
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
         theJpegCompressQuality = property->valueToString().toInt32();
      }
      else if(property->getName() == ossimKeywordNames::COMPRESSION_TYPE_KW)
      {
      ossimString value = property->valueToString();
      value = value.downcase();
      if(value == "jpeg")
      {
         theTiffCompressType =  COMPRESSION_JPEG;
      }
      else if(value == "lzw")
      {
         theTiffCompressType =  COMPRESSION_LZW;
         
      }
      else if(value == "deflate")
      {
         theTiffCompressType =  COMPRESSION_DEFLATE;
      }
      else if(value == "packbits")
      {
         theTiffCompressType =  COMPRESSION_PACKBITS;
      }
      else
      {
         theTiffCompressType = COMPRESSION_NONE;
      }
      }
      else if(property->getName() == COPY_ALL_KW)
      {
         theCopyAllFlag = property->valueToString().toBool();
      }
      else if(property->getName() ==
              ossimKeywordNames::OVERVIEW_STOP_DIMENSION_KW)
      {
         theOverviewStopDimension = property->valueToString().toUInt32();
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

