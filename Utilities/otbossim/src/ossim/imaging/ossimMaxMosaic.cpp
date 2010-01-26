//*******************************************************************
// Copyright (C) 2005 SANZ Inc. 
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Kenneth Melero (kmelero@sanz.com)
//
// Description: This combiner is designed to "float" the maximum pixel value
//              of all inputs to top of the mosaic output. 
//
//*************************************************************************
// $Id: ossimMaxMosaic.cpp 15766 2009-10-20 12:37:09Z gpotts $

#include <ossim/imaging/ossimMaxMosaic.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimTrace.h>

static const ossimTrace traceDebug("ossimMaxMosaic:debug");

using namespace std;

RTTI_DEF1(ossimMaxMosaic, "ossimMaxMosaic", ossimImageCombiner)
ossimMaxMosaic::ossimMaxMosaic()
   :ossimImageCombiner(),
    theTile(NULL)
{
}

ossimMaxMosaic::ossimMaxMosaic(ossimConnectableObject::ConnectableObjectList& inputSources)
    : ossimImageCombiner(inputSources),
      theTile(NULL)
{
}


ossimMaxMosaic::~ossimMaxMosaic()
{
}

ossimRefPtr<ossimImageData> ossimMaxMosaic::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   long size = getNumberOfInputs();
   ossim_uint32 layerIdx = 0;
   // If there is only one in the mosaic then just return it.
   if(size == 1)
   {
      return getNextTile(layerIdx, 0, tileRect, resLevel);
   }
   
   ossimIpt origin = tileRect.ul();
   ossim_uint32 w = tileRect.width();
   ossim_uint32 h = tileRect.height();
   
   if(!theTile.valid())
   {
      // First time through...
      allocate();

      // If we still don't have a buffer then we will leave.
      if(!theTile.valid())
      {
         return ossimRefPtr<ossimImageData>();
      }
   }
   
   ossim_uint32 tileW = theTile->getWidth();
   ossim_uint32 tileH = theTile->getHeight();
   if((w != tileW)||
      (h != tileH))
   {
      theTile->setWidth(w);
      theTile->setHeight(h);
      if((w*h)!=(tileW*tileH))
      {
         theTile->initialize();
      }
   }
   theTile->setOrigin(origin);

   //---
   // General Note:
   //
   // Note: I will not check for disabled or enabled since we have
   // no clear way to handle this within a mosaic. The default will be
   // to do a simple a A over B type mosaic.  Derived classes should
   // check for the enabled and disabled and always
   // use this default implementation if they are disabled.
   //---
   theTile->setOrigin(origin);
   theTile->makeBlank();
   switch(theTile->getScalarType())
   {
      case OSSIM_UCHAR:
      {
         if(!hasDifferentInputs())
         {
            return combine(static_cast<ossim_uint8>(0),
                           tileRect,
                           resLevel);
         }
         else
         {
            return combineNorm(static_cast<ossim_uint8>(0),
                               tileRect,
                               resLevel);
         }
      }
      case OSSIM_SINT8:
      {
         if(!hasDifferentInputs())
         {
            return combine(static_cast<ossim_sint8>(0),
                           tileRect,
                           resLevel);
         }
         else
         {
            return combineNorm(static_cast<ossim_sint8>(0),
                               tileRect,
                               resLevel);
         }
      }
      case OSSIM_FLOAT: 
      case OSSIM_NORMALIZED_FLOAT:
      {
         if(!hasDifferentInputs())
         {
            return combine(static_cast<float>(0),
                           tileRect,
                           resLevel);
         }
         else
         {
            return combineNorm(static_cast<float>(0),
                               tileRect,
                               resLevel);
         }
      }
      case OSSIM_USHORT16:
      case OSSIM_USHORT11:
      {
         if(!hasDifferentInputs())
         {
            return combine(static_cast<ossim_uint16>(0),
                           tileRect,
                           resLevel);
         }
         else
         {
            return combineNorm(static_cast<ossim_uint16>(0),
                               tileRect,
                               resLevel);
         }
      }
      case OSSIM_SSHORT16:
      {
         if(!hasDifferentInputs())
         {
            return combine(static_cast<ossim_sint16>(0),
                           tileRect,
                           resLevel);
         }
         else
         {
            return combineNorm(static_cast<ossim_sint16>(0),
                               tileRect,
                               resLevel);
         }
      }
      case OSSIM_SINT32:
      {
         if(!hasDifferentInputs())
         {
            return combine(static_cast<ossim_sint32>(0),
                           tileRect,
                           resLevel);
         }
         else
         {
            return combineNorm(static_cast<ossim_sint32>(0),
                               tileRect,
                               resLevel);
         }
      }
      case OSSIM_UINT32:
      {
         if(!hasDifferentInputs())
         {
            return combine(static_cast<ossim_uint32>(0),
                           tileRect,
                           resLevel);
         }
         else
         {
            return combineNorm(static_cast<ossim_uint32>(0),
                               tileRect,
                               resLevel);
         }
      }
      case OSSIM_DOUBLE:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         if(!hasDifferentInputs())
         {
            return combine(static_cast<double>(0),
                           tileRect,
                           resLevel);
         }
         else
         {
            return combineNorm(static_cast<double>(0),
                               tileRect,
                               resLevel);
         }
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Scalar type = " << theTile->getScalarType()
            << " Not supported by ossimMaxMosaic" << endl;
      }
   }

   return ossimRefPtr<ossimImageData>();
}

