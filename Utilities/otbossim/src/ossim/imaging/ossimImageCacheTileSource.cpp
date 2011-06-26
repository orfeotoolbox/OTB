//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Mingjie Su   
//
//********************************************************************
// $Id: ossimImageCacheTileSource.cpp 2644 2011-05-26 15:20:11Z oscar.kramer $
#include <algorithm>
using namespace std;

#include <ossim/imaging/ossimImageCacheTileSource.h>

#include <ossim/base/ossimStdOutProgress.h>
#include <ossim/base/ossimNBandLutDataObject.h>
#include <ossim/imaging/ossimHistogramWriter.h>
#include <ossim/imaging/ossimImageHistogramSource.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimContainerProperty.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/support_data/ossimRpfFrame.h>
#include <ossim/imaging/ossimImageDataFactory.h>

#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimPolygon.h>

static ossimTrace traceDebug = ossimTrace("ossimImageCacheTileSource:debug");

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimImageCacheTileSource.cpp 2644 2011-05-26 15:20:11Z oscar.kramer $";
#endif

RTTI_DEF1(ossimImageCacheTileSource, "ossimImageCacheTileSource", ossimImageCacheBase)

ossimImageCacheTileSource::ossimImageCacheTileSource()
   :
   ossimImageCacheBase(),
   m_tile(0),
   m_imageHandler(0),
   m_minPixelValue(ossim::nan()),
   m_maxPixelValue(ossim::nan()),
   m_nullPixelValue(ossim::nan())
{
}

ossimImageCacheTileSource::~ossimImageCacheTileSource()
{
   if(m_imageHandler.valid())
   {
      m_imageHandler = 0;
   }
  close();
}

ossimString ossimImageCacheTileSource::getShortName()const
{
  return ossimString("ImageCache");
}

ossimString ossimImageCacheTileSource::getLongName()const
{
  return ossimString("ImageCache reader");
}


void ossimImageCacheTileSource::close()
{
  deleteAll();
}

bool ossimImageCacheTileSource::open()
{
  if(traceDebug())
  {
    ossimNotify(ossimNotifyLevel_DEBUG) << "ossimImageCacheTileSource::open(): Entered....." << std::endl;
  }

  ossimFilename imageFile = theImageFile;
  bool result = true;

  if(isOpen() == false)
  {
    close();
    result = false;
  }

  if(result)
  {
    if (m_fileNames.size() == 0 && m_frameEntryArray.size() == 0)
    {
      if (buildFrameEntryArray(imageFile) == false)
      {
        return false;
      }
    }

    //---
    // Adjust image rect so not to go over the -180 to 180 and -90 to 90
    // bounds.
    //---
    setActualImageRect();

    // Set the base class image file name.
    theImageFile = imageFile;
    m_tile = ossimImageDataFactory::instance()->create(this, this);
    m_tile->initialize();
    completeOpen();
  }

  if(traceDebug())
  {
    ossimNotify(ossimNotifyLevel_DEBUG) << "ossimImageCacheTileSource::open(): Leaving at line" << __LINE__ << std::endl;
  }

  return result;
}

bool ossimImageCacheTileSource::buildFrameEntryArray(ossimFilename imageFile)
{
   static const char MODULE[] = "ossimImageCacheTileSource::buildFrameEntryArray";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered..." << endl;
   }

   std::ifstream in((imageFile).c_str() );
   std::string line;
   ossim_uint32 index = 0; // used throughout
   ossim_uint32 frameWidth = 0;
   ossim_uint32 frameHeight = 0;
   while( in.good() )
   {
      // Read in a line.
      std::getline(in, line);
      ossimString tmpStr = ossimString(line);
      if (index == 0)
      {
         std::vector<ossimString> box_lat_lon;
         tmpStr.split(box_lat_lon, "|");
         if (box_lat_lon.size() == 4)
         {
            std::vector<ossimString> pixelInfos = box_lat_lon[3].split(",");
            if (pixelInfos.size() == 3)
            {
               m_minPixelValue = ossimString::toFloat64(pixelInfos[0]);
               m_maxPixelValue = ossimString::toFloat64(pixelInfos[1]);
               m_nullPixelValue = ossimString::toFloat64(pixelInfos[2]);
            }
         }
      }
      else if (index == 1)
      {
         std::vector<ossimString> frame_lat_lon = tmpStr.split("|");
         ossimFilename firstFile = frame_lat_lon[0];
         if(m_workFrame->parseFile(firstFile) == ossimErrorCodes::OSSIM_ERROR)
         {
            m_imageHandler = ossimImageHandlerRegistry::instance()->open(firstFile);
            if (m_imageHandler.valid())
            {
               frameWidth = m_imageHandler->getBoundingRect().width();
               frameHeight = m_imageHandler->getBoundingRect().height();
               break;
            }
         }
         else
         {
            return false;
         }
      }
      ++index;
   }
   in.close();

   return ossimImageCacheBase::buildFrameEntryArray(imageFile, frameWidth, frameHeight);
}

