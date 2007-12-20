//*******************************************************************
//
// See top level LICENSE.txt file.
//
// Author: Ken Melero
// Contributor: David A. Horner (DAH) - http://dave.thehorners.com
// 
// Description: This class give the capability to access tiles from an
//              ADRG file.
//
//********************************************************************
// $Id: ossimAdrgTileSource.cpp 11286 2007-07-11 16:38:46Z dburken $

#include <iostream>

#include <ossim/imaging/ossimAdrgTileSource.h>
#include <ossim/imaging/ossimAdrgHeader.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimUnitTypeLut.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimU8ImageData.h>

RTTI_DEF1(ossimAdrgTileSource, "ossimAdrgTileSource", ossimImageHandler)

//***
// Static trace for debugging
//***
static ossimTrace traceDebug("ossimAdrgTileSource:debug");

//***
// NOTE:  Currently this tilesource is hard-coded to access tiles
//        from the ZDR image file. This will change as I wrote support
//        data classes for all the ADRG image files: ZDR, overview,
//        and legend support data.  Next step in development will
//        be to put in flags to access overview and legend image files.
//***

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimAdrgTileSource::ossimAdrgTileSource()
   :
      ossimImageHandler(),
      theTile(0),
      theTileBuffer(0),
      theFileStr(),
      theAdrgHeader(0)
{
   // Construction not complete.  Users should call "open" method.
}

ossimAdrgTileSource::~ossimAdrgTileSource()
{
   if(theAdrgHeader)
   {
      delete theAdrgHeader;
      theAdrgHeader = 0;
   }
   if (theTileBuffer)
   {
      delete [] theTileBuffer;
      theTileBuffer = 0;
   }

   close();
}

ossimRefPtr<ossimImageData> ossimAdrgTileSource::getTile(
   const ossimIrect& rect,
   ossim_uint32 resLevel)
{
   // This tile source bypassed, or invalid res level, return a blank tile.
   if (!isSourceEnabled() || !isValidRLevel(resLevel))
   {
      return ossimRefPtr<ossimImageData>();
   }

   if (theOverview)
   {
      if (theOverview->hasR0() || resLevel)
      {
         return theOverview->getTile(rect, resLevel);
      }
   }

   theTile->setImageRectangle(rect);
   ossimIrect image_rect = getImageRectangle(resLevel);
   theTile->makeBlank();

   //***
   // See if any point of the requested tile is in the image.
   //***
   if ( rect.intersects(image_rect) )
   {
      ossimIrect clip_rect = rect.clipToRect(image_rect);
      
//       if ( !rect.completely_within(clip_rect) )
//       {
//          // Start with a blank tile.
//          theTile->makeBlank();
//       }

      // Load the tile buffer with data from the adrg.
      if (fillBuffer(rect, clip_rect))
      {
         theTile->validate();
      }
   }
//    else
//    {
//       theTile->makeBlank();
//    }

   return theTile;
}

