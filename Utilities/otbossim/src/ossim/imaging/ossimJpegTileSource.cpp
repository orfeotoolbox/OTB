//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
//
// Contains class definition for JpegTileSource.
//*******************************************************************
//  $Id: ossimJpegTileSource.cpp 13054 2008-06-23 13:55:13Z gpotts $
#if defined(__BORLANDC__)
#include <iostream>
using std::size_t;
#include <stdlib.h>
#include <stdio.h>
#endif
extern "C"
{
#include <stdio.h>
#include <jpeglib.h>
#include <setjmp.h>
}

#include <ossim/imaging/ossimJpegTileSource.h>
#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/imaging/ossimU8ImageData.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimContainerProperty.h>
//---
// Using windows .NET compiler there is a conflict in the libjpeg with INT32
// in the file jmorecfg.h.  Defining XMD_H fixes this.
//---

RTTI_DEF1_INST(ossimJpegTileSource, "ossimJpegTileSource", ossimImageHandler)

static ossimTrace traceDebug("ossimJpegTileSource:degug");  

class ossimJpegTileSource::PrivateData
{
public:
	PrivateData()
		:theCinfo(),
		theJerr()
	{

	}
	virtual ~PrivateData()
	{
		clear();
	}
	void clear()
	{
		jpeg_destroy_decompress( &theCinfo );
	}
   struct jpeg_decompress_struct theCinfo;
   struct jpeg_error_mgr         theJerr;
};
//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimJpegTileSource::ossimJpegTileSource()
   :
      ossimImageHandler(),
      theTile(NULL),
      theCacheTile(NULL),
      theLineBuffer(NULL),
      theFilePtr(0),
      theBufferRect(0, 0, 0, 0),
      theImageRect(0, 0, 0, 0),
      theNumberOfBands(0),
      theCacheSize	(0),
	  thePrivateData(0),
      theCacheId(-1)
{
}

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimJpegTileSource::ossimJpegTileSource(const ossimKeywordlist& kwl,
                               const char* prefix)
   :
      ossimImageHandler(),
      theTile(NULL),
      theCacheTile(NULL),
      theLineBuffer(NULL),
      theFilePtr(0),
      theBufferRect(0, 0, 0, 0),
      theImageRect(0, 0, 0, 0),
      theNumberOfBands(0),
      theCacheSize	(0),
	  thePrivateData(0),
      theCacheId(-1)
{
   if (loadState(kwl, prefix) == false)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
   }
}

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimJpegTileSource::ossimJpegTileSource(const char* jpeg_file)
   :
      ossimImageHandler(),
      theTile(NULL),
      theCacheTile(NULL),
      theLineBuffer(NULL),
      theFilePtr(0),
      theBufferRect(0, 0, 0, 0),
      theImageRect(0, 0, 0, 0),
      theNumberOfBands(0),
      theCacheSize(0),
	  thePrivateData(0),
      theCacheId(-1)
{
   static const char MODULE[]
      = "ossimJpegTileSource::ossimJpegTileSource";

   if (!open())
   {

      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE
         << "\nCannot open:  " << jpeg_file
         << endl;
   }
}

//*******************************************************************
// Destructor:
//*******************************************************************
ossimJpegTileSource::~ossimJpegTileSource()
{
   destroy();
}

void ossimJpegTileSource::destroy()
{
   ossimAppFixedTileCache::instance()->deleteCache(theCacheId);

   theTile      = NULL;
   theCacheTile = NULL;

   if (theLineBuffer)
   {
      delete [] theLineBuffer;
      theLineBuffer = NULL;
   }
   if (theFilePtr)
   {
      fclose(theFilePtr);
      theFilePtr = NULL;
   }
   if(thePrivateData)
   {
	   delete thePrivateData;
	   thePrivateData = 0;
   }
}

