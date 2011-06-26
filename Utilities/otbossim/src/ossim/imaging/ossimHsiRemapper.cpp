//*******************************************************************
// Copyright (C) 2002 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Remapper to adjust hue, saturation and intensity.
//
//*************************************************************************
// $Id: ossimHsiRemapper.cpp 19714 2011-06-03 17:23:45Z gpotts $

#include <cstdlib>
#include <ossim/imaging/ossimHsiRemapper.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNormRgbVector.h>
#include <ossim/base/ossimHsiVector.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimNumericProperty.h>

RTTI_DEF1(ossimHsiRemapper, "ossimHsiRemapper", ossimImageSourceFilter)

static ossimTrace traceDebug("ossimHsiRemapper:debug");

//***
// State keywords:
//***
static const char MASTER_HUE_OFFSET_KW[] = "hsi_master_hue_offset";
static const char MASTER_SATURATION_OFFSET_KW[] = "hsi_master_saturation_offset";
static const char MASTER_INTENSITY_OFFSET_KW[] = "hsi_master_intensity_offset";
static const char MASTER_INTENSITY_LOW_CLIP_KW[] = "hsi_master_intensity_low_clip";
static const char MASTER_INTENSITY_HIGH_CLIP_KW[] = "hsi_master_intensity_high_clip";

static const char RED_HUE_OFFSET_KW[] = "hsi_red_hue_offset";
static const char RED_HUE_LOW_RANGE_KW[] = "hsi_red_hue_low_range";
static const char RED_HUE_HIGH_RANGE_KW[] = "hsi_red_hue_high_range";
static const char RED_HUE_BLEND_RANGE_KW[] = "hsi_red_hue_blend_range";
static const char RED_SATURATION_OFFSET_KW[] = "hsi_red_saturation_offset";
static const char RED_INTENSITY_OFFSET_KW[] = "hsi_red_intensity_offset";

static const char YELLOW_HUE_OFFSET_KW[] = "hsi_yellow_hue_offset";
static const char YELLOW_HUE_LOW_RANGE_KW[] = "hsi_yellow_hue_low_range";
static const char YELLOW_HUE_HIGH_RANGE_KW[] = "hsi_yellow_hue_high_range";
static const char YELLOW_HUE_BLEND_RANGE_KW[] = "hsi_yellow_hue_blend_range";
static const char YELLOW_SATURATION_OFFSET_KW[] = "hsi_yellow_saturation_offset";
static const char YELLOW_INTENSITY_OFFSET_KW[] = "hsi_yellow_intensity_offset";

static const char GREEN_HUE_OFFSET_KW[] = "hsi_green_hue_offset";
static const char GREEN_HUE_LOW_RANGE_KW[] = "hsi_green_hue_low_range";
static const char GREEN_HUE_HIGH_RANGE_KW[] = "hsi_green_hue_high_range";
static const char GREEN_HUE_BLEND_RANGE_KW[] = "hsi_green_hue_blend_range";
static const char GREEN_SATURATION_OFFSET_KW[] = "hsi_green_saturation_offset";
static const char GREEN_INTENSITY_OFFSET_KW[] = "hsi_green_intensity_offset";

static const char CYAN_HUE_OFFSET_KW[] = "hsi_cyan_hue_offset";
static const char CYAN_HUE_LOW_RANGE_KW[] = "hsi_cyan_hue_low_range";
static const char CYAN_HUE_HIGH_RANGE_KW[] = "hsi_cyan_hue_high_range";
static const char CYAN_HUE_BLEND_RANGE_KW[] = "hsi_cyan_hue_blend_range";
static const char CYAN_SATURATION_OFFSET_KW[] = "hsi_cyan_saturation_offset";
static const char CYAN_INTENSITY_OFFSET_KW[] = "hsi_cyan_intensity_offset";

static const char BLUE_HUE_OFFSET_KW[] = "hsi_blue_hue_offset";
static const char BLUE_HUE_LOW_RANGE_KW[] = "hsi_blue_hue_low_range";
static const char BLUE_HUE_HIGH_RANGE_KW[] = "hsi_blue_hue_high_range";
static const char BLUE_HUE_BLEND_RANGE_KW[] = "hsi_blue_hue_blend_range";
static const char BLUE_SATURATION_OFFSET_KW[] = "hsi_blue_saturation_offset";
static const char BLUE_INTENSITY_OFFSET_KW[] = "hsi_blue_intensity_offset";

static const char MAGENTA_HUE_OFFSET_KW[] = "hsi_magenta_hue_offset";
static const char MAGENTA_HUE_LOW_RANGE_KW[] = "hsi_magenta_hue_low_range";
static const char MAGENTA_HUE_HIGH_RANGE_KW[] = "hsi_magenta_hue_high_range";
static const char MAGENTA_HUE_BLEND_RANGE_KW[] = "hsi_magenta_hue_blend_range";
static const char MAGENTA_SATURATION_OFFSET_KW[] = "hsi_magenta_saturation_offset";
static const char MAGENTA_INTENSITY_OFFSET_KW[] = "hsi_magenta_intensity_offset";

static const char WHITE_OBJECT_CLIP_KW[] = "hsi_white_object_clip";

static const double DEFAULT_BLEND = 15.0;
static const double MAX_BLEND     = 30.0;

ossimHsiRemapper::ossimHsiRemapper()
   :
      ossimImageSourceFilter      (),  // base class
      
      theTile                     (NULL),
      theBuffer                   (NULL),
      theNormalizedMinPix         (0.0),

      theMasterHueOffset          (0.0),
      theMasterSaturationOffset   (0.0),
      theMasterIntensityOffset    (0.0),
      theMasterIntensityLowClip   (0.0),
      theMasterIntensityHighClip  (1.0),
      
      theRedHueOffset             (0.0),
      theRedHueLowRange           (-30.0),
      theRedHueHighRange          (30.0),
      theRedHueBlendRange         (DEFAULT_BLEND),
      theRedSaturationOffset      (0.0),
      theRedIntensityOffset       (0.0),
      
      theYellowHueOffset          (0.0),
      theYellowHueLowRange        (30.0),
      theYellowHueHighRange       (90.0),
      theYellowHueBlendRange      (DEFAULT_BLEND),
      theYellowSaturationOffset   (0.0),
      theYellowIntensityOffset    (0.0),
      
      theGreenHueOffset           (0.0),
      theGreenHueLowRange         (90.0),
      theGreenHueHighRange        (150.0),
      theGreenHueBlendRange       (DEFAULT_BLEND),
      theGreenSaturationOffset    (0.0),
      theGreenIntensityOffset     (0.0),
      
      theCyanHueOffset            (0.0),
      theCyanHueLowRange          (150.0),
      theCyanHueHighRange         (210.0),
      theCyanHueBlendRange        (DEFAULT_BLEND),
      theCyanSaturationOffset     (0.0),
      theCyanIntensityOffset      (0.0),
      
      theBlueHueOffset            (0.0),
      theBlueHueLowRange          (210.0),
      theBlueHueHighRange         (270.0),
      theBlueHueBlendRange        (DEFAULT_BLEND),
      theBlueSaturationOffset     (0.0),
      theBlueIntensityOffset      (0.0),
      
      theMagentaHueOffset         (0.0),
      theMagentaHueLowRange       (270.0),
      theMagentaHueHighRange      (330.0),
      theMagentaHueBlendRange     (DEFAULT_BLEND),
      theMagentaSaturationOffset  (0.0),
      theMagentaIntensityOffset   (0.0),

      theWhiteObjectClip          (1.0)
{
   //***
   // Set the base class "theEnableFlag" to off since no adjustments have been
   // made yet.
   //***
   //disableSource();
   theValidFlag = false;
   // Construction not complete.
}

ossimHsiRemapper::~ossimHsiRemapper()
{
   theTile = NULL;
   if (theBuffer)
   {
      delete [] theBuffer;
      theBuffer = NULL;
   }
}

