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
// $Id: ossimImageSharpenFilter.cpp 11347 2007-07-23 13:01:59Z gpotts $
#include <ossim/imaging/ossimImageSharpenFilter.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/imaging/ossimImageDataFactory.h>

RTTI_DEF1(ossimImageSharpenFilter, "ossimImageSharpenFilter", ossimImageSourceFilter);

ossimImageSharpenFilter::ossimImageSharpenFilter(ossimObject* owner)
   :ossimImageSourceFilter(owner),
    theTile(NULL)
{
}

ossimImageSharpenFilter::~ossimImageSharpenFilter()
{
}

ossimRefPtr<ossimImageData> ossimImageSharpenFilter::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      return NULL;
   }

   if(!isSourceEnabled())
   {
      return theInputConnection->getTile(tileRect, resLevel);
   }
   
   // we have a 3x3 matrix so stretch the rect out to cover
   // the required pixels
   //
   ossimIrect newRect(ossimIpt(tileRect.ul().x - 1,
                               tileRect.ul().y - 1),
                      ossimIpt(tileRect.lr().x + 1,
                               tileRect.lr().y + 1));
   
   ossimRefPtr<ossimImageData> data =
      theInputConnection->getTile(newRect, resLevel);

   if ( !data.valid() ||
        ( data->getDataObjectStatus() == OSSIM_NULL ) ||
        ( data->getDataObjectStatus() == OSSIM_EMPTY ) )
   {
      // Caller wants tileRect, not newRect so don't return "data".
      return theInputConnection->getTile(tileRect, resLevel);
   }

   if (!theTile)
   {
      allocate(); // First time through...
   }
   
   long w     = tileRect.width();
   long h     = tileRect.height();
   long tileW = theTile->getWidth();
   long tileH = theTile->getHeight();
   if((w != tileW)||
      (h != tileH))
   {
      theTile->setWidth(w);
      theTile->setHeight(h);
      if((w*h)!=(tileW*tileH))
      {
         theTile->initialize();
      }
      else
      {
         theTile->makeBlank();
      }
   }
   else
   {
      theTile->makeBlank();
   }
   theTile->setOrigin(tileRect.ul());
   
   switch(data->getScalarType())
   {
   case OSSIM_UCHAR:
   {
      if(data->getDataObjectStatus() == OSSIM_FULL)
      {
         sharpenFull(static_cast<ossim_uint8>(0),
                     data,
                     theTile);
      }
      else
      {
         sharpenPartial(static_cast<ossim_uint8>(0),
                        data,
                        theTile);
      }
      break;
   }
   case OSSIM_FLOAT: 
   case OSSIM_NORMALIZED_FLOAT:
   {
      if(data->getDataObjectStatus() == OSSIM_FULL)
      {
         sharpenFull(static_cast<float>(0),
                     data,
                     theTile);
      }
      else
      {
         sharpenPartial(static_cast<float>(0),
                        data,
                        theTile);
      }
      break;
   }
   case OSSIM_USHORT16:
   case OSSIM_USHORT11:
   {
      if(data->getDataObjectStatus() == OSSIM_FULL)
      {
         sharpenFull(static_cast<ossim_uint16>(0),
                     data,
                     theTile);
      }
      else
      {
         sharpenPartial(static_cast<ossim_uint16>(0),
                        data,
                        theTile);
      }
      break;
   }
   case OSSIM_SSHORT16:
   {
      if(data->getDataObjectStatus() == OSSIM_FULL)
      {
         sharpenFull(static_cast<ossim_sint16>(0),
                     data,
                     theTile);
      }
      else
      {
         sharpenPartial(static_cast<ossim_sint16>(0),
                        data,
                        theTile);
      }
      break;
   }
   case OSSIM_DOUBLE:
   case OSSIM_NORMALIZED_DOUBLE:
   {
      if(data->getDataObjectStatus() == OSSIM_FULL)
      {
         sharpenFull(static_cast<double>(0),
                     data,
                     theTile);
      }
      else
      {
         sharpenPartial(static_cast<double>(0),
                        data,
                        theTile);
      }
      break;
   }
   default:
   {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimImageSharpenFilter::getTile WARNING: Scalar type = " << theTile->getScalarType() << " Not supported!" << std::endl;
      break;
   }
   }
   theTile->validate();
   
   return theTile;
}


