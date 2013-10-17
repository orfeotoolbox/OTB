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
//  $Id: ossimGeneralRasterTileSource.cpp 22364 2013-08-08 13:32:18Z dburken $

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
      m_tile(0),
      m_buffer(0),
      m_lineBuffer(0),
      m_bufferInterleave(OSSIM_BIL),
      m_fileStrList(0),
      m_rasterInfo(),
      m_bufferRect(0, 0, 0, 0),
      m_swapBytesFlag(false),
      m_bufferSizeInPixels(0),
      m_outputBandList(0)
{}

ossimGeneralRasterTileSource::~ossimGeneralRasterTileSource()
{
   close();
}

ossimRefPtr<ossimImageData> ossimGeneralRasterTileSource::getTile(
   const ossimIrect& tile_rect, ossim_uint32 resLevel)
{
   if ( m_tile.valid() == false )
   {
      allocateTile(); // First time through...
   }
   
   if (m_tile.valid())
   {
      // Image rectangle must be set prior to calling getTile.
      m_tile->setImageRectangle(tile_rect);
      
      if ( getTile( m_tile.get(), resLevel ) == false )
      {
         if (m_tile->getDataObjectStatus() != OSSIM_NULL)
         {
            m_tile->makeBlank();
         }
      }
   }
   
   return m_tile;
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

            // Initialize the tile if needed as we're going to stuff it.
            if (result->getDataObjectStatus() == OSSIM_NULL)
            {
               result->initialize();
            }

            ossimIrect clip_rect = tile_rect.clipToRect(image_rect);

            if ( ! tile_rect.completely_within(m_bufferRect) )
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

               // Reallocate the buffer if needed.
               if ( m_bufferSizeInPixels != result->getSize() )
               {
                  allocateBuffer( result );
               }

               ossimIpt size(static_cast<ossim_int32>(result->getWidth()),
                             static_cast<ossim_int32>(result->getHeight()));

               if( !fillBuffer(clip_rect.ul(), size) )
               {
                  ossimNotify(ossimNotifyLevel_WARN)
                     << "Error from fill buffer..."
                     << std::endl;
                  //---
                  // Error in filling buffer.
                  //---
                  setErrorStatus();
                  status = false;
               }
            }
            
            result->loadTile(m_buffer,
                             m_bufferRect,
                             clip_rect,
                             m_bufferInterleave);
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

bool ossimGeneralRasterTileSource::fillBuffer(const ossimIpt& origin, const ossimIpt& size)
{

   static const char MODULE[] = "ossimGeneralRasterTileSource::fillBuffer";

   // Note:  InterleaveType enumerations in "constants.h" file.
   bool status = false;
   switch ( m_rasterInfo.interleaveType() )
   {
      case OSSIM_BIP:
      {
         status = fillBIP(origin, size);
         break;
      }
      case OSSIM_BIL:
      {
         status = fillBIL(origin, size);
         break;
      }
      case OSSIM_BSQ:
      {
         status = fillBSQ(origin, size);
         break;
      }
      case OSSIM_BSQ_MULTI_FILE:
      {
         status = fillBsqMultiFile(origin, size);
         break;
      }
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " ERROR:\n"
            << " Unsupported interleave type:  "
            << ILUT.getEntryString(m_rasterInfo.interleaveType())
            << std::endl;
      }
   }
   
   if (status && m_swapBytesFlag)
   {
      ossimEndian oe;
      oe.swap(m_rasterInfo.getImageMetaData().getScalarType(),
              m_buffer,
              m_bufferSizeInPixels);
   }

   return status;
}

