//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
//
// Contains class definition for ossimGeneralRasterTileSource.
//*******************************************************************
//  $Id: ossimGeneralRasterTileSource.cpp 13768 2008-10-22 19:32:18Z gpotts $

#include <ossim/imaging/ossimGeneralRasterTileSource.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimInterleaveTypeLut.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/base/ossimStreamFactoryRegistry.h>

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
      theTile(NULL),
      theBuffer(NULL),
      theBufferInterleave(OSSIM_BIL),
      theFileStrList(0),
      theImageData(),
      theBufferRect(0, 0, 0, 0),
      theSwapBytesFlag(false),
      theBufferSizeInPixels(0)
{}

ossimGeneralRasterTileSource::~ossimGeneralRasterTileSource()
{
   if (theBuffer)
   {
      delete [] theBuffer;
      theBuffer = NULL;
   }

   std::vector<ossimRefPtr<ossimIFStream> >::iterator is =
      theFileStrList.begin();

   while (is != theFileStrList.end())
   {
      (*is)->close();
      ++is;
   }

   theFileStrList.clear();
}

ossimRefPtr<ossimImageData> ossimGeneralRasterTileSource::getTile(
   const ossimIrect& tile_rect, ossim_uint32 resLevel)
{
   if( !isOpen() || !isSourceEnabled() || !isValidRLevel(resLevel) )
   {
      return ossimRefPtr<ossimImageData>();
   }
   
   // general rasters can have a subimage as an offset so let's make sure we 
   // shift it if its in the header
   ossimIrect zeroBasedTileRect = tile_rect - getSubImageOffset(resLevel);

   if (theOverview)
   {
      if (resLevel || theOverview->hasR0())
      {
         //---
         // Overviews are not aware of sub image offsets so request a zero
         // based rectangle.
         //---
         ossimRefPtr<ossimImageData> tile =
            theOverview->getTile(zeroBasedTileRect, resLevel);
         if (tile.valid())
         {
            //---
            // Since the overview request was zero based we must reset the
            // rectangle to be relative to any sub image offset.
            //---
            tile->setImageRectangle(tile_rect);
            
            if(getOutputScalarType() == OSSIM_USHORT11)
            {
               //---
               // Temp fix:
               // The overview handler could return a tile of OSSIM_UINT16 if
               // the max sample value was not set to 2047.
               //---
               tile->setScalarType(OSSIM_USHORT11);
            }
         }

         return tile;
      }
   }

   theTile->setImageRectangle(zeroBasedTileRect);
   
   ossimIrect image_rect = ossimIrect(0, 
				      0, 
				      getNumberOfSamples(resLevel) - 1,
				      getNumberOfLines(resLevel) - 1);

   ossimIrect clip_rect = zeroBasedTileRect.clipToRect(image_rect);
    
   checkBuffer(tile_rect);

   //***
   // Check origin to see if it falls with the image.  If not, return a
   // blank chip.
   //***
   if ( zeroBasedTileRect.intersects(image_rect) )
   {
      if ( ! zeroBasedTileRect.completely_within(theBufferRect) )
      {
         // A new buffer must be loaded.
         if ( !zeroBasedTileRect.completely_within(clip_rect) )
         {
            //***
            // Start with a blank tile since the whole tile buffer will not be
            // filled.
            //***
            theTile->makeBlank();
         }

         if(!fillBuffer(clip_rect.ul()))
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "Error from fill buffer..."
               << endl;
            //***
            // Error in filling buffer.
            //***
            setErrorStatus();
            return ossimRefPtr<ossimImageData>();
         }
      }
         
      theTile->loadTile(theBuffer,
                        theBufferRect,
                        clip_rect,
                        theBufferInterleave);
      theTile->validate();
   }
   else
   {
      // tile does not intersect the image rectangle.
      return ossimRefPtr<ossimImageData>();
   }

   return theTile;
   
}

