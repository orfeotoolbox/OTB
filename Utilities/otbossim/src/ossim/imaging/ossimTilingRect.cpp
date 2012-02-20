//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Mingjie Su
// 
// Description: implementation for image generator
//
//*************************************************************************
// $Id: ossimTilingRect.cpp 2725 2011-06-15 18:13:07Z david.burken $

#include <sstream>
#include <iomanip>
#include <ossim/imaging/ossimTilingRect.h>
#include <ossim/base/ossimUnitTypeLut.h>
#include <ossim/base/ossimUnitConversionTool.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimEsriShapeFileInterface.h>

static ossimTrace traceDebug("ossimTilingRect:debug");

RTTI_DEF1(ossimTilingRect, "ossimTilingRect", ossimTiling);

ossimTilingRect::ossimTilingRect()
      : ossimTiling(),
      m_clipToAoi(true),
      m_lastWidth(0),
      m_lastHeight(0),
      m_useOffset(false),
      m_offsetInPixels(0,0),
      m_tilingDistanceInPixels(0,0),
      m_tilinResolutionDistance(ossim::nan(),ossim::nan()),
      m_tilingResolutionUnitType(OSSIM_UNIT_UNKNOWN),
      m_fractionalPixelMisalignment(ossim::nan(),ossim::nan())
{
}

ossimTilingRect::~ossimTilingRect()
{
}

