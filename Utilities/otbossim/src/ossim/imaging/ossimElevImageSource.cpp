//*******************************************************************
// Copyright (C) 2002 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:  Class provides an elevation image source from the elevation
// manager.
// 
//*******************************************************************
//  $Id: ossimElevImageSource.cpp 21631 2012-09-06 18:10:55Z dburken $

#include <ossim/imaging/ossimElevImageSource.h>
#include <ossim/elevation/ossimElevManager.h>
#include <ossim/projection/ossimLlxyProjection.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/imaging/ossimImageData.h>
#include <ctime>

RTTI_DEF1(ossimElevImageSource, "ossimElevImageSource", ossimImageSource)

// Static trace for debugging
static ossimTrace traceDebug("ossimElevImage:debug");
   
ossimElevImageSource::ossimElevImageSource()
   :
      ossimImageSource(NULL,
                       0,
                       0,
                       true,
                       false),// output list is not fixed
      theElevManager(NULL),
      theTile(NULL),
      theTiePoint(),
      theLatSpacing(0.0),
      theLonSpacing(0.0),
      theNumberOfLines(0),
      theNumberOfSamps(0)
{}

ossimElevImageSource::ossimElevImageSource(ossimObject* owner)
   :
      ossimImageSource(owner,
                       0,
                       0,
                       true,
                       false),// output list is not fixed
      theElevManager(NULL),
      theTile(NULL),
      theTiePoint(),
      theLatSpacing(0.0),
      theLonSpacing(0.0),
      theNumberOfLines(0),
      theNumberOfSamps(0)
{}

ossimElevImageSource::ossimElevImageSource(ossimObject* owner,
                                           const ossimGpt& tie,
                                           double latSpacing,
                                           double lonSpacing,
                                           ossim_uint32 numberLines,
                                           ossim_uint32 numberSamples)
   :
      ossimImageSource(owner,
                       0,
                       0,
                       true,
                       false),// output list is not fixed
      theElevManager(NULL),
      theTile(NULL),
      theTiePoint(tie),
      theLatSpacing(latSpacing),
      theLonSpacing(lonSpacing),
      theNumberOfLines(numberLines),
      theNumberOfSamps(numberSamples)
{
   initialize();
}

ossimElevImageSource::ossimElevImageSource(ossimObject* owner,
                                           const ossimKeywordlist& kwl,
                                           const char* prefix)
   :
      ossimImageSource(owner,
                       0,
                       0,
                       true,
                       false),
      theElevManager(NULL),
      theTile(NULL),
      theTiePoint(),
      theLatSpacing(0),
      theLonSpacing(0),
      theNumberOfLines(0),
      theNumberOfSamps(0)
{
   if (loadState(kwl, prefix) == false)
   {
      setErrorStatus();
   }
}

ossimElevImageSource::~ossimElevImageSource()
{
}

