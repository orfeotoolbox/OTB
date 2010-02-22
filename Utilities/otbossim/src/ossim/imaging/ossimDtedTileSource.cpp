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
// Contains class declaration for ossimDtedTileSource.
//
//********************************************************************
// $Id: ossimDtedTileSource.cpp 16283 2010-01-06 21:25:19Z dburken $

#include <cstdlib>
#include <iostream>
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
#include <ossim/support_data/ossimDtedInfo.h>

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
   if (ossim::byteOrder() == OSSIM_LITTLE_ENDIAN)
   {
      theSwapBytesFlag = true;
   }

   // Set the tile size to the default.
   ossimIpt defaultTileSize;
   ossim::defaultTileSize(defaultTileSize);
   theTileWidth  = static_cast<ossim_uint32>(defaultTileSize.x);
   theTileHeight = static_cast<ossim_uint32>(defaultTileSize.y);
   
   // Construction not complete.  Users should call "open" method.
}

ossimDtedTileSource::~ossimDtedTileSource()
{
   close();
}

ossimString ossimDtedTileSource::getShortName()const
{
   return ossimString("dted");
}

ossimString ossimDtedTileSource::getLongName()const
{
   return ossimString("dted reader");
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

   // Image rectangle must be set prior to calling getTile.
   theTile->setImageRectangle(tile_rect);
   
   if ( getTile( theTile.get(), resLevel ) == false )
   {
      if (theTile->getDataObjectStatus() != OSSIM_NULL)
      {
         theTile->makeBlank();
      }
   }

   return theTile;
}

bool ossimDtedTileSource::getTile(ossimImageData* result,
                                  ossim_uint32 resLevel)
{
   bool status = false;
   
   //---
   // Not open, this tile source bypassed, or invalid res level,
   // return a blank tile.
   //---
   if( isOpen() && isSourceEnabled() && isValidRLevel(resLevel) &&
       result && (result->getNumberOfBands() == getNumberOfOutputBands()) )
   {
      result->ref(); // Increment ref count.

      //---
      // Check for overview tile.  Some overviews can contain r0 so always
      // call even if resLevel is 0.  Method returns true on success, false
      // on error.
      //---
      status = getOverviewTile(resLevel, result);

      if ( !status )  // Did not get an overview tile.
      {  
         ossimIrect image_rect = getImageRectangle(resLevel);

         ossimIrect tile_rect = result->getImageRectangle();
         
         //---
         // See if any point of the requested tile is in the image.
         //---
         if ( tile_rect.intersects(image_rect) )
         {
            ossimIrect clip_rect = tile_rect.clipToRect(image_rect);
            
            if ( !tile_rect.completely_within(clip_rect) )
            {
               // Start with a blank tile.
               result->makeBlank();
            }

            // Load the tile buffer with data from the dted cell.
            status = fillBuffer(tile_rect, clip_rect, result);
            
         } // End of if ( tile_rect.intersects(image_rect) )
      }

       result->unref(); // Decrement ref count.
   }
   
   return status;
}

//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimDtedTileSource::fillBuffer(const ossimIrect& tile_rect,
                                     const ossimIrect& clip_rect,
                                     ossimImageData* tile)
{
   //***
   // NOTE:
   // Elevation posts are organized positive line up.
   // The "tile_rect" and "clip_rect" is organized positive line down.
   //***

   const ossim_int32 TILE_WIDTH = tile->getWidth();
   
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
   ossim_sint16* d = static_cast<ossim_sint16*>(tile->getBuf());
   
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

   tile->validate();
   
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

void ossimDtedTileSource::close()
{
   theFileStr.close();
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
      
	  if(traceDebug())
	  {
		  ossimNotify(ossimNotifyLevel_WARN) << MODULE << " ERROR!"
											 << "\nCould not open file:  " << theImageFile.c_str()
											 << "\nReturning..." << endl;
		  CLOG << "returning with error..." << endl;
	  }
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
   m_vol.parse(theFileStr);
   m_hdr.parse(theFileStr);
   m_uhl.parse(theFileStr);
   m_dsi.parse(theFileStr);
   m_acc.parse(theFileStr);
   
   //***
   // Check for errors.  Must have uhl, dsi and acc records.  vol and hdr
   // are for magnetic tape only; hence, may or may not be there.
   //***
   if (m_uhl.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR ||
       m_dsi.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR ||
       m_acc.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR)
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
   theNumberOfLines = m_uhl.numLatPoints();
   theNumberOfSamps = m_uhl.numLonLines();
   thePostSpacing.x = m_uhl.lonInterval();
   thePostSpacing.y = m_uhl.latInterval();
   theOffsetToFirstDataRecord = m_acc.stopOffset();
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
   if(theTile.valid())
   {
      theTile->initialize();
   }

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

//**************************************************************************************************
// Returns the image geometry object associated with this tile source or NULL if not defined.
// The geometry contains full-to-local image transform as well as projection (image-to-world)
//**************************************************************************************************
ossimImageGeometry* ossimDtedTileSource::getImageGeometry()
{
   static const char* MODULE = "ossimDtedTileSource::getImageGeometry() -- ";

   if (theGeometry.valid())
      return theGeometry.get();

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

   //***
   // Check for errors.  Must have uhl, dsi and acc records.  vol and hdr
   // are for magnetic tape only; hence, may or may not be there.
   //***
   if (m_uhl.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR ||
       m_dsi.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR ||
       m_acc.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR)
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
//   ossimString projPref = prefix?prefix:"";
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
   double lat = m_uhl.latOrigin() +
                (m_uhl.latInterval() * (m_uhl.numLatPoints()-1.0));
   
   // Add the tie point.
   ossimKeywordlist kwl;
   const char* prefix = 0; // legacy
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
           m_uhl.lonOrigin(),
           true);

   // Add the pixel scale.
   kwl.add(prefix,
           ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT,
           m_uhl.latInterval(),
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON,
           m_uhl.lonInterval(),
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
           static_cast<int>(m_uhl.numLatPoints()));

   kwl.add(prefix,
           ossimKeywordNames::NUMBER_SAMPLES_KW,
           static_cast<int>(m_uhl.numLonLines()));

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

   // Capture this for next time.
   theGeometry = new ossimImageGeometry;
   theGeometry->loadState(kwl, prefix);
   return theGeometry.get();
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
   else if (theOverview.valid())
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
   else if (theOverview.valid())
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

ossimRefPtr<ossimProperty> ossimDtedTileSource::getProperty(
   const ossimString& name)const
{
   // look in base class.
   ossimRefPtr<ossimProperty> result = ossimImageHandler::getProperty(name);

   if (result.valid() == false)
   {
      ossimDtedInfo info;
      if (info.open(theImageFile))
      {
         result = info.getProperty(name);
      }
   }

   return result;
}

void ossimDtedTileSource::getPropertyNames(
   std::vector<ossimString>& propertyNames) const
{
   ossimImageHandler::getPropertyNames(propertyNames);

   ossimDtedInfo info;
   if (info.open(theImageFile))
   {
      info.getPropertyNames(propertyNames);
   }
}

const ossimDtedTileSource& ossimDtedTileSource::operator=(const  ossimDtedTileSource& rhs)
{
   return rhs;
} 

ossimDtedTileSource::ossimDtedTileSource(const ossimDtedTileSource&)
{
}

