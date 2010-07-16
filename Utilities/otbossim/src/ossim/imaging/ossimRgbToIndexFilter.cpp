//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimRgbToIndexFilter.cpp 17195 2010-04-23 17:32:18Z dburken $

#include <ossim/imaging/ossimRgbToIndexFilter.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageDataFactory.h>


RTTI_DEF1(ossimRgbToIndexFilter,
          "ossimRgbToIndexFilter",
          ossimImageSourceFilter);

ossimRgbToIndexFilter::ossimRgbToIndexFilter()
   :ossimImageSourceFilter(),
    theLut(new ossimRgbLutDataObject()),
    theTile(NULL)
{
}

ossimRgbToIndexFilter::ossimRgbToIndexFilter(ossimImageSource* inputSource,
                                             const ossimRgbLutDataObject& rgbLut)
   :ossimImageSourceFilter(inputSource),
    theLut((ossimRgbLutDataObject*)rgbLut.dup()),
    theTile(NULL)
{
}

ossimRgbToIndexFilter::~ossimRgbToIndexFilter()
{
   theLut = 0;
}

void ossimRgbToIndexFilter::initialize()
{
   ossimImageSourceFilter::initialize();

   // Force allocate on next getTile.
   theTile      = NULL;
}

void ossimRgbToIndexFilter::allocate()
{
   theTile      = NULL;
   
   if(isSourceEnabled())
   {
      theTile      = ossimImageDataFactory::instance()->create(this, this);
      theTile->initialize();
   }
}

void ossimRgbToIndexFilter::disableSource()
{
   ossimImageSourceFilter::disableSource();
   theTile = NULL;
}

ossimRefPtr<ossimImageData> ossimRgbToIndexFilter::getTile(
   const ossimIrect& origin,
   ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      return ossimRefPtr<ossimImageData>();
   }

   if(!theTile.valid())
   {
      allocate();
   }
   
   ossimRefPtr<ossimImageData> input = theInputConnection->getTile(origin,
                                                                   resLevel);

   if(!theTile.valid() || !input.valid())
   {
      return input;
   }

   theTile->setImageRectangle(origin);

   if(input->getScalarType() == OSSIM_UCHAR)
   {
      theTile->setDataObjectStatus(OSSIM_FULL);
      return convertInputTile(input);
   }
   else
   {
      theTile->makeBlank();
   }

   theTile->setDataObjectStatus(OSSIM_FULL);
   
   return theTile;
}

ossimRefPtr<ossimImageData> ossimRgbToIndexFilter::convertInputTile(ossimRefPtr<ossimImageData>& tile)
{
   if (!tile)
   {
      return theTile;
   }
   
   const ossim_uint32 BANDS = tile->getNumberOfBands();

   if (!BANDS)
   {
      return theTile;
   }
   
   ossim_uint8* band[3];
   ossim_uint8* outBand = (ossim_uint8*)theTile->getBuf();

   if(BANDS >= 3)
   {
      band[0] = (ossim_uint8*)(tile->getBuf(0));
      band[1] = (ossim_uint8*)(tile->getBuf(1));
      band[2] = (ossim_uint8*)(tile->getBuf(2));
   }
   else
   {
      band[0] = (ossim_uint8*)(tile->getBuf(0));
      band[1] = (ossim_uint8*)(tile->getBuf(0));
      band[2] = (ossim_uint8*)(tile->getBuf(0));
   }

   if(band[0])
   {
      ossim_uint32 upper = tile->getWidth()*tile->getHeight();

      for(ossim_uint32 offset = 0; offset < upper; ++offset)
      {
         *outBand = theLut->findIndex(*band[0], *band[1], *band[2]);
         
         ++outBand;
         ++band[0];
         ++band[1];
         ++band[2];
      }
   }
   
   return theTile;
}

bool ossimRgbToIndexFilter::saveState(ossimKeywordlist& kwl,
                                      const char* prefix)const
{
   ossimString newPrefix = prefix;
   newPrefix = newPrefix + "lut.";

   theLut->saveState(kwl, newPrefix.c_str());

   return ossimImageSourceFilter::saveState(kwl, prefix);
}

bool ossimRgbToIndexFilter::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   ossimString newPrefix = prefix;
   newPrefix = newPrefix + "lut.";

   theLut->loadState(kwl, newPrefix.c_str());
   
   return ossimImageSourceFilter::loadState(kwl, prefix);
}

ossim_uint32 ossimRgbToIndexFilter::getNumberOfOutputBands() const
{
   if(isSourceEnabled())
   {
      return 1;
   }
   
   return ossimImageSourceFilter::getNumberOfOutputBands();
}
   
ossimScalarType ossimRgbToIndexFilter::getOutputScalarType() const
{
   if(isSourceEnabled())
   {
      return OSSIM_UCHAR;
   }
   
   return ossimImageSourceFilter::getOutputScalarType();
}

void ossimRgbToIndexFilter::setLut(ossimRgbLutDataObject& lut)
{
   theLut = (ossimRgbLutDataObject*) lut.dup();
}

double ossimRgbToIndexFilter::getNullPixelValue(ossim_uint32 band)const
{
   if(isSourceEnabled())
   {
      return 0.0;
   }
   
   return ossimImageSourceFilter::getNullPixelValue(band);
}

double ossimRgbToIndexFilter::getMinPixelValue(ossim_uint32 /* band */)const
{
   if(isSourceEnabled())
   {
      return 1.0;
   }
   
   return ossimImageSourceFilter::getMinPixelValue();
}

double ossimRgbToIndexFilter::getMaxPixelValue(ossim_uint32 /* band */)const
{
   if(isSourceEnabled())
   {
      return 255.0;
   }
   
   return ossimImageSourceFilter::getMaxPixelValue();
}
