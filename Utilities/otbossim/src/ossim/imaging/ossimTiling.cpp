//*******************************************************************
// Copyright (C) 2004 Garrett Potts
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: implementation for image generator
//
//*************************************************************************
// $Id: ossimTiling.cpp 20103 2011-09-17 16:10:42Z dburken $

#include <sstream>
#include <iomanip>
#include <ossim/imaging/ossimTiling.h>
#include <ossim/base/ossimUnitTypeLut.h>
#include <ossim/base/ossimUnitConversionTool.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimTrace.h>

static ossimTrace traceDebug("ossimTiling:debug");

RTTI_DEF1(ossimTiling, "ossimTiling", ossimObject);

ossimTiling::ossimTiling()
      :theTilingDistance(0,0),
       theTilingDistanceUnitType(OSSIM_UNIT_UNKNOWN),
       theDelta(0,0),
       theDeltaType(ossimTilingDeltaType_TOTAL_PIXELS),
       theOutputSizeInBytes(0),
       theNumberOfBands(1),
       theNumberOfBytesPerPixelPerBand(1),
       thePaddingSizeInPixels(0.0, 0.0),
       theMapProjection(),
       theImageRect(),
       theTilingRect(),
       theTileId(-1),
       theTotalHorizontalTiles(0),
       theTotalVerticalTiles(0),
       theTotalTiles(0),
       theTileNameMask(),
       theEdgeToEdgeFlag(false)
{
}

bool ossimTiling::initialize(const ossimMapProjection& proj,
                             const ossimIrect& boundingRect)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimTiling::initialize DEBUG: Entered..."
         << "\nBounding rect === " << boundingRect
         << std::endl;
   }
   
   bool result = true;
   theMapProjection = (ossimMapProjection*)proj.dup();
   theImageRect     = boundingRect;

   result = validate();

   ossimDpt convertedTilingDistance = theTilingDistance;

   if(result)
   {
      // Let's setup the bounding tile rect.
      if(theTilingDistanceUnitType != OSSIM_PIXEL)
      {
         getConvertedTilingDistance(convertedTilingDistance);
         
         if(theMapProjection->isGeographic())
         {
            ossimGpt ul;
            ossimGpt ur;
            ossimGpt lr;
            ossimGpt ll;

            theMapProjection->lineSampleToWorld(theImageRect.ul(),
                                                ul);
            theMapProjection->lineSampleToWorld(theImageRect.ur(),
                                                ur);
            theMapProjection->lineSampleToWorld(theImageRect.lr(),
                                                lr);
            theMapProjection->lineSampleToWorld(theImageRect.ll(),
                                                ll);

            theTilingRect = ossimDrect(ossimDpt(ul),
                                       ossimDpt(ur),
                                       ossimDpt(lr),
                                       ossimDpt(ll),
                                       OSSIM_RIGHT_HANDED);

            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "DEBUG: Geographic input bounds =  "
                  << theTilingRect
                  << std::endl;
            }
            
            theTilingRect.stretchToTileBoundary(convertedTilingDistance);
            
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
            
            theMapProjection->lineSampleToEastingNorthing(theImageRect.ul(),
                                                          ul);
            theMapProjection->lineSampleToEastingNorthing(theImageRect.ur(),
                                                          ur);
            theMapProjection->lineSampleToEastingNorthing(theImageRect.lr(),
                                                          lr);
            theMapProjection->lineSampleToEastingNorthing(theImageRect.ll(),
                                                          ll);
            theTilingRect = ossimDrect(ul,
                                       ur,
                                       lr,
                                       ll,
                                       OSSIM_RIGHT_HANDED);
            
            theTilingRect.stretchToTileBoundary(convertedTilingDistance);
         }
      }
      else
      {
         ossim_int32 remainderX = (ossim_int32)((theImageRect.width()%((ossim_int32)theDelta.x)) != 0);
         ossim_int32 remainderY = (ossim_int32)((theImageRect.height()%((ossim_int32)theDelta.y)) != 0);
         
         ossim_int32 w = (ossim_int32)((theImageRect.width()/(ossim_int32)theDelta.x)*theDelta.x +
                                       (remainderX*theDelta.x));
         ossim_int32 h = (ossim_int32)((theImageRect.height()/(ossim_int32)theDelta.y)*theDelta.y +
                                       (remainderY*theDelta.y));
         theTilingRect = ossimDrect(theImageRect.ul().x,
                                    theImageRect.ul().y,
                                    theImageRect.ul().x + (w-1),
                                    theImageRect.ul().y + (h-1));

         // since we are pixel space we force delta to be 1 pixel unit
         //
         theDelta.x   = 1.0;
         theDelta.y   = 1.0;
         theDeltaType = ossimTilingDeltaType_PER_PIXEL;
      }
   }
   
   theTotalHorizontalTiles = ossim::round<int>(fabs(theTilingRect.ur().x - theTilingRect.ul().x)/convertedTilingDistance.x);
   theTotalVerticalTiles   = ossim::round<int>(fabs(theTilingRect.ur().y - theTilingRect.lr().y)/convertedTilingDistance.y);
   theTotalTiles           = theTotalHorizontalTiles*theTotalVerticalTiles;

   // reset the tile id to the beginning
   //
   reset();
   
   if(traceDebug())
   {
      print(ossimNotify(ossimNotifyLevel_DEBUG));
      
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\nossimTiling::initialize DEBUG: Leaving..." << std::endl;
   }
   return result;
}