//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimAdrgTileSource::fillBuffer(const ossimIrect& tile_rect,
                                     const ossimIrect& clip_rect)
{
//    const ossim_uint8 NULL_PIXEL         = (ossim_uint8)theTile->getNullPix(0);
   // const ossim_int32 TILE_SIZE_PER_BAND = theTile->getSizePerBand();

   //***
   // Shift the upper left corner of the "clip_rect" to the an even chunk
   // boundry.
   //***
   ossimIpt tileOrigin = clip_rect.ul();
   adjustToStartOfTile(tileOrigin);

   //***
   // Calculate the number of tiles needed in the line/sample directions.
   //***
   ossim_int32 size_in_x = clip_rect.lr().x - tileOrigin.x + 1;
   ossim_int32 size_in_y = clip_rect.lr().y - tileOrigin.y + 1;
   
   ossim_int32 tiles_in_x_dir = size_in_x / ADRG_TILE_WIDTH  + (size_in_x % ADRG_TILE_WIDTH  ? 1 : 0);
   ossim_int32 tiles_in_y_dir = size_in_y / ADRG_TILE_HEIGHT + (size_in_y % ADRG_TILE_HEIGHT ? 1 : 0);


   ossimIpt ulTilePt = tileOrigin;
   
   // Chunk loop in line direction.
   for (int32 y=0; y<tiles_in_y_dir; y++)
   {
      ulTilePt.x = tileOrigin.x;

      // Tile loop in sample direction.
      for (int32 x=0; x<tiles_in_x_dir; x++)
      {
         ossimIrect adrg_tile_rect(ulTilePt.x,
                                   ulTilePt.y,
                                   ulTilePt.x + ADRG_TILE_WIDTH- 1,
                                   ulTilePt.y + ADRG_TILE_HEIGHT - 1);

         if (adrg_tile_rect.intersects(clip_rect))
         {
            ossimIrect tile_clip_rect = clip_rect.clipToRect(adrg_tile_rect);
            
            //***
            // Some point in the chip intersect the tile so grab the
            // data.
            //***
            ossim_int32 row = (ossim_int32) ulTilePt.y / ADRG_TILE_HEIGHT; 
            ossim_int32 col = (ossim_int32) ulTilePt.x / ADRG_TILE_WIDTH;
            ossim_int32 tileOffset = theAdrgHeader->tim(row, col);

            if(tileOffset != 0)
            {
               // Get the data.
               int seek_position = (tileOffset - 1) * 49152 + 2048;
               int band;

               // seek to start of chip
               theFileStr.seekg(seek_position, ios::beg);
               for (band=0; band<3; band++)
               {
                  // Seek to the chip
//                   theFileStr.seekg(seek_position, ios::beg);

                  //***
                  // Read the chip from the ccf file into the chunk buffer.
                  // This will get all the bands.  Bands are interleaved by
                  // chip.
                  //***
                  if (!theFileStr.read((char*)theTileBuffer,
                                       ADRG_TILE_SIZE))
                  {
                     theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
                     return false;
                  }

//                  seek_position += BAND_OFFSET;
                  theTile->loadBand(theTileBuffer,
                                    adrg_tile_rect,
                                    tile_clip_rect,
                                    band);

               } // End of band loop.
               
            } // End of if (tileOffset != 0)
            
         } // End of if (adrg_tile_rect.intersects(clip_rect))
         
         ulTilePt.x += ADRG_TILE_WIDTH;
         
      }  // End of tile loop in the sample direction.

      ulTilePt.y += ADRG_TILE_HEIGHT;
      
   }  // End of tile loop in the line direction.

   return true;
}

ossim_uint32 ossimAdrgTileSource::getNumberOfOutputBands()const
{
   return getNumberOfInputBands();
}

//*******************************************************************
// Public Method:
//*******************************************************************
ossimIrect
ossimAdrgTileSource::getImageRectangle(ossim_uint32 reduced_res_level) const
{
   return ossimIrect(0,                         // upper left x
                     0,                         // upper left y
                     getNumberOfSamples(reduced_res_level) - 1,  // lower right x
                     getNumberOfLines(reduced_res_level)   - 1); // lower right y
}

