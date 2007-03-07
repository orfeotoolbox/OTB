//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author:  David Burken
//          Frank Warmerdam (warmerdam@pobox.com)
//
// Description:
//
// Contains class definition for TiffTileSource.
//
//*******************************************************************
//  $Id: ossimTiffTileSource.cpp,v 1.104 2006/01/05 14:59:29 dburken Exp $

#include <imaging/formats/tiff/ossimTiffTileSource.h>
#include <support_data/tiff/ossimGeoTiff.h>
#include <base/common/ossimConstants.h>
#include <base/common/ossimTrace.h>
#include <base/data_types/ossimIpt.h>
#include <base/data_types/ossimDpt.h>
#include <base/data_types/ossimFilename.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/data_types/ellipse/ossimEllipsoid.h>
#include <base/data_types/datum/ossimDatum.h>
#include <base/property/ossimBooleanProperty.h>
#include <imaging/factory/ossimImageDataFactory.h>
#include <projections/map_projections/ossimEquDistCylProjection.h>
#include <projections/map_projections/ossimAlbersProjection.h>
#include <projections/map_projections/ossimCylEquAreaProjection.h>
#include <projections/map_projections/ossimEquDistCylProjection.h>
#include <projections/map_projections/ossimVanDerGrintenProjection.h>
#include <projections/map_projections/ossimLambertConformalConicProjection.h>
#include <projections/map_projections/ossimMercatorProjection.h>
#include <projections/map_projections/ossimMillerProjection.h>
#include <projections/map_projections/ossimOrthoGraphicProjection.h>
#include <projections/map_projections/ossimPolyconicProjection.h>
#include <projections/map_projections/ossimSinusoidalProjection.h>
#include <projections/map_projections/ossimTransMercatorProjection.h>
#include <projections/map_projections/ossimCassiniProjection.h>
#include <projections/map_projections/ossimUtmProjection.h>

RTTI_DEF1(ossimTiffTileSource, "ossimTiffTileSource", ossimImageHandler)

static ossimTrace traceDebug("ossimTiffTileSource:degug");

#ifndef MAX
#  define MIN(a,b)      ((a<b) ? a : b)
#  define MAX(a,b)      ((a>b) ? a : b)
#endif

#define OSSIM_TIFF_UNPACK_R4(value) ( (value)&0x000000FF)
#define OSSIM_TIFF_UNPACK_G4(value) ( ((value)>>8)&0x000000FF)
#define OSSIM_TIFF_UNPACK_B4(value) ( ((value)>>16)&0x000000FF)
#define OSSIM_TIFF_UNPACK_A4(value) ( ((value)>>24)&0x000000FF)

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimTiffTileSource::ossimTiffTileSource()
   :
      ossimImageHandler(),
      theTile(0),
      theBuffer(0),
      theBufferSize(0),
      theBufferRect(0, 0, 0, 0),
      theBufferRLevel(0),
      theTiffPtr(0),
      theSamplesPerPixel(0),
      theBitsPerSample(0),
      theSampleFormatUnit(0),
      theMaxSampleValue(0),
      theMinSampleValue(0),
      theNumberOfDirectories(0),
      theCurrentDirectory(0),
      theR0isFullRes(false),
      theBytesPerPixel(0),
      theScalarType(OSSIM_SCALAR_UNKNOWN),
      theApplyColorPaletteFlag(true),
      theImageWidth(0),
      theImageLength(0),
      theReadMethod(0),
      thePlanarConfig(0),
      thePhotometric(0),
      theRowsPerStrip(0),
      theImageTileWidth(0),
      theImageTileLength(0)
{}

ossimTiffTileSource::~ossimTiffTileSource()
{
   close();
}

