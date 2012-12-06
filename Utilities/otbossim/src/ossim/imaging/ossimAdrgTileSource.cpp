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
// $Id: ossimAdrgTileSource.cpp 21631 2012-09-06 18:10:55Z dburken $

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
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimContainerProperty.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/imaging/ossimU8ImageData.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <iostream>

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
      m_Tile(0),
      m_TileBuffer(0),
      m_FileStr(),
      m_AdrgHeader(0)
{
   // Construction not complete.  Users should call "open" method.
}

ossimAdrgTileSource::~ossimAdrgTileSource()
{
   if(m_AdrgHeader)
   {
      delete m_AdrgHeader;
      m_AdrgHeader = 0;
   }
   if (m_TileBuffer)
   {
      delete [] m_TileBuffer;
      m_TileBuffer = 0;
   }

   close();
}

ossimRefPtr<ossimImageData> ossimAdrgTileSource::getTile(
   const ossimIrect& rect,
   ossim_uint32 resLevel)
{
   if (m_Tile.valid())
   {
      // Image rectangle must be set prior to calling getTile.
      m_Tile->setImageRectangle(rect);
      
      if ( getTile( m_Tile.get(), resLevel ) == false )
      {
         if (m_Tile->getDataObjectStatus() != OSSIM_NULL)
         {
            m_Tile->makeBlank();
         }
      }
   }
   
   return m_Tile;
}

bool ossimAdrgTileSource::getTile(ossimImageData* result,
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
      result->ref();  // Increment ref count.

      //---
      // Check for overview tile.  Some overviews can contain r0 so always
      // call even if resLevel is 0.  Method returns true on success, false
      // on error.
      //---
      status = getOverviewTile(resLevel, result);
      
      if (!status) // Did not get an overview tile.
      {
         status = true;
         
         ossimIrect tile_rect = result->getImageRectangle();
         
         ossimIrect image_rect = getImageRectangle(resLevel);
         
         result->makeBlank();
         
         //---
         // See if any point of the requested tile is in the image.
         //---
         if ( tile_rect.intersects(image_rect) )
         {
            ossimIrect clip_rect = tile_rect.clipToRect(image_rect);
            
            // Load the tile buffer with data from the adrg.
            status = fillBuffer(tile_rect, clip_rect, result);

            if (status)
            {
               result->validate();
            }
         }
      }

      result->unref();  // Decrement ref count.
   }

   return status;
}