ossimRefPtr<ossimImageData> ossimHsiRemapper::getTile(
   const ossimIrect& tile_rect,
   ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      return ossimRefPtr<ossimImageData>();
   }

   // Fetch tile from pointer from the input source.
   ossimRefPtr<ossimImageData> inputTile = theInputConnection->getTile(
      tile_rect, resLevel);

   // Check for remap bypass or a null tile return from input:
   if (!isSourceEnabled() || !inputTile||!theValidFlag)
   {
      return inputTile;
   }

   // Check for first time through or size change.
   if ( !theTile.valid() ||  
        tile_rect.height() != theTile->getHeight() ||
        tile_rect.width()  != theTile->getWidth() )
   {
      allocate(tile_rect);
   }

   //---
   // Set the image rectangle of the tile.
   // Note that this will resize the tiles buffers if 
   theTile->setImageRectangle(tile_rect);

   // Get its status.
   ossimDataObjectStatus tile_status = inputTile->getDataObjectStatus();
   if ( (tile_status == OSSIM_NULL) || (tile_status == OSSIM_EMPTY) )
   {
      theTile->makeBlank();
      return theTile;
   }

   double* rgbBuf[3];
   const ossim_uint32 PPT = theTile->getSizePerBand();  // Pixels Per Tile

   rgbBuf[0] = theBuffer;
   rgbBuf[1] = &(theBuffer[PPT]);
   rgbBuf[2] = &(rgbBuf[1][PPT]);
   
   switch(inputTile->getNumberOfBands())
   {
      case 1:
      case 2:
      {
         // Copy the first band only.
         inputTile->copyTileBandToNormalizedBuffer(0, rgbBuf[0]);
         memcpy(rgbBuf[1], rgbBuf[0], PPT*sizeof(double));
         memcpy(rgbBuf[2], rgbBuf[0], PPT*sizeof(double));
	 break;
      }
      case 3:
      {
         inputTile->copyTileToNormalizedBuffer(theBuffer);
         break;
      }
      default:
      {
         inputTile->copyTileBandToNormalizedBuffer(0, rgbBuf[0]);
         inputTile->copyTileBandToNormalizedBuffer(1, rgbBuf[1]);
         inputTile->copyTileBandToNormalizedBuffer(2, rgbBuf[2]);
	 break;
      }
   }   

   ossimNormRgbVector rgb;
   ossimHsiVector     hsi;
   double r = 0.0;
   double g = 0.0;
   double b = 0.0;
   double h = 0.0;
   double s = 0.0;
   double i = 0.0;
   ossim_uint32 idx;
   // Convert the rgb value to hsi and adjust values.
   for (idx=0; idx<PPT; ++idx)
   {
      r = rgbBuf[0][idx];
      g = rgbBuf[1][idx];
      b = rgbBuf[2][idx];
      
      rgb = ossimNormRgbVector(r,g,b);
      hsi = rgb;

      h   = hsi.getH();
      s   = hsi.getS();
      i   = hsi.getI();
      
      if(i > FLT_EPSILON)
      {
         double h_offset = theMasterHueOffset;
         double s_offset = theMasterSaturationOffset;
         double i_offset = theMasterIntensityOffset;

         //***
         // Note:  For the purpose of checking to see if in the red range,
         //        make the hue negative if it's >= 315.0 and < 360.0.
         //        The red low and high ranges are stored in the same manner.
         //***
         double red_hue  = h;
         if (red_hue  >= 315.0 && red_hue  < 360.0) red_hue  = red_hue - 360.0;
                          
         if ( red_hue >= theRedHueLowRange && red_hue <= theRedHueHighRange)
         {
            // Adjust the reds.
            
            double bf = 1.0; // blend factor
            if ( red_hue < (theRedHueLowRange + theRedHueBlendRange) )
            {
               bf = (red_hue - theRedHueLowRange) / theRedHueBlendRange;
            }
            else if ( red_hue > (theRedHueHighRange - theRedHueBlendRange) )
            {
               bf = (theRedHueHighRange - red_hue) / theRedHueBlendRange;
            }
            
            h_offset += (theRedHueOffset        * bf);
            s_offset += (theRedSaturationOffset * bf);
            i_offset += (theRedIntensityOffset  * bf);
         }
            
         if (h >= theYellowHueLowRange && h <= theYellowHueHighRange) 
         {
            // Adjust the yellows.
            
            double bf = 1.0; // blend factor
            if ( h < (theYellowHueLowRange + theYellowHueBlendRange) )
            {
               bf = (h - theYellowHueLowRange) / theYellowHueBlendRange;
            }
            else if ( h > (theYellowHueHighRange - theYellowHueBlendRange) )
            {
               bf = (theYellowHueHighRange - h) / theYellowHueBlendRange;
            }
            
            h_offset += (theYellowHueOffset        * bf);
            s_offset += (theYellowSaturationOffset * bf);
            i_offset += (theYellowIntensityOffset  * bf);
         }
         
         if (h >= theGreenHueLowRange && h <= theGreenHueHighRange) 
         {
            // Adjust the greens. 

            double bf = 1.0; // blend factor
            if ( h < (theGreenHueLowRange + theGreenHueBlendRange) )
            {
               bf = (h - theGreenHueLowRange) / theGreenHueBlendRange;
            }
            else if ( h > (theGreenHueHighRange - theGreenHueBlendRange) )
            {
               bf = (theGreenHueHighRange - h) / theGreenHueBlendRange;
            }
            
            h_offset += (theGreenHueOffset        * bf);
            s_offset += (theGreenSaturationOffset * bf);
            i_offset += (theGreenIntensityOffset  * bf);
         }

         if (h >= theCyanHueLowRange && h <= theCyanHueHighRange) 
         {
            // Adjust the cyans.
            
            double bf = 1.0; // blend factor
            if ( h < (theCyanHueLowRange + theCyanHueBlendRange) )
            {
               bf = (h - theCyanHueLowRange) / theCyanHueBlendRange;
            }
            else if ( h > (theCyanHueHighRange - theCyanHueBlendRange) )
            {
               bf = (theCyanHueHighRange - h) / theCyanHueBlendRange;
            }
            
            h_offset += (theCyanHueOffset        * bf);
            s_offset += (theCyanSaturationOffset * bf);
            i_offset += (theCyanIntensityOffset  * bf);
         }

         if (h >= theBlueHueLowRange && h <= theBlueHueHighRange) 
         {
            // Adjust the blues.
            
            double bf = 1.0; // blend factor
            if ( h < (theBlueHueLowRange + theBlueHueBlendRange) )
            {
               bf = (h - theBlueHueLowRange) / theBlueHueBlendRange;
            }
            else if ( h > (theBlueHueHighRange - theBlueHueBlendRange) )
            {
               bf = (theBlueHueHighRange - h) / theBlueHueBlendRange;
            }
            
            h_offset += (theBlueHueOffset        * bf);
            s_offset += (theBlueSaturationOffset * bf);
            i_offset += (theBlueIntensityOffset  * bf);
         }

         if (h >= theMagentaHueLowRange && h <= theMagentaHueHighRange) 
         {
            // Adjust the magentas.
            
            double bf = 1.0; // blend factor
            if ( h < (theMagentaHueLowRange + theMagentaHueBlendRange) )
            {
               bf = (h - theMagentaHueLowRange) / theMagentaHueBlendRange;
            }
            else if ( h > (theMagentaHueHighRange - theMagentaHueBlendRange) )
            {
               bf = (theMagentaHueHighRange - h) / theMagentaHueBlendRange;
            }
            
            h_offset += (theMagentaHueOffset        * bf);
            s_offset += (theMagentaSaturationOffset * bf);
            i_offset += (theMagentaIntensityOffset  * bf);
         }
         
         // Apply the hue offset.
         h += h_offset;

         // Make sure the hue is between 0 and 360...
         if (h < 0.0) h += 360.0;
         else if (h >= 360) h -= 360.0;

         // Apply the saturation offset clamping/clipping to 0.0/1.0.
         s += s_offset;
         s = (s > 0.0 ? (s < 1.0 ? s : 1.0) : 0.0);
         
         // Apply the intensity offset clamping/clipping to 0.0/1.0.
         i += i_offset;
         i = (i > 0.0 ? (i < 1.0 ? i : 1.0) : 0.0);

         // Stretch the intensity channel.
//          i = (i - theMasterIntensityLowClip) *
//              ( 1.0 / (theMasterIntensityHighClip -
//                       theMasterIntensityLowClip) );
         i = (i - theMasterIntensityLowClip)/(theMasterIntensityHighClip -
                                              theMasterIntensityLowClip);
         
         hsi.setH(h);
         hsi.setS(s);
         hsi.setI(i);

         rgb = hsi;
         
         r = rgb.getR();
         g = rgb.getG();
         b = rgb.getB();
         if ( (theWhiteObjectClip < 1.0) &&
              (r > theWhiteObjectClip)   &&
              (g > theWhiteObjectClip)   &&
              (b > theWhiteObjectClip) )
         {
            r = theWhiteObjectClip;
            g = theWhiteObjectClip;
            b = theWhiteObjectClip;
         }
         // Do min/max range check and assign back to buffer.
         rgbBuf[0][idx] = r > theNormalizedMinPix ? (r < 1.0 ? r : 1.0) :
            theNormalizedMinPix;
         rgbBuf[1][idx] = g > theNormalizedMinPix ? (g < 1.0 ? g : 1.0) :
            theNormalizedMinPix;
         rgbBuf[2][idx] = b > theNormalizedMinPix ? (b < 1.0 ? b : 1.0) :
            theNormalizedMinPix;
      } // End of "if(i > FLT_EPSILON)"
      else
      {
         rgbBuf[0][idx] = 0.0;
         rgbBuf[1][idx] = 0.0;
         rgbBuf[2][idx] = 0.0;
      }
         
   }  // End of loop through pixels in a tile.
   // Copy the buffer to the output tile.
   theTile->copyNormalizedBufferToTile(theBuffer);
   
   // Update the tile status.
   theTile->validate();

   return theTile;
}

void ossimHsiRemapper::initialize()
{
   ossimImageSourceFilter::initialize();
   if (theTile.valid())
   {
      theTile = NULL;
      if(theBuffer)
      {
         delete []theBuffer;
         theBuffer = NULL;
      }
   }
}