ossimRefPtr<ossimImageData> ossimTiffTileSource::getTile(
   const  ossimIrect& tile_rect,
   ossim_uint32 resLevel)
{
   static const char MODULE[] ="ossimTiffTileSource::getTile(rect, res)";

   //---
   // Not open, this tile source bypassed, or invalid res level,
   // return a blank tile.
   //---
   if( !isOpen() || !isSourceEnabled() || !isValidRLevel(resLevel) )
   {
      return ossimRefPtr<ossimImageData>();
   }

   //---
   // Subtract an sub image offset to work in zero based image space from
   // here on out.
   //---
   ossimIrect zeroBasedTileRect = tile_rect - getSubImageOffset(resLevel);
   
   //---
   // If there is an overview and the current rr level is not at "0" then
   // send the getTile call to the overview image handler.
   //---
   if (resLevel&&theOverview)
   {
      //---
      // Overviews are not aware of sub image offsets so request a zero based
      // rectangle.
      //---
      ossimRefPtr<ossimImageData> tile =
         theOverview->getTile(zeroBasedTileRect, resLevel);
      if (tile.valid())
      {
         //---
         // Since the overview request was zero based we must reset the
         // rectangle to be relative to any sub image offset.
         //---
         tile->setImageRectangle(tile_rect);
      }

      if(getOutputScalarType() == OSSIM_USHORT11)
      {
         //---
         // Temp fix:
         // The overview handler could return a tile of OSSIM_UINT16 if
         // the max sample value was not set to 2047.
         //---
         tile->setScalarType(OSSIM_USHORT11);
      }

      return tile;
   }

   bool reallocateBuffer = false;   
   if ( (tile_rect.width()  != theTile->getWidth()) ||
        (tile_rect.height() != theTile->getHeight()) )
   {
      reallocateBuffer = true;
   }
   
   if (getCurrentTiffRLevel() != resLevel)
   {
      // Change the tiff directory to the requested level.
      ossim_uint32 dir = 0;
      if (resLevel)
      {
         dir = theR0isFullRes ? resLevel : resLevel - 1;
      }

      if (setTiffDirectory(dir) == false)
      {
         return ossimRefPtr<ossimImageData>();
      }
      reallocateBuffer = true;
   }

   //---
   // The tile's internal rectangle should be relative to any sub image.
   // 
   // Note:
   //
   // Since the allocate buffer method relies on theTile->getHeight() it is
   // important to do this prior to the allocateBuffer call.
   //---
   theTile->setImageRectangle(tile_rect);

   if (reallocateBuffer)
   {
      allocateBuffer();
   }

   // This should be the zero base image rectangle for this res level.
   ossimIrect image_rect = getImageRectangle(resLevel);
        
   //---
   // See if any point of the requested tile is in the image.
   //---
   if ( zeroBasedTileRect.intersects(image_rect) )
   {
      ossimIrect clip_rect = zeroBasedTileRect.clipToRect(image_rect);

      if ( !zeroBasedTileRect.completely_within(clip_rect) )
      {
         // Start with a blank tile.
         theTile->makeBlank();
      }

      // Load the tile buffer with data from the tif.
      if (loadTile(zeroBasedTileRect, clip_rect))
      {
         theTile->validate();
         return theTile;
      }
      else
      {
         // Error in filling buffer.
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " Returning blank tile..." << endl;
         return ossimRefPtr<ossimImageData>();
      }
   }

   // No part of requested tile within the image rectangle.
   return ossimRefPtr<ossimImageData>();
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimTiffTileSource::saveState(ossimKeywordlist& kwl,
                                    const char* prefix) const
{
   kwl.add(prefix,
           "apply_color_palette_flag",
           theApplyColorPaletteFlag,
           true);
   // Currently nothing to do here.
   return ossimImageHandler::saveState(kwl, prefix);
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimTiffTileSource::loadState(const ossimKeywordlist& kwl,
                                    const char* prefix)
{
   if (ossimImageHandler::loadState(kwl, prefix))
   {
      ossimString flag = kwl.find(prefix,
                                  "apply_color_palette_flag");
      if(flag != "")
      {
         theApplyColorPaletteFlag = flag.toBool();
      }
      else
      {
         theApplyColorPaletteFlag = true;
      }
      
     return open(theImageFile);
   }

   return false;
}

bool ossimTiffTileSource::open(const ossimFilename& image_file)
{
   if (theTiffPtr)
   {
     close();
   }
   theImageFile = image_file;
   return open();
}

void ossimTiffTileSource::close()
{
   if(theTiffPtr)
   {
      TIFFClose(theTiffPtr);
      theTiffPtr = 0;
   }
   theImageWidth.clear();
   theImageLength.clear();
   theReadMethod.clear();
   thePlanarConfig.clear();
   thePhotometric.clear();
   theRowsPerStrip.clear();
   theImageTileWidth.clear();
   theImageTileLength.clear();
   if (theBuffer)
   {
      delete [] theBuffer;
      theBuffer = NULL;
   }
}

bool ossimTiffTileSource::open()
{
   static const char MODULE[] = "ossimTiffTileSource::open";

   if(isOpen())
   {
     close();
   }
   //***
   // First we do a quick test to see if the file looks like a tiff file.
   //***
   FILE		*fp;
   unsigned char header[2];

   fp = fopen( theImageFile.c_str(), "rb" );
   if( fp == NULL )
       return false;

   fread( header, 2, 1, fp );
   fclose( fp );

   if( (header[0] != 'M' || header[1] != 'M')
       && (header[0] != 'I' || header[1] != 'I') )
       return false;

   //---
   // Note:  The 'm' in "rm" is to tell TIFFOpen to not memory map the file.
   //---
   theTiffPtr = TIFFOpen(theImageFile.c_str(), "rm");

   // Check for errors.
   if (!theTiffPtr)
   {
      return false;
   }

   if (!theTiffPtr)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:\n"
         << "Object has null tiff descriptor." << endl;
      return false;
   }

   //***
   // Get the general tiff info.
   //***
   
   //***
   // See if the first directory is of FILETYPE_REDUCEDIMAGE; if not,
   // the first level is considered to be full resolution data.
   // Note:  If the tag is not present, consider the first level full
   // resolution.
   //***
   long sub_file_type;
   if ( !TIFFGetField( theTiffPtr,
                       TIFFTAG_SUBFILETYPE ,
                       &sub_file_type ) )
   {
      sub_file_type = 0;
   }

   if (sub_file_type == FILETYPE_REDUCEDIMAGE)
   {
      theR0isFullRes = false;
   }
   else
   {
      theR0isFullRes = true;
   }
   
   if( !TIFFGetField(theTiffPtr, TIFFTAG_BITSPERSAMPLE, &(theBitsPerSample)) )
   {
      theBitsPerSample = 8;
   }

   if( !TIFFGetField(theTiffPtr,
                     TIFFTAG_SAMPLESPERPIXEL,
                     &theSamplesPerPixel ) )
   {
      theSamplesPerPixel = 1; 
   }

   if ( !TIFFGetField( theTiffPtr,
                       TIFFTAG_SAMPLEFORMAT,
                       &theSampleFormatUnit ) )
   {
      theSampleFormatUnit = 0;
   }

   if ( !TIFFGetField( theTiffPtr,
                       TIFFTAG_SMAXSAMPLEVALUE,
                       &theMaxSampleValue ) )
   {
      uint16 maxValue = 0;
      if(!TIFFGetField( theTiffPtr,
                        TIFFTAG_MAXSAMPLEVALUE,
                        &maxValue))
      {
         theMaxSampleValue = OSSIM_DBL_NAN;
      }
      else
      {
         theMaxSampleValue = maxValue;
      }
   }

   if ( !TIFFGetField( theTiffPtr,
                       TIFFTAG_SMINSAMPLEVALUE,
                       &theMinSampleValue ) )
   {
      uint16 minValue = 0;
      if(!TIFFGetField( theTiffPtr,
                        TIFFTAG_MINSAMPLEVALUE,
                        &minValue))
      {
         theMinSampleValue = OSSIM_DBL_NAN;
      }
      else
      {
         theMinSampleValue = minValue;
      }
   }

   if (traceDebug())
   {
      CLOG << "DEBUG:"
           << "\ntheMinSampleValue:  " << theMinSampleValue
           << "\ntheMaxSampleValue:  " << theMaxSampleValue
           << endl;
   }

   //***
   // Get the number of directories.
   //***
   theNumberOfDirectories = 0;
   do
   {
      theNumberOfDirectories++;
   } while (TIFFReadDirectory(theTiffPtr));

   theImageWidth.resize(theNumberOfDirectories);
   theImageLength.resize(theNumberOfDirectories);
   theReadMethod.resize(theNumberOfDirectories);
   thePlanarConfig.resize(theNumberOfDirectories);
   thePhotometric.resize(theNumberOfDirectories);
   theRowsPerStrip.resize(theNumberOfDirectories);
   theImageTileWidth.resize(theNumberOfDirectories);
   theImageTileLength.resize(theNumberOfDirectories);
   
   for (ossim_uint32 dir=0; dir<theNumberOfDirectories; ++dir)
   {
      if (setTiffDirectory(dir) == false)
      {
         return false;
      }
      
      if( !TIFFGetField( theTiffPtr, TIFFTAG_PLANARCONFIG,
                         &(thePlanarConfig[dir]) ) )
      {
         thePlanarConfig[dir] = PLANARCONFIG_CONTIG;
      }
      
      if( !TIFFGetField( theTiffPtr, TIFFTAG_PHOTOMETRIC,
                         &(thePhotometric[dir]) ) )
      {
         thePhotometric[dir] = PHOTOMETRIC_MINISBLACK;
      }
      theLut = 0;
      // Check for palette.
      uint16* red;
      uint16* green;
      uint16* blue;
      if(TIFFGetField(theTiffPtr, TIFFTAG_COLORMAP, &red, &green, &blue))
      {
         if(theApplyColorPaletteFlag)
         {
            thePhotometric[dir] = PHOTOMETRIC_PALETTE;
            theSamplesPerPixel = 3;
         }
         populateLut();
      }

      if( TIFFIsTiled(theTiffPtr))
      {
         theRowsPerStrip[dir] = 0;
         if ( !TIFFGetField( theTiffPtr,
                             TIFFTAG_TILEWIDTH,
                             &theImageTileWidth[dir] ) )
         {
            theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimTiffTileSource::getTiffTileWidth ERROR:"
               << "\nCannot determine tile width." << endl;
            theImageTileWidth[dir] = 0;
         }
         if ( !TIFFGetField( theTiffPtr,
                             TIFFTAG_TILELENGTH,
                             &theImageTileLength[dir] ) )
         {
            theErrorStatus = ossimErrorCodes::OSSIM_ERROR;   
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimTiffTileSource::getTiffTileLength ERROR:"
               << "\nCannot determine tile length." << endl;
             theImageTileLength[dir] = 0;
         }
      }
      else
      {
         theImageTileWidth[dir]  = 0;
         theImageTileLength[dir] = 0;
         if( !TIFFGetField( theTiffPtr, TIFFTAG_ROWSPERSTRIP,
                            &(theRowsPerStrip[dir]) ) )
         {
            theRowsPerStrip[dir] = 1;
         }
      }
      
      if ( !TIFFGetField( theTiffPtr,
                          TIFFTAG_IMAGELENGTH,
                          &theImageLength[dir] ) )
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " Cannot determine image length."
            << endl;
      }

      if ( !TIFFGetField( theTiffPtr,
                          TIFFTAG_IMAGEWIDTH,
                          &theImageWidth[dir] ) )
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " Cannot determine image width."
            << endl;
      }
      
   }  // End of "for (ossim_uint32 dir=0; dir<theNumberOfDirectories; dir++)"

   // Reset the directory back to "0".
   if (setTiffDirectory(0) == false)
   {
      return false;
   }

   //---
   // Get the scalar type.
   //---
   theScalarType = OSSIM_SCALAR_UNKNOWN;
   
   if (theBitsPerSample == 16)
   {
      theBytesPerPixel = 2;
      theScalarType = OSSIM_USHORT16; // Default to unsigned...
      
      if (theMaxSampleValue == 2047) // 2^11-1
      {
         theScalarType = OSSIM_USHORT11; // IKONOS probably...
      }
      else if (theSampleFormatUnit == SAMPLEFORMAT_INT)
      {
         theScalarType = OSSIM_SSHORT16;
      }
   }
   else if ( (theBitsPerSample == 32) &&
             (theSampleFormatUnit == SAMPLEFORMAT_UINT) )
   {
      theBytesPerPixel = 4;
      theScalarType = OSSIM_UINT32;
   }
   else if (theBitsPerSample == 32 &&
            theSampleFormatUnit == SAMPLEFORMAT_IEEEFP)
   {
      theBytesPerPixel = 4;
      theScalarType = OSSIM_FLOAT;
   }
   else if(theBitsPerSample == 64 &&
	   theSampleFormatUnit == SAMPLEFORMAT_IEEEFP)
     {
       theBytesPerPixel = 8;
       theScalarType = OSSIM_DOUBLE;
     }
   else if (theBitsPerSample <= 8)
   {
      theBytesPerPixel = 1;
      theScalarType = OSSIM_UCHAR;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Error:\nCannot determine scalar type.\n"
         << "Trace dump follows:\n";
      print(ossimNotify(ossimNotifyLevel_WARN));
      
      return false;
   }
   double tempMax = ossimGetDefaultMax(theScalarType);
   double tempMin = ossimGetDefaultMin(theScalarType);

   if(theMinSampleValue == OSSIM_DBL_NAN)
   {
      theMinSampleValue = tempMin;
   }
   if(theMaxSampleValue == OSSIM_DBL_NAN)
   {
      theMaxSampleValue = tempMax;
   }
   
   setReadMethod();
   
   // Check for an overview file...
   if (theOverview == NULL)
   {
      openOverview();
   }

   openValidVertices();
   loadMetaData();

   initializeBuffers();

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " Debug:";
      print(ossimNotify(ossimNotifyLevel_DEBUG));
   }
   
   // Finished...
   return true;
}
   
