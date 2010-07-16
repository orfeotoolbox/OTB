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
// $Id: ossimHsvToRgbSource.cpp 17195 2010-04-23 17:32:18Z dburken $
#include <ossim/imaging/ossimHsvToRgbSource.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimHsvVector.h>
#include <ossim/base/ossimRgbVector.h>
#include <ossim/imaging/ossimImageDataFactory.h>

RTTI_DEF1(ossimHsvToRgbSource, "ossimHsvToRgbSource", ossimImageSourceFilter)

ossimHsvToRgbSource::ossimHsvToRgbSource()
   :ossimImageSourceFilter(),
    theBlankTile(NULL),
    theTile(NULL)
{
}

ossimHsvToRgbSource::ossimHsvToRgbSource(ossimImageSource* inputSource)
   : ossimImageSourceFilter(inputSource),
     theBlankTile(NULL),
     theTile(NULL)
{
}

ossimHsvToRgbSource::~ossimHsvToRgbSource()
{
}

ossimRefPtr<ossimImageData> ossimHsvToRgbSource::getTile(
   const  ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if(theInputConnection)
   {
      ossimRefPtr<ossimImageData> imageData =
         theInputConnection->getTile(tileRect, resLevel);

      if(!imageData.valid()) return theBlankTile;
      
      if(!isSourceEnabled())
      {
         return imageData;
      }

      if(!theTile.valid()) allocate();
      if(!theTile.valid())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Unable to initialize ossimHsvToRgbSource in getTile"
            << std::endl;
         return ossimRefPtr<ossimImageData>();
      }
      
      long w  = tileRect.width();
      long h  = tileRect.height();
      long tw = theTile->getWidth();
      long th = theTile->getHeight();
      
      theBlankTile->setOrigin(tileRect.ul());
      theBlankTile->setWidthHeight(w, h);
      theTile->setWidthHeight(w, h);
      theTile->setOrigin(tileRect.ul());
      if( (tw*th) != (w*h))
      {
         theTile->initialize();
      }
      else
      {
         theTile->makeBlank();
      }
      
      if((imageData->getNumberOfBands()==3)&&
         (imageData->getScalarType()==OSSIM_NORMALIZED_FLOAT)&&
         (imageData->getDataObjectStatus()!=OSSIM_NULL))
      {
         ossim_uint8* outputBands[3];
         float* inputBands[3];
         outputBands[0] = static_cast<ossim_uint8*>(theTile->getBuf(0));
         outputBands[1] = static_cast<ossim_uint8*>(theTile->getBuf(1));
         outputBands[2] = static_cast<ossim_uint8*>(theTile->getBuf(2));
         inputBands[0] = static_cast<float*>(imageData->getBuf(0));
         inputBands[1] = static_cast<float*>(imageData->getBuf(1));
         inputBands[2] = static_cast<float*>(imageData->getBuf(2));
         
         long height = imageData->getHeight();
         long width  = imageData->getWidth();
         long offset = 0;
         for(long row = 0; row < height; ++row)
         {
            for(long col = 0; col < width; ++col)
            {
               ossimHsvVector hsv(inputBands[0][offset],
                                  inputBands[1][offset],
                                  inputBands[2][offset]);
               
               ossimRgbVector rgb(hsv);

               outputBands[0][offset] = rgb.getR();
               outputBands[1][offset] = rgb.getG();
               outputBands[2][offset] = rgb.getB();

               ++offset;
            }
         }
      }
      else
      {
         return imageData;
      }
      theTile->validate();
      return theTile;
   }
   
   return theBlankTile;
}

void ossimHsvToRgbSource::initialize()
{
   theBlankTile = NULL;
   theTile      = NULL;
}

void ossimHsvToRgbSource::allocate()
{
   theBlankTile = ossimImageDataFactory::instance()->create(this, this);
   theTile = (ossimImageData*)theBlankTile->dup();
   theTile->initialize();
}

ossimScalarType ossimHsvToRgbSource::getOutputScalarType() const
{
   return OSSIM_UINT8;
}

double ossimHsvToRgbSource::getNullPixelValue()const
{
   return 0;
}

double ossimHsvToRgbSource::getMinPixelValue(ossim_uint32 /* band */)const
{
   return OSSIM_DEFAULT_MIN_PIX_UCHAR;
}

double ossimHsvToRgbSource::getMaxPixelValue(ossim_uint32 /* band */)const
{
   return OSSIM_DEFAULT_MAX_PIX_UCHAR;
}  
