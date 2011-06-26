//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description:
//
// Contains class declaration for ossimUsgsDemTileSource.
//
//********************************************************************
// $Id: ossimUsgsDemTileSource.cpp 19640 2011-05-25 15:58:00Z oscarkramer $

#include <iostream>
#include <fstream>
#include <ossim/imaging/ossimUsgsDemTileSource.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/support_data/ossimDemHeader.h>
#include <ossim/support_data/ossimDemProfile.h>
#include <ossim/support_data/ossimDemGrid.h>
#include <ossim/support_data/ossimDemUtil.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/imaging/ossimImageGeometryRegistry.h>

RTTI_DEF1(ossimUsgsDemTileSource, "ossimUsgsDemTileSource", ossimImageHandler)

// Static trace for debugging
static ossimTrace traceDebug("ossimUsgsDemTileSource:debug");

static const char DEM_TYPE_KW[] = "dem_type";
static const char USGS_DEM_KW[] = "usgs_dem";


ossimUsgsDemTileSource::ossimUsgsDemTileSource()
   :
      ossimImageHandler(),
      theDem(0),
      theTile(0),
      theNullValue(0.0),
      theMinHeight(0.0),
      theMaxHeight(0.0),
      theFeetFlag(false),
      theIsDemFlag(false),
      theScalarType(OSSIM_SINT16)
{
   // Construction not complete.  Users should call "open" method.
}

ossimUsgsDemTileSource::~ossimUsgsDemTileSource()
{
   if (theDem)
   {
      delete theDem;
      theDem = 0;
   }
   theTile = 0;
}

ossimRefPtr<ossimImageData> ossimUsgsDemTileSource::getTile(
   const  ossimIrect& tile_rect, ossim_uint32 resLevel)
{
   if (theTile.valid())
   {
      // Image rectangle must be set prior to calling getTile.
      theTile->setImageRectangle(tile_rect);
      
      if ( getTile( theTile.get(), resLevel ) == false )
      {
         if (theTile->getDataObjectStatus() != OSSIM_NULL)
         {
            theTile->makeBlank();
         }
      }
   }
   
   return theTile;
}

bool ossimUsgsDemTileSource::getTile(ossimImageData* result,
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
      
      if (status) // From old code.  Not sure if still needed.
      {
         result->setMinPix(theTile->getMinPix(), theTile->getNumberOfBands());
         result->setMaxPix(theTile->getMaxPix(), theTile->getNumberOfBands());
         result->setNullPix(theTile->getNullPix(), theTile->getNumberOfBands());
      }
      
      if (!status) // Did not get an overview tile.
      {
         status = true;
         
         ossimIrect tile_rect = result->getImageRectangle();
         
         ossimIrect image_rect = getImageRectangle(resLevel);
         
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
            
            // Load the tile buffer with data from the dem cell.
            if (theScalarType == OSSIM_FLOAT32)
            {
               status = fillBuffer(ossim_float32(0.0),
                                   tile_rect,
                                   clip_rect,
                                   result);
            }
            else
            {
               status = fillBuffer(ossim_sint16(0),
                                   tile_rect,
                                   clip_rect,
                                   result);
            }
            if (status == true)
            {
               result->validate();
            }
         }    
         else // No intersection:  if ( tile_rect.intersects(image_rect) )
         {
            result->makeBlank();
         }
      }

      result->unref(); // Decrement ref count.
   }

   return status;
}

//*******************************************************************
// Private Method:
//*******************************************************************
template <class T>
bool ossimUsgsDemTileSource::fillBuffer(T,
                                        const ossimIrect& tile_rect,
                                        const ossimIrect& clip_rect,
                                        ossimImageData* tile)
{
   const uint32 TILE_WIDTH = tile->getWidth();

   T* d = static_cast<T*>(tile->getBuf());
   
   ossim_float32 spatialResZ = theDem->getHeader().getSpatialResZ();
   if (spatialResZ == 0.0) spatialResZ = 1.0;

   // Compute the starting offset.
   uint32 offset = (clip_rect.ul().y - tile_rect.ul().y) * TILE_WIDTH +
                   clip_rect.ul().x - tile_rect.ul().x;

   for (int32 line = clip_rect.ul().y; line <= clip_rect.lr().y; ++line)
   {
      // Loop in the longitude or sample direction.
      int samp_offset = 0;
      for (int32 samp = clip_rect.ul().x; samp <= clip_rect.lr().x; ++samp)
      {
         ossim_float32 p = theDem->getElevation(samp, line);
         if (p != theNullValue)
         {
            p *= spatialResZ;
            if (theFeetFlag) p = p * MTRS_PER_FT;
         }
         d[offset + samp_offset] = static_cast<T>(p);
         ++samp_offset;
      }
      offset += TILE_WIDTH;
   }

   return true;
}