ossim_uint32 ossimTiffTileSource::getNumberOfLines(ossim_uint32 reduced_res_level) const
{
   if(!theTiffPtr) return OSSIM_ULONG_NAN;
   
   if (isValidRLevel(reduced_res_level))
   {
      if (reduced_res_level && theOverview)
      {
         return theOverview->getNumberOfLines(reduced_res_level);
      }
      
      return (theR0isFullRes ?
              theImageLength[reduced_res_level] :
              theImageLength[reduced_res_level - 1]);
   }

   return OSSIM_ULONG_NAN;
}

ossim_uint32
ossimTiffTileSource::getNumberOfSamples(ossim_uint32 reduced_res_level) const
{
   if(!theTiffPtr) return OSSIM_ULONG_NAN;
   if (isValidRLevel(reduced_res_level))
   {
      if (reduced_res_level && theOverview)
      {
         return theOverview->getNumberOfSamples(reduced_res_level);
      }
      
      return (theR0isFullRes ?
              theImageWidth[reduced_res_level] :
              theImageWidth[reduced_res_level-1]);
   }
   
   return OSSIM_ULONG_NAN;
}

ossim_uint32 ossimTiffTileSource::getNumberOfDecimationLevels() const
{
   if (theOverview)
   {
      return theOverview->getNumberOfDecimationLevels();
   }
   
   return ( theR0isFullRes ? theNumberOfDirectories :
            theNumberOfDirectories+1 );
}

