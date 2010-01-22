//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Eric Hirschorn
//
// Description:
//
// Contains class definition for VirtualImageHandler.
//
//*******************************************************************
//  $Id: ossimVirtualImageHandler.cpp 14655 2009-06-05 11:58:56Z dburken $

#include <xtiffio.h>
#include <cstdlib> /* for abs(int) */
#include <ossim/imaging/ossimVirtualImageHandler.h>
#include <ossim/support_data/ossimGeoTiff.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimTiffTileSource.h>

RTTI_DEF1( ossimVirtualImageHandler, "ossimVirtualImageHandler", ossimImageHandler )

static ossimTrace traceDebug( "ossimVirtualImageHandler:debug" );

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimVirtualImageHandler::ossimVirtualImageHandler()
   :
      ossimImageHandler(),
      theBuffer(0),
      theBufferSize(0),
      theBufferRect(0, 0, 0, 0),
      theNullBuffer(0),
      theSampleFormatUnit(0),
      theMaxSampleValue(0),
      theMinSampleValue(0),
      theBitsPerSample(0),
      theBytesPerPixel(0),
      theImageSubdirectory(""),
      theCurrentFrameName(""),
      theVirtualWriterType(""),
      theMajorVersion(""),
      theMinorVersion(""),
      theCompressType(1),
      theCompressQuality(75),
      theOverviewFlag(false),
      theOpenedFlag(false),
      theR0isFullRes(false),
      theEntryIndex(-1),
      theResLevelStart(0),
      theResLevelEnd(0),
      theSamplesPerPixel(0),
      theNumberOfResLevels(0),
      thePlanarConfig(PLANARCONFIG_SEPARATE),
      theScalarType(OSSIM_SCALAR_UNKNOWN),
      theNumberOfFrames(0),
      theReadMethod(READ_TILE),
      theImageTileWidth(-1),
      theImageTileLength(-1),
      theImageFrameWidth(-1),
      theImageFrameLength(-1),
      theR0NumberOfLines(-1),
      theR0NumberOfSamples(-1),
      thePhotometric(PHOTOMETRIC_MINISBLACK),
      theTif(0),
      theTile(0),
      theImageWidth(0),
      theImageLength(0)
{}

ossimVirtualImageHandler::~ossimVirtualImageHandler()
{
   close();
}

bool ossimVirtualImageHandler::open( const ossimFilename& image_file )
{
   theImageFile = image_file;
   return open();
}

void ossimVirtualImageHandler::close()
{
   theOpenedFlag = false;

   theImageWidth.clear();
   theImageLength.clear();

   if (theBuffer)
   {
      delete [] theBuffer;
      theBuffer = 0;
      theBufferSize = 0;
   }
   if (theNullBuffer)
   {
      delete [] theNullBuffer;
      theNullBuffer = 0;
   }
   ossimImageHandler::close();
}

bool ossimVirtualImageHandler::open()
{
   static const char MODULE[] = "ossimVirtualImageHandler::open";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " Entered..."
         << "\nFile:  " << theImageFile.c_str() << std::endl;
   }

   if(isOpen())
   {
      close();
   }

   if ( theImageFile.empty() )
   {
      return false;
   }
   if ( theImageFile.isReadable() == false )
   {
      return false;
   }

   ossimKeywordlist header_kwl( theImageFile );

   if ( header_kwl.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << " Keywordlist open error detected." << endl;
      }
      return false;
   }

   theOpenedFlag = loadHeaderInfo( header_kwl ) && initializeBuffers();

   return theOpenedFlag;
}