ossimIrect
ossimUsgsDemTileSource::getImageRectangle(ossim_uint32 reduced_res_level) const
{
   return ossimIrect(0,
                     0,
                     getNumberOfSamples(reduced_res_level) - 1,
                     getNumberOfLines(reduced_res_level)   - 1);
}

bool ossimUsgsDemTileSource::open()
{
   static const char MODULE[] = "ossimUsgsDemTileSource::open";

   if (traceDebug())
   {
      CLOG << "DEBUG:"
           << "\nAttempting to parse file:  " << theImageFile.c_str()
           << std::endl;
   }

   theIsDemFlag = ossimDemUtil::isUsgsDem(theImageFile);

   if (theIsDemFlag)
   {
      // Open up the file for reading.
      std::ifstream is(theImageFile.c_str(),
                       std::ios_base::in | std::ios_base::binary);
      if (is.good())
      {
         // Start out with a fresh dem.
         if (theDem) delete theDem;
         
         //---
         // Set the null to -32768.  This will also be the missing data values.
         //---
         theNullValue = OSSIM_DEFAULT_NULL_PIX_SINT16;
         
         //---
         // Read the dem.
         // 
         // NOTE:  This defines the missing data value.  It should be the
         // same as null for mosaicing and min/max calculations.
         //---
         theDem = new ossimDemGrid(theNullValue);
         theDem->read(is);
         
         is.close();
      }
      else
      {
         theIsDemFlag = false;
      }
   }
   
   if (theIsDemFlag)
   {
      theFeetFlag  = (theDem->getHeader().getElevationUnits() == 1) ?
         true : false;
      
      theMinHeight = theFeetFlag ? theDem->getHeader().getMinimumElev() *
         MTRS_PER_FT : theDem->getHeader().getMinimumElev();
      theMaxHeight = theFeetFlag ? theDem->getHeader().getMaximumElev() *
         MTRS_PER_FT : theDem->getHeader().getMaximumElev();
      
      completeOpen();
      
      //---
      // Set up the tiles.  Note the blank tile will not get initialized to
      // save memory.  This includes setting the min and max pixel value
      // gathered from the statistics.
      //---
      theTile = ossimImageDataFactory::instance()->create(this, this);
      theTile->initialize();
      
      if (traceDebug())
      {
         CLOG << setiosflags(ios::fixed) << setprecision(5) << "DEBUG:"
              << "\nNull pix:  " << (theTile->getNullPix(0))
              << "\nMin  pix:  " << (theTile->getMinPix(0))
              << "\nMax  pix:  " << (theTile->getMaxPix(0))
              << "\nlines:     " << theDem->getHeight()
              << "\nsamples:   " << theDem->getWidth()
              << std::endl;
      }
   }

   return theIsDemFlag;
}

bool ossimUsgsDemTileSource::saveState(ossimKeywordlist& kwl,
                                       const char* prefix) const
{
   // Save of the dem type keyword.
   kwl.add(prefix, DEM_TYPE_KW, USGS_DEM_KW, true);

   // Save the scalar type.
   kwl.add(prefix,
           ossimKeywordNames::SCALAR_TYPE_KW,
           ossimScalarTypeLut::instance()->getEntryString(theScalarType));
   
   return ossimImageHandler::saveState(kwl, prefix);
}

bool ossimUsgsDemTileSource::loadState(const ossimKeywordlist& kwl,
                                       const char* prefix)
{
   if (ossimImageHandler::loadState(kwl, prefix))
   {
      //***
      // Look for "dem_type: usgs_dem" key value pair.
      // Note:  If not in the keyword list the "open" will look for it on
      //        disk.
      //***
      const char* lookup = kwl.find(prefix, DEM_TYPE_KW);
      if (lookup)
      {
         ossimString s = lookup;
         s.downcase();
         if (s == USGS_DEM_KW) theIsDemFlag = true;
      }

      //---
      // Look for scalar type override.
      //
      // Note: We only allow float or signed 16 bit.
      //---
      lookup = kwl.find(prefix, ossimKeywordNames::SCALAR_TYPE_KW);
      if (lookup)
      {
         ossimScalarType st =
            static_cast<ossimScalarType>(ossimScalarTypeLut::instance()->
                                         getEntryNumber(lookup));
         if ( (st == OSSIM_FLOAT32) || (st == OSSIM_SINT16))
         {
            theScalarType = st;
         }
         else
         {
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "ossimUsgsDemTileSource::loadState WARNING:"
                  << "\nInvalid scalar type: "
                  << ossimScalarTypeLut::instance()->
                  getEntryString(st)
                  << std::endl;
            }
         }
      }

      if (open())
      {
         return true;
      }
   }

   return false;
}

