//*******************************************************************
// Copyright (C) 2010 Radiant Blue Technologies Inc. 
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: William Watkins
// 
// Description: A brief description of the contents of the file.
//
//*************************************************************************

#include <ossim/imaging/ossimSICDToDetectedImage.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimTrace.h>

RTTI_DEF1(ossimSICDToDetectedImage,
          "ossimSICDToDetectedImage",
          ossimImageSourceFilter)

ossimSICDToDetectedImage::ossimSICDToDetectedImage()
:ossimImageSourceFilter()
{  
}

ossimSICDToDetectedImage::~ossimSICDToDetectedImage()
{
}

ossimRefPtr<ossimImageData> ossimSICDToDetectedImage::getTile(
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
         (tile->getDataObjectStatus()==OSSIM_EMPTY))
      {
         return tile;
      }
      
      if(!theTile.valid()) 
      {
         allocate();
      }
      
      if(!theTile.valid())
      {
         return tile;
      }
      
      theTile->setImageRectangle(tileRect);
      
      if (tile->getNumberOfBands() == 2)
      {
         switch(tile->getScalarType())
         {
            case OSSIM_UINT8:				// SICD 8 Bit data is Amplitude and Phase
            {
               processAmplitudeAngleTile((ossim_uint8)0,
                                         tile.get());
               break;
            }
            case OSSIM_SINT8:
            {
               processAmplitudeAngleTile((ossim_sint8)0,
                                         tile.get());
               break;
            }
            case OSSIM_UINT16:			// SICD 16 Bit data is complex
            {
               processComplexTile((ossim_uint16)0,
                                  tile.get());
               break;
            }
            case OSSIM_SINT16:
            {
               processComplexTile((ossim_sint16)0,
                                  tile.get());
               break;
            }			  
            case OSSIM_FLOAT32:
            {
               processComplexTile((ossim_float32)0,
                                  tile.get());
               break;
            }
            default:
            {
               break;
            }
         }
         theTile->validate();
      }
      
      return theTile;
   }
   
   return theTile;
}

template<class T>
void ossimSICDToDetectedImage::processComplexTile(T /* dummy */, ossimImageData* tile)
{
	T* bands[2];		
	T* result = (T*)theTile->getBuf();		
   
   bands[0] = (T*)tile->getBuf(0);
   bands[1] = (T*)tile->getBuf(1);
   
   ossim_uint32 offset = 0;
   ossim_uint32 maxIdx = tile->getWidth()*tile->getHeight();
   
   for(offset = 0; offset < maxIdx; ++offset)
   {
      result[offset] = sqrt((ossim_float64)bands[0][offset]*(ossim_float64)bands[0][offset] + (ossim_float64)bands[1][offset]*(ossim_float64)bands[1][offset]);
   }
   
}

template<class T>
void ossimSICDToDetectedImage::processAmplitudeAngleTile(T /* dummy */, ossimImageData* tile)
{
	T* bands[2];		
	T* result = (T*)theTile->getBuf();		
	
	bands[0] = (T*)tile->getBuf(0);
	bands[1] = (T*)tile->getBuf(1);
	
	ossim_uint32 offset = 0;
	ossim_uint32 maxIdx = tile->getWidth()*tile->getHeight();
	
   
	for(offset = 0; offset < maxIdx; ++offset)
	{
		result[offset] = bands[0][offset];  // The SICD spec says 8 bit data is Mag 8 bits and Phase 8 bits (Not complex)
	}
}


void ossimSICDToDetectedImage::initialize()
{
   ossimImageSourceFilter::initialize();
   theTile     = 0;
}

void ossimSICDToDetectedImage::allocate()
{
   if( isSourceEnabled() && theInputConnection )
   {
      theTile = ossimImageDataFactory::instance()->create(this, this);
      if(theTile.valid())
      {
      }
	   if(theTile.valid())
      {
         theTile->initialize();
      }
      else
      {
         theTile     = 0;
      }
   }
}

void ossimSICDToDetectedImage::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property)
   {
      return;
   }
   else
   {
      ossimImageSourceFilter::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimSICDToDetectedImage::getProperty(const ossimString& name)const
{
   return ossimImageSourceFilter::getProperty(name);
}

void ossimSICDToDetectedImage::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSourceFilter::getPropertyNames(propertyNames);
}





