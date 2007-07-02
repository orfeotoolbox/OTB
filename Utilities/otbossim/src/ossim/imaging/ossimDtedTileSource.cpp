//*******************************************************************
// Copyright (C) 2001 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Contains class declaration for ossimDtedTileSource.
//
//********************************************************************
// $Id: ossimDtedTileSource.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <iostream>
#include <fstream>
using namespace std;

#include <ossim/imaging/ossimDtedTileSource.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimS16ImageData.h>
#include <ossim/support_data/ossimDtedVol.h>
#include <ossim/support_data/ossimDtedHdr.h>
#include <ossim/support_data/ossimDtedUhl.h>
#include <ossim/support_data/ossimDtedDsi.h>
#include <ossim/support_data/ossimDtedAcc.h>

RTTI_DEF1(ossimDtedTileSource, "ossimDtedTileSource", ossimImageHandler)

// Static trace for debugging
static ossimTrace traceDebug("ossimDtedTileSource:debug");


ossimDtedTileSource::ossimDtedTileSource()
   :
      ossimImageHandler(),
      theTile(NULL),
      theFileStr(),
      theTileWidth(0),
      theTileHeight(0),
      theNumberOfLines(0),
      theNumberOfSamps(0),
      theOffsetToFirstDataRecord(0),
      theDataRecordSize(0),
      theMinHeight(0),
      theMaxHeight(0),
      thePostSpacing(0.0, 0.0),
      theSwapBytesFlag(false)
{
   if (ossimGetByteOrder() == OSSIM_LITTLE_ENDIAN)
   {
      theSwapBytesFlag = true;
   }

   // Set the tile size to the default.
   ossimIpt defaultTileSize;
   ossimGetDefaultTileSize(defaultTileSize);
   theTileWidth  = static_cast<ossim_uint32>(defaultTileSize.x);
   theTileHeight = static_cast<ossim_uint32>(defaultTileSize.y);
   
   // Construction not complete.  Users should call "open" method.
}

ossimDtedTileSource::~ossimDtedTileSource()
{
   theFileStr.close();
}

ossimString ossimDtedTileSource::getShortName()const
{
   return ossimString("dted");
}

ossimString ossimDtedTileSource::getLongName()const
{
   return ossimString("dted reader");
}

ossimString ossimDtedTileSource::className()const
{
   return ossimString("ossimDtedTileSource");
}   

