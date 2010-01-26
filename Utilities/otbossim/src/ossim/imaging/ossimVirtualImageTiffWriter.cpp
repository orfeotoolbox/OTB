//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Eric Hirschorn
//
//*******************************************************************
//  $Id: ossimVirtualImageTiffWriter.cpp 11971 2007-11-01 16:44:19Z gpotts $

#include <algorithm>
#include <sstream>

#include <tiffio.h>
#include <xtiffio.h>

#include <ossim/ossimConfig.h>
#include <ossim/init/ossimInit.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimStdOutProgress.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/parallel/ossimMpi.h>
#include <ossim/parallel/ossimMpiMasterOverviewSequencer.h>
#include <ossim/parallel/ossimMpiSlaveOverviewSequencer.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/support_data/ossimGeoTiff.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimOverviewSequencer.h>
#include <ossim/imaging/ossimImageSourceSequencer.h>
#include <ossim/imaging/ossimCibCadrgTileSource.h>
#include <ossim/imaging/ossimVirtualImageTiffWriter.h>

static ossimTrace traceDebug("ossimVirtualImageTiffWriter:debug");

static const long DEFAULT_COMPRESS_QUALITY = 75;

RTTI_DEF1( ossimVirtualImageTiffWriter, "ossimVirtualImageTiffWriter", ossimVirtualImageWriter );

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimVirtualImageTiffWriter.cpp 11971 2007-11-01 16:44:19Z gpotts $";
#endif

ossimVirtualImageTiffWriter::ossimVirtualImageTiffWriter( const ossimFilename& file,
                                                          ossimImageHandler* inputSource,
                                                          bool overviewFlag )
   :
      ossimVirtualImageWriter( file, inputSource, overviewFlag ),
      theTif(0),
      theProjectionInfo(0),
      theCurrentFrameName(""),
      theCurrentFrameNameTmp("")
{
   static const char* MODULE = "ossimVirtualImageTiffWriter::ossimVirtualImageTiffWriter";

   theOutputSubdirectory = "_cache";
   theVirtualWriterType  = "ossim-virtual-tiff"; // this is fixed.
   theMinorVersion       = "1.00";               // for derived writers to set uniquely 

   if ( theOutputFile == ossimFilename::NIL )
   {
      initializeOutputFilenamFromHandler();
   }
   else
   {
      // Temporary header file used to help build Rn for n>1.
      theOutputFileTmp = theOutputFile + ".tmp";
   }

   ossimString driveString;
   ossimString pathString;
   ossimString fileString;
   ossimString extString;
   theOutputFile.split( driveString, pathString, fileString, extString );

   theOutputSubdirectory = fileString + theOutputSubdirectory;

   theOutputImageType = "tiff_tiled_band_separate";
#ifdef OSSIM_ID_ENABLED /* to quell unused variable warning. */
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< "OSSIM_ID: " << OSSIM_ID << endl;
   }
#endif

   switch( theImageHandler->getOutputScalarType() )
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
         {
            // Set the error...
            theErrorStatus = ossimErrorCodes::OSSIM_ERROR;

            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " ERROR:"
               << "\nUnknown pixel type: "
               << ( ossimScalarTypeLut::instance()->getEntryString( theImageHandler->getOutputScalarType() ) )
               << std::endl;

            ossimSetError( getClassName(),
                           ossimErrorCodes::OSSIM_ERROR,
                           "Unknown pixel type!",
                           __FILE__,
                           __LINE__ );
         }
         break;
   }
}

ossimVirtualImageTiffWriter::~ossimVirtualImageTiffWriter()
{
}

bool ossimVirtualImageTiffWriter::openTiff( int resLevel, int row, int col )
{
   static const char* MODULE = "ossimVirtualImageTiffWriter::openTiff";

   if ( theTif ) // Close the existing file pointer.
   {
#ifdef OSSIM_HAS_GEOTIFF
#  if OSSIM_HAS_GEOTIFF 
      XTIFFClose( theTif );
#  else
      TIFFClose( theTif );
#  endif
#else
      TIFFClose( theTif );   
#endif
   }

   // Check for empty file name.
   if (theOutputFile.empty())
   {
      return false;
   }

   ossimString driveString;
   ossimString pathString;
   ossimString fileString;
   ossimString extString;
   theOutputFile.split( driveString, pathString, fileString, extString );

   // If the virtual image header filename is image.ovr, the current frame
   // name is e.g. ./ossim-virtual-tiff/res0/row0/col0.tif

   ossimFilename pathFName( pathString );
   ossimFilename subdirFName1( "." );
   ossimFilename subdirFName2( subdirFName1.dirCat(theOutputSubdirectory) );
   ossimFilename subdirFName3( subdirFName2.dirCat("res") );
   subdirFName3.append( ossimString::toString( resLevel ) );
   ossimFilename subdirFName4( subdirFName3.dirCat("row") );
   subdirFName4.append( ossimString::toString( row ) );
   ossimString newPathString( pathFName.dirCat( subdirFName4 ) );

   ossimFilename driveFName( driveString );
   ossimFilename newPathFName( newPathString );
   ossimFilename newDirFName( driveFName.dirCat( newPathFName ) );
   newDirFName.createDirectory(true);

   ossimString newFileString( "col" );
   newFileString.append( ossimString::toString(col) );

   ossimString newExtString( "tif" );

   theCurrentFrameName.merge( driveString, newPathString, newFileString, newExtString );

   theCurrentFrameNameTmp = theCurrentFrameName + ".tmp";

   ossim_uint64 fourGigs = (static_cast<ossim_uint64>(1024)*
                            static_cast<ossim_uint64>(1024)*
                            static_cast<ossim_uint64>(1024)*
                            static_cast<ossim_uint64>(4));
   ossimIrect bounds = theInputConnection->getBoundingRect();
   ossim_uint64 byteCheck = (static_cast<ossim_uint64>(bounds.width())*
                             static_cast<ossim_uint64>(bounds.height())*
                             static_cast<ossim_uint64>(theInputConnection->getNumberOfOutputBands())*
                             static_cast<ossim_uint64>(ossim::scalarSizeInBytes(theInputConnection->getOutputScalarType())));
   ossimString openMode = "w";
   if( (byteCheck * static_cast<ossim_uint64>(2)) > fourGigs )
   {
      openMode += "8";
   }

   //---
   // See if the file can be opened for writing.
   // Note:  If this file existed previously it will be overwritten.
   //---

#ifdef OSSIM_HAS_GEOTIFF
#  if OSSIM_HAS_GEOTIFF
   theTif = XTIFFOpen( theCurrentFrameNameTmp, openMode );
#  else
   theTif = TIFFOpen( theCurrentFrameNameTmp, openMode );
#  endif
#else
   theTif = TIFFOpen( theCurrentFrameNameTmp, openMode );
#endif

   if ( !theTif )
   {
      setErrorStatus(); // base class
      ossimSetError( getClassName().c_str(),
                     ossimErrorCodes::OSSIM_ERROR,
                     "File %s line %d Module %s Error:\n Error opening file:  %s\n",
                     __FILE__,
                     __LINE__,
                     MODULE,
                     theCurrentFrameNameTmp.c_str() );

      return false;
   }
   return true;
}