bool ossimVirtualImageHandler::openTiff( int resLevel, int row, int col )
{
   static const char* MODULE = "ossimVirtualImageHandler::openTiff";

   closeTiff();

   // Check for empty file name.
   if (theImageFile.empty())
   {
      return false;
   }

   ossimString driveString;
   ossimString pathString;
   ossimString fileString;
   ossimString extString;
   theImageFile.split( driveString, pathString, fileString, extString );

   // If the virtual image header filename is image.ovr, the current frame
   // name is e.g. ./ossim-virtual-tiff/res0/row0/col0.tif

   ossimFilename pathFName( pathString );
   ossimFilename subdirFName1( "." );
   ossimFilename subdirFName2( subdirFName1.dirCat(theImageSubdirectory) );
   ossimFilename subdirFName3( subdirFName2.dirCat("res") );
   subdirFName3.append( ossimString::toString( resLevel ) );
   ossimFilename subdirFName4( subdirFName3.dirCat("row") );
   subdirFName4.append( ossimString::toString( row ) );
   ossimString newPathString( pathFName.dirCat( subdirFName4 ) );

   ossimFilename driveFName( driveString );
   ossimFilename newPathFName( newPathString );
   ossimFilename newDirFName( driveFName.dirCat( newPathFName ) );

   ossimString newFileString( "col" );
   newFileString.append( ossimString::toString(col) );

   ossimString newExtString( "tif" );

   theCurrentFrameName.merge( driveString, newPathString, newFileString, newExtString );

   // First we do a quick test to see if the file looks like a tiff file.
   unsigned char header[2];

   FILE* fp = fopen( theCurrentFrameName.c_str(), "rb" );
   if ( fp == NULL )
      return false;

   fread( header, 2, 1, fp );
   fclose( fp );

   if( (header[0] != 'M' || header[1] != 'M')
      && (header[0] != 'I' || header[1] != 'I') )
      return false;

   //---
   // See if the file can be opened for reading.
   //---
   ossimString openMode = "rm";

#ifdef OSSIM_HAS_GEOTIFF
#  if OSSIM_HAS_GEOTIFF
   theTif = XTIFFOpen( theCurrentFrameName, openMode );
#  else
   theTif = TIFFOpen( theCurrentFrameName, openMode );
#  endif
#else
   theTif = TIFFOpen( theCurrentFrameName, openMode );
#endif

   if ( !theTif )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " ERROR:\n"
            << "could not open tiff file: "
            << theCurrentFrameName
            << std::endl;
      }
      return false;
   }

   return true;
}

bool ossimVirtualImageHandler::closeTiff()
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

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimVirtualImageHandler::loadHeaderInfo( const ossimKeywordlist& kwl )
{
   static const char MODULE[] = "ossimVirtualImageHandler::loadHeaderInfo";

   bool bRetVal = true;

   // Virtual images currently can only have 1 entry.
   ossimString lookupStr = kwl.find( ossimKeywordNames::NUMBER_ENTRIES_KW );
   if ( lookupStr.empty() == false )
   {
      ossim_int16 numEntries = lookupStr.toInt16();
      if ( numEntries != 1 )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\nERROR: Number of entries (" << numEntries << ") in virtual image header != 1."
            << std::endl;

         bRetVal = false;
      }
      else
      {
         std::vector<ossimString> keyList = kwl.findAllKeysThatContains(
            ossimKeywordNames::ENTRY_KW );

         int numKeys = (int)keyList.size();
         if ( numKeys != 1 )
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE
               << "\nERROR: Number of entry lines (" << numKeys << ") in virtual image header != 1."
               << std::endl;

            bRetVal = false;
         }
         else
         {
            ossimString key = keyList[0];
            ossimString lookupStr = kwl.find( key );
            if ( lookupStr.empty() == false )
            {
               theEntryIndex = lookupStr.toInt16();
            }
            else
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << MODULE
                  << "\nERROR: No valid entries found in virtual image header."
                  << std::endl;

               bRetVal = false;
            }
         }
      }
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE
            << "\nERROR: Number of entries not found in virtual image header."
            << std::endl;
      }

      bRetVal = false;
   }

   if ( theEntryIndex > -1 )
   {
      ossimString prefix = "image";
      prefix += ossimString::toString( theEntryIndex ) + ".";

      loadGeometryKeywordEntry( kwl, prefix );
      loadGeneralKeywordEntry ( kwl, prefix );
      loadNativeKeywordEntry  ( kwl, prefix );
   }

   return bRetVal;
}