bool ossimGeneralRasterTileSource::fillBIP(const ossimIpt& origin, const ossimIpt& size )
{
   static const char MODULE[] = "ossimGeneralRasterTileSource::fillBIP ";

   m_bufferRect.set_ul(origin);
   m_bufferRect.set_lry(min( (origin.y + size.y -1),
                              m_rasterInfo.imageRect().lr().y));
   m_bufferRect.set_lrx(min( (origin.x + size.x -1),
                              m_rasterInfo.imageRect().lr().x));

   const ossim_int32 WIDTH                  = static_cast<ossim_int32>( m_bufferRect.width() ); 
   const ossim_int32 HEIGHT                 = static_cast<ossim_int32>( m_bufferRect.height() ); 
   const ossim_int32 INPUT_BANDS            = m_rasterInfo.numberOfBands();
   const ossim_int32 OUTPUT_BANDS           = static_cast<ossim_int32>( m_outputBandList.size() );
   const ossim_int32 BYTES_PER_PIXEL        = m_rasterInfo.bytesPerPixel();
   const ossim_int32 INPUT_BYTES_PER_SAMPLE = BYTES_PER_PIXEL * INPUT_BANDS;
   const ossim_int32 OUTPUT_BYTES_PER_SAMPLE = BYTES_PER_PIXEL * OUTPUT_BANDS;
   
   // Seek position.
   std::streamoff rasterOffset = m_rasterInfo.offsetToFirstValidSample() +
        origin.y * m_rasterInfo.bytesPerRawLine() +
        origin.x * INPUT_BYTES_PER_SAMPLE;

    // Input line buffer, all bands.
   std::streamsize inputLineBufferWidth = WIDTH * INPUT_BYTES_PER_SAMPLE;
  
   // Output buffer width:
   std::streamsize outputLineBufferWidth = WIDTH * OUTPUT_BYTES_PER_SAMPLE;

#if 0 /* Please keep for debug. (drb) */
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\nDEBUG:"
         << "\norigin:                 " << origin
         << "\nSeek position:          " << rasterOffset
         << "\ninputLineBufferWidth:   " << inputLineBufferWidth
         << "\noutputLineBufferWidth:  " << outputLineBufferWidth
         << "\nINPUT_BANDS:            " << INPUT_BANDS
         << "\nOUTPUT_BANDS:           " << OUTPUT_BANDS
         << std::endl;
#endif
   
   ossim_int32 bufferOffset = 0;   
   
   // Line loop:
   ossim_int32 currentLine = 0;
   while ( currentLine < HEIGHT )
   {
      // Seek to line.
      m_fileStrList[0]->seekg(rasterOffset, ios::beg);
      if (!(*m_fileStrList[0]))
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " ERROR:\n"
            << " Seek error!  Returning with error..." << std::endl;
         return false;
      }

      // Read image data from line for all bands into line buffer.   
      m_fileStrList[0]->read( (char*)m_lineBuffer, inputLineBufferWidth );
      if ( m_fileStrList[0]->gcount() != inputLineBufferWidth ) 
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << "\nERROR:  Reading image line." << std::endl;
         return false;
      }
      
      // Sample loop:
      for ( ossim_int32 sample = 0; sample < WIDTH; ++sample )
      {
         // Band loop:
         for ( ossim_int32 band = 0; band < OUTPUT_BANDS; ++band )
         {
            ossim_int32 selectedBand = static_cast<ossim_int32>(m_outputBandList[band]);
            memcpy( (void*)(m_buffer + bufferOffset +
                            sample * OUTPUT_BYTES_PER_SAMPLE +
                            band * BYTES_PER_PIXEL),
                    (void*)(m_lineBuffer +
                            sample * INPUT_BYTES_PER_SAMPLE + 
                            selectedBand * BYTES_PER_PIXEL),
                    BYTES_PER_PIXEL );
         }
      }
               
      ++currentLine;
      bufferOffset += outputLineBufferWidth;
      rasterOffset += m_rasterInfo.bytesPerRawLine();
   }
   
   return true;
   
} // End: bool ossimGeneralRasterTileSource::fillBipBandSelect(...