//*******************************************************************
// Public method:
//*******************************************************************
ossimScalarType ossimTiffTileSource::getOutputScalarType() const
{
   return theScalarType;
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimTiffTileSource::getTileWidth() const
{
   return ( theTile.valid() ? theTile->getWidth() : 0 );
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimTiffTileSource::getTileHeight() const
{
   return ( theTile.valid() ? theTile->getHeight() : 0 );
}

bool ossimTiffTileSource::loadTile(const ossimIrect& tile_rect,
                                   const ossimIrect& clip_rect)
{
   static const char MODULE[] = "ossimTiffTileSource::loadTile";

   bool status = false;
   
   switch(theReadMethod[theCurrentDirectory])
   {
      case READ_TILE:
         status = loadFromTile(clip_rect);
         break;
         
      case READ_SCAN_LINE:
         status = loadFromScanLine(clip_rect);
         break;
         
      case READ_RGBA_U8_TILE:
         status = loadFromRgbaU8Tile(tile_rect, clip_rect);
         break;
         
      case READ_RGBA_U8_STRIP:
         status = loadFromRgbaU8Strip(tile_rect, clip_rect);
         break;
         
      case READ_RGBA_U8A_STRIP:
         status = loadFromRgbaU8aStrip(tile_rect, clip_rect);
         break;
         
      default:
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " Unsupported tiff type!" << endl;
         status = false;
         break;
   }
   
   return status;
}

bool ossimTiffTileSource::loadFromScanLine(const ossimIrect& clip_rect)
{
   ossimInterleaveType type =
      (thePlanarConfig[theCurrentDirectory] == PLANARCONFIG_CONTIG) ?
       OSSIM_BIP : OSSIM_BIL;
   
   if ( theBufferRLevel != getCurrentTiffRLevel() ||
        !clip_rect.completely_within(theBufferRect) )
   {
      //***
      // Must reload the buffer.  Grab enough lines to fill the depth of the
      // clip rectangle.
      //***
      theBufferRLevel = getCurrentTiffRLevel();
      theBufferRect   = getImageRectangle(theBufferRLevel);
      theBufferRect.set_uly(clip_rect.ul().y);
      theBufferRect.set_lry(clip_rect.lr().y);
      ossim_uint32 startLine = clip_rect.ul().y;
      ossim_uint32 stopLine  = clip_rect.lr().y;
      ossim_uint8* buf = theBuffer;

      if (thePlanarConfig[theCurrentDirectory] == PLANARCONFIG_CONTIG)
      {
         ossim_uint32 lineSizeInBytes = getNumberOfSamples(theBufferRLevel) *
                                  theBytesPerPixel * theSamplesPerPixel;

         for (ossim_uint32 line = startLine; line <= stopLine; ++line)
         {
            TIFFReadScanline(theTiffPtr, (void*)buf, line, 0);
            buf += lineSizeInBytes;
         }
      }
      else
      {
         ossim_uint32 lineSizeInBytes = getNumberOfSamples(theBufferRLevel) *
                                  theBytesPerPixel;

         for (ossim_uint32 line = startLine; line <= stopLine; ++line)
         {
            for (ossim_uint32 band = 0; band < theSamplesPerPixel; ++band)
            {
               TIFFReadScanline(theTiffPtr, (void*)buf, line, band);
               buf += lineSizeInBytes;
            }
         }
      }
   }

   theTile->loadTile(theBuffer,
                     theBufferRect,
                     clip_rect,
                     type);
   return true;
}

bool ossimTiffTileSource::loadFromTile(const ossimIrect& clip_rect)
{
   static const char MODULE[] = "ossimTiffTileSource::loadFromTile";
   
   //***
   // Shift the upper left corner of the "clip_rect" to the an even tile
   // boundary.  Note this will shift in the upper left direction.
   //***
   ossimIpt tileOrigin = clip_rect.ul();
   adjustToStartOfTile(tileOrigin);
   
   //---
   // Calculate the number of tiles needed in the line/sample directions.
   //---

        
   ossim_uint32 tiles_in_v_dir = (clip_rect.lr().x-tileOrigin.x+1) /
      theImageTileWidth[theCurrentDirectory];
   ossim_uint32 tiles_in_u_dir = (clip_rect.lr().y-tileOrigin.y+1) /
      theImageTileLength[theCurrentDirectory];

   if ( (clip_rect.lr().x-tileOrigin.x+1) %
        theImageTileWidth[theCurrentDirectory]  ) ++tiles_in_v_dir;
   if ( (clip_rect.lr().y-tileOrigin.y+1) %
        theImageTileLength[theCurrentDirectory] ) ++tiles_in_u_dir;

   ossimInterleaveType type =
   (thePlanarConfig[theCurrentDirectory] == PLANARCONFIG_CONTIG) ?
   OSSIM_BIP : OSSIM_BIL;

   ossimIpt ulTilePt = tileOrigin;
   
   // Tile loop in line direction.
   for (ossim_uint32 u=0; u<tiles_in_u_dir; ++u)
   {
      ulTilePt.x = tileOrigin.x;

      // Tile loop in sample direction.
      for (ossim_uint32 v=0; v<tiles_in_v_dir; ++v)
      {
         ossimIrect tiff_tile_rect(ulTilePt.x,
                                   ulTilePt.y,
                                   ulTilePt.x +
                                   theImageTileWidth[theCurrentDirectory]  - 1,
                                   ulTilePt.y +
                                   theImageTileLength[theCurrentDirectory] - 1);
         
         if (tiff_tile_rect.intersects(clip_rect))
         {
            ossimIrect tiff_tile_clip_rect
               = tiff_tile_rect.clipToRect(clip_rect);
            
            if  (thePlanarConfig[theCurrentDirectory] == PLANARCONFIG_CONTIG)
            {
               if (TIFFReadTile(theTiffPtr,
                                theBuffer,
                                ulTilePt.x,
                                ulTilePt.y,
                                0,
                                0) == -1)
               {
                  ossimNotify(ossimNotifyLevel_WARN)
                     << MODULE << " Read Error!"
                     << "\nReturning error...  " << endl;
                  theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
                  return false;
               }

               theTile->loadTile(theBuffer,
                                 tiff_tile_rect,
                                 tiff_tile_clip_rect,
                                 type);
            }
            else
            {
               // band separate tiles...
               for (ossim_uint32 band=0; band<theSamplesPerPixel; ++band)
               {
                  // Read a tile into the buffer.
                  if (TIFFReadTile(theTiffPtr,
                                   theBuffer,
                                   ulTilePt.x,
                                   ulTilePt.y,
                                   0,
                                   band) == -1)
                  {
                     ossimNotify(ossimNotifyLevel_WARN)
                        << MODULE << " Read Error!"
                        << "\nReturning error...  " << endl;
                     theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
                     return false;
                  }
                  
                  theTile->loadBand(theBuffer,
                                    tiff_tile_rect,
                                    tiff_tile_clip_rect,
                                    band);
               }
            }

         } // End of if (tiff_tile_rect.intersects(clip_rect))
         
         ulTilePt.x += theImageTileWidth[theCurrentDirectory];
         
      }  // End of tile loop in the sample direction.

      ulTilePt.y += theImageTileLength[theCurrentDirectory];
      
   }  // End of tile loop in the line direction.

   return true;
}

bool ossimTiffTileSource::loadFromRgbaU8Tile(const ossimIrect& tile_rect,
                                             const ossimIrect& clip_rect)
{
   static const char MODULE[] = "ossimTiffTileSource::loadFromRgbaTile";

   if (theSamplesPerPixel != 3 || theBytesPerPixel!=1)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Error:"
         << "\nInvalid number of bands or bytes per pixel!" << endl;
   }
   
   //***
   // Shift the upper left corner of the "clip_rect" to the an even tile
   // boundary.  Note this will shift in the upper left direction.
   //***
   ossimIpt tileOrigin = clip_rect.ul();
   adjustToStartOfTile(tileOrigin);

   //---
   // Calculate the number of tiles needed in the line/sample directions
   // to fill the tile.
   //---
   ossim_uint32 tiles_in_v_dir = (clip_rect.lr().x-tileOrigin.x+1) /
      theImageTileWidth[theCurrentDirectory];
   ossim_uint32 tiles_in_u_dir = (clip_rect.lr().y-tileOrigin.y+1) /
      theImageTileLength[theCurrentDirectory];

   if ( (clip_rect.lr().x-tileOrigin.x+1) %
        theImageTileWidth[theCurrentDirectory]  ) ++tiles_in_v_dir;
   if ( (clip_rect.lr().y-tileOrigin.y+1) %
        theImageTileLength[theCurrentDirectory] ) ++tiles_in_u_dir;
   
   ossimIpt ulTilePt = tileOrigin;

#if 0
   if (traceDebug())
   {
      CLOG << "DEBUG:"
           << "\ntile_rect:  " << tile_rect
           << "\nclip_rect:  " << clip_rect
           << "\ntiles_in_v_dir:  " << tiles_in_v_dir
           << "\ntiles_in_u_dir:  " << tiles_in_u_dir
           << endl;
   }
#endif
   
   
   // Tile loop in line direction.
   for (ossim_uint32 u=0; u<tiles_in_u_dir; u++)
   {
      ulTilePt.x = tileOrigin.x;

      // Tile loop in sample direction.
      for (ossim_uint32 v=0; v<tiles_in_v_dir; v++)
      {
         ossimIrect tiff_tile_rect
            = ossimIrect(ulTilePt.x,
                         ulTilePt.y,
                         ulTilePt.x +
                         theImageTileWidth[theCurrentDirectory]  - 1,
                         ulTilePt.y +
                         theImageTileLength[theCurrentDirectory] - 1);
         
         if ( getCurrentTiffRLevel() != theBufferRLevel ||
              tiff_tile_rect     != theBufferRect)
         {
            // Need to grab a new tile.
            // Read a tile into the buffer.
            if ( !TIFFReadRGBATile(theTiffPtr,
                                   ulTilePt.x,
                                   ulTilePt.y,
                                   (uint32*)theBuffer) ) // use tiff typedef
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << MODULE << " Read Error!"
                  << "\nReturning error..." << endl;
               theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
               return false;
            }

            // Capture the rectangle.
            theBufferRect   = tiff_tile_rect;
            theBufferRLevel = getCurrentTiffRLevel();
         }

         ossimIrect tile_clip_rect = clip_rect.clipToRect(theBufferRect);
         
         //***
         // Get the offset to the first valid pixel.
         // 
         // Note: The data in the tile buffer is organized bottom up.  So the
         //       coordinate must be negated in the line direction since
         //       the met assumes an origin of upper left.
         //***
         ossim_uint32 in_buf_offset =
              (tiff_tile_rect.lr().y-tile_clip_rect.ul().y)*
              theImageTileWidth[theCurrentDirectory]*4 +
              ((tile_clip_rect.ul().x - ulTilePt.x)*4);
         
         ossim_uint32 out_buf_offset =
            (tile_clip_rect.ul().y - tile_rect.ul().y) *
            ((ossim_int32)theTile->getWidth()) +
            tile_clip_rect.ul().x - tile_rect.ul().x;
         
         //
         // Get a pointer positioned at the first valid pixel in buffers.
         //
         ossim_uint32* s = (ossim_uint32*)(theBuffer + in_buf_offset);  // s for source...
//         ossim_uint8* s = theBuffer + in_buf_offset;  // s for source...
         ossim_uint8* r = static_cast<ossim_uint8*>(theTile->getBuf(0))+
            out_buf_offset;
         ossim_uint8* g = static_cast<ossim_uint8*>(theTile->getBuf(1))+
            out_buf_offset;
         ossim_uint8* b = static_cast<ossim_uint8*>(theTile->getBuf(2))+
            out_buf_offset;
         
         ossim_uint32 lines2copy = tile_clip_rect.lr().y-tile_clip_rect.ul().y+1;
         ossim_uint32 samps2copy = tile_clip_rect.lr().x-tile_clip_rect.ul().x+1;
         
         // Line loop through valid portion of the tiff tile.         
         for (ossim_uint32 line = 0; line < lines2copy; line++)
         {
            // Sample loop through the tiff tile.
            ossim_uint32 i=0;
            ossim_uint32 j=0;
            
            // note the bands from the TIFF READ are stored in a, b, g, r ordering.
            // we must reverse the bands and skip the first byte.
            for (ossim_uint32 sample = 0; sample < samps2copy; sample++)
            {
               r[i] = (ossim_uint8)OSSIM_TIFF_UNPACK_R4(s[j]);
               g[i] = (ossim_uint8)OSSIM_TIFF_UNPACK_G4(s[j]);
               b[i] = (ossim_uint8)OSSIM_TIFF_UNPACK_B4(s[j]);
               i++;
               ++j;
            }
            
            // Increment the pointers by one line.
            const ossim_uint32 OUTPUT_TILE_WIDTH = theTile->getWidth();
            r += OUTPUT_TILE_WIDTH;
            g += OUTPUT_TILE_WIDTH;
            b += OUTPUT_TILE_WIDTH;
            s -= theImageTileWidth[theCurrentDirectory];
         }
      
         ulTilePt.x += theImageTileWidth[theCurrentDirectory];
      
      }  // End of tile loop in the sample direction.
      
      ulTilePt.y += theImageTileLength[theCurrentDirectory];
      
   }  // End of tile loop in the line direction.
   
   return true;
}

