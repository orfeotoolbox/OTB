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
// Contains class definition for ossimGeneralRasterTileSource.
//*******************************************************************
//  $Id: ossimGeneralRasterTileSource.cpp 19900 2011-08-04 14:19:57Z dburken $

#include <ossim/imaging/ossimGeneralRasterTileSource.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimInterleaveTypeLut.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/base/ossimStreamFactoryRegistry.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageGeometryRegistry.h>
#include <ossim/projection/ossimMapProjectionFactory.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/support_data/ossimFgdcXmlDoc.h>

RTTI_DEF1_INST(ossimGeneralRasterTileSource,
               "ossimGeneralRasterTileSource",
               ossimImageHandler)

static ossimTrace traceDebug("ossimGeneralRasterTileSource:debug");

// For interleave type enum to string conversions.
static const ossimInterleaveTypeLut ILUT;

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimGeneralRasterTileSource::ossimGeneralRasterTileSource()
   :
      ossimImageHandler(),
      theTile(0),
      theBuffer(0),
      theBufferInterleave(OSSIM_BIL),
      theFileStrList(0),
      theImageData(),
      theBufferRect(0, 0, 0, 0),
      theSwapBytesFlag(false),
      theBufferSizeInPixels(0)
{}

ossimGeneralRasterTileSource::~ossimGeneralRasterTileSource()
{
   close();
}

ossimRefPtr<ossimImageData> ossimGeneralRasterTileSource::getTile(
   const ossimIrect& tile_rect, ossim_uint32 resLevel)
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

bool ossimGeneralRasterTileSource::getTile(ossimImageData* result,
                                           ossim_uint32 resLevel)
{
   bool status = false;
   
   //---
   // Not open, this tile source bypassed, or invalid res level,
   // return a blank tile.
   //---
   if( isOpen() && isSourceEnabled() && isValidRLevel(resLevel)  &&
       result && (result->getNumberOfBands() == getNumberOfOutputBands()) )
   {
      //---
      // Check for overview tile.  Some overviews can contain r0 so always
      // call even if resLevel is 0.  Method returns true on success, false
      // on error.
      //---
      status = getOverviewTile(resLevel, result);
      if (status)
      {
         if(getOutputScalarType() == OSSIM_USHORT11)
         {
            //---
            // Temp fix:
            // The overview handler could return a tile of OSSIM_UINT16 if
            // the max sample value was not set to 2047.
            //---
            result->setScalarType(OSSIM_USHORT11);
         }
      }
      
      if (!status) // Did not get an overview tile.
      {
         status = true;
         
         //---
         // Subtract any sub image offset to get the zero based image space
         // rectangle.
         //---
         ossimIrect tile_rect = result->getImageRectangle();
         
         // This should be the zero base image rectangle for this res level.
         ossimIrect image_rect = getImageRectangle(resLevel);
         
         //---
         // See if any point of the requested tile is in the image.
         //---
         if ( tile_rect.intersects(image_rect) )
         {
            // Make the tile rectangle zero base.
            result->setImageRectangle(tile_rect);

            // This will reallocate the buffer if needed.
            checkBuffer(tile_rect);

            // Initialize the tile if needed as we're going to stuff it.
            if (result->getDataObjectStatus() == OSSIM_NULL)
            {
               result->initialize();
            }

            ossimIrect clip_rect = tile_rect.clipToRect(image_rect);

            if ( ! tile_rect.completely_within(theBufferRect) )
            {
               // A new buffer must be loaded.
               
               if ( !tile_rect.completely_within(clip_rect) )
               {
                  //---
                  // Start with a blank tile since the whole tile buffer will
                  // not be
                  // filled.
                  //---
                  result->makeBlank();
               }
               
               ossimIpt size(static_cast<ossim_int32>(result->getWidth()),
                             static_cast<ossim_int32>(result->getHeight()));
               
               if( !fillBuffer(clip_rect.ul(), size) )
               {
                  ossimNotify(ossimNotifyLevel_WARN)
                     << "Error from fill buffer..."
                     << endl;
                  //---
                  // Error in filling buffer.
                  //---
                  setErrorStatus();
                  status = false;
               }
            }
            
            result->loadTile(theBuffer,
                             theBufferRect,
                             clip_rect,
                             theBufferInterleave);
            result->validate();

            // Set the rectangle back.
            result->setImageRectangle(tile_rect);
            
         }
         else // No intersection.
         {
            result->makeBlank();
         }
      }
   }
   return status;
}

//*******************************************************************
// Private Method:
//*******************************************************************
     bool ossimGeneralRasterTileSource::fillBuffer(const ossimIpt& origin,
                                                   const ossimIpt& size)
{
   static const char MODULE[] = "ossimGeneralRasterTileSource::fillBuffer";

   // Note:  InterleaveType enumerations in "constants.h" file.
   bool status = false;
   switch (theImageData.interleaveType())
   {
      case OSSIM_BIP:
         status = fillBIP(origin, size);
         break;
      case OSSIM_BIL:
         status = fillBIL(origin, size);
         break;
      case OSSIM_BSQ:
         status = fillBSQ(origin, size);
         break;
      case OSSIM_BSQ_MULTI_FILE:
         status = fillBsqMultiFile(origin, size);
         break;
      default:
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " ERROR:\n"
            << " Unsupported interleave type:  "
            << ILUT.getEntryString(theImageData.interleaveType())
            << endl;
   }
   
   if (status && theSwapBytesFlag)
   {
      ossimEndian oe;
      oe.swap(theImageData.getScalarType(),
              theBuffer,
              theBufferSizeInPixels);
   }

   return status;
}