bool ossimVirtualImageTiffWriter::closeTiff()
{
   if ( theTif )
   {
#ifdef OSSIM_HAS_GEOTIFF
#  if OSSIM_HAS_GEOTIFF
      XTIFFClose( theTif );
#  else
      TIFFClose( theTif );
#  endif
#else
      TIFFClose( theTif );
#endif
      theTif = 0;
   }

   return true;
}

// Flush the currently open tiff file
void ossimVirtualImageTiffWriter::flushTiff()
{
   if(theTif)
   {
      TIFFFlush(theTif);
   }
}

// Rename the currently open tiff file
void ossimVirtualImageTiffWriter::renameTiff()
{
   theCurrentFrameNameTmp.rename( theCurrentFrameName );
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_INFO)
         << "Wrote file:  " << theCurrentFrameName.c_str() << std::endl;
   }
}

ossimIpt ossimVirtualImageTiffWriter::getOutputTileSize()const
{
   return theOutputTileSize;
}

void ossimVirtualImageTiffWriter::setOutputTileSize( const ossimIpt& tileSize )
{
   if ( (tileSize.x % 16) || (tileSize.y % 16) )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimVirtualImageTiffWriter::changeTileSize ERROR:"
            << "\nTile size must be a multiple of 32!"
            << "\nSize remains:  " << theOutputTileSize
            << std::endl;
      }
      return;
   }

   ossimVirtualImageWriter::setOutputTileSize( tileSize );
}

void ossimVirtualImageTiffWriter::setCompressionType( const ossimString& type )
{
   if( type == "jpeg" )
   {
      theCompressType = COMPRESSION_JPEG;
   }
   else
   if( type == "lzw" )
   {
      theCompressType = COMPRESSION_LZW;
   }
   else
   if( type == "deflate" )
   {
      theCompressType = COMPRESSION_DEFLATE;
   }
   else
   if( type == "packbits" )
   {
      theCompressType = COMPRESSION_PACKBITS;
   }
   else
   {
      theCompressType = COMPRESSION_NONE;
      if (traceDebug())
      {
         ossimSetError( getClassName(),
            ossimErrorCodes::OSSIM_WARNING,
            "ossimVirtualImageTiffWriter::setCompressionType\nfile %s line %d\n\
            Unsupported compression type: %d  Defaulting to none.",
            __FILE__,
            __LINE__,
            theCompressType );
      }
   }
}

void ossimVirtualImageTiffWriter::setCompressionQuality( ossim_int32 quality )
{
   if ( quality > 1 && quality < 101 )
   {
      theCompressQuality = quality;
   }
   else
   {
      theCompressQuality = DEFAULT_COMPRESS_QUALITY;

      if (traceDebug())
      {
         ossimSetError( getClassName(),
            ossimErrorCodes::OSSIM_WARNING,
            "\
            ossimVirtualImageTiffWriter::setCompressionQuality\n%s file %s \
            line %d Compression quality of %d is out of range!\nis out of range!\n\
            Range is 100 to 1.  Current quality set to default of 75.",
            __FILE__,
            __LINE__,
            quality );
      }
   }
}

ossimString ossimVirtualImageTiffWriter::getOverviewType( 
   ossimFilterResampler::ossimFilterResamplerType resamplerType )
{
   ossimString overviewType("unknown");

   if ( resamplerType == ossimFilterResampler::ossimFilterResampler_NEAREST_NEIGHBOR )
   {
      overviewType = "ossim_virtual_tiff_nearest";
   }
   else
   {
      overviewType = "ossim_virtual_tiff_box";
   }

   return overviewType;
}

ossimFilterResampler::ossimFilterResamplerType 
   ossimVirtualImageTiffWriter::getResamplerType( const ossimString& type )
{
   ossimFilterResampler::ossimFilterResamplerType resamplerType =
      ossimFilterResampler::ossimFilterResampler_NEAREST_NEIGHBOR;

   if (type == "ossim_virtual_tiff_nearest")
   {
      resamplerType =
         ossimFilterResampler::ossimFilterResampler_NEAREST_NEIGHBOR;

   }
   else
   if (type == "ossim_virtual_tiff_box")
   {
      resamplerType = ossimFilterResampler::ossimFilterResampler_BOX;
   }

   return resamplerType;
}

bool ossimVirtualImageTiffWriter::isOverviewTypeHandled( const ossimString& type )
{
   bool bIsHandled = false;
   if (type == "ossim_virtual_tiff_nearest")
   {
      bIsHandled = true;
   }
   else
   if (type == "ossim_virtual_tiff_box")
   {
      bIsHandled = true;
   }

   return bIsHandled;
}

void ossimVirtualImageTiffWriter::getTypeNameList(
                                       std::vector<ossimString>& typeList )
{
   typeList.push_back( ossimString("ossim_virtual_tiff_box") );
   typeList.push_back( ossimString("ossim_virtual_tiff_nearest") );
}