void ossimAdrgTileSource::close()
{
   if(theAdrgHeader)
   {
      delete theAdrgHeader;
      theAdrgHeader = 0;
   }
   if(theFileStr.is_open())
   {
      theFileStr.close();
   }
   theTile = 0;
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimAdrgTileSource::open()
{
   static const char MODULE[] = "ossimAdrgTileSource::open";

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << "Entered..."<<std::endl;
   }
   if(isOpen())
   {
      close();
   }
   if(theAdrgHeader)
   {
      delete theAdrgHeader;
      theAdrgHeader = 0;
   }
   // Instantiate support data class to parse header file.
   theAdrgHeader = new ossimAdrgHeader(theImageFile);

   // Check for errors.
   if (theAdrgHeader->errorStatus() == ossimErrorCodes::OSSIM_ERROR)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " Error in ossimAdrg header detected." << std::endl;
      }

      close();
      return false;
   }

   theFileStr.open(theAdrgHeader->imageFile().c_str(),
                   ios::in | ios::binary);

   // Check the file pointer.
   if(!theFileStr)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << MODULE << "\nCannot open:  "
              << theAdrgHeader->imageFile().c_str() << std::endl;
      }
      close();
      
      return false;
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE
         << "File is opened -> "<< theAdrgHeader->imageFile()<<std::endl;
   }

   // allow the base handler to check for other overrides
   completeOpen();
   // Allocate memory.
   theTile      = ossimImageDataFactory::instance()->create(this, this);
   theTile->initialize();
   theTileBuffer  = new ossim_uint8[ADRG_TILE_SIZE];

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\nminLon:   " << theAdrgHeader->minLon()
         << "\nminLond:  " << theAdrgHeader->minLongitude() 
         << "\nminLat:   " << theAdrgHeader->minLat()
         << "\nminLatd:  " << theAdrgHeader->minLatitude()
         << "\nmaxLon:   " << theAdrgHeader->maxLon()
         << "\nmaxLond:  " << theAdrgHeader->maxLongitude()
         << "\nmaxLat:   " << theAdrgHeader->maxLat()
         << "\nmaxLatd:  " << theAdrgHeader->maxLatitude()
         << std::endl;
   }
   
   return true;
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimAdrgTileSource::saveState(ossimKeywordlist& kwl,
                                    const char* prefix) const
{
   // Currently nothing to do here.
   return ossimImageHandler::saveState(kwl, prefix);
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimAdrgTileSource::loadState(const ossimKeywordlist& kwl,
                                    const char* prefix)
{
   if (ossimImageHandler::loadState(kwl, prefix))
   {
      if (open())
      {
         return true;
      }
   }

   return false;
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimAdrgTileSource::getImageGeometry(ossimKeywordlist& kwl,
                                           const char* prefix)
{
   if(ossimImageSource::getImageGeometry(kwl, prefix))
   {
      return true;
   }

   // origin of latitude
   ossim_float64 originLatitude = (theAdrgHeader->maxLatitude() +
                                   theAdrgHeader->minLatitude()) / 2.0;
   
   // central meridian.
   ossim_float64 centralMeridian = (theAdrgHeader->maxLongitude() +
                                    theAdrgHeader->minLongitude()) / 2.0;

   //---
   // Compute the pixel size in latitude and longitude direction.  This will
   // be full image extents divided by full image lines and samples.
   //---
   
   // Samples in full image (used to compute degPerPixelX).
   ossim_float64 samples = theAdrgHeader->samples();
   
   // Lines in full image (used to compute degPerPixelX).
   ossim_float64 lines = theAdrgHeader->lines();

   // Degrees in latitude direction of the full image.
   ossim_float64 degrees_in_lat_dir = theAdrgHeader->maxLatitude() -
      theAdrgHeader->minLatitude();

   // Degrees in longitude direction of the full image.
   ossim_float64 degrees_in_lon_dir = theAdrgHeader->maxLongitude() -
      theAdrgHeader->minLongitude();
   
   ossim_float64 degPerPixelY = degrees_in_lat_dir / lines;
   ossim_float64 degPerPixelX = degrees_in_lon_dir / samples;
   
   //---
   // The tie is determined with the following assumptions that need to be
   // verified:
   // 1) Rows and columns start at 1.
   // 2) The min / max latitudes longitudes go to the edge of the pixel.
   // 3) Latitude decreases by degPerPixelY with each line.
   // 4) Longitude increases by degPerPixelX with each sample.
   //---

   // OLD:
   //    double ul_lat = (theAdrgHeader->maxLatitude() + 
   //                     theAdrgHeader->startRow()*degPerPixelY) - (degPerPixelY*.5);
   //    double ul_lon = (theAdrgHeader->minLongitude() -
   //                     theAdrgHeader->startCol()*degPerPixelX) +  (degPerPixelX*.5); 
   
   ossim_float64 ul_lat = (theAdrgHeader->maxLatitude() - 
                    ( (theAdrgHeader->startRow() - 1) *
                      degPerPixelY ) - ( degPerPixelY * 0.5 ) );
   ossim_float64 ul_lon = (theAdrgHeader->minLongitude() +
                    ( (theAdrgHeader->startCol() -1) *
                      degPerPixelX ) +  ( degPerPixelX * 0.5 ) );
   
   // projection type
   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           "ossimEquDistCylProjection",
           true);

   // datum.
   kwl.add(prefix,
           ossimKeywordNames::DATUM_KW,
           "WGE",
           true);

   // origin latitude
   kwl.add(prefix,
           ossimKeywordNames::ORIGIN_LATITUDE_KW,
           originLatitude,
           true);

   // central meridin
   kwl.add(prefix,
           ossimKeywordNames::CENTRAL_MERIDIAN_KW,
           centralMeridian,
           true);

   // Save the tie point.
   kwl.add(prefix,
           ossimKeywordNames::TIE_POINT_XY_KW,
           ossimDpt(ul_lon, ul_lat).toString().c_str(),
           true);
   kwl.add(prefix,
           ossimKeywordNames::TIE_POINT_UNITS_KW,
           ossimUnitTypeLut::instance()->getEntryString(OSSIM_DEGREES),
           true);

   // Save the scale.
   kwl.add(prefix,
           ossimKeywordNames::TIE_POINT_LAT_KW,
           ul_lat,
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::TIE_POINT_LON_KW,
           ul_lon,
           true);

   // Save the scale.
   kwl.add(prefix,
           ossimKeywordNames::PIXEL_SCALE_XY_KW,
           ossimDpt(degPerPixelX, degPerPixelY).toString().c_str(),
           true);
   kwl.add(prefix,
           ossimKeywordNames::PIXEL_SCALE_UNITS_KW,
           ossimUnitTypeLut::instance()->getEntryString(OSSIM_DEGREES),
           true);  

   // lines
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_LINES_KW,
           getNumberOfLines());

   // samples
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_SAMPLES_KW,
           getNumberOfSamples());

   // res sets
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_REDUCED_RES_SETS_KW,
           getNumberOfDecimationLevels());

   // bands
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_INPUT_BANDS_KW,
           getNumberOfInputBands());

   // bands
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_OUTPUT_BANDS_KW,
           getNumberOfOutputBands());

   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\nminLon:             " << theAdrgHeader->minLon()
         << "\nminLond:            " << theAdrgHeader->minLongitude() 
         << "\nminLat:             " << theAdrgHeader->minLat()
         << "\nminLatd:            " << theAdrgHeader->minLatitude()
         << "\nmaxLon:             " << theAdrgHeader->maxLon()
         << "\nmaxLond:            " << theAdrgHeader->maxLongitude()
         << "\nmaxLat:             " << theAdrgHeader->maxLat()
         << "\nmaxLatd:            " << theAdrgHeader->maxLatitude()
         << "\nstartRow:           " << theAdrgHeader->startRow()
         << "\nstartCol:           " << theAdrgHeader->startCol()
         << "\nstopRow:            " << theAdrgHeader->stopRow()
         << "\nstopCol:            " << theAdrgHeader->stopCol()
         << "\nfull image lines:   " << lines
         << "\nfull image samples: " << samples
         << "\nkwl:\n"               << kwl
         << std::endl;
   }

   setImageGeometry(kwl);

   return true;
}