void ossimVirtualImageHandler::loadNativeKeywordEntry( const ossimKeywordlist& kwl,
                                                       const ossimString& prefix )
{
   static const char MODULE[] = "ossimVirtualImageHandler::loadNativeKeywordEntry";

   ossimString extPrefix = prefix + ossimString( "virtual" ) + ".";

   ossimString lookupStr = kwl.find( extPrefix, "subdirectory" );
   if ( lookupStr.empty() == false )
   {
      theImageSubdirectory = lookupStr;
   }
   lookupStr = kwl.find( extPrefix, "writer_type" );
   if ( lookupStr.empty() == false )
   {
      theVirtualWriterType = lookupStr;
   }
   lookupStr = kwl.find( extPrefix, "frame_size_x" );
   if ( lookupStr.empty() == false )
   {
      theImageFrameWidth = lookupStr.toInt32();
   }
   lookupStr = kwl.find( extPrefix, "frame_size_y" );
   if ( lookupStr.empty() == false )
   {
      theImageFrameLength = lookupStr.toInt32();
   }
   lookupStr = kwl.find( extPrefix, "tile_size_x" );
   if ( lookupStr.empty() == false )
   {
      theImageTileWidth = lookupStr.toInt32();
   }
   lookupStr = kwl.find( extPrefix, "tile_size_y" );
   if ( lookupStr.empty() == false )
   {
      theImageTileLength = lookupStr.toInt32();
   }
   lookupStr = kwl.find( extPrefix, "version_major" );
   if ( lookupStr.empty() == false )
   {
      theMajorVersion = lookupStr;
   }
   lookupStr = kwl.find( extPrefix, "version_minor" );
   if ( lookupStr.empty() == false )
   {
      theMinorVersion = lookupStr;
   }
   lookupStr = kwl.find( extPrefix, "overview_flag" );
   if ( lookupStr.empty() == false )
   {
      theOverviewFlag = lookupStr.toBool();
      setStartingResLevel( theOverviewFlag ? 1 : 0 );
   }
   lookupStr = kwl.find( extPrefix, "includes_r0" );
   if ( lookupStr.empty() == false )
   {
      theR0isFullRes = lookupStr.toBool();
   }
   lookupStr = kwl.find( extPrefix, "bits_per_sample" );
   if ( lookupStr.empty() == false )
   {
      theBitsPerSample = lookupStr.toUInt16();
   }
   lookupStr = kwl.find( extPrefix, "bytes_per_pixel" );
   if ( lookupStr.empty() == false )
   {
      theBytesPerPixel = lookupStr.toUInt32();
   }
   lookupStr = kwl.find( extPrefix, "resolution_level_starting" );
   if ( lookupStr.empty() == false )
   {
      theResLevelStart = lookupStr.toUInt16();
   }
   lookupStr = kwl.find( extPrefix, "resolution_level_ending" );
   if ( lookupStr.empty() == false )
   {
      theResLevelEnd = lookupStr.toUInt16();
   }

   // number of resolution levels available in the virtual image
   theNumberOfResLevels = theResLevelEnd - theResLevelStart + 1;

   theImageWidth.resize(theNumberOfResLevels);
   theImageLength.resize(theNumberOfResLevels);
   theNumberOfFrames.resize(theNumberOfResLevels);

   extPrefix += ossimString( "resolution_level_" );

   ossim_uint32 r;
   ossim_uint32 d=0;
   for ( r=theResLevelStart; r<=theResLevelEnd; ++r )
   {
      theImageWidth [d] = theR0NumberOfSamples >> r;
      theImageLength[d] = theR0NumberOfLines   >> r;

      ossimString fullPrefix = extPrefix + ossimString::toString( r ) + ".";

      ossimIpt nFrames;
      lookupStr = kwl.find( fullPrefix, "number_of_frames_x" );
      if ( lookupStr.empty() == false )
      {
         nFrames.x = lookupStr.toInt32();
      }
      lookupStr = kwl.find( fullPrefix, "number_of_frames_y" );
      if ( lookupStr.empty() == false )
      {
         nFrames.y = lookupStr.toInt32();

      }

      theNumberOfFrames[d++] = nFrames;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE
         << "\nVirtual image information:"
         << "\nSubdirectory for frames:  " << theImageSubdirectory
         << "\nWriter type:              " << theVirtualWriterType
         << "\nFrame size (x):           " << theImageFrameWidth
         << "\nFrame size (y):           " << theImageFrameLength
         << "\nTile size  (x):           " << theImageTileWidth
         << "\nTile size  (y):           " << theImageTileLength
         << "\nMajor version:            " << theMajorVersion
         << "\nMinor version:            " << theMinorVersion
         << "\nOverview flag (boolean):  " << theOverviewFlag
         << "\nStarting reduced res set: " << theResLevelStart
         << "\nEnding reduced res sets:  " << theResLevelEnd
         << std::endl;

         d=0;
         for ( r=theResLevelStart; r<=theResLevelEnd; ++r )
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "Number of frames["       << r << "].x:    " << theNumberOfFrames[d].x
               << "\nNumber of frames["     << r << "].y:    " << theNumberOfFrames[d].y
               << "\nVirtual image width["  << r << "]:   "    << theImageWidth[d]
               << "\nVirtual image length[" << r << "]:  "     << theImageLength[d]
               << std::endl;
            ++d;
         }
   }
}

void ossimVirtualImageHandler::loadGeometryKeywordEntry( const ossimKeywordlist& kwl,
                                                         const ossimString& prefix )
{
   static const char MODULE[] = "ossimVirtualImageHandler::loadGeometryKeywordEntry";

   ossimKeywordlist tempKwl(kwl);
   tempKwl.stripPrefixFromAll( prefix );

   const char* lookup = tempKwl.find(ossimKeywordNames::TYPE_KW);
   if ( lookup )
   {
      if ( !theGeometry.get() )
      {
         // allocate an empty geometry if nothing present
         theGeometry = new ossimImageGeometry();
      }
      theGeometry->loadState( tempKwl );
   }
   else
   {
      if ( traceDebug() )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\nNo projection type found in: " 
            << theImageFile
            << std::endl;
      }
   }
}