bool ossimVirtualImageTiffWriter::writeR0Partial()
{
   static const char MODULE[] = "ossimVirtualImageBuilder::writeR0Partial";

   ossim_int32 numberOfFrames = getNumberOfBuiltFrames( 0, true ); // Frames per image

   ossimIpt    tilesPerFrame  = getNumberOfTilesPerOutputFrame();
   ossim_int32 tilesWide      = tilesPerFrame.x;
   ossim_int32 tilesHigh      = tilesPerFrame.y;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG:"
         << "\nresolution level        : " << 0
         << "\ntiles  per frame (horz) : " << tilesWide
         << "\ntiles  per frame (vert) : " << tilesHigh
         << "\nframes per image (total): " << numberOfFrames
         << std::endl;
   }

   setProcessStatus(ossimProcessInterface::PROCESS_STATUS_EXECUTING);
   setPercentComplete(0.0);

   setCurrentMessage(ossimString("Copying r0..."));

   ossim_int32 frameNumber = 0;
   std::vector<ossimIpt> ossimIptList;
   bool bIsFrameAlreadyDone = false;

   ossim_uint32 nQ = (ossim_uint32)theInputFrameInfoQueue.size();
   ossim_uint32 idxQ;
   for ( idxQ=0; idxQ<nQ; ++idxQ )
   {
      InputFrameInfo* pInfo = theInputFrameInfoQueue[idxQ];
      if ( pInfo && pInfo->isValid() )
      {
         //***
         // Frame loop in the line (height) direction.
         //***
         ossim_int32 iF;
         ossim_int32 yRangeMin = pInfo->yRangeMin[0];
         ossim_int32 yRangeMax = pInfo->yRangeMax[0];
         for( iF = yRangeMin; iF <= yRangeMax; ++iF )
         {
             // Origin of a frame with respect to the entire image.
            ossimIpt originI(0, 0);
            originI.y = iF * theOutputFrameSize.y;

            //***
            // Frame loop in the sample (width) direction.
            //***
            ossim_int32 jF;
            ossim_int32 xRangeMin = pInfo->xRangeMin[0];
            ossim_int32 xRangeMax = pInfo->xRangeMax[0];
            for( jF = xRangeMin; jF <= xRangeMax; ++jF )
            {
               // Add check here to see if (jF,iF) has already been processed for
               // a previous input frame in the queue.
               bIsFrameAlreadyDone = isFrameAlreadyDone( idxQ, 0, jF, iF );

               if ( bIsFrameAlreadyDone == false )
               {
                  originI.x = jF * theOutputFrameSize.x;

                  // Only create an output image frame if data is found
                  // in the input image to write to it. E.g. if the input image
                  // is RPF, there can be missing input frames.
                  bool bCreatedFrame = false;

                  ossimIptList.clear();

                  ossim_int32 tileNumber = 0;

                  //***
                  // Tile loop in the line direction.
                  //***
                  ossim_int32 iT;
                  for( iT = 0; iT < tilesHigh; ++iT )
                  {
                     // Origin of a tile within a single output frame.
                     ossimIpt originF(0, 0);
                     originF.y = iT * theOutputTileSize.y;

                     //***
                     // Tile loop in the sample (width) direction.
                     //***
                     ossim_int32 jT;
                     for( jT = 0; jT < tilesWide; ++jT )
                     {
                        originF.x = jT * theOutputTileSize.x;

                        // Origin of a tile with respect to the entire image.
                        ossimIpt originT(originI);
                        originT += originF;

                        ossimRefPtr<ossimImageData> t =
                           theImageHandler->getTile( ossimIrect( originT.x,
                                                                 originT.y,
                                                                 originT.x + (theOutputTileSize.x-1),
                                                                 originT.y + (theOutputTileSize.y-1) ) );

                        ossimDataObjectStatus doStatus = t->getDataObjectStatus();
                        if ( t.valid() && 
                           (doStatus == OSSIM_PARTIAL || doStatus == OSSIM_FULL) )
                        {
                           if ( bCreatedFrame == false )
                           {
                              // Open a single frame file.
                              openTiff( 0, iF, jF );

                              ossimIrect rect( originI.x, 
                                               originI.y, 
                                               originI.x + theOutputFrameSize.x-1, 
                                               originI.y + theOutputFrameSize.y-1 );

                              if ( !setTags( 0, rect ) )
                              {
                                 ossimNotify(ossimNotifyLevel_WARN)
                                    << MODULE 
                                    << "\nError writing tags!" << std::endl;
                                 closeTiff();
                                 return false;
                              }
                              if ( 1 )
                              {
                                 ossimDrect areaOfInterest(rect);
                                 ossimRefPtr<ossimMapProjectionInfo> projInfo = 
                                    new ossimMapProjectionInfo( theInputMapProjection, 
                                                                areaOfInterest );
                                 ossimGeoTiff::writeTags( theTif, projInfo );
                              }

                              bCreatedFrame = true;
                           }

                           uint32 band;
                           for ( band=0; band<theImageHandler->getNumberOfInputBands(); ++band )
                           {
                              // Grab a pointer to the tile for the band.
                              tdata_t data = static_cast<tdata_t>(t->getBuf(band));

                              // Write the tile.
                              int bytesWritten = 0;
                              bytesWritten = TIFFWriteTile( theTif,
                                                            data,
                                                            originF.x,
                                                            originF.y,
                                                            0,      // z
                                                            band ); // sample

                              if ( bytesWritten != theTileSizeInBytes )
                              {
                                 ossimNotify(ossimNotifyLevel_WARN)
                                    << MODULE << " ERROR:"
                                    << "\nError returned writing tiff tile: " << tileNumber
                                    << " of frame: " << frameNumber
                                    << "\nExpected bytes written:  " << theTileSizeInBytes
                                    << "\nBytes written:  " << bytesWritten
                                    << std::endl;
                                 theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
                                 closeTiff();
                                 return false;
                              }
                           }
                        }
                        else
                        {
                           if ( traceDebug() )
                           {
                              ossimNotify(ossimNotifyLevel_DEBUG)
                                 << MODULE << " DEBUG:"
                                 << "\nNo data found for tiff tile: " << tileNumber
                                 << " of frame: " << frameNumber
                                 << std::endl;
                           }

                           ossimIptList.push_back( originF );
                        }

                        ++tileNumber;

                     } // End of tile loop in the sample (width) direction.

                  } // End of tile loop in the line (height) direction.

                  // Write NULL data into partially occupied output frame.
                  if ( bCreatedFrame == true )
                  {
                     tdata_t data = static_cast<tdata_t>(&(theNullDataBuffer.front()));
                     ossim_uint32 numIpts = (ossim_uint32)ossimIptList.size();
                     ossim_uint32 i;
                     for ( i=0; i<numIpts; ++i )
                     {
                        ossimIpt originF = ossimIptList[i];

                        uint32 band;
                        for ( band=0; band<theImageHandler->getNumberOfInputBands(); ++band )
                        {
                           // Write the tile.
                           int bytesWritten = 0;
                           bytesWritten = TIFFWriteTile( theTif,
                                                         data,
                                                         originF.x,
                                                         originF.y,
                                                         0,      // z
                                                         band ); // sample

                           if ( bytesWritten != theTileSizeInBytes )
                           {
                              ossimNotify(ossimNotifyLevel_WARN)
                                 << MODULE << " ERROR:"
                                 << "\nError returned writing tiff frame: " << frameNumber
                                 << "\nExpected bytes written:  " << theTileSizeInBytes
                                 << "\nBytes written:  " << bytesWritten
                                 << std::endl;
                              theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
                              closeTiff();
                              return false;
                           }
                        }
                     }

                     // Close and rename the new frame file.
                     closeTiff();
                     renameTiff();
                  }

                  ++frameNumber;

               } // is frame already done check

            } // End of frame loop in the sample (width) direction.

            if (needsAborting())
            {
               setPercentComplete(100.0);
               return true;
            }
            else
            if ( bIsFrameAlreadyDone == false )
            {
               double frame = frameNumber;
               setPercentComplete(frame / numberOfFrames * 100.0);
            }

         } // End of frame loop in the line (height) direction.

      } // End of pInfo NULL check

   } // End of queue loop

   return true;
}