ossimRefPtr<ossimImageData> ossimElevImageSource::getTile(
   const  ossimIrect& tile_rect,
   ossim_uint32 resLevel)
{
   if (!theTile.get())
   {
      return theTile;
   }
   
   // First make sure our tile is the right size.
   ossim_int32 w = tile_rect.width();
   ossim_int32 h = tile_rect.height();
   ossim_int32 tileW = theTile->getWidth();
   ossim_int32 tileH = theTile->getHeight();
   if( (w != tileW) || (h != tileH) )
   {
      theTile->setWidth(w);
      theTile->setHeight(h);
      if((w*h)!=(tileW*tileH))
      {
         theTile->initialize();

         //***
         // Initialize can reset the min max to defaults if the min happens
         // to be "0" so reset it just in case.
         // NOTE:  We need to fix initialize!
         //***
         theTile->setMinPix(theElevManager->getMinHeightAboveMSL(), 0);
         theTile->setMaxPix(theElevManager->getMaxHeightAboveMSL(), 0);
      }
   }

   // Set the origin.
   theTile->setOrigin(tile_rect.ul());
      
   
   if(!isSourceEnabled())
   {
      // This tile source bypassed.
      theTile->makeBlank();
      return theTile;
   }

   //***
   // No overview support yet...
   //***
   if (resLevel)
   {
      // NOTE:  Need to add overview support.
      cerr << "ossimElevImageSource::getTile ERROR:\nOverviews not supported!"
           << endl;
      theTile->makeBlank();
      return theTile;
   }

   ossimIrect image_rect = getImageRectangle(0);

   if ( !tile_rect.intersects(image_rect) )
   {
      // No point in the tile falls within the set boundaries of this source.
      theTile->makeBlank();
      return theTile;
   }

   // Ok fill the tile with the data from the post...
   ossimIrect clip_rect = tile_rect.clipToRect(image_rect);

   if ( !tile_rect.completely_within(clip_rect) )
   {
      // Start with a blank tile since it won't be filled all the way.
      theTile->makeBlank();
   }


   // Move the buffer pointer to the first valid pixel.
   ossim_uint32 tile_width = theTile->getWidth();
   
   ossim_int32 start_offset = (clip_rect.lr().y - tile_rect.ul().y) * tile_width +
      clip_rect.ul().x - tile_rect.ul().x;

   //***
   // Since most elevation formats have posts organized positive line up,
   // start at the lower left side of the cell so all reads are going
   // forward in the file.
   //***
   double start_lat = theTiePoint.latd() - theLatSpacing *
      (clip_rect.lr().y - image_rect.ul().y);
   if (start_lat < -90.0)
   {
      start_lat = -(start_lat + 180.0);  // Wrapped around the south poll.
   }

   double lon = theTiePoint.lond() + theLonSpacing *
      (clip_rect.ul().x  - image_rect.ul().x);
   if (lon > 180.0)
   {
      lon -= 360.0; // Went across the central meridian.
   }

   // Copy the data.
   ossim_uint32 clipHeight = clip_rect.height();
   ossim_uint32 clipWidth  = clip_rect.width();

   // Get a pointer to the tile buffer.
   ossim_float32* buf = static_cast<ossim_float32*>(theTile->getBuf());

   for (ossim_uint32 sample = 0; sample < clipWidth; ++sample)
   {
      double lat = start_lat;
      ossim_int32 offset = start_offset;
      for (ossim_uint32 line = 0; line < clipHeight; ++line)
      {
         ossimGpt gpt(lat, lon);
         buf[offset+sample] = theElevManager->getHeightAboveMSL(gpt);
         
         lat += theLatSpacing;
         if (lat > 90) lat = 180.0 - lat;

         offset -= tile_width;
      }
      
      lon += theLonSpacing;
      if (lon > 180.0) lon = lon - 360.0; // Went across the central meridian.
   }
   
#if 0   
   for (ossim_uint32 line = 0; line < clipHeight; ++line)
   {
      double lon = start_lon;
      for (ossim_uint32 sample = 0; sample < clipWidth; ++sample)
      {
         ossimGpt gpt(lat, lon);
         buf[sample] = theElevManager->getHeightAboveMSL(gpt);
         lon += theLonSpacing;
         if (start_lon > 180.0)
         {
            start_lon -= 360.0; // Went across the central meridian.
         }
      }

      buf += tile_width;
      lat -= theLatSpacing;
      if (lat < -90.0) lat = -(lat + 180.0);// Wrapped around the south poll.
   }
#endif
   
   theTile->validate();
   return theTile;
}

