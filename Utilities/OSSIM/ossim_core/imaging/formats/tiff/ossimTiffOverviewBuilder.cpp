//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
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
//*******************************************************************
//  $Id: ossimTiffOverviewBuilder.cpp,v 1.44 2005/09/27 10:57:36 gpotts Exp $

#include <cstring>
#include <sstream>
using namespace std;

#include <tiffio.h>

#include <imaging/formats/tiff/ossimTiffOverviewBuilder.h>
#include <imaging/factory/ossimImageDataFactory.h>
#include <base/common/ossimCommon.h>
#include <base/common/ossimErrorCodes.h>
#include <base/context/ossimErrorContext.h>
#include <base/common/ossimStdOutProgress.h>
#include <base/data_types/ossimIpt.h>
#include <base/data_types/ossimDpt3d.h>
#include <base/data_types/ossimIrect.h>
#include <base/data_types/ossimFilename.h>
#include <imaging/formats/ossimImageHandler.h>
#include <imaging/ossimImageData.h>
#include <imaging/ossimImageMetaData.h>
#include <base/common/ossimTrace.h>
#include <base/misc/lookup_tables/ossimScalarTypeLut.h>

RTTI_DEF3(ossimTiffOverviewBuilder, "ossimTiffOverviewBuilder", ossimSource, ossimProcessInterface, ossimConnectableObjectListener)

static ossimTrace traceDebug("ossimTiffOverviewBuilder:degug");


//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimTiffOverviewBuilder::
ossimTiffOverviewBuilder(ossimImageHandler* imageHandler)
   :
      ossimSource(),
      ossimProcessInterface(),
      ossimConnectableObjectListener(),
      theImageHandler(imageHandler),
      theOutputFile(ossimFilename::NIL),
      theNullDataBuffer(NULL),
      theBytesPerPixel(1),
      theBitsPerSample(8),
      theTileWidth(0),
      theTileHeight(0),
      theTileSizeInBytes(0),
      theSampleFormat(0),
      theCurrentTiffDir(0),
      theTiffCompressType(COMPRESSION_NONE),
      theJpegCompressQuality(DEFAULT_COMPRESS_QUALITY),
      theResampleType(ossimFilterResampler::ossimFilterResampler_BOX)
{
   static const char MODULE[]
      = "ossimTiffOverviewBuilder::ossimTiffOverviewBuilder";
   
   if (!theImageHandler)
   {
      // Set the error...
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nNull image handler pointer passed to constructor! Returning..."
         << endl;
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "%s File %s line %d\nNULL pointer passed to constructor!",
                    MODULE,
                    __FILE__,
                    __LINE__);
      return;
   }
   else if (theImageHandler->getErrorStatus() ==
            ossimErrorCodes::OSSIM_ERROR)
   {
      // Set the error...
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nError detected in image handler!  Returning..."
         << endl;
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "%s file %s line %d\nImageHandler error detected!",
                    MODULE,
                    __FILE__,
                    __LINE__);
      return;
   }

   // Note:  Need a default overview tile size in preferences...
   ossimIpt tileSize;
   ossimGetDefaultTileSize(tileSize);
   theTileWidth  = tileSize.x;
   theTileHeight = tileSize.y;

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
           << endl;
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
            << endl;
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_ERROR,
                       "Unknown pixel type!",
                       __FILE__,
                       __LINE__);
         return;
   }

   theTileSizeInBytes = theTileWidth * theTileHeight * theBytesPerPixel;

   //***
   // Make a buffer to pass to pass to the write tile methods when an image
   // handler returns a null tile.
   //***
   theNullDataBuffer = new ossim_uint8[theTileSizeInBytes];
   
   memset(theNullDataBuffer, 0, theTileSizeInBytes); // Fill it with zeroes.

   // Set the output filename to a default.
   theOutputFile = theImageHandler->getFilename();
   if(theImageHandler->getNumberOfEntries() > 1)
   {
      ossim_uint32 currentEntry = theImageHandler->getCurrentEntry();
      theOutputFile.setExtension("");
      theOutputFile += "_e";
      theOutputFile += ossimString::toString(currentEntry);
      
      //---
      // Note:  Set extension was not used here deliberately to avoid messing
      // up a MODIS file in hdf format with multiple '.'s in the file
      // name.
      //---
      theOutputFile += ".ovr";
   }
   else
   {
      theOutputFile.setExtension("ovr");
   }

   if (theOutputFile == theImageHandler->getFilename())
   {
      // Don't allow this.
      theOutputFile = ossimFilename::NIL;
   }
}