ossimRefPtr<ossimImageData> ossimDtedTileSource::getTile(
   const  ossimIrect& tile_rect, ossim_uint32 resLevel)
{
   if (!theTile.valid())
   {
      allocate(); // First getTile call or after changeTileSize.
      if (!theTile.valid())
      {
         return ossimRefPtr<ossimImageData>(); // Throw exception???
      }
   }

   // This tile source bypassed, or invalid res level, return a blank tile.
   if (!isOpen()||!isSourceEnabled() || !isValidRLevel(resLevel))
   {
      return ossimRefPtr<ossimImageData>();
   }

   if (theOverview)
   {
      if (theOverview->hasR0() || resLevel)
      {
         ossimRefPtr<ossimImageData> temp =
            theOverview->getTile(tile_rect, resLevel);
         if(temp.valid() && temp->getBuf())
         {
            temp->setMinPix(theTile->getMinPix(), theTile->getNumberOfBands());
            temp->setMaxPix(theTile->getMaxPix(), theTile->getNumberOfBands());
            temp->setNullPix(theTile->getNullPix(),
                             theTile->getNumberOfBands());
         }
         else
         {
            return ossimRefPtr<ossimImageData>();
         }
         return temp;
      }
   }

   // Set the origing and resize if needed...
   theTile->setImageRectangle(tile_rect);
   
   ossimIrect image_rect = getImageRectangle(resLevel);
   
   //***
   // See if any point of the requested tile is in the image.
   //***
   if ( tile_rect.intersects(image_rect) )
   {
      ossimIrect clip_rect = tile_rect.clipToRect(image_rect);
      
      if ( !tile_rect.completely_within(clip_rect) )
      {
         // Start with a blank tile.
         theTile->makeBlank();
      }

      // Load the tile buffer with data from the dted cell.
      if (fillBuffer(tile_rect, clip_rect))
      {
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
bool ossimDtedTileSource::fillBuffer(const ossimIrect& tile_rect,
                                     const ossimIrect& clip_rect)
{
   //***
   // NOTE:
   // Elevation posts are organized positive line up.
   // The "tile_rect" and "clip_rect" is organized positive line down.
   //***

   const ossim_int32 TILE_WIDTH = theTile->getWidth();
   
   //***
   // Position the file pointer to the first record needed.
   // Posts are organized positive line up so we're going to start at
   // the lower left side of the cell so all reads are going forward in
   // the file.
   //***
   ossim_int32 seek_position = theOffsetToFirstDataRecord +
                         (clip_rect.ul().x * theDataRecordSize);
   seek_position += DATA_RECORD_OFFSET_TO_POST +
                    (theNumberOfLines - 1 - clip_rect.lr().y) * 2;
   
   //***
   // Get a pointer positioned at the first valid pixel in
   // the tile.  Tiles are organized positive line down.
   //***
   ossim_sint16* d = static_cast<ossim_sint16*>(theTile->getBuf());
   
   // Loop in the longitude or sample direction.
   for (ossim_int32 samp = clip_rect.ul().x; samp <= clip_rect.lr().x; samp++)
   {
      ossim_int32 d_offset = (clip_rect.lr().y - tile_rect.ul().y) *
                       TILE_WIDTH +
                       (samp - tile_rect.ul().x);

      theFileStr.seekg(seek_position, ios::beg); // Seek to the first post.

      for (ossim_int32 line = clip_rect.lr().y; line >= clip_rect.ul().y; line--)
      {
         // Grab all the post needed from the column.
         ossim_uint16 temp;
         ossim_sint16 s;
         theFileStr.read((char*)&temp, POST_SIZE);
         s = convertSignedMagnitude(temp);
         d[d_offset] = s;
         d_offset -= TILE_WIDTH;
      }
      seek_position += theDataRecordSize;
   }


   return true;
}

ossim_uint32 ossimDtedTileSource::getNumberOfOutputBands()const
{
   return getNumberOfInputBands();
}

ossimIrect
ossimDtedTileSource::getImageRectangle(ossim_uint32 reduced_res_level) const
{
   return ossimIrect(0,
                     0,
                     getNumberOfSamples(reduced_res_level) - 1,
                     getNumberOfLines(reduced_res_level)   - 1);
}

bool ossimDtedTileSource::isOpen()const
{
   return theFileStr.is_open();
}

bool ossimDtedTileSource::open()
{
   static const char MODULE[] = "ossimDtedTileSource::open";

   if (traceDebug())
   {
      CLOG << "DEBUG:"
           << "\nAttempting to parse file:  " << theImageFile.c_str()
           << endl;
   }
   
   if (!theImageFile.exists()) // See if file exists.
   {
      if (traceDebug())
      {
         CLOG << "DEBUG:"
              << "\nFile " << theImageFile.c_str() << " does not exist!"
              << endl;
      }
      return false;
   }

   // Open up the file for reading.
   theFileStr.open(theImageFile.c_str(), ios::in | ios::binary);
   if (!theFileStr)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      
      ossimNotify(ossimNotifyLevel_WARN) << MODULE << " ERROR!"
                                         << "\nCould not open file:  " << theImageFile.c_str()
                                         << "\nReturning..." << endl;
      if (traceDebug())  CLOG << "returning with error..." << endl;
      return false;
   }
                                   
   if(!theFileStr)
   {
      if (traceDebug())
      {
         CLOG << "DEBUG:"
              << "\nCannot open:  " << theImageFile.c_str()
              << endl;
      }
      theFileStr.close();
      return false;
   }
                                 
   // Attempt to parse.
   ossimDtedVol vol(theFileStr, 0);
   ossimDtedHdr hdr(theFileStr, vol.stopOffset());
   ossimDtedUhl uhl(theFileStr, hdr.stopOffset());
   ossimDtedDsi dsi(theFileStr, uhl.stopOffset());
   ossimDtedAcc acc(theFileStr, dsi.stopOffset());

   //***
   // Check for errors.  Must have uhl, dsi and acc records.  vol and hdr
   // are for magnetic tape only; hence, may or may not be there.
   //***
   if (uhl.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR ||
       dsi.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR ||
       acc.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR)
   {
      if (traceDebug())
      {
         CLOG << "DEBUG:"
              << "\nError parsing file:  " << theImageFile.c_str()
              << "\nPossibly not a dted file."
              << endl;
      }
      
      theFileStr.close();
      return false;
   }

   // Get the cell specific info needed for later.
   theNumberOfLines = uhl.numLatPoints();
   theNumberOfSamps = uhl.numLonLines();
   thePostSpacing.x = uhl.lonInterval();
   thePostSpacing.y = uhl.latInterval();
   theOffsetToFirstDataRecord = acc.stopOffset();
   theDataRecordSize = DATA_RECORD_OFFSET_TO_POST +
                       (theNumberOfLines * sizeof(ossim_sint16)) +
                       DATA_RECORD_CHECKSUM_SIZE;

   // Scan the file to get the min / max post.
   gatherStatistics();
   
   completeOpen();

   if (traceDebug())
   {
      CLOG << "DEBUG:"
           << "\nNull pix:  " << getNullPixelValue(0)
           << "\nMin  pix:  " << getMinPixelValue(0)
           << "\nMax  pix:  " << getMaxPixelValue(0)
           << "\ntheNumberOfLines:  " << theNumberOfLines
           << "\ntheNumberOfSamps:  " << theNumberOfSamps
           << "\ntheOffsetToFirstDataRecord:  " << theOffsetToFirstDataRecord
           << "\ntheDataRecordSize:  " << theDataRecordSize
           << endl;
   }


   return true;
}

void ossimDtedTileSource::allocate()
{
   //---
   // Set up the tiles.  Note the blank tile will not get initialized to save
   // memory.  This includes setting the min and max pixel value gathered
   // from the statistics.
   //---
   theTile = ossimImageDataFactory::instance()->create(this, this);
   theTile->initialize();

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG:"
         << "\ntheTile:\n" << *theTile
         << endl;
   }
}

bool ossimDtedTileSource::saveState(ossimKeywordlist& kwl,
                                    const char* prefix) const
{
   // Currently nothing to do here.
   return ossimImageHandler::saveState(kwl, prefix);
}

bool ossimDtedTileSource::loadState(const ossimKeywordlist& kwl,
                                    const char* prefix)
{
   if (ossimImageHandler::loadState(kwl, prefix))
   {
      if (ossimImageHandler::open(theImageFile))
      {
         return true;
      }
   }

   return false;
}

bool ossimDtedTileSource::getImageGeometry(ossimKeywordlist& kwl,
                                           const char* prefix)
{
   static const char MODULE[] = "ossimDtedTileSource::getImageGeometry";

   if (traceDebug()) CLOG << " Entered..." << endl;

   // Previously saved...
   if (theGeometryKwl.getSize())
   {
      kwl = theGeometryKwl;
      return true;
   }   
      
   if(!theFileStr)
   {
      if (traceDebug())
      {
         CLOG << "DEBUG:"
              << "\nCannot open:  " << theImageFile.c_str()
              << endl;
      }

      return false;
   }

   // Attempt to parse.
   ossimDtedVol vol(theFileStr, (ossim_int32)0);
   ossimDtedHdr hdr(theFileStr, vol.stopOffset());
   ossimDtedUhl uhl(theFileStr, hdr.stopOffset());
   ossimDtedDsi dsi(theFileStr, uhl.stopOffset());
   ossimDtedAcc acc(theFileStr, dsi.stopOffset());

   //***
   // Check for errors.  Must have uhl, dsi and acc records.  vol and hdr
   // are for magnetic tape only; hence, may or may not be there.
   //***
   if (uhl.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR ||
       dsi.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR ||
       acc.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR)
   {
      if (traceDebug())
      {
         CLOG << "DEBUG:"
              << "\nError parsing file:  " << theImageFile.c_str()
              << "\nPossibly not a dted file."
              << endl;
      }
      return false;
   }

   //***
   // Make an Equidistant Cylindrical projection with a origin at the equator
   // since the DTED post spacing is considered to be square.  Save its
   // state to the keyword list.
   //***
   ossimString projPref = prefix?prefix:"";
//   const ossimDatum* datum = ossimDatumFactory::instance()->wgs84();
//    ossimProjection* proj
//       = new ossimEquDistCylProjection(*(datum->ellipsoid()),
//                                       ossimGpt(0.0, 0.0),
//                                       0.0,   // false easting
//                                       0.0);  // false northing
//    proj->saveState(kwl, projPref);

//    delete proj;
//    proj = NULL;

   //***
   // Get the latitude origin.  DTED uses the South West corner as the origin
   // so the latitude must be shifted to the upper left for the purpose of
   // a tie point.
   //***
   double lat = uhl.latOrigin() +
                (uhl.latInterval() * (uhl.numLatPoints()-1.0));
   
   // Add the tie point.
   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           "ossimEquDistCylProjection",
           true);

   kwl.add(prefix,
           ossimKeywordNames::ORIGIN_LATITUDE_KW,
           0.0,
           true);

   kwl.add(prefix,
           ossimKeywordNames::CENTRAL_MERIDIAN_KW,
           0.0,
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::TIE_POINT_LAT_KW,
           lat,
           true);

   kwl.add(prefix,
           ossimKeywordNames::TIE_POINT_LON_KW,
           uhl.lonOrigin(),
           true);

   // Add the pixel scale.
   kwl.add(prefix,
           ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT,
           uhl.latInterval(),
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON,
           uhl.lonInterval(),
           true);

   //***
   // The projection code will save a meters_per_pixel of 1.0 off so remove it
   // since we want it to use the DECIMAL_DEGREES_PER_PIXEL keyword instead.
   //***
//   kwl.remove(prefix, ossimKeywordNames::METERS_PER_PIXEL_X_KW);
//   kwl.remove(prefix, ossimKeywordNames::METERS_PER_PIXEL_Y_KW);

   // Add the datum.  (always WGS-84 per spec)
   kwl.add(prefix,
           ossimKeywordNames::DATUM_KW,
           ossimDatumFactory::instance()->wgs84()->code(),
           true);

   // Add the number of lines and samples.
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_LINES_KW,
           static_cast<int>(uhl.numLatPoints()));

   kwl.add(prefix,
           ossimKeywordNames::NUMBER_SAMPLES_KW,
           static_cast<int>(uhl.numLonLines()));

   // Add the number of reduced resolution sets.
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_REDUCED_RES_SETS_KW,
           getNumberOfDecimationLevels());

   // Add the number of bands.  (alway one...)
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_INPUT_BANDS_KW,
           1);

   kwl.add(prefix,
           ossimKeywordNames::NUMBER_OUTPUT_BANDS_KW,
           1);

   if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG) << "kwl:\n" << kwl << MODULE << " Exited..." << endl;

   // Capture for next time.
   setImageGeometry(kwl);
   
   return true;
}