void ossimVirtualImageHandler::loadGeneralKeywordEntry( const ossimKeywordlist& kwl,
                                                        const ossimString& prefix )
{
   static const char MODULE[] = "ossimVirtualImageHandler::loadGeneralKeywordEntry";

   /* Get the nul, min, max values, as a function of band index */
   loadMetaData( kwl );

   ossimString lookupStr = kwl.find( prefix, ossimKeywordNames::NUMBER_INPUT_BANDS_KW );
   if ( lookupStr.empty() == false )
   {
      theSamplesPerPixel = lookupStr.toUInt16();

      if( theSamplesPerPixel == 3 )
         thePhotometric = PHOTOMETRIC_RGB;
      else
         thePhotometric = PHOTOMETRIC_MINISBLACK;
   }

   lookupStr = kwl.find( prefix, ossimKeywordNames::NUMBER_LINES_KW );
   if ( lookupStr.empty() == false )
   {
      theR0NumberOfLines = lookupStr.toInt32();
   }

   lookupStr = kwl.find( prefix, ossimKeywordNames::NUMBER_SAMPLES_KW );
   if ( lookupStr.empty() == false )
   {
      theR0NumberOfSamples = lookupStr.toInt32();
   }

   lookupStr = kwl.find( prefix, "radiometry" );
   theScalarType = OSSIM_SCALAR_UNKNOWN;
   if ( lookupStr.empty() == false )
   {
      if ( lookupStr.contains("8-bit") )
      {
         theScalarType = OSSIM_UINT8;
      }
      else
      if ( lookupStr.contains("11-bit") )
      {
         theScalarType = OSSIM_USHORT11;
      }
      else
      if ( lookupStr.contains("16-bit unsigned") )
      {
         theScalarType =  OSSIM_UINT16;
      }
      else
      if ( lookupStr.contains("16-bit signed") )
      {
         theScalarType =  OSSIM_SINT16;
      }
      else
      if ( lookupStr.contains("32-bit unsigned") )
      {
         theScalarType =  OSSIM_UINT32;
      }
      else
      if ( lookupStr.contains("float") )
      {
         theScalarType =  OSSIM_FLOAT32;
      }
      else
      if ( lookupStr.contains("normalized float") )
      {
         theScalarType =  OSSIM_FLOAT32;
      }
      else
      {
         /* Do nothing */

         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\nERROR: Unrecognized pixel scalar type description: " 
            <<  lookupStr
            << std::endl;
      }
   }
}

void ossimVirtualImageHandler::loadMetaData( const ossimKeywordlist& kwl )
{
   theMetaData.clear();
   theMetaData.loadState( kwl );
}

ossim_uint32 ossimVirtualImageHandler::getNumberOfLines( ossim_uint32 resLevel ) const
{
   ossim_uint32 result = 0;

   if ( theOpenedFlag && isValidRLevel(resLevel) )
   {
      //---
      // If we have r0 our reslevels are the same as the callers so
      // no adjustment necessary.
      //---
      if ( !theStartingResLevel || theR0isFullRes ) // not an overview or has r0.
      {
         //---
         // If we have r0 our reslevels are the same as the callers so
         // no adjustment necessary.
         //---
         if (resLevel < theNumberOfResLevels)
         {
            result = theImageLength[resLevel];
         }
      }
      else // this is an overview without r0
      if (resLevel >= theStartingResLevel)
      {
         //---
         // Adjust the level to be relative to the reader using this as
         // overview.
         //---
         ossim_uint32 level = resLevel - theStartingResLevel;
         if (level < theNumberOfResLevels)
         {
            result = theImageLength[level];
         }
      }
   }
   else
   if ( resLevel < theStartingResLevel &&
        theStartingResLevel > 0 &&
       !theR0isFullRes &&
        theNumberOfResLevels > 0 )
   {
      result = theImageLength[0] * (1<<(theStartingResLevel-resLevel));
   }

   return result;
}

ossim_uint32 ossimVirtualImageHandler::getNumberOfSamples( ossim_uint32 resLevel ) const
{
   ossim_uint32 result = 0;

   if ( theOpenedFlag && isValidRLevel(resLevel) )
   {
      //---
      // If we have r0 our reslevels are the same as the callers so
      // no adjustment necessary.
      //---
      if ( !theStartingResLevel || theR0isFullRes ) // not an overview or has r0.
      {
         //---
         // If we have r0 our reslevels are the same as the callers so
         // no adjustment necessary.
         //---
         if (resLevel < theNumberOfResLevels)
         {
            result = theImageWidth[resLevel];
         }
      }
      else // this is an overview without r0
      if (resLevel >= theStartingResLevel)
      {
         //---
         // Adjust the level to be relative to the reader using this as
         // overview.
         //---
         ossim_uint32 level = resLevel - theStartingResLevel;
         if (level < theNumberOfResLevels)
         {
            result = theImageWidth[level];
         }
      }
   }
   else
   if ( resLevel < theStartingResLevel &&
        theStartingResLevel > 0  && 
       !theR0isFullRes && 
        theNumberOfResLevels > 0 )
   {
      result = theImageWidth[0] * (1<<(theStartingResLevel-resLevel));
   }

   return result;
}