//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimAdrgTileSource::fillBuffer(const ossimIrect& /* tile_rect */,
                                     const ossimIrect& clip_rect,
                                     ossimImageData* tile)
{
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
   
   ossim_int32 tiles_in_x_dir = size_in_x / ADRG_TILE_WIDTH  +
      (size_in_x % ADRG_TILE_WIDTH  ? 1 : 0);
   ossim_int32 tiles_in_y_dir = size_in_y / ADRG_TILE_HEIGHT +
      (size_in_y % ADRG_TILE_HEIGHT ? 1 : 0);


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
            
            //---
            // Some point in the chip intersect the tile so grab the
            // data.
            //---
            ossim_int32 row = (ossim_int32) ulTilePt.y / ADRG_TILE_HEIGHT; 
            ossim_int32 col = (ossim_int32) ulTilePt.x / ADRG_TILE_WIDTH;
            ossim_int32 tileOffset = m_AdrgHeader->tim(row, col);

            if(tileOffset != 0)
            {
               // Get the data.
               int seek_position = (tileOffset - 1) * 49152 + 2048;
               int band;

               // seek to start of chip
               m_FileStr.seekg(seek_position, ios::beg);
               for (band=0; band<3; band++)
               {
                  //***
                  // Read the chip from the ccf file into the chunk buffer.
                  // This will get all the bands.  Bands are interleaved by
                  // chip.
                  //***
                  if (!m_FileStr.read((char*)m_TileBuffer,
                                       ADRG_TILE_SIZE))
                  {
                     theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
                     return false;
                  }

                  tile->loadBand(m_TileBuffer,
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
   if(m_AdrgHeader)
   {
      delete m_AdrgHeader;
      m_AdrgHeader = 0;
   }
   if(m_FileStr.is_open())
   {
      m_FileStr.close();
   }
   m_Tile = 0;
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
   if(m_AdrgHeader)
   {
      delete m_AdrgHeader;
      m_AdrgHeader = 0;
   }
   // Instantiate support data class to parse header file.
   m_AdrgHeader = new ossimAdrgHeader(theImageFile);

   // Check for errors.
   if (m_AdrgHeader->errorStatus() == ossimErrorCodes::OSSIM_ERROR)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " Error in ossimAdrg header detected." << std::endl;
      }

      close();
      return false;
   }

   m_FileStr.open(m_AdrgHeader->imageFile().c_str(),
                   ios::in | ios::binary);

   // Check the file pointer.
   if(!m_FileStr)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << MODULE << "\nCannot open:  "
              << m_AdrgHeader->imageFile().c_str() << std::endl;
      }
      close();
      
      return false;
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE
         << "File is opened -> "<< m_AdrgHeader->imageFile()<<std::endl;
   }

   // allow the base handler to check for other overrides
   completeOpen();
   // Allocate memory.
   m_Tile      = ossimImageDataFactory::instance()->create(this, this);
   m_Tile->initialize();
   m_TileBuffer  = new ossim_uint8[ADRG_TILE_SIZE];

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\nminLon:   " << m_AdrgHeader->minLon()
         << "\nminLond:  " << m_AdrgHeader->minLongitude() 
         << "\nminLat:   " << m_AdrgHeader->minLat()
         << "\nminLatd:  " << m_AdrgHeader->minLatitude()
         << "\nmaxLon:   " << m_AdrgHeader->maxLon()
         << "\nmaxLond:  " << m_AdrgHeader->maxLongitude()
         << "\nmaxLat:   " << m_AdrgHeader->maxLat()
         << "\nmaxLatd:  " << m_AdrgHeader->maxLatitude()
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
ossimRefPtr<ossimImageGeometry> ossimAdrgTileSource::getImageGeometry()
{
   if ( !theGeometry )
   {
      // Check for external geom:
      theGeometry = getExternalImageGeometry();
      
      if ( !theGeometry )
      {
         // origin of latitude
         ossim_float64 originLatitude = (m_AdrgHeader->maxLatitude() +
                                         m_AdrgHeader->minLatitude()) / 2.0;
         
         // central meridian.
         ossim_float64 centralMeridian = (m_AdrgHeader->maxLongitude() +
                                          m_AdrgHeader->minLongitude()) / 2.0;
         
         //---
         // Compute the pixel size in latitude and longitude direction.  This will
         // be full image extents divided by full image lines and samples.
         //---
         
         // Samples in full image (used to compute degPerPixelX).
         ossim_float64 samples = m_AdrgHeader->samples();
         
         // Lines in full image (used to compute degPerPixelX).
         ossim_float64 lines = m_AdrgHeader->lines();
         
         // Degrees in latitude direction of the full image.
         ossim_float64 degrees_in_lat_dir = m_AdrgHeader->maxLatitude() -
            m_AdrgHeader->minLatitude();
         
         // Degrees in longitude direction of the full image.
         ossim_float64 degrees_in_lon_dir = m_AdrgHeader->maxLongitude() -
            m_AdrgHeader->minLongitude();
         
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
         ossim_float64 ul_lat = (m_AdrgHeader->maxLatitude() - 
                                 ( (m_AdrgHeader->startRow() - 1) *
                                   degPerPixelY ) - ( degPerPixelY * 0.5 ) );
         ossim_float64 ul_lon = (m_AdrgHeader->minLongitude() +
                                 ( (m_AdrgHeader->startCol() -1) *
                                   degPerPixelX ) +  ( degPerPixelX * 0.5 ) );
         
         // projection type
         ossimKeywordlist kwl;
         const char* prefix = 0;
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
               << "\nminLon:             " << m_AdrgHeader->minLon()
               << "\nminLond:            " << m_AdrgHeader->minLongitude() 
               << "\nminLat:             " << m_AdrgHeader->minLat()
               << "\nminLatd:            " << m_AdrgHeader->minLatitude()
               << "\nmaxLon:             " << m_AdrgHeader->maxLon()
               << "\nmaxLond:            " << m_AdrgHeader->maxLongitude()
               << "\nmaxLat:             " << m_AdrgHeader->maxLat()
               << "\nmaxLatd:            " << m_AdrgHeader->maxLatitude()
               << "\nstartRow:           " << m_AdrgHeader->startRow()
               << "\nstartCol:           " << m_AdrgHeader->startCol()
               << "\nstopRow:            " << m_AdrgHeader->stopRow()
               << "\nstopCol:            " << m_AdrgHeader->stopCol()
               << "\nfull image lines:   " << lines
               << "\nfull image samples: " << samples
               << "\nkwl:\n"               << kwl
               << std::endl;
         }

         ossimProjection* new_proj = ossimProjectionFactoryRegistry::instance()->createProjection(kwl);
         theGeometry = new ossimImageGeometry;
         theGeometry->setProjection(new_proj);  // assumes management of projection instance
         
      } // matches (after getExternalImageGeometry()):  if ( !theGeometry ) 
      
      // Set image things the geometry object should know about.
      initImageParameters( theGeometry.get() );
      
   } // matches: if ( !theGeometry )

   return theGeometry;
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
   return ( m_Tile.valid() ? m_Tile->getWidth() : 0 );
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimAdrgTileSource::getTileHeight() const
{
   return ( m_Tile.valid() ? m_Tile->getHeight() : 0 );
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimAdrgTileSource::getNumberOfLines(ossim_uint32 reduced_res_level) const
{
   if ( (reduced_res_level == 0) && m_AdrgHeader )
   {
      return (m_AdrgHeader->stopRow() - m_AdrgHeader->startRow()) + 1;
   }
   else if (theOverview.valid())
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
   if ( (reduced_res_level == 0) && m_AdrgHeader )
   {
      return (m_AdrgHeader->stopCol() - m_AdrgHeader->startCol()) + 1;
   }
   else if (theOverview.valid())
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

ossimRefPtr<ossimProperty> ossimAdrgTileSource::getProperty(const ossimString& name)const
{
	if(name == "file_type")
	{
		return new ossimStringProperty(name, "ADRG");
	}
	
	return ossimImageHandler::getProperty(name);
}

void ossimAdrgTileSource::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
	ossimImageHandler::getPropertyNames(propertyNames);
	propertyNames.push_back("file_type");
}

ossimString ossimAdrgTileSource::getShortName()const
{
   return ossimString("adrg");
}

ossimString ossimAdrgTileSource::getLongName()const
{
   return ossimString("adrg reader");
}

ossim_uint32 ossimAdrgTileSource::getNumberOfInputBands() const
{
   return m_AdrgHeader->numberOfBands();
}

bool ossimAdrgTileSource::isOpen()const
{
   return (m_AdrgHeader!=0);
}

void ossimAdrgTileSource::adjustToStartOfTile(ossimIpt& pt) const
{
   pt.x &= 0xffffff80;
   pt.y &= 0xffffff80;
}