void ossimMaxMosaic::initialize()
{
  ossimImageCombiner::initialize();
  theTile = NULL;
}

void ossimMaxMosaic::allocate()
{
   theTile = NULL;
   
   if( (getNumberOfInputs() > 0) && getInput(0) )
   {
      theTile = ossimImageDataFactory::instance()->create(this, this);
      theTile->initialize();
   }
}

bool ossimMaxMosaic::saveState(ossimKeywordlist& kwl,
                                 const char* prefix)const
{
   return ossimImageCombiner::saveState(kwl, prefix);
}

bool ossimMaxMosaic::loadState(const ossimKeywordlist& kwl,
                                 const char* prefix)
{
   return ossimImageCombiner::loadState(kwl, prefix);
}

template <class T> ossimRefPtr<ossimImageData> ossimMaxMosaic::combineNorm(
   T,// dummy template variable 
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   ossim_uint32 layerIdx = 0;
   ossimRefPtr<ossimImageData> destination = theTile;
   
   ossimRefPtr<ossimImageData> currentImageData =
      getNextNormTile(layerIdx, 0, tileRect, resLevel);
      
   if(!currentImageData)
   {
      return currentImageData;
   }
   
   float** srcBands        = new float*[theLargestNumberOfInputBands];
   float* srcBandsNullPix  = new float[theLargestNumberOfInputBands];
   T** destBands = new T*[theLargestNumberOfInputBands];
   T* destBandsNullPix = new T[theLargestNumberOfInputBands];
   T* destBandsMinPix = new T[theLargestNumberOfInputBands];
   T* destBandsMaxPix = new T[theLargestNumberOfInputBands];
      
   ossim_uint32 band;
   ossim_uint32 upperBound = destination->getWidth()*destination->getHeight();
   ossim_uint32 minNumberOfBands = currentImageData->getNumberOfBands();
   for(band = 0; band < minNumberOfBands; ++band)
   {
      srcBands[band]  = static_cast<float*>(currentImageData->getBuf(band));
      srcBandsNullPix[band]  = static_cast<float>(currentImageData->getNullPix(band));
      
      destBands[band] = static_cast<T*>(theTile->getBuf(band));
      destBandsNullPix[band] = static_cast<T>(theTile->getNullPix(band));
      destBandsMinPix[band] = static_cast<T>(theTile->getMinPix(band));
      destBandsMaxPix[band] = static_cast<T>(theTile->getMaxPix(band));
   }
   // if the src is smaller than the destination in number
   // of bands we will just duplicate the last band.
   for(;band < theLargestNumberOfInputBands; ++band)
   {
      srcBands[band]  = static_cast<float*>(srcBands[minNumberOfBands - 1]);
      srcBandsNullPix[band] = static_cast<float>(currentImageData->getNullPix(minNumberOfBands - 1));
      
      destBands[band] = static_cast<T*>(theTile->getBuf(band));
      destBandsNullPix[band] = static_cast<T>(theTile->getNullPix(band));
      destBandsMinPix[band] = static_cast<T>(theTile->getMinPix(band));
      destBandsMaxPix[band] = static_cast<T>(theTile->getMaxPix(band));
   }
   // most of the time we will not overlap so let's
   // copy the first tile into destination and check later.
   //
   ossim_uint32 tempBandIdx = 0;
   for(band = 0; band < theTile->getNumberOfBands();++band)
   {
      if(band < currentImageData->getNumberOfBands())
      {
         theTile->copyNormalizedBufferToTile(band,
                                             (float*)currentImageData->getBuf(band));
         ++tempBandIdx;
      }
      else
      {
         if(tempBandIdx)
         {
            theTile->copyNormalizedBufferToTile(band,
                                                (float*)currentImageData->getBuf(tempBandIdx-1));
         }
      }
   }
   destination->validate();
   
   currentImageData = getNextNormTile(layerIdx, tileRect, resLevel);

   while(currentImageData.valid())
   {  
      ossim_uint32 minNumberOfBands           = currentImageData->getNumberOfBands();
      ossimDataObjectStatus currentStatus     = currentImageData->getDataObjectStatus();
      ossimDataObjectStatus destinationStatus = destination->getDataObjectStatus();
      
      if(destinationStatus == OSSIM_FULL)
      {
         return destination;
      }
      for(band = 0; band < minNumberOfBands; ++band)
      {
         srcBands[band]        = static_cast<float*>(currentImageData->getBuf(band));
         srcBandsNullPix[band] = static_cast<float>(currentImageData->getNullPix(band));
      }
      // if the src is smaller than the destination in number
      // of bands we will just duplicate the last band.
      for(;band < theLargestNumberOfInputBands; ++band)
      {
         srcBands[band] = srcBands[minNumberOfBands - 1];
         srcBandsNullPix[band] = static_cast<T>(currentImageData->getNullPix(minNumberOfBands - 1));
      }

      if((destinationStatus == OSSIM_EMPTY)&&
         (currentStatus     != OSSIM_EMPTY)&&
         (currentStatus     != OSSIM_NULL))
      {
         ossim_uint32 upperBound = destination->getWidth()*destination->getHeight();
         for(band=0; band < theLargestNumberOfInputBands; ++band)
         {
            float delta = destBandsMaxPix[band] - destBandsMinPix[band];
            float minP  = destBandsMinPix[band];
            for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
            {
               destBands[band][offset] = (T)( minP + delta*srcBands[band][offset]);
            }
         }
      }
      else if((destinationStatus == OSSIM_PARTIAL)&&
              (currentStatus     != OSSIM_EMPTY)&&
              (currentStatus     != OSSIM_NULL))
      {
         for(band = 0; band < theLargestNumberOfInputBands; ++band)
         {
            
            float delta = destBandsMaxPix[band] - destBandsMinPix[band];
            float minP  = destBandsMinPix[band];
            for(ossim_uint32 offset = 0;
                offset < upperBound;
                ++offset)
            {
               
               if(destBands[band][offset] == destBandsNullPix[band])
               {
                  destBands[band][offset] = (T)(minP + delta*srcBands[band][offset]);
               }
            }
         }
      }
      destination->validate();
      
      currentImageData = getNextNormTile(layerIdx, tileRect, resLevel);
   }
   // Cleanup...
   delete [] srcBands;
   delete [] srcBandsNullPix;
   delete [] destBands;
   delete [] destBandsNullPix;
   delete [] destBandsMinPix;
   delete [] destBandsMaxPix;

   return destination;
}

