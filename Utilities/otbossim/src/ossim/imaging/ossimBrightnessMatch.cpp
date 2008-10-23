//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: A brief description of the contents of the file.
//
//*************************************************************************
// $Id: ossimBrightnessMatch.cpp 11955 2007-10-31 16:10:22Z gpotts $

#include <ossim/imaging/ossimBrightnessMatch.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNormRgbVector.h>
#include <ossim/base/ossimHsiVector.h>
#include <ossim/base/ossimContainerProperty.h>
#include <ossim/base/ossimNumericProperty.h>

RTTI_DEF1(ossimBrightnessMatch,
          "ossimBrightnessMatch",
          ossimImageSourceFilter)
   
ossimBrightnessMatch::ossimBrightnessMatch()
   :ossimImageSourceFilter(),
    theTargetBrightness(0.5),
    theInputBrightness(0.5)
{
   theInputBrightness = ossim::nan();
   theBrightnessContrastSource = new ossimBrightnessContrastSource;
}


ossimBrightnessMatch::~ossimBrightnessMatch()
{
}

ossimRefPtr<ossimImageData> ossimBrightnessMatch::getTile(
   const ossimIrect& tileRect, ossim_uint32 resLevel)
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getTile(tileRect, resLevel);
   }
   if(theInputConnection)
   {
      if(ossim::isnan(theInputBrightness))
      {
         computeInputBrightness();
      }
      return theBrightnessContrastSource->getTile(tileRect, resLevel);
   }
   return 0;
}

void ossimBrightnessMatch::initialize()
{
   theBrightnessContrastSource->connectMyInputTo(0, getInput());
   theNormTile = 0;
}

void ossimBrightnessMatch::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property)
   {
      return;
   }
   ossimString name = property->getName();
   if(name == "input_brightness")
   {
      theInputBrightness = property->valueToString().toDouble();
      theBrightnessContrastSource->setBrightness(theTargetBrightness-theInputBrightness);
   }
   else if(name == "target_brightness")
   {
      theTargetBrightness = property->valueToString().toDouble();
      theBrightnessContrastSource->setBrightness(theTargetBrightness-theInputBrightness);
   }
   else
   {
      ossimImageSourceFilter::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimBrightnessMatch::getProperty(const ossimString& name)const
{
   if(name == "target_brightness")
   {
      ossimNumericProperty* numeric = new ossimNumericProperty(name,
                                                               ossimString::toString(theTargetBrightness),
                                                               0.0, 1.0);
      numeric->setNumericType(ossimNumericProperty::ossimNumericPropertyType_FLOAT64);
      numeric->setCacheRefreshBit();
      return numeric;
   }
   else if(name == "input_brightness")
   {
      ossimNumericProperty* numeric = new ossimNumericProperty(name,
                                                               ossimString::toString(theInputBrightness),
                                                               0.0, 1.0);
      numeric->setNumericType(ossimNumericProperty::ossimNumericPropertyType_FLOAT64);
      numeric->setCacheRefreshBit();
      return numeric;
   }

    return ossimImageSourceFilter::getProperty(name);
}

void ossimBrightnessMatch::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSourceFilter::getPropertyNames(propertyNames);
   
   propertyNames.push_back("input_brightness");
   propertyNames.push_back("target_brightness");
}


bool ossimBrightnessMatch::loadState(const ossimKeywordlist& kwl,
                                              const char* prefix)
{
   const char* input_brightness = kwl.find(prefix, "input_brightness");
   const char* target_brightness = kwl.find(prefix, "target_brightness");

   if(input_brightness)
   {
      theInputBrightness = ossimString(input_brightness).toDouble();
   }
   if(target_brightness)
   {
      theTargetBrightness = ossimString(target_brightness).toDouble();
   }
   
   return ossimImageSourceFilter::loadState(kwl, prefix);
}

bool ossimBrightnessMatch::saveState(ossimKeywordlist& kwl,
                                              const char* prefix)const
{
   kwl.add(prefix,
           "input_brightness",
           theInputBrightness,
           true);
   kwl.add(prefix,
           "target_brightness",
           theTargetBrightness,
            true);

   return ossimImageSourceFilter::saveState(kwl, prefix);   
}


void ossimBrightnessMatch::computeInputBrightness()
{
   if(theInputConnection)
   {
      ossimIrect inputRect = getBoundingRect();
      ossim_uint32 rlevel = 0;
      ossim_uint32 nlevels = getNumberOfDecimationLevels();
      if(nlevels>1)
      {
         while((ossim::max(inputRect.width(), inputRect.height()) > 2048)&&
               (rlevel < nlevels))
         {
            ++rlevel;
            inputRect = getBoundingRect(rlevel);
         }
      }
      ossimIpt centerPt = inputRect.midPoint();
      centerPt.x -= 1024;
      centerPt.y -= 1024;
      ossimIrect reqRect(centerPt.x,
                         centerPt.y,
                         centerPt.x + 2047,
                         centerPt.x + 2047);
      reqRect = reqRect.clipToRect(inputRect);
      ossimRefPtr<ossimImageData> inputTile = theInputConnection->getTile(reqRect, rlevel);

      if(inputTile.valid())
      {
         theNormTile = new ossimImageData(0,
                                          OSSIM_FLOAT32,
                                          inputTile->getNumberOfBands());
         theNormTile->initialize();
         theNormTile->setImageRectangle(reqRect);
         inputTile->copyTileToNormalizedBuffer((ossim_float32*)theNormTile->getBuf());
         theNormTile->setDataObjectStatus(inputTile->getDataObjectStatus());
         ossim_uint32 maxIdx = theNormTile->getWidth()*theNormTile->getHeight();
         ossim_float32* bands[3];
         double averageI = 0.0;
         double count = 0.0;
         ossim_uint32 offset = 0;
         
         bands[0] = (ossim_float32*)theNormTile->getBuf();
         if(theNormTile->getNumberOfBands()>2)
         {
            bands[1] = (ossim_float32*)theNormTile->getBuf(1);
            bands[2] = (ossim_float32*)theNormTile->getBuf(2);
         }
         else
         {
            bands[1] = bands[0];
            bands[2] = bands[0];
         }
         ossimHsiVector hsi;
         if(theNormTile->getDataObjectStatus() == OSSIM_FULL)
         {
            count = maxIdx;
            
            for(offset = 0; offset < maxIdx; ++offset)
            {
               hsi = ossimNormRgbVector(bands[0][offset], bands[1][offset], bands[2][offset]);
               
               averageI += hsi.getI();
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
                  hsi = ossimNormRgbVector(bands[0][offset], bands[1][offset], bands[2][offset]);
                  averageI += hsi.getI();
                  
                  ++count;
               }
            }
         }
         theInputBrightness = averageI / count;
         theBrightnessContrastSource->setBrightness(theTargetBrightness-theInputBrightness);
      }
      else
      {
         theInputBrightness = .5;
      }
   }
   theNormTile = 0;
}