bool ossimGeneralRasterTileSource::fillBIL(const ossimIpt& origin,
                                           const ossimIpt& size)
{
   static const char MODULE[] = "ossimGeneralRasterTileSource::fillBIL";
   
   //***
   // This will fill a buffer the full width of valid samples * tileHeight().
   //***
   m_bufferRect.set_ul(origin);
   m_bufferRect.set_lry(min((origin.y + size.y - 1),
                             m_rasterInfo.imageRect().lr().y));
   m_bufferRect.set_lrx(min((origin.x + size.x - 1),
                             m_rasterInfo.imageRect().lr().x));
   
   ossim_sint64 currentLine = origin.y;

   // Bytes in one line all bands.
   const std::streamoff LINE_OFFSET =
      m_rasterInfo.bytesPerRawLine() * m_rasterInfo.numberOfBands();

   // Start seek position.
   std::streamoff offset = ( m_rasterInfo.offsetToFirstValidSample() +
                             currentLine * LINE_OFFSET +
                             origin.x * m_rasterInfo.bytesPerPixel() );

   //---
   // Loop through and process lines. 
   //---
   ossim_int32 linesProcessed = 0;
   std::streamsize buffer_width = m_bufferRect.width() * m_rasterInfo.bytesPerPixel();
   ossim_uint8* buf = m_buffer;

#if 0 /* Please leave for debug.  (drb) */
   ossimNotify(ossimNotifyLevel_DEBUG)
      << "\nDEBUG:"
      << "\norigin:                 " << origin
      << "\nSeek position:          " << offset
      << "\nStarting line number:   " << currentLine
      << "\nbuffer_width:           " << buffer_width << std::endl;
#endif

   // Line loop:
   while ((currentLine <= static_cast<ossim_sint64>(m_rasterInfo.imageRect().lr().y)) &&
          linesProcessed < size.y)
   {
      // Band loop:
      std::vector<ossim_uint32>::const_iterator i = m_outputBandList.begin();
      while ( i != m_outputBandList.end() )
      {
         ossim_int64 band = static_cast<ossim_sint64>( (*i) );
         const std::streamoff bandOffset = band * m_rasterInfo.bytesPerRawLine();
         
         // Seek to line.
         m_fileStrList[0]->seekg(offset + bandOffset, ios::beg);
         if (!m_fileStrList[0])
         {
            theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " ERROR:\n"
               << " Seek error!  Returning with error..." << std::endl;
            return false;
         }
         
         // Read the line of image data.   
         m_fileStrList[0]->read( (char*)buf, buffer_width );

         if ( m_fileStrList[0]->gcount() != buffer_width ) 
         {
            theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << "\nERROR:  Reading image line."
               << "\ncurrentLine:  " << currentLine << std::endl;
            return false;
         }
         
         buf += buffer_width;
         ++i;

      } // End of band loop.
      
      ++linesProcessed;
      ++currentLine;
      offset += LINE_OFFSET;
      
   } // End: line loop
   
   return true;
}
   
//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimGeneralRasterTileSource::fillBSQ(const ossimIpt& origin,
                                           const ossimIpt& size)
{
   static const char MODULE[] = "ossimGeneralRasterTileSource::fillBSQ";

   // This will fill a buffer the full width of valid samples * tileHeight().

   m_bufferRect.set_ul(origin);
   
   m_bufferRect.set_lry(min((origin.y + size.y -1),
                             m_rasterInfo.imageRect().lr().y));
   m_bufferRect.set_lrx(min((origin.x + size.x - 1),
                             m_rasterInfo.imageRect().lr().x));
   
   // Start seek position.
   std::streamoff startSeekPosition
      = m_rasterInfo.offsetToFirstValidSample() +
        origin.y * m_rasterInfo.bytesPerRawLine() +
        origin.x * m_rasterInfo.bytesPerPixel();

   std::streamsize buffer_width = m_bufferRect.width() * m_rasterInfo.bytesPerPixel();
   
   ossim_uint8* buf = (ossim_uint8*)m_buffer;

   std::streamoff bandOffset
      = m_rasterInfo.bytesPerRawLine() * m_rasterInfo.rawLines();

#if 0 /* Please leave for debug. (drb) */
   ossimNotify(ossimNotifyLevel_DEBUG)
      << "\nDEBUG:"
      << "\norigin:                 " << origin
      << "\nSeek position:          " << startSeekPosition
      << "\nStarting line number:   " << origin.y
      << "\nbuffer_width:           " << buffer_width
      << "\nbytesPerRawLine():      "
      << m_rasterInfo.bytesPerRawLine()
      << "\nm_rasterInfo.offsetToFirstValidSample():  "
      << m_rasterInfo.offsetToFirstValidSample()
      << "\nbandOffset: " << bandOffset << std::endl;
#endif

   ossim_int32 num_bands = m_rasterInfo.numberOfBands();
   ossim_int32 height    = size.y;

   // Band loop:
   for (ossim_int32 band = 0; band < num_bands; ++band)
   {
      ossim_sint64 currentLine    = origin.y;
      ossim_sint64 linesProcessed = 0;

      std::streamoff offset = startSeekPosition + (band * bandOffset);

      // Line loop:
      while (currentLine <= m_rasterInfo.imageRect().lr().y &&
             linesProcessed < height)
      {
         // Seek to line.
         m_fileStrList[0]->seekg(offset, ios::beg);
         if (!m_fileStrList[0])
         {
            theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " ERROR:\n"
               << " Seek error!  Returning with error..." << std::endl;
            return false;
         }
         
         // Read the line of image data.   
         m_fileStrList[0]->read( (char*)buf, buffer_width );
         if ( m_fileStrList[0]->gcount() != buffer_width ) 
         {
            theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << "\nERROR:  Reading image line."
               << "\ncurrentLine:  " << currentLine << std::endl;
            return false;
         }

         // Increment everybody accordingly.
         buf += buffer_width;
         offset += m_rasterInfo.bytesPerRawLine();
         ++linesProcessed;
         ++currentLine;

      } // End of line loop.

   } // End of band loop.
   
   return true;
}