bool ossimVirtualImageTiffWriter::writeR0Full()
{
   static const char MODULE[] = "ossimVirtualImageBuilder::writeR0Full";

   ossimIpt    framesPerImage = getNumberOfOutputFrames();
   ossim_int32 framesWide     = framesPerImage.x;
   ossim_int32 framesHigh     = framesPerImage.y;
   ossim_int32 numberOfFrames = framesWide * framesHigh; // Frames per image

   ossimIpt    tilesPerFrame  = getNumberOfTilesPerOutputFrame();
   ossim_int32 tilesWide      = tilesPerFrame.x;
   ossim_int32 tilesHigh      = tilesPerFrame.y;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG:"
         << "\ntiles  per frame (horz) : " << tilesWide
         << "\ntiles  per frame (vert) : " << tilesHigh
         << "\nframes per image (horz) : " << framesWide
         << "\nframes per image (vert) : " << framesHigh
         << "\nframes per image (total): " << numberOfFrames
         << std::endl;
   }

   setProcessStatus(ossimProcessInterface::PROCESS_STATUS_EXECUTING);
   setPercentComplete(0.0);

   setCurrentMessage(ossimString("Copying r0..."));

   ossim_int32 frameNumber = 0;
   std::vector<ossimIpt> ossimIptList;

   //***
   // Frame loop in the line (height) direction.
   //***
   ossim_int32 iF;
   for( iF = 0; iF < framesHigh; ++iF )
   {
      // Origin of a frame with respect to the entire image.
      ossimIpt originI(0, 0);
      originI.y = iF * theOutputFrameSize.y;

      //***
      // Frame loop in the sample (width) direction.
      //***
      ossim_int32 jF;
      for( jF = 0; jF < framesWide; ++jF )
      {
         originI.x = jF * theOutputFrameSize.x;

         // Only create an output image frame if data is found
         // in the input image to write to it. E.g. if the input image
         // is RPF, there can be missing input frames.
         bool bCreatedFrame = false;

         ossimIptList.clear();

         ossim_int32 tileNumber = 0;

         //***
         // Tile loop in the line direction.
         //***
         ossim_int32 iT;
         for( iT = 0; iT < tilesHigh; ++iT )
         {
            // Origin of a tile within a single output frame.
            ossimIpt originF(0, 0);
            originF.y = iT * theOutputTileSize.y;

            //***
            // Tile loop in the sample (width) direction.
            //***
            ossim_int32 jT;
            for( jT = 0; jT < tilesWide; ++jT )
            {
               originF.x = jT * theOutputTileSize.x;

               // Origin of a tile with respect to the entire image.
               ossimIpt originT(originI);
               originT += originF;

               ossimRefPtr<ossimImageData> t =
                              theImageHandler->getTile( ossimIrect( originT.x,
                                                                    originT.y,
                                                                    originT.x + (theOutputTileSize.x-1),
                                                                    originT.y + (theOutputTileSize.y-1) ) );

               ossimDataObjectStatus doStatus = t->getDataObjectStatus();
               if ( t.valid() && 
                   (doStatus == OSSIM_PARTIAL || doStatus == OSSIM_FULL) )
               {
                  if ( bCreatedFrame == false )
                  {
                     // Open a single frame file.
                     openTiff( 0, iF, jF );

                     ossimIrect rect( originI.x, 
                                      originI.y, 
                                      originI.x + theOutputFrameSize.x-1, 
                                      originI.y + theOutputFrameSize.y-1 );

                     if ( !setTags( 0, rect ) )
                     {
                        ossimNotify(ossimNotifyLevel_WARN)
                           << MODULE 
                           << "\nError writing tags!" << std::endl;
                        closeTiff();
                        return false;
                     }
                     if ( 1 )
                     {
                        ossimDrect areaOfInterest(rect);
                        ossimRefPtr<ossimMapProjectionInfo> projInfo = 
                           new ossimMapProjectionInfo( theInputMapProjection, areaOfInterest );
                        ossimGeoTiff::writeTags( theTif, projInfo );
                     }

                     bCreatedFrame = true;
                  }

                  uint32 band;
                  for ( band=0; band<theImageHandler->getNumberOfInputBands(); ++band )
                  {
                     // Grab a pointer to the tile for the band.
                     tdata_t data = static_cast<tdata_t>(t->getBuf(band));

                     // Write the tile.
                     int bytesWritten = 0;
                     bytesWritten = TIFFWriteTile( theTif,
                                                   data,
                                                   originF.x,
                                                   originF.y,
                                                   0,      // z
                                                   band ); // sample

                     if ( bytesWritten != theTileSizeInBytes )
                     {
                        ossimNotify(ossimNotifyLevel_WARN)
                           << MODULE << " ERROR:"
                           << "\nError returned writing tiff tile: " << tileNumber
                           << " of frame: " << frameNumber
                           << "\nExpected bytes written:  " << theTileSizeInBytes
                           << "\nBytes written:  " << bytesWritten
                           << std::endl;
                        theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
                        closeTiff();
                        return false;
                     }
                  }
               }
               else
               {
                  if ( traceDebug() )
                  {
                     ossimNotify(ossimNotifyLevel_DEBUG)
                        << MODULE << " DEBUG:"
                        << "\nNo data found for tiff tile: " << tileNumber
                        << " of frame: " << frameNumber
                        << std::endl;
                  }

                  ossimIptList.push_back( originF );
               }

               ++tileNumber;

            } // End of tile loop in the sample (width) direction.

         } // End of tile loop in the line (height) direction.

         // Write NULL data into partially occupied output frame.
         if ( bCreatedFrame == true )
         {
            tdata_t data = static_cast<tdata_t>(&(theNullDataBuffer.front()));
            ossim_uint32 numIpts = (ossim_uint32)ossimIptList.size();
            ossim_uint32 i;
            for ( i=0; i<numIpts; ++i )
            {
               ossimIpt originF = ossimIptList[i];

               uint32 band;
               for ( band=0; band<theImageHandler->getNumberOfInputBands(); ++band )
               {
                  // Write the tile.
                  int bytesWritten = 0;
                  bytesWritten = TIFFWriteTile( theTif,
                                                data,
                                                originF.x,
                                                originF.y,
                                                0,      // z
                                                band ); // sample

                  if ( bytesWritten != theTileSizeInBytes )
                  {
                     ossimNotify(ossimNotifyLevel_WARN)
                        << MODULE << " ERROR:"
                        << "\nError returned writing tiff frame: " << frameNumber
                        << "\nExpected bytes written:  " << theTileSizeInBytes
                        << "\nBytes written:  " << bytesWritten
                        << std::endl;
                     theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
                     closeTiff();
                     return false;
                  }
               }
            }

            // Close and rename the new frame file.
            closeTiff();
            renameTiff();
         }

         ++frameNumber;

      } // End of frame loop in the sample (width) direction.

      if (needsAborting())
      {
         setPercentComplete(100.0);
         break;
      }
      else
      {
         double frame = frameNumber;
         setPercentComplete(frame / numberOfFrames * 100.0);
      }

   } // End of frame loop in the line (height) direction.

   return true;
}