void ossimJpegTileSource::allocate()
{
   if(theLineBuffer)
   {
      delete [] theLineBuffer;
      theLineBuffer = 0;
   }
   // Make the cache tile the height of one tile by the image width.
   ossim::defaultTileSize(theCacheSize);
   theCacheSize.x = theImageRect.width();

   ossimAppFixedTileCache::instance()->deleteCache(theCacheId);
   theCacheId = ossimAppFixedTileCache::instance()->newTileCache(theImageRect, theCacheSize);

   theTile = ossimImageDataFactory::instance()->create(this, this);
   theCacheTile = (ossimImageData*)theTile->dup();
   theTile->initialize();
   
   ossimIrect cache_rect(theImageRect.ul().x,
                         theImageRect.ul().y,
                         theImageRect.ul().x + (theCacheSize.x-1),
                         theImageRect.ul().y + (theCacheSize.y-1));
   
   theCacheTile->setImageRectangle(cache_rect);
   theCacheTile->initialize();
   
   theLineBuffer = new ossim_uint8[theImageRect.width() * theNumberOfBands];

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimJpegTileSource::allocate DEBUG:"
         << "\ncache tile size:           " << theCacheSize
         << "\nimage width:               " << theImageRect.width()
         << "\nimage height:              " << theImageRect.height()
         << "\nnumber of bands:           " << theNumberOfBands
         << endl;
   }
}

ossimRefPtr<ossimImageData> ossimJpegTileSource::getTile(
   const ossimIrect& tile_rect, ossim_uint32 resLevel)
{
   if(!isSourceEnabled()||!isOpen()||!isValidRLevel(resLevel))
   {
      return ossimRefPtr<ossimImageData>();
   }
   
   if (theOverview)
   {
      if (theOverview->hasR0() || resLevel)
      {
         return theOverview->getTile(tile_rect, resLevel);
      }
   }

   if (!theTile.valid())
   {
      return ossimRefPtr<ossimImageData>();
   }

   theTile->setImageRectangle(tile_rect);
   if (getImageRectangle(0).intersects(tile_rect))
   {
      // Make a clip rect.
      ossimIrect clip_rect = tile_rect.clipToRect(getImageRectangle(0));

      fillTile(clip_rect);
   }
   else
   {
      // No point in requested tile within the image rectangle.
      return ossimRefPtr<ossimImageData>();
   }

   return theTile;
}

void ossimJpegTileSource::fillTile(const ossimIrect& clip_rect)
{
   if (!theTile || !theFilePtr) return;

   ossimIrect buffer_rect = clip_rect;
   buffer_rect.stretchToTileBoundary(theCacheSize);
   buffer_rect.set_ulx(0);
   buffer_rect.set_lrx(getImageRectangle(0).lr().x);

   // Check for a partial tile.
   if ( ! theTile->getImageRectangle().completely_within(buffer_rect) )
   {
      theTile->makeBlank();
   }

   ossim_int32 number_of_cache_tiles = buffer_rect.height()/theCacheSize.y;

#if 0
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "tiles high:  " << number_of_cache_tiles
         << endl;
   }