//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimGeneralRasterTileSource::fillBIP(const ossimIpt& origin,
                                           const ossimIpt& size )
{
   static const char MODULE[] = "ossimGeneralRasterTileSource::fillBIP";
   
   //***
   // This will fill a buffer the full width of valid samples * getHeight().
   //***
   theBufferRect.set_ul(origin);

   theBufferRect.set_lry(min( (origin.y + size.y -1),
                              theImageData.imageRect().lr().y));
   theBufferRect.set_lrx(min( (origin.x + size.x -1),
                              theImageData.imageRect().lr().x));
   
   ossim_sint64 currentLine = origin.y;
   
   //***
   // Seek position.  The seek position must take into account:
   // 1).  Any header offset "theOffsetInBytes".
   // 2).  Any line offset "theLineOffset."
   // 3).  Number of bands.
   //
   // NOTE:  seekPos is in bytes so theBytesPerPixel must be taken into
   //        account also.
   //***
   std::streampos offset = theImageData.offsetToFirstValidSample() +
        currentLine * theImageData.bytesPerRawLine() +
        static_cast<ossim_uint64>(origin.x) * theImageData.bytesPerPixel() *
        static_cast<ossim_uint64>(theImageData.numberOfBands());

#if 0
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\nDEBUG:"
         << "\norigin:                 " << origin
         << "\nSeek position:          " << offset
         << "\nStarting line number:   " << currentLine << endl;
   }
#endif
   
   //***
   // Loop through and process lines. 
   //***
   ossim_uint64 linesProcessed = 0;
   
   size_t buffer_width = theBufferRect.width() * theImageData.numberOfBands() *
                         theImageData.bytesPerPixel();
   
   ossim_uint8* buf = theBuffer;

   ossim_uint64 height = size.y;
   
   while ((currentLine <= static_cast<ossim_sint64>(theImageData.imageRect().lr().y)) &&
          linesProcessed < height)
   {
      //***
      // Seek to line.
      //***
      theFileStrList[0]->seekg(offset, ios::beg);
      if (!(*theFileStrList[0]))
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         ossimNotify(ossimNotifyLevel_WARN) << MODULE << " ERROR:\n"
                                            << " Seek error!  Returning with error..." << endl;
         return false;
      }

      //***
      // Read the line of image data.   
      //***
      theFileStrList[0]->read((char*)buf, (std::streamsize)buffer_width);
      if ((long)theFileStrList[0]->gcount() !=  (long)buffer_width) 
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         ossimNotify(ossimNotifyLevel_WARN) << MODULE << "\nERROR:  Reading image line." << endl;
         return false;
      }

      ++currentLine;
      ++linesProcessed;
      buf += buffer_width;
      offset += theImageData.bytesPerRawLine();
   }
   
   return true;
}
   
//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimGeneralRasterTileSource::fillBIL(const ossimIpt& origin,
                                           const ossimIpt& size)
{
   static const char MODULE[] = "ossimGeneralRasterTileSource::fillBIL";
   
   //***
   // This will fill a buffer the full width of valid samples * tileHeight().
   //***
   theBufferRect.set_ul(origin);
   theBufferRect.set_lry(min((origin.y + size.y - 1),
                             theImageData.imageRect().lr().y));
   theBufferRect.set_lrx(min((origin.x + size.x - 1),
                             theImageData.imageRect().lr().x));
   
   ossim_sint64 currentLine = origin.y;
   // Start seek position.
   std::streampos offset = (static_cast<ossim_uint64>(theImageData.offsetToFirstValidSample()) +
                            currentLine * static_cast<ossim_uint64>(theImageData.bytesPerRawLine()) *
                            theImageData.numberOfBands() +
                            origin.x * theImageData.bytesPerPixel());

   //***
   // Loop through and process lines. 
   //***
   ossim_uint64 linesProcessed = 0;
   
   size_t buffer_width = theBufferRect.width() * theImageData.bytesPerPixel();
   
   ossim_uint8* buf = theBuffer;

#if 0
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\nDEBUG:"
         << "\norigin:                 " << origin
         << "\nSeek position:          " << offset
         << "\nStarting line number:   " << currentLine
         << "\nbuffer_width:           " << buffer_width << endl;
   }
#endif

   ossim_uint64 height    = size.y;
   ossim_sint64 num_bands = theImageData.numberOfBands();
   
   while ((currentLine <= static_cast<ossim_sint64>(theImageData.imageRect().lr().y)) &&
          linesProcessed < height)
   {
      for (ossim_int32 band = 0; band < num_bands; ++band)
      {
         //***
         // Seek to line.
         //***
         theFileStrList[0]->seekg(offset, ios::beg);
         if (!theFileStrList[0])
         {
            theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
             ossimNotify(ossimNotifyLevel_WARN) << MODULE << " ERROR:\n"
                                                << " Seek error!  Returning with error..." << endl;
            return false;
         }
         
         //***
         // Read the line of image data.   
         //***
         theFileStrList[0]->read((char*)buf, (std::streamsize)buffer_width);
         if ((long)theFileStrList[0]->gcount() !=  (long)buffer_width) 
         {
            theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
            ossimNotify(ossimNotifyLevel_WARN) << MODULE << "\nERROR:  Reading image line."
                                               << "\ncurrentLine:  " << currentLine << endl;
            return false;
         }

         buf += buffer_width;
         offset += theImageData.bytesPerRawLine();

      } // End of band loop.
      
      ++linesProcessed;
      ++currentLine;
   }
   
   return true;
}
   