bool ossimElevImageSource::saveState(ossimKeywordlist& kwl,
                                  const char* prefix) const
{
   static const char MODULE[] = "ossimElevImageSource::saveState";

   if (kwl.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR)
   {
      cerr << MODULE
           << " ERROR detected in keyword list!  State not saved."
           << endl;
      return false;
   }

   // Save the state of the base class.
   ossimImageSource::saveState(kwl, prefix);

   // Save the tie point.
   kwl.add(prefix,
           ossimKeywordNames::TIE_POINT_LAT_KW,
           theTiePoint.latd(),
           true);
   kwl.add(prefix,
           ossimKeywordNames::TIE_POINT_LON_KW,
           theTiePoint.lond(),
           true);

   // Save the post spacing.
   kwl.add(prefix,
           ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT,
           theLatSpacing,
           true);
   kwl.add(prefix,
           ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON,
           theLonSpacing,
           true);

   // Save the image size
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_LINES_KW,
           theNumberOfLines,
           true);
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_SAMPLES_KW,
           theNumberOfSamps,
           true);

   // Save the min / max pixel values.
   kwl.add(prefix,
           ossimKeywordNames::MIN_VALUE_KW,
           getMinPixelValue(0),
           true);
   kwl.add(prefix,
           ossimKeywordNames::MAX_VALUE_KW,
           getMaxPixelValue(0),
           true);

   return true;
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimElevImageSource::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   static const char MODULE[] = "ossimElevImageSource::loadState";

   if (kwl.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR)
   {
      cerr << MODULE
           << " ERROR detected in keyword list!  State not load."
           << endl;
      return false;
   }

   // Base class...
   ossimImageSource::loadState(kwl, prefix);
   
   const char* lookup;

   // Get the tie point.
   lookup = kwl.find(prefix, ossimKeywordNames::TIE_POINT_LAT_KW);
   if (lookup)
   {
      theTiePoint.latd(ossimString(lookup).toDouble());
   }
   else
   {
      if (traceDebug())
      {
         CLOG << "DEBUG:"
              << "\nRequired keyword not found:  "
              << ossimKeywordNames::TIE_POINT_LAT_KW
              << "\nReturning false"
              << endl;
      }

      return false;
   }
   
   lookup = kwl.find(prefix, ossimKeywordNames::TIE_POINT_LON_KW);
   if (lookup)
   {
      theTiePoint.lond(ossimString(lookup).toDouble());
   }
   else
   {
      if (traceDebug())
      {
         CLOG << "DEBUG:"
              << "\nRequired keyword not found:  "
              << ossimKeywordNames::TIE_POINT_LON_KW
              << "\nReturning false"
              << endl;
      }

      return false;
   }

   // Get the post spacing.
   lookup = kwl.find(prefix, ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT);
   if (lookup)
   {
      theLatSpacing = ossimString(lookup).toDouble();
   }
   else
   {
      if (traceDebug())
      {
         CLOG << "DEBUG:"
              << "\nRequired keyword not found:  "
              << ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT
              << "\nReturning false"
              << endl;
      }

      return false;
   }

   lookup = kwl.find(prefix, ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON);
   if (lookup)
   {
      theLonSpacing = ossimString(lookup).toDouble();
   }
   else
   {
      if (traceDebug())
      {
         CLOG << "DEBUG:"
              << "\nRequired keyword not found:  "
              << ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON
              << "\nReturning false"
              << endl;
      }

      return false;
   }

   // Get the image size.
   lookup = kwl.find(prefix, ossimKeywordNames::NUMBER_LINES_KW);
   if (lookup)
   {
      theNumberOfLines = ossimString(lookup).toULong();
   }
   else
   {
      if (traceDebug())
      {
         CLOG << "DEBUG:"
              << "\nRequired keyword not found:  "
              << ossimKeywordNames::NUMBER_LINES_KW
              << "\nReturning false"
              << endl;
      }

      return false;
   }

   lookup = kwl.find(prefix, ossimKeywordNames::NUMBER_SAMPLES_KW);
   if (lookup)
   {
      theNumberOfSamps = ossimString(lookup).toULong();
   }
   else
   {
      if (traceDebug())
      {
         CLOG << "DEBUG:"
              << "\nRequired keyword not found:  "
              << ossimKeywordNames::NUMBER_SAMPLES_KW
              << "\nReturning false"
              << endl;
      }

      return false;
   }

   initialize();

   //***
   // See if the min / max keyword was set and reset it.
   // Note this must be done after initialize since it sets the min / max from
   // the elevation manager.
   //***
   lookup = kwl.find(prefix, ossimKeywordNames::MIN_VALUE_KW);
   if (lookup)
   {
      setMinPixelValue(ossimString(lookup).toDouble());
   }
   lookup = kwl.find(prefix, ossimKeywordNames::MAX_VALUE_KW);
   if (lookup)
   {
      setMaxPixelValue(ossimString(lookup).toDouble());
   }

   if (getErrorStatus() != ossimErrorCodes::OSSIM_OK)
   {
      return false;
   }

   //***
   // Reset the base class to have a fixed input list of "0" size.
   // Note:  To not do this will result in a core dump destroying objects
   // connected to this output.
   //***
   theInputObjectList.clear();
   theInputListIsFixedFlag = true;
   theOutputListIsFixedFlag = false;
  
   return true;
}