//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimGeneralRasterTileSource::fillBuffer(const ossimIpt& origin)
{
   static const char MODULE[] = "ossimGeneralRasterTileSource::fillBuffer";

   // Note:  InterleaveType enumerations in "constants.h" file.
   bool status = false;
   switch (theImageData.interleaveType())
   {
   case OSSIM_BIP:
      status = fillBIP(origin);
      break;
   case OSSIM_BIL:
      status = fillBIL(origin);
      break;
   case OSSIM_BSQ:
      status = fillBSQ(origin);
      break;
   case OSSIM_BSQ_MULTI_FILE:
      status = fillBsqMultiFile(origin);
      break;
   default:
      ossimNotify(ossimNotifyLevel_WARN) << MODULE << " ERROR:\n"
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
bool ossimGeneralRasterTileSource::fillBIP(const ossimIpt& origin)
{
   static const char MODULE[] = "ossimGeneralRasterTileSource::fillBIP";
   
   //***
   // This will fill a buffer the full width of valid samples * getHeight().
   //***
   theBufferRect.set_ul(origin);

   theBufferRect.set_lry(min( (origin.y + (ossim_int32)theTile->getHeight() -1),
                              theImageData.imageRect().lr().y));
   theBufferRect.set_lrx(min( (origin.x + (ossim_int32)theTile->getWidth() -1),
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

   ossim_uint64 height = theTile->getHeight();
   
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
      theFileStrList[0]->read((char*)buf, buffer_width);
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
bool ossimGeneralRasterTileSource::fillBIL(const ossimIpt& origin)
{
   static const char MODULE[] = "ossimGeneralRasterTileSource::fillBIL";
   
   //***
   // This will fill a buffer the full width of valid samples * tileHeight().
   //***
   theBufferRect.set_ul(origin);
   theBufferRect.set_lry(min((origin.y + (ossim_int32)theTile->getHeight() - 1),
                             theImageData.imageRect().lr().y));
   theBufferRect.set_lrx(min((origin.x + (ossim_int32)theTile->getWidth() - 1),
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

   ossim_uint64 height    = theTile->getHeight();
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
         theFileStrList[0]->read((char*)buf, buffer_width);
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
bool ossimGeneralRasterTileSource::fillBSQ(const ossimIpt& origin)
{
   static const char MODULE[] = "ossimGeneralRasterTileSource::fillBSQ";

   //***
   // This will fill a buffer the full width of valid samples * tileHeight().
   //***
   theBufferRect.set_ul(origin);
   
   theBufferRect.set_lry(min((origin.y + (ossim_int32)theTile->getHeight() -1),
                             theImageData.imageRect().lr().y));
   theBufferRect.set_lrx(min((origin.x + (ossim_int32)theTile->getWidth() - 1),
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
   ossim_int32 height    = theTile->getHeight();
   
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
         theFileStrList[0]->read((char*)buf, buffer_width);
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
bool ossimGeneralRasterTileSource::fillBsqMultiFile(const ossimIpt& origin)
{
   static const char MODULE[]
      = "ossimGeneralRasterTileSource::fillBsqMultiFile";

   if (traceDebug()) CLOG << " Entered..." << endl;
   
   
   //***
   // This will fill a buffer the full width of valid samples * tileHeight().
   //***
   theBufferRect.set_ul(origin);
   
   theBufferRect.set_lry(min((origin.y + (ossim_int32)theTile->getHeight() -1),
                             theImageData.imageRect().lr().y));
   theBufferRect.set_lrx(min((origin.x + (ossim_int32)theTile->getWidth() - 1),
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
   ossim_int32 height    = theTile->getHeight();
   
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
         theFileStrList[band]->read((char*)buf, buffer_width);
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
// Public Method:
//*******************************************************************
// ossimIrect
// ossimGeneralRasterTileSource::getImageRectangle(ossim_uint32 reduced_res_level) const
// {
//   if(reduced_res_level < getNumberOfDecimationLevels())
//     {
//       ossimDpt decimation;
//       getDecimationFactor(reduced_res_level, decimation);
//       ossimIpt offset = theImageData.subImageOffset();
//       offset.x = ossim::round<int>(offset.x*decimation.x);
//       offset.y = ossim::round<int>(offset.y*decimation.y);
      
//       return ossimIrect(offset.x,                         // upper left x
// 			offset.y,                         // upper left y
// 			offset.x + getNumberOfSamples(reduced_res_level) - 1,  // lower right x
// 			offset.y + getNumberOfLines(reduced_res_level)   - 1); // lower right y
//     }
//   ossimIrect result;
//   result.makeNan();
//   return result;
// }
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
   else if (theOverview)
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
   else if (theOverview)
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
   else if (theOverview)
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
  if(isOpen())
  {
     close();
  }
  static const char MODULE[] = "ossimGeneralRasterTileSource::open";
  
  if (traceDebug()) CLOG << " Entered..." << endl;
  
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
           // No header file so get out.
           return false;
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
   
   theSubImageOffset = theImageData.subImageOffset();

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

   if(aList.size()==1)
   {
      theImageFile  = aList[0];
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
	  theBuffer = NULL;
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
   if (theFileStrList.size() > 0)
   {
      if(theFileStrList[0].valid())
      {
         return !(theFileStrList[0]->fail());
      }
//      return  const_cast<ifstream*>(theFileStrList[0])->is_open();
   }

   return false;
}

void ossimGeneralRasterTileSource::close()
{
   ossimImageHandler::close();  // base class

   theTile = NULL;
   if (theBuffer)
   {
      delete [] theBuffer;
      theBuffer = 0;
   }

   // Delete the list of input streams and clear the list.
//    vector<istream*>::iterator is = theFileStrList.begin();
//    while (is != theFileStrList.end())
//    {
//       delete *is;
//       *is = NULL;
//       ++is;
//    }

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

ossimString ossimGeneralRasterTileSource::className()const
{
   return ossimString("ossimGeneralRasterTileSource");
}

ossim_uint32 ossimGeneralRasterTileSource::getNumberOfInputBands() const
{
   return theImageData.numberOfBands();
}

ossim_uint32 ossimGeneralRasterTileSource::getNumberOfOutputBands() const
{
   return getNumberOfInputBands();
}