//*******************************************************************
// Destructor:
//*******************************************************************
ossimTiffOverviewBuilder::~ossimTiffOverviewBuilder()
{
   if (theNullDataBuffer)
   {
      delete [] theNullDataBuffer;
      theNullDataBuffer = NULL;
   }
}

void ossimTiffOverviewBuilder::setResampleType(ossimFilterResampler::ossimFilterResamplerType resampleType)
{
   theResampleType = resampleType;
}

//*******************************************************************
// Public Method:
//*******************************************************************
bool
ossimTiffOverviewBuilder::buildOverview(const ossimFilename& overview_file,
                                        bool copy_all)
{
   static const char MODULE[] = "ossimTiffOverviewBuilder::buildOverview";

   if (theErrorStatus == ossimErrorCodes::OSSIM_ERROR)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nError status has been previously set!  Returning..."
         << endl;
      return false;
   }

   if (overview_file == theImageHandler->getFilename() ||
       overview_file == ossimFilename::NIL)
   {
      return false;
   }
   else
   {
      theOutputFile = overview_file;
   }
   
   //***
   // See if the file can be opened for writing.
   // Note:  If this file existed previously it will be overwritten.
   //***
   TIFF* tif = TIFFOpen(overview_file.c_str(), "w");
   if (!tif)
   {
      // Set the error...
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_OPEN_FILE_ERROR,
                    "%s file %s line %d\nCannot open file:  %s",
                    MODULE,
                    __FILE__,
                    __LINE__,
                    overview_file.c_str());      
      return false;
   }

   //---
   // Check for a listeners.  If the list is empty, add a standard out listener
   // so that command line apps like img2rr will get some progress.
   //---
   ossimStdOutProgress* progressListener = NULL;
   if (theListenerList.empty())
   {
      progressListener = new ossimStdOutProgress(0, true);
      addListener(progressListener);
   }
   
   setProcessStatus(ossimProcessInterface::PROCESS_STATUS_EXECUTING);
   setPercentComplete(0.0);

   if (copy_all)
   {
      if (!writeR0(tif))
      {
         // Set the error...
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_ERROR,
                       "File %s line %d\nError copying image!",
                       __FILE__,
                       __LINE__);
         TIFFClose(tif);
         if (progressListener)
         {
            removeListener(progressListener);
            delete progressListener;
            progressListener = NULL;
         }
         return false;
      }
   }   

   if (needsAborting())
   {
      TIFFClose(tif);
      if (progressListener)
      {
         removeListener(progressListener);
         delete progressListener;
         progressListener = NULL;
      }
      return false;
   }
   
   //***
   // Calculate the number of reduced resolution data sets (rrds).
   // The image will be reduced until the smallest dimension is
   // It will go all the way to a 1x1 pixel.
   //***
   ossim_int32 number_of_sets = getNumberOfSets();

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " Number of reduced res sets:  " << number_of_sets
         << endl;
   }
   
   if (!number_of_sets)
   {
      ossimNotify(ossimNotifyLevel_INFO)
         << MODULE << " NOTICE:"
         << "\nFile size not large enough to require reduced resolution"
         << " data sets."
         << "\nReturning..." << endl;
      TIFFClose(tif);
      if (progressListener)
      {
         removeListener(progressListener);
         delete progressListener;
         progressListener = NULL;
      }
      return false;
   }

   if (!writeR1(tif))
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Error copying image!" << endl;
      TIFFClose(tif);
      if (progressListener)
      {
         removeListener(progressListener);
         delete progressListener;
         progressListener = NULL;
      }
      return false;
   }
   
   if (needsAborting())
   {
      TIFFClose(tif);
      if (progressListener)
      {
         removeListener(progressListener);
         delete progressListener;
         progressListener = NULL;
      }
      return false;
   }
   
   if (number_of_sets > 1)
   {
      for (ossim_int32 i=2; i<=number_of_sets; ++i)
      {
         if (!writeRn(tif, overview_file, i))
         {
            // Set the error...
            ossimSetError(getClassName(),
                          ossimErrorCodes::OSSIM_WRITE_FILE_ERROR,
                          "\
%s file %s line %d\nError creating reduced res set!",
                          MODULE,
                          __FILE__,
                          __LINE__);            
            TIFFClose(tif);
            if (progressListener)
            {
               removeListener(progressListener);
               delete progressListener;
               progressListener = NULL;
            }
            return false;
         }
         if (needsAborting())
         {
            TIFFClose(tif);
            if (progressListener)
            {
               removeListener(progressListener);
               delete progressListener;
               progressListener = NULL;
            }
            return false;
         }
      }
   }

   // Close the tif file out.
   TIFFClose(tif);
   
   ossimFilename file=overview_file;
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
   
   // Remove the listener if we had one.
   if (progressListener)
   {
      removeListener(progressListener);
      delete progressListener;
      progressListener = NULL;
   }

   return true;
}