bool ossimTiling::next(ossimRefPtr<ossimMapProjection>& resultProjection,
                       ossimIrect& resultingBounds,
                       ossimString& resultingName)const
{
   bool result = true;
   
   if(theTileId < theTotalTiles)
   {
      ossimDpt origin;
      ossim_int64 row=0;
      ossim_int64 col=0;

      getRowCol(row, col, theTileId);

      getTileName(resultingName,
                  row, col, theTileId);
      
      getOrigin(origin, row, col);

      if(theTilingDistanceUnitType == OSSIM_PIXEL)
      {
         ossimIpt ul = origin;

         resultingBounds =
            ossimIrect(ul.x - (ossim_int32)thePaddingSizeInPixels.x,
                       ul.y - (ossim_int32)thePaddingSizeInPixels.y,
                       ul.x + ((ossim_int32)theTilingDistance.x-1) +
                       (ossim_int32)thePaddingSizeInPixels.x,
                       ul.y + ((ossim_int32)theTilingDistance.y-1) +
                       (ossim_int32)thePaddingSizeInPixels.y);
      }
      else
      {
         
         ossimDpt deltaPerPixel = getDeltaPerPixel();
         ossimIpt pixels;

         if(theDeltaType == ossimTilingDeltaType_TOTAL_PIXELS)
         {
            pixels =   theDelta;
         }
         else
         {
            pixels =   ossimDpt(theTilingDistance.x/theDelta.x,
                                theTilingDistance.y/theDelta.y);
         }

         if(theMapProjection->isGeographic())
         {
            if ( theEdgeToEdgeFlag )
            {
               theMapProjection->setUlTiePoints(
                  ossimGpt( origin.lat - (deltaPerPixel.y/2.0),
                            origin.lon + (deltaPerPixel.x/2.0),
                            0.0,
                            theMapProjection->origin().datum()) );
            }
            else
            {
               theMapProjection->setUlTiePoints(
                  ossimGpt(origin.lat,
                           origin.lon,
                           0.0,
                           theMapProjection->origin().datum()));
               
            }
            theMapProjection->setDecimalDegreesPerPixel(deltaPerPixel);
         }
         else
         {
            if ( theEdgeToEdgeFlag )
            {
               theMapProjection->setUlTiePoints(
                  ossimDpt( origin.x + (deltaPerPixel.x/2.0),
                            origin.y - (deltaPerPixel.y/2.0) ) );
            }
            else
            {
               theMapProjection->setUlTiePoints(ossimDpt(origin.x, origin.y));
            }
            theMapProjection->setMetersPerPixel(deltaPerPixel);
         }
         resultingBounds = ossimIrect(-(ossim_int32)thePaddingSizeInPixels.x,
                                      -(ossim_int32)thePaddingSizeInPixels.y,
                                      (pixels.x-1) + (ossim_int32)thePaddingSizeInPixels.x,
                                      (pixels.y-1) + (ossim_int32)thePaddingSizeInPixels.y);
      }
      resultProjection = theMapProjection;

      ++theTileId;
      if(traceDebug()&&resultProjection.valid())
      {
         ossimKeywordlist projKwl;
         
         resultProjection->saveState(projKwl);
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimTiling::next DEBUG: rect = " << resultingBounds
            << "\nproj                          = " << projKwl
            << "\ntileName                      = " << resultingName
            << "\norigin                        = " << origin << std::endl;
      }
   }
   else
   {
      result = false;
   }

   
   return result;
}