//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimGeneralRasterTileSource::fillBSQ(const ossimIpt& origin,
                                           const ossimIpt& size)
{
   static const char MODULE[] = "ossimGeneralRasterTileSource::fillBSQ";

   //***
   // This will fill a buffer the full width of valid samples * tileHeight().
   //***
   theBufferRect.set_ul(origin);
   
   theBufferRect.set_lry(min((origin.y + size.y -1),
                             theImageData.imageRect().lr().y));
   theBufferRect.set_lrx(min((origin.x + size.x - 1),
                             theImageData.imageRect().lr().x));
   
   //***
   // Start seek position.
   //***
   std::streampos startSeekPosition
      = theImageData.offsetToFirstValidSample() +
        static_cast<ossim_sint64>(origin.y) *
        theImageData.bytesPerRawLine() +
        static_cast<ossim_sint64>(origin.x) *
        theImageData.bytesPerPixel();

   //***
   // Loop through and process lines. 
   //***
   size_t buffer_width = theBufferRect.width() * theImageData.bytesPerPixel();
   
   ossim_uint8* buf = (ossim_uint8*)theBuffer;

#if 0
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\nDEBUG:"
         << "\norigin:                 " << origin
         << "\nSeek position:          " << startSeekPosition
         << "\nStarting line number:   " << origin.y
         << "\nbuffer_width:           " << buffer_width
         << "\nbytesPerRawLine():      "
         << theImageData.bytesPerRawLine()
         << "\ntheImageData.offsetToFirstValidSample():  "
         << theImageData.offsetToFirstValidSample() << endl;
   }
#endif

   std::streampos bandOffset
      = theImageData.bytesPerRawLine() * theImageData.rawLines();

   ossim_int32 num_bands = theImageData.numberOfBands();
   ossim_int32 height    = size.y;
   
   for (ossim_int32 band = 0; band < num_bands; ++band)
   {
      ossim_sint64 currentLine    = origin.y;
      ossim_sint64 linesProcessed = 0;

      std::streampos offset = startSeekPosition + (band * bandOffset);
      
      while (currentLine <= theImageData.imageRect().lr().y &&
             linesProcessed < height)
      {
         //***
         // Seek to line.
         //***
         theFileStrList[0]->seekg(offset, ios::beg);
         if (!theFileStrList[0])
         {
            theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
            ossimNotify(ossimNotifyLevel_WARN) << MODULE << " ERROR:\n"
                                               << " Seek error!  Returning with error..." << endl;
            return false;
         }
         
         //***
         // Read the line of image data.   
         //***
         theFileStrList[0]->read((char*)buf, (std::streamsize)buffer_width);
         if ((long)theFileStrList[0]->gcount() !=  (long)buffer_width) 
         {
            theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
            ossimNotify(ossimNotifyLevel_WARN) << MODULE << "\nERROR:  Reading image line."
                                               << "\ncurrentLine:  " << currentLine << endl;
            return false;
         }

         // Increment everybody accordingly.
         buf += buffer_width;
         offset += theImageData.bytesPerRawLine();
         ++linesProcessed;
         ++currentLine;

      } // End of line loop.

   } // End of band loop.
   
   return true;
}

//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimGeneralRasterTileSource::fillBsqMultiFile(const ossimIpt& origin,
                                                    const ossimIpt& size)
{
   static const char MODULE[]
      = "ossimGeneralRasterTileSource::fillBsqMultiFile";

   if (traceDebug()) CLOG << " Entered..." << endl;
   
   
   //***
   // This will fill a buffer the full width of valid samples * tileHeight().
   //***
   theBufferRect.set_ul(origin);
   
   theBufferRect.set_lry(min((origin.y + size.y -1),
                             theImageData.imageRect().lr().y));
   theBufferRect.set_lrx(min((origin.x + size.x - 1),
                             theImageData.imageRect().lr().x));
   
   //***
   // Start seek position.
   //***
   std::streampos startSeekPosition
      = theImageData.offsetToFirstValidSample() +
      origin.y * theImageData.bytesPerRawLine() +
      origin.x * theImageData.bytesPerPixel();

   //***
   // Loop through and process lines. 
   //***
   size_t buffer_width = theBufferRect.width() * theImageData.bytesPerPixel();
   
   ossim_uint8* buf = (ossim_uint8*)theBuffer;

#if 0
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\nDEBUG:"
         << "\norigin:                 " << origin
         << "\nSeek position:          " << startSeekPosition
         << "\nStarting line number:   " << origin.y
         << "\nbuffer_width:           " << buffer_width
         << "\nbuffer_rect:            " << theBufferRect
         << "\nbytesPerRawLine():      "
         << theImageData.bytesPerRawLine()
         << "\ntheImageData.offsetToFirstValidSample():  "
         << theImageData.offsetToFirstValidSample() << endl;
   }
