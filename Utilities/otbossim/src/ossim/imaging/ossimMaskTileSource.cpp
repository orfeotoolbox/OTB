//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Mingjie Su
//
// Description:
//
// Contains class definition for ossimMaskTileSource.
//*******************************************************************
//  $Id: ossimMaskTileSource.cpp 1186 2010-07-28 18:57:24Z ming.su $

#include <math.h>

#include <ossim/imaging/ossimMaskTileSource.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimInterleaveTypeLut.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>

RTTI_DEF1_INST(ossimMaskTileSource,
               "ossimMaskTileSource",
               ossimImageHandler)

static ossimTrace traceDebug("ossimMaskTileSource:debug");

const ossim_uint32 ossimMaskTileSource::DEFAULT_CHAR_SIZE = 8; 

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimMaskTileSource::ossimMaskTileSource()
   :
      ossimImageHandler(),
      theTile(NULL),
      m_numberOfSamples(0),
      m_numberOfLines(0),
      m_buffer(NULL),
      m_bufferSize(0)
{}

ossimMaskTileSource::~ossimMaskTileSource()
{
   close();
}

ossimRefPtr<ossimImageData> ossimMaskTileSource::getTile(
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

bool ossimMaskTileSource::getTile(ossimImageData* result,
                                  ossim_uint32 resLevel)
{
  bool status = false;

  //---
  // Not open, this tile source bypassed, or invalid res level,
  // return a blank tile.
  //---
  if( isOpen() && isSourceEnabled() && isValidRLevel(resLevel)  )
  {
    result->ref();  // Increment ref count.

    status = true;

    ossimIrect tile_rect = result->getImageRectangle();     

    if (getImageRectangle().intersects(tile_rect))
    {
      if (!tile_rect.completely_within(getImageRectangle()) )
      {
        result->makeBlank();
      }

      fillTile(ossim_uint8(0), tile_rect, result, resLevel);
    }
    else // No intersection...
    {
      result->makeBlank();
    }

    result->unref();  // Decrement ref count.
  }

  return status;
}

//*******************************************************************
// Private Method:
//*******************************************************************
template <class T>
void ossimMaskTileSource::fillTile(T, // dummy template variable
              const ossimIrect& tile_rect, 
              ossimImageData* tile,
              ossim_uint32 resLevel)
{
  T* destinationBand = static_cast<T*>(tile->getBuf(0));

  ossim_uint32 startRow = tile_rect.ul().y;
  ossim_uint32 startCol = tile_rect.ul().x;

  ossim_int32  pixelOffset = 0;
  if (resLevel > 1)
  {
    pixelOffset = (ossim_uint32)std::pow(2.0,(ossim_int32)(resLevel -1));
  }

  ossim_uint32 rowOffset = m_numberOfSamples * startRow + startCol;
  ossim_uint32 sourceIndex = 0;
  for (ossim_uint32 line = 0; line < tile_rect.height(); line++)
  {
    ossim_uint32 linePos = startRow + line;
    for (ossim_uint32 sample = 0; sample < tile_rect.width(); sample++)
    {
      ossim_uint32 maskIndex = rowOffset + sample;
      ossim_uint32 samplePos = startCol + sample; 
      destinationBand[sourceIndex] = getPixelValue(maskIndex, 
                                                   samplePos,
                                                   linePos,
                                                   pixelOffset);
      sourceIndex++;
    }
    rowOffset += m_numberOfSamples;
  }
  tile->validate();
}

ossim_uint32 ossimMaskTileSource::getPixelValue(ossim_uint32 index,
                                                ossim_uint32 samplePos,
                                                ossim_uint32 linePos,
                                                ossim_uint32 pixelOffset)
{
  if (m_buffer != NULL)
  {
    if (pixelOffset == 0) //means level 1
    {
      ossim_uint32 bufferPos = index/DEFAULT_CHAR_SIZE;
      ossim_uint32 maskPos = index%DEFAULT_CHAR_SIZE;
      if (bufferPos < m_bufferSize)
      {
        char* new_mask = new char[DEFAULT_CHAR_SIZE];
        ossim_uint32 mask_index = 0;

        new_mask[mask_index++] = (m_buffer[bufferPos] >> 7) & 1;
        new_mask[mask_index++] = (m_buffer[bufferPos] >> 6) & 1;
        new_mask[mask_index++] = (m_buffer[bufferPos] >> 5) & 1;
        new_mask[mask_index++] = (m_buffer[bufferPos] >> 4) & 1;
        new_mask[mask_index++] = (m_buffer[bufferPos] >> 3) & 1;
        new_mask[mask_index++] = (m_buffer[bufferPos] >> 2) & 1;
        new_mask[mask_index++] = (m_buffer[bufferPos] >> 1) & 1;
        new_mask[mask_index++] = m_buffer[bufferPos] & 1;

        if (new_mask[maskPos])
        {
          delete [] new_mask;
          return 255;
        }
        else
        {
          delete [] new_mask;
          return 0;
        }
      }
    }
    else
    {
      ossim_uint32 index = samplePos * pixelOffset + pixelOffset * m_numberOfSamples * linePos;
      for (ossim_uint32 offset = 0; offset < pixelOffset; offset++)
      {
        ossim_uint32 bufferPos = index/DEFAULT_CHAR_SIZE;
        ossim_uint32 maskPos = index%DEFAULT_CHAR_SIZE;
        ossim_uint32 endMaskPos = maskPos + pixelOffset;
        if (endMaskPos > DEFAULT_CHAR_SIZE)//make endMaskPos less than default char size
        {
          endMaskPos = DEFAULT_CHAR_SIZE;
        }

        if (bufferPos < m_bufferSize)
        {
          char* new_mask = new char[DEFAULT_CHAR_SIZE];
          ossim_uint32 mask_index = 0;

          new_mask[mask_index++] = (m_buffer[bufferPos] >> 7) & 1;
          new_mask[mask_index++] = (m_buffer[bufferPos] >> 6) & 1;
          new_mask[mask_index++] = (m_buffer[bufferPos] >> 5) & 1;
          new_mask[mask_index++] = (m_buffer[bufferPos] >> 4) & 1;
          new_mask[mask_index++] = (m_buffer[bufferPos] >> 3) & 1;
          new_mask[mask_index++] = (m_buffer[bufferPos] >> 2) & 1;
          new_mask[mask_index++] = (m_buffer[bufferPos] >> 1) & 1;
          new_mask[mask_index++] = m_buffer[bufferPos] & 1;

          for (ossim_uint32 pos = maskPos; pos < endMaskPos; pos++)
          {
            if (new_mask[pos])
            {
              delete [] new_mask;
              return 255;
            }
          }
          delete [] new_mask;
          index += m_numberOfSamples;
        }
      }
      return 0;
    }
  }
  return 0;
}

//*******************************************************************
// Public method:
//*******************************************************************
ossimScalarType ossimMaskTileSource::getOutputScalarType() const
{
   return ( theTile.valid() ? theTile->getScalarType()
            : OSSIM_SCALAR_UNKNOWN );
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimMaskTileSource::getTileWidth() const
{
   return ( theTile.valid() ? theTile->getWidth() : 0 );
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimMaskTileSource::getTileHeight() const
{
   return ( theTile.valid() ? theTile->getHeight() : 0 );
}

//*******************************************************************
// Public method:
//*******************************************************************
bool
ossimMaskTileSource::isValidRLevel(ossim_uint32 reduced_res_level) const
{
   static const char MODULE[] = "ossimMaskTileSource::isValidRLevel";
   
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
ossimMaskTileSource::getNumberOfLines(ossim_uint32 reduced_res_level) const
{
   ossim_uint32 result = 0;
   if (reduced_res_level == 0)
   {
     result = m_numberOfLines;
   }
   else if (theOverview.valid())
   {
     result = theOverview->getNumberOfLines(reduced_res_level);
   }
   return result;
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimMaskTileSource::getNumberOfSamples(ossim_uint32 reduced_res_level) const
{
  ossim_uint32 result = 0;
  if (reduced_res_level == 0)
  {
    result = m_numberOfSamples;
  }
  else if (theOverview.valid())
  {
    result = theOverview->getNumberOfSamples(reduced_res_level);
  }
  return result;
}

double ossimMaskTileSource::getNullPixelValue(ossim_uint32 /*band*/) const
{
   return 0;
}

double ossimMaskTileSource::getMinPixelValue(ossim_uint32 /*band*/)const
{
   return 0;
}

double ossimMaskTileSource::getMaxPixelValue(ossim_uint32 /*band*/)const
{
   return 255;
}

bool ossimMaskTileSource::open()
{
  if (!isSupportedExtension())
  {
    close();
    return false;
  }

  if(isOpen())
  {
    close();
  }

  static const char MODULE[] = "ossimMaskTileSource::open";
  
  if (traceDebug()) CLOG << " Entered..." << endl;

  if (m_numberOfSamples == 0 && m_numberOfLines == 0)
  {
    return false;
  }
  
  std::ifstream fileStream(theImageFile.c_str(), ios::in|ios::binary);

  fileStream.seekg(0, std::ifstream::end);
  m_bufferSize = (ossim_uint32)fileStream.tellg();
  fileStream.seekg(0);

  m_buffer = new char[m_bufferSize];
  fileStream.read(m_buffer, m_bufferSize);

  if (fileStream.fail())
  {
    ossimNotify(ossimNotifyLevel_FATAL)
      << MODULE << " ERROR:"
      << "\nInput file not open:  " << theImageFile.c_str() << std::endl;
    setErrorStatus();
    fileStream.close();
    return false;
  }

  theTile = ossimImageDataFactory::instance()->create(this, this);

  theTile->initialize();

  completeOpen();

  fileStream.close();

  return true;
}

bool ossimMaskTileSource::isOpen() const
{
   if (m_buffer != NULL)
   {
     return true;
   }
   return false;
}

bool ossimMaskTileSource::isSupportedExtension()
{
  if (theImageFile.size() > 0)
  {
    ossimString ext = theImageFile.ext();
    if (ext == "mask")
    {
      return true;
    }
  }
  return false;
}

void ossimMaskTileSource::close()
{
   ossimImageHandler::close();  // base class

   if (m_buffer != NULL)
   {
     delete [] m_buffer;
   }

   theTile = NULL;
}

ossim_uint32 ossimMaskTileSource::getImageTileWidth() const
{
   return 0;
}

ossim_uint32 ossimMaskTileSource::getImageTileHeight() const
{
   return 0;
}

ossimString ossimMaskTileSource::getShortName()const
{
   return ossimString("mask");
}

ossimString ossimMaskTileSource::getLongName()const
{
   return ossimString("mask reader");
}

ossim_uint32 ossimMaskTileSource::getNumberOfInputBands() const
{
   return 1;
}

ossim_uint32 ossimMaskTileSource::getNumberOfOutputBands() const
{
   return getNumberOfInputBands();
}

void ossimMaskTileSource::setImageBoundingRect(ossimIrect rect)
{
  ossimIpt defaultTileSize;
  ossim::defaultTileSize(defaultTileSize);
  ossim_uint32 tileWidth  = static_cast<ossim_uint32>(defaultTileSize.x);
  ossim_uint32 tileHeight = static_cast<ossim_uint32>(defaultTileSize.y);
  
  ossim_uint32 tileWidthCount = rect.width()/tileWidth;
  ossim_uint32 tileWidthRemain = rect.width()%tileWidth;
  if (tileWidthRemain > 0)
  {
    tileWidthCount = tileWidthCount + 1;
  }
  m_numberOfSamples = tileWidth * tileWidthCount;

  ossim_uint32 tileHeightCount = rect.height()/tileHeight;
  ossim_uint32 tileHeightRemain = rect.height()%tileHeight;
  if (tileHeightRemain > 0)
  {
    tileHeightCount = tileHeightCount + 1;
  }
  m_numberOfLines = tileHeight * tileHeightCount;
}

