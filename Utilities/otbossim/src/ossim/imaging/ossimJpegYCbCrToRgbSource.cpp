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
// $Id: ossimJpegYCbCrToRgbSource.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <ossim/imaging/ossimJpegYCbCrToRgbSource.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimJpegYCbCrVector.h>
#include <ossim/base/ossimRgbVector.h>

RTTI_DEF1(ossimJpegYCbCrToRgbSource,
          "ossimJpegYCbCrToRgbSource" ,
          ossimImageSourceFilter)

ossimJpegYCbCrToRgbSource::ossimJpegYCbCrToRgbSource()
   :ossimImageSourceFilter(),
    theBlankTile()
{
}

ossimJpegYCbCrToRgbSource::ossimJpegYCbCrToRgbSource(ossimImageSource* inputSource)
   : ossimImageSourceFilter(inputSource),
     theBlankTile()
{
}

ossimJpegYCbCrToRgbSource::~ossimJpegYCbCrToRgbSource()
{
}

void ossimJpegYCbCrToRgbSource::initialize()
{
   ossimImageSourceFilter::initialize();
}

void ossimJpegYCbCrToRgbSource::allocate()
{
   theBlankTile = new ossimImageData(this,
                                     OSSIM_UCHAR,
                                     3);
}

ossimRefPtr<ossimImageData> ossimJpegYCbCrToRgbSource::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if (!theBlankTile.valid())
   {
      allocate(); // first time through.
   }
   
   if(theBlankTile.valid())
   {
      theBlankTile->setOrigin(tileRect.ul());
      theBlankTile->setWidthHeight(tileRect.width(), tileRect.height());
   }
   
   if(theInputConnection)
   {
      ossimRefPtr<ossimImageData> imageData =
         theInputConnection->getTile(tileRect, resLevel);

      if(!imageData.valid())
      {
         return theBlankTile;
      }
      if((isSourceEnabled())&&
         (imageData->getNumberOfBands()==3)&&
         (imageData->getScalarType()==OSSIM_UCHAR)&&
         (imageData->getDataObjectStatus()!=OSSIM_NULL)&&
         (imageData->getDataObjectStatus()!=OSSIM_EMPTY))
      {
         ossim_uint8* bands[3];
         
         bands[0] = static_cast<ossim_uint8*>(imageData->getBuf(0));
         bands[1] = static_cast<ossim_uint8*>(imageData->getBuf(1));
         bands[2] = static_cast<ossim_uint8*>(imageData->getBuf(2));
         
         long height = imageData->getHeight();
         long width  = imageData->getWidth();
         long offset = 0;
         for(long row = 0; row < height; ++row)
         {
            for(long col = 0; col < width; ++col)
            {
               
               ossimJpegYCbCrVector ycbcr(bands[0][offset],
                                          bands[1][offset],
                                          bands[2][offset]);
               ossimRgbVector rgb(ycbcr);
               
               
               bands[0][offset] = rgb.getR();
               bands[1][offset] = rgb.getG();
               bands[2][offset] = rgb.getB();
               
               ++offset;
            }
         }
         imageData->validate();
      }
      return imageData;
   }
   
   return theBlankTile;
}  