bool ossimTilingRect::initializeBase(const ossimMapProjection& proj,
   const ossimIrect& boundingRect,
   ossimDpt& convertedTilingDistance)
{
   theMapProjection = (ossimMapProjection*)proj.dup();
   theImageRect     = boundingRect;

   ossimDpt gsd = theMapProjection->getMetersPerPixel();
   // Find image center and GSD at that point:
   if (theMapProjection->isGeographic() && theTilingDistanceUnitType != OSSIM_DEGREES)
   {
      ossimRefPtr<ossimImageGeometry> geom = new ossimImageGeometry (0, theMapProjection.get());
      geom->setImageSize(ossimIpt(theImageRect.width(), theImageRect.height()));
      gsd = geom->getMetersPerPixel();
   }

   m_tilingDistanceInPixels = theTilingDistance;

   //if resolution and resolution type are defined, need to reset degree/meter per pixel and the image rect. do calculation below
   ossim_float64 latDiff = 0.0;
   ossim_float64 lonDiff = 0.0;
   ossim_uint32 imageWidth = 0;
   ossim_uint32 imageHeight = 0;
   if (!m_tilinResolutionDistance.hasNans())
   {
      ossimDpt result(0,0);
      if (m_tilingResolutionUnitType == OSSIM_DEGREES)
      {
         ossimGpt ulg;
         ossimGpt llg;
         ossimGpt lrg;
         ossimGpt urg;

         theMapProjection->lineSampleToWorld(theImageRect.ul(), ulg);
         theMapProjection->lineSampleToWorld(theImageRect.ll(), llg);
         theMapProjection->lineSampleToWorld(theImageRect.lr(), lrg);
         theMapProjection->lineSampleToWorld(theImageRect.ur(), urg);

         latDiff = fabs(ulg.lat - llg.lat);
         lonDiff = fabs(urg.lon - llg.lon);

         if (!theMapProjection->isGeographic())
         {
            ossim_float64 tileHorizatalNum = lonDiff/m_tilinResolutionDistance.x;
            ossim_float64 tileVerticalNum = latDiff/m_tilinResolutionDistance.y;

            if (theTilingDistanceUnitType != OSSIM_PIXEL) //convert tiling distance to pixel if unit type is not pixel
            {
               ossimDpt convertedDis = m_tilingDistanceInPixels;
               getConvertedDistance(convertedDis, m_tilingDistanceInPixels);
               imageWidth = (ossim_uint32)convertedDis.x/gsd.x*tileHorizatalNum;
               imageHeight = (ossim_uint32)convertedDis.y/gsd.y*tileVerticalNum;
            }
            else
            {
               imageWidth = (ossim_uint32)m_tilingDistanceInPixels.x*tileHorizatalNum;
               imageHeight = (ossim_uint32)m_tilingDistanceInPixels.y*tileVerticalNum;
            }
            result = ossimDpt(theImageRect.width()*gsd.x/imageWidth, theImageRect.height()*gsd.y/imageHeight);
            gsd = result;

            theMapProjection->setMetersPerPixel(result);
         }
         else
         {
            result = ossimDpt(m_tilinResolutionDistance.x/m_tilingDistanceInPixels.x,
               m_tilinResolutionDistance.y/m_tilingDistanceInPixels.y);

            theMapProjection->setDecimalDegreesPerPixel(result);
         }
      }
      else //only handle degree type for resolution
      {
         return false;
      }
   }

   if (theTilingDistanceUnitType != OSSIM_PIXEL)
   {
      ossimDpt paddingSize = getPaddingSize();//convert padding size to meters or degree
      getConvertedDistance(convertedTilingDistance, theTilingDistance);

      ossimDpt convertedOffset = m_offsetInPixels;
      if (m_useOffset)
      {
         getConvertedDistance(convertedOffset, m_offsetInPixels);
      }

      if (theMapProjection->isGeographic() && theTilingDistanceUnitType == OSSIM_DEGREES)
      {
         thePaddingSizeInPixels.x = ossim::round<int>(paddingSize.x / theMapProjection->getDecimalDegreesPerPixel().x);
         thePaddingSizeInPixels.y = ossim::round<int>(paddingSize.y / theMapProjection->getDecimalDegreesPerPixel().y);

         double intpart;
         m_fractionalPixelMisalignment.x = modf(convertedTilingDistance.x / theMapProjection->getDecimalDegreesPerPixel().x, &intpart);
         m_fractionalPixelMisalignment.y = modf(convertedTilingDistance.y / theMapProjection->getDecimalDegreesPerPixel().y, &intpart);

         m_tilingDistanceInPixels.x = ossim::round<int>(convertedTilingDistance.x / theMapProjection->getDecimalDegreesPerPixel().x);
         m_tilingDistanceInPixels.y = ossim::round<int>(convertedTilingDistance.y / theMapProjection->getDecimalDegreesPerPixel().y);

         if (m_useOffset)
         {
            m_offsetInPixels.x = ossim::round<int>(convertedOffset.x / theMapProjection->getDecimalDegreesPerPixel().x);
            m_offsetInPixels.y = ossim::round<int>(convertedOffset.y / theMapProjection->getDecimalDegreesPerPixel().y);
         }
      }
      else
      {
         thePaddingSizeInPixels.x = ossim::round<int>(paddingSize.x / gsd.x);
         thePaddingSizeInPixels.y = ossim::round<int>(paddingSize.y / gsd.y);

         double intpart;
         m_fractionalPixelMisalignment.x = modf(convertedTilingDistance.x / gsd.x, &intpart);
         m_fractionalPixelMisalignment.y = modf(convertedTilingDistance.y / gsd.y, &intpart);

         m_tilingDistanceInPixels.x = ossim::round<int>(convertedTilingDistance.x / gsd.x);
         m_tilingDistanceInPixels.y = ossim::round<int>(convertedTilingDistance.y / gsd.y);

         if (m_useOffset)
         {
            m_offsetInPixels.x = ossim::round<int>(convertedOffset.x / gsd.x);
            m_offsetInPixels.y = ossim::round<int>(convertedOffset.y / gsd.y);
         }
      }
   }
   else
   {
      if (theMapProjection->isGeographic())
      {
         theTilingDistance.x = theTilingDistance.x * theMapProjection->getDecimalDegreesPerPixel().x;
         theTilingDistance.y = theTilingDistance.y * theMapProjection->getDecimalDegreesPerPixel().y;
      }
      else
      {
         theTilingDistance.x = theTilingDistance.x * gsd.x;
         theTilingDistance.y = theTilingDistance.y * gsd.y;
      }
   }

   //re-calculation the image rect if resolution and resolution type defined
   if (!m_tilinResolutionDistance.hasNans())
   {
      if (theMapProjection->isGeographic())
      {
         imageWidth = (ossim_uint32)m_tilingDistanceInPixels.x * lonDiff;
         imageHeight = (ossim_uint32)m_tilingDistanceInPixels.y * latDiff;
      }

      //avoid very small pixel offset by calculation if tiling distance unit type is pixel
      if (theTilingDistanceUnitType == OSSIM_PIXEL)
      {
         if (imageWidth % (ossim_int32)m_tilingDistanceInPixels.x != 0)
         {
            ossim_int32 numX =  ossim::round<int>((double)imageWidth/m_tilingDistanceInPixels.x);
            imageWidth = m_tilingDistanceInPixels.x * numX;
         }
         if (imageHeight % (ossim_int32)m_tilingDistanceInPixels.y != 0)
         {
            ossim_int32 numY =  ossim::round<int>((double)imageHeight/m_tilingDistanceInPixels.y);
            imageHeight = m_tilingDistanceInPixels.y * numY;
         }
      }
    
      theImageRect = ossimIrect(boundingRect.ul(),
         ossimIpt((boundingRect.ul().x + imageWidth), boundingRect.ul().y),
         ossimIpt((boundingRect.ul().x + imageWidth), (imageHeight - boundingRect.ul().y)),
         ossimIpt(boundingRect.ul().x , (imageHeight - boundingRect.ul().y)));
   }
   return true;
}