bool ossimTiffTileSource::loadFromRgbaU8Strip(const ossimIrect& tile_rect,
                                              const ossimIrect& clip_rect)
{
   static const char MODULE[] = "ossimTiffTileSource::loadFromRgbaU8Strip";

   if (theSamplesPerPixel > 4 || theBytesPerPixel != 1)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Error:"
         << "\nInvalid number of bands or bytes per pixel!" << endl;
   }
   
   //***
   // Calculate the number of strips to read.
   //***
   const ossim_uint32 OUTPUT_TILE_WIDTH = theTile->getWidth();

   ossim_uint32 starting_strip = clip_rect.ul().y /
      theRowsPerStrip[theCurrentDirectory];
   ossim_uint32 ending_strip   = clip_rect.lr().y /
      theRowsPerStrip[theCurrentDirectory];
   ossim_uint32 strip_width    = theImageWidth[theCurrentDirectory]*4;   
   ossim_uint32 output_tile_offset = (clip_rect.ul().y - tile_rect.ul().y) *
                                OUTPUT_TILE_WIDTH + clip_rect.ul().x -
                                tile_rect.ul().x;

#if 0
   if (traceDebug())
   {
      CLOG << "DEBUG:"
           << "\nsamples:         " << theSamplesPerPixel
           << "\ntile_rect:       " << tile_rect
           << "\nclip_rect:       " << clip_rect
           << "\nstarting_strip:  " << starting_strip
           << "\nending_strip:    " << ending_strip
           << "\nstrip_width:     " << strip_width
           << "\noutput_tile_offset:  " << output_tile_offset

           << endl;
   }