void ossimHsiRemapper::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property.valid()) return;
   ossimString name = property->getName();
   if(name == MASTER_HUE_OFFSET_KW)
   {
      setMasterHueOffset(property->valueToString().toDouble());
   }
   else if(name == MASTER_SATURATION_OFFSET_KW)
   {
      setMasterSaturationOffset(property->valueToString().toDouble());
   }
   else if(name == MASTER_INTENSITY_OFFSET_KW)
   {
      setMasterIntensityOffset(property->valueToString().toDouble());
   }
   else if(name == MASTER_INTENSITY_LOW_CLIP_KW)
   {
      setMasterIntensityLowClip(property->valueToString().toDouble());
   }
   else if(name == MASTER_INTENSITY_HIGH_CLIP_KW)
   {
      setMasterIntensityHighClip(property->valueToString().toDouble());
   }
   else if(name == RED_HUE_OFFSET_KW)
   {
      setRedHueOffset(property->valueToString().toDouble());
   }
   else if(name == RED_HUE_LOW_RANGE_KW)
   {
      setRedHueLowRange(property->valueToString().toDouble());
   }
   else if(name == RED_HUE_HIGH_RANGE_KW)
   {
      setRedHueHighRange(property->valueToString().toDouble());
   }
   else if(name == RED_HUE_BLEND_RANGE_KW)
   {
      setRedHueBlendRange(property->valueToString().toDouble());
   }
   else if(name == RED_SATURATION_OFFSET_KW)
   {
      setRedSaturationOffset(property->valueToString().toDouble());
   }
   else if(name == RED_INTENSITY_OFFSET_KW)
   {
      setRedIntensityOffset(property->valueToString().toDouble());
  }
   else if(name == YELLOW_HUE_OFFSET_KW)
   {
      setYellowHueOffset(property->valueToString().toDouble());
   }
   else if(name == YELLOW_HUE_LOW_RANGE_KW)
   {
      setYellowHueLowRange(property->valueToString().toDouble());
   }
   else if(name == YELLOW_HUE_HIGH_RANGE_KW)
   {
      setYellowHueHighRange(property->valueToString().toDouble());
   }
   else if(name == YELLOW_HUE_BLEND_RANGE_KW)
   {
      setYellowHueBlendRange(property->valueToString().toDouble());
   }
   else if(name == YELLOW_SATURATION_OFFSET_KW)
   {
      setYellowSaturationOffset(property->valueToString().toDouble());
   }
   else if(name == YELLOW_INTENSITY_OFFSET_KW)
   {
      setYellowIntensityOffset(property->valueToString().toDouble());
   }
   else if(name == GREEN_HUE_OFFSET_KW)
   {
      setGreenHueOffset(property->valueToString().toDouble());
   }
   else if(name == GREEN_HUE_LOW_RANGE_KW)
   {
      setGreenHueLowRange(property->valueToString().toDouble());
   }
   else if(name == GREEN_HUE_HIGH_RANGE_KW)
   {
      setGreenHueHighRange(property->valueToString().toDouble());
   }
   else if(name == GREEN_HUE_BLEND_RANGE_KW)
   {
      setGreenHueBlendRange(property->valueToString().toDouble());
   }
   else if(name == GREEN_SATURATION_OFFSET_KW)
   {
      setGreenSaturationOffset(property->valueToString().toDouble());
   }
   else if(name == GREEN_INTENSITY_OFFSET_KW)
   {
      setGreenIntensityOffset(property->valueToString().toDouble());
   }
   else if(name == CYAN_HUE_OFFSET_KW)
   {
      setCyanHueOffset(property->valueToString().toDouble());
   }
   else if(name == CYAN_HUE_LOW_RANGE_KW)
   {
      setCyanHueLowRange(property->valueToString().toDouble());
   }
   else if(name == CYAN_HUE_HIGH_RANGE_KW)
   {
      setCyanHueHighRange(property->valueToString().toDouble());
   }
   else if(name == CYAN_HUE_BLEND_RANGE_KW)
   {
      setCyanHueBlendRange(property->valueToString().toDouble());
   }
   else if(name == CYAN_SATURATION_OFFSET_KW)
   {
      setCyanSaturationOffset(property->valueToString().toDouble());
   }
   else if(name == CYAN_INTENSITY_OFFSET_KW)
   {
      setCyanIntensityOffset(property->valueToString().toDouble());
   }
   else if(name == BLUE_HUE_OFFSET_KW)
   {
      setBlueHueOffset(property->valueToString().toDouble());
   }
   else if(name == BLUE_HUE_LOW_RANGE_KW)
   {
      setBlueHueLowRange(property->valueToString().toDouble());
   }
   else if(name == BLUE_HUE_HIGH_RANGE_KW)
   {
      setBlueHueHighRange(property->valueToString().toDouble());
   }
   else if(name == BLUE_HUE_BLEND_RANGE_KW)
   {
      setBlueHueBlendRange(property->valueToString().toDouble());
   }
   else if(name == BLUE_SATURATION_OFFSET_KW)
   {
      setBlueSaturationOffset(property->valueToString().toDouble());
   }
   else if(name == BLUE_INTENSITY_OFFSET_KW)
   {
      setBlueIntensityOffset(property->valueToString().toDouble());
   }
   else if(name == MAGENTA_HUE_OFFSET_KW)
   {
      setMagentaHueOffset(property->valueToString().toDouble());
   }
   else if(name == MAGENTA_HUE_LOW_RANGE_KW)
   {
      setMagentaHueLowRange(property->valueToString().toDouble());
   }
   else if(name == MAGENTA_HUE_HIGH_RANGE_KW)
   {
      setMagentaHueHighRange(property->valueToString().toDouble());
   }
   else if(name == MAGENTA_HUE_BLEND_RANGE_KW)
   {
      setMagentaHueBlendRange(property->valueToString().toDouble());
   }
   else if(name == MAGENTA_SATURATION_OFFSET_KW)
   {
      setMagentaSaturationOffset(property->valueToString().toDouble());
   }
   else if(name == MAGENTA_INTENSITY_OFFSET_KW)
   {
      setMagentaIntensityOffset(property->valueToString().toDouble());
   }
   else if(name == WHITE_OBJECT_CLIP_KW)
   {
      setWhiteObjectClip(property->valueToString().toDouble());
   }
   else
   {
      ossimImageSourceFilter::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimHsiRemapper::getProperty(const ossimString& name)const
{
   ossimRefPtr<ossimProperty> result;
   if(name == MASTER_HUE_OFFSET_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theMasterHueOffset), -180, 180);
      result->setCacheRefreshBit();
   }
   else if(name == MASTER_SATURATION_OFFSET_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theMasterSaturationOffset), -1, 1);
      result->setCacheRefreshBit();
  }
   else if(name == MASTER_INTENSITY_OFFSET_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theMasterIntensityOffset), -1, 1);
      result->setCacheRefreshBit();
   }
   else if(name == MASTER_INTENSITY_LOW_CLIP_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theMasterIntensityLowClip), 0, 1);
      result->setCacheRefreshBit();
   }
   else if(name == MASTER_INTENSITY_HIGH_CLIP_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theMasterIntensityHighClip), 0, 1);
      result->setCacheRefreshBit();
   }
   else if(name == RED_HUE_OFFSET_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theRedHueOffset), -180, 180);
      result->setCacheRefreshBit();
   }
   else if(name == RED_HUE_LOW_RANGE_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theRedHueLowRange), -30, 30);
      result->setCacheRefreshBit();
   }
   else if(name == RED_HUE_HIGH_RANGE_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theRedHueHighRange), -30, 30);
      result->setCacheRefreshBit();
   }
   else if(name == RED_HUE_BLEND_RANGE_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theRedHueBlendRange), 0, 30);
      result->setCacheRefreshBit();
   }
   else if(name == RED_SATURATION_OFFSET_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theRedSaturationOffset), -1, 1);
      result->setCacheRefreshBit();
   }
   else if(name == RED_INTENSITY_OFFSET_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theRedIntensityOffset), -1, 1);
      result->setCacheRefreshBit();
   }
   else if(name == YELLOW_HUE_OFFSET_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theYellowHueOffset), -180, 180);
      result->setCacheRefreshBit();
   }
   else if(name == YELLOW_HUE_LOW_RANGE_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theYellowHueLowRange), 30, 90);
      result->setCacheRefreshBit();
   }
   else if(name == YELLOW_HUE_HIGH_RANGE_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theYellowHueHighRange), 30, 90);
      result->setCacheRefreshBit();
   }
   else if(name == YELLOW_HUE_BLEND_RANGE_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theYellowHueBlendRange), 0, 30);
      result->setCacheRefreshBit();
   }
   else if(name == YELLOW_SATURATION_OFFSET_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theYellowSaturationOffset), -1, 1);
      result->setCacheRefreshBit();
   }
   else if(name == YELLOW_INTENSITY_OFFSET_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theYellowIntensityOffset), -1, 1);
      result->setCacheRefreshBit();
   }
   else if(name == GREEN_HUE_OFFSET_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theGreenHueOffset), -180, 180);
      result->setCacheRefreshBit();
   }
   else if(name == GREEN_HUE_LOW_RANGE_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theGreenHueLowRange), 90, 150);
      result->setCacheRefreshBit();
   }
   else if(name == GREEN_HUE_HIGH_RANGE_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theGreenHueHighRange), 90, 150);
      result->setCacheRefreshBit();
   }
   else if(name == GREEN_HUE_BLEND_RANGE_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theGreenHueBlendRange), 0, 30);
      result->setCacheRefreshBit();
   }
   else if(name == GREEN_SATURATION_OFFSET_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theGreenSaturationOffset), -1, 1);
      result->setCacheRefreshBit();
   }
   else if(name == GREEN_INTENSITY_OFFSET_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theGreenIntensityOffset), -1, 1);
      result->setCacheRefreshBit();
   }
   else if(name == CYAN_HUE_OFFSET_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theCyanHueOffset), -180, 180);
      result->setCacheRefreshBit();
   }
   else if(name == CYAN_HUE_LOW_RANGE_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theCyanHueLowRange), 150, 210);
      result->setCacheRefreshBit();
   }
   else if(name == CYAN_HUE_HIGH_RANGE_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theCyanHueHighRange), 150, 210);
      result->setCacheRefreshBit();
   }
   else if(name == CYAN_HUE_BLEND_RANGE_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theCyanHueBlendRange), 0, 30);
      result->setCacheRefreshBit();
   }
   else if(name == CYAN_SATURATION_OFFSET_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theCyanSaturationOffset), -1, 1);
      result->setCacheRefreshBit();
   }
   else if(name == CYAN_INTENSITY_OFFSET_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theCyanIntensityOffset), -1, 1);
      result->setCacheRefreshBit();
   }
   else if(name == BLUE_HUE_OFFSET_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theBlueHueOffset), -180, 180);
      result->setCacheRefreshBit();
   }
   else if(name == BLUE_HUE_LOW_RANGE_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theBlueHueLowRange), 210, 270);
      result->setCacheRefreshBit();
   }
   else if(name == BLUE_HUE_HIGH_RANGE_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theBlueHueHighRange), 210, 270);
      result->setCacheRefreshBit();
   }
   else if(name == BLUE_HUE_BLEND_RANGE_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theBlueHueBlendRange), 0, 30);
      result->setCacheRefreshBit();
   }
   else if(name == BLUE_SATURATION_OFFSET_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theBlueSaturationOffset), -1, 1);
      result->setCacheRefreshBit();
   }
   else if(name == BLUE_INTENSITY_OFFSET_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theBlueIntensityOffset), -1, 1);
      result->setCacheRefreshBit();
   }
   else if(name == MAGENTA_HUE_OFFSET_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theMagentaHueOffset), -180, 180);
      result->setCacheRefreshBit();
   }
   else if(name == MAGENTA_HUE_LOW_RANGE_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theMagentaHueLowRange), 270, 330);
      result->setCacheRefreshBit();
   }
   else if(name == MAGENTA_HUE_HIGH_RANGE_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theMagentaHueHighRange), 270, 330);
      result->setCacheRefreshBit();
   }
   else if(name == MAGENTA_HUE_BLEND_RANGE_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theMagentaHueBlendRange), 0, 30);
      result->setCacheRefreshBit();
   }
   else if(name == MAGENTA_SATURATION_OFFSET_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theMagentaSaturationOffset), -1, 1);
      result->setCacheRefreshBit();
   }
   else if(name == MAGENTA_INTENSITY_OFFSET_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theMagentaIntensityOffset), -1, 1);
      result->setCacheRefreshBit();
   }
   else if(name == WHITE_OBJECT_CLIP_KW)
   {
      result = new ossimNumericProperty(name, ossimString::toString(theWhiteObjectClip), 0.8, 1.0);
      result->setCacheRefreshBit();
   }
   else
   {
     result = ossimImageSourceFilter::getProperty(name);
   }
   
   return result;
}

void ossimHsiRemapper::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSourceFilter::getPropertyNames(propertyNames);
   propertyNames.push_back(MASTER_HUE_OFFSET_KW);
   propertyNames.push_back(MASTER_SATURATION_OFFSET_KW);
   propertyNames.push_back(MASTER_INTENSITY_OFFSET_KW);
   propertyNames.push_back(MASTER_INTENSITY_LOW_CLIP_KW);
   propertyNames.push_back(MASTER_INTENSITY_HIGH_CLIP_KW);
   propertyNames.push_back(RED_HUE_OFFSET_KW);
   propertyNames.push_back(RED_HUE_LOW_RANGE_KW);
   propertyNames.push_back(RED_HUE_HIGH_RANGE_KW);
   propertyNames.push_back(RED_HUE_BLEND_RANGE_KW);
   propertyNames.push_back(RED_SATURATION_OFFSET_KW);
   propertyNames.push_back(RED_INTENSITY_OFFSET_KW);
   propertyNames.push_back(YELLOW_HUE_OFFSET_KW);
   propertyNames.push_back(YELLOW_HUE_LOW_RANGE_KW);
   propertyNames.push_back(YELLOW_HUE_HIGH_RANGE_KW);
   propertyNames.push_back(YELLOW_HUE_BLEND_RANGE_KW);
   propertyNames.push_back(YELLOW_SATURATION_OFFSET_KW);
   propertyNames.push_back(YELLOW_INTENSITY_OFFSET_KW);
   propertyNames.push_back(GREEN_HUE_OFFSET_KW);
   propertyNames.push_back(GREEN_HUE_LOW_RANGE_KW);
   propertyNames.push_back(GREEN_HUE_HIGH_RANGE_KW);
   propertyNames.push_back(GREEN_HUE_BLEND_RANGE_KW);
   propertyNames.push_back(GREEN_SATURATION_OFFSET_KW);
   propertyNames.push_back(GREEN_INTENSITY_OFFSET_KW);
   propertyNames.push_back(CYAN_HUE_OFFSET_KW);
   propertyNames.push_back(CYAN_HUE_LOW_RANGE_KW);
   propertyNames.push_back(CYAN_HUE_HIGH_RANGE_KW);
   propertyNames.push_back(CYAN_HUE_BLEND_RANGE_KW);
   propertyNames.push_back(CYAN_SATURATION_OFFSET_KW);
   propertyNames.push_back(CYAN_INTENSITY_OFFSET_KW);
   propertyNames.push_back(BLUE_HUE_OFFSET_KW);
   propertyNames.push_back(BLUE_HUE_LOW_RANGE_KW);
   propertyNames.push_back(BLUE_HUE_HIGH_RANGE_KW);
   propertyNames.push_back(BLUE_HUE_BLEND_RANGE_KW);
   propertyNames.push_back(BLUE_SATURATION_OFFSET_KW);
   propertyNames.push_back(BLUE_INTENSITY_OFFSET_KW);
   propertyNames.push_back(MAGENTA_HUE_OFFSET_KW);
   propertyNames.push_back(MAGENTA_HUE_LOW_RANGE_KW);
   propertyNames.push_back(MAGENTA_HUE_HIGH_RANGE_KW);
   propertyNames.push_back(MAGENTA_HUE_BLEND_RANGE_KW);
   propertyNames.push_back(MAGENTA_SATURATION_OFFSET_KW);
   propertyNames.push_back(MAGENTA_INTENSITY_OFFSET_KW);
   propertyNames.push_back(WHITE_OBJECT_CLIP_KW);
}