bool ossimTilingRect::initialize(const ossimMapProjection& proj,
   const ossimIrect& boundingRect)
{
   bool result = false;
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimTilingRect::initialize DEBUG: Entered..."
         << "\nBounding rect === " << boundingRect
         << std::endl;
   }

   ossimDpt convertedTilingDistance = theTilingDistance;
   result = initializeBase(proj, boundingRect, convertedTilingDistance);
   ossimDrect tilingRectInPixels = theImageRect;

   // Let's setup the bounding tile rect.
   if(theMapProjection->isGeographic())
   {
      ossimGpt ul;
      ossimGpt ur;
      ossimGpt lr;
      ossimGpt ll;

      theMapProjection->lineSampleToWorld(theImageRect.ul(), ul);
      theMapProjection->lineSampleToWorld(theImageRect.ur(), ur);
      theMapProjection->lineSampleToWorld(theImageRect.lr(), lr);
      theMapProjection->lineSampleToWorld(theImageRect.ll(), ll);

      theTilingRect = ossimDrect(ossimDpt(ul), ossimDpt(ur), ossimDpt(lr), ossimDpt(ll),
         OSSIM_RIGHT_HANDED);

      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG: Geographic input bounds =  "
            << theTilingRect
            << std::endl;
      }

      if (!m_clipToAoi)
      {
         theTilingRect.stretchToTileBoundary(convertedTilingDistance);
      }

      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG: Geographic input bounds stretched =  "
            << theTilingRect << std::endl;
      }

      clampGeographic(theTilingRect);

      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG: Geographic input bounds clamped =  "
            << theTilingRect << std::endl;
      }

   }  // End "if(theMapProjection->isGeographic())"
   else // meters
   {
      ossimDpt ul;
      ossimDpt ur;
      ossimDpt lr;
      ossimDpt ll;

      theMapProjection->lineSampleToEastingNorthing(theImageRect.ul(), ul);
      theMapProjection->lineSampleToEastingNorthing(theImageRect.ur(), ur);
      theMapProjection->lineSampleToEastingNorthing(theImageRect.lr(), lr);
      theMapProjection->lineSampleToEastingNorthing(theImageRect.ll(), ll);
      theTilingRect = ossimDrect(ul, ur, lr, ll, OSSIM_RIGHT_HANDED);

      if (!m_clipToAoi)
      {
         theTilingRect.stretchToTileBoundary(convertedTilingDistance);
      }
   }

   theTotalHorizontalTiles = ossim::round<int>(fabs(tilingRectInPixels.ur().x - tilingRectInPixels.ul().x)/m_tilingDistanceInPixels.x);
   theTotalVerticalTiles   = ossim::round<int>(fabs(tilingRectInPixels.ur().y - tilingRectInPixels.lr().y)/m_tilingDistanceInPixels.y);

   if (m_clipToAoi)
   {
      if (m_useOffset)
      {
         ossim_float32 totalHorizontalTilesAfterOffset = 
            ((fabs(tilingRectInPixels.ur().x - tilingRectInPixels.ul().x))-m_tilingDistanceInPixels.x)/(m_tilingDistanceInPixels.x - m_offsetInPixels.x) + 1;
         ossim_float32 totalVerticalTilesAfterOffset = 
            ((fabs(tilingRectInPixels.ur().y - tilingRectInPixels.lr().y))-m_tilingDistanceInPixels.y)/(m_tilingDistanceInPixels.y - m_offsetInPixels.y) + 1;

         ossim_float32 lastHorizontal = totalHorizontalTilesAfterOffset - (ossim_int32)totalHorizontalTilesAfterOffset;
         ossim_float32 lastVertical = totalVerticalTilesAfterOffset - (ossim_int32)totalVerticalTilesAfterOffset;

         m_lastWidth = ossim::round<int>(fabs(lastHorizontal* (m_tilingDistanceInPixels.x - m_offsetInPixels.x) + m_offsetInPixels.x));
         m_lastHeight = ossim::round<int>(fabs(lastVertical* (m_tilingDistanceInPixels.y - m_offsetInPixels.y) + m_offsetInPixels.y));

         theTotalHorizontalTiles = ossim::round<int>(totalHorizontalTilesAfterOffset);
         theTotalVerticalTiles = ossim::round<int>(totalVerticalTilesAfterOffset);

         if (m_lastWidth > 0 && (lastHorizontal < 0.5))//if lastHorizontal is less than 0.5, the last tile will not add to total tiles after rounding, do here
         {
            theTotalHorizontalTiles = theTotalHorizontalTiles + 1;
         }
         if (m_lastHeight > 0 && (lastVertical < 0.5))
         {
            theTotalVerticalTiles = theTotalVerticalTiles + 1;
         }
      }
      else
      {
         ossim_int32 tmpX = (ossim_int32)(fabs(tilingRectInPixels.ur().x - tilingRectInPixels.ul().x)) + 1;
         ossim_int32 tmpY = (ossim_int32)(fabs(tilingRectInPixels.ur().y - tilingRectInPixels.lr().y)) + 1;

         ossim_int32 modValueX = tmpX % (ossim_int32)m_tilingDistanceInPixels.x;
         ossim_int32 modValueY = tmpY % (ossim_int32)m_tilingDistanceInPixels.y;
         if (modValueX >= 1)
         {
            m_lastWidth = modValueX;
         }
         else
         {
            m_lastWidth = m_tilingDistanceInPixels.x;
         }
         if (modValueY >= 1)
         {
            m_lastHeight = modValueY;
         }
         else
         {
            m_lastHeight = m_tilingDistanceInPixels.y;
         }

         if (m_lastWidth > 0)
         {  
            if (m_lastWidth < m_tilingDistanceInPixels.x/2)
            {
               theTotalHorizontalTiles = theTotalHorizontalTiles + 1;
            }
         }
         if (m_lastHeight > 0)
         {
            if (m_lastHeight < m_tilingDistanceInPixels.y/2)
            {
               theTotalVerticalTiles = theTotalVerticalTiles + 1;
            }
         }
      }
   }

   theTotalTiles = theTotalHorizontalTiles*theTotalVerticalTiles;

   // reset the tile id to the beginning
   reset();

   if(traceDebug())
   {
      print(ossimNotify(ossimNotifyLevel_DEBUG));

      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\nossimTiling::initialize DEBUG: Leaving..." << std::endl;
   }

   return result;
}