void ossimTiling::reset()
{
   theTileId = 0;
}

void ossimTiling::setTilingDistance(const ossimDpt& tilingDistance,
                                    ossimUnitType unitType)
{
   theTilingDistance         = tilingDistance;
   theTilingDistanceUnitType = unitType;
}

void ossimTiling::setDelta(const ossimDpt& delta,
                           ossimTilingDeltaType deltaType)
{
   theDelta     = delta;
   theDeltaType = deltaType;
}

bool ossimTiling::validate()const
{
   bool result = true;

   if(theMapProjection.valid())
   {
      if(theMapProjection->isGeographic())
      {
         switch(theTilingDistanceUnitType)
         {
            case OSSIM_DEGREES:
            case OSSIM_RADIANS:
            case OSSIM_SECONDS:
            case OSSIM_MINUTES:
            case OSSIM_PIXEL:
            {
               break;
            }
            default:
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "ossimTiling::validate()\n"
                  << "Map projeciton requires tiling in angular units but the"
                  << "\nspacing is in non angular" << std::endl;
               result = false;
            }
         }
      }
      else
      {
         switch(theTilingDistanceUnitType)
         {
            case OSSIM_METERS:
            case OSSIM_FEET:
            case OSSIM_US_SURVEY_FEET:
            case OSSIM_NAUTICAL_MILES:
            case OSSIM_PIXEL:
            case OSSIM_MILES:
            case OSSIM_MILLIMETERS:
            {
               break;
            }
            default:
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "ossimTiling::validate()"
                  << "\nMap projeciton requires tiling using linear spacing"
                  << " but the spacing is in non linear" << std::endl;
               result = false;
            }
         }
      }
   }
   else
   {
      result = false;
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimTiling::validate()"
         << "\nthe image projection is null and so not valid"
         << std::endl;
   }
   if((theDelta.x <= FLT_EPSILON) ||
      (theDelta.y <= FLT_EPSILON))
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimTiling::validate() the delta is too small or is negative"
         << "value = " << theDelta << std::endl;
      result = false;
   }
   if((theTilingDistance.x <= FLT_EPSILON)||
      (theTilingDistance.y <= FLT_EPSILON))
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimTiling::validate()"
         << "\nthe tiling distance is too small or is negative"
         << "Value = " << theTilingDistance << std::endl;
   }
   
   if(theImageRect.hasNans())
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimTiling::validate() the image rect has nans"
         << std::endl;
      result = false;
   }
   
   return result;
}

bool ossimTiling::isAngularUnit(ossimUnitType unitType)const
{
   bool result = false;
   
   switch(unitType)
   {
      case OSSIM_DEGREES:
      case OSSIM_RADIANS:
      case OSSIM_SECONDS:
      case OSSIM_MINUTES:
      {
         result = true;
         break;
      }
      default:
      {
         break;
      }
   }
   return result;
}