ossimScalarType
ossimDtedTileSource::getOutputScalarType() const
{
   return OSSIM_SSHORT16;  // Always signed 16 bit.
}

ossim_uint32 ossimDtedTileSource::getTileWidth() const
{
   return theTileWidth;
}

ossim_uint32 ossimDtedTileSource::getTileHeight() const
{
   return theTileHeight;
}

ossim_uint32 ossimDtedTileSource::getNumberOfLines(ossim_uint32 reduced_res_level) const
{
   if (reduced_res_level == 0)
   {
      return theNumberOfLines;
   }
   else if (theOverview)
   {
      return theOverview->getNumberOfLines(reduced_res_level);
   }

   return 0;
}

ossim_uint32 ossimDtedTileSource::getNumberOfSamples(ossim_uint32 reduced_res_level) const
{
   if (reduced_res_level == 0)
   {
      return theNumberOfSamps;
   }
   else if (theOverview)
   {
      return theOverview->getNumberOfSamples(reduced_res_level);
   }

   return 0;
}

void ossimDtedTileSource::gatherStatistics()
{
   //***
   // Check to see if there is a statistics file already.  If so; do a lookup
   // for the min and max values.
   //***
   ossimFilename stats_file = theImageFile.path();
   stats_file = stats_file.dirCat(theImageFile.fileNoExtension());
   stats_file += ".statistics";
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "Looking for " << stats_file
                                          << " statistics file..." << std::endl;
   }

   ossimKeywordlist kwl;
   const char* min_str = NULL;
   const char* max_str = NULL;

   if (stats_file.exists())
   {
      if (kwl.addFile(stats_file))
      {
         // Look for the min_pixel_value keyword.
         min_str = kwl.find(ossimKeywordNames::MIN_VALUE_KW);
         max_str = kwl.find(ossimKeywordNames::MAX_VALUE_KW);
      }
   }

   if (min_str && max_str)
   {
      theMinHeight = atoi(min_str);
      theMaxHeight = atoi(max_str);
   }
   else  // Scan the cell and gather the statistics...
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimDtedTileSource::gatherStatistics() scanning for min/max"
                                          << "\nThis may take a while..."
                                          << std::endl;
      
      // Start off with the min and max pegged.
      theMinHeight =  32767;
      theMaxHeight = -32767;
      
      // Put the file pointer at the start of the first elevation post.
      theFileStr.seekg(theOffsetToFirstDataRecord, ios::beg);
      
      //***
      // Loop through all records and scan for lowest min and highest max.
      // Each record contains a row of latitude points for a given longitude.
      // There are eight bytes in front of the post and four checksum bytes at
      // the end so ignore them.
      //***
      for (ossim_uint32 i=0; i<theNumberOfSamps; i++)  // longitude direction
      {
         theFileStr.seekg(DATA_RECORD_OFFSET_TO_POST, ios::cur);
         
         for (ossim_uint32 j=0; j<theNumberOfLines; j++) // latitude direction
         {
            ossim_uint16 temp;
            ossim_sint16 s;
            theFileStr.read((char*)&temp, POST_SIZE);
            s = convertSignedMagnitude(temp);
            if (s < theMinHeight && s != NULL_PIXEL) theMinHeight = s;
            if (s > theMaxHeight) theMaxHeight = s;
         }

         theFileStr.seekg(DATA_RECORD_CHECKSUM_SIZE, ios::cur);
      }

      // Add the stats to the keyword list.
      kwl.add(ossimKeywordNames::MIN_VALUE_KW, theMinHeight);
      kwl.add(ossimKeywordNames::MAX_VALUE_KW, theMaxHeight);
      
      // Write out the statistics file.
      kwl.write(stats_file.c_str());
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimDtedTileSource::gatherStatistics DEBUG:"
         << "\ntheMinHeight:  " << theMinHeight
         << "\ntheMaxHeight:  " << theMaxHeight
         << std::endl;
   }
}

