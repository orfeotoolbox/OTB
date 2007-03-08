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
// $Id: ossimRgbToJpegYCbCrSource.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <ossim/imaging/ossimRgbToJpegYCbCrSource.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimJpegYCbCrVector.h>
#include <ossim/base/ossimRgbVector.h>

RTTI_DEF1(ossimRgbToJpegYCbCrSource,
          "ossimRgbToJpegYCbCrSource",
          ossimImageSourceFilter)

ossimRgbToJpegYCbCrSource::ossimRgbToJpegYCbCrSource()
   :ossimImageSourceFilter()
{
   theBlankTile = new ossimImageData(this,
                                     OSSIM_UCHAR,
                                     3);
}

ossimRgbToJpegYCbCrSource::ossimRgbToJpegYCbCrSource(ossimImageSource* inputSource)
   : ossimImageSourceFilter(inputSource)
{
}

ossimRgbToJpegYCbCrSource::~ossimRgbToJpegYCbCrSource()
{
}

ossimRefPtr<ossimImageData> ossimRgbToJpegYCbCrSource::getTile(const  ossimIrect& tileRect,
                                                   ossim_uint32 resLevel)
{
   theBlankTile->setOrigin(tileRect.ul());
   theBlankTile->setWidthHeight(tileRect.width(), tileRect.height());
   
   if(theInputConnection)
   {
      ossimRefPtr<ossimImageData> imageData = theInputConnection->getTile(tileRect,
                                                              resLevel);
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
               ossimRgbVector rgb(bands[0][offset],
                                  bands[1][offset],
                                  bands[2][offset]);
               
               ossimJpegYCbCrVector ycbcr(rgb);
               
               
               bands[0][offset] = ycbcr.getY();
               bands[1][offset] = ycbcr.getCb();
               bands[2][offset] = ycbcr.getCr();
               ++offset;
            }
         }
         imageData->validate();
      }
      return imageData;
   }
   
   return theBlankTile;
}  