ossimIrect ossimVirtualImageHandler::getImageRectangle(ossim_uint32 resLevel) const
{
   ossimIrect result;

   if( theOpenedFlag && isValidRLevel(resLevel) )
   {
      ossim_int32 lines   = getNumberOfLines(resLevel);
      ossim_int32 samples = getNumberOfSamples(resLevel);
      if( !lines || !samples )
      {
         result.makeNan();
      }
      else
      {
         result = ossimIrect(0, 0, samples-1, lines-1);
      }
   }
   else
   if ( resLevel < theStartingResLevel &&
        theStartingResLevel > 0 &&
       !theR0isFullRes &&
        theNumberOfResLevels > 0 )
   {
      ossim_uint32 scale   = (1<<(theStartingResLevel-resLevel));
      ossim_uint32 lines   = theImageLength[0] * scale;
      ossim_uint32 samples = theImageWidth[0]  * scale;

      result = ossimIrect(0, 0, samples-1, lines-1);
   }
   else
   {
      result.makeNan();
   }

   return result;
}

ossim_uint32 ossimVirtualImageHandler::getNumberOfDecimationLevels() const
{
   ossim_uint32 result = theNumberOfResLevels;

   if ( theOverviewFlag && theR0isFullRes )
   {
      // Don't count r0.
      --result;
   }

   return result;
}