bool ossimVirtualImageTiffWriter::writeRnPartial( ossim_uint32 resLevel )
{
   static const char MODULE[] = "ossimVirtualImageTiffWriter::writeRnPartial";

   if ( resLevel == 0 )
   {
      return false;
   }

   ossimRefPtr<ossimImageHandler> imageHandler;

   //---
   // If we copied r0 to the virtual image use it instead of the
   // original image handler as it is probably faster.
   //---
   if ( resLevel <= theImageHandler->getNumberOfDecimationLevels() )
   {
      imageHandler = theImageHandler.get();
   }
   else
   {
      imageHandler = ossimImageHandlerRegistry::instance()->open( theOutputFileTmp );
      if ( !imageHandler.valid() )
      {
         // Set the error...
         ossimSetError( getClassName(),
            ossimErrorCodes::OSSIM_OPEN_FILE_ERROR,
            "%s file %s line %d\nCannot open file:  %s",
            MODULE,
            __FILE__,
            __LINE__,
            theOutputFileTmp.c_str() );   
         return false;
      }
   }

   // If the image handler was created in this member function, 
   // make sure we delete it on return.
   bool bLocalImageHandler = (theImageHandler.get() != imageHandler.get());

   //---
   // Set up the sequencer.  This will be one of three depending on if we're
   // running mpi and if we are a master process or a slave process.
   //---
   ossimRefPtr<ossimOverviewSequencer> sequencer;

   if( ossimMpi::instance()->getNumberOfProcessors() > 1 )
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

   sequencer->setImageHandler( imageHandler.get() );

   //---
   // If the source image had built in overviews then we must subtract from the
   // resLevel given to the sequencer or it will get the wrong image rectangle
   // for the area of interest.
   //---
   ossim_uint32 sourceResLevel = ( bLocalImageHandler == false ) ? 
                  resLevel - 1 : 
                  resLevel - theImageHandler->getNumberOfDecimationLevels();

   sequencer->setSourceLevel( sourceResLevel );
   sequencer->setResampleType( theResampleType );
   sequencer->setTileSize( ossimIpt( theOutputTileSize.x, theOutputTileSize.y ) );
   sequencer->initialize();

   // If we are a slave process start the resampling of tiles.
   if ( ossimMpi::instance()->getRank() != 0 )
   {
      sequencer->slaveProcessTiles();
      if ( bLocalImageHandler == true )
      {
         imageHandler = 0;
      }
      return true;
   }

   //---
   // The rest of the method on master node only.
   //---

   setProcessStatus( ossimProcessInterface::PROCESS_STATUS_EXECUTING );
   setPercentComplete( 0.0 );

   ostringstream os;
   os << "creating r" << resLevel << "...";
   setCurrentMessage( os.str() );

   ossim_int32 numberOfFrames = getNumberOfBuiltFrames( resLevel, true ); // partial build

   ossimIpt    tilesPerFrame  = getNumberOfTilesPerOutputFrame(); // resLevel independent
   ossim_int32 tilesWide      = tilesPerFrame.x;
   ossim_int32 tilesHigh      = tilesPerFrame.y;

   ossimIpt    tilesPerImage        = getNumberOfOutputTiles( resLevel ); // assumes full build
   ossim_int32 tilesWideAcrossImage = tilesPerImage.x;
   ossim_int32 tilesHighAcrossImage = tilesPerImage.y;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG:"
         << "\nresolution level        : " << resLevel
         << "\ntiles  per frame (horz) : " << tilesWide
         << "\ntiles  per frame (vert) : " << tilesHigh
         << "\nframes per image (total): " << numberOfFrames
         << std::endl;
   }

   ossim_int32 frameNumber = 0;
   std::vector<ossimIpt> ossimIptList;
   bool bIsFrameAlreadyDone = false;

   ossim_uint32 nQ = (ossim_uint32)theInputFrameInfoQueue.size();
   ossim_uint32 idxQ;
   for ( idxQ=0; idxQ<nQ; ++idxQ )
   {
      InputFrameInfo* pInfo = theInputFrameInfoQueue[idxQ];
      if ( pInfo && pInfo->isValid(resLevel) )
      {
         //***
         // Frame loop in the line (height) direction.
         //***
         ossim_int32 iF;
         ossim_int32 yRangeMin = pInfo->yRangeMin[resLevel];
         ossim_int32 yRangeMax = pInfo->yRangeMax[resLevel];
         for( iF = yRangeMin; iF <= yRangeMax; ++iF )
         {
            // Origin of a frame with respect to the entire image.
            ossimIpt originI(0, 0);
            originI.y = iF * theOutputFrameSize.y;

            //***
            // Frame loop in the sample (width) direction.
            //***
            ossim_int32 jF;
            ossim_int32 xRangeMin = pInfo->xRangeMin[resLevel];
            ossim_int32 xRangeMax = pInfo->xRangeMax[resLevel];
            for( jF = xRangeMin; jF <= xRangeMax; ++jF )
            {
               // Add check here to see if (jF,iF) has already been processed for
               // a previous input frame in the queue for this resolution level.
               bIsFrameAlreadyDone = isFrameAlreadyDone( idxQ, resLevel, jF, iF );

               if ( bIsFrameAlreadyDone == false )
               {
                  originI.x = jF * theOutputFrameSize.x;

                  // Only create an output image frame if data is found
                  // in the input image to write to it. E.g. if the input image
                  // is RPF, there can be missing input frames.
                  bool bCreatedFrame = false;

                  ossimIptList.clear();

                  //***
                  // Tile loop in the line direction.
                  //***
                  ossim_int32 iT;
                  for( iT = 0; iT < tilesHigh; ++iT )
                  {
                     // Origin of a tile within a single output frame.
                     ossimIpt originF(0, 0);
                     originF.y = iT * theOutputTileSize.y;

                     //***
                     // Tile loop in the sample (width) direction.
                     //***
                     ossim_int32 jT;
                     for( jT = 0; jT < tilesWide; ++jT )
                     {
                        originF.x = jT * theOutputTileSize.x;

                        // tile index with respect to the image at resLevel
                        ossim_int32 iI = iF * tilesHigh + iT;
                        ossim_int32 jI = jF * tilesWide + jT;

                        ossim_uint32 tileNumber = -1;
                        ossimRefPtr<ossimImageData> t;
                        bool bWritingImageData = false;
                        if ( iI < tilesHighAcrossImage && jI < tilesWideAcrossImage )
                        {
                           tileNumber = iI * tilesWideAcrossImage + jI;

                           // Grab the resampled tile.
                           sequencer->setCurrentTileNumber( tileNumber );
                           t = sequencer->getNextTile();

                           ossimDataObjectStatus doStatus = t->getDataObjectStatus();
                           if ( t.valid() && 
                              (doStatus == OSSIM_PARTIAL || doStatus == OSSIM_FULL) )
                           {
                              bWritingImageData = true;

                              if ( bCreatedFrame == false )
                              {
                                 // Open a single frame file.
                                 openTiff( resLevel, iF, jF );

                                 ossimIrect bounding_area( originI.x, 
                                    originI.y, 
                                    originI.x + theOutputFrameSize.x - 1, 
                                    originI.y + theOutputFrameSize.y - 1 );

                                 if ( !setTags( resLevel, bounding_area ) )
                                 {
                                    ossimNotify(ossimNotifyLevel_WARN)
                                       << MODULE 
                                       << "\nError writing tags!" << std::endl;
                                    closeTiff();
                                    if ( bLocalImageHandler == true )
                                    {
                                       imageHandler = 0;
                                    }
                                    return false;
                                 }
                                 if ( 1 )
                                 {
                                    ossim_uint32 pixelSizeScale = (ossim_uint32)(1<<resLevel);

                                    // Rescale pixel size based on the resolution level.
                                    ossimDpt degPerPix = theInputMapProjection->getDecimalDegreesPerPixel();
                                    ossimDpt degPerPixScaled = degPerPix * pixelSizeScale;

                                    theInputMapProjection->setDecimalDegreesPerPixel( degPerPixScaled );

                                    ossimDrect areaOfInterest(bounding_area);
                                    ossimRefPtr<ossimMapProjectionInfo> projInfo = 
                                       new ossimMapProjectionInfo( theInputMapProjection, 
                                                                   areaOfInterest );
                                    ossimGeoTiff::writeTags( theTif, projInfo );

                                    // reset to resLevel 0 value
                                    theInputMapProjection->setDecimalDegreesPerPixel( degPerPix );
                                 }

                                 bCreatedFrame = true;
                              }

                              uint32 band;
                              for ( band=0; band<theImageHandler->getNumberOfInputBands(); ++band )
                              {
                                 // Grab a pointer to the tile for the band.
                                 tdata_t data = static_cast<tdata_t>(t->getBuf(band));

                                 int bytesWritten = TIFFWriteTile( theTif,
                                    data,
                                    originF.x,
                                    originF.y,
                                    0,      // z
                                    band ); // sample

                                 if ( bytesWritten != theTileSizeInBytes )
                                 {
                                    ossimNotify(ossimNotifyLevel_WARN)
                                       << MODULE << " ERROR:"
                                       << "\nError returned writing tiff tile: " << tileNumber
                                       << " of frame: " << frameNumber
                                       << "\nExpected bytes written:  " << theTileSizeInBytes
                                       << "\nBytes written:  " << bytesWritten
                                       << std::endl;
                                    theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
                                    closeTiff();
                                    if ( bLocalImageHandler == true )
                                    {
                                       imageHandler = 0;
                                    }
                                    return false;
                                 }
                              } 
                           }
                        }
                        else
                        if ( traceDebug() )
                        {
                           ossimNotify(ossimNotifyLevel_DEBUG)
                              << MODULE << " DEBUG:"
                              << "\nNo data found for tiff tile: " << tileNumber
                              << " of frame: " << frameNumber
                              << std::endl;
                        }

                        if ( bWritingImageData == false )
                        {
                           ossimIptList.push_back( originF );
                        }

                     } // End of tile loop in the sample (width) direction.

                  } // End of tile loop in the line (height) direction.

                  // Write NULL data into partially occupied output frame.
                  if ( bCreatedFrame == true )
                  {
                     tdata_t data = static_cast<tdata_t>(&(theNullDataBuffer.front()));
                     ossim_uint32 numIpts = (ossim_uint32)ossimIptList.size();
                     ossim_uint32 i;
                     for ( i=0; i<numIpts; ++i )
                     {
                        ossimIpt originF = ossimIptList[i];

                        uint32 band;
                        for ( band=0; band<theImageHandler->getNumberOfInputBands(); ++band )
                        {
                           int bytesWritten = TIFFWriteTile( theTif,
                              data,
                              originF.x,
                              originF.y,
                              0,      // z
                              band ); // sample

                           if ( bytesWritten != theTileSizeInBytes )
                           {
                              ossimNotify(ossimNotifyLevel_WARN)
                                 << MODULE << " ERROR:"
                                 << "\nError returned writing tiff frame: " << frameNumber
                                 << "\nExpected bytes written:  " << theTileSizeInBytes
                                 << "\nBytes written:  " << bytesWritten
                                 << std::endl;
                              theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
                              closeTiff();
                              if ( bLocalImageHandler == true )
                              {
                                 imageHandler = 0;
                              }
                              return false;
                           }
                        }
                     }

                     // Close and rename the new frame file.
                     closeTiff();
                     renameTiff();
                  }

                  ++frameNumber;

               } // is output frame already done check

            } // End of frame loop in the sample (width) direction.

            if (needsAborting())
            {
               setPercentComplete(100.0);
               if ( bLocalImageHandler == true )
               {
                  imageHandler = 0;
               }
               return true;
            }
            else
            if ( bIsFrameAlreadyDone == false )
            {
               double frame = frameNumber;
               setPercentComplete(frame / numberOfFrames * 100.0);
            }

         } // End of frame loop in the line (height) direction.

      } // End of pInfo NULL check

   } // End of queue loop

   if ( bLocalImageHandler == true )
   {
      imageHandler = 0;
   }

   return true;
}