void ossimHsiRemapper::allocate(const ossimIrect& rect)
{
   
   if(theInputConnection)
   {
      theTile = ossimImageDataFactory::instance()->create(this,this);
      theTile->initialize();

      ossim_uint32 width  = rect.width();
      ossim_uint32 height = rect.height();
      if(theBuffer)
      {
         delete [] theBuffer;
         theBuffer = 0;
      }
      ossim_uint32 size = width * height * 3; // Buffer always 3 bands.
      theBuffer = new double[size];
      memset(theBuffer, '\0', sizeof(double) * size);
      
      // Get the minimum normalized pixel value.
      theNormalizedMinPix = calculateMinNormValue();
   }
}

bool ossimHsiRemapper::loadState(const ossimKeywordlist& kwl,
                                 const char* prefix)
{
   static const char MODULE[] = "ossimHsiRemapper::loadState()";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entering..." << endl;
   }

   // Make a prefix.
   ossimString tmpPrefix;
   if (prefix) tmpPrefix += prefix;
   
   const char* lookupReturn;

   lookupReturn = kwl.find(tmpPrefix.c_str(), MASTER_HUE_OFFSET_KW);
   if(lookupReturn)
   {
      setMasterHueOffset(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), MASTER_SATURATION_OFFSET_KW);
   if(lookupReturn)
   {
      setMasterSaturationOffset(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), MASTER_INTENSITY_OFFSET_KW);
   if(lookupReturn)
   {
      setMasterIntensityOffset(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), MASTER_INTENSITY_LOW_CLIP_KW);
   if(lookupReturn)
   {
      setMasterIntensityLowClip(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), MASTER_INTENSITY_HIGH_CLIP_KW);
   if(lookupReturn)
   {
      setMasterIntensityHighClip(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), RED_HUE_OFFSET_KW);
   if(lookupReturn)
   {
      setRedHueOffset(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), RED_HUE_LOW_RANGE_KW);
   if(lookupReturn)
   {
      setRedHueLowRange(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), RED_HUE_HIGH_RANGE_KW);
   if(lookupReturn)
   {
      setRedHueHighRange(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), RED_HUE_BLEND_RANGE_KW);
   if(lookupReturn)
   {
      setRedHueBlendRange(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), RED_SATURATION_OFFSET_KW);
   if(lookupReturn)
   {
      setRedSaturationOffset(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), RED_INTENSITY_OFFSET_KW);
   if(lookupReturn)
   {
      setRedIntensityOffset(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), YELLOW_HUE_OFFSET_KW);
   if(lookupReturn)
   {
      setYellowHueOffset(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), YELLOW_HUE_LOW_RANGE_KW);
   if(lookupReturn)
   {
      setYellowHueLowRange(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), YELLOW_HUE_HIGH_RANGE_KW);
   if(lookupReturn)
   {
      setYellowHueHighRange(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), YELLOW_HUE_BLEND_RANGE_KW);
   if(lookupReturn)
   {
      setYellowHueBlendRange(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), YELLOW_SATURATION_OFFSET_KW);
   if(lookupReturn)
   {
      setYellowSaturationOffset(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), YELLOW_INTENSITY_OFFSET_KW);
   if(lookupReturn)
   {
      setYellowIntensityOffset(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), GREEN_HUE_OFFSET_KW);
   if(lookupReturn)
   {
      setGreenHueOffset(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), GREEN_HUE_LOW_RANGE_KW);
   if(lookupReturn)
   {
      setGreenHueLowRange(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), GREEN_HUE_HIGH_RANGE_KW);
   if(lookupReturn)
   {
      setGreenHueHighRange(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), GREEN_HUE_BLEND_RANGE_KW);
   if(lookupReturn)
   {
      setGreenHueBlendRange(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), GREEN_SATURATION_OFFSET_KW);
   if(lookupReturn)
   {
      setGreenSaturationOffset(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), GREEN_INTENSITY_OFFSET_KW);
   if(lookupReturn)
   {
      setGreenIntensityOffset(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), CYAN_HUE_OFFSET_KW);
   if(lookupReturn)
   {
      setCyanHueOffset(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), CYAN_HUE_LOW_RANGE_KW);
   if(lookupReturn)
   {
      setCyanHueLowRange(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), CYAN_HUE_HIGH_RANGE_KW);
   if(lookupReturn)
   {
      setCyanHueHighRange(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), CYAN_HUE_BLEND_RANGE_KW);
   if(lookupReturn)
   {
      setCyanHueBlendRange(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), CYAN_SATURATION_OFFSET_KW);
   if(lookupReturn)
   {
      setCyanSaturationOffset(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), CYAN_INTENSITY_OFFSET_KW);
   if(lookupReturn)
   {
      setCyanIntensityOffset(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), BLUE_HUE_OFFSET_KW);
   if(lookupReturn)
   {
      setBlueHueOffset(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), BLUE_HUE_LOW_RANGE_KW);
   if(lookupReturn)
   {
      setBlueHueLowRange(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), BLUE_HUE_HIGH_RANGE_KW);
   if(lookupReturn)
   {
      setBlueHueHighRange(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), BLUE_HUE_BLEND_RANGE_KW);
   if(lookupReturn)
   {
      setBlueHueBlendRange(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), BLUE_SATURATION_OFFSET_KW);
   if(lookupReturn)
   {
      setBlueSaturationOffset(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), BLUE_INTENSITY_OFFSET_KW);
   if(lookupReturn)
   {
      setBlueIntensityOffset(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), MAGENTA_HUE_OFFSET_KW);
   if(lookupReturn)
   {
      setMagentaHueOffset(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), MAGENTA_HUE_LOW_RANGE_KW);
   if(lookupReturn)
   {
      setMagentaHueLowRange(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), MAGENTA_HUE_HIGH_RANGE_KW);
   if(lookupReturn)
   {
      setMagentaHueHighRange(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), MAGENTA_HUE_BLEND_RANGE_KW);
   if(lookupReturn)
   {
      setMagentaHueBlendRange(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), MAGENTA_SATURATION_OFFSET_KW);
   if(lookupReturn)
   {
      setMagentaSaturationOffset(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), MAGENTA_INTENSITY_OFFSET_KW);
   if(lookupReturn)
   {
      setMagentaIntensityOffset(atof(lookupReturn));
   }

   lookupReturn = kwl.find(tmpPrefix.c_str(), WHITE_OBJECT_CLIP_KW);
   if(lookupReturn)
   {
      setWhiteObjectClip(atof(lookupReturn));
   }

   //***
   // Initialize the base class.  Do this last so that the enable/disable
   // doesn't get overridden by the "set*" methods.
   //***
   ossimImageSourceFilter::loadState(kwl, tmpPrefix.c_str());
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << ""
         << *this
         << "\nreturning..."
         << endl;
   }

   return true;
}

bool ossimHsiRemapper::saveState(ossimKeywordlist& kwl,
                                 const char* prefix) const
{
   static const char MODULE[] = "ossimHsiRemapper::saveStateTo()";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << "entering..." << endl;
   }

   // Call the base class getStateFrom.
   ossimImageSourceFilter::saveState(kwl, prefix);

   kwl.add(prefix, MASTER_HUE_OFFSET_KW, theMasterHueOffset);

   kwl.add(prefix, MASTER_SATURATION_OFFSET_KW, theMasterSaturationOffset);

   kwl.add(prefix, MASTER_INTENSITY_OFFSET_KW, theMasterIntensityOffset);

   kwl.add(prefix, MASTER_INTENSITY_LOW_CLIP_KW, theMasterIntensityLowClip);

   kwl.add(prefix, MASTER_INTENSITY_HIGH_CLIP_KW, theMasterIntensityHighClip);

   kwl.add(prefix, RED_HUE_OFFSET_KW, theRedHueOffset);

   kwl.add(prefix, RED_HUE_LOW_RANGE_KW, theRedHueLowRange);

   kwl.add(prefix, RED_HUE_HIGH_RANGE_KW, theRedHueHighRange);

   kwl.add(prefix, RED_HUE_BLEND_RANGE_KW, theRedHueBlendRange);

   kwl.add(prefix, RED_SATURATION_OFFSET_KW, theRedSaturationOffset);
           
   kwl.add(prefix, RED_INTENSITY_OFFSET_KW, theRedIntensityOffset);

   kwl.add(prefix, YELLOW_HUE_OFFSET_KW, theYellowHueOffset);

   kwl.add(prefix, YELLOW_HUE_LOW_RANGE_KW, theYellowHueLowRange);

   kwl.add(prefix, YELLOW_HUE_HIGH_RANGE_KW, theYellowHueHighRange);
           
   kwl.add(prefix, YELLOW_HUE_BLEND_RANGE_KW, theYellowHueBlendRange);

   kwl.add(prefix, YELLOW_SATURATION_OFFSET_KW, theYellowSaturationOffset);

   kwl.add(prefix, YELLOW_INTENSITY_OFFSET_KW, theYellowIntensityOffset);

   kwl.add(prefix, GREEN_HUE_OFFSET_KW, theGreenHueOffset);

   kwl.add(prefix, GREEN_HUE_LOW_RANGE_KW, theGreenHueLowRange);

   kwl.add(prefix, GREEN_HUE_HIGH_RANGE_KW, theGreenHueHighRange);

   kwl.add(prefix, GREEN_HUE_BLEND_RANGE_KW, theGreenHueBlendRange);
           
   kwl.add(prefix, GREEN_SATURATION_OFFSET_KW, theGreenSaturationOffset);
           
   kwl.add(prefix, GREEN_INTENSITY_OFFSET_KW, theGreenIntensityOffset);

   kwl.add(prefix, CYAN_HUE_OFFSET_KW, theCyanHueOffset);

   kwl.add(prefix, CYAN_HUE_LOW_RANGE_KW, theCyanHueLowRange);

   kwl.add(prefix, CYAN_HUE_HIGH_RANGE_KW, theCyanHueHighRange);
           
   kwl.add(prefix, CYAN_HUE_BLEND_RANGE_KW, theCyanHueBlendRange);
           
   kwl.add(prefix, CYAN_SATURATION_OFFSET_KW, theCyanSaturationOffset);
           
   kwl.add(prefix, CYAN_INTENSITY_OFFSET_KW, theCyanIntensityOffset);

   kwl.add(prefix, BLUE_HUE_OFFSET_KW, theBlueHueOffset);

   kwl.add(prefix, BLUE_HUE_LOW_RANGE_KW, theBlueHueLowRange);

   kwl.add(prefix, BLUE_HUE_HIGH_RANGE_KW, theBlueHueHighRange);

   kwl.add(prefix, BLUE_HUE_BLEND_RANGE_KW, theBlueHueBlendRange);

   kwl.add(prefix, BLUE_SATURATION_OFFSET_KW, theBlueSaturationOffset);
           
   kwl.add(prefix, BLUE_INTENSITY_OFFSET_KW, theBlueIntensityOffset);
           
   kwl.add(prefix, MAGENTA_HUE_OFFSET_KW, theMagentaHueOffset);
           
   kwl.add(prefix, MAGENTA_HUE_LOW_RANGE_KW, theMagentaHueLowRange);
           
   kwl.add(prefix, MAGENTA_HUE_HIGH_RANGE_KW, theMagentaHueHighRange);
           
   kwl.add(prefix, MAGENTA_HUE_BLEND_RANGE_KW, theMagentaHueBlendRange);
           
   kwl.add(prefix, MAGENTA_SATURATION_OFFSET_KW, theMagentaSaturationOffset);

   kwl.add(prefix, MAGENTA_INTENSITY_OFFSET_KW, theMagentaIntensityOffset);

   kwl.add(prefix, WHITE_OBJECT_CLIP_KW, theWhiteObjectClip);

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << "returning..." << endl;
   }

   return true;
}