void ossimElevImageSource::initialize()
{
   static const char MODULE[] = "ossimElevImageSource::initialize";
   
   if (traceDebug()) CLOG << " Entered..." << endl;
   
   //***
   // First see if the manager pointer has been captured.
   //***
   if (!theElevManager) theElevManager = ossimElevManager::instance();

   if (!theElevManager)
   {
      setErrorStatus();
      cerr << MODULE << "ERROR:\nNULL elevation manager pointer!"
           << "\nObject not initialized!" << endl;
      return;
   }

   // Basic sanity checks.
   if (!theLatSpacing || !theLonSpacing ||
       !theNumberOfLines || !theNumberOfSamps)
   {
      setErrorStatus();
      cerr << MODULE << "ERROR:"
           << "\nMust set latitude/longitude spacing and number of line and"
           << " samples."
           << "Object not initialized!" << endl;
      return;
      
   }

   // Check the ground point.
   if ( theTiePoint.latd() > 90.0  || theTiePoint.latd() < -90.0  ||
        theTiePoint.lond() > 180.0 || theTiePoint.lond() < -180.0 )
   {
      setErrorStatus();
      cerr << MODULE << "ERROR:\nBogus tie point."
           << "\nObject not initialized!" << endl;
      return;
   }

   if (traceDebug())
   {
      CLOG << "DEBUG:"
           << "\nTie point:  " << theTiePoint
           << "\nLatitude spacing:  " << theLatSpacing
           << "\nLongitude spacing:  " << theLonSpacing
           << "\nLines:              " << theNumberOfLines
           << "\nSamples:            " << theNumberOfSamps
           << endl;
   }
   
   //***
   // Since this will return float data we need to set the min / max values
   // of the data so that anybody who remaps it to eight bit will do it
   // properly.  So scan the entire image rectangle using the manager.
   // This will do two things, force the elevation manager to load all the
   // cells, and at the same time the min / max elevation value will be set.
   // NOTE:
   // ??? Should the elevation manager clear the list of sources prior to
   // the code segment so that the min / max is only from the cells we
   // need!
   //***

   cout << "Initializing elevation manager..." << endl;

#if 0
   // Loop in the longitude or sample direction.
   time_t start_t = time(NULL);
   double lon = theTiePoint.lond();
   for (ossim_uint32 samp = 0; samp < theNumberOfSamps; ++samp)
   {
      double lat = theTiePoint.latd() - theLatSpacing * (theNumberOfLines - 1);
      if (lat < -90.0) lat = -(lat + 180.0); // Wrapped around the south poll.
      
      for (ossim_uint32 line = 0; line < theNumberOfLines; ++line)
      {
         ossimGpt gpt(lat, lon);
         theElevManager->getHeightAboveMSL(gpt);

         lat += theLatSpacing;
         if (lat > 90) lat = 180 - lat; // Went across poll.
      }
      
      lon += theLonSpacing;
      if (lon > 180.0) lon -= 360.0; // Went across the central meridian.
   }
   time_t stop_t = time(NULL);
   cout << "Finished loop two..." << endl;
   cout << "Elapsed time for loop two:  " << (stop_t - start_t) << endl;
#endif
   
   theTile = new ossimImageData(this,
                                OSSIM_FLOAT);
   theTile->initialize();

   // Set the min / max for any normalization down the chain...
   theTile->setMinPix(theElevManager->getMinHeightAboveMSL(), 0);
   theTile->setMaxPix(theElevManager->getMaxHeightAboveMSL(), 0);

   if (traceDebug())
   {
      CLOG << "DEBUG:"
           << "\nMin pix:            " << theTile->getMinPix(0)
           << "\nMax pix:            " << theTile->getMaxPix(0)
           << endl;
   }
}

ossimIrect ossimElevImageSource::getImageRectangle(ossim_uint32 reduced_res_level) const
{
   ossimIrect result(0, 0, theNumberOfSamps-1, theNumberOfLines-1);
   
   if (reduced_res_level != 0)
   {
      cerr << "ossimElevImageSource::getImageRectangle ERROR:"
           << "\nOnly R0 is supported." << endl;
   }

   return result;
}

bool ossimElevImageSource::getImageGeometry(ossimKeywordlist& kwl,
                                            const char* prefix)
{
   // Save off the image dimensions.
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_LINES_KW,
           theNumberOfLines,
           true);
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_SAMPLES_KW,
           theNumberOfLines,
           true);

   // Save off the projection info (tie and post spacing).
   ossimLlxyProjection proj(theTiePoint, theLatSpacing, theLonSpacing);
   return proj.saveState(kwl, prefix);
}

void ossimElevImageSource::getDecimationFactor(ossim_uint32 resLevel, ossimDpt& result) const
{
   if (resLevel)
   {
      cerr << "ossimElevImageSource::getDecimationFactor ERROR:"
           << "\nReduced res sets currently not supported!"
           << endl;
   }

   result.line = 1.0;
   result.samp = 1.0;
}