bool ossimVirtualImageTiffWriter::writeRnFull( ossim_uint32 resLevel )
{
   static const char MODULE[] = "ossimVirtualImageTiffWriter::writeRnFull";

   if ( resLevel == 0 )
   {
      return false;
   }

   ossimRefPtr<ossimImageHandler> imageHandler;

   //---
   // If we copied r0 to the virtual image use it instead of the
   // original image handler as it is probably faster.
   //---
   if ( resLevel <= theImageHandler->getNumberOfDecimationLevels() )
   {
      imageHandler = theImageHandler.get();
   }
   else
   {
      imageHandler = ossimImageHandlerRegistry::instance()->open( theOutputFileTmp );
      if ( !imageHandler.valid() )
      {
         // Set the error...
         ossimSetError( getClassName(),
                        ossimErrorCodes::OSSIM_OPEN_FILE_ERROR,
                        "%s file %s line %d\nCannot open file:  %s",
                        MODULE,
                        __FILE__,
                        __LINE__,
                        theOutputFileTmp.c_str() );   
         return false;
      }
   }

   // If the image handler was created in this member function, 
   // make sure we delete it on return.
   bool bLocalImageHandler = (theImageHandler.get() != imageHandler.get());

   //---
   // Set up the sequencer.  This will be one of three depending on if we're
   // running mpi and if we are a master process or a slave process.
   //---
   ossimRefPtr<ossimOverviewSequencer> sequencer;

   if( ossimMpi::instance()->getNumberOfProcessors() > 1 )
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

   sequencer->setImageHandler( imageHandler.get() );

   //---
   // If the source image had built in overviews then we must subtract from the
   // resLevel given to the sequencer or it will get the wrong image rectangle
   // for the area of interest.
   //---
   ossim_uint32 sourceResLevel = ( bLocalImageHandler == false ) ? 
            resLevel - 1 : 
            resLevel - theImageHandler->getNumberOfDecimationLevels();

   sequencer->setSourceLevel( sourceResLevel );
   sequencer->setResampleType( theResampleType );
   sequencer->setTileSize( ossimIpt( theOutputTileSize.x, theOutputTileSize.y ) );
   sequencer->initialize();

   // If we are a slave process start the resampling of tiles.
   if ( ossimMpi::instance()->getRank() != 0 )
   {
      sequencer->slaveProcessTiles();
      if ( bLocalImageHandler == true )
      {
         imageHandler = 0;
      }
      return true;
   }

   //---
   // The rest of the method on master node only.
   //---

   setProcessStatus( ossimProcessInterface::PROCESS_STATUS_EXECUTING );
   setPercentComplete( 0.0 );

   ostringstream os;
   os << "creating r" << resLevel << "...";
   setCurrentMessage( os.str() );

   ossimIpt    framesPerImage = getNumberOfOutputFrames( resLevel ); // full build
   ossim_int32 framesWide     = framesPerImage.x;
   ossim_int32 framesHigh     = framesPerImage.y;
   ossim_int32 numberOfFrames = framesWide * framesHigh; // Frames per image

   ossimIpt    tilesPerFrame  = getNumberOfTilesPerOutputFrame(); // resLevel independent
   ossim_int32 tilesWide      = tilesPerFrame.x;
   ossim_int32 tilesHigh      = tilesPerFrame.y;

   ossimIpt    tilesPerImage        = getNumberOfOutputTiles( resLevel ); // assumes full build
   ossim_int32 tilesWideAcrossImage = tilesPerImage.x;
   ossim_int32 tilesHighAcrossImage = tilesPerImage.y;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG:"
         << "\nresolution level        : " << resLevel
         << "\ntiles  per frame (horz) : " << tilesWide
         << "\ntiles  per frame (vert) : " << tilesHigh
         << "\nframes per image (horz) : " << framesWide
         << "\nframes per image (vert) : " << framesHigh
         << "\nframes per image (total): " << numberOfFrames
         << std::endl;
   }

   ossim_int32 frameNumber = 0;
   std::vector<ossimIpt> ossimIptList;

   //***
   // Frame loop in the line (height) direction.
   //***
   ossim_int32 iF;
   for( iF = 0; iF < framesHigh; ++iF )
   {
       // Origin of a frame with respect to the entire image.
      ossimIpt originI(0, 0);
      originI.y = iF * theOutputFrameSize.y;

      //***
      // Frame loop in the sample (width) direction.
      //***
      ossim_int32 jF;
      for( jF = 0; jF < framesWide; ++jF )
      {
         originI.x = jF * theOutputFrameSize.x;

         // Only create an output image frame if data is found
         // in the input image to write to it. E.g. if the input image
         // is RPF, there can be missing input frames.
         bool bCreatedFrame = false;

         ossimIptList.clear();

         //***
         // Tile loop in the line direction.
         //***
         ossim_int32 iT;
         for( iT = 0; iT < tilesHigh; ++iT )
         {
            // Origin of a tile within a single output frame.
            ossimIpt originF(0, 0);
            originF.y = iT * theOutputTileSize.y;

            //***
            // Tile loop in the sample (width) direction.
            //***
            ossim_int32 jT;
            for( jT = 0; jT < tilesWide; ++jT )
            {
               originF.x = jT * theOutputTileSize.x;

               // tile index with respect to the image at resLevel
               ossim_int32 iI = iF * tilesHigh + iT;
               ossim_int32 jI = jF * tilesWide + jT;

               ossim_uint32 tileNumber = -1;
               ossimRefPtr<ossimImageData> t;
               bool bWritingImageData = false;
               if ( iI < tilesHighAcrossImage && jI < tilesWideAcrossImage )
               {
                  tileNumber = iI * tilesWideAcrossImage + jI;

                  // Grab the resampled tile.
                  sequencer->setCurrentTileNumber( tileNumber );
                  t = sequencer->getNextTile();

                  ossimDataObjectStatus doStatus = t->getDataObjectStatus();
                  if ( t.valid() && 
                      (doStatus == OSSIM_PARTIAL || doStatus == OSSIM_FULL) )
                  {
                     bWritingImageData = true;

                     if ( bCreatedFrame == false )
                     {
                        // Open a single frame file.
                        openTiff( resLevel, iF, jF );

                        ossimIrect bounding_area( originI.x, 
                                                  originI.y, 
                                                  originI.x + theOutputFrameSize.x - 1, 
                                                  originI.y + theOutputFrameSize.y - 1 );

                        if ( !setTags( resLevel, bounding_area ) )
                        {
                           ossimNotify(ossimNotifyLevel_WARN)
                              << MODULE 
                              << "\nError writing tags!" << std::endl;
                           closeTiff();
                           if ( bLocalImageHandler == true )
                           {
                              imageHandler = 0;
                           }
                           return false;
                        }
                        if ( 1 )
                        {
                           ossim_uint32 pixelSizeScale = (ossim_uint32)(1<<resLevel);

                           // Rescale pixel size based on the resolution level.
                           ossimDpt degPerPix = theInputMapProjection->getDecimalDegreesPerPixel();
                           ossimDpt degPerPixScaled = degPerPix * pixelSizeScale;

                           theInputMapProjection->setDecimalDegreesPerPixel( degPerPixScaled );

                           ossimDrect areaOfInterest(bounding_area);
                           ossimRefPtr<ossimMapProjectionInfo> projInfo = 
                              new ossimMapProjectionInfo( theInputMapProjection, 
                                                          areaOfInterest );
                           ossimGeoTiff::writeTags( theTif, projInfo );

                           // reset to resLevel 0 value
                           theInputMapProjection->setDecimalDegreesPerPixel( degPerPix );
                        }

                        bCreatedFrame = true;
                     }

                     uint32 band;
                     for ( band=0; band<theImageHandler->getNumberOfInputBands(); ++band )
                     {
                        // Grab a pointer to the tile for the band.
                        tdata_t data = static_cast<tdata_t>(t->getBuf(band));

                        int bytesWritten = TIFFWriteTile( theTif,
                                                          data,
                                                          originF.x,
                                                          originF.y,
                                                          0,      // z
                                                          band ); // sample

                        if ( bytesWritten != theTileSizeInBytes )
                        {
                           ossimNotify(ossimNotifyLevel_WARN)
                              << MODULE << " ERROR:"
                              << "\nError returned writing tiff tile: " << tileNumber
                              << " of frame: " << frameNumber
                              << "\nExpected bytes written:  " << theTileSizeInBytes
                              << "\nBytes written:  " << bytesWritten
                              << std::endl;
                           theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
                           closeTiff();
                           if ( bLocalImageHandler == true )
                           {
                              imageHandler = 0;
                           }
                           return false;
                        }
                     } 
                  }
               }
               else
               if ( traceDebug() )
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << MODULE << " DEBUG:"
                     << "\nNo data found for tiff tile: " << tileNumber
                     << " of frame: " << frameNumber
                     << std::endl;
               }

               if ( bWritingImageData == false )
               {
                  ossimIptList.push_back( originF );
               }

            } // End of tile loop in the sample (width) direction.

         } // End of tile loop in the line (height) direction.

         // Write NULL data into partially occupied output frame.
         if ( bCreatedFrame == true )
         {
            tdata_t data = static_cast<tdata_t>(&(theNullDataBuffer.front()));
            ossim_uint32 numIpts = (ossim_uint32)ossimIptList.size();
            ossim_uint32 i;
            for ( i=0; i<numIpts; ++i )
            {
               ossimIpt originF = ossimIptList[i];

               uint32 band;
               for ( band=0; band<theImageHandler->getNumberOfInputBands(); ++band )
               {
                  int bytesWritten = TIFFWriteTile( theTif,
                                                    data,
                                                    originF.x,
                                                    originF.y,
                                                    0,      // z
                                                    band ); // sample

                  if ( bytesWritten != theTileSizeInBytes )
                  {
                     ossimNotify(ossimNotifyLevel_WARN)
                        << MODULE << " ERROR:"
                        << "\nError returned writing tiff frame: " << frameNumber
                        << "\nExpected bytes written:  " << theTileSizeInBytes
                        << "\nBytes written:  " << bytesWritten
                        << std::endl;
                     theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
                     closeTiff();
                     if ( bLocalImageHandler == true )
                     {
                        imageHandler = 0;
                     }
                     return false;
                  }
               }
            }

            // Close and rename the new frame file.
            closeTiff();
            renameTiff();
         }

         ++frameNumber;

      } // End of frame loop in the sample (width) direction.

      if (needsAborting())
      {
         setPercentComplete(100.0);
         break;
      }
      else
      {
         double frame = frameNumber;
         setPercentComplete(frame / numberOfFrames * 100.0);
      }

   } // End of frame loop in the line (height) direction.

   if ( bLocalImageHandler == true )
   {
      imageHandler = 0;
   }

   return true;
}