bool ossimTilingRect::next(ossimRefPtr<ossimMapProjection>& resultProjection,
   ossimIrect& resultingBounds,
   ossimString& resultingName)
{
   bool result = true;

   if(theTileId < theTotalTiles)
   {
      ossimDpt origin;
      ossim_int64 row=0;
      ossim_int64 col=0;

      getRowCol(row, col, theTileId);

      getTileName(resultingName,
         (row+1), (col+1), theTileId);

      getOrigin(origin, row, col);

      if (!m_fractionalPixelMisalignment.isNan())
      {
         if (row == 0 && col != 0)
         {
            if (m_fractionalPixelMisalignment.x >= 0.5)
            {
               if (theMapProjection->isGeographic())
               {
                  origin.x = origin.x + (1 - m_fractionalPixelMisalignment.x) * theMapProjection->getDecimalDegreesPerPixel().x * col;
               }
               else
               {
                  origin.x = origin.x + (1 - m_fractionalPixelMisalignment.x) * theMapProjection->getMetersPerPixel().x * col;
               }
            }  
            else
            {
               if (theMapProjection->isGeographic())
               {
                  origin.x = origin.x - m_fractionalPixelMisalignment.x * theMapProjection->getDecimalDegreesPerPixel().x * col;
               }
               else
               {
                  origin.x = origin.x - m_fractionalPixelMisalignment.x * theMapProjection->getMetersPerPixel().x * col;
               }
            }
         }
         else if (col == 0 && row != 0)
         {
            if (m_fractionalPixelMisalignment.y >= 0.5)
            {
               if (theMapProjection->isGeographic())
               {
                  origin.y = origin.y - (1 - m_fractionalPixelMisalignment.y) * theMapProjection->getDecimalDegreesPerPixel().y * row;
               }
               else
               {
                  origin.y = origin.y - (1 - m_fractionalPixelMisalignment.y) * theMapProjection->getMetersPerPixel().y * row;
               }
            }
            else
            {
               if (theMapProjection->isGeographic())
               {
                  origin.y = origin.y + m_fractionalPixelMisalignment.y * theMapProjection->getDecimalDegreesPerPixel().y * row;
               }
               else
               {
                  origin.y = origin.y + m_fractionalPixelMisalignment.y * theMapProjection->getMetersPerPixel().y * row;
               }  
            }
         }
         else if (col != 0 && row != 0)
         {
            if (m_fractionalPixelMisalignment.x >= 0.5)
            {
               if (theMapProjection->isGeographic())
               {
                  origin.x = origin.x + (1 - m_fractionalPixelMisalignment.x) * theMapProjection->getDecimalDegreesPerPixel().x * col;
               }
               else
               {
                  origin.x = origin.x + (1 - m_fractionalPixelMisalignment.x) * theMapProjection->getMetersPerPixel().x * col;
               }
            }  
            else
            {
               if (theMapProjection->isGeographic())
               {
                  origin.x = origin.x - m_fractionalPixelMisalignment.x * theMapProjection->getDecimalDegreesPerPixel().x * col;
               }
               else
               {
                  origin.x = origin.x - m_fractionalPixelMisalignment.x * theMapProjection->getMetersPerPixel().x * col;
               }
            }

            if (m_fractionalPixelMisalignment.y >= 0.5)
            {
               if (theMapProjection->isGeographic())
               {
                  origin.y = origin.y - (1 - m_fractionalPixelMisalignment.y) * theMapProjection->getDecimalDegreesPerPixel().y * row;
               }
               else
               {
                  origin.y = origin.y - (1 - m_fractionalPixelMisalignment.y) * theMapProjection->getMetersPerPixel().y * row;
               }
            }
            else
            {
               if (theMapProjection->isGeographic())
               {
                  origin.y = origin.y + m_fractionalPixelMisalignment.y * theMapProjection->getDecimalDegreesPerPixel().y * row;
               }
               else
               {
                  origin.y = origin.y + m_fractionalPixelMisalignment.y * theMapProjection->getMetersPerPixel().y * row;
               }  
            }
         }
      }

      ossimIpt pixels = ossimDpt(m_tilingDistanceInPixels.x, m_tilingDistanceInPixels.y);

      if(theMapProjection->isGeographic())
      {
         theMapProjection->setUlTiePoints(
            ossimGpt(origin.lat,
            origin.lon,
            0.0,
            theMapProjection->origin().datum()));
      }
      else
      {
         theMapProjection->setUlTiePoints(ossimDpt(origin.x, origin.y));
      }

      if (m_clipToAoi || m_useOffset)
      {
         if (row == 0 && col == 0)
         {
            resultingBounds = ossimIrect(0, 0,
               (pixels.x-1) + (ossim_int32)thePaddingSizeInPixels.x,
               (pixels.y-1) + (ossim_int32)thePaddingSizeInPixels.y);
         }
         else if (row == 0 && col != 0)
         {
            if (col == (theTotalHorizontalTiles-1))
            {
               resultingBounds = ossimIrect(-(ossim_int32)thePaddingSizeInPixels.x, 0,
                  (m_lastWidth-1),
                  (pixels.y-1) + (ossim_int32)thePaddingSizeInPixels.y);

               if (m_useOffset)
               {
                  ossim_int32 lr_x = -((ossim_int32)m_offsetInPixels.x*col)- resultingBounds.width();
                  resultingBounds = ossimIrect(-((ossim_int32)m_offsetInPixels.x*col), 0,
                     lr_x, (resultingBounds.height()-1));
               }
            }
            else
            {
               resultingBounds = ossimIrect(-(ossim_int32)thePaddingSizeInPixels.x, 0,
                  (pixels.x-1) + (ossim_int32)thePaddingSizeInPixels.x,
                  (pixels.y-1) + (ossim_int32)thePaddingSizeInPixels.y);

               if (m_useOffset)
               {
                  resultingBounds = ossimIrect(-((ossim_int32)m_offsetInPixels.x*col), 0,
                     (resultingBounds.width()-1)-((ossim_int32)m_offsetInPixels.x*col), 
                     (resultingBounds.height()-1));
               }
            }
         }
         else if (row != 0 && col == 0)
         {
            if (row == (theTotalVerticalTiles-1))
            {
               resultingBounds = ossimIrect(0, -(ossim_int32)thePaddingSizeInPixels.y,
                  (pixels.x-1) + (ossim_int32)thePaddingSizeInPixels.x,
                  (m_lastHeight-1));

               if (m_useOffset)
               {
                  ossim_int32 lr_y = -((ossim_int32)m_offsetInPixels.y*row)-resultingBounds.height();
                  resultingBounds = ossimIrect(0, -((ossim_int32)m_offsetInPixels.y*row),
                     (resultingBounds.width()-1), lr_y);
               }
            }
            else
            {
               resultingBounds = ossimIrect(0,
                  -(ossim_int32)thePaddingSizeInPixels.y,
                  (pixels.x-1) + (ossim_int32)thePaddingSizeInPixels.x,
                  (pixels.y-1) + (ossim_int32)thePaddingSizeInPixels.y);

               if (m_useOffset)
               {
                  resultingBounds = ossimIrect(0, -((ossim_int32)m_offsetInPixels.y*row),
                     (resultingBounds.width()-1), 
                     (resultingBounds.height()-1)-((ossim_int32)m_offsetInPixels.y*row));
               }
            }
         }
         else if (row != 0 && col != 0)
         {
            if (row == (theTotalVerticalTiles-1) && col == (theTotalHorizontalTiles-1))
            {
               resultingBounds = ossimIrect(-(ossim_int32)thePaddingSizeInPixels.x,
                  -(ossim_int32)thePaddingSizeInPixels.y,
                  (m_lastWidth-1),
                  (m_lastHeight-1));

               if (m_useOffset)
               {
                  ossim_int32 lr_x = -((ossim_int32)m_offsetInPixels.x*col)-resultingBounds.width();
                  ossim_int32 lr_y = -((ossim_int32)m_offsetInPixels.y*row)-resultingBounds.height();
                  resultingBounds = ossimIrect(-((ossim_int32)m_offsetInPixels.x*col), 
                     -((ossim_int32)m_offsetInPixels.y*row),
                     lr_x, lr_y);
               }
            }
            else if (row != (theTotalVerticalTiles-1) && col == (theTotalHorizontalTiles-1))
            {
               resultingBounds = ossimIrect(-(ossim_int32)thePaddingSizeInPixels.x,
                  -(ossim_int32)thePaddingSizeInPixels.y,
                  (m_lastWidth-1),
                  (pixels.y-1) + (ossim_int32)thePaddingSizeInPixels.y);

               if (m_useOffset)
               {
                  ossim_int32 lr_x = -((ossim_int32)m_offsetInPixels.x*col)-resultingBounds.width();
                  resultingBounds = ossimIrect(-((ossim_int32)m_offsetInPixels.x*col), 
                     -((ossim_int32)m_offsetInPixels.y*row),
                     lr_x, (resultingBounds.height()-1)-((ossim_int32)m_offsetInPixels.y*row));
               }
            }
            else if (row == (theTotalVerticalTiles-1) && col != (theTotalHorizontalTiles-1))
            {
               resultingBounds = ossimIrect(-(ossim_int32)thePaddingSizeInPixels.x,
                  -(ossim_int32)thePaddingSizeInPixels.y,
                  (pixels.x-1) + (ossim_int32)thePaddingSizeInPixels.x,
                  (m_lastHeight-1));

               if (m_useOffset)
               {
                  ossim_int32 lr_y = -((ossim_int32)m_offsetInPixels.y*row)-resultingBounds.height();
                  resultingBounds = ossimIrect(-((ossim_int32)m_offsetInPixels.x*col), 
                     -((ossim_int32)m_offsetInPixels.y*row),
                     (resultingBounds.width()-1)-((ossim_int32)m_offsetInPixels.x*col), 
                     lr_y);
               }
            }
            else
            {
               resultingBounds = ossimIrect(-(ossim_int32)thePaddingSizeInPixels.x,
                  -(ossim_int32)thePaddingSizeInPixels.y,
                  (pixels.x-1) + (ossim_int32)thePaddingSizeInPixels.x,
                  (pixels.y-1) + (ossim_int32)thePaddingSizeInPixels.y);

               if (m_useOffset)
               {
                  resultingBounds = ossimIrect(-((ossim_int32)m_offsetInPixels.x*col), 
                     -((ossim_int32)m_offsetInPixels.y*row),
                     (resultingBounds.width()-1)-((ossim_int32)m_offsetInPixels.x*col), 
                     (resultingBounds.height()-1)-((ossim_int32)m_offsetInPixels.y*row));
               }
            }
         }
      }
      else
      {
         resultingBounds = ossimIrect(-(ossim_int32)thePaddingSizeInPixels.x,
            -(ossim_int32)thePaddingSizeInPixels.y,
            (pixels.x-1) + (ossim_int32)thePaddingSizeInPixels.x,
            (pixels.y-1) + (ossim_int32)thePaddingSizeInPixels.y);

         if (m_useOffset)
         {
            resultingBounds = ossimIrect(-(ossim_int32)m_offsetInPixels.x, 
               -(ossim_int32)m_offsetInPixels.y,
               (resultingBounds.width()-1), (resultingBounds.height()-1));
         }
      }

      if(traceDebug()&&resultProjection.valid())
      {
         ossimKeywordlist projKwl;

         resultProjection->saveState(projKwl);
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimTilingRect::next DEBUG: rect = " << resultingBounds
            << "\nproj                          = " << projKwl
            << "\ntileName                      = " << resultingName
            << "\norigin                        = " << origin << std::endl;
      }

      resultProjection = theMapProjection;

      ++theTileId; 
   }
   else
   {
      result = false;
   }

   return result;
}