//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimGeneralRasterTileSource::fillBsqMultiFile(const ossimIpt& origin, const ossimIpt& size)
{
   static const char MODULE[] = "ossimGeneralRasterTileSource::fillBsqMultiFile";

   if (traceDebug()) CLOG << " Entered..." << std::endl;
   

   // This will fill a buffer the full width of valid samples * tileHeight().
   m_bufferRect.set_ul(origin);
   
   m_bufferRect.set_lry(min((origin.y + size.y -1),
                             m_rasterInfo.imageRect().lr().y));
   m_bufferRect.set_lrx(min((origin.x + size.x - 1),
                             m_rasterInfo.imageRect().lr().x));
   
   //---
   // Start seek position.
   //---
   std::streamoff startSeekPosition = m_rasterInfo.offsetToFirstValidSample() +
      origin.y * m_rasterInfo.bytesPerRawLine() +
      origin.x * m_rasterInfo.bytesPerPixel();
   
   //---
   // Loop through and process lines. 
   //---
   std::streamsize buffer_width = m_bufferRect.width() * m_rasterInfo.bytesPerPixel();
   
   ossim_uint8* buf = (ossim_uint8*)m_buffer;

#if 0
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\nDEBUG:"
         << "\norigin:                 " << origin
         << "\nSeek position:          " << startSeekPosition
         << "\nStarting line number:   " << origin.y
         << "\nbuffer_width:           " << buffer_width
         << "\nbuffer_rect:            " << m_bufferRect
         << "\nbytesPerRawLine():      "
         << m_rasterInfo.bytesPerRawLine()
         << "\nm_rasterInfo.offsetToFirstValidSample():  "
         << m_rasterInfo.offsetToFirstValidSample() << std::endl;
   }