ossim_uint32 ossimDtedTileSource::getImageTileWidth() const
{
   return 0;
}

ossim_uint32 ossimDtedTileSource::getImageTileHeight() const
{
   return 0;
}

ossim_uint32 ossimDtedTileSource::getNumberOfInputBands() const
{
   return 1;
}

double ossimDtedTileSource::getNullPixelValue(ossim_uint32 band)const
{
   if(theMetaData.getNumberOfBands())
   {
      return theMetaData.getNullPix(0);
   }
   return -32767.0;
}
double ossimDtedTileSource::getMinPixelValue(ossim_uint32 band)const
{
   if(theMetaData.getNumberOfBands())
   {
      return theMetaData.getMinPix(band);
   }
   return theMinHeight;
}

 double ossimDtedTileSource::getMaxPixelValue(ossim_uint32 band)const
{
   if(theMetaData.getNumberOfBands())
   {
      return theMetaData.getMaxPix(band);
   }
   return theMaxHeight;
}

void ossimDtedTileSource::getPostSpacing(ossimDpt& postSpacing) const
{
   postSpacing.x = thePostSpacing.x;
   postSpacing.y = thePostSpacing.y;
}

const ossimDtedTileSource& ossimDtedTileSource::operator=(const  ossimDtedTileSource& rhs)
{
   return rhs;
} 

ossimDtedTileSource::ossimDtedTileSource(const ossimDtedTileSource&)
{
}