#endif
   
   //***
   // Get the pointers positioned at the first valid pixel in the buffers.
   // s = source
   // d = destination
   //***
   ossim_uint32 band;

   ossim_uint8** d = new ossim_uint8*[theSamplesPerPixel];
   for (band = 0; band < theSamplesPerPixel; band++)
   {
      d[band] = static_cast<ossim_uint8*>(theTile->getBuf(band))+output_tile_offset;
   }

   // Loop through strips...
   for (ossim_uint32 strip=starting_strip; strip<=ending_strip; strip++)
   {
      if (TIFFReadRGBAStrip(theTiffPtr,
                            (strip*theRowsPerStrip[theCurrentDirectory]),
                            (uint32*)theBuffer) == 0) // use tiff typedef
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " Error reading strip!" <<endl;
         delete [] d;
         return false;
      }

      //***
      // If the last strip is a partial strip then the first line of the
      // strip will be the last line of the image.
      //***
      ossim_uint32 last_line = theImageLength[theCurrentDirectory] - 1;

      ossim_uint32 strip_offset
         = ((strip * theRowsPerStrip[theCurrentDirectory]) +
            theRowsPerStrip[theCurrentDirectory] - 1) <
         last_line ?  0 :
         ((strip * theRowsPerStrip[theCurrentDirectory]) +
          theRowsPerStrip[theCurrentDirectory] - 1) - last_line;

      ossim_uint32 total_rows = theRowsPerStrip[theCurrentDirectory] -
         strip_offset;
      
      for (ossim_uint32 row=0; row<total_rows; row++)
      {
         // Write the line if it's in the clip rectangle.
         ossim_int32 current_line = strip * theRowsPerStrip[theCurrentDirectory]
            + row;
         if  (current_line >= clip_rect.ul().y &&
              current_line <= clip_rect.lr().y)
         {
            //
            // Position the stip pointer to the correct spot.
            // 
            // Note:
            // A strip is organized from bottom to top and the raster buffer is
            // orgainized from top to bottom so the lineBuf must be offset
            // accordingly.
            //
               ossim_uint32* s = (ossim_uint32*)(theBuffer+ ((theRowsPerStrip[theCurrentDirectory] - row -
                                                              strip_offset - 1) * strip_width + clip_rect.ul().x * 4));
            
            // Copy the data to the output buffer.
            ossim_uint32 i=0;
                                                 
            for (int32 sample=clip_rect.ul().x;
                 sample<=clip_rect.lr().x;
                 sample++)
            {
               d[0][i] = OSSIM_TIFF_UNPACK_R4(*s);
               d[1][i] = OSSIM_TIFF_UNPACK_G4(*s);
               d[2][i] = OSSIM_TIFF_UNPACK_B4(*s);
               ++i;
               ++s;
            }

            for (band = 0; band < theSamplesPerPixel; band++)
            {
               d[band] += OUTPUT_TILE_WIDTH;
            }
         }
      }  // End of loop through rows in a strip.

   }  // End of loop through strips.

   delete [] d;
               
   return true;
}

//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimTiffTileSource::loadFromRgbaU8aStrip(const ossimIrect& tile_rect,
                                               const ossimIrect& clip_rect)
{
   static const char MODULE[] = "ossimTiffTileSource::loadFromRgbaU8aStrip";

   //***
   // Specialized for one bit data to handle null values.
   //***
   const ossim_uint32 OUTPUT_TILE_WIDTH = theTile->getWidth();
   const ossim_uint8 NULL_PIX = static_cast<ossim_uint8>(theTile->getNullPix(0));
   const ossim_uint8 MIN_PIX  = static_cast<ossim_uint8>(theTile->getMinPix(0));

   if (theSamplesPerPixel > 4 || theBytesPerPixel!= 1)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Error:"
         << "\nInvalid number of bands or bytes per pixel!" << endl;
   }
   
   //***
   // Calculate the number of strips to read.
   //***
   ossim_uint32 starting_strip = clip_rect.ul().y /
        theRowsPerStrip[theCurrentDirectory];
   ossim_uint32 ending_strip   = clip_rect.lr().y /
      theRowsPerStrip[theCurrentDirectory];
   ossim_uint32 output_tile_offset = (clip_rect.ul().y - tile_rect.ul().y) *
                                OUTPUT_TILE_WIDTH + clip_rect.ul().x -
                                tile_rect.ul().x;

#if 0
   if (traceDebug())
   {
      CLOG << "DEBUG:"
           << "\ntile_rect:       " << tile_rect
           << "\nclip_rect:       " << clip_rect
           << "\nstarting_strip:  " << starting_strip
           << "\nending_strip:    " << ending_strip
           << "\nstrip_width:     " << strip_width
           << "\noutput_tile_offset:     " << output_tile_offset
           << "\nsamples:         " << theSamplesPerPixel
           << endl;
   }
#endif
   
   //***
   // Get the pointers positioned at the first valid pixel in the buffers.
   // s = source
   // d = destination
   //***
   ossim_uint32 band;

   ossim_uint8** d = new ossim_uint8*[theSamplesPerPixel];
   for (band = 0; band < theSamplesPerPixel; band++)
   {
      d[band] = static_cast<ossim_uint8*>(theTile->getBuf(band))+output_tile_offset;
   }

   // Loop through strips...
   for (ossim_uint32 strip=starting_strip; strip<=ending_strip; strip++)
   {
      if (TIFFReadRGBAStrip(theTiffPtr,
                            (strip*theRowsPerStrip[theCurrentDirectory]),
                            (uint32*)theBuffer) == 0) // use tiff typedef
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " Error reading strip!" <<endl;
         delete [] d;
         return false;
      }

      //***
      // If the last strip is a partial strip then the first line of the
      // strip will be the last line of the image.
      //***
      ossim_uint32 last_line = theImageLength[theCurrentDirectory] - 1;

      ossim_uint32 strip_offset
         = ((strip * theRowsPerStrip[theCurrentDirectory]) +
            theRowsPerStrip[theCurrentDirectory] - 1) < last_line ?  0 :
         ((strip * theRowsPerStrip[theCurrentDirectory]) +
          theRowsPerStrip[theCurrentDirectory] - 1) - last_line;

      ossim_uint32 total_rows = theRowsPerStrip[theCurrentDirectory] -
         strip_offset;
      
      for (ossim_uint32 row=0; row<total_rows; row++)
      {
         // Write the line if it's in the clip rectangle.
         ossim_int32 current_line = strip * theRowsPerStrip[theCurrentDirectory]
            + row;
         if  (current_line >= clip_rect.ul().y &&
              current_line <= clip_rect.lr().y)
         {
            //***
            // Position the stip pointer to the correct spot.
            // 
            // Note:
            // A strip is organized from bottom to top and the raster buffer is
            // orgainized from top to bottom so the lineBuf must be offset
            // accordingly.
            //***
            ossim_uint8* s = theBuffer;
            s += (theRowsPerStrip[theCurrentDirectory] - row -
                  strip_offset - 1) *
                 theImageWidth[theCurrentDirectory] * 4 +
                 clip_rect.ul().x * 4;
            
            // Copy the data to the output buffer.
            ossim_uint32 i=0;
            ossim_uint32 j=0;
            for (int32 sample=clip_rect.ul().x;
                 sample<=clip_rect.lr().x;
                 sample++)
            {
               for (band = 0; band < theSamplesPerPixel; band++)
               {
                  ossim_uint8 pix = s[j + band];
                  d[band][i] = pix != NULL_PIX ? pix : MIN_PIX;
               }
               ++i;
               j += 4;
            }

            for (band = 0; band < theSamplesPerPixel; band++)
            {
               d[band] += OUTPUT_TILE_WIDTH;
            }
         }
      }  // End of loop through rows in a strip.

   }  // End of loop through strips.

   delete [] d;
               
   return true;
}