bool ossimVirtualImageTiffWriter::setTags( ossim_int32 resLevel,
                                           const ossimIrect& outputRect ) const
{
   if ( theTif == 0 )
   {
      return false;
   }

   int16         samplesPerPixel = theImageHandler->getNumberOfOutputBands();
   ossim_float64 minSampleValue  = theImageHandler->getMinPixelValue();
   ossim_float64 maxSampleValue  = theImageHandler->getMaxPixelValue();

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimVirtualImageBuilder::setTags DEBUG:"
         << "\nrrds_level:      " << resLevel
         << "\nminSampleValue:  " << minSampleValue
         << "\nmaxSampleValue:  " << maxSampleValue
         << std::endl;
   }

   TIFFSetField( theTif, TIFFTAG_PLANARCONFIG,    PLANARCONFIG_SEPARATE );
   TIFFSetField( theTif, TIFFTAG_IMAGEWIDTH,      theOutputFrameSize.x );
   TIFFSetField( theTif, TIFFTAG_IMAGELENGTH,     theOutputFrameSize.y );
   TIFFSetField( theTif, TIFFTAG_BITSPERSAMPLE,   theBitsPerSample );
   TIFFSetField( theTif, TIFFTAG_SAMPLEFORMAT,    theSampleFormat );
   TIFFSetField( theTif, TIFFTAG_SAMPLESPERPIXEL, samplesPerPixel );

   if( theImageHandler->getNumberOfInputBands() == 3 )
      TIFFSetField( theTif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB );
   else
      TIFFSetField( theTif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK );

   TIFFSetField( theTif, TIFFTAG_TILEWIDTH,  theOutputTileSize.x  );
   TIFFSetField( theTif, TIFFTAG_TILELENGTH, theOutputTileSize.y );

   // Set the compression related tags...
   TIFFSetField( theTif, TIFFTAG_COMPRESSION, theCompressType );
   if ( theCompressType == COMPRESSION_JPEG )
   {
      TIFFSetField( theTif, TIFFTAG_JPEGQUALITY,  theCompressQuality );
   }

   // Set the min/max values.
   switch( theImageHandler->getOutputScalarType() )
   {
   case OSSIM_SINT16:
   case OSSIM_FLOAT32:
   case OSSIM_FLOAT64:
   case OSSIM_NORMALIZED_DOUBLE:
      TIFFSetField( theTif, TIFFTAG_SMINSAMPLEVALUE, minSampleValue );
      TIFFSetField( theTif, TIFFTAG_SMAXSAMPLEVALUE, maxSampleValue );
      break;

   case OSSIM_UINT8:
   case OSSIM_USHORT11:
   case OSSIM_UINT16:
   case OSSIM_UINT32:
   default:
      TIFFSetField( theTif, TIFFTAG_MINSAMPLEVALUE, static_cast<int>(minSampleValue) );
      TIFFSetField( theTif, TIFFTAG_MAXSAMPLEVALUE, static_cast<int>(maxSampleValue) );
      break;
   }

   return true;
}