#endif

   ossimIpt origin = buffer_rect.ul();
   
   for (int tile = 0; tile < number_of_cache_tiles; ++tile)
   {
      // See if it's in the cache already.
      ossimRefPtr<ossimImageData> tempTile;
      tempTile = ossimAppFixedTileCache::instance()->
         getTile(theCacheId, origin);
      if (tempTile.valid())
      {
         theTile->loadTile(tempTile.get());
      }
      else
      {
         // Have to read from the jpeg file.
         ossim_uint32 start_line = static_cast<ossim_uint32>(origin.y);
         ossim_uint32 stop_line  = 
            static_cast<ossim_uint32>( min(origin.y+theCacheSize.y-1,
                                           getImageRectangle().lr().y) );
         ossimIrect cache_rect(origin.x,
                               origin.y,
                               origin.x+theCacheSize.x-1,
                               origin.y+theCacheSize.y-1);

         theCacheTile->setImageRectangle(cache_rect);

         if ( !theCacheTile->getImageRectangle().
              completely_within(getImageRectangle()) )
         {
            theCacheTile->makeBlank();
         }

         if (start_line < thePrivateData->theCinfo.output_scanline)
         {
            // Must restart the compression process again.
            restart();
         }
         
         // Get pointers to the cache tile buffers.
         JSAMPROW jbuf[1];
         std::vector<ossim_uint8*> buf(theNumberOfBands);
         ossim_uint32 band = 0;
         for (band = 0; band < theNumberOfBands; ++band)
         {
            buf[band] = theCacheTile->getUcharBuf(band);
         }

         const ossim_uint32 SAMPLES = getNumberOfSamples();
         jbuf[0] = (JSAMPROW) theLineBuffer;

         // Gobble any not needed lines.
         while (thePrivateData->theCinfo.output_scanline < start_line)
         {
            jpeg_read_scanlines(&thePrivateData->theCinfo, jbuf, 1);
         }

         while (thePrivateData->theCinfo.output_scanline <= stop_line)
         {
            // Read a line from the jpeg file.
            jpeg_read_scanlines(&thePrivateData->theCinfo, jbuf, 1);
            
            //---
            // Copy the line which if band interleaved by pixel the the band
            // separate buffers.
            //---
            ossim_uint32 index = 0;
            for (ossim_uint32 sample = 0; sample < SAMPLES; ++sample)
            {
               for (band = 0; band < theNumberOfBands; ++band)
               {
                  buf[band][sample] = theLineBuffer[index];
                  ++index;
               }
            }
            
            for (band = 0; band < theNumberOfBands; ++band)
            {
               buf[band] += SAMPLES;
            }
         }
         theCacheTile->validate();
         
         theTile->loadTile(theCacheTile.get());
         
         // Add it to the cache for the next time.
         ossimAppFixedTileCache::instance()->addTile(theCacheId, theCacheTile);
         

//         delete[] buf;

      } // End of reading for jpeg file.
      
      origin.y += theCacheSize.y;
      
   } // for (int tile = 0; tile < number_of_cache_tiles; ++tile)

   theTile->validate();
}

//*******************************************************************
// Public Method:
//*******************************************************************
ossimIrect
ossimJpegTileSource::getImageRectangle(ossim_uint32 reduced_res_level) const
{
   return ossimIrect(0,
                     0,
                     getNumberOfSamples(reduced_res_level) - 1,
                     getNumberOfLines(reduced_res_level)   - 1);
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimJpegTileSource::saveState(ossimKeywordlist& kwl,
                               const char* prefix) const
{
   return ossimImageHandler::saveState(kwl, prefix);
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimJpegTileSource::loadState(const ossimKeywordlist& kwl,
                               const char* prefix)
{
   if (ossimImageHandler::loadState(kwl, prefix))
   {
      return open();
   }

   return false;
}


//*******************************************************************
// Public method:
//*******************************************************************
bool ossimJpegTileSource::open(const ossimFilename& jpeg_file)
{
   theImageFile = jpeg_file;

   return open();
}

//*******************************************************************
// Private method:I have problems

//*******************************************************************
bool ossimJpegTileSource::open()
{
   static const char MODULE[] = "ossimJpegTileSource::open";

   // Start with a clean slate.
   destroy();
   // Check for empty filename.
   if (theImageFile.empty())
   {
      return false;
   }
   
   // Open Jpeg file.
   if((theFilePtr = fopen(theImageFile.c_str(), "rb")) == NULL)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << "\nERROR:\n"
            << "Could not open:  " << theImageFile.c_str()
            << endl;
      }
      
      return false;
   }

   //***
   // Verify the file is a jpeg by checking the first two bytes.
   //***
   ossim_uint8 c[2];
   fread(c, 2, 1, theFilePtr);
   if( c[0] != 0xFF || c[1] != 0xD8 )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " NOTICE:\n"
            << "Not a jpeg file..." << endl;
      }
      
      fclose(theFilePtr);
      theFilePtr = NULL;
      return false;
   }

   thePrivateData = new PrivateData();
   rewind(theFilePtr);

   //---
   // Step 1: allocate and initialize JPEG decompression object
   // We set up the normal JPEG error routines, then override error_exit.
   //---   
   thePrivateData->theCinfo.err = jpeg_std_error(&thePrivateData->theJerr);

   // Initialize the JPEG decompression object.
   jpeg_create_decompress(&thePrivateData->theCinfo);

   // Specify data source.
   jpeg_stdio_src(&thePrivateData->theCinfo, theFilePtr);

   // Read the file parameters with jpeg_read_header.
   jpeg_read_header(&thePrivateData->theCinfo, TRUE);

   jpeg_start_decompress(&thePrivateData->theCinfo);

   theNumberOfBands = thePrivateData->theCinfo.output_components;

   theImageRect = ossimIrect(0,
                             0,
                             thePrivateData->theCinfo.output_width  - 1,
                             thePrivateData->theCinfo.output_height - 1);
   
   theBufferRect.set_lrx(thePrivateData->theCinfo.output_width  - 1);
   
   completeOpen();

   // Allocate memory...
   allocate();

   return true;
}