bool ossimTilingRect::saveState(ossimKeywordlist& kwl,
                            const char* prefix)const
{
   std::stringstream tilingDistance;

   tilingDistance << theTilingDistance.x << " " << theTilingDistance.y << std::endl;
   
   kwl.add(prefix,
           "tile_size",
           tilingDistance.str().c_str(),
           true);

   kwl.add(prefix,
           "units",
           ossimUnitTypeLut::instance()->
           getTableIndexString(theTilingDistanceUnitType),
           true);
   
   std::ostringstream padding;

   padding << thePaddingSizeInPixels.x << " " << thePaddingSizeInPixels.y;
   kwl.add(prefix,
           "padding",
           padding.str().c_str(),
           true);

   kwl.add(prefix,
      "output_file_name",
      theTileNameMask.c_str(),
      true);

   kwl.add(prefix,
      "clip_to_aoi",
      m_clipToAoi,
      true);

   return true;
}

bool ossimTilingRect::loadState(const ossimKeywordlist& kwl,
                            const char* prefix)
{
   bool result = false;
   
   // Get the mask if any.
   theTileNameMask = kwl.find(prefix, "output_file_name");
   if (!theTileNameMask.contains("%f%")) //%f% format is used for feature id in a shape file
   {
      setTileNameMask(theTileNameMask);
   }
  
   const char* paddingSizeInPixels = kwl.find(prefix, "padding");

   if (paddingSizeInPixels)
   {
      ossimString x,y;
      std::istringstream padding(ossimString(paddingSizeInPixels).c_str());
      padding >> x >> y;
      thePaddingSizeInPixels.x = x.toFloat64();
      thePaddingSizeInPixels.y = y.toFloat64();
   }
  
   const char* tilingDistance = kwl.find(prefix, "tile_size");

   const char* tilingDistanceType = kwl.find(prefix, "units");
   
   result = tilingDistance&&tilingDistanceType&&paddingSizeInPixels;
   
   ossimString x,y;
   std::istringstream distance(ossimString(tilingDistance).c_str());
   distance >> x >> y;
   theTilingDistance.x = x.toFloat64();
   theTilingDistance.y = y.toFloat64();
   if(theTilingDistance.x <= 0.0)
   {
      theTilingDistance.x = 1.0;
   }
   if(theTilingDistance.y<=0.0)
   {
      theTilingDistance.y = theTilingDistance.x;
   }

   theTilingDistanceUnitType = (ossimUnitType)
      ossimUnitTypeLut::instance()->getEntryNumber(
      ossimString(tilingDistanceType).c_str());

   const char* clipToAoi = kwl.find(prefix, "clip_to_aoi");
   if (clipToAoi)
   {
      if (!ossimString(clipToAoi).toBool())
      {
         m_clipToAoi = false;
      }
   }

   const char* offset = kwl.find(prefix, "use_offset");
   if (offset)
   {
      ossimString width, height;
      std::istringstream distanceOffset(ossimString(offset).c_str());
      distanceOffset >> width >> height;
      m_offsetInPixels.x = width.toFloat64();
      m_offsetInPixels.y = height.toFloat64();
      if (m_offsetInPixels.x > 0 || m_offsetInPixels.y > 0.0)
      {
         m_useOffset = true;
         result = true;
      }
   }

   const char* tileRes = kwl.find(prefix, "resolution");
   if (tileRes)
   {
      theDelta = ossimDpt(0,0);
      x="";
      y="";
      std::istringstream tileResValues(ossimString(tileRes).c_str());
      tileResValues >> x >> y;

      m_tilinResolutionDistance.x = x.toFloat64();
      m_tilinResolutionDistance.y = y.toFloat64();
   }
 
   const char* tilingResType = kwl.find(prefix, "resolution_units");
   if (tilingResType)
   {
      m_tilingResolutionUnitType = (ossimUnitType)
         ossimUnitTypeLut::instance()->getEntryNumber(
         ossimString(tilingResType).c_str());
   }

   if (traceDebug())
   {
      print(ossimNotify(ossimNotifyLevel_DEBUG));
   }
   return result;
}

