//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
// 
// Description: A brief description of the contents of the file.
//
//*************************************************************************
// $Id: ossimBrightnessContrastSource.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <ossim/imaging/ossimBrightnessContrastSource.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimHsiVector.h>
#include <ossim/base/ossimNormRgbVector.h>
#include <ossim/base/ossimContainerProperty.h>
#include <ossim/base/ossimNumericProperty.h>

RTTI_DEF1(ossimBrightnessContrastSource,
          "ossimBrightnessContrastSource",
          ossimImageSourceFilter)
   
ossimBrightnessContrastSource::ossimBrightnessContrastSource()
   :ossimImageSourceFilter(),
    theBrightness(0.0),
    theContrast(1.0)
{  
}


ossimBrightnessContrastSource::~ossimBrightnessContrastSource()
{
}

ossimRefPtr<ossimImageData> ossimBrightnessContrastSource::getTile(
   const ossimIrect& tileRect, ossim_uint32 resLevel)
{
   ossimRefPtr<ossimImageData> tile = NULL;

   if(theInputConnection)
   {
      tile = theInputConnection->getTile(tileRect, resLevel);
      
      if(!tile.valid())
      {
         return tile;
      }

      if(!isSourceEnabled() ||
         (tile->getDataObjectStatus()==OSSIM_NULL)||
         (tile->getDataObjectStatus()==OSSIM_EMPTY)||
         ( (theBrightness == 0.0) && (theContrast   == 1.0) ) )
      {
         return tile;
      }

      if(!theTile.valid() || !theNormTile.valid())
      {
         allocate();
      }
      
      if(!theTile.valid() || !theNormTile.valid())
      {
         return tile;
      }

      theTile->setImageRectangle(tileRect);
      theNormTile->setImageRectangle(tileRect);

      tile->copyTileToNormalizedBuffer((ossim_float32*)theNormTile->getBuf());
      theNormTile->setDataObjectStatus(tile->getDataObjectStatus());

      if (theNormTile->getNumberOfBands() == 3)
      {
         processRgbTile();
      }
      else
      {
         processNBandTile();
      }
      
      theTile->copyNormalizedBufferToTile((ossim_float32*)
                                          theNormTile->getBuf());
      theTile->validate();
      return theTile;
   }
   
   return tile;
}

void ossimBrightnessContrastSource::processRgbTile()
{
   ossim_float32* bands[3];

   bands[0] = (ossim_float32*)theNormTile->getBuf(0);
   bands[1] = (ossim_float32*)theNormTile->getBuf(1);
   bands[2] = (ossim_float32*)theNormTile->getBuf(2);

   ossim_uint32 offset = 0;
   ossim_uint32 maxIdx = theNormTile->getWidth()*theNormTile->getHeight();
   ossimHsiVector hsi;
   ossim_float32 i;
   if(theNormTile->getDataObjectStatus() == OSSIM_FULL)
   {
      for(offset = 0; offset < maxIdx; ++offset)
      {
         ossimNormRgbVector rgb(bands[0][offset], bands[1][offset], bands[2][offset]);
         hsi = rgb;
         i = (hsi.getI()*theContrast + theBrightness);
         if(i < OSSIM_DEFAULT_MIN_PIX_NORM_FLOAT) i = OSSIM_DEFAULT_MIN_PIX_NORM_FLOAT;
         if(i > 1.0) i = 1.0;
         hsi.setI(i);
         rgb = hsi;
         bands[0][offset] = rgb.getR();
         bands[1][offset] = rgb.getG();
         bands[2][offset] = rgb.getB();
      }
   }
   else
   {
      for(offset = 0; offset < maxIdx; ++offset)
      {
         if((bands[0][offset] != 0.0)&&
            (bands[1][offset] != 0.0)&&
            (bands[2][offset] != 0.0))
         {
            ossimNormRgbVector rgb(bands[0][offset], bands[1][offset], bands[2][offset]);
            hsi = rgb;
            i = (hsi.getI()*theContrast + theBrightness);
            if(i < OSSIM_DEFAULT_MIN_PIX_NORM_FLOAT) i = OSSIM_DEFAULT_MIN_PIX_NORM_FLOAT;
            if(i > 1.0) i = 1.0;
            hsi.setI(i);
            rgb = hsi;
            bands[0][offset] = rgb.getR();
            bands[1][offset] = rgb.getG();
            bands[2][offset] = rgb.getB();
         }
      }
   }
}