ossimRefPtr<ossimProperty> ossimJpegTileSource::getProperty(const ossimString& name)const
{
	if(name == "file_type")
	{
		return new ossimStringProperty("file_type", "JPEG");
	}
	return ossimImageHandler::getProperty(name);
}

void ossimJpegTileSource::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
	ossimImageHandler::getPropertyNames(propertyNames);
	propertyNames.push_back("file_type");
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimJpegTileSource::getTileWidth() const
{
   return ( theTile.valid() ? theTile->getWidth() : 0 );
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimJpegTileSource::getTileHeight() const
{
   return ( theTile.valid() ? theTile->getHeight() : 0 );
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimJpegTileSource::isValidRLevel(ossim_uint32 reduced_res_level) const
{
   static const char MODULE[] = "ossimJpegTileSource::isValidRLevel";

   if (reduced_res_level == 0)
   {
      return true;
   }
   else if (theOverview)
   {
      return theOverview->isValidRLevel(reduced_res_level);
   }
   else
   {
      cerr << MODULE << " Invalid reduced_res_level:  " << reduced_res_level
           << "\nHighest available:  " << (getNumberOfDecimationLevels() - 1)
           << endl;
      return false;
   }
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimJpegTileSource::getNumberOfLines(ossim_uint32 reduced_res_level) const
{
   if (reduced_res_level == 0)
   {
      return theImageRect.lr().y - theImageRect.ul().y + 1;
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
ossim_uint32 ossimJpegTileSource::getNumberOfSamples(ossim_uint32 reduced_res_level) const
{
   if (reduced_res_level == 0)
   {
      return theImageRect.lr().x - theImageRect.ul().x + 1;;
   }
   else if (theOverview)
   {
      return theOverview->getNumberOfSamples(reduced_res_level);
   }

   return 0;
}

ossim_uint32 ossimJpegTileSource::getImageTileWidth() const
{
   return 0;
}

ossim_uint32 ossimJpegTileSource::getImageTileHeight() const
{
   return 0;
}

ossimString ossimJpegTileSource::getShortName()const
{
   return ossimString("jpg");
}
   
ossimString ossimJpegTileSource::getLongName()const
{
   return ossimString("jpg reader");
}

ossimString  ossimJpegTileSource::className()const
{
   return ossimString("ossimJpegTileSource");
}

ossim_uint32 ossimJpegTileSource::getNumberOfInputBands() const
{
   return theNumberOfBands;
}

ossim_uint32 ossimJpegTileSource::getNumberOfOutputBands()const
{
   return getNumberOfInputBands();
}

ossimScalarType ossimJpegTileSource::getOutputScalarType() const
{
   return OSSIM_UCHAR;
}

bool ossimJpegTileSource::isOpen()const
{
   return (theFilePtr != NULL);
}

void ossimJpegTileSource::restart()
{
   jpeg_abort_decompress( &thePrivateData->theCinfo );
   jpeg_destroy_decompress( &thePrivateData->theCinfo );
   
   // Put the theFilePtr back to the start...
   rewind(theFilePtr);

   // Initialize the JPEG decompression object.
   jpeg_create_decompress(&thePrivateData->theCinfo);

   // Specify data source.
   jpeg_stdio_src(&thePrivateData->theCinfo, theFilePtr);

   // Read the file parameters with jpeg_read_header.
   jpeg_read_header(&thePrivateData->theCinfo, TRUE);

   jpeg_start_decompress(&thePrivateData->theCinfo);
}