void ossimTiffTileSource::adjustToStartOfTile(ossimIpt& pt) const
{
   //***
   // Notes:
   // - Assumes an origin of (0,0)
   // - Shifts in to the upper left direction.
   //***
   ossim_int32 tw =
      static_cast<ossim_int32>(theImageTileWidth[theCurrentDirectory]);
   ossim_int32 th =
      static_cast<ossim_int32>(theImageTileLength[theCurrentDirectory]);
   
   if (pt.x > 0)
   {
      pt.x = (pt.x/tw) * tw;
   }
   else if (pt.x < 0)
   {
      pt.x = ossimAbs(pt.x) < tw ? -tw : (pt.x/tw)*tw;
   }

   if (pt.y > 0)
   {
      pt.y = (pt.y/th) * th;
   }
   else if (pt.y < 0)
   {
      pt.y = ossimAbs(pt.y) < th ? -th : (pt.y/th)*th;
   }
}

bool ossimTiffTileSource::isValidRLevel(ossim_uint32 reduced_res_level) const
{
   if ( ( reduced_res_level > getHighestReducedResSet()  ) ||
        ( !theR0isFullRes && (reduced_res_level == 0) ) )
   {
#if 0
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimTiffTileSource::isValidRLevel Invalid reduced_res_level:  "
         << reduced_res_level
         << "\nHighest available:  " << getHighestReducedResSet()
         << "\nR0 is "
         << (theR0isFullRes ? "valid level." : "not a valid level.") 
         << endl;
#endif
      return false;
   }

   return true;
}

ossim_uint32 ossimTiffTileSource::getHighestReducedResSet() const
{
   if (theOverview)
   {
      return theOverview->getHighestReducedResSet();
   }
   
   return (theR0isFullRes ?
           theNumberOfDirectories-1 :
           theNumberOfDirectories);
}

ossim_uint32 ossimTiffTileSource::getCurrentTiffRLevel() const
{
   return (theR0isFullRes ? theCurrentDirectory : theCurrentDirectory + 1);
}

ossimString ossimTiffTileSource::getReadMethod(ossim_uint32 directory) const
{
   switch (theReadMethod[directory])
   {
      case READ_RGBA_U8_TILE:
         return ossimString("READ_RGBA_U8_TILE");
      case READ_RGBA_U8_STRIP:
         return ossimString("READ_RGBA_U8_STRIP");
      case READ_RGBA_U8A_STRIP:
         return ossimString("READ_RGBA_U8A_STRIP");
      case READ_SCAN_LINE:
         return ossimString("READ_SCAN_LINE");
      case READ_TILE:
         return ossimString("READ_TILE");
      case UNKNOWN:
      default:
         return ossimString("UNKNOWN");
   }
}      

bool ossimTiffTileSource::allocateBuffer()
{
   //***
   // Allocate memory for a buffer to hold data grabbed from the tiff file.
   //***
   ossim_uint32 buffer_size=0;
   switch (theReadMethod[theCurrentDirectory])
   {
      case READ_RGBA_U8_TILE:
         buffer_size = theImageTileWidth[theCurrentDirectory]*
            theImageTileWidth[theCurrentDirectory]*theBytesPerPixel*4;
         break;
         
      case READ_TILE:
         if (thePlanarConfig[theCurrentDirectory] == PLANARCONFIG_CONTIG)
         {
            buffer_size = theImageTileWidth[theCurrentDirectory] *
               theImageTileLength[theCurrentDirectory] *
               theBytesPerPixel * theSamplesPerPixel;
         }
         else
         {
            buffer_size = theImageTileWidth[theCurrentDirectory] *
               theImageTileLength[theCurrentDirectory] *
               theBytesPerPixel;
         }
         break;
         
      case READ_RGBA_U8_STRIP:
      case READ_RGBA_U8A_STRIP:
         buffer_size = theImageWidth[0]*theRowsPerStrip[theCurrentDirectory]*
            theBytesPerPixel*4;
         break;
         
      case READ_SCAN_LINE:
         buffer_size = theImageWidth[0] * theBytesPerPixel *
            theSamplesPerPixel * theTile->getHeight();
         break;
         
      default:
         ossimNotify(ossimNotifyLevel_WARN)
            << "Unknown read method!" << endl;
         print(ossimNotify(ossimNotifyLevel_WARN));
         return false;
         break;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimTiffTileSource::allocateBuffer DEBUG:"
         << "\nbuffer_size:  " << buffer_size
         << endl;
   }

   theBufferRect.makeNan();
   theBufferRLevel = theCurrentDirectory;

   if (buffer_size != theBufferSize)
   {
      theBufferSize = buffer_size;
      if (theBuffer)
      {
         delete [] theBuffer;
      }
      theBuffer = new ossim_uint8[buffer_size];
   }

   return true;
}

ossim_uint32 ossimTiffTileSource::getNumberOfDirectories() const
{
   return theNumberOfDirectories;
}

ossim_uint32 ossimTiffTileSource::getImageTileWidth() const
{
   return theImageTileLength[theCurrentDirectory];
}

ossim_uint32 ossimTiffTileSource::getImageTileHeight() const
{
   return theImageTileLength[theCurrentDirectory];
}

void ossimTiffTileSource::setApplyColorPaletteFlag(bool flag)
{
   theApplyColorPaletteFlag = flag;
   
   if(isColorMapped())
   {
      if(theApplyColorPaletteFlag)
      {
         thePhotometric[0] = PHOTOMETRIC_PALETTE;
         theSamplesPerPixel = 3;
      }
      else
      {
         thePhotometric[0] = PHOTOMETRIC_MINISBLACK;
         theSamplesPerPixel = 1; 
      }
      
      setReadMethod();

      initializeBuffers();
   }
}

bool ossimTiffTileSource::getApplyColorPaletteFlag()const
{
   return theApplyColorPaletteFlag;
}

ossimString ossimTiffTileSource::getLongName()const
{
   return ossimString("TIFF Image Handler");
}

ossimString ossimTiffTileSource::getShortName()const
{
   return ossimString("TIFF Image Handler");
}


std::ostream& ossimTiffTileSource::print(std::ostream& os) const
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
      << "\ntheNumberOfDirectories:      " << theNumberOfDirectories
      << "\nr0_is_full_res:              " << theR0isFullRes;

   
   for (ossim_uint32 i=0; i<theNumberOfDirectories; ++i)
   {
      os << "\ndirectory[" << i << "]"
         << "\nimage width:     " << theImageWidth[i]
         << "\nimage_length:    " << theImageLength[i]
         << "\nread method:     " << getReadMethod(i).c_str()
         << "\nplanar:          " << thePlanarConfig[i]
         << "\nphotometric:     " << thePhotometric[i];
      if (theRowsPerStrip[i])
      {
         os << "\nrows per strip:  " << theRowsPerStrip[i];
      }
      if (theImageTileWidth[i])
      {
         os << "\ntile_width:      " << theImageTileWidth[i];
      }
      if (theImageTileLength[i])
      {
         os << "\ntile_length:     " << theImageTileLength[i];
      }
      os << endl;
   }

   if (theTile.valid())
   {
      os << "\nOutput tile dump:\n" << *theTile << endl;
   }

   if (theOverview)
   {
      os << "\nOverview file:\n";
      theOverview->print(os);
   }

   os << endl;
   
   return ossimSource::print(os);
}