bool ossimTiling::isLinearUnit(ossimUnitType unitType)const
{
   bool result = false;
   
   switch(unitType)
   {
      case OSSIM_METERS:
      case OSSIM_FEET:
      case OSSIM_US_SURVEY_FEET:
      case OSSIM_NAUTICAL_MILES:
      case OSSIM_MILES:
      case OSSIM_MILLIMETERS:
      {
         result = true;
         break;
      }
      default:
      {
         break;
      }
   }
   return result;
   
}

void ossimTiling::setTileNameMask(const ossimString& mask)
{
   if(theTileNameMask == "")
   {
      theTileNameMask = "tile%i%";
   }
   if(mask.contains("%i%")||
      mask.contains("%oc%")||
      mask.contains("%or%")||
      mask.contains("%r%")||
      mask.contains("%c%")||
      mask.contains("%SRTM%"))
   {
      theTileNameMask = mask;
   }
   else
   {
      theTileNameMask += "%i%";
   }
}

ossimString ossimTiling::getTileNameMask()const
{
   return theTileNameMask;
}

bool ossimTiling::getRowCol(ossim_int64& row,
                            ossim_int64& col,
                            ossim_int64 tileId)const
{
   bool result = true;
   
   if((theTotalHorizontalTiles > 0)&&
      (tileId < theTotalTiles))
   {
      row = tileId/static_cast<ossim_int64>(theTotalHorizontalTiles);
      col = tileId%static_cast<ossim_int64>(theTotalHorizontalTiles);
      if((col >= static_cast<ossim_int32>(theTotalHorizontalTiles)) &&
         (row >= static_cast<ossim_int32>(theTotalVerticalTiles)))
      {           
         result = false;
      }
   }
   return result;
}

bool ossimTiling::getOrigin(ossimDpt& origin,
                            ossim_int64 tileId)const
{
   ossim_int64 row=0;
   ossim_int64 col=0;
   
   bool result = getRowCol(row, col, tileId);
   if(result)
   {
      result = getOrigin(origin, row, col);
   }

   return result;
}

bool ossimTiling::getOrigin(ossimDpt& origin,
                            ossim_int64 row,
                            ossim_int64 col)const
{
   bool result = false;

   if((col < static_cast<ossim_int64>(theTotalHorizontalTiles)) &&
      (row < static_cast<ossim_int64>(theTotalVerticalTiles)))
   {
      ossimDpt convertedTilingDistance;
      getConvertedTilingDistance(convertedTilingDistance);

      result = true;
      if(theTilingRect.orientMode() == OSSIM_RIGHT_HANDED)
      {
         origin.x = theTilingRect.ul().x + col*convertedTilingDistance.x;
         origin.y = theTilingRect.ul().y - row*convertedTilingDistance.y;
      }
      else
      {
         origin.x = theTilingRect.ul().x + col*convertedTilingDistance.x;
         origin.y = theTilingRect.ul().y + row*convertedTilingDistance.y;
      }
   }

   return result;
}

ossimDpt ossimTiling::getDeltaPerPixel()const
{
   
   ossimDpt result = theDelta;

   if (!theMapProjection)
   {
      return result;
   }

   if(theDeltaType == ossimTilingDeltaType_TOTAL_PIXELS)
   {
      result = ossimDpt(theTilingDistance.x/theDelta.x,
                        theTilingDistance.y/theDelta.y);
   }

   ossimUnitConversionTool unitConverter(theMapProjection->origin(),
                                         1.0,
                                         theTilingDistanceUnitType);
   if(theMapProjection->isGeographic())
   {
      unitConverter.setValue(result.x, theTilingDistanceUnitType);
      result.x = unitConverter.getValue(OSSIM_DEGREES);
      
      unitConverter.setValue(result.y,
                             theTilingDistanceUnitType);
      result.y = unitConverter.getValue(OSSIM_DEGREES);
   }
   else
   {
      unitConverter.setValue(result.x,
                             theTilingDistanceUnitType);
      result.x = unitConverter.getValue(OSSIM_METERS);
      
      unitConverter.setValue(result.y,
                             theTilingDistanceUnitType);
      result.y = unitConverter.getValue(OSSIM_METERS);
   }

   return result;
}