#endif

   // ossim_int32 num_bands = m_rasterInfo.numberOfBands();
   std::vector<ossim_uint32>::const_iterator bandIter = m_outputBandList.begin();
   while ( bandIter != m_outputBandList.end() )
   {
      ossim_int32 currentLine    = origin.y;
      ossim_int32 linesProcessed = 0;
      ossim_int64 offset         = startSeekPosition;
      
      while (currentLine <= m_rasterInfo.imageRect().lr().y && linesProcessed < size.y)
      {
         //---
         // Seek to line.
         //---
         m_fileStrList[ *bandIter ]->seekg(offset, ios::beg);

         if ( !m_fileStrList[ *bandIter ] )
         {
            theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " ERROR:\n"
               << " Seek error!  Returning with error..." << std::endl;
            return false;
         }
         
         //---
         // Read the line of image data.   
         //---
         m_fileStrList[ *bandIter ]->read((char*)buf, buffer_width);
         
         if ( m_fileStrList[ *bandIter ]->gcount() != buffer_width) 
         {
            theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << "\nERROR:  Reading image line."
               << "\ncurrentLine:  " << currentLine << std::endl;
            return false;
         }

         // Increment everybody accordingly.
         buf += buffer_width;
         offset += m_rasterInfo.bytesPerRawLine();
         ++linesProcessed;
         ++currentLine;
         
      } // End of line loop.

      ++bandIter; // Next band...

   } // End: while ( bandIter ! = m_outputBandList.end() )
   
   return true;
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimGeneralRasterTileSource::saveState(ossimKeywordlist& kwl,
                                             const char* prefix) const
{   
   // Our stuff:
   m_rasterInfo.saveState(kwl, prefix);

   // Base class:
   bool result = ossimImageHandler::saveState(kwl, prefix);
   
   if ( result && isBandSelector() && m_outputBandList.size() )
   {
      if ( isIdentityBandList( m_outputBandList ) == false )
      {
         // If we're not identity output the bands.
         ossimString bandsString;
         ossim::toSimpleStringList(bandsString, m_outputBandList);
         kwl.add(prefix,
                 ossimKeywordNames::BANDS_KW,
                 bandsString,
                 true);
      }
   }
   
   return result;
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimGeneralRasterTileSource::loadState(const ossimKeywordlist& kwl,
                                             const char* prefix)
{
   bool result = false;
   m_outputBandList.clear();

   if ( ossimImageHandler::loadState(kwl, prefix) )
   {  
      // Set the band list if key is present.
      std::string pfx = ( prefix ? prefix : "" );
      std::string key = ossimKeywordNames::BANDS_KW;
      ossimString value;
      value.string() = kwl.findKey( pfx, key );
      if ( value.size() )
      {
         ossim::toSimpleVector( m_outputBandList, value );
      }
      result = open();
   }
   return result;
}

//*******************************************************************
// Public method:
//*******************************************************************
ossimScalarType ossimGeneralRasterTileSource::getOutputScalarType() const
{
   return m_rasterInfo.getImageMetaData().getScalarType();
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimGeneralRasterTileSource::getTileWidth() const
{
   ossim_uint32 result = 0;
   if ( m_tile.valid() )
   {
      m_tile->getWidth();
   }
   else
   {
      ossimIpt tileSize;
      ossim::defaultTileSize(tileSize);
      result = tileSize.x;
   }
   return result;
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimGeneralRasterTileSource::getTileHeight() const
{
   ossim_uint32 result = 0;
   if ( m_tile.valid() )
   {
      m_tile->getHeight();
   }
   else
   {
      ossimIpt tileSize;
      ossim::defaultTileSize(tileSize);
      result = tileSize.y;
   }
   return result;
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
         << std::endl;
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
      return m_rasterInfo.validLines();
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
ossim_uint32 ossimGeneralRasterTileSource::getNumberOfSamples(
   ossim_uint32 reduced_res_level) const
{
   if (reduced_res_level == 0)
   {
      return m_rasterInfo.validSamples();
   }
   else if (theOverview.valid())
   {
      return theOverview->getNumberOfSamples(reduced_res_level);
   }

   return 0;
}

double ossimGeneralRasterTileSource::getNullPixelValue(ossim_uint32 band) const
{
   return m_rasterInfo.getImageMetaData().getNullPix(band);
}

double ossimGeneralRasterTileSource::getMinPixelValue(ossim_uint32 band)const
{
   return m_rasterInfo.getImageMetaData().getMinPix(band);
}

double ossimGeneralRasterTileSource::getMaxPixelValue(ossim_uint32 band)const
{
   return m_rasterInfo.getImageMetaData().getMaxPix(band);
}

bool ossimGeneralRasterTileSource::open()
{
   static const char MODULE[] = "ossimGeneralRasterTileSource::open";

   if (traceDebug()) CLOG << " Entered..." << std::endl;
   
   bool result = false;
   
   if(isOpen())
   {
      close();
   }
   
   //---
   // Find the header file:
   //
   // We need lines, samples, bands, scalar and interleave at a minimum:
   // 
   // A general raster image requires a keyword list to get essential image
   // information or meta data as its sometimes called.  The meta data file
   // can have four types of extensions: ".omd", ".hdr", ".kwl" and xml.
   // Look for them in that order.
   // Note that the ".omd" extension is for "Ossim Meta Data" and was made
   // up to avoid conflicting with other software packages ".hdr" files.
   //---
   if ( m_rasterInfo.open( theImageFile ) )
   {
      theMetaData = m_rasterInfo.getImageMetaData();
      
      result = initializeHandler();
      if ( result )
      {
         completeOpen();

         if ( isBandSelector() && m_outputBandList.size() && ( isIdentityBandList( m_outputBandList ) == false ) )
         {
            // This does range checking and will pass to overview if open.
            setOutputBandList( m_outputBandList );
         }
      }
   }

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " Exit status: " << (result?"true":"false") << std::endl;
   }
   return result;
}
                                             
bool ossimGeneralRasterTileSource::open( const ossimGeneralRasterInfo& info )
{
   if( isOpen() )
   {
      close();
   }
   
   m_rasterInfo = info;
   
   if( initializeHandler() )
   {
      completeOpen();  

       if ( isBandSelector() && m_outputBandList.size() && ( isIdentityBandList( m_outputBandList ) == false ) )
       { 
          // This does range checking and will pass to overview if open.
          setOutputBandList( m_outputBandList );
       }
   }
   else
   {
      return false;
   }
   
   return true;
}

bool ossimGeneralRasterTileSource::initializeHandler()
{
   //---
   // This private method assumes that "m_rasterInfo" object has been
   // initialized.  Note that "close() should have already been called if
   // there was an open file prior to this.
   //---
   std::vector<ossimFilename> aList = m_rasterInfo.getImageFileList();

   for (ossim_uint32 i=0; i<aList.size(); ++i)
   {
      ossimFilename f = aList[i];
      
      // open it...
      ossimRefPtr<ossimIFStream> is = ossimStreamFactoryRegistry::instance()->
         createNewIFStream(f, std::ios::in|std::ios::binary);

      // check the stream...
      if(is.valid())
      {
         // Check the file stream.
         if ( is->fail() )
         {
            theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimGeneralRasterTileSource::open" << " ERROR:\n"
               << "Cannot open:  " << f.c_str() << std::endl;
            is = 0;
            return false;
         }
      }

      // Check the file size (removed).

      m_fileStrList.push_back(is); // Add it to the list...
   }

   if ((aList.size()==1) && theImageFile.empty())
   {
      theImageFile = aList[0];
   }

   // Set the buffer interleave type.
   m_bufferInterleave = m_rasterInfo.interleaveType();
   if (m_bufferInterleave == OSSIM_BSQ_MULTI_FILE)
   {
      m_bufferInterleave = OSSIM_BSQ;
   }

   if ( m_outputBandList.empty() )
   {
      // Set starting output band list to identity.
      ossimImageSource::getOutputBandList( m_outputBandList );
   }

   //---
   // Get the byte order of the image data and host machine.  If different,
   // set the swap bytes flag...
   //---
   if (m_rasterInfo.getImageDataByteOrder() != ossim::byteOrder())
   {
      m_swapBytesFlag = true;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimGeneralRasterTileSource::initializeHandler()" << " DEBUG:"
         << "\nScalar type:  "
         << ossimScalarTypeLut::instance()->
         getEntryString(m_rasterInfo.getImageMetaData().getScalarType())
         << "\nByte swapping is "
         << (m_swapBytesFlag?"enabled.":"not enabled.")
         // << "\nm_bufferSizeInPixels:  " << m_bufferSizeInPixels
         // << "\nbuffer size:  " << buffer_size
         << "\nRasterInfo:\n";
      m_rasterInfo.print(ossimNotify(ossimNotifyLevel_DEBUG));
   }
   
   return true;
}

bool ossimGeneralRasterTileSource::isOpen() const
{
   bool result = false;
   if (m_fileStrList.size() > 0)
   {
      if(m_fileStrList[0].valid())
      {
         result = !(m_fileStrList[0]->fail());
      }
   }
   return result;
}

void ossimGeneralRasterTileSource::close()
{
   ossimImageHandler::close();  // base class

   m_tile = 0; // Not a leak, ref ptr.
   
   if ( m_buffer )
   {
      delete [] m_buffer;
      m_buffer = 0;
      m_bufferSizeInPixels = 0; // Must zero out for check in getTile method.
   }

   if ( m_lineBuffer )
   {
      delete [] m_lineBuffer;
      m_lineBuffer = 0;
   }

   std::vector<ossimRefPtr<ossimIFStream> >::iterator is = m_fileStrList.begin();
   while (is != m_fileStrList.end())
   {
      (*is)->close();
      // delete (*is);
      // (*is) = 0;
      ++is;
   }
   m_fileStrList.clear();
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
   return ossimString("ossim_raster");
}

ossimString ossimGeneralRasterTileSource::getLongName()const
{
   return ossimString("general raster reader");
}

ossim_uint32 ossimGeneralRasterTileSource::getNumberOfInputBands() const
{
   return m_rasterInfo.getImageMetaData().getNumberOfBands();
}

ossim_uint32 ossimGeneralRasterTileSource::getNumberOfOutputBands() const
{
   ossim_uint32 result = 0;
   if ( isBandSelector() && m_outputBandList.size() )
   {
      result = m_outputBandList.size();
   }
   else
   {
      result = m_rasterInfo.getImageMetaData().getNumberOfBands();
   }  
   return result;
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
   // ossim_int32 skipBytes = 0;
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

   // lookup = kwl.find("skipbytes");
   // if (lookup)
   // {
   //    skipBytes = ossimString(lookup).toInt();
   // }

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

bool ossimGeneralRasterTileSource::isBandSelector() const
{
   bool result = false;
   if ( ( m_rasterInfo.interleaveType() == OSSIM_BSQ_MULTI_FILE ) ||
        ( m_rasterInfo.interleaveType() == OSSIM_BIP ) ||
        ( m_rasterInfo.interleaveType() == OSSIM_BIL ) )
   {
      result = true;
   }

   if ( result && theOverview.valid() )
   {
      result = theOverview->isBandSelector();
   }   
   return result;
}

bool ossimGeneralRasterTileSource::setOutputBandList(const std::vector<ossim_uint32>& band_list)
{
   bool result = false;
   if ( isBandSelector() )
   {
      // Making a copy as passed in list could be our m_outputBandList.
      std::vector<ossim_uint32> inputList = band_list;
      result = ossimImageHandler::setOutputBandList( inputList, m_outputBandList );
      if ( result && m_tile.valid() )
      {
         if ( m_tile->getNumberOfBands() != m_outputBandList.size() )
         {
            m_tile = 0; // Force a reinitialize on next getTile.
         }
      }
   }
   return result;
}

void ossimGeneralRasterTileSource::getOutputBandList(std::vector<ossim_uint32>& bandList) const
{
   bandList = m_outputBandList;
}

void ossimGeneralRasterTileSource::allocateTile()
{
   m_tile = 0;
   ossim_uint32 bands = 0;
   if ( m_outputBandList.empty() )
   {
      bands = m_rasterInfo.numberOfBands();
   }
   else
   {
      bands = m_outputBandList.size();
   }
   
   if ( bands )
   {
      m_tile = ossimImageDataFactory::instance()->create(
         this, m_rasterInfo.getImageMetaData().getScalarType(), bands );
      
      if ( m_tile.valid() )
      {
         // These values can be overridden by loadState...
         for(ossim_uint32 band = 0; band < bands; ++ band)
         {
            m_tile->setNullPix(m_rasterInfo.getImageMetaData().getNullPix(band), band);
            m_tile->setMinPix(m_rasterInfo.getImageMetaData().getMinPix(band), band);
            m_tile->setMaxPix(m_rasterInfo.getImageMetaData().getMaxPix(band), band);
         }
         m_tile->initialize(); // This does a makeBlank().
      }
   }
}

void ossimGeneralRasterTileSource::allocateBuffer( const ossimImageData* tile )
{
   if( m_buffer )
   {
      delete [] m_buffer;
      m_buffer = 0;
      m_bufferSizeInPixels = 0; // Must zero out for check in getTile method.
   }
   if ( m_lineBuffer )
   {
      delete [] m_lineBuffer;
      m_lineBuffer = 0;
   }
   
   if ( tile )
   {
      // Store the size of the buffer in pixels for swapping bytes.
      m_bufferSizeInPixels = tile->getSize();
      if ( m_bufferSizeInPixels )
      {
         // Initialize buffer. This is bytes, not pixels.
         m_buffer = new ossim_uint8[ tile->getSizeInBytes() ];
         
         // Zero out the buffer rect.
         m_bufferRect = ossimIrect(0, 0, 0, 0);
      }

      if ( m_rasterInfo.interleaveType() == OSSIM_BIP )
      {
         // Big enough to hold a whole line all bands.
         ossim_uint32 widthInBytes =
            tile->getWidth() * m_rasterInfo.getImageMetaData().getNumberOfBands() *
            m_rasterInfo.getImageMetaData().getBytesPerPixel();
         
         m_lineBuffer = new ossim_uint8[ widthInBytes ];
      }
   }
}