ossimDpt ossimTilingRect::getPaddingSize()const
{
   ossimDpt result = thePaddingSizeInPixels;

   if (!theMapProjection.valid())
      return result;

   if(theMapProjection->isGeographic() && theTilingDistanceUnitType == OSSIM_DEGREES)
   {
      return result;
   }
   else
   {
      ossimUnitConversionTool unitConverter(1.0, theTilingDistanceUnitType);
      unitConverter.setValue(result.x, theTilingDistanceUnitType);
      result.x = unitConverter.getValue(OSSIM_METERS);

      unitConverter.setValue(result.y, theTilingDistanceUnitType);
      result.y = unitConverter.getValue(OSSIM_METERS);
   }

   return result;
}

void ossimTilingRect::getConvertedDistance(ossimDpt& pt, ossimDpt inputDistance) const
{
   if (!theMapProjection)
   {
      // This should not happen...
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimTiling::getConvertedDistance WARNING"
         << " projection not set!" << endl;
      pt = inputDistance;
      return;
   }

   if(theMapProjection->isGeographic() && theTilingDistanceUnitType == OSSIM_DEGREES)
   {
      pt = inputDistance;
   }
   else
   {
      ossimUnitConversionTool unitConverter(theMapProjection->origin(),
         1.0,
         theTilingDistanceUnitType);

      unitConverter.setValue(inputDistance.x,
         theTilingDistanceUnitType);
      pt.x = unitConverter.getValue(OSSIM_METERS);

      unitConverter.setValue(inputDistance.y,
         theTilingDistanceUnitType);
      pt.y = unitConverter.getValue(OSSIM_METERS);
   }
}