template<class T> void ossimImageSharpenFilter::sharpenPartial(
   T,
   const ossimRefPtr<ossimImageData>& inputData,
   ossimRefPtr<ossimImageData>& outputData)
{
   double sum = 0.0;
   ossim_int32 inputW        = (ossim_int32)inputData->getWidth();
   ossim_int32 outputW       = (ossim_int32)outputData->getWidth();
   ossim_int32 outputH       = (ossim_int32)outputData->getHeight();
   ossim_int32 numberOfBands = (ossim_int32)inputData->getNumberOfBands();
   ossimIpt outputOrigin = outputData->getOrigin();
   ossimIpt inputOrigin  = inputData->getOrigin();
   
   ossim_int32 startInputOffset = std::abs(outputOrigin.y - inputOrigin.y)*
      inputW + std::abs(outputOrigin.x - inputOrigin.x);
   ossim_int32 ulKernelStart    = -inputW - 1;
   ossim_int32 leftKernelStart  = -1;
   ossim_int32 llKernelStart    = inputW  - 1;
   
   const T* ulKernelStartBuf   = NULL;
   const T* leftKernelStartBuf = NULL;
   const T* llKernelStartBuf   = NULL;
   
   for(ossim_int32 band = 0; band < numberOfBands; ++band)
   {
      const T* inputBuf  = static_cast<const T*>(inputData->getBuf(band)) +
         startInputOffset;
      T* outputBuf = static_cast<T*>(outputData->getBuf(band));
      T maxPix     = static_cast<T>(inputData->getMaxPix(band));
      T minPix     = static_cast<T>(inputData->getMinPix(band));
      T nullPix    = static_cast<T>(inputData->getNullPix(band));
      
      if(inputBuf&&outputBuf)
      {
         for(ossim_int32 row = 0; row < outputW; ++row)
         {
            ossim_int32 rowOffset    = inputW*row;
            ulKernelStartBuf   = inputBuf + (rowOffset + ulKernelStart);
            leftKernelStartBuf = inputBuf + (rowOffset + leftKernelStart);
            llKernelStartBuf   = inputBuf + (rowOffset + llKernelStart);
            for(ossim_int32 col = 0; col < outputH; ++col)
            {
               if((ulKernelStartBuf[0]   != nullPix)&&
                  (ulKernelStartBuf[1]   != nullPix)&&
                  (ulKernelStartBuf[2]   != nullPix)&&
                  (leftKernelStartBuf[0] != nullPix)&&
                  (leftKernelStartBuf[1] != nullPix)&&
                  (leftKernelStartBuf[2] != nullPix)&&
                  (llKernelStartBuf[0]   != nullPix)&&
                  (llKernelStartBuf[1]   != nullPix)&&
                  (llKernelStartBuf[2]   != nullPix))
               {
                  sum = -1.0*(double)ulKernelStartBuf[0]  + -2.0*(double)ulKernelStartBuf[1] + -1.0*(double)ulKernelStartBuf[2] +
                        -2.0*(double)leftKernelStartBuf[0]  + 16.0*(double)leftKernelStartBuf[1] + -2.0*(double)leftKernelStartBuf[2] +
                        -1.0*(double)llKernelStartBuf[0]  + -2.0*(double)llKernelStartBuf[1] + -1.0*(double)llKernelStartBuf[2];
                  sum /= 4.0;
                  
                  if(sum > maxPix)
                  {
                     *outputBuf = maxPix;
                  }
                  else if(sum < minPix || ((T)sum == nullPix))
                  {
                     *outputBuf = minPix;
                  }
                  else
                  {
                     *outputBuf = static_cast<T>(sum);
                  }
               }
               else
               {
                  *outputBuf = nullPix;
               }
               
               ++ulKernelStartBuf;
               ++leftKernelStartBuf;
               ++llKernelStartBuf;
               ++outputBuf;
            }
         }
      }
   }
}