void ossimTiling::getTileName(ossimString& resultingName,
                              ossim_int64 row,
                              ossim_int64 col,
                              ossim_int64 id)const
{
   if(theTileNameMask == "")
   {
      ostringstream idString;
      idString << id;
      resultingName = ossimString("tile") + idString.str().c_str();
   }
   else if(theTileNameMask.contains("%SRTM%"))
   {
      // must be a 1x1 degree tiling distance
      //
      if((theTilingDistanceUnitType == OSSIM_DEGREES)&&
         (theTilingDistance.x == 1.0)&&
         (theTilingDistance.x == 1.0))
      {
         ostringstream latString;
         ostringstream lonString;
         
         resultingName = "";
         ossimDpt origin;
         getOrigin(origin, row, col);
         // the name is lower left and not upper left 
         // subtract one degree
         //
         origin.lat -=1;
         latString << setfill('0') << setw(2) << fabs(origin.lat);
         lonString << setfill('0') << setw(3) << fabs(origin.lon);
         
         if(origin.lat < 0.0)
         {
            resultingName += "S";
         }
         else 
         {
            resultingName += "N";
         }
         resultingName += latString.str().c_str();
         if(origin.lon < 0.0)
         {
            resultingName += "W";
         }
         else
         {
            resultingName += "E";
         }
         resultingName += lonString.str().c_str();
         resultingName += ".hgt";
      }
   }
   else
   {
      ostringstream rowString;
      ostringstream colString;
      ostringstream idString;
      ostringstream oRowString;
      ostringstream oColString;
      ossimDpt      origin;
      getOrigin(origin, row, col);
      
      rowString << row;
      colString << col;
      idString  << id;
      oRowString << row;
      oColString << col;
      
      resultingName = theTileNameMask;
      resultingName = resultingName.substitute("%r%",
                                               rowString.str().c_str());
      resultingName = resultingName.substitute("%c%",
                                               colString.str().c_str());
      resultingName = resultingName.substitute("%i%",
                                               idString.str().c_str());
      resultingName = resultingName.substitute("%oc",
                                               oColString.str().c_str());
      resultingName = resultingName.substitute("%or",
                                               oRowString.str().c_str());
   }
}

void ossimTiling::clampGeographic(ossimDrect& rect)const
{
   ossimDpt ul = rect.ul();
   ossimDpt lr = rect.lr();

   if(ul.lat > 90) ul.lat = 90.0;
   if(lr.lat < -90.0) lr.lat = -90.0;

   if(ul.lon < -180) ul.lon = -180;
   if(lr.lon > 180)  lr.lon = 180;

   rect = ossimDrect(ul, lr, rect.orientMode());
}

bool ossimTiling::saveState(ossimKeywordlist& kwl,
                            const char* prefix)const
{
   std::stringstream tilingDistance;

   tilingDistance << theTilingDistance.x << " " << theTilingDistance.y << std::endl;
   
   kwl.add(prefix,
           "tiling_distance",
           tilingDistance.str().c_str(),
           true);

   
   kwl.add(prefix,
           "tiling_distance_type",
           ossimUnitTypeLut::instance()->
           getTableIndexString(theTilingDistanceUnitType),
           true);
   std::ostringstream delta;
   delta << theDelta.x << " " << theDelta.y;
   kwl.add(prefix,
           "delta",
           delta.str().c_str(),
           true);
   
   if(theDeltaType == ossimTilingDeltaType_TOTAL_PIXELS)
   {
      kwl.add(prefix,
              "delta_type",
              "total_pixels",
              true);
   }
   else
   {
      kwl.add(prefix,
              "delta_type",
              "delta_per_pixels",
              true);
   }
   std::ostringstream padding;

   padding << thePaddingSizeInPixels.x << " " << thePaddingSizeInPixels.y;
   kwl.add(prefix,
           "padding_size_in_pixels",
           padding.str().c_str(),
           true);

   kwl.add(prefix,
           "tile_name_mask",
           theTileNameMask.c_str(),
           true);

   if (theOutputSizeInBytes)
   {
      kwl.add(prefix,
              "output_size_in_bytes",
              ossimString::toString(theOutputSizeInBytes).c_str(),
              true);
      kwl.add(prefix,
              "output_bands",
              theNumberOfBands,
              true);
      kwl.add(prefix,
              "output_bytes_per_pixel_per_band",
              theNumberOfBytesPerPixelPerBand,
              true);
   }

   kwl.add(prefix,
           "edge_to_edge",
           ossimString::toString(theEdgeToEdgeFlag),
           true);
   
   
   return true;
}

