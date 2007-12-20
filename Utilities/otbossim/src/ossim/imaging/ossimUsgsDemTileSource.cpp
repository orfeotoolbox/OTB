//*******************************************************************
//
// License:  See top level LICENSE.txt.
//
// Author:  David Burken
//
// Description:
//
// Contains class declaration for ossimUsgsDemTileSource.
//
//********************************************************************
// $Id: ossimUsgsDemTileSource.cpp 11419 2007-07-27 16:24:57Z dburken $

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


RTTI_DEF1(ossimUsgsDemTileSource, "ossimUsgsDemTileSource", ossimImageHandler)

// Static trace for debugging
static ossimTrace traceDebug("ossimUsgsDemTileSource:debug");

static const char DEM_TYPE_KW[] = "dem_type";
static const char USGS_DEM_KW[] = "usgs_dem";


ossimUsgsDemTileSource::ossimUsgsDemTileSource()
   :
      ossimImageHandler(),
      theDem(NULL),
      theTile(NULL),
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
      theDem = NULL;
   }
   theTile = NULL;
}

ossimRefPtr<ossimImageData> ossimUsgsDemTileSource::getTile(
   const  ossimIrect& tile_rect, ossim_uint32 resLevel)
{
   // This tile source bypassed, or invalid res level, return a blank tile.
   if (!isOpen() || !isSourceEnabled() || !isValidRLevel(resLevel))
   {
      return ossimRefPtr<ossimImageData>();
   }

   if (theOverview)
   {
      if (theOverview->hasR0() || resLevel)
      {
         ossimRefPtr<ossimImageData> temp = theOverview->getTile(tile_rect,
                                                                 resLevel);
         if(temp.valid() && temp->getBuf())
         {
            temp->setMinPix(theTile->getMinPix(), theTile->getNumberOfBands());
            temp->setMaxPix(theTile->getMaxPix(), theTile->getNumberOfBands());
            temp->setNullPix(theTile->getNullPix(), theTile->getNumberOfBands());
         }
         else
         {
            return ossimRefPtr<ossimImageData>();
         }
         return temp;
      }
   }

   if (!theTile.valid())
   {
      return theTile;
   }

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

      // Load the tile buffer with data from the dem cell.
      bool status;
      if (theScalarType == OSSIM_FLOAT32)
      {
         status = fillBuffer(ossim_float32(0.0), tile_rect, clip_rect);
      }
      else
      {
         status = fillBuffer(ossim_sint16(0), tile_rect, clip_rect);
  
      }
      if (status == true)
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
template <class T>
bool ossimUsgsDemTileSource::fillBuffer(T,
                                        const ossimIrect& tile_rect,
                                        const ossimIrect& clip_rect)
{
   const uint32 TILE_WIDTH = theTile->getWidth();

   T* d = static_cast<T*>(theTile->getBuf());
   
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
   
   if (!theImageFile.exists()) // See if file exists.
   {
      if (traceDebug())
      {
         CLOG << "DEBUG:"
              << "\nFile " << theImageFile.c_str() << " does not exist!"
              << std::endl;
      }
      return false;
   }

   //---
   // Open checks:
   // 1) Check extension for .dem
   //
   // 2) Look for file.omd (ossim meta data) file containing keyword
   //    "dem_type" with value of "usgs_dem".
   //
   // 3) Look for file.kwl (keyword list) file containing keyword
   //    "dem_type" with value of "usgs_dem".
   //
   // NOTE:
   // There is a keyword list template stored in the templates directory:
   // "ossim/etc/templates/usgs_dem_template.kwl"
   //---
   ossimString ext = theImageFile.ext();
   ext.downcase();
   if ( (ext == "kwl") || (ext == "omd") )
   {
      // Don't allow keyword list to be fed to open.  Only image files.
      return false;
   }
      
   if (ext == "dem")
   {
      theIsDemFlag = true;
   }

   // Look for filename.omd
   ossimFilename kwl_file = theImageFile;
   kwl_file.setExtension("omd");
   if (kwl_file.exists()) // See if file exists.
   {
      ossimKeywordlist kwl(kwl_file);
      if (kwl.getErrorStatus() == ossimErrorCodes::OSSIM_OK)
      {
         const char* lookup;
         
         if (!theIsDemFlag)
         {
            const char* lookup = kwl.find(DEM_TYPE_KW);
            if (lookup)
            {
               ossimString s = lookup;
               s.downcase();
               if (s == USGS_DEM_KW)
               {
                  theIsDemFlag = true;
               }
            }
         }

         //---
         // Look for scalar type override.
         //
         // Note: We only allow float or signed 16 bit.
         //---
         lookup = kwl.find(ossimKeywordNames::SCALAR_TYPE_KW);
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
      }
   }

   if (!theIsDemFlag)
   {
      return false;
   }

   // Open up the file for reading.
   std::ifstream is(theImageFile.c_str(), ios::in | ios::binary);
   if (!is)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      
      cerr << MODULE << " ERROR!"
           << "\nCould not open file:  " << theImageFile.c_str()
           << "\nReturning..." << std::endl;
      if (traceDebug())  CLOG << "returning with error..." << std::endl;
      theIsDemFlag = false;
      return false;
   }

   //---
   // SPECIAL HACK (drb):
   // Check some bytes and make sure there is no binary data.  There are
   // files out there with .dem extension that are binary rasters.
   //---
   ossim_uint8* ubuf = new ossim_uint8[512];
   is.read((char*)ubuf, 512);
   if (is)
   {
      for (int i = 0; i < 512; ++i)
      {
         if (ubuf[i] > 0x7f)
         {
            theIsDemFlag = false;
            break;
         }
      }
   }
   else
   {
      theIsDemFlag = false;
   }
   delete [] ubuf;
   ubuf = NULL;
   if (theIsDemFlag == false)
   {
      is.close();
      return false;
   }
   is.seekg(0, ios::beg);
   // End of "SPECIAL HACK".

   // Start out with a fresh dem.
   if (theDem) delete theDem;

   //---
   // Set the null to -32768.  This will also be the missing data values.
   //---
   theNullValue = OSSIM_DEFAULT_NULL_PIX_SINT16;

   //---
   // Read the dem.
   // 
   // NOTE:  This defines the missing data value.  It should be the same as
   // null for mosaicing and min/max calculations.
   //---
   theDem = new ossimDemGrid(theNullValue);
   theDem->read(is);

   is.close();

   theFeetFlag  = (theDem->getHeader().getElevationUnits() == 1) ?
                  true : false;

   theMinHeight = theFeetFlag ? theDem->getHeader().getMinimumElev() *
                  MTRS_PER_FT : theDem->getHeader().getMinimumElev();
   theMaxHeight = theFeetFlag ? theDem->getHeader().getMaximumElev() *
                  MTRS_PER_FT : theDem->getHeader().getMaximumElev();

   completeOpen();

   //***
   // Set up the tiles.  Note the blank tile will not get initialized to save
   // memory.  This includes setting the min and max pixel value gathered
   // from the statistics.
   //***
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

   return true;
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

bool ossimUsgsDemTileSource::getImageGeometry(ossimKeywordlist& kwl,
                                              const char* prefix)
{
   if (!theDem)
   {
      return false;
   }

   // Previously saved...
   if (theGeometryKwl.getSize())
   {
      kwl = theGeometryKwl;
      return true;
   }  
      
   const ossimDemHeader HDR = theDem->getHeader();

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimUsgsDemTileSource::getImageGeometry DEBUG:"
         << "\nDEM Header:"
         << std::endl;
      HDR.print(ossimNotify(ossimNotifyLevel_DEBUG));
   }

   if (HDR.getImageGeometry(kwl, prefix) == false)
   {
      return false;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimUsgsDemTileSource::getImageGeometry DEBUG:"
         << "keyword list:\n" << kwl
         << std::endl;
   }

   // Capture for next time.
   setImageGeometry(kwl);

   return true;
}

ossimScalarType
ossimUsgsDemTileSource::getOutputScalarType() const
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
   else if (theOverview)
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
   else if (theOverview)
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

ossimString ossimUsgsDemTileSource::className() const
{
   return ossimString("ossimUsgsDemTileSource");
}

ossim_uint32 ossimUsgsDemTileSource::getNumberOfInputBands() const
{
   return 1;
}

ossim_uint32 ossimUsgsDemTileSource::getNumberOfOutputBands()const
{
   return 1;
}

double ossimUsgsDemTileSource::getNullPixelValue(ossim_uint32 band)const 
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