//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimTiffOverviewBuilder::writeR0(TIFF* tif)
{
   static const char MODULE[] = "ossimTiffOverviewBuilder::writeR0";

   if (!setTags(tif, 0))
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Error writing tags!" << endl;
      return false;
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
         << endl;
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
               data = static_cast<tdata_t>(theNullDataBuffer);
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
                  << endl;
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
         << MODULE << " Error writing directory!" << endl;
      return false;
   }

   ++theCurrentTiffDir;

   return true;
}

//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimTiffOverviewBuilder::writeR1(TIFF* tif)
{
   static const char MODULE[] = "ossimTiffOverviewBuilder::writeR1";

   //---
   // Resample the original full res data from the ImageHandler to the
   // first reduced res set.
   //---
   setCurrentMessage(ossimString("creating r1..."));
   if (!setTags(tif, 1))
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Error writing tags!" << endl;
      return false;
   }

   ossimRefPtr<ossimImageData> imageBuffer = ossimImageDataFactory::instance()->create(0, theImageHandler);
   if(!imageBuffer.valid())
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "Error returned allocating tile buffer at line " << __LINE__ << " in file " << __FILE__ 
         << endl;
      return false;
   }
//   ossimIrect rect = theImageHandler->getBoundingRect(1);
   ossimIrect rect             = theImageHandler->getBoundingRect();
   ossim_int32 samples         = theImageHandler->getNumberOfSamples();
   ossim_int32 lines           = theImageHandler->getNumberOfLines();
   ossim_int32 outputSamples   = samples / 2;
   ossim_int32 outputLines     = lines   / 2;
   ossim_int32 outputTilesWide = (outputSamples % theTileWidth ?
                                  outputSamples / theTileWidth + 1 :
                                  outputSamples / theTileWidth);
   ossim_int32 outputTilesHigh = (outputLines % theTileHeight ?
                                  outputLines / theTileHeight + 1 :
                                  outputLines / theTileHeight);

   ossim_int32 numberOfTiles   = outputTilesWide * outputTilesHigh;
   imageBuffer->initialize();
   imageBuffer->setImageRectangle(ossimIrect(0, 0, theTileWidth-1, theTileHeight-1));
   // Allocate buffers for the output tiles.
//   ossim_uint8** buf = new ossim_uint8*[theImageHandler->getNumberOfInputBands()];
   uint32 band;
//    for(band=0; band<theImageHandler->getNumberOfInputBands(); ++band)
//    {
//       buf[band] = new ossim_uint8[theTileSizeInBytes];
//    }