//*******************************************************************
// Public method:
//*******************************************************************
ossimScalarType ossimAdrgTileSource::getOutputScalarType() const
{
  return OSSIM_UCHAR;
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimAdrgTileSource::getTileWidth() const
{
   return ( theTile.valid() ? theTile->getWidth() : 0 );
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimAdrgTileSource::getTileHeight() const
{
   return ( theTile.valid() ? theTile->getHeight() : 0 );
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimAdrgTileSource::getNumberOfLines(ossim_uint32 reduced_res_level) const
{
   if ( (reduced_res_level == 0) && theAdrgHeader )
   {
      return (theAdrgHeader->stopRow() - theAdrgHeader->startRow()) + 1;
   }
   else if (theOverview)
   {
      return theOverview->getNumberOfLines(reduced_res_level);
   }

   return 0;
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimAdrgTileSource::getNumberOfSamples(ossim_uint32 reduced_res_level) const
{
   if ( (reduced_res_level == 0) && theAdrgHeader )
   {
      return (theAdrgHeader->stopCol() - theAdrgHeader->startCol()) + 1;
   }
   else if (theOverview)
   {
      return theOverview->getNumberOfSamples(reduced_res_level);
   }

   return 0;
}

ossim_uint32 ossimAdrgTileSource::getImageTileWidth() const
{
   return ADRG_TILE_WIDTH;
}

ossim_uint32 ossimAdrgTileSource::getImageTileHeight() const
{
   return ADRG_TILE_HEIGHT;
}

ossimString ossimAdrgTileSource::getShortName()const
{
   return ossimString("adrg");
}

ossimString ossimAdrgTileSource::getLongName()const
{
   return ossimString("adrg reader");
}

ossimString ossimAdrgTileSource::className()const
{
   return ossimString("ossimAdrgTileSource");
}

ossim_uint32 ossimAdrgTileSource::getNumberOfInputBands() const
{
   return theAdrgHeader->numberOfBands();
}

bool ossimAdrgTileSource::isOpen()const
{
   return (theAdrgHeader!=0);
}

void ossimAdrgTileSource::adjustToStartOfTile(ossimIpt& pt) const
{
   pt.x &= 0xffffff80;
   pt.y &= 0xffffff80;
}
