//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Ken Melero
//
// Author: Ken Melero
// 
// Description: This class give the capability to access tiles from an
//              ADRG file.
//
//********************************************************************
// $Id: ossimAdrgTileSource.cpp,v 1.50 2005/02/11 15:07:30 dburken Exp $

#include <iostream>
using namespace std;

#include <imaging/formats/adrg/ossimAdrgTileSource.h>
#include <base/common/ossimConstants.h>
#include <base/common/ossimCommon.h>
#include <base/common/ossimKeywordNames.h>
#include <base/common/ossimTrace.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/data_types/ellipse/ossimEllipsoid.h>
#include <base/data_types/datum/ossimDatum.h>
#include <projections/map_projections/ossimMapProjection.h>
#include <projections/map_projections/ossimEquDistCylProjection.h>
#include <imaging/formats/tiff/ossimTiffTileSource.h>
#include <imaging/factory/ossimImageDataFactory.h>
#include <imaging/ossimU8ImageData.h>

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
      theTile(NULL),
      theTileBuffer(NULL),
      theFileStr(),
      theAdrgHeader(NULL)
{
   // Construction not complete.  Users should call "open" method.
}

ossimAdrgTileSource::~ossimAdrgTileSource()
{
   if(theAdrgHeader)
   {
      delete theAdrgHeader;
      theAdrgHeader = NULL;
   }
   if (theTileBuffer)
   {
      delete [] theTileBuffer;
      theTileBuffer = NULL;
   }

   close();
}

ossimRefPtr<ossimImageData> ossimAdrgTileSource::getTile(
   const ossimIrect& rect,
   ossim_uint32 resLevel)
{
   long w = rect.width();
   long h = rect.height();
   
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

   long tileW = theTile->getWidth();
   long tileH = theTile->getHeight();
   if((w != tileW)||
      (h != tileH))
   {
      theTile->setWidth(w);
      theTile->setHeight(h);
      if((w*h)!=(tileW*tileH))
      {
         theTile->initialize();
      }
   }
   theTile->setOrigin(rect.ul());
   ossimIrect image_rect = getImageRectangle(resLevel);
   
   //***
   // See if any point of the requested tile is in the image.
   //***
   if ( rect.intersects(image_rect) )
   {
      ossimIrect clip_rect = rect.clipToRect(image_rect);
      
      if ( !rect.completely_within(clip_rect) )
      {
         // Start with a blank tile.
         theTile->makeBlank();
      }

      // Load the tile buffer with data from the adrg.
      if (fillBuffer(rect, clip_rect))
      {
//         theTile->setOrigin(rect.ul()); // Capture the origin...
         theTile->validate();
         return theTile;
      }
      else
      {
         return ossimRefPtr<ossimImageData>();
      }
      
   } // End of if ( tile_rect.intersects(image_rect) )

   // No part of requested tile within the image rectangle.
   return ossimRefPtr<ossimImageData>();
}