ossimRefPtr<ossimImageGeometry> ossimUsgsDemTileSource::getImageGeometry()
{
   if ( !theGeometry )
   {
      // Check for external geom:
      theGeometry = getExternalImageGeometry();

      if ( !theGeometry )
      {
         // Check the internal geometry first to avoid a factory call.
         theGeometry = getInternalImageGeometry();

         // At this point it is assured theGeometry is set.

         //---
         // WARNING:
         // Must create/set theGeometry at this point or the next call to
         // ossimImageGeometryRegistry::extendGeometry will put us in an infinite loop
         // as it does a recursive call back to ossimImageHandler::getImageGeometry().
         //---         

         // Check for set projection.
         if ( !theGeometry->getProjection() )
         {
            // Try factories for projection.
            ossimImageGeometryRegistry::instance()->extendGeometry(this);
         }
      }

      // Set image things the geometry object should know about.
      initImageParameters( theGeometry.get() );
   }

   return theGeometry;
}


ossimRefPtr<ossimImageGeometry> ossimUsgsDemTileSource::getInternalImageGeometry() const
{
   ossimRefPtr<ossimImageGeometry> geom = new ossimImageGeometry();
   
   if (theDem)
   {
      const ossimDemHeader HDR = theDem->getHeader();
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimUsgsDemTileSource::getInternalImageGeometry DEBUG:"
            << "\nDEM Header:"
            << std::endl;
         HDR.print(ossimNotify(ossimNotifyLevel_DEBUG));
      }
      
      // The DEM's image geometry is a map projection, obtained here via KWL:
      ossimKeywordlist proj_kwl;
      if ( HDR.getImageGeometry(proj_kwl) ) 
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimUsgsDemTileSource::getInternalImageGeometry DEBUG:"
               << "keyword list:\n" << proj_kwl
               << std::endl;
         }
         
         // Capture for next time.
         ossimRefPtr<ossimProjection> proj =
            ossimProjectionFactoryRegistry::instance()->
            createProjection(proj_kwl);
         if ( proj.valid() )
         {
            geom->setProjection(proj.get());
         }
      }
   }
   
   return geom;
}

ossimScalarType ossimUsgsDemTileSource::getOutputScalarType() const
{
   return theScalarType;
}

ossim_uint32 ossimUsgsDemTileSource::getTileWidth() const
{
   return ( theTile.valid() ? theTile->getWidth() : 0 );
}

ossim_uint32 ossimUsgsDemTileSource::getTileHeight() const
{
   return ( theTile.valid() ? theTile->getHeight() : 0 );
}

ossim_uint32 ossimUsgsDemTileSource::getNumberOfLines(ossim_uint32 reduced_res_level) const
{
   if (reduced_res_level == 0)
   {
      if (theDem)
      {
         return theDem->getHeight();
      }
      else
      {
         cerr << "ossimUsgsDemTileSource::getNumberOfLines ERROR:"
              << "\nNot initialized!" << std::endl;
         return 0;
      }
   }
   else if (theOverview.valid())
   {
      return theOverview->getNumberOfLines(reduced_res_level);
   }

   return 0;
}

ossim_uint32 ossimUsgsDemTileSource::getNumberOfSamples(ossim_uint32 reduced_res_level) const
{
   if (reduced_res_level == 0)
   {
      if (theDem)
      {
         return theDem->getWidth();
      }
      else
      {
         cerr << "ossimUsgsDemTileSource::getNumberOfSamples ERROR:"
              << "\nNot initialized!" << std::endl;
         return 0;
      }
   }
   else if (theOverview.valid())
   {
      return theOverview->getNumberOfSamples(reduced_res_level);
   }

   return 0;
}

ossim_uint32 ossimUsgsDemTileSource::getImageTileWidth() const
{
   return 0;
}

ossim_uint32 ossimUsgsDemTileSource::getImageTileHeight() const
{
   return 0;
}

ossimString ossimUsgsDemTileSource::getShortName() const
{
   return ossimString("usgs dem");
}

ossimString ossimUsgsDemTileSource::getLongName() const
{
   return ossimString("usgs dem reader");
}

ossim_uint32 ossimUsgsDemTileSource::getNumberOfInputBands() const
{
   return 1;
}

ossim_uint32 ossimUsgsDemTileSource::getNumberOfOutputBands()const
{
   return 1;
}

double ossimUsgsDemTileSource::getNullPixelValue(ossim_uint32 /* band */)const 
{
   return theNullValue;
}

double ossimUsgsDemTileSource::getMinPixelValue(ossim_uint32 band)const 
{
   if(band < theMetaData.getNumberOfBands())
   {
      return theMetaData.getMinPix(band);
   }
   return theMinHeight; 
}

double ossimUsgsDemTileSource::getMaxPixelValue(ossim_uint32 band)const 
{
   if(theMetaData.getNumberOfBands())
   {
      return theMetaData.getMaxPix(band);
   }
   return theMaxHeight; 
}

bool ossimUsgsDemTileSource::isOpen()const
{
   return (theDem ? true : false );
}

const ossimUsgsDemTileSource& ossimUsgsDemTileSource::operator=(const  ossimUsgsDemTileSource& rhs)
{
   return rhs;
} 

ossimUsgsDemTileSource::ossimUsgsDemTileSource(const ossimUsgsDemTileSource&)
{
}