void ossimHsiRemapper::resetGroup(int color_group)
{
   switch (color_group)
   {
      case RED:
         resetRed();
         break;
      case YELLOW:
         resetYellow();
         break;
      case GREEN:
         resetGreen();
         break;
      case CYAN:
         resetCyan();
         break;
      case BLUE:
         resetBlue();
         break;
      case MAGENTA:
         resetMagenta();
         break;
      case ALL:
         resetMaster();
         break;
      default:
         ossimNotify(ossimNotifyLevel_NOTICE)
            << "ossimHsiRemapper::setSaturationOffset NOTICE:  Range error!"
            << endl;
         break;
   }
   verifyEnabled();
}

void ossimHsiRemapper::resetAll()
{
   theMasterHueOffset          = 0.0;
   theMasterSaturationOffset   = 0.0;
   theMasterIntensityOffset    = 0.0;
   theMasterIntensityLowClip   = 0.0;
   theMasterIntensityHighClip  = 1.0;
   
   theRedHueOffset             = 0.0;
   theRedHueLowRange           = -30.0;
   theRedHueHighRange          = 30.0;
   theRedHueBlendRange         = DEFAULT_BLEND;
   theRedSaturationOffset      = 0.0;
   theRedIntensityOffset       = 0.0;
   
   theYellowHueOffset          = 0.0;
   theYellowHueLowRange        = 30.0;
   theYellowHueHighRange       = 90.0;
   theYellowHueBlendRange      = DEFAULT_BLEND;
   theYellowSaturationOffset   = 0.0;
   theYellowIntensityOffset    = 0.0;
   
   theGreenHueOffset           = 0.0;
   theGreenHueLowRange         = 90.0;
   theGreenHueHighRange        = 150.0;
   theGreenHueBlendRange       = DEFAULT_BLEND;
   theGreenSaturationOffset    = 0.0;
   theGreenIntensityOffset     = 0.0;
   
   theCyanHueOffset            = 0.0;
   theCyanHueLowRange          = 150.0;
   theCyanHueHighRange         = 210.0;
   theCyanHueBlendRange        = DEFAULT_BLEND;
   theCyanSaturationOffset     = 0.0;
   theCyanIntensityOffset      = 0.0;
   
   theBlueHueOffset            = 0.0;
   theBlueHueLowRange          = 210.0;
   theBlueHueHighRange         = 270.0;
   theBlueHueBlendRange        = DEFAULT_BLEND;
   theBlueSaturationOffset     = 0.0;
   theBlueIntensityOffset      = 0.0;
   
   theMagentaHueOffset         = 0.0;
   theMagentaHueLowRange       = 270.0;
   theMagentaHueHighRange      = 330.0;
   theMagentaHueBlendRange     = DEFAULT_BLEND;
   theMagentaSaturationOffset  = 0.0;
   theMagentaIntensityOffset   = 0.0;

   theWhiteObjectClip          = 1.0;
   theValidFlag = false;
   //disableSource();
}

void ossimHsiRemapper::resetMaster()
{
   theMasterHueOffset          = 0.0;
   theMasterSaturationOffset   = 0.0;
   theMasterIntensityOffset    = 0.0;
   theMasterIntensityLowClip   = 0.0;
   theMasterIntensityHighClip  = 1.0;

   verifyEnabled();
}

void ossimHsiRemapper::resetRed()
{
   theRedHueOffset             = 0.0;
   theRedHueLowRange           = -30.0;
   theRedHueHighRange          = 30.0;
   theRedHueBlendRange         = DEFAULT_BLEND;
   theRedSaturationOffset      = 0.0;
   theRedIntensityOffset       = 0.0;

   verifyEnabled();
}

void ossimHsiRemapper::resetYellow()
{
   theYellowHueOffset          = 0.0;
   theYellowHueLowRange        = 30.0;
   theYellowHueHighRange       = 90.0;
   theYellowHueBlendRange      = DEFAULT_BLEND;
   theYellowSaturationOffset   = 0.0;
   theYellowIntensityOffset    = 0.0;

   verifyEnabled();
}

void ossimHsiRemapper::resetGreen()
{
   theGreenHueOffset           = 0.0;
   theGreenHueLowRange         = 90.0;
   theGreenHueHighRange        = 150.0;
   theGreenHueBlendRange       = DEFAULT_BLEND;
   theGreenSaturationOffset    = 0.0;
   theGreenIntensityOffset     = 0.0;

   verifyEnabled();
}

void ossimHsiRemapper::resetCyan()
{
   theCyanHueOffset            = 0.0;
   theCyanHueLowRange          = 150.0;
   theCyanHueHighRange         = 210.0;
   theCyanHueBlendRange        = DEFAULT_BLEND;
   theCyanSaturationOffset     = 0.0;
   theCyanIntensityOffset      = 0.0;

   verifyEnabled();
}

void ossimHsiRemapper::resetBlue()
{
   theBlueHueOffset            = 0.0;
   theBlueHueLowRange          = 210.0;
   theBlueHueHighRange         = 270.0;
   theBlueHueBlendRange        = DEFAULT_BLEND;
   theBlueSaturationOffset     = 0.0;
   theBlueIntensityOffset      = 0.0;

   verifyEnabled();
}

void ossimHsiRemapper::resetMagenta()
{
   theMagentaHueOffset         = 0.0;
   theMagentaHueLowRange       = 270.0;
   theMagentaHueHighRange      = 330.0;
   theMagentaHueBlendRange     = DEFAULT_BLEND;
   theMagentaSaturationOffset  = 0.0;
   theMagentaIntensityOffset   = 0.0;

   verifyEnabled();
}

void ossimHsiRemapper::verifyEnabled()
{
   // Start off disabled...
   //disableSource();
   theValidFlag = false;
   if (!theInputConnection)
   {
      //***
      // Since this filter can be constructed with no input connection do not
      // output and error, simply return.
      //***
      return;
   }
   
   //***
   // Add all the offsets and the low clips.
   // If greater than zero enable getTile method.
   //***
   double d = theMasterHueOffset         +
           theMasterSaturationOffset  +
           theMasterIntensityOffset   +
           theMasterIntensityLowClip  +
           theRedHueOffset            +
           theRedSaturationOffset     +
           theRedIntensityOffset      +
           theYellowHueOffset         +
           theYellowSaturationOffset  +
           theYellowIntensityOffset   +
           theGreenHueOffset          +
           theGreenSaturationOffset   +
           theGreenIntensityOffset    +
           theCyanHueOffset           +
           theCyanSaturationOffset    +
           theCyanIntensityOffset     +
           theBlueHueOffset           +
           theBlueSaturationOffset    +
           theBlueIntensityOffset     +
           theMagentaHueOffset        +
           theMagentaSaturationOffset +
           theMagentaIntensityOffset;
   
   if ( d != 0.0 ||
        theMasterIntensityHighClip != 1.0 ||
        theWhiteObjectClip != 1.0 )
   {
      theValidFlag = true;
     // enableSource();
   }
}

void ossimHsiRemapper::setMasterHueOffset(double offset)
{
   if (offset >= -180.0 && offset <= 180.0)
   {
      theMasterHueOffset = offset;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setMasterHueOffset range error:"
         << "\nOffset of " << offset << " is out of range!"
         << "\nMust be equal to or greater than -180.0 and less than"
         << "\nor equal to 180.0"
         << endl;
   }
}

void ossimHsiRemapper::setMasterSaturationOffset(double offset)
{
   if (offset >= -1.0 && offset <= 1.0)
   {
      theMasterSaturationOffset = offset;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setMasterSaturationOffset range error:"
         << "\nOffset of " << offset << " is out of range!"
         << "\nMust be equal to or greater than -1.0 and less than"
         << "\nor equal to 1.0"
         << endl;
   }
}

void ossimHsiRemapper::setMasterIntensityOffset(double offset)
{
   if (offset >= -1.0 && offset <= 1.0)
   {
      theMasterIntensityOffset = offset;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setMasterIntensityOffset range error:"
         << "\nOffset of " << offset << " is out of range!"
         << "\nMust be equal to or greater than -1.0 and less than"
         << "\nor equal to 1.0"
         << endl;
   }
}

void ossimHsiRemapper::setMasterIntensityLowClip(double clip)
{
   if (clip >= 0.0 && clip < theMasterIntensityHighClip)
   {
      theMasterIntensityLowClip = clip;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setMasterIntensityClip range error:"
         << "\nLow clip of " << clip << " is out of range!"
         << "\nMust be equal to or greater than 0.0 and less than"
         << "\nthe high clip of " << theMasterIntensityHighClip
         << endl;
   }
}

void ossimHsiRemapper::setMasterIntensityHighClip(double clip)
{
   if (clip <= 1.0 && clip > theMasterIntensityLowClip)
   {
      theMasterIntensityHighClip = clip;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setMasterIntensityHighClip range error:"
         << "\nHigh clip of " << clip << " is out of range!"
         << "\nMust be greater than low clip of "
         << theMasterIntensityLowClip << " and less than 1.0"
         << endl;
   }
}

void ossimHsiRemapper::setRedHueOffset(double offset)
{
   if (offset >= -180.0 && offset <= 180.0)
   {
      theRedHueOffset = offset;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setRedHueOffset range error:"
         << "\nOffset of " << offset << " is out of range!"
         << "\nMust be equal to or greater than -180.0 and less than"
         << "\nor equal to 180.0"
         << endl;
   }
}

void ossimHsiRemapper::setRedHueLowRange(double range)
{
   //***
   // Default red range: 330 - 30
   // Allow 315 to 15 as long as it's less than the high range.
   // Note:  Store the range as a negative if it's between 315 and 360.
   //***
   double r  = range;
   double h  = theRedHueHighRange;

   if (r >= 315.0 && r < 360.0) r = r - 360.0;
   if (h >= 315.0 && h < 360.0) h = h - 360.0;

   if (r >= -45.0 && r <= 15.0 && r < h)
   {
      theRedHueLowRange = r;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setRedHueLow range error:"
         << "\nRange of " << range << " is out of range!"
         << endl;
   }
}

void ossimHsiRemapper::setRedHueHighRange(double range)
{
   if (range == 360.0) range = 0.0;
   
   //***
   // Default red range: 330 - 30
   // Allow 345 to 45 as long as it's greater than the low range.
   // Note:  Store the range as a negative if it's between 345 and 360.
   //***

   double r = range;
   double l = theRedHueLowRange;

   if (r >= 315.0 && r < 360.0) r = r - 360.0;
   if (l >= 315.0 && l < 360.0) l = l - 360.0;

   if (r >= -15.0 && r <= 45 && l < r)
   {
      theRedHueHighRange = r;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setRedHueHigh range error:"
         << "\nRange of " << range << " is out of range!"
         << endl;
   }
}

void ossimHsiRemapper::setRedHueBlendRange(double range)
{
   //***
   // Allowable range: 0.0 to 30.0 as long as it's not greater than the
   // half range.  If so snap it to half range.
   //***
   if ( range >= 0.0  &&
        range <= MAX_BLEND )
   {
      double half_range = (theRedHueHighRange - theRedHueLowRange) / 2.0;

      if (range <= half_range)
      {
         theRedHueBlendRange = range;
      }
      else
      {
         // Put it in the middle of the range.
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimHsiRemapper::setRedHueBlendRange range error:"
            << "\nRange of " << range << " is greater than the full range"
            << "\ndivided by 2!"
            << endl;
         
         theRedHueBlendRange = half_range;
      }
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setRedHueBlendRange range error:"
         << "\nRange of " << range << " is out of range!"
         << endl;
   }
}