void ossimElevImageSource::getDecimationFactors(vector<ossimDpt>& decimations) const
{
   ossimDpt pt(1.0, 1.0);
   decimations.clear();
   decimations.push_back(pt);
}

ossim_uint32 ossimElevImageSource::getNumberOfDecimationLevels()const
{
   return 1;
}

ossim_uint32 ossimElevImageSource::getNumberOfInputBands() const
{
   return 1;
}

ossimScalarType ossimElevImageSource::getOutputScalarType() const
{
   return OSSIM_FLOAT;
}

ossim_uint32 ossimElevImageSource::getTileWidth() const
{
   if (theTile.get()) return theTile->getWidth();

   return 0;
}

ossim_uint32 ossimElevImageSource::getTileHeight() const
{
   if (theTile.get()) return theTile->getHeight();

   return 0;
}

void ossimElevImageSource::changeTileSize(const ossimIpt& size)
{
   if (traceDebug())
   {
      cout << "ossimElevImageSource::changeTileSize DEBUG:"
           << "\nx size:  " << size.x
           << "\ny size:  " << size.y
           << endl;
   }

   if (!theTile)
   {
      cerr << "ossimElevImageSource::changeTileSize ERROR:"
           << "\nObject not initialized!  Returning..." << endl;
      return;
   }

   theTile = new ossimImageData(this,
                                OSSIM_FLOAT,
                                1,
                                size.x,
                                size.y);
   theTile->initialize();
   
   // Set the min / max for any normalization down the chain...
   theTile->setMinPix(theElevManager->getMinHeightAboveMSL(), 0);
   theTile->setMaxPix(theElevManager->getMaxHeightAboveMSL(), 0);
}

ossim_uint32 ossimElevImageSource::getNumberOfLines(ossim_uint32 reduced_res_level) const
{
   if (reduced_res_level)
   {
      cerr << "ossimElevImageSource::getNumberOfLines ERROR:"
           << "\nReduced res sets currently not supported!"
           << endl;
      return 0;
   }

   return theNumberOfLines;
}

ossim_uint32 ossimElevImageSource::getNumberOfSamples(ossim_uint32 reduced_res_level) const
{
   if (reduced_res_level)
   {
      cerr << "ossimElevImageSource::getNumberOfSamples ERROR:"
           << "\nReduced res sets currently not supported!"
           << endl;
      return 0;
   }

   return theNumberOfSamps;
}

double ossimElevImageSource::getMinPixelValue(ossim_uint32 band) const
{
   if (band)
   {
      cerr << "ossimElevImageSource::getMinPixelValue ERROR:"
           << "\nReduced res sets currently not supported!"
           << endl;
      return 0.0;
   }

   if (theTile.get())
   {
      return theTile->getMinPix(0);
   }
   else if (theElevManager)
   {
      return theElevManager->getMinHeightAboveMSL();
   }

   return 0.0;
}

double ossimElevImageSource::getMaxPixelValue(ossim_uint32 band) const
{
   if (band)
   {
      cerr << "ossimElevImageSource::getMaxPixelValue ERROR:"
           << "\nReduced res sets currently not supported!"
           << endl;
      return 0.0;
   }

   if (theTile.get())
   {
      return theTile->getMaxPix(0);
   }
   else if (theElevManager)
   {
      return theElevManager->getMaxHeightAboveMSL();
   }

   return 0.0;
}

void ossimElevImageSource::setMinPixelValue(ossim_float64 min_pix)
{
   if (theTile.get()) 
   {
      theTile->setMinPix(min_pix, 0);
      if (traceDebug())
      {
         cout << "ossimElevImageSource::setMinPixelValue DEBUG:"
              << "\nMin pixel value:  " << min_pix
              << endl;
      }
   }
   else
   {
      cerr << "ossimElevImageSource::setMinPixelValue ERROR:"
           << "\nObject not initialized!"
           << endl;
   }
}

void ossimElevImageSource::setMaxPixelValue(ossim_float64 max_pix)
{
   if (theTile.get()) 
   {
      theTile->setMaxPix(max_pix, 0);
      if (traceDebug())
      {
         cout << "ossimElevImageSource::setMaxPixelValue DEBUG:"
              << "\nMax pixel value:  " << max_pix
              << endl;
      }
   }
   else
   {
      cerr << "ossimElevImageSource::setMinPixelValue ERROR:"
           << "\nObject not initialized!"
           << endl;
   }
}      