//    if (outputLines < theTileHeight || outputSamples < theTileWidth)
//    {
//       // The full tile will not be filled so make sure it's blank.
//       for(band=0; band<theImageHandler->getNumberOfInputBands(); ++band)
//       {
//          memset((void*)buf[band], 0, theTileSizeInBytes);
//       }
//    }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimTiffOverviewBuilder::writeR1 DEBUG:"
         << "\nsamples:          " << samples
         << "\nlines:            " << lines
         << "\noutputTilesWide:  " << outputTilesWide
         << "\noutputTilesHigh:  " << outputTilesHigh
         << "\nnumberOfTiles:    " << numberOfTiles
         << "\nrect:             " << rect
         << endl;
   }

   theNullPixelValues.resize(theImageHandler->getNumberOfOutputBands());
   int tileNumber = 0;
   ossim_uint32 idx = 0;

   for(idx = 0; idx < theNullPixelValues.size(); ++idx)
   {
      theNullPixelValues[idx] = theImageHandler->getNullPixelValue(idx);
      
   }
   //***
   // Tile loop in the line direction.
   //***
   for(int i = 0; i < outputTilesHigh; ++i)
   {
      //***
      // Tile loop in the sample (width) direction.
      //***
      for(int j = 0; j < outputTilesWide; ++j)
      {
         //***
         // For each output tile, four source tiles must be grabbed.
         //***

         // Get the upper left tile.
         ossimIpt origin(j * 2 * theTileWidth,
                         i * 2 * theTileHeight);

         origin = origin + rect.ul();
         ossimRefPtr<ossimImageData> t =
            theImageHandler->getTile(ossimIrect(origin.x,
                                                origin.y,
                                                origin.x +(theTileWidth-1),
                                                origin.y +(theTileHeight-1)));
         imageBuffer->setDataObjectStatus(OSSIM_FULL);
         imageBuffer->makeBlank();
         if(t.valid()&&(t->getBuf())&&(t->getDataObjectStatus()!=OSSIM_EMPTY))
         {
            // Band loop.
            for (band=0; band<theImageHandler->getNumberOfInputBands(); band++)
            {
               tdata_t data;
               data = static_cast<tdata_t>(t->getBuf(band));
               resampleTile(data,
                            imageBuffer->getBuf(band),
                            UL,
                            theNullPixelValues[band]);
            }
         }
         origin.x += theTileWidth;

         t = theImageHandler->getTile(ossimIrect(origin.x,
                                                 origin.y,
                                                 origin.x +(theTileWidth-1),
                                                 origin.y +(theTileHeight-1)));

         if(t.valid()&&(t->getBuf())&&(t->getDataObjectStatus()!=OSSIM_EMPTY))
         {
            // Band loop.
            for (band=0; band<theImageHandler->getNumberOfInputBands(); ++band)
            {
               tdata_t data;
               
               // Grab a pointer to the tile for the band.
               data = static_cast<tdata_t>(t->getBuf(band));
               resampleTile(data,
                            imageBuffer->getBuf(band),
                            UR,
                            theNullPixelValues[band]);
            }
         }

         // Get the lower left tile.
         origin.x -= theTileWidth;
         origin.y += theTileHeight;

         t = theImageHandler->getTile(ossimIrect(origin.x,
                                                 origin.y,
                                                 origin.x +(theTileWidth-1),
                                                 origin.y +(theTileHeight-1)));
         
         // Band loop.
         if(t.valid()&&(t->getBuf())&&(t->getDataObjectStatus()!=OSSIM_EMPTY))
         {
            for (band=0; band<theImageHandler->getNumberOfInputBands(); ++band)
            {
               tdata_t data;
               
               data = static_cast<tdata_t>(t->getBuf(band));
               
               resampleTile(data,
                            imageBuffer->getBuf(band),
                            LL,
                            theNullPixelValues[band]);
            }
         }

         // Get the lower right tile.
         origin.x += theTileWidth;

         t = theImageHandler->getTile(ossimIrect(origin.x,
                                                 origin.y,
                                                 origin.x +(theTileWidth-1),
                                                 origin.y +(theTileHeight-1)));

         //
         // Band loop.
         // Since this is the last quadrant the tile can be written to the
         // tif file in the band loop.
         //
         for (band=0; band<theImageHandler->getNumberOfInputBands(); ++band)
         {
            tdata_t data;
            
            if ( t.valid() && t->getBuf() &&(t->getDataObjectStatus() != OSSIM_EMPTY))
            {
               // Grab a pointer to the tile for the band.
               data = static_cast<tdata_t>(t->getBuf(band));
               resampleTile(data,
                            imageBuffer->getBuf(band),
                            LR,
                            theNullPixelValues[band]);
            }
            
            
            // Write the tile.
            int bytesWritten = 0;
            bytesWritten = TIFFWriteTile(tif,
                                         imageBuffer->getBuf(band),
                                         j*theTileWidth,
                                         i*theTileHeight,
                                         0,        // z
                                         band);    // sample
            
            if (bytesWritten != theTileSizeInBytes)
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << MODULE << " ERROR:"
                  << "Error returned writing tiff tile:  " << i
                  << "\nExpected bytes written:  " << theTileSizeInBytes
                  << "\nBytes written:  " << bytesWritten
                  << endl;
               theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
               
               return false;
            }
            
         }
         
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
         << MODULE << " Error writing directory!" << endl;
      return false;
   }

   theCurrentTiffDir++;

   return true;
}