void ossimHsiRemapper::setRedSaturationOffset(double offset)
{
   if (offset >= -1.0 && offset <= 1.0)
   {
      theRedSaturationOffset = offset;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setRedSaturationOffset range error:"
         << "\nOffset of " << offset << " is out of range!"
         << "\nMust be equal to or greater than -1.0 and less than"
         << "\nor equal to 1.0"
         << endl;
   }
}

void ossimHsiRemapper::setRedIntensityOffset(double offset)
{
   if (offset >= -1.0 && offset <= 1.0)
   {
      theRedIntensityOffset = offset;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setRedIntensityOffset range error:"
         << "\nOffset of " << offset << " is out of range!"
         << "\nMust be equal to or greater than -1.0 and less than"
         << "\nor equal to 1.0"
         << endl;
   }
}

void ossimHsiRemapper::setYellowHueOffset(double offset)
{
   if (offset >= -180.0 && offset <= 180.0)
   {
      theYellowHueOffset = offset;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setYellowHueOffset range error:"
         << "\nOffset of " << offset << " is out of range!"
         << "\nMust be equal to or greater than -180.0 and less than"
         << "\nor equal to 180.0"
         << endl;
   }
}

void ossimHsiRemapper::setYellowHueLowRange(double range)
{
   //***
   // Default yellow range: 30 - 90
   // Allow 15 to 75 as long as it's less than the high range.
   //***
   if ( range >= 15.0 &&
        range <= 75.0 &&
        range < theYellowHueHighRange )
   {
      theYellowHueLowRange = range;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setYellowHueLowRange range error:"
         << "\nRange of " << range << " is out of range!"
         << endl;
   }
}

void ossimHsiRemapper::setYellowHueHighRange(double range)
{
   //***
   // Default yellow range: 30 - 90
   // Allow 45 to 105 as long as it's greater than the low range.
   //***
   if ( range >= 45.0  &&
        range <= 105.0 &&
        range > theYellowHueLowRange )
   {
      theYellowHueHighRange = range;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setYellowHueHighRange range error:"
         << "\nRange of " << range << " is out of range!"
         << endl;
   }
}

void ossimHsiRemapper::setYellowHueBlendRange(double range)
{
   //***
   // Allowable range: 0.0 to 30.0 as long as it's not greater than the
   // half range.  If so snap it to half range.
   //***
   if ( range >= 0.0  &&
        range <= MAX_BLEND )
   {
      double half_range = (theYellowHueHighRange - theYellowHueLowRange) / 2.0;
      
      if (range <= half_range)
      {
         theYellowHueBlendRange = range;
      }
      else
      {
         // Put it in the middle of the range.
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimHsiRemapper::setYellowHueBlendRange range error:"
            << "\nRange of " << range << " is greater than the full range"
            << "\ndivided by 2!"
            << endl;
         
         theYellowHueBlendRange = half_range;
      } 
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setYellowHueBlendRange range error:"
         << "\nRange of " << range << " is out of range!"
         << endl;
   }
}

void ossimHsiRemapper::setYellowSaturationOffset(double offset)
{
   if (offset >= -1.0 && offset <= 1.0)
   {
      theYellowSaturationOffset = offset;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setYellowSaturationOffset range error:"
         << "\nOffset of " << offset << " is out of range!"
         << "\nMust be equal to or greater than -1.0 and less than"
         << "\nor equal to 1.0"
         << endl;
   }
}

void ossimHsiRemapper::setYellowIntensityOffset(double offset)
{
   if (offset >= -1.0 && offset <= 1.0)
   {
      theYellowIntensityOffset = offset;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setYellowIntensityOffset range error:"
         << "\nOffset of " << offset << " is out of range!"
         << "\nMust be equal to or greater than -1.0 and less than"
         << "\nor equal to 1.0"
         << endl;
   }
}

void ossimHsiRemapper::setGreenHueOffset(double offset)
{
   if (offset >= -180.0 && offset <= 180.0)
   {
      theGreenHueOffset = offset;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setGreenHueOffset range error:"
         << "\nOffset of " << offset << " is out of range!"
         << "\nMust be equal to or greater than -180.0 and less than"
         << "\nor equal to 180.0"
         << endl;
   }
}

void ossimHsiRemapper::setGreenHueLowRange(double range)
{
   //***
   // Default green range:  90 - 150
   // Allow 75 to 135 as long as it's less than the high range.
   //***

   if ( range >= 75.0 &&
        range <= 135.0 &&
        range < theGreenHueHighRange )
   {
      theGreenHueLowRange = range;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setGreenHueLowRange range error:"
         << "\nRange of " << range << " is out of range!"
         << endl;
   }
}

void ossimHsiRemapper::setGreenHueHighRange(double range)
{
   //***
   // Default green range:  90 - 150
   // Allow 105 to 165 as long as it's greater than the low range.
   //***

   if ( range >= 105.0  &&
        range <= 165.0 &&
        range > theGreenHueLowRange )
   {
      theGreenHueHighRange = range;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setGreenHueHighRange range error:"
         << "\nRange of " << range << " is out of range!"
         << endl;
   }
}

void ossimHsiRemapper::setGreenHueBlendRange(double range)
{
   //***
   // Allowable range: 0.0 to 30.0 as long as it's not greater than the
   // half range.  If so snap it to half range.
   //***
   if ( range >= 0.0  &&
        range <= MAX_BLEND )
   {
      double half_range = (theGreenHueHighRange - theGreenHueLowRange) / 2.0;

      if (range <= half_range)
      {
         theGreenHueBlendRange = range;
      }
      else
      {
         // Put it in the middle of the range.
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimHsiRemapper::setGreenHueBlendRange range error:"
            << "\nRange of " << range << " is greater than the full range"
            << "\ndivided by 2!"
            << endl;
         
         theGreenHueBlendRange = half_range;
      }
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setGreenHueBlendRange range error:"
         << "\nRange of " << range << " is out of range!"
         << endl;
   }
}

void ossimHsiRemapper::setGreenSaturationOffset(double offset)
{
   if (offset >= -1.0 && offset <= 1.0)
   {
      theGreenSaturationOffset = offset;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setGreenSaturationOffset range error:"
         << "\nOffset of " << offset << " is out of range!"
         << "\nMust be equal to or greater than -1.0 and less than"
         << "\nor equal to 1.0"
         << endl;
   }
}

void ossimHsiRemapper::setGreenIntensityOffset(double offset)
{
   if (offset >= -1.0 && offset <= 1.0)
   {
      theGreenIntensityOffset = offset;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setGreenIntensityOffset range error:"
         << "\nOffset of " << offset << " is out of range!"
         << "\nMust be equal to or greater than -1.0 and less than"
         << "\nor equal to 1.0"
         << endl;
   }
}

void ossimHsiRemapper::setCyanHueOffset(double offset)
{
   if (offset >= -180.0 && offset <= 180.0)
   {
      theCyanHueOffset = offset;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setCyanHueOffset range error:"
         << "\nOffset of " << offset << " is out of range!"
         << "\nMust be equal to or greater than -180.0 and less than"
         << "\nor equal to 180.0"
         << endl;
   }
}

void ossimHsiRemapper::setCyanHueLowRange(double range)
{
   //***
   // Default cyan range: 150 - 210
   // Allow 135 to 195 as long as it's less than the high range.
   //***
   
   if ( range >= 135.0 &&
        range <= 195.0 &&
        range < theCyanHueHighRange )
   {
      theCyanHueLowRange = range;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setCyanHueLowRange range error:"
         << "\nRange of " << range << " is out of range!"
         << endl;
   }
}

void ossimHsiRemapper::setCyanHueHighRange(double range)
{
   //***
   // Default cyan range: 150 - 210
   // Allow 165 to 225 as long as it's greater than the low range.
   //***
   
   if ( range >= 165.0 &&
        range <= 225.0 &&
        range > theCyanHueLowRange )
   {
      theCyanHueHighRange = range;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setCyanHueHighRange range error:"
         << "\nRange of " << range << " is out of range!"
         << endl;
   }
}

void ossimHsiRemapper::setCyanHueBlendRange(double range)
{
   //***
   // Allowable range: 0.0 to 30.0 as long as it's not greater than the
   // half range.  If so snap it to half range.
   //***
   if ( range >= 0.0  &&
        range <= MAX_BLEND )
   {
      double half_range = (theCyanHueHighRange - theCyanHueLowRange) / 2.0;
      
      if (range <= half_range)
      {
         theCyanHueBlendRange = range;
      }
      else
      {
         // Put it in the middle of the range.
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimHsiRemapper::setCyanHueBlendRange range error:"
            << "\nRange of " << range << " is greater than the full range"
            << "\ndivided by 2!"
            << endl;
         
         theCyanHueBlendRange = half_range;
      } 
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setCyanHueBlendRange range error:"
         << "\nRange of " << range << " is out of range!"
         << endl;
   }
}

void ossimHsiRemapper::setCyanSaturationOffset(double offset)
{
   if (offset >= -1.0 && offset <= 1.0)
   {
      theCyanSaturationOffset = offset;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setCyanSaturationOffset range error:"
         << "\nOffset of " << offset << " is out of range!"
         << "\nMust be equal to or greater than -1.0 and less than"
         << "\nor equal to 1.0"
         << endl;
   }
}

void ossimHsiRemapper::setCyanIntensityOffset(double offset)
{
   if (offset >= -1.0 && offset <= 1.0)
   {
      theCyanIntensityOffset = offset;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setCyanIntensityOffset range error:"
         << "\nOffset of " << offset << " is out of range!"
         << "\nMust be equal to or greater than -1.0 and less than"
         << "\nor equal to 1.0"
         << endl;
   }
}

void ossimHsiRemapper::setBlueHueOffset(double offset)
{
   if (offset >= -180.0 && offset <= 180.0)
   {
      theBlueHueOffset = offset;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setBlueHueOffset range error:"
         << "\nOffset of " << offset << " is out of range!"
         << "\nMust be equal to or greater than -180.0 and less than"
         << "\nor equal to 180.0"
         << endl;
   }
}

void ossimHsiRemapper::setBlueHueLowRange(double range)
{
   //***
   // Default blue range: 210 - 270
   // Allow 195 to 255 as long as it's less than the high range.
   //***
   if ( range >= 195.0 &&
        range <= 255.0 &&
        range < theBlueHueHighRange )
   {
      theBlueHueLowRange = range;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setBlueHueLowRange range error:"
         << "\nRange of " << range << " is out of range!"
         << endl;
   }
}

void ossimHsiRemapper::setBlueHueHighRange(double range)
{
   //***
   // Default blue range: 210 - 270
   // Allow 225 to 285 as long as it's greater than the low range.
   //***
   if ( range >= 225.0  &&
        range <= 285.0 &&
        range > theBlueHueLowRange )
   {
      theBlueHueHighRange = range;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setBlueHueHighRange range error:"
         << "\nRange of " << range << " is out of range!"
         << endl;
   }
}

void ossimHsiRemapper::setBlueHueBlendRange(double range)
{
   //***
   // Allowable range: 0.0 to 30.0 as long as it's not greater than the
   // half range.  If so snap it to half range.
   //***
   if ( range >= 0.0  &&
        range <= MAX_BLEND )
   {
      double half_range = (theBlueHueHighRange - theBlueHueLowRange) / 2.0;

      if (range <= half_range)
      {
         theBlueHueBlendRange = range;
      }
      else
      {
         // Put it in the middle of the range.
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimHsiRemapper::setBlueHueBlendRange range error:"
            << "\nRange of " << range << " is greater than the full range"
            << "\ndivided by 2!"
            << endl;
         
         theBlueHueBlendRange = half_range;
      }
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setBlueHueBlendRange range error:"
         << "\nRange of " << range << " is out of range!"
         << endl;
   }
}