bool ossimTiling::loadState(const ossimKeywordlist& kwl,
                            const char* prefix)
{
   bool result = false;
   
   // Get the mask if any.
   theTileNameMask = kwl.find(prefix, "tile_name_mask");
   setTileNameMask(theTileNameMask);

   const char* paddingSizeInPixels = kwl.find(prefix,
                                              "padding_size_in_pixels");
   if (paddingSizeInPixels)
   {
      ossimString x,y;
      std::istringstream padding(ossimString(paddingSizeInPixels).c_str());
      padding >> x >> y;
      thePaddingSizeInPixels.x = x.toFloat64();
      thePaddingSizeInPixels.y = y.toFloat64();
   }
   
   // Check to tile by size in bytes.
   const char* lookup;
   if (kwl.numberOf(prefix, "output_size_in_"))
   {
      lookup = kwl.find(prefix, "output_size_in_bytes");
      if (lookup)
      {
         theOutputSizeInBytes = ossimString(lookup).toInt64();
      }
      lookup = kwl.find(prefix, "output_size_in_kilo_bytes");
      if (lookup)
      {
         theOutputSizeInBytes = ossimString(lookup).toInt64() * 1024;
      }
      lookup = kwl.find(prefix, "output_size_in_mega_bytes");
      if (lookup)
      {
         theOutputSizeInBytes = ossimString(lookup).toInt64() * 1024 * 1024;
      }
   }

   if (theOutputSizeInBytes)
   {
      
      lookup = kwl.find(prefix, "output_bands");
      if (lookup)
      {
         theNumberOfBands = ossimString(lookup).toInt32();
      }
      lookup = kwl.find(prefix, "output_bytes_per_pixel_per_band");
      if (lookup)
      {
         theNumberOfBytesPerPixelPerBand = ossimString(lookup).toInt32();
      }
      
      ossim_int64 pixelsPerBand = theOutputSizeInBytes /
         (theNumberOfBands * theNumberOfBytesPerPixelPerBand);
      
      ossim_int32 oneDimension =
         static_cast<ossim_int32>(floor(sqrt((double)pixelsPerBand)));

      //---
      // TODO: Clamp to power of two option. (drb)
      //---
      // theDelta.x = oneDimension;
      // theDelta.y = oneDimension;
      // theTilingDistance.x = 1;
      // theTilingDistance.y = 1;
      
      theDelta.x = 1;
      theDelta.y = 1;
      theTilingDistance.x = oneDimension;
      theTilingDistance.y = oneDimension;
      theDeltaType = ossimTilingDeltaType_TOTAL_PIXELS;
      theTilingDistanceUnitType = OSSIM_PIXEL;
      result = true;
   }
   else
   {
      const char* tilingDistance = kwl.find(prefix,
                                            "tiling_distance");
      const char* tilingDistanceType = kwl.find(prefix,
                                                "tiling_distance_type");
      const char* delta = kwl.find(prefix,
                                   "delta");
      
      const char* deltaType = kwl.find(prefix,
                                       "delta_type");
      
      result = tilingDistance&&tilingDistanceType&&delta&&deltaType&&paddingSizeInPixels;
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
      
      // unitLut.getTableIndexString(theTilingDistanceUnitType), ?????
      
      theTilingDistanceUnitType = (ossimUnitType)
         ossimUnitTypeLut::instance()->getEntryNumber(
            ossimString(tilingDistanceType).c_str());
      
      theDelta = ossimDpt(0,0);
      x="";
      y="";
      std::istringstream deltaValues(ossimString(delta).c_str());
      deltaValues >> x >> y;
      theDelta.x = x.toFloat64();
      theDelta.y = y.toFloat64();
      
      if(theDelta.x <= 0.0)
      {
         theDelta.x = 1.0;
      }
      
      if(theDelta.y <= 0.0)
      {
         theDelta.y = theDelta.x;
      }
      
      if(ossimString(deltaType).downcase().contains("total"))
      {
         theDeltaType = ossimTilingDeltaType_TOTAL_PIXELS;
      }
      else
      {
         theDeltaType = ossimTilingDeltaType_PER_PIXEL;
      }
   }
   
   lookup = kwl.find(prefix, "edge_to_edge");
   if ( lookup )
   {
      theEdgeToEdgeFlag = ossimString::toBool(lookup);
   }
      
   if (traceDebug())
   {
      print(ossimNotify(ossimNotifyLevel_DEBUG));
   }

   return result;
}

