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
// $Id: ossimHsiToRgbSource.cpp 17206 2010-04-25 23:20:40Z dburken $
#include <ossim/imaging/ossimHsiToRgbSource.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimHsiVector.h>
#include <ossim/base/ossimRgbVector.h>
#include <ossim/imaging/ossimImageDataFactory.h>

RTTI_DEF1(ossimHsiToRgbSource, "ossimHsiToRgbSource", ossimImageSourceFilter)

ossimHsiToRgbSource::ossimHsiToRgbSource()
   :ossimImageSourceFilter(),
    theBlankTile(NULL),
    theTile(NULL)
{
}

ossimHsiToRgbSource::ossimHsiToRgbSource(ossimImageSource* inputSource)
   : ossimImageSourceFilter(inputSource),
    theBlankTile(NULL),
    theTile(NULL)
{
}

ossimHsiToRgbSource::~ossimHsiToRgbSource()
{
}

ossimRefPtr<ossimImageData> ossimHsiToRgbSource::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      return NULL;  // This filter requires an input.
   }
   
   ossimRefPtr<ossimImageData> inputTile =
      theInputConnection->getTile(tileRect, resLevel);   
   if(!isSourceEnabled())
   {
      return inputTile;
   }
   
   if(!theTile.valid())
   {
      allocate(); // First time through...
   }
   
   if( !inputTile.valid() ||
       inputTile->getDataObjectStatus() == OSSIM_NULL ||
       inputTile->getDataObjectStatus() == OSSIM_EMPTY )
   {
      theBlankTile->setImageRectangle(tileRect);
      return theBlankTile;
   }

   if((inputTile->getNumberOfBands()==3)&&
      (inputTile->getScalarType()==OSSIM_NORMALIZED_FLOAT)&&
      (inputTile->getDataObjectStatus()!=OSSIM_NULL))
   {
      // Set the origin, resize if needed of the output tile.
      theTile->setImageRectangle(tileRect);
      
      ossim_uint8* outputBands[3];
      float* inputBands[3];
      outputBands[0] = static_cast<ossim_uint8*>(theTile->getBuf(0));
      outputBands[1] = static_cast<ossim_uint8*>(theTile->getBuf(1));
      outputBands[2] = static_cast<ossim_uint8*>(theTile->getBuf(2));
      inputBands[0] = static_cast<float*>(inputTile->getBuf(0));
      inputBands[1] = static_cast<float*>(inputTile->getBuf(1));
      inputBands[2] = static_cast<float*>(inputTile->getBuf(2));
      
      long height = inputTile->getHeight();
      long width  = inputTile->getWidth();
      long offset = 0;
      for(long row = 0; row < height; ++row)
      {
         for(long col = 0; col < width; ++col)
         {
            ossimHsiVector hsi(inputBands[0][offset],
                               inputBands[1][offset],
                               inputBands[2][offset]);
            
            ossimRgbVector rgb(hsi);
            
            
            outputBands[0][offset] = rgb.getR();
            outputBands[1][offset] = rgb.getG();
            outputBands[2][offset] = rgb.getB();
            
            ++offset;
         }
      }
   }
   else
   {
      return inputTile;
   }

   theTile->validate();

   return theTile;
}

void ossimHsiToRgbSource::initialize()
{
   ossimImageSourceFilter::initialize();
}

void ossimHsiToRgbSource::allocate()
{
   theBlankTile = ossimImageDataFactory::instance()->create(this, this);
   theTile = (ossimImageData*)theBlankTile->dup();
   theTile->initialize();
}

ossimScalarType ossimHsiToRgbSource::getOutputScalarType() const
{
   return OSSIM_UCHAR;
}

double ossimHsiToRgbSource::getNullPixelValue()const
{
   return 0.0;
}

double ossimHsiToRgbSource::getMinPixelValue(ossim_uint32 /* band */)const
{
   return OSSIM_DEFAULT_MIN_PIX_UCHAR;
}

double ossimHsiToRgbSource::getMaxPixelValue(ossim_uint32 /* band */)const
{
   return OSSIM_DEFAULT_MAX_PIX_UCHAR;
}