void ossimHsiRemapper::setBlueSaturationOffset(double offset)
{
   if (offset >= -1.0 && offset <= 1.0)
   {
      theBlueSaturationOffset = offset;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setBlueSaturationOffset range error:"
         << "\nOffset of " << offset << " is out of range!"
         << "\nMust be equal to or greater than -1.0 and less than"
         << "\nor equal to 1.0"
         << endl;
   }
}

void ossimHsiRemapper::setBlueIntensityOffset(double offset)
{
   if (offset >= -1.0 && offset <= 1.0)
   {
      theBlueIntensityOffset = offset;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setBlueIntensityOffset range error:"
         << "\nOffset of " << offset << " is out of range!"
         << "\nMust be equal to or greater than -1.0 and less than"
         << "\nor equal to 1.0"
         << endl;
   }
}

void ossimHsiRemapper::setMagentaHueOffset(double offset)
{
   if (offset >= -180.0 && offset <= 180.0)
   {
      theMagentaHueOffset = offset;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setMagentaHueOffset range error:"
         << "\nOffset of " << offset << " is out of range!"
         << "\nMust be equal to or greater than -180.0 and less than"
         << "\nor equal to 180.0"
         << endl;
   }
}

void ossimHsiRemapper::setMagentaHueLowRange(double range)
{
   //***
   // Default magenta range:  270 - 330
   // Allow 255 to 315 as long as it's less than the high range.
   //***
   if ( range >= 255.0 &&
        range <= 315.0 &&
        range < theMagentaHueHighRange )
   {
      theMagentaHueLowRange = range;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setMagentaHueLowRange range error:"
         << "\nRange of " << range << " is out of range!"
         << endl;
   }
}

void ossimHsiRemapper::setMagentaHueHighRange(double range)
{
   //***
   // Default magenta range:  270 - 330
   // Allow 285 to 345 as long as it's greater than the low range.
   //***
   if ( range >= 285.0  &&
        range <= 345.0 &&
        range > theMagentaHueLowRange )
   {
      theMagentaHueHighRange = range;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setMagentaHueHighRange range error:"
         << "\nRange of " << range << " is out of range!"
         << endl;
   }
}

void ossimHsiRemapper::setMagentaHueBlendRange(double range)
{
   //***
   // Allowable range: 0.0 to 30.0 as long as it's not greater than the
   // half range.  If so snap it to half range.
   //***
   if ( range >= 0.0  &&
        range <= MAX_BLEND )
   {
      double half_range = (theMagentaHueHighRange-theMagentaHueLowRange)/2.0;

      if (range <= half_range)
      {
         theMagentaHueBlendRange = range;
      }
      else
      {
         // Put it in the middle of the range.
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimHsiRemapper::setMagentaHueBlendRange range error:"
            << "\nRange of " << range << " is greater than the full range"
            << "\ndivided by 2!"
            << endl;
         
         theMagentaHueBlendRange = half_range;
      }
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setMagentaHueBlendRange range error:"
         << "\nRange of " << range << " is out of range!"
         << endl;
   }
}

void ossimHsiRemapper::setMagentaSaturationOffset(double offset)
{
   if (offset >= -1.0 && offset <= 1.0)
   {
      theMagentaSaturationOffset = offset;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setMagentaSaturationOffset range error:"
         << "\nOffset of " << offset << " is out of range!"
         << "\nMust be equal to or greater than -1.0 and less than"
         << "\nor equal to 1.0"
         << endl;
   }
}

void ossimHsiRemapper::setMagentaIntensityOffset(double offset)
{
   if (offset >= -1.0 && offset <= 1.0)
   {
      theMagentaIntensityOffset = offset;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHsiRemapper::setMagentaIntensityOffset range error:"
         << "\nOffset of " << offset << " is out of range!"
         << "\nMust be equal to or greater than -1.0 and less than"
         << "\nor equal to 1.0"
         << endl;
   }
}

double ossimHsiRemapper::calculateMinNormValue()
{
   static const char MODULE[] = "ossimHsiRemapper::calculateMinNormValue";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " Entered..." << endl;
   }
   
   if (!theTile)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nClass not initialized!"
         << endl;
      return 0.0;
   }

   double min_pix = theTile->getMinPix(0);
   double max_pix = theTile->getMaxPix(0);

   for (ossim_uint32 band=1; band<theTile->getNumberOfBands(); ++band)
   {
      if (min_pix != theTile->getMinPix(band))
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " Warning:"
            << "\nMixed minimum values for bands..."
            << endl;
      }
      if (max_pix != theTile->getMaxPix(band))
      {
         ossimNotify(ossimNotifyLevel_WARN) << " Warning:"
              << "\nMixed maximum values for bands..."
              << endl;
      }
   }

   if (min_pix < 0.0)
   {
      //
      // Assigning some arbituary number for float data.
      // This should really come from the normalizer (ossimImageData) of
      // the data.
      //
      return OSSIM_DEFAULT_MIN_PIX_NORM_DOUBLE;
   }

   return (min_pix / max_pix);
}

ossim_uint32 ossimHsiRemapper::getNumberOfOutputBands() const
{
   if (isSourceEnabled()&&theValidFlag) // Always rgb tile out...
   {
      return 3;
   }

   // Filter bypassed so getTile will return input tile.
   return ossimImageSourceFilter::getNumberOfInputBands();
}


double ossimHsiRemapper::getMasterHueOffset () const
{
   return theMasterHueOffset;
}

double ossimHsiRemapper::getMasterSaturationOffset() const
{
   return theMasterSaturationOffset;
}

double ossimHsiRemapper::getMasterIntensityOffset() const
{
   return theMasterIntensityOffset;
}

double ossimHsiRemapper::getMasterIntensityLowClip() const
{
   return theMasterIntensityLowClip;
}

double ossimHsiRemapper::getMasterIntensityHighClip() const
{
   return theMasterIntensityHighClip;
}

double ossimHsiRemapper::getRedHueOffset() const
{
   return theRedHueOffset;
}

double ossimHsiRemapper::getRedHueLowRange() const
{
   return theRedHueLowRange;
}

double ossimHsiRemapper::getRedHueHighRange() const
{
   return theRedHueHighRange;
}

double ossimHsiRemapper::getRedHueBlendRange() const
{
   return theRedHueBlendRange;
}

double ossimHsiRemapper::getRedSaturationOffset() const
{
   return theRedSaturationOffset;
}

double ossimHsiRemapper::getRedIntensityOffset() const
{
   return theRedIntensityOffset;
}

double ossimHsiRemapper::getYellowHueOffset () const
{
   return theYellowHueOffset;
}

double ossimHsiRemapper::getYellowHueLowRange() const
{
   return theYellowHueLowRange;
}

double ossimHsiRemapper::getYellowHueHighRange() const
{
   return theYellowHueHighRange;
}

double ossimHsiRemapper::getYellowHueBlendRange() const
{
   return theYellowHueBlendRange;
}

double ossimHsiRemapper::getYellowSaturationOffset() const
{
   return theYellowSaturationOffset;
}

double ossimHsiRemapper::getYellowIntensityOffset() const
{
   return theYellowIntensityOffset;
}

double ossimHsiRemapper::getGreenHueOffset () const
{
   return theGreenHueOffset;
}

double ossimHsiRemapper::getGreenHueLowRange() const
{
   return theGreenHueLowRange;
}

double ossimHsiRemapper::getGreenHueHighRange() const
{
   return theGreenHueHighRange;
}

double ossimHsiRemapper::getGreenHueBlendRange() const
{
   return theGreenHueBlendRange;
}

double ossimHsiRemapper::getGreenSaturationOffset() const
{
   return theGreenSaturationOffset;
}

double ossimHsiRemapper::getGreenIntensityOffset() const
{
   return theGreenIntensityOffset;
}

double ossimHsiRemapper::getCyanHueOffset () const
{
   return theCyanHueOffset;
}

double ossimHsiRemapper::getCyanHueLowRange() const
{
   return theCyanHueLowRange;
}

double ossimHsiRemapper::getCyanHueHighRange() const
{
   return theCyanHueHighRange;
}

double ossimHsiRemapper::getCyanHueBlendRange() const
{
   return theCyanHueBlendRange;
}

double ossimHsiRemapper::getCyanSaturationOffset() const
{
   return theCyanSaturationOffset;
}

double ossimHsiRemapper::getCyanIntensityOffset() const
{
   return theCyanIntensityOffset;
}

double ossimHsiRemapper::getBlueHueOffset () const
{
   return theBlueHueOffset;
}

double ossimHsiRemapper::getBlueHueLowRange() const
{
   return theBlueHueLowRange;
}

double ossimHsiRemapper::getBlueHueHighRange() const
{
   return theBlueHueHighRange;
}

double ossimHsiRemapper::getBlueHueBlendRange() const
{
   return theBlueHueBlendRange;
}

double ossimHsiRemapper::getBlueSaturationOffset() const
{
   return theBlueSaturationOffset;
}

double ossimHsiRemapper::getBlueIntensityOffset() const
{
   return theBlueIntensityOffset;
}

double ossimHsiRemapper::getMagentaHueOffset () const
{
   return theMagentaHueOffset;
}

double ossimHsiRemapper::getMagentaHueLowRange() const
{
   return theMagentaHueLowRange;
}

double ossimHsiRemapper::getMagentaHueHighRange() const
{
   return theMagentaHueHighRange;
}

double ossimHsiRemapper::getMagentaHueBlendRange() const
{
   return theMagentaHueBlendRange;
}

double ossimHsiRemapper::getMagentaSaturationOffset() const
{
   return theMagentaSaturationOffset;
}

double ossimHsiRemapper::getMagentaIntensityOffset() const
{
   return theMagentaIntensityOffset;
}

void ossimHsiRemapper::setHueOffset(int color_group, double offset)
{
   switch (color_group)
   {
      case RED:
         setRedHueOffset(offset);
         break;
      case YELLOW:
         setYellowHueOffset(offset);
         break;
      case GREEN:
         setGreenHueOffset(offset);
         break;
      case CYAN:
         setCyanHueOffset(offset);
         break;
      case BLUE:
         setBlueHueOffset(offset);
         break;
      case MAGENTA:
         setMagentaHueOffset(offset);
         break;
      case ALL:
         setMasterHueOffset(offset);
         break;
      default:
         ossimNotify(ossimNotifyLevel_NOTICE)
            << "ossimHsiRemapper::setHueOffset NOTICE:  Range error!" << endl;
   }
}

void ossimHsiRemapper::setHueLowRange(int color_group, double range)
{
   switch (color_group)
   {
      case RED:
         setRedHueLowRange(range);
         break;
      case YELLOW:
         setYellowHueLowRange(range);
         break;
      case GREEN:
         setGreenHueLowRange(range);
         break;
      case CYAN:
         setCyanHueLowRange(range);
         break;
      case BLUE:
         setBlueHueLowRange(range);
         break;
      case MAGENTA:
         setMagentaHueLowRange(range);
         break;
      default:
         ossimNotify(ossimNotifyLevel_NOTICE)
            << "ossimHsiRemapper::setHueLowRange NOTICE:  Range error!"
            << endl;
   }
}