std::ostream& ossimTiling::print(std::ostream& out) const
{
   out << "theTilingDistance:                 " << theTilingDistance
       << "\ntheTilingDistanceUnitType:       " << theTilingDistanceUnitType
       << "\ntheDelta:                        " << theDelta
       << "\ntheDeltaType:                    " << theDeltaType
       << "\nthePaddingSizeInPixels:          " << thePaddingSizeInPixels
       << "\ntheImageRect:                    " << theImageRect
       << "\ntheTilingRect:                   " << theTilingRect
       << "\ntheTileId:                       " << theTileId
       << "\ntheTotalHorizontalTiles:         " << theTotalHorizontalTiles
       << "\ntheTotalVerticalTiles:           " << theTotalVerticalTiles
       << "\ntheTotalTiles:                   " << theTotalTiles
       << "\ntheTileNameMask:                 " << theTileNameMask
       << "\ntheOutputSizeInBytes:            " << theOutputSizeInBytes
       << "\ntheNumberOfBands:                " << theNumberOfBands
       << "\ntheNumberOfBytesPerPixelPerBand: " <<
      theNumberOfBytesPerPixelPerBand
       << "\ntheEdgeToEdgeFlag:               " << theEdgeToEdgeFlag
       << "\n";

   if (theMapProjection.valid())
   {
	   out << "\ntheMapProjection:\n";
       theMapProjection->print(out);
   }
   else
   {
      out << "theMapProjection is not set.";
   }
   out << endl;

   return out;
}

void ossimTiling::getConvertedTilingDistance(ossimDpt& pt) const
{
   if(theTilingDistanceUnitType == OSSIM_PIXEL)
   {
      pt = theTilingDistance;
      return;
   }
   if (!theMapProjection)
   {
      // This should not happen...
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimTiling::getConvertedTilingDistance WARNING"
         << " projection not set!" << endl;
      pt = theTilingDistance;
      return;
   }
   ossimUnitConversionTool unitConverter(theMapProjection->origin(),
                                         1.0,
                                         theTilingDistanceUnitType);

   if(theMapProjection->isGeographic())
   {
      unitConverter.setValue(theTilingDistance.x,
                             theTilingDistanceUnitType);
      pt.x = unitConverter.getValue(OSSIM_DEGREES);
      
      unitConverter.setValue(theTilingDistance.y,
                             theTilingDistanceUnitType);
      pt.y = unitConverter.getValue(OSSIM_DEGREES);
   }
   else
   {
      unitConverter.setValue(theTilingDistance.x,
                             theTilingDistanceUnitType);
      pt.x = unitConverter.getValue(OSSIM_METERS);
      
      unitConverter.setValue(theTilingDistance.y,
                             theTilingDistanceUnitType);
      pt.y = unitConverter.getValue(OSSIM_METERS);
   }
}