template <class T> ossimRefPtr<ossimImageData> ossimMaxMosaic::combine(
   T,// dummy template variable 
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   ossim_uint32 layerIdx = 0;
   ossimRefPtr<ossimImageData> destination = theTile;

   ossimRefPtr<ossimImageData> currentImageData =
      getNextTile(layerIdx, 0, tileRect, resLevel);
      
   T** srcBands         = new T*[theLargestNumberOfInputBands];
   T*  srcBandsNullPix  = new T[theLargestNumberOfInputBands];
   T** destBands        = new T*[theLargestNumberOfInputBands];
   T*  destBandsNullPix = new T[theLargestNumberOfInputBands];
      
   ossim_uint32 band;
   ossim_uint32 upperBound = destination->getWidth()*destination->getHeight();
   ossim_uint32 bandIndex  = 0;
   if(!currentImageData)
   {
      return currentImageData;
   }
   ossim_uint32 offset=0;
   ossim_uint32 minNumberOfBands = currentImageData->getNumberOfBands();
   for(band = 0; band < minNumberOfBands; ++band)
   {
      srcBands[band]  = static_cast<T*>(currentImageData->getBuf(band));
      destBands[band] = static_cast<T*>(theTile->getBuf(band));
      srcBandsNullPix[band]  = static_cast<T>(currentImageData->getNullPix(band));
      destBandsNullPix[band] = static_cast<T>(theTile->getNullPix(band));
   }
   // if the src is smaller than the destination in number
   // of bands we will just duplicate the last band.
   for(;band < theLargestNumberOfInputBands; ++band)
   {
      srcBands[band]  = static_cast<T*>(srcBands[minNumberOfBands - 1]);
      destBands[band] = static_cast<T*>(theTile->getBuf(band));
      srcBandsNullPix[band] = static_cast<T>(currentImageData->getNullPix(minNumberOfBands - 1));
      destBandsNullPix[band] = static_cast<T>(theTile->getNullPix(band));
   }
   // most of the time we will not overlap so let's
   // copy the first tile into destination and check later.
   //
   for(band = 0; band < theTile->getNumberOfBands();++band)
   {
      T* destBand = destBands[band];
      T* srcBand  = srcBands[band];
      if(destBand&&srcBand)
      {
         for(offset = 0; offset < upperBound;++offset)
         {
	   *destBand = *srcBand;
	   ++srcBand; ++destBand;
         }
      }
   }
   destination->setDataObjectStatus(currentImageData->getDataObjectStatus());

   currentImageData = getNextTile(layerIdx,
                                  tileRect,
                                  resLevel);

   while(currentImageData.valid())
   {  
      ossim_uint32 minNumberOfBands           = currentImageData->getNumberOfBands();
      ossimDataObjectStatus currentStatus     = currentImageData->getDataObjectStatus();
      ossimDataObjectStatus destinationStatus = destination->getDataObjectStatus();
      
      for(band = 0; band < minNumberOfBands; ++band)
      {
         srcBands[band]        = static_cast<T*>(currentImageData->getBuf(band));
         srcBandsNullPix[band] = static_cast<T>(currentImageData->getNullPix(band));
      }
      // if the src is smaller than the destination in number
      // of bands we will just duplicate the last band.
      for(;band < theLargestNumberOfInputBands; ++band)
      {
         srcBands[band] = srcBands[minNumberOfBands - 1];
         srcBandsNullPix[band] = static_cast<T>(currentImageData->getNullPix(minNumberOfBands - 1));
      }


      if((destinationStatus == OSSIM_PARTIAL)&&
	 (currentStatus     != OSSIM_EMPTY)&&
	 (currentStatus     != OSSIM_NULL))
	{
	  for(bandIndex = 0; bandIndex < theLargestNumberOfInputBands; ++bandIndex)
	    {
	      
	      for(ossim_uint32 offset = 0;
		  offset < upperBound;
		  ++offset)
		{
		  if(srcBands[bandIndex][offset] > destBands[bandIndex][offset])
		    {
		      destBands[bandIndex][offset] = srcBands[bandIndex][offset];
		    }
		}
	    }
	}
      else
	{
	  ossim_uint32 upperBound = destination->getWidth()*destination->getHeight();
	  for(ossim_uint32 band=0; band < theLargestNumberOfInputBands; ++band)
	    {
	      for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
		{
		  if(srcBands[band][offset] > destBands[band][offset])
		    {
		      destBands[band][offset] = srcBands[band][offset];
		    }
		}
	    }
	}
      
      destination->validate();
      
      currentImageData = getNextTile(layerIdx,tileRect, resLevel);
   }
   // Cleanup...
   delete [] srcBands;
   delete [] srcBandsNullPix;
   delete [] destBands;
   delete [] destBandsNullPix;
   return destination;
}

ossimString ossimMaxMosaic::getShortName()const
{
   return ossimString("ossimMaxMosaic");
}

ossimString ossimMaxMosaic::getLongName()const
{
   return ossimString("Max Mosaic");
}

ossimString ossimMaxMosaic::getDescription()const
{
   return ossimString("Combiner which puts maximum dn value on image.");
}