//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimAdrgTileSource::fillBuffer(const ossimIrect& tile_rect,
                                     const ossimIrect& clip_rect)
{
   const ossim_uint8 NULL_PIXEL         = (ossim_uint8)theTile->getNullPix(0);
   const ossim_int32 TILE_SIZE_PER_BAND = theTile->getSizePerBand();

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
                              ulTilePt.x + ADRG_TILE_SIZE - 1,
                              ulTilePt.y + ADRG_TILE_SIZE - 1);
         
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

               for (band=0; band<3; band++)
               {
                  // Seek to the chip
                  theFileStr.seekg(seek_position, ios::beg);

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

                  seek_position += BAND_OFFSET;

                  //***
                  // Get a pointer positioned at the first valid pixel in
                  // the adrg tile.
                  //***
                  ossim_uint8* s
                     = theTileBuffer + 
                     (tile_clip_rect.ul().y - adrg_tile_rect.ul().y) *
                     ADRG_TILE_WIDTH + tile_clip_rect.ul().x -
                     adrg_tile_rect.ul().x;

                  //***
                  // Get a pointer positioned at the first valid pixel in
                  // the tile.
                  //***
                  ossim_uint8* d
                     = (ossim_uint8*)theTile->getBuf() +
                     band * TILE_SIZE_PER_BAND +
                     (tile_clip_rect.ul().y - tile_rect.ul().y) *
                     theTile->getWidth() + tile_clip_rect.ul().x -
                     tile_rect.ul().x;      

                  // Line loop through valid portion of the adrg tile.
                  for (ossim_int32 line = tile_clip_rect.ul().y;
                       line <= tile_clip_rect.lr().y;
                       line++)
                  {
                     // Sample loop through the adrg tile.
                     int i=0;
                     for (ossim_int32 sample = tile_clip_rect.ul().x;
                          sample <= tile_clip_rect.lr().x;
                          sample++)
                     {
                        d[i] = s[i];
                        i++;
                     }

                     // Increment the pointers by one line.
                     s += ADRG_TILE_WIDTH;
                     d += theTile->getWidth();
                  }

               } // End of band loop.

            } // End of if (tileOffset != 0)
            else
            {
               // The adrg tile is null.

               for (int band=0; band<3; band++)
               {
                  //***
                  // Get a pointer positioned at the first valid pixel in
                  // the tile.
                  //***
                  ossim_uint8* d
                     = (ossim_uint8*)theTile->getBuf() +
                     band * TILE_SIZE_PER_BAND +
                     (tile_clip_rect.ul().y - tile_rect.ul().y) *
                     theTile->getWidth() + tile_clip_rect.ul().x -
                     tile_rect.ul().x;      
                  
                  // Line loop through valid portion of the adrg tile.
                  for (ossim_int32 line = tile_clip_rect.ul().y;
                       line <= tile_clip_rect.lr().y;
                       line++)
                  {
                     // Sample loop through a chip.
                     int i=0;
                     for (ossim_int32 sample = tile_clip_rect.ul().x;
                          sample <= tile_clip_rect.lr().x;
                          sample++)
                     {
                        d[i] = NULL_PIXEL;
                        i++;
                     }
                     
                     // Increment the pointers by one line.
                     d += theTile->getWidth();
                  }
                  
               } // End of band loop.

            } // End of else.

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
      theAdrgHeader = NULL;
   }
   if(theFileStr.is_open())
   {
      theFileStr.close();
   }
   theTile = NULL;
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimAdrgTileSource::open()
{
   static const char MODULE[] = "ossimAdrgTileSource::open";

   if(traceDebug())
   {
      CLOG << "Entered..."<<endl;
   }
   if(isOpen())
   {
      close();
   }
   if(theAdrgHeader)
   {
      delete theAdrgHeader;
      theAdrgHeader = NULL;
   }
   // Instantiate support data class to parse header file.
   theAdrgHeader = new ossimAdrgHeader(theImageFile);

   // Check for errors.
   if (theAdrgHeader->errorStatus() == ossimErrorCodes::OSSIM_ERROR)
   {
      if (traceDebug())
      {
         cerr << MODULE << " Error in ossimAdrg header detected." << endl;
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
         cerr << MODULE << "\nCannot open:  "
              << theAdrgHeader->imageFile().c_str() << endl;
      }
      close();
      
      return false;
   }
   
   if(traceDebug())
   {
      CLOG << "File is opened -> "<< theAdrgHeader->imageFile()<<endl;
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
         << endl;
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
   
   //***
   // NOTE:
   // All these keywords are going to change.  Hard coded right now for test.
   //***
   
   // model type.  This will be going away soon and will
   // be replaced with projection.type bellow.
   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           "ossimEquDistCylProjection",
           true);
   
   ossimString projPref = prefix?prefix:"";

   // origin of latitude
   double lat = (theAdrgHeader->maxLatitude() +
                 theAdrgHeader->minLatitude()) / 2.0;
   
   // central meridian.
   double lon = (theAdrgHeader->maxLongitude() +
                 theAdrgHeader->minLongitude()) / 2.0;
   
   //***
   // Make a projection to get the easting / northing of the tie point and
   // the scale in meters.
   //***
   const ossimDatum* datum = ossimDatumFactory::instance()->wgs84();

   ossimProjection* proj
      = new ossimEquDistCylProjection(*(datum->ellipsoid()),
                                      ossimGpt(lat, lon),
                                      0.0,   // false easting
                                      0.0);  // false northing
   proj->saveState(kwl, projPref);
   
   // Get the scale in meters.
   ossimGpt ul_gpt(theAdrgHeader->maxLatitude(),
                   theAdrgHeader->minLongitude(),
                   OSSIM_DBL_NAN);

   //   ossimDpt ul_dpt = proj->forward(ul_gpt);
   
   ossimGpt ur_gpt(theAdrgHeader->maxLatitude(),
                   theAdrgHeader->maxLongitude(),
                   0.0);

   //   ossimDpt ur_dpt = proj->forward(ur_gpt);

   ossimGpt ll_gpt(theAdrgHeader->minLatitude(),
                   theAdrgHeader->minLongitude(),
                   0.0);
   
   //   ossimDpt ll_dpt = proj->forward(ll_gpt);
   
   ossimGpt lr_gpt(theAdrgHeader->minLatitude(),
                   theAdrgHeader->maxLongitude(),
                   0.0);
   
   //   ossimDpt lr_dpt = proj->forward(lr_gpt);

   double samples = theAdrgHeader->stopCol() - theAdrgHeader->startCol() + 1;
   double lines   = theAdrgHeader->stopRow() - theAdrgHeader->startRow() + 1;

//    double meters_per_pix_x = (ur_dpt.x - ul_dpt.x) / samples;
//    double meters_per_pix_y = (ur_dpt.y - lr_dpt.y) / lines;

   // will remove these two and replace with the next two
   // we will for now just duplicate with different prefix.
//    kwl.add(prefix,
//            ossimKeywordNames::METERS_PER_PIXEL_X_KW,
//            meters_per_pix_x,
//            true);

//    kwl.add(prefix,
//            ossimKeywordNames::METERS_PER_PIXEL_Y_KW,
//            meters_per_pix_y,
//            true);


   //***
   // To get the tie point the padding must be accounted for...
   // The min / max lat / lon is relative to the unpadded image.
   //***
   double degrees_in_lat_dir = theAdrgHeader->maxLatitude() -
                               theAdrgHeader->minLatitude();
   double degrees_in_lon_dir = theAdrgHeader->maxLongitude() -
                               theAdrgHeader->minLongitude();
   
   double degPerPixelY = degrees_in_lat_dir / lines;
   double degPerPixelX = degrees_in_lon_dir / samples;

//    double ul_lat = theAdrgHeader->maxLatitude() +
//                    (theAdrgHeader->startRow() - 1.0) * degPerPixelY;
//    double ul_lon = theAdrgHeader->minLongitude() -
//                    (theAdrgHeader->startCol() - 1.0) * degPerPixelX;
   double ul_lat = (theAdrgHeader->maxLatitude() + 
                    theAdrgHeader->startRow()*degPerPixelY) - (degPerPixelY*.5);
   double ul_lon = (theAdrgHeader->minLongitude() -
                    theAdrgHeader->startCol()*degPerPixelX) +  (degPerPixelX*.5);

//    ossimGpt ul_adjusted_gpt(ul_lat,
//                             ul_lon,
//                             0.0);

//   ossimDpt ul_adjusted_dpt = proj->forward(ul_adjusted_gpt);

   kwl.add(prefix,
           ossimKeywordNames::TIE_POINT_LAT_KW,
           ul_lat,
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::TIE_POINT_LON_KW,
           ul_lon,
           true);

   kwl.add(prefix,
           ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT,
           degPerPixelY,
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON,
           degPerPixelX,
           true);
   
   
   // will remove the first two here and replace
   // with the second 2
//    kwl.add(prefix,
//            ossimKeywordNames::TIE_POINT_EASTING_KW,
//            ul_adjusted_dpt.x,
//            true);

//    kwl.add(prefix,
//            ossimKeywordNames::TIE_POINT_NORTHING_KW,
//            ul_adjusted_dpt.y,
//            true);

//    kwl.add(projPref,
//            ossimKeywordNames::TIE_POINT_EASTING_KW,
//            ul_adjusted_dpt.x,
//            true);

//    kwl.add(projPref,
//            ossimKeywordNames::TIE_POINT_NORTHING_KW,
//            ul_adjusted_dpt.y,
//            true);
   
   // now add the general image geometry to the
   // list.
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_LINES_KW,
           getNumberOfLines());

   kwl.add(prefix,
           ossimKeywordNames::NUMBER_SAMPLES_KW,
           getNumberOfSamples());
   
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_REDUCED_RES_SETS_KW,
           getNumberOfDecimationLevels());

   kwl.add(prefix,
           ossimKeywordNames::NUMBER_INPUT_BANDS_KW,
           getNumberOfInputBands());

   kwl.add(prefix,
           ossimKeywordNames::NUMBER_OUTPUT_BANDS_KW,
           getNumberOfOutputBands());

   kwl.add(prefix,
           ossimKeywordNames::DATUM_KW,
           "WGE",
           true);
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\nminLon:           " << theAdrgHeader->minLon()
         << "\nminLond:          " << theAdrgHeader->minLongitude() 
         << "\nminLat:           " << theAdrgHeader->minLat()
         << "\nminLatd:          " << theAdrgHeader->minLatitude()
         << "\nmaxLon:           " << theAdrgHeader->maxLon()
         << "\nmaxLond:          " << theAdrgHeader->maxLongitude()
         << "\nmaxLat:           " << theAdrgHeader->maxLat()
         << "\nmaxLatd:          " << theAdrgHeader->maxLatitude()
         << "\nstartRow:         " << theAdrgHeader->startRow()
         << "\nstartCol:         " << theAdrgHeader->startCol()
         << "\nstopRow:          " << theAdrgHeader->stopRow()
         << "\nstopCol:          " << theAdrgHeader->stopCol()
//            << "\nul_adjusted_gpt:  " << ul_adjusted_gpt
         << "\nul_gpt:           " << ul_gpt
         << "\nur_gpt:           " << ur_gpt
         << "\nlr_gpt:           " << lr_gpt
         << "\nll_gpt:           " << ll_gpt
//            << "\nul_adjusted_dpt:  " << ul_adjusted_dpt
//            << "\nul_dpt:           " << ul_dpt
//            << "\nur_dpt:           " << ur_dpt
//            << "\nlr_dpt:           " << lr_dpt
//            << "\nll_dpt:           " << ll_dpt
//            << "\nmeters_per_pix_x: " << meters_per_pix_x
//            << "\nmeters_per_pix_y: " << meters_per_pix_y
         << "kwl:\n"               << kwl
         << endl;
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
   if (reduced_res_level == 0)
   {
      return theAdrgHeader->lines();
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
   if (reduced_res_level == 0)
   {
      return theAdrgHeader->samples();
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
   return (theAdrgHeader!=NULL);
}

void ossimAdrgTileSource::adjustToStartOfTile(ossimIpt& pt) const
{
   pt.x &= 0xffffff80;
   pt.y &= 0xffffff80;
}