void ossimHsiRemapper::setHueHighRange(int color_group, double range)
{
   switch (color_group)
   {
      case RED:
         setRedHueHighRange(range);
         break;
      case YELLOW:
         setYellowHueHighRange(range);
         break;
      case GREEN:
         setGreenHueHighRange(range);
         break;
      case CYAN:
         setCyanHueHighRange(range);
         break;
      case BLUE:
         setBlueHueHighRange(range);
         break;
      case MAGENTA:
         setMagentaHueHighRange(range);
         break;
      default:
         ossimNotify(ossimNotifyLevel_NOTICE)
            << "ossimHsiRemapper::setHueHighRange NOTICE:  Range error!"
            << endl;
   }
}

void ossimHsiRemapper::setHueBlendRange(int color_group, double range)
{
   switch (color_group)
   {
      case RED:
         setRedHueBlendRange(range);
         break;
      case YELLOW:
         setYellowHueBlendRange(range);
         break;
      case GREEN:
         setGreenHueBlendRange(range);
         break;
      case CYAN:
         setCyanHueBlendRange(range);
         break;
      case BLUE:
         setBlueHueBlendRange(range);
         break;
      case MAGENTA:
         setMagentaHueBlendRange(range);
         break;
      default:
         ossimNotify(ossimNotifyLevel_NOTICE)
            << "ossimHsiRemapper::setHueBlendRange NOTICE:  Range error!"
            << endl;
   }
}

void ossimHsiRemapper::setSaturationOffset(int color_group, double offset)
{
   switch (color_group)
   {
      case RED:
         setRedSaturationOffset(offset);
         break;
      case YELLOW:
         setYellowSaturationOffset(offset);
         break;
      case GREEN:
         setGreenSaturationOffset(offset);
         break;
      case CYAN:
         setCyanSaturationOffset(offset);
         break;
      case BLUE:
         setBlueSaturationOffset(offset);
         break;
      case MAGENTA:
         setMagentaSaturationOffset(offset);
         break;
      case ALL:
         setMasterSaturationOffset(offset);
         break;
      default:
         ossimNotify(ossimNotifyLevel_NOTICE)
            << "ossimHsiRemapper::setSaturationOffset NOTICE:  Range error!"
            << endl;
         break;
   }
}

void ossimHsiRemapper::setIntensityOffset(int color_group, double offset)
{
   switch (color_group)
   {
      case RED:
         setRedIntensityOffset(offset);
         break;
      case YELLOW:
         setYellowIntensityOffset(offset);
         break;
      case GREEN:
         setGreenIntensityOffset(offset);
         break;
      case CYAN:
         setCyanIntensityOffset(offset);
         break;
      case BLUE:
         setBlueIntensityOffset(offset);
         break;
      case MAGENTA:
         setMagentaIntensityOffset(offset);
         break;
      case ALL:
         setMasterIntensityOffset(offset);
         break;
      default:
         ossimNotify(ossimNotifyLevel_NOTICE)
            << "ossimHsiRemapper::setIntensityOffset NOTICE:  Range error!"
            << endl;
         break;
   }
}

double ossimHsiRemapper::getHueOffset(int color_group) const
{
   switch (color_group)
   {
      case RED:
         return getRedHueOffset();
         break;
      case YELLOW:
         return getYellowHueOffset();
         break;
      case GREEN:
         return getGreenHueOffset();
         break;
      case CYAN:
         return getCyanHueOffset();
         break;
      case BLUE:
         return getBlueHueOffset();
         break;
      case MAGENTA:
         return getMagentaHueOffset();
         break;
      case ALL:
         return getMasterHueOffset();
         break;
      default:
         ossimNotify(ossimNotifyLevel_NOTICE)
            << "ossimHsiRemapper::getHueOffset NOTICE:  Range error!"
            << endl;
         break;
   }
   return 0.0;
}

double ossimHsiRemapper::getHueLowRange(int color_group) const
{
   switch (color_group)
   {
      case RED:
         return getRedHueLowRange();
         break;
      case YELLOW:
         return getYellowHueLowRange();
         break;
      case GREEN:
         return getGreenHueLowRange();
         break;
      case CYAN:
         return getCyanHueLowRange();
         break;
      case BLUE:
         return getBlueHueLowRange();
         break;
      case MAGENTA:
         return getMagentaHueLowRange();
         break;
      default:
         ossimNotify(ossimNotifyLevel_NOTICE)
            << "ossimHsiRemapper::getHueLowRange NOTICE:  Range error!"
            << endl;
         break;
   }
   return 0.0;
}

double ossimHsiRemapper::getHueHighRange(int color_group) const
{
   switch (color_group)
   {
      case RED:
         return getRedHueHighRange();
         break;
      case YELLOW:
         return getYellowHueHighRange();
         break;
      case GREEN:
         return getGreenHueHighRange();
         break;
      case CYAN:
         return getCyanHueHighRange();
         break;
      case BLUE:
         return getBlueHueHighRange();
         break;
      case MAGENTA:
         return getMagentaHueHighRange();
         break;
      default:
         ossimNotify(ossimNotifyLevel_NOTICE)
            << "ossimHsiRemapper::getHueHighRange NOTICE:  Range error!"
            << endl;
         break;
   }
   return 0.0;
}

double ossimHsiRemapper::getHueBlendRange(int color_group) const
{
   switch (color_group)
   {
      case RED:
         return getRedHueBlendRange();
         break;
      case YELLOW:
         return getYellowHueBlendRange();
         break;
      case GREEN:
         return getGreenHueBlendRange();
         break;
      case CYAN:
         return getCyanHueBlendRange();
         break;
      case BLUE:
         return getBlueHueBlendRange();
         break;
      case MAGENTA:
         return getMagentaHueBlendRange();
         break;
      default:
         ossimNotify(ossimNotifyLevel_NOTICE)
            << "ossimHsiRemapper::getHueBlendRange NOTICE:  Range error!"
            << endl;
         break;
   }
   return 0.0;
}

double ossimHsiRemapper::getSaturationOffset(int color_group) const
{
   switch (color_group)
   {
      case RED:
         return getRedSaturationOffset();
         break;
      case YELLOW:
         return getYellowSaturationOffset();
         break;
      case GREEN:
         return getGreenSaturationOffset();
         break;
      case CYAN:
         return getCyanSaturationOffset();
         break;
      case BLUE:
         return getBlueSaturationOffset();
         break;
      case MAGENTA:
         return getMagentaSaturationOffset();
         break;
      case ALL:
         return getMasterSaturationOffset();
         break;
      default:
         ossimNotify(ossimNotifyLevel_NOTICE)
            << "ossimHsiRemapper::getSaturationOffset NOTICE:  Range error!"
            << endl;
         break;
   }
   return 0.0;
}

double ossimHsiRemapper::getIntensityOffset(int color_group) const
{
   switch (color_group)
   {
      case RED:
         return getRedIntensityOffset();
         break;
      case YELLOW:
         return getYellowIntensityOffset();
         break;
      case GREEN:
         return getGreenIntensityOffset();
         break;
      case CYAN:
         return getCyanIntensityOffset();
         break;
      case BLUE:
         return getBlueIntensityOffset();
         break;
      case MAGENTA:
         return getMagentaIntensityOffset();
         break;
      case ALL:
         return getMasterIntensityOffset();
         break;
      default:
         ossimNotify(ossimNotifyLevel_NOTICE)
            << "ossimHsiRemapper::getIntensityOffset NOTICE:  Range error!"
            << endl;
         break;
   }
   return 0.0;
}

void ossimHsiRemapper::setWhiteObjectClip(double clip)
{
   if ( (clip >= .8) && (clip <= 1.0) )
   {
      theWhiteObjectClip = clip;
      verifyEnabled();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "HsiRemapper::setWhiteObjectClip range error:"
         << "\nClip of " << clip << " is out of range!"
         << "\nMust be between .8 and 1.0"
         << endl;
   }
}

double ossimHsiRemapper::getWhiteObjectClip() const
{
   return theWhiteObjectClip;
}

void ossimHsiRemapper::resetWhiteObjectClip()
{
   theWhiteObjectClip = 1.0;
   verifyEnabled();
}

ossimString ossimHsiRemapper::getLongName()const
{
   return ossimString("HSI Remapper, filter with controls for all HSI(Hue, Saturation, and Intensity) adjustments.");
}

ossimString ossimHsiRemapper::getShortName()const
{
   return ossimString("HSI Remapper");
}

ostream& ossimHsiRemapper::print(ostream& os) const
{
   os << setiosflags(ios::fixed) << setprecision(2)
      << "\nossimHsiRemapper:"
      << "\ntheEnableFlag:                " << theEnableFlag

      << "\ntheMasterHueOffset:           " << theMasterHueOffset
      << "\ntheMasterSaturationOffset:    " << theMasterSaturationOffset
      << "\ntheMasterIntensityOffset:     " << theMasterIntensityOffset
      << "\ntheMasterIntensityLowClip:    " << theMasterIntensityLowClip
      << "\ntheMasterIntensityHighClip:   " << theMasterIntensityHighClip
      
      << "\ntheRedHueOffset:              " << theRedHueOffset
      << "\ntheRedHueLowRange:            " << theRedHueLowRange
      << "\ntheRedHueHighRange:           " << theRedHueHighRange
      << "\ntheRedSaturationOffset:       " << theRedSaturationOffset
      << "\ntheRedIntensityOffset:        " << theRedIntensityOffset
      
      << "\ntheYellowHueOffset:           " << theYellowHueOffset
      << "\ntheYellowHueLowRange:         " << theYellowHueLowRange
      << "\ntheYellowHueHighRange:        " << theYellowHueHighRange
      << "\ntheYellowSaturationOffset:    " << theYellowSaturationOffset
      << "\ntheYellowIntensityOffset:     " << theYellowIntensityOffset
      
      << "\ntheGreenHueOffset:            " << theGreenHueOffset
      << "\ntheGreenHueLowRange:          " << theGreenHueLowRange
      << "\ntheGreenHueHighRange:         " << theGreenHueHighRange
      << "\ntheGreenSaturationOffset:     " << theGreenSaturationOffset
      << "\ntheGreenIntensityOffset:      " << theGreenIntensityOffset
      
      << "\ntheCyanHueOffset:             " << theCyanHueOffset
      << "\ntheCyanHueLowRange:           " << theCyanHueLowRange
      << "\ntheCyanHueHighRange:          " << theCyanHueHighRange
      << "\ntheCyanSaturationOffset:      " << theCyanSaturationOffset
      << "\ntheCyanIntensityOffset:       " << theCyanIntensityOffset
      
      << "\ntheBlueHueOffset:             " << theBlueHueOffset
      << "\ntheBlueHueLowRange:           " << theBlueHueLowRange
      << "\ntheBlueHueHighRange:          " << theBlueHueHighRange
      << "\ntheBlueSaturationOffset:      " << theBlueSaturationOffset
      << "\ntheBlueIntensityOffset:       " << theBlueIntensityOffset
      
      << "\ntheMagentaHueOffset:          " << theMagentaHueOffset
      << "\ntheMagentaHueLowRange:        " << theMagentaHueLowRange
      << "\ntheMagentaHueHighRange:       " << theMagentaHueHighRange
      << "\ntheMagentaSaturationOffset:   " << theMagentaSaturationOffset
      << "\ntheMagentaIntensityOffset:    " << theMagentaIntensityOffset

      << "\ntheWhiteObjectClip:           " << theWhiteObjectClip
      << endl;
   
   return os;
}

ostream& operator<<(ostream& os, const ossimHsiRemapper& hr)
{
   return hr.print(os);
}