ossim_uint32 ossimTiffTileSource::getNumberOfInputBands() const
{
   return theSamplesPerPixel;
}

ossim_uint32 ossimTiffTileSource::getNumberOfOutputBands () const
{
   return getNumberOfInputBands();
}

bool ossimTiffTileSource::isOpen()const
{
   return (theTiffPtr!=NULL);
}

bool ossimTiffTileSource::hasR0() const
{
   return theR0isFullRes;
}

double ossimTiffTileSource::getMinPixelValue(ossim_uint32 band)const
{
   if(theMetaData.getNumberOfBands())
   {
      return ossimImageHandler::getMinPixelValue(band);
   }
   return theMinSampleValue;
}

double ossimTiffTileSource::getMaxPixelValue(ossim_uint32 band)const
{
   if(theMetaData.getNumberOfBands())
   {
      return ossimImageHandler::getMaxPixelValue(band);
   }
   return theMaxSampleValue;
}

bool ossimTiffTileSource::isColorMapped() const
{
   uint16* red;
   uint16* green;
   uint16* blue;
   
   return static_cast<bool>(TIFFGetField(theTiffPtr,
                                         TIFFTAG_COLORMAP,
                                         &red, &green, &blue));
}

void ossimTiffTileSource::setReadMethod()
{
   for (ossim_uint32 dir=0; dir<theNumberOfDirectories; ++dir)
   {
      if (setTiffDirectory(dir) == false)
      {
         return;
      }
      
      //---
      // Establish how this tiff directory will be read.
      //---
      if (TIFFIsTiled(theTiffPtr)) 
      {
         if ( ( thePhotometric[dir] == PHOTOMETRIC_YCBCR ||
                thePhotometric[dir] == PHOTOMETRIC_PALETTE ) &&
              (theSamplesPerPixel <= 3) &&
              (theBitsPerSample   <= 8 ))
         {
            theReadMethod[dir] = READ_RGBA_U8_TILE;
         }
         else
         {
            theReadMethod[dir] = READ_TILE;
         }
      }
      else // Not tiled...
      {
         if ( (thePhotometric[dir] == PHOTOMETRIC_PALETTE ||
               thePhotometric[dir] == PHOTOMETRIC_YCBCR ) &&
              theSamplesPerPixel <= 3 &&
              theBitsPerSample   <= 8 )
         {
            theReadMethod[dir] = READ_RGBA_U8_STRIP;
         }
         else if (theSamplesPerPixel <= 3 && theBitsPerSample == 1)
         {
            //---
            // Note:  One bit data expands to zeroes and 255's so run it through
            //        a specialized method to flip zeroes to one's since zeroes
            //        are usually reserved for null value.
            //---
            theReadMethod[dir] = READ_RGBA_U8A_STRIP;
         }
         else
         {
            theReadMethod[dir] = READ_SCAN_LINE;
         }
      }
      
   } // End of loop through directories.

   // Reset the directory back to "0".
   setTiffDirectory(0);
}

void ossimTiffTileSource::initializeBuffers()
{
   if(theBuffer)
   {
      delete [] theBuffer;
      theBuffer = 0;
   }

   ossimImageDataFactory* idf = ossimImageDataFactory::instance();

   theTile = idf->create(this,
                         this);
   
   //
   // Tiles are constructed with no buffer storage.  Call initialize for
   // "theTile" to allocate memory.  Leave "theBlankTile" with a
   // ossimDataObjectStatus of OSSIM_NULL since no data will ever be
   // stuffed in it.
   //
   theTile->initialize();
   allocateBuffer();
}


void ossimTiffTileSource::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property.valid())
   {
      return;
   }
   if(property->getName() == "apply_color_palette_flag")
   {
      // Assuming first directory...
      setApplyColorPaletteFlag(property->valueToString().toBool());
   }
   else
   {
      ossimImageHandler::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimTiffTileSource::getProperty(const ossimString& name)const
{
   if(name == "apply_color_palette_flag")
   {
      ossimBooleanProperty* property = new ossimBooleanProperty("apply_color_palette_flag",
                                                                theApplyColorPaletteFlag);
      property->clearChangeType();
      property->setFullRefreshBit();
      return property;
   }
   
   return ossimImageHandler::getProperty(name);
}

void ossimTiffTileSource::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageHandler::getPropertyNames(propertyNames);

   // Assuming first directory...
   if(isColorMapped())
   {
      propertyNames.push_back("apply_color_palette_flag");
   }
}

bool ossimTiffTileSource::setTiffDirectory(ossim_uint32 directory)
{
   bool status = TIFFSetDirectory(theTiffPtr, directory);
   if (status == true)
   {
      theCurrentDirectory = directory;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimTiffTileSource::setTiffDirectory ERROR setting directory "
         << directory << "!" << endl;
   }
   return status;
}

void ossimTiffTileSource::populateLut()
{
   ossim_uint16* r;
   ossim_uint16* g;
   ossim_uint16* b;
   if(TIFFGetField(theTiffPtr, TIFFTAG_COLORMAP, &r, &g, &b))
   {
      ossim_uint32 numEntries = 256;
      ossimScalarType scalarType = OSSIM_UINT8;
      if(theBitsPerSample == 16)
      {
         numEntries = 65536;
         scalarType = OSSIM_UINT16;
      }
      theLut = new ossimNBandLutDataObject(numEntries,
                                           3,
                                           scalarType,
                                           0);
      ossim_uint32 entryIdx = 0;
      for(entryIdx = 0; entryIdx < numEntries; ++entryIdx)
      {
         if(scalarType == OSSIM_UINT8)
         {
            (*theLut)[entryIdx][0] = (ossimNBandLutDataObject::LUT_ENTRY_TYPE)(((*r)/65535.0)*255.0);
            (*theLut)[entryIdx][1] = (ossimNBandLutDataObject::LUT_ENTRY_TYPE)(((*g)/65535.0)*255.0);
            (*theLut)[entryIdx][2] = (ossimNBandLutDataObject::LUT_ENTRY_TYPE)(((*b)/65535.0)*255.0);
         }
         else
         {
            (*theLut)[entryIdx][0] = (ossimNBandLutDataObject::LUT_ENTRY_TYPE)(*r);
            (*theLut)[entryIdx][1] = (ossimNBandLutDataObject::LUT_ENTRY_TYPE)(*g);
            (*theLut)[entryIdx][2] = (ossimNBandLutDataObject::LUT_ENTRY_TYPE)(*b);
         }
         ++r;++g;++b;
      }
   }
}