//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimTiffOverviewBuilder::writeRn(TIFF* tif,
                                       const ossimFilename& overview_file,
                                       ossim_int32 rrds_level)
{
   static const char MODULE[] = "ossimTiffOverviewBuilder::writeRn";

   //---
   // Resample a previous reduced res level to the next level from the
   // same tif file.
   //---
   ostringstream os;
   os << "creating r" << rrds_level << "...";
   setCurrentMessage(os.str());

   TIFF* tifr = NULL;
   
    ossimRefPtr<ossimImageData> imageBuffer = ossimImageDataFactory::instance()->create(0, theImageHandler);
   if(!imageBuffer.valid())
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "Error returned allocating tile buffer at line " << __LINE__ << " in file " << __FILE__ 
         << endl;
      return false;
   }
  if (theCurrentTiffDir)
   {
      //***
      // Open up the same tif file being written to read only to grab the
      // tiles from the previous reduced res set.
      //
      // NOTE:  The 'm' in "rm" is to not memory map the image; hence,
      // saving memory.
      //***
      tifr = TIFFOpen(overview_file.c_str(), "rm");
      if (!tifr)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " ERROR!"
            << "\nCannot open file:  " << overview_file.c_str()
            << "\nReturning..." << endl;
         return false;
      }

      //***
      // Set the directory of the read only tiff.
      //***
      if (!TIFFSetDirectory(tifr, theCurrentTiffDir - 1))
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " ERROR setting directory "
            << ossim_int32(theCurrentTiffDir - 1) << "!" << endl;
         TIFFClose(tifr);
         return false;
      }
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "No previous reduced res set to copy from!"
         << endl;
      return false;
   }

   if (!setTags(tif, rrds_level))
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Error writing tags!" << endl;
      TIFFClose(tifr);
      return false;
   }
  
   ossim_int32 samples;
   ossim_int32 lines;
   TIFFGetField( tifr, TIFFTAG_IMAGEWIDTH, &samples );
   TIFFGetField( tifr, TIFFTAG_IMAGELENGTH, &lines );
   
//    ossim_int32 outputSamples   = (ossim_int32)ceil(samples / 2.0);
//    ossim_int32 outputLines     = (ossim_int32)ceil(lines / 2.0);
   ossim_int32 outputSamples   = samples / 2;
   ossim_int32 outputLines     = lines   / 2;
   ossim_int32 outputTilesWide = outputSamples % theTileWidth ?
                           outputSamples / theTileWidth + 1 :
                                outputSamples / theTileWidth;
   ossim_int32 outputTilesHigh = outputLines % theTileHeight ?
                           outputLines / theTileHeight + 1 :
                           outputLines / theTileHeight;

   ossim_int32 numberOfTiles   = outputTilesWide * outputTilesHigh;

   imageBuffer->initialize();
   imageBuffer->setImageRectangle(ossimIrect(0, 0, theTileWidth-1, theTileHeight-1));
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_INFO)
         << MODULE << " DEBUG:"
         << "\nrrds_level:     " << rrds_level
         << "\noutputSamples:  " << outputSamples
         << "\noutputLines:    " << outputLines
         << "\nnumberOfTiles:  " << numberOfTiles
         << endl;
   }
      
   // Allocate buffers for the output tiles.
//    ossim_uint8** buf =
//       new ossim_uint8*[theImageHandler->getNumberOfInputBands()];
   ossim_uint32 band;
//    for(band=0; band<theImageHandler->getNumberOfInputBands(); band++)
//    {
//       buf[band] = new ossim_uint8[theTileSizeInBytes];
//    }

    ossim_uint8* tbuf = new ossim_uint8[theTileSizeInBytes];