ossimRefPtr<ossimImageData> ossimImageCacheTileSource::getTile(
  const  ossimIrect& rect, ossim_uint32 resLevel)
{
  if (m_tile.valid())
  {
    // Image rectangle must be set prior to calling getTile.
    m_tile->setImageRectangle(rect);

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

bool ossimImageCacheTileSource::getTile(ossimImageData* result,
                                      ossim_uint32 resLevel)
{
   bool status = false;
   
   //---
   // Not open, this tile source bypassed, or invalid res level,
   // return a blank tile.
   //---
   if( isOpen() && isSourceEnabled() && isValidRLevel(resLevel) &&
       result && (result->getNumberOfBands() == getNumberOfOutputBands()))
   {
      if ( resLevel > 0 )
      {
         status = getOverviewTile(resLevel, result);
      }
      
      if (!status) // Did not get an overview tile.
      {
         status = true;
         
         ossimIrect rect = result->getImageRectangle();
         
         ossimIrect imageRect = getImageRectangle();
         
         if ( rect.intersects(imageRect) )
         {
            //---
            // Start with a blank tile in case there is not total coverage
            // for rect.
            //---
            result->makeBlank();
            
            vector<ossimFrameEntryData> frames = getIntersectingEntries(rect);
            if(frames.size() > 0)
            {
               //---
               // Now lets render each frame.  Note we will have to find
               // subframes
               // that intersect the rectangle of interest for each frame.
               //---
               fillTile(rect, frames, result);
               
               // Revalidate tile status.
               result->validate();
            }
         }
         else
         {
            result->makeBlank();
         }
      }
   }
   return status;
}

void ossimImageCacheTileSource::fillTile(
   const ossimIrect& tileRect,
   const vector<ossimFrameEntryData>& framesInvolved,
   ossimImageData* tile)
{
   ossim_uint32 idx = 0;
   for(idx = 0; idx < framesInvolved.size(); ++idx)
   {
      if (m_imageHandler.valid())
      {
         if (m_imageHandler->getFilename() != framesInvolved[idx].theFrameEntry.getFullPath())
         {
            m_imageHandler = 0;
            m_imageHandler = ossimImageHandlerRegistry::instance()->open(framesInvolved[idx].theFrameEntry.getFullPath());
         }
      }
      else
      {
         m_imageHandler = ossimImageHandlerRegistry::instance()->open(framesInvolved[idx].theFrameEntry.getFullPath());
      }

      if (m_imageHandler.valid())
      {
         if (m_imageHandler->isOpen() == false)
         {
            m_imageHandler->open(framesInvolved[idx].theFrameEntry.getFullPath());
         }

         ossimIrect frameRect(framesInvolved[idx].thePixelCol,
            framesInvolved[idx].thePixelRow,
            framesInvolved[idx].thePixelCol + m_frame_width  - 1,
            framesInvolved[idx].thePixelRow + m_frame_height - 1);

         ossimIrect clipRect = tileRect.clipToRect(frameRect);

         ossimIpt tempDelta(clipRect.ul().x - framesInvolved[idx].thePixelCol,
            clipRect.ul().y - framesInvolved[idx].thePixelRow);

         ossimIrect offsetRect(tempDelta.x,
            tempDelta.y,
            tempDelta.x + clipRect.width()-1,
            tempDelta.y + clipRect.height()-1);

         //if the current image size is different from the default size, resample the image data
         if ( ( m_imageHandler->getBoundingRect().width() != m_frame_width ) ||
              ( m_imageHandler->getBoundingRect().height() != m_frame_height ) )
         {

            //calculate the ratio
            ossim_float64 widthRatio = static_cast<ossim_float64>(m_imageHandler->getBoundingRect().width())/static_cast<ossim_float64>(m_frame_width);
            ossim_float64 heightRatio = static_cast<ossim_float64>(m_imageHandler->getBoundingRect().height())/static_cast<ossim_float64>(m_frame_height);

            //initialize the rect bounding for getting image data from image handler
            ossimDpt ul(offsetRect.ul().x*widthRatio, offsetRect.ul().y*heightRatio);
            ossimDpt lr(offsetRect.lr().x*widthRatio, offsetRect.lr().y*heightRatio);

            //---
            // Floor, ceil to avoid rect size of zero from ul, lr, delta being only fractional
            // resulting in size of 0 if cast to an int or passed to ossim<int>::round().
            //---
            ossimIrect actualOffsetRect(static_cast<ossim_int32>(std::floor(ul.x)),
                                        static_cast<ossim_int32>(std::floor(ul.y)),
                                        static_cast<ossim_int32>(std::ceil(lr.x)),
                                        static_cast<ossim_int32>(std::ceil(lr.y)));

            //get accurate rect in case the actual rect is larger than the rect of image 
            actualOffsetRect = actualOffsetRect.clipToRect(m_imageHandler->getBoundingRect());
            
            ossimRefPtr<ossimImageData> imageData = m_imageHandler->getTile(actualOffsetRect);
            if (imageData.valid())
            {
               ossimFilterResampler* resampler = new ossimFilterResampler();
               
               double denominatorY = 1.0;
               double denominatorX = 1.0;
               if(clipRect.height() > 2) // Cannot be zero.
               {
                  denominatorY = clipRect.height()-1.0;
               }
               if(clipRect.width() > 2) // Cannot be zero.
               {
                  denominatorX = clipRect.width()-1.0;
               }

               //create a tile and set the rect size
               ossimRefPtr<ossimImageData> tmpTile = (ossimImageData*)imageData->dup();
               tmpTile->setImageRectangle(offsetRect);
               tmpTile->makeBlank();

               ossimDpt deltaUl(((actualOffsetRect.ll().x - actualOffsetRect.ul().x)/denominatorX),
                                ((actualOffsetRect.ll().y - actualOffsetRect.ul().y)/denominatorY));
               ossimDpt deltaUr(((actualOffsetRect.lr().x - actualOffsetRect.ur().x)/denominatorX),
                                ((actualOffsetRect.lr().y - actualOffsetRect.ur().y)/denominatorY));
               ossimDpt length(offsetRect.width(),offsetRect.height());
               
               //resample the image data
               resampler->resample(imageData, 
                                   tmpTile,
                                   actualOffsetRect.ul(),
                                   actualOffsetRect.ur(),
                                   deltaUl,
                                   deltaUr,
                                   length);

               tile->loadTile(tmpTile->getBuf(), clipRect, OSSIM_BSQ);

               tmpTile = 0;
               delete resampler;
               resampler = 0;
            }
         }
         else
         {
            ossimRefPtr<ossimImageData> imageData = m_imageHandler->getTile(offsetRect);
            if (imageData.valid())
            {
               tile->loadTile(imageData->getBuf(), clipRect, OSSIM_BSQ);
            }
         }
      }
   }
}

ossimScalarType ossimImageCacheTileSource::getOutputScalarType() const
{
   if (m_imageHandler.valid())
   {
      return m_imageHandler->getOutputScalarType();
   }
   return OSSIM_UCHAR;
}

double ossimImageCacheTileSource::getMinPixelValue(ossim_uint32 band)const
{
   if (ossim::isnan(m_minPixelValue))
   {
      return ossimImageHandler::getMinPixelValue(band);
   }
   return m_minPixelValue;
}

double ossimImageCacheTileSource::getMaxPixelValue(ossim_uint32 band)const
{
   if (ossim::isnan(m_maxPixelValue))
   {
      return ossimImageHandler::getMaxPixelValue(band);
   }
   return m_maxPixelValue;
}

double ossimImageCacheTileSource::getNullPixelValue(ossim_uint32 band)const
{
   if (ossim::isnan(m_nullPixelValue))
   {
      return ossimImageHandler::getNullPixelValue(band);
   }
   return m_nullPixelValue;
}

ossim_uint32 ossimImageCacheTileSource::getNumberOfInputBands()const
{
   if (m_imageHandler.valid())
   {
      return m_imageHandler->getNumberOfInputBands();
   }
   return m_numberOfBands;
}

ossim_uint32 ossimImageCacheTileSource::getNumberOfOutputBands()const
{
   if (m_imageHandler.valid())
   {
      return m_imageHandler->getNumberOfInputBands();
   }
   return m_numberOfBands;
}