bool ossimVirtualImageTiffWriter::initializeOutputFilenamFromHandler()
{
   if ( !theImageHandler )
   {
      return false;
   }

   // Set the output filename to a default.
   theOutputFile = theImageHandler->getFilename();
   if( theImageHandler->getNumberOfEntries() > 1 )
   {
      ossim_uint32 currentEntry = theImageHandler->getCurrentEntry();
      theOutputFile.setExtension( "" );
      theOutputFile += "_e";
      theOutputFile += ossimString::toString( currentEntry );

      // .ovr: file extension for all overview files in OSSIM.
      // .ovi: "ossim virtual image" for non-overview output images.
      theOutputFile += theOverviewFlag ? ".ovr" : "ovi";
   }
   else
   {
      // .ovr: file extension for all overview files in OSSIM.
      // .ovi: "ossim virtual image" for non-overview output images.
      theOutputFile.setExtension( theOverviewFlag ? ".ovr" : "ovi" );
   }

   if ( theOutputFile == theImageHandler->getFilename() )
   {
      // Don't allow this.
      theOutputFile = ossimFilename::NIL;
   }

   // Temporary header file used to help build Rn for n>1.
   theOutputFileTmp = theOutputFile + ".tmp";

   if ( theOutputFile.empty() )
   {
      return false;
   }

   return true;
}