void ossimBrightnessContrastSource::processNBandTile()
{
   const ossim_uint32  BANDS = theNormTile->getNumberOfBands();
   const ossim_uint32  PPB   = theNormTile->getSizePerBand();
   const ossim_float32 MP    = theNormTile->getMinNormalizedPix();
   
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      ossim_float32* buf = theNormTile->getFloatBuf(band);

      for (ossim_uint32 i = 0; i < PPB; ++i)
      {
         if (buf[i] != 0.0)
         {
            ossim_float32 p = buf[i] * theContrast + theBrightness;
            buf[i] = (p < 1.0) ? ( (p > MP) ? p : 0.0 ) : 1.0;
         }
      }
   }
}

void ossimBrightnessContrastSource::initialize()
{
   ossimImageSourceFilter::initialize();

   theTile     = 0;
   theNormTile = 0;
}

void ossimBrightnessContrastSource::allocate()
{
   if( isSourceEnabled() && theInputConnection )
   {
      theTile = ossimImageDataFactory::instance()->create(this, this);
      if(theTile.valid())
      {
         theNormTile = ossimImageDataFactory::instance()->create(this,
                                                                 OSSIM_FLOAT32,
                                                                 theTile->getNumberOfBands());
      }
      if(theTile.valid() && theNormTile.valid())
      {
         theTile->initialize();
         theNormTile->initialize();
      }
      else
      {
         theTile     = 0;
         theNormTile = 0;
      }
   }
}

void ossimBrightnessContrastSource::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property)
   {
      return;
   }
   ossimString name = property->getName();
   if(name == "brightness")
   {
      theBrightness = property->valueToString().toDouble();
   }
   else if(name == "contrast")
   {
      theContrast = property->valueToString().toDouble();
   }
   else
   {
      ossimImageSourceFilter::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimBrightnessContrastSource::getProperty(const ossimString& name)const
{
   if(name == "brightness")
   {
      ossimNumericProperty* numeric = new ossimNumericProperty(name,
                                                               ossimString::toString(getBrightness()),
                                                               -1.0, 1.0);
      numeric->setNumericType(ossimNumericProperty::ossimNumericPropertyType_FLOAT64);
      numeric->setCacheRefreshBit();
      return numeric;
   }
   else if(name == "contrast")
   {
      ossimNumericProperty* numeric = new ossimNumericProperty(name,
                                                               ossimString::toString(getContrast()),
                                                               0.0, 20.0);
      numeric->setNumericType(ossimNumericProperty::ossimNumericPropertyType_FLOAT64);
      numeric->setCacheRefreshBit();
      return numeric;
   }

    return ossimImageSourceFilter::getProperty(name);
}

void ossimBrightnessContrastSource::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSourceFilter::getPropertyNames(propertyNames);
   
   propertyNames.push_back("brightness");
   propertyNames.push_back("contrast");
}


bool ossimBrightnessContrastSource::loadState(const ossimKeywordlist& kwl,
                                              const char* prefix)
{
   const char* brightness = kwl.find(prefix, "brightness");
   const char* contrast = kwl.find(prefix, "contrast");

   if(brightness)
   {
      theBrightness = ossimString(brightness).toDouble();
   }
   if(contrast)
   {
      theContrast   = ossimString(contrast).toDouble();
   }
   
   return ossimImageSourceFilter::loadState(kwl, prefix);
}

bool ossimBrightnessContrastSource::saveState(ossimKeywordlist& kwl,
                                              const char* prefix)const
{
   kwl.add(prefix,
           "brightness",
           theBrightness,
           true);
   kwl.add(prefix,
           "contrast",
           theContrast,
           true);

   return ossimImageSourceFilter::saveState(kwl, prefix);   
}


void ossimBrightnessContrastSource::setBrightnessContrast(
   ossim_float64 brightness, ossim_float64 contrast)
{
   theBrightness = brightness;
   theContrast   = contrast;
}

void ossimBrightnessContrastSource::setBrightness(ossim_float64 brightness)
{
   setBrightnessContrast(brightness, getContrast());
}

void ossimBrightnessContrastSource::setContrast(ossim_float64 contrast)
{
   setBrightnessContrast(getBrightness(), contrast);
}

ossim_float64 ossimBrightnessContrastSource::getBrightness()const
{
   return theBrightness;
}

ossim_float64 ossimBrightnessContrastSource::getContrast()const
{
   return theContrast;
}