//*******************************************************************
// Public method:
//*******************************************************************
ossimScalarType ossimVirtualImageHandler::getOutputScalarType() const
{
   return theScalarType;
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimVirtualImageHandler::getTileWidth() const
{
   if( isOpen() )
   {
      return theImageTileWidth;
   }

   return 0;
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimVirtualImageHandler::getTileHeight() const
{
   if( isOpen() )
   {
      return theImageTileLength;
   }

   return 0;
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimVirtualImageHandler::getFrameWidth() const
{
   if( isOpen() )
   {
      return theImageFrameWidth;
   }

   return 0;
}  

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimVirtualImageHandler::getFrameHeight() const
{
   if( isOpen() )
   {
      return theImageFrameLength;
   }

   return 0;
}

ossimRefPtr<ossimImageData> ossimVirtualImageHandler::getTile(
   const ossimIrect& tile_rect, ossim_uint32 resLevel )
{
   if (theTile.valid())
   {
      // Image rectangle must be set prior to calling getTile.
      theTile->setImageRectangle(tile_rect);

      if ( getTile( *(theTile.get()), resLevel ) == false )
      {
         if (theTile->getDataObjectStatus() != OSSIM_NULL)
         {
            theTile->makeBlank();
         }
      }
   }

   theTile->setImageRectangle(tile_rect);
   return theTile;
}

bool ossimVirtualImageHandler::getTile( ossimImageData& result,
                                        ossim_uint32 resLevel )
{
   static const char MODULE[] ="ossimVirtualImageHandler::getTile(ossimImageData&,ossim_uint32)";

   bool status = false;

   //---
   // Not open, this tile source bypassed, or invalid res level,
   // return a blank tile.
   //---
   if( isOpen() && isSourceEnabled() && isValidRLevel(resLevel) )
   {
      ossimIrect tile_rect = result.getImageRectangle();

      // This should be the zero base image rectangle for this res level.
      ossimIrect image_rect = getImageRectangle(resLevel);

      //---
      // See if any point of the requested tile is in the image.
      //---
      if ( tile_rect.intersects(image_rect) )
      {
         // Initialize the tile if needed as we're going to stuff it.
         if (result.getDataObjectStatus() == OSSIM_NULL)
         {
            result.initialize();
         }

         ossimIrect clip_rect = tile_rect.clipToRect(image_rect);

         if ( !tile_rect.completely_within(clip_rect) )
         {
            //---
            // We're not going to fill the whole tile so start with a
            // blank tile.
            //---
            result.makeBlank();
         }

         // Load the tile buffer with data from the tif.
         if ( loadTile( tile_rect, result, resLevel ) )
         {
            result.validate();
            status = true;
         }
         else
         {
            // Would like to change this to throw ossimException.(drb)
            status = false;
            if(traceDebug())
            {
               // Error in filling buffer.
               ossimNotify(ossimNotifyLevel_WARN)
                  << MODULE
                  << " Error filling buffer. Return status = false..."
                  << std::endl;
            }
         }
      } // matches:  if ( tile_rect.intersects(image_rect) )
      else 
      {
         // No part of requested tile within the image rectangle.
         status = true; // Not an error.
         result.makeBlank();
      }

   } // matches: if( isOpen() && isSourceEnabled() && isValidRLevel(resLevel) )

   return status;
}

bool ossimVirtualImageHandler::loadTile( const ossimIrect& virtual_clip_rect,
                                         ossimImageData& result,
                                         ossim_uint32 resLevel )
{
   static const char MODULE[] = "ossimVirtualImageHandler::loadTile";

   ossimInterleaveType type = ( thePlanarConfig == PLANARCONFIG_CONTIG ) ? 
                              OSSIM_BIP : OSSIM_BIL;

   ossimIpt tilesPerFrame = getNumberOfTilesPerFrame();

   ossim_int32 tilesWide = tilesPerFrame.x;
   ossim_int32 tilesHigh = tilesPerFrame.y;

   ossim_int32 virtual_minx, virtual_miny, virtual_maxx, virtual_maxy;
   virtual_clip_rect.getBounds( virtual_minx, virtual_miny, 
                                virtual_maxx, virtual_maxy );

   // Get the indices of the frame that contains the top-left corner
   ossim_int32 rowFrameIdxI = virtual_miny / theImageFrameLength;
   ossim_int32 colFrameIdxI = virtual_minx / theImageFrameWidth;

   // Get the indices of the frame that contains the bottom-right corner
   ossim_int32 rowFrameIdxF = virtual_maxy / theImageFrameLength;
   ossim_int32 colFrameIdxF = virtual_maxx / theImageFrameWidth;

   // Get the virtual line,sample of the frame origin
   ossimIpt frame_shiftI( colFrameIdxI * theImageFrameWidth, 
                          rowFrameIdxI * theImageFrameLength );

   ossimIrect clip_rectI( virtual_clip_rect );
   clip_rectI -= frame_shiftI;

   result.setImageRectangle( clip_rectI );

   //***
   // Frame loop in the line (height) direction.
   //***
   ossim_int32 rowFrameIdx;
   for( rowFrameIdx = rowFrameIdxI; rowFrameIdx <= rowFrameIdxF; ++rowFrameIdx )
   {
      // Origin of a tile within a single output frame.
      ossimIpt originOF(0, 0);
      originOF.y = (rowFrameIdx-rowFrameIdxI) * theImageFrameLength;

      //***
      // Frame loop in the sample (width) direction.
      //***
      ossim_int32 colFrameIdx;
      for( colFrameIdx = colFrameIdxI; colFrameIdx <= colFrameIdxF; ++colFrameIdx )
      {
         originOF.x = (colFrameIdx-colFrameIdxI) * theImageFrameWidth;

         // Open a single frame file for reading.
         bool bOpenedTiff = openTiff( resLevel, rowFrameIdx, colFrameIdx );

         //***
         // Tile loop in the line direction.
         //***
         ossim_int32 iT;
         for( iT = 0; iT < tilesHigh; ++iT )
         {
            // Origin of a tile within a single input frame.
            ossimIpt originIF(0, 0);
            originIF.y = iT * theImageTileLength;

            //***
            // Tile loop in the sample (width) direction.
            //***
            ossim_int32 jT;
            for( jT = 0; jT < tilesWide; ++jT )
            {
               originIF.x = jT * theImageTileWidth;

               ossimIrect tile_rectOF( originOF.x + originIF.x,
                                       originOF.y + originIF.y,
                                       originOF.x + originIF.x + theImageTileWidth  - 1,
                                       originOF.y + originIF.y + theImageTileLength - 1 );

               if ( tile_rectOF.intersects(clip_rectI) )
               {
                  ossimIrect tile_clip_rect = tile_rectOF.clipToRect(clip_rectI);

                  if ( thePlanarConfig == PLANARCONFIG_CONTIG )
                  {
                     if ( bOpenedTiff )
                     {
                        ossim_int32 tileSizeRead = TIFFReadTile( theTif,
                                                                 theBuffer,
                                                                 originIF.x, 
                                                                 originIF.y, 
                                                                 0, 0 );
                        if ( tileSizeRead > 0 )
                        {
                           result.loadTile( theBuffer,
                                            tile_rectOF,
                                            tile_clip_rect,
                                            type );
                        }
                        else
                        if( tileSizeRead < 0 )
                        {
                           if( traceDebug() )
                           {
                              ossimNotify(ossimNotifyLevel_WARN)
                                 << MODULE << " Read Error!"
                                 << "\nReturning error...  " << endl;
                           }
                           theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
                           closeTiff();
                           return false;
                        }
                     }
                     else
                     {
                        // fill with NULLs
                        result.loadTile( theNullBuffer,
                                         tile_rectOF,
                                         tile_clip_rect,
                                         type );
                     }
                  }
                  else
                  {
                     // band separate tiles...
                     for ( ossim_uint32 band=0; band<theSamplesPerPixel; ++band )
                     {
                        if ( bOpenedTiff )
                        {
                           ossim_int32 tileSizeRead = TIFFReadTile( theTif,
                                                                    theBuffer,
                                                                    originIF.x, 
                                                                    originIF.y, 
                                                                    0,
                                                                    band );
                           if ( tileSizeRead > 0 )
                           {
                              result.loadBand( theBuffer,
                                               tile_rectOF,
                                               tile_clip_rect,
                                               band );
                           }
                           else
                           if ( tileSizeRead < 0 )
                           {
                              if (traceDebug())
                              {
                                 ossimNotify(ossimNotifyLevel_WARN)
                                    << MODULE << " Read Error!"
                                    << "\nReturning error...  " << endl;
                              }
                              theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
                              closeTiff();
                              return false;
                           }
                        }
                        else
                        {
                           // fill with NULLs
                           result.loadBand( theNullBuffer,
                                            tile_rectOF,
                                            tile_clip_rect,
                                            band );
                        }
                     }
                  }
               }
            }
         }
      }
   }

   // All done with the current frame file.
   closeTiff();
   return true;
}

ossimIpt ossimVirtualImageHandler::getNumberOfTilesPerFrame() const
{
   ossim_int32 frameSamples  = theImageFrameWidth;
   ossim_int32 frameLines    = theImageFrameLength;

   ossim_int32 tileSamples   = theImageTileWidth;
   ossim_int32 tileLines     = theImageTileLength;

   ossim_int32 tilesWide     = (frameSamples % tileSamples) ?
                               (frameSamples / tileSamples) + 1 : (frameSamples / tileSamples);
   ossim_int32 tilesHigh     = (frameLines % tileLines) ?
                               (frameLines / tileLines) + 1 : (frameLines / tileLines);

   return ossimIpt( tilesWide, tilesHigh );
}

bool ossimVirtualImageHandler::isValidRLevel( ossim_uint32 resLevel ) const
{
   bool result = false;

   //---
   // If we have r0 our reslevels are the same as the callers so
   // no adjustment necessary.
   //---
   if ( !theStartingResLevel || theR0isFullRes) // Not an overview or has r0.
   {
      result = (resLevel < theNumberOfResLevels);
   }
   else if (resLevel >= theStartingResLevel) // Used as overview.
   {
      result = ( (resLevel - theStartingResLevel) < theNumberOfResLevels);
   }

   return result;
}

bool ossimVirtualImageHandler::allocateBuffer()
{
   //***
   // Allocate memory for a buffer to hold data grabbed from the tiff file.
   //***
   ossim_uint32 buffer_size=0;
   switch ( theReadMethod )
   {
      case READ_TILE:
         if ( thePlanarConfig == PLANARCONFIG_CONTIG )
         {
            buffer_size = theImageTileWidth  *
                          theImageTileLength *
                          theBytesPerPixel   * 
                          theSamplesPerPixel;
         }
         else
         {
            buffer_size = theImageTileWidth  *
                          theImageTileLength *
                          theBytesPerPixel;
         }
         break;

      case READ_RGBA_U8_TILE:
      case READ_RGBA_U8_STRIP:
      case READ_RGBA_U8A_STRIP:
      case READ_SCAN_LINE:
      default:
         ossimNotify(ossimNotifyLevel_WARN)
            << "Read method not implemented!" << endl;
         print(ossimNotify(ossimNotifyLevel_WARN));
         return false;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimVirtualImageHandler::allocateBuffer DEBUG:"
         << "\nbuffer_size:  " << buffer_size
         << endl;
   }

   theBufferRect.makeNan();

   bool bSuccess = true;
   if ( buffer_size != theBufferSize )
   {
      theBufferSize = buffer_size;
      if ( theBuffer )
      {
         delete [] theBuffer;
      }
      if ( theNullBuffer )
      {
         delete [] theNullBuffer;
      }

      // ESH 05/2009 -- Fix for ticket #738:  
      // image_info crashing on aerial_ortho image during ingest
      try
      {
         theBuffer = new ossim_uint8[buffer_size];
         theNullBuffer = new ossim_uint8[buffer_size];
      }
      catch(...)
      {
         if ( theBuffer )
         {
            delete [] theBuffer;
            theBuffer = 0;
         }
         if ( theNullBuffer )
         {
            delete [] theNullBuffer;
            theNullBuffer = 0;
         }

         bSuccess = false;
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimVirtualImageHandler::allocateBuffer WARN:"
               << "\nNot enough memory: buffer_size:  " << buffer_size
               << endl;
         }
      }
   }

   // initialize the NULL buffer
   ossim_uint32 b;
   for ( b=0; b<buffer_size; ++b )
   {
      theNullBuffer[b] = 0;
   }

   return bSuccess;
}

ossim_uint32 ossimVirtualImageHandler::getImageTileWidth() const
{
   return theImageTileWidth;
}

ossim_uint32 ossimVirtualImageHandler::getImageTileHeight() const
{
   return theImageTileLength;
}

ossimString ossimVirtualImageHandler::getLongName() const
{
   return ossimString( "Virtual Image Handler" );
}

ossimString ossimVirtualImageHandler::getShortName() const
{
   return ossimString( "Virtual Image Handler" );
}

std::ostream& ossimVirtualImageHandler::print( std::ostream& os ) const
{
   //***
   // Use a keyword format.
   //***
   os << "image_file:                    " << theImageFile
      << "\nsamples_per_pixel:           " << theSamplesPerPixel
      << "\nbits_per_sample:             " << theBitsPerSample
      << "\nsample_format_unit:          " << theSampleFormatUnit
      << "\nmin_sample_value:            " << theMinSampleValue
      << "\nmax_sample_value:            " << theMaxSampleValue
      << "\ntheNumberOfResLevels:        " << theNumberOfResLevels
      << "\ntile_width:                  " << theImageTileWidth
      << "\ntile_length:                 " << theImageTileLength
      << "\nphotometric:                 " << thePhotometric
      << "\nr0_is_full_res:              " << theR0isFullRes;

   for ( ossim_uint32 i=0; i<theNumberOfResLevels; ++i )
   {
      os << "\ndirectory[" << i << "]"
         << "\nimage width:     " << theImageWidth[i]
         << "\nimage_length:    " << theImageLength[i];
      os << endl;
   }

   if ( theTile.valid() )
   {
      os << "\nOutput tile dump:\n" << *theTile << endl;
   }

   os << endl;
   
   return ossimSource::print( os );
}

ossim_uint32 ossimVirtualImageHandler::getNumberOfInputBands() const
{
   return theSamplesPerPixel;
}

ossim_uint32 ossimVirtualImageHandler::getNumberOfOutputBands () const
{
   return getNumberOfInputBands();
}

bool ossimVirtualImageHandler::isOpen() const
{
   return theOpenedFlag;
}

bool ossimVirtualImageHandler::hasR0() const
{
   return theR0isFullRes;
}

double ossimVirtualImageHandler::getMinPixelValue( ossim_uint32 band ) const
{
   if( theMetaData.getNumberOfBands() )
   {
      return ossimImageHandler::getMinPixelValue( band );
   }
   return theMinSampleValue;
}

double ossimVirtualImageHandler::getMaxPixelValue( ossim_uint32 band ) const
{
   if( theMetaData.getNumberOfBands() )
   {
      return ossimImageHandler::getMaxPixelValue( band );
   }
   return theMaxSampleValue;
}

bool ossimVirtualImageHandler::initializeBuffers()
{
   if( theBuffer )
   {
      delete [] theBuffer;
      theBuffer = 0;
   }
   if( theNullBuffer )
   {
      delete [] theNullBuffer;
      theNullBuffer = 0;
   }

   ossimImageDataFactory* idf = ossimImageDataFactory::instance();

   theTile = idf->create( this, this );

   // The width and height must be set prior to call to allocateBuffer.
   theTile->setWidth (theImageTileWidth);
   theTile->setHeight(theImageTileLength);

   //
   // Tiles are constructed with no buffer storage.  Call initialize for
   // "theTile" to allocate memory.  Leave "theBlankTile" with a
   // ossimDataObjectStatus of OSSIM_NULL since no data will ever be
   // stuffed in it.
   //
   theTile->initialize();

   return allocateBuffer();
}


void ossimVirtualImageHandler::setProperty( ossimRefPtr<ossimProperty> property )
{
   if( !property.valid() )
   {
      return;
   }

   ossimImageHandler::setProperty( property );
}

ossimRefPtr<ossimProperty> ossimVirtualImageHandler::getProperty( const ossimString& name )const
{
   if( name == "file_type" )
	{
		return new ossimStringProperty( name, "TIFF" );
	}
	
   return ossimImageHandler::getProperty( name );
}

void ossimVirtualImageHandler::getPropertyNames( std::vector<ossimString>& propertyNames )const
{
   ossimImageHandler::getPropertyNames( propertyNames );
	propertyNames.push_back( "file_type" );
}

void ossimVirtualImageHandler::validateMinMax()
{
   double tempNull = ossim::defaultNull( theScalarType );
   double tempMax  = ossim::defaultMax ( theScalarType );
   double tempMin  = ossim::defaultMin ( theScalarType );
   
   if( ( theMinSampleValue == tempNull ) || ossim::isnan( theMinSampleValue ) ) 
   {
      theMinSampleValue = tempMin;
   }
   if( ( theMaxSampleValue == tempNull ) || ossim::isnan( theMaxSampleValue ) )
   {
      theMaxSampleValue = tempMax;
   }
}