//    if (outputLines < theTileHeight || outputSamples < theTileWidth)
//    {
//       // The full tile will not be filled so make sure it's blank.
//       for(band=0; band<theImageHandler->getNumberOfInputBands(); band++)
//       {
//          memset((void*)buf[band], 0, theTileSizeInBytes);
//       }
//    }
   
   int tileNumber = 0;
   
   //***
   // Tile loop in the line direction.
   //***
   for(int i = 0; i < outputTilesHigh; i++)
   {
      //***
      // Tile loop in the sample (width) direction.
      //***
      for(int j = 0; j < outputTilesWide; j++)
      {
         //***
         // For each output tile, four source tiles must be grabbed.
         //
         // NOTE:
         // Because the output tile bounds can be greater than the source
         // data grabbed from, the origin bounds must be checked after
         // the upper left tile is copied.
         //***

         // Get the upper left tile.
         ossimIpt origin(j * 2 * theTileWidth, i * 2 * theTileHeight);

         // Get the tiles.
         
         // Band loop.
         for (band=0; band<theImageHandler->getNumberOfInputBands(); band++)
         {
            // Grab the upper left source tile for the band.
            int bytes = TIFFReadTile(tifr,
                                     (void*)tbuf,
                                     origin.x,
                                     origin.y,
                                     0,
                                     band);
            
            if (bytes != theTileSizeInBytes)
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << MODULE << " Read Error!"
                  << "\nBytes expected:  " << theTileSizeInBytes
                  << "\nBytes read:      " << bytes
                  << "\nReturning error..." << endl;
               TIFFClose(tifr);
               delete [] tbuf;
               tbuf = NULL;
               return false;
            }
            
            resampleTile((void*)tbuf,
                         imageBuffer->getBuf(band),
                         UL,
			 theNullPixelValues[band]);
         }

         // Get the upper right tile.
         origin.x += theTileWidth;

         if (origin.x < samples && origin.y < lines)
         {
            // Get the tiles.
            
            // Band loop.
            for (band=0; band<theImageHandler->getNumberOfInputBands(); band++)
            {
               // Grab the upper right source tile for the band.
               int bytes = TIFFReadTile(tifr,
                                        (void*)tbuf,
                                        origin.x,
                                        origin.y,
                                        0,
                                        band);
               
               if (bytes != theTileSizeInBytes)
               {
                  ossimNotify(ossimNotifyLevel_WARN)
                     << MODULE << " Read Error!"
                     << "\nBytes expected:  " << theTileSizeInBytes
                     << "\nBytes read:      " << bytes
                     << "\nReturning error..." << endl;
                  TIFFClose(tifr);
                  delete [] tbuf;
                  tbuf = NULL;
                  return false;
               }
               
               resampleTile((void*)tbuf,
                            imageBuffer->getBuf(band),
                            UR,
			    theNullPixelValues[band]);
            }

         }  // End of if (origin.x <= samples && origin.y <= lines)

         // Get the lower left tile.
         origin.x -= theTileWidth;
         origin.y += theTileHeight;

         if (origin.x < samples && origin.y < lines)
         {
            // Get the tiles.
            
            // Band loop.
            for (band=0; band<theImageHandler->getNumberOfInputBands(); band++)
            {
               // Grab the lower left source tile for the band.
               int bytes = TIFFReadTile(tifr,
                                        (void*)tbuf,
                                        origin.x,
                                        origin.y,
                                        0,
                                        band);
               
               if (bytes != theTileSizeInBytes)
               {
                  ossimNotify(ossimNotifyLevel_WARN)
                     << MODULE << " Read Error!"
                     << "\nBytes expected:  " << theTileSizeInBytes
                     << "\nBytes read:      " << bytes
                     << "\nReturning error..." << endl;
                  delete [] tbuf;
                  tbuf = NULL;
                  TIFFClose(tifr);
                  return false;
               }

               resampleTile((void*)tbuf,
                            imageBuffer->getBuf(band),
                            LL,
			    theNullPixelValues[band]);
            }

         }  // End of if (origin.x <= samples && origin.y <= lines)

         // Get the lower right tile.
         origin.x += theTileWidth;

         if (origin.x < samples && origin.y < lines)
         {
            // Get the tiles.
            
            // Band loop.
            for (band=0; band<theImageHandler->getNumberOfInputBands(); band++)
            {
               // Grab the lower right source tile for the band.
               int bytes = TIFFReadTile(tifr,
                                        (void*)tbuf,
                                        origin.x,
                                        origin.y,
                                        0,
                                        band);
               
               if (bytes != theTileSizeInBytes)
               {
                  ossimNotify(ossimNotifyLevel_WARN)
                     << MODULE << " Read Error!"
                     << "\nBytes expected:  " << theTileSizeInBytes
                     << "\nBytes read:      " << bytes
                     << "\nReturning error..." << endl;

                  // Free memory.
                  delete [] tbuf;
                  tbuf = NULL;
                  
                  TIFFClose(tifr);
                  return false;
               }
               
               resampleTile((void*)tbuf,
                            imageBuffer->getBuf(band),
                            LR,
			    theNullPixelValues[band]);
            }

         }  // End of if (origin.x <= samples && origin.y <= lines)

         for (band=0; band<theImageHandler->getNumberOfInputBands(); band++)
         {
            // Write the tile.
            int bytesWritten = 0;
            bytesWritten = TIFFWriteTile(tif,
                                         imageBuffer->getBuf(band),
                                         j*theTileWidth,
                                         i*theTileHeight,
                                         0,        // z
                                         band);    // sample

            if (bytesWritten != theTileSizeInBytes)
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << MODULE << " ERROR:"
                  << "Error returned writing tiff tile:  " << i
                  << "\nExpected bytes written:  " << theTileSizeInBytes
                  << "\nBytes written:  " << bytesWritten
                  << endl;

               delete [] tbuf;
               tbuf = NULL;
               
               TIFFClose(tifr);
               return false;
            }
         }
         
         tileNumber++;

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

   delete [] tbuf;
   tbuf = NULL;
   TIFFClose(tifr);
         
   //***
   // Write the current dirctory.
   //***
   if (!TIFFWriteDirectory(tif))
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Error writing directory!" << endl;
      return false;
   }

   theCurrentTiffDir++;
   
   return true;
}
   