template<class T> void ossimImageSharpenFilter::sharpenFull(
   T,
   const ossimRefPtr<ossimImageData>& inputData,
   ossimRefPtr<ossimImageData>& outputData)
{
   double sum = 0.0;
   ossim_int32 inputW        = (ossim_int32)inputData->getWidth();
   ossim_int32 outputW       = (ossim_int32)outputData->getWidth();
   ossim_int32 outputH       = (ossim_int32)outputData->getHeight();
   ossim_int32 numberOfBands = (ossim_int32)inputData->getNumberOfBands();
   ossimIpt outputOrigin = outputData->getOrigin();
   ossimIpt inputOrigin  = inputData->getOrigin();
   
   ossim_int32 startInputOffset = std::abs(outputOrigin.y - inputOrigin.y)*
      inputW + std::abs(outputOrigin.x - inputOrigin.x);
   ossim_int32 ulKernelStart    = -inputW - 1;
   ossim_int32 leftKernelStart  = -1;
   ossim_int32 llKernelStart    = inputW  - 1;
   
   const T* ulKernelStartBuf   = NULL;
   const T* leftKernelStartBuf = NULL;
   const T* llKernelStartBuf   = NULL;
   
   for(ossim_int32 band = 0; band < numberOfBands; ++band)
   {
      const T* inputBuf  = static_cast<const T*>(inputData->getBuf(band)) +
         startInputOffset;
      T* outputBuf = static_cast<T*>(outputData->getBuf(band));
      T maxPix     = static_cast<T>(inputData->getMaxPix(band));
      T minPix     = static_cast<T>(inputData->getMinPix(band));
      T nullPix    = static_cast<T>(inputData->getNullPix(band));
      
      if(inputBuf&&outputBuf)
      {
         for(ossim_int32 row = 0; row < outputW; ++row)
         {
            ossim_int32 rowOffset    = inputW*row;
            ulKernelStartBuf   = inputBuf + (rowOffset + ulKernelStart);
            leftKernelStartBuf = inputBuf + (rowOffset + leftKernelStart);
            llKernelStartBuf   = inputBuf + (rowOffset + llKernelStart);
            for(ossim_int32 col = 0; col < outputH; ++col)
            {
               sum = -1.0*(double)ulKernelStartBuf[0]  + -2.0*(double)ulKernelStartBuf[1] + -1.0*(double)ulKernelStartBuf[2] +
                     -2.0*(double)leftKernelStartBuf[0]  + 16.0*(double)leftKernelStartBuf[1] + -2.0*(double)leftKernelStartBuf[2] +
                     -1.0*(double)llKernelStartBuf[0]  + -2.0*(double)llKernelStartBuf[1] + -1.0*(double)llKernelStartBuf[2];
               sum /= 4.0;
               
               if(sum > maxPix)
               {
                  *outputBuf = maxPix;
               }
               else if((sum < minPix) || (sum == nullPix))
               {
                  *outputBuf = minPix;
               }
               else
               {
                  *outputBuf = static_cast<T>(sum);
               }
               
               ++ulKernelStartBuf
                  ;
               ++leftKernelStartBuf;
               ++llKernelStartBuf;
               ++outputBuf;
            }
         }
      }
   }
}

void ossimImageSharpenFilter::initialize()
{
   // Hmmm... (drb)
}

void ossimImageSharpenFilter::allocate()
{   
   theTile = NULL;
   if(theInputConnection)
   {
      ossimImageDataFactory* idf = ossimImageDataFactory::instance();
      theTile = idf->create(this, this);
      theTile->initialize();
   }
}

ossimString ossimImageSharpenFilter::getShortName() const
{
   return ossimString("Sharpen");
}

ossimString ossimImageSharpenFilter::getLongName() const
{
   return ossimString("Sharpens the input input");
}