#endif

   ossim_int32 num_bands = theImageData.numberOfBands();
   ossim_int32 height    = size.y;
   
   for (ossim_int32 band = 0; band < num_bands; ++band)
   {
      ossim_int32 currentLine    = origin.y;
      ossim_int32 linesProcessed = 0;

      ossim_int64 offset = startSeekPosition;
      
      while (currentLine <= theImageData.imageRect().lr().y &&
             linesProcessed < height)
      {
         //***
         // Seek to line.
         //***
         theFileStrList[band]->seekg(offset, ios::beg);
         if (!theFileStrList[band])
         {
            theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
            ossimNotify(ossimNotifyLevel_WARN) << MODULE << " ERROR:\n"
                                                << " Seek error!  Returning with error..." << endl;
            return false;
         }
         
         //***
         // Read the line of image data.   
         //***
         theFileStrList[band]->read((char*)buf, (std::streamsize)buffer_width);
         if ((long)theFileStrList[band]->gcount() !=  (long)buffer_width) 
         {
            theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
            ossimNotify(ossimNotifyLevel_WARN) << MODULE << "\nERROR:  Reading image line."
                                               << "\ncurrentLine:  " << currentLine << endl;
            return false;
         }

         // Increment everybody accordingly.
         buf += buffer_width;
         offset += theImageData.bytesPerRawLine();
         ++linesProcessed;
         ++currentLine;

      } // End of line loop.

   } // End of band loop.
   
   return true;
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimGeneralRasterTileSource::saveState(ossimKeywordlist& kwl,
                                        const char* prefix) const
{
   theImageData.saveState(kwl, prefix);
   
   return ossimImageHandler::saveState(kwl, prefix);
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimGeneralRasterTileSource::loadState(const ossimKeywordlist& kwl,
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
ossimScalarType ossimGeneralRasterTileSource::getOutputScalarType() const
{
   return ( theTile.valid() ? theTile->getScalarType()
            : OSSIM_SCALAR_UNKNOWN );
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimGeneralRasterTileSource::getTileWidth() const
{
   return ( theTile.valid() ? theTile->getWidth() : 0 );
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimGeneralRasterTileSource::getTileHeight() const
{
   return ( theTile.valid() ? theTile->getHeight() : 0 );
}

//*******************************************************************
// Public method:
//*******************************************************************
bool
ossimGeneralRasterTileSource::isValidRLevel(ossim_uint32 reduced_res_level) const
{
   static const char MODULE[] = "ossimGeneralRasterTileSource::isValidRLevel";
   
   if (reduced_res_level == 0)
   {
      return true;
   }
   else if (theOverview.valid())
   {
      return theOverview->isValidRLevel(reduced_res_level);
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE
         << " Invalid reduced_res_level:  " << reduced_res_level
         << "\nHighest available:  " << (getNumberOfDecimationLevels() - 1)
         << endl;
      return false;
   }
}



//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32
ossimGeneralRasterTileSource::getNumberOfLines(ossim_uint32 reduced_res_level) const
{
   if (reduced_res_level == 0)
   {
      return theImageData.validLines();
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
ossim_uint32 ossimGeneralRasterTileSource::
getNumberOfSamples(ossim_uint32 reduced_res_level) const
{
   if (reduced_res_level == 0)
   {
      return theImageData.validSamples();
   }
   else if (theOverview.valid())
   {
      return theOverview->getNumberOfSamples(reduced_res_level);
   }

   return 0;
}

double ossimGeneralRasterTileSource::getNullPixelValue(ossim_uint32 band) const
{
   return theImageData.getNullPixelValue(band);
}

double ossimGeneralRasterTileSource::getMinPixelValue(ossim_uint32 band)const
{
   return theImageData.getMinPixelValue(band);
}

double ossimGeneralRasterTileSource::getMaxPixelValue(ossim_uint32 band)const
{
   return theImageData.getMaxPixelValue(band);
}

bool ossimGeneralRasterTileSource::open()
{
   static const char MODULE[] = "ossimGeneralRasterTileSource::open";
   if (traceDebug()) CLOG << " Entered..." << endl;
   
   if(isOpen())
   {
      close();
   }
   
   //
   // A general raster image requires a keyword list to get essential image
   // information or meta data as its sometimes called.  The meta data file
   // can have three types of extensions: ".omd", ".hdr" and ".kwl"
   // Look for them in that order.
   // Note that the ".omd" extension is for "Ossim Meta Data" and was made
   // up to avoid conflicting with other software packages ".hdr" files.
   //
   
   ossimFilename hdr = theImageFile;
   
   hdr.setExtension("omd");   
   
   // See if it's readable.
   if ( ! hdr.isReadable() )
   {
      hdr.setExtension("hdr");   
      
      // See if this one's readable.
      if ( ! hdr.isReadable() )
      {
         // Ok try .kwl extension.
         hdr.setExtension("kwl");   
         if ( ! hdr.isReadable() )
         {
            //try .xml extension
            hdr.setExtension("xml");
            if (!hdr.isReadable())
            {
               // No header file so get out.
               return false;
            }
         }
      }
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE
         << "\nHeader file:  " << hdr.c_str() << endl;
   }
   
   // Give the header file to the keyword list object.
   ossimKeywordlist kwl(hdr);
   
   if (hdr.ext() == "xml")
   {
      kwl.clear();
      ossimKeywordlist xmlKwl = getXmlInfo(hdr);
      if (xmlKwl.getSize() > 0)
      {
         kwl.addList(xmlKwl);
         if (kwl.getSize() > 0)
         {
            kwl.setErrorStatus(ossimErrorCodes::OSSIM_OK);
         }
      }
      else
      {
         return false;
      }
   }
   
   if ((kwl.getSize() == 0) || (kwl.find(ossimKeywordNames::NUMBER_SAMPLES_KW) == 0))
   {
      if (hdr.ext().downcase() != "hdr")
      {
         hdr = hdr.setExtension("hdr");
      }
      if (hdr.exists())
      {
         ossimKeywordlist hdrKwl = getHdrInfo(hdr);
         if (hdrKwl.getSize() > 0)
         {
            kwl.addList(hdrKwl);
            if (kwl.getSize() > 0)
            {
               kwl.setErrorStatus(ossimErrorCodes::OSSIM_OK);
            }
         }
      }
   }
   
   // Check for errors.
   if (kwl.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
             << MODULE
             << " Keywordlist open error detected." << endl;
      }
      
      return false;
   }
   
   //
   // If the file name passed to us was the header it's assumed the image_file
   // keyword is present else assume it's the image file and add if to the
   // keyword list.  This will overwrite the "image_file" in the header if
   // it's in there.
   //
   if (theImageFile != hdr)
   {
      kwl.add(ossimKeywordNames::FILENAME_KW,
              theImageFile.c_str(),
              true);
   }

   ossimGeneralRasterInfo generalRasterInfo;
   
   bool status = generalRasterInfo.loadState(kwl);
   theMetaData.loadState(kwl);
   // Let the load state do the rest...
   //   bool status = loadState(kwl);
   if(status)
   {
      status = open(generalRasterInfo);
   }
   
   if (traceDebug()) CLOG << " Exited..." << endl;
   
   return status;
}

bool ossimGeneralRasterTileSource::open(const ossimGeneralRasterInfo& info)
{
   if(isOpen())
   {
      close();
   }
   
   theImageData = info;
   
   if(initializeHandler())
   {
      completeOpen();
   }
   else
   {
      return false;
   }
   
   return true;
}

bool ossimGeneralRasterTileSource::initializeHandler()
{
   //***
   // This private method assumes that "theImageData" object has been
   // initialized.  Note that "close() should have already been called if
   // there was an open file prior to this.
   //***
   if(isOpen())
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimGeneralRasterTileSource::initialize() Coding Error!"
         << "\nFile was not closed prior to initialize call!"
         << "\nReturning false..."
         << endl;
      return false;
   }
   
//   theSubImageOffset = theImageData.subImageOffset();

   ossim_uint32 number_of_bands = theImageData.numberOfBands();

//    std::streampos expectedSizeInBytes;

//    if (theImageData.interleaveType() != OSSIM_BSQ_MULTI_FILE)
//    {
//       expectedSizeInBytes = theImageData.validSamples()  *
//                             theImageData.validLines()    *
//                             number_of_bands *
//                             theImageData.bytesPerPixel();
//    }
//    else
//    {
//       expectedSizeInBytes = theImageData.validSamples()  *
//                             theImageData.validLines()    *
//                             theImageData.bytesPerPixel();
//    }
   
   vector<ossimFilename> aList = theImageData.getImageFileList();

   for (ossim_uint32 i=0; i<aList.size(); ++i)
   {
      // open it...
      ossimRefPtr<ossimIFStream> is = ossimStreamFactoryRegistry::instance()->
         createNewIFStream(aList[i], std::ios::in|std::ios::binary);

      // check the stream...
      if(is.valid())
      {
         // Check the file stream.
         if ( is->fail() )
         {
            theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimGeneralRasterTileSource::open" << " ERROR:\n"
               << "Cannot open:  " << aList[i].c_str() << endl;
            is = 0;
            return false;
         }
      }
#if 0
      // check the size
      if ((!is->isCompressed()) &&
          (aList[i].fileSize() < expectedSizeInBytes))
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimGeneralRasterTileSource::open ERROR:"
               << "\nFile size not big enough!"
               << "\nExpected:  " << expectedSizeInBytes
               << "\nGot:       " << aList[i].fileSize()
               << "\nFile:      " << aList[i]
               << "\nReturning with error..." << endl;
         }
         return false;
      }
#endif
      theFileStrList.push_back(is); // Add it to the list...
   }

   if ((aList.size()==1) && theImageFile.empty())
   {
      theImageFile = aList[0];
   }
   
   //***
   // Determine the pixel type and make the appropriate tiles.
   //***
   ossimScalarType scalar = theImageData.getScalarType();
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimGeneralRasterTileSource::open DEBUG:"
         << "\nScalar type:  "
         << ossimScalarTypeLut::instance()->
         getEntryString(theImageData.getScalarType())
         << endl;
   }

   // Make the tiles for the getTile return.
                            
   ossimImageDataFactory* idf = ossimImageDataFactory::instance();

   theTile = idf->create(this,
                         scalar,
                         number_of_bands);
   
   theTile->initialize();

   // These values can be overridden by loadState...
   for(ossim_uint32 band = 0; band < number_of_bands; ++ band)
     {
       theTile->setNullPix(theImageData.getNullPixelValue(band), band);
       theTile->setMinPix(theImageData.getMinPixelValue(band), band);
       theTile->setMaxPix(theImageData.getMaxPixelValue(band), band);
     }
   theTile->makeBlank();
   // Store the size of the buffer in pixels for swapping bytes.
   theBufferSizeInPixels = theTile->getWidth() * theTile->getHeight() *
                           number_of_bands;
   
   // Get the buffer size.  This is bytes, not pixels.
   ossim_int32 buffer_size = theBufferSizeInPixels * theImageData.bytesPerPixel();

   if(theBuffer)
   {
      delete [] theBuffer;
      theBuffer = 0;
   }
   theBuffer = new ossim_uint8[buffer_size];

   // Set the buffer interleave type.
   theBufferInterleave = theImageData.interleaveType();
   if (theBufferInterleave == OSSIM_BSQ_MULTI_FILE)
   {
      theBufferInterleave = OSSIM_BSQ;
   }

   // Zero out the buffer rect.
   theBufferRect = ossimIrect(0, 0, 0, 0);
   
//    // Check for an overview if not already initialized.
//    if (!theOverview)
//    {
//       openOverview();
//    }

//    // Check for valid image vertices file if not already initialized.
//    if (theValidImageVertices.size() == 0)
//    {
//       openValidVertices();
//    }

   //***
   // Get the byte order of the image data and host machine.  If different,
   // set the swap bytes flag...
   //***
   if (theImageData.getImageDataByteOrder() != ossim::byteOrder())
   {
      theSwapBytesFlag = true;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimGeneralRasterTileSource::open" << " DEBUG:"
         << "\nByte swapping is "
         << (theSwapBytesFlag?"enabled.":"not enabled.")
         << "\ntheBufferSizeInPixels:  " << theBufferSizeInPixels
         << "\nbuffer size:  " << buffer_size
         << "\nImageData:\n";
      theImageData.print(ossimNotify(ossimNotifyLevel_DEBUG));
   }
   
   return true;
}

void ossimGeneralRasterTileSource::checkBuffer(const ossimIrect& rect)
{
  if((theBufferRect.width()*theBufferRect.height()) !=
     (rect.width()*rect.height()))
    {
      if(theBuffer)
	{
	  delete [] theBuffer;
	  theBuffer = 0;
	}
    }

  if(!theBuffer)
    {
      ossim_uint32 number_of_bands = theImageData.numberOfBands();
      theBufferSizeInPixels = rect.width()*rect.height()*number_of_bands;
      
      // Get the buffer size.  This is bytes, not pixels.
      ossim_int32 buffer_size = theBufferSizeInPixels * theImageData.bytesPerPixel();
      
      theBuffer = new ossim_uint8[buffer_size];
      theBufferRect = ossimIrect(0,0,0,0);
    }
}


bool ossimGeneralRasterTileSource::isOpen() const
{
   bool result = false;
   if (theFileStrList.size() > 0)
   {
      if(theFileStrList[0].valid())
      {
         result = !(theFileStrList[0]->fail());
      }
   }
   return result;
}

void ossimGeneralRasterTileSource::close()
{
   ossimImageHandler::close();  // base class

   theTile = 0;
   if (theBuffer)
   {
      delete [] theBuffer;
      theBuffer = 0;
   }

   std::vector<ossimRefPtr<ossimIFStream> >::iterator is = theFileStrList.begin();
   while (is != theFileStrList.end())
   {
      (*is)->close();
      // delete (*is);
      // (*is) = 0;
      ++is;
   }
   theFileStrList.clear();
}

ossim_uint32 ossimGeneralRasterTileSource::getImageTileWidth() const
{
   return 0;
}

ossim_uint32 ossimGeneralRasterTileSource::getImageTileHeight() const
{
   return 0;
}

ossimString ossimGeneralRasterTileSource::getShortName()const
{
   return ossimString("ras");
}

ossimString ossimGeneralRasterTileSource::getLongName()const
{
   return ossimString("general raster reader");
}

ossim_uint32 ossimGeneralRasterTileSource::getNumberOfInputBands() const
{
   return theImageData.numberOfBands();
}

ossim_uint32 ossimGeneralRasterTileSource::getNumberOfOutputBands() const
{
   return getNumberOfInputBands();
}

ossimKeywordlist ossimGeneralRasterTileSource::getHdrInfo(ossimFilename hdrFile)
{
   ossimKeywordlist kwl;
   char delimeter = ' ';
   kwl.change_delimiter(delimeter);
   kwl.addFile(hdrFile);
   kwl.downcaseKeywords();

   ossimKeywordlist geoKwl;
   ossim_uint32 lines = 0;
   ossim_uint32 samples = 0;
   ossim_float32 noData = -9999;
   ossimString scalarType = "ossim_uint8";
   ossim_int32 numBands = 1;
   ossim_int32 skipBytes = 0;
   ossim_int32 numBits = -1; 
   ossimString chPixelType = "N"; // not defined
   ossimString interleaveType = "BIL";
   ossimString byteOrder;
   bool noDataFound = false;

   const char* lookup = kwl.find("ncols");
   if (lookup)
   {
      samples = ossimString(lookup).toUInt32();
      geoKwl.add(ossimKeywordNames::NUMBER_SAMPLES_KW, samples);
   }

   lookup = kwl.find("nrows");
   if (lookup)
   {
      lines = ossimString(lookup).toUInt32();
      geoKwl.add(ossimKeywordNames::NUMBER_LINES_KW, lines);
   }

   lookup = kwl.find("skipbytes");
   if (lookup)
   {
      skipBytes = ossimString(lookup).toInt();
   }

   lookup = kwl.find("nbands");
   if (lookup)
   {
      numBands = ossimString(lookup).toInt();
   }

   lookup = kwl.find("nodata");
   if (lookup)
   {
      noData = ossimString(lookup).toFloat32();
      noDataFound = true;
   }
   lookup = kwl.find("nodata_value");
   if (lookup)
   {
      noData = ossimString(lookup).toFloat32();
      noDataFound = true;
   }

   lookup = kwl.find("nbits");
   if (lookup)
   {
      numBits = ossimString(lookup).toInt();
   }

   lookup = kwl.find("pixeltype");
   if (lookup)
   {
      chPixelType = ossimString(lookup);
   }

   lookup = kwl.find("layout");
   if (lookup)
   {
      interleaveType = ossimString(lookup);
   }

   lookup = kwl.find("byteorder");
   if (lookup)
   {
      byteOrder = ossimString(lookup);
   }

   if (numBits == -1)
   {
      FILE* fp;
      ossim_int64 size = 0;
      fp = fopen(theImageFile.c_str(), "r");
      if (fp != 0)
      {
         fseek(fp, 0, SEEK_END);
         size = ftell(fp);
      }
      fclose(fp);

      if (lines > 0 && samples > 0)
      {
         ossim_int32 numBytes = size/samples/lines/numBands;
         if( numBytes > 0 && numBytes != 3 )
         {
            numBits = numBytes*8;

            if( numBytes == 4 )
            {
               chPixelType = "F";
            }
         }
      }
   }

   if( numBits == 16 )
   {
      if (chPixelType == "S")
      {
         scalarType = "ossim_sint16";
      }
      else
      {
         scalarType = "ossim_uint16"; // default
      }
   }
   else if( numBits == 32 )
   {
      if( chPixelType == "S")
      {
         scalarType = "ossim_sint32";
      }
      else if( chPixelType == "F")
      {
         scalarType = "ossim_float32";
      }
      else
      {
         scalarType = "ossim_uint32"; // default 
      }
   }
   else if( numBits == 8 )
   {
      scalarType = "ossim_uint8";
      numBits = 8;
   }
   else if( numBits < 8 && numBits >= 1 )
   {
      scalarType = "ossim_uint8";
   }
   else if(numBits == -1)
   {
      if( chPixelType == "F")
      {
         scalarType = "ossim_float32";
         numBits = 32;
      }
      else
      {
         scalarType = "ossim_uint8";
         numBits = 8;
      }
   }

   if (noDataFound)
   {
      for (ossim_int32 i = 0; i < numBands; i++)
      {
         ossimString prefix = "band" + ossimString::toString(i+1) + ": ";
         geoKwl.add(prefix, ossimKeywordNames::NULL_VALUE_KW, noData);
      }
   }

   geoKwl.add(ossimKeywordNames::NUMBER_BANDS_KW, numBands);
   geoKwl.add(ossimKeywordNames::SCALAR_TYPE_KW, scalarType);
   geoKwl.add(ossimKeywordNames::INTERLEAVE_TYPE_KW, interleaveType);

   return geoKwl;
}

ossimKeywordlist ossimGeneralRasterTileSource::getXmlInfo(ossimFilename xmlFile)
{
   ossimKeywordlist kwl;
   ossimFgdcXmlDoc file;
   if (file.open(xmlFile))
   {

      ossimString scalarType = "ossim_uint8";
      ossim_int32 numBits = -1; 
      ossimString interleaveType = "BIL";

      ossimIpt size;
      ossim_int32 samples = 0;
      ossim_int32 lines = 0;
      if (file.getImageSize(size))
      {
         samples = size.x;
         lines = size.y;
      }
      if (samples > 0)
      {
         kwl.add(ossimKeywordNames::NUMBER_SAMPLES_KW, samples);
      }
      if (lines > 0)
      {
         kwl.add(ossimKeywordNames::NUMBER_LINES_KW, lines);
      }

      ossim_int32 bands = file.getNumberOfBands();
      if (bands > 0)
      {
         kwl.add(ossimKeywordNames::NUMBER_BANDS_KW, bands);
      }
      else
      {
         if (samples > 0 && lines > 0)//if there is no bands info but samples and lines info, default number of bands to 1
         {
            bands = 1;
            kwl.add(ossimKeywordNames::NUMBER_BANDS_KW, bands);
         }
      }

      ossimString eainfo;
      file.getPath("/metadata/eainfo/detailed/enttyp/enttypd", eainfo);

      if (numBits == -1)
      {
         if ( (lines > 0) && (samples > 0) && (bands > 0) )
         {
            ossim_int64 size = theImageFile.fileSize();            
            ossim_int32 numBytes = size/samples/lines/bands;
            if( numBytes > 0 && numBytes != 3 )
            {
               numBits = numBytes*8;
            }
         }
      }

      if( numBits == 16 )
      {
         scalarType = "ossim_uint16"; // default
      }
      else if( numBits == 32 )
      {
         if(eainfo.contains("float"))
         {
            scalarType = "ossim_float32";
         }
         else
         {
            scalarType = "ossim_uint32"; // default 
         }
      }
      else if( numBits == 8 )
      {
         scalarType = "ossim_uint8";
         numBits = 8;
      }
      else if( numBits < 8 && numBits >= 1 )
      {
         scalarType = "ossim_uint8";
      }

      kwl.add(ossimKeywordNames::SCALAR_TYPE_KW, scalarType);
      kwl.add(ossimKeywordNames::INTERLEAVE_TYPE_KW, interleaveType);
   }
   return kwl;
}

ossimRefPtr<ossimImageGeometry> ossimGeneralRasterTileSource::getImageGeometry()
{
   if ( !theGeometry.valid() )
   {
      // Check for external geom:
      theGeometry = getExternalImageGeometry();

      if ( !theGeometry.valid() )
      {
         theGeometry = new ossimImageGeometry();
         
         ossimString ext = theImageFile.ext();
         ossimFilename hdrFile = theImageFile;
         ossimFilename xmlFile = theImageFile;
         hdrFile = hdrFile.setExtension("hdr");
         xmlFile = xmlFile.setExtension("xml");
         if (hdrFile.exists())
         {
            ossimKeywordlist geoKwl;
            ossimKeywordlist kwl(hdrFile, ' ');
            kwl.downcaseKeywords();
            
            ossim_uint32 lines = 0;
            ossim_uint32 samples = 0;
            ossim_float32 ll_lon = 0.0;
            ossim_float32 ll_lat = 0.0;
            ossim_float32 xCellSize = 1.0;
            ossim_float32 yCellSize = 1.0;
            
            const char* lookup = kwl.find("ncols");
            if (lookup)
            {
               samples = ossimString(lookup).toUInt32();
               geoKwl.add(ossimKeywordNames::NUMBER_SAMPLES_KW, samples);
            }
            
            lookup = kwl.find("nrows");
            if (lookup)
            {
               lines = ossimString(lookup).toUInt32();
               geoKwl.add(ossimKeywordNames::NUMBER_LINES_KW, lines);
            }
            
            lookup = kwl.find("cellsize");
            if (lookup)
            {
               xCellSize = ossimString(lookup).toFloat32();
               yCellSize = xCellSize;
               geoKwl.add(ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT, yCellSize);
               geoKwl.add(ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON, xCellSize);
            }
            
            lookup = kwl.find("xdim");
            if (lookup)
            {
               xCellSize = ossimString(lookup).toFloat32();
               geoKwl.add(ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON, xCellSize);
            }
            
            lookup = kwl.find("ydim");
            if (lookup)
            {
               yCellSize = ossimString(lookup).toFloat32();
               geoKwl.add(ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT, yCellSize);
            }
            
            lookup = kwl.find("xllcenter");
            if (lookup)
            {
               ossim_float32 centerX = ossimString(lookup).toFloat32();
               ll_lon = centerX + xCellSize * 0.5;
               geoKwl.add(ossimKeywordNames::TIE_POINT_LON_KW, ll_lon);
            }
            
            lookup = kwl.find("yllcenter");
            if (lookup)
            {
               ossim_float32 centerY = ossimString(lookup).toFloat32();
               ll_lat = (centerY + (lines - 1) * yCellSize) + yCellSize * 0.5;
               geoKwl.add(ossimKeywordNames::TIE_POINT_LAT_KW, ll_lat);
            }
            
            lookup = kwl.find("xllcorner");
            if (lookup)
            {
               ll_lon = ossimString(lookup).toFloat32();
               geoKwl.add(ossimKeywordNames::TIE_POINT_LON_KW, ll_lon);
            }
            
            lookup = kwl.find("yllcorner");
            if (lookup)
            {
               ossim_uint32 centerY = ossimString(lookup).toFloat32();
               ll_lat = centerY + lines * yCellSize;
               geoKwl.add(ossimKeywordNames::TIE_POINT_LAT_KW, ll_lat);
            }
            
            lookup = kwl.find("ulxmap");
            if (lookup)
            {
               ll_lon = ossimString(lookup).toFloat32();
               geoKwl.add(ossimKeywordNames::TIE_POINT_LON_KW, ll_lon);
            }
            
            lookup = kwl.find("ulymap");
            if (lookup)
            {
               ossim_uint32 centerY = ossimString(lookup).toFloat32();
               ll_lat = centerY + lines * yCellSize;
               geoKwl.add(ossimKeywordNames::TIE_POINT_LAT_KW, ll_lat);
            }
            
            kwl.add(ossimKeywordNames::ORIGIN_LATITUDE_KW, ll_lat);
            
            geoKwl.add(ossimKeywordNames::TYPE_KW, "ossimEquDistCylProjection");
            
            geoKwl.add(ossimKeywordNames::DATUM_KW, ossimDatumFactory::instance()->wgs84()->
                       code());
            
            ossimRefPtr<ossimProjection> proj = ossimMapProjectionFactory::instance()->
               createProjection(geoKwl);

            if ( proj.valid() )
            {
               theGeometry->setProjection(proj.get());
            }
         }
         else if (xmlFile.exists())
         {
            ossimFgdcXmlDoc file;
            if ( file.open(xmlFile) )
            {
               ossimRefPtr<ossimProjection> proj = file.getProjection();
               if ( !proj.valid() )
               {
                  proj = file.getGridCoordSysProjection();
               }
               if ( proj.valid() )
               {
                  theGeometry->setProjection(proj.get());
               }
            }
            
         } // xml file exist...

      } // Matches second if ( !theGeometry.valid() )

      //---
      // WARNING:
      // Must have theGeometry at this point or the next call to
      // ossimImageGeometryRegistry::extendGeometry will put us in an infinite loop
      // as it does a recursive call back to ossimImageHandler::getImageGeometry().
      //---         
      
      // Check for set projection.
      if ( !theGeometry->getProjection() )
      {
         // Try factories for projection.
         ossimImageGeometryRegistry::instance()->extendGeometry(this);
      }

      // Set image things the geometry object should know about.
      initImageParameters( theGeometry.get() );
      
   } // Matches first if ( !theGeometry.valid() )

   return theGeometry;
}