//*******************************************************************
// Private Method:
//*******************************************************************
ossim_int32 ossimTiffOverviewBuilder::getNumberOfSets() const
{
   ossim_int32 a = theImageHandler->getNumberOfSamples(0) <
             theImageHandler->getNumberOfLines(0) ?
             theImageHandler->getNumberOfSamples(0) :
             theImageHandler->getNumberOfLines(0);

   ossim_int32 n = 0;

   while(a > 2)
   {
      a /= 2;
      ++n;
   }

   return n;
}

//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimTiffOverviewBuilder::setTags(TIFF* tif, ossim_int32 rrds_level) const
{
   ossim_int32 imageWidth      = theImageHandler->getNumberOfSamples();
   ossim_int32 imageHeight     = theImageHandler->getNumberOfLines();
   int16 samplesPerPixel = theImageHandler->getNumberOfOutputBands();
   ossim_float64   minSampleValue  = theImageHandler->getMinPixelValue();
   ossim_float64   maxSampleValue  = theImageHandler->getMaxPixelValue();

   if (rrds_level)
   {
      imageWidth  = imageWidth  >> rrds_level;
      imageHeight = imageHeight >> rrds_level;
      TIFFSetField( tif, TIFFTAG_SUBFILETYPE, FILETYPE_REDUCEDIMAGE );
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimTiffOverviewBuilder::setTags DEBUG:"
         << "\nrrds_level:      " << rrds_level
         << "\nimageWidth:      " << imageWidth
         << "\nimageHeight:     " << imageHeight
         << "\nminSampleValue:  " << minSampleValue
         << "\nmaxSampleValue:  " << maxSampleValue
         << endl;
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

//*******************************************************************
// Private Method:
//*******************************************************************
void ossimTiffOverviewBuilder::resampleTile(void*    source_tile,
                                            void*    output_tile,
                                            Quadrant quadrant,
					    double   nullPix)
{
   static const char MODULE[] = "ossimTiffOverviewBuilder::resampleTile";
   
   switch(theImageHandler->getOutputScalarType())
   {
      case OSSIM_UINT8:
      {
         resampleTile((ossim_uint8*)source_tile,
                      (ossim_uint8*)output_tile,
                      quadrant,
                      (ossim_uint8)nullPix);
         break;
      }

      case OSSIM_USHORT11:
      case OSSIM_UINT16:
      {
         resampleTile((ossim_uint16*)source_tile,
                      (ossim_uint16*)output_tile,
                      quadrant,
                      (ossim_uint16)nullPix);
         break;
      }
      case OSSIM_SINT16:
      {
         resampleTile((ossim_sint16*)source_tile,
                      (ossim_sint16*)output_tile,
                      quadrant,
                      (ossim_sint16)nullPix);
         break;
      }

      case OSSIM_UINT32:
      {
         resampleTile((ossim_uint32*)source_tile,
                      (ossim_uint32*)output_tile,
                      quadrant,
                      (ossim_uint32)nullPix);
         break;
      }
      
      case OSSIM_FLOAT32:
      {
         resampleTile((ossim_float32*)source_tile,
                      (ossim_float32*)output_tile,
                      quadrant,
                      (ossim_float32)nullPix);
         break;
      }
         
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
      {
         resampleTile((ossim_float64*)source_tile,
                      (ossim_float64*)output_tile,
                      quadrant,
                      (ossim_float64)nullPix);
         break;
      }
      default:
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " Unkown pixel type!" << endl;
         return;
   }
}

template <class T>
void  ossimTiffOverviewBuilder::resampleTile(T* source_tile,
					     T* output_tile,
					     Quadrant quadrant,
					     T  nullPixel)
{
   //***
   // Buffers are assumed to be tiles.  This will fill one quarter of the
   // output tile
   //***
   ossim_int32 lines   = theTileHeight / 2;
   ossim_int32 samples = theTileWidth  / 2;

   T* s = (T*)source_tile; // source buffer
   T* d = (T*)output_tile; // destination buffer

   // Adjust the d pointer to the start of data.
   switch (quadrant)
   {
      case ossimTiffOverviewBuilder::UR:
         d += theTileWidth / 2;
         break;
      case ossimTiffOverviewBuilder::LL:
         d += theTileHeight / 2 * theTileWidth;
         break;
      case ossimTiffOverviewBuilder::LR:
         d += theTileHeight / 2 * theTileWidth + theTileWidth / 2;
      case ossimTiffOverviewBuilder::UL:
      default:
         break;
   }

   double weight=0.0;
   double value=0.0;
   for (ossim_int32 i=0; i<lines; i++)
   {
      for (ossim_int32 j=0; j<samples; j++)
      {
         switch(theResampleType)
         {
            case ossimFilterResampler::ossimFilterResampler_NEAREST_NEIGHBOR:
            {
               weight = 1.0;
               value  = *(s + i*2*theTileWidth     + j*2);
               break;
            }
            case ossimFilterResampler::ossimFilterResampler_BOX:
            {
               weight = 0;
               value  = 0;
               
               // Grab four pixels from the source, average, and assign to output.
               ossim_float64 ul = *(s + i*2*theTileWidth     + j*2);
               ossim_float64 ur = *(s + i*2*theTileWidth     + j*2+1);
               ossim_float64 ll = *(s + (i*2+1)*theTileWidth + j*2);
               ossim_float64 lr = *(s + (i*2+1)*theTileWidth + j*2+1);
               
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
               break;
            }
            default:
            {
               weight = 1.0;
               value  = *(s + i*2*theTileWidth     + j*2);
               break;
            }
            
         }
         if(weight)
         {
            d[j] = (T)( value/weight  );
         }
         else
         {
            d[j] = nullPixel;
         }
      }
      d += theTileWidth;
   }
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

ossimObject* ossimTiffOverviewBuilder::getObject()
{
   return this;
}

const ossimObject* ossimTiffOverviewBuilder::getObject() const
{
   return this;
}

bool ossimTiffOverviewBuilder::execute()
{
   return buildOverview(theOutputFile);
}

ossimFilename ossimTiffOverviewBuilder::getOutputFile() const
{
   return theOutputFile;
}

void ossimTiffOverviewBuilder::setOutputFile(const ossimFilename& file)
{
   
   theOutputFile = file;
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

