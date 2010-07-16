//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimTilePatch.cpp 17195 2010-04-23 17:32:18Z dburken $

#include <vector>
using namespace std;

#include <ossim/imaging/ossimTilePatch.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/base/ossimErrorCodes.h>
ossimDiscreteNearestNeighbor ossimTilePatch::nearestNeighborKernel;

ossimTilePatch::ossimTilePatch(ossimImageSource* input)
{
   thePatchData = ossimImageDataFactory::instance()->create(NULL, input);
   thePatchData->initialize();
}

ossimTilePatch::ossimTilePatch(ossimScalarType scalarType,
                               long numberOfComponents,
                               long width,
                               long height)
   :thePatchData(NULL)
{
   thePatchData = ossimImageDataFactory::instance()->create(NULL,
                                                            scalarType,
                                                            numberOfComponents,
                                                            width,
                                                            height);

   thePatchData->initialize();
}

ossimTilePatch::~ossimTilePatch()
{
}

ossimIrect ossimTilePatch::getRect()const
{
   return thePatchData->getImageRectangle();
}

ossimDrect ossimTilePatch::findBoundingRect(const ossimDpt &p1,
                                            const ossimDpt &p2,
                                            const ossimDpt &p3,
                                            const ossimDpt &p4)
{
   vector<ossimDpt> points(4);
   
   points[0] = p1;
   points[1] = p2;
   points[2] = p3;
   points[3] = p4;

   return ossimDrect(points);
}

void ossimTilePatch::fillPatch(ossimImageSource* tileSource,
                               long resLevel)
{
   // long w = tileSource->getTileWidth();
   // long h = tileSource->getTileHeight();
   
   if(thePatchData.valid())
   {
      ossimIrect rect   = thePatchData->getImageRectangle();
      thePatchData->makeBlank();
      // ossimIpt origin(rect.ul());
      ossimRefPtr<ossimImageData> aTile = tileSource->getTile(rect, resLevel);
      thePatchData->loadTile(aTile.get());
   }
}

void ossimTilePatch::setData(ossimRefPtr<ossimImageData>& patchData)
{
   thePatchData = patchData;
}

void ossimTilePatch::convolve(ossimDiscreteConvolutionKernel* kernel)
{
   if(!kernel) return;

   long w = thePatchData->getWidth();
   long h = thePatchData->getHeight();

   long kW = kernel->getWidth();
   long kH = kernel->getHeight();
   ossimIpt origin = thePatchData->getOrigin();
   
   if( (w < kW) || (h < kH))
   {
      cerr << " Error ossimTilePatch::convolve(kernel): patch is smaller than kernel size" << endl;
      return;
   }
   ossimRefPtr<ossimImageData> kernelBuffer =(ossimImageData*) thePatchData->dup();

   // now let's make the buffer just fit the convolution filter
   //
   kernelBuffer->setWidth(w - kW);
   kernelBuffer->setHeight(h - kH);
   kernelBuffer->setOrigin(ossimDpt(origin.x + kW/2.0,
                                    origin.y + kH/2.0));
   kernelBuffer->initialize();

   fillTile(kernelBuffer, kernel);

   kernelBuffer->validate();
   // now copy the data back to the buffer
   //
   thePatchData->loadTile(kernelBuffer.get());

   // now make sure we validate the buffer.
   thePatchData->validate();
}

void ossimTilePatch::fillTile(ossimRefPtr<ossimImageData>& aTile)const
{
   if(aTile.valid() &&
      (aTile->getNumberOfBands()==thePatchData->getNumberOfBands())&&
      (aTile->getBuf())&&
      (thePatchData->getBuf())&&
      (thePatchData->getScalarType()==aTile->getScalarType()))
   {
      aTile->loadTile(thePatchData.get());
   }
}

void ossimTilePatch::fillTile(ossimRefPtr<ossimImageData>& result,
                              ossimDiscreteConvolutionKernel* kernel)const
{
   if(!kernel)
   {
      cerr << "Error: Kernel is NULL in  ossimTilePatch::fillTile(tile, kernel)!!" << endl;
   }
   if(thePatchData->getScalarType() != result->getScalarType())
   {
      //ERROR
      return;
   }
   else
   {
      switch(thePatchData->getScalarType())
      {
      case OSSIM_UCHAR:
      {
         fillTileTemplate(static_cast<ossim_uint8>(0),
                          result,
                          kernel);
         break;
      }
      case OSSIM_USHORT16:
      case OSSIM_USHORT11:
      {
         fillTileTemplate(static_cast<ossim_uint16>(0),
                          result,
                          kernel);
         break;
      }
      case OSSIM_SSHORT16:
      {
         fillTileTemplate(static_cast<ossim_sint16>(0),
                          result,
                          kernel);
         break;
      }
      case OSSIM_FLOAT:
      case OSSIM_NORMALIZED_FLOAT:
      {
         fillTileTemplate(static_cast<float>(0),
                          result,
                          kernel);
         break;
      }
      case OSSIM_DOUBLE:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         fillTileTemplate(static_cast<double>(0),
                          result,
                          kernel);
         break;
      }
      default:
	{
	  ossimSetError("ossimTilePatch",
			ossimErrorCodes::OSSIM_ERROR,
			"unhandled scalar type %d",
			(int)thePatchData->getScalarType());
	  break;
	}
      }
   }
}



void ossimTilePatch::fillTile(ossimRefPtr<ossimImageData>& result,
                              const ossimDpt   &ul,
                              const ossimDpt   &ur,
                              const ossimDpt   &deltaUl,
                              const ossimDpt   &deltaUr,
                              const ossimDpt   &length)const
{   
   result->setNullPix(thePatchData->getNullPix(), thePatchData->getNumberOfBands());
   result->setMinPix(thePatchData->getMinPix(), thePatchData->getNumberOfBands());
   result->setMaxPix(thePatchData->getMaxPix(), thePatchData->getNumberOfBands());
   result->makeBlank();
   if(thePatchData->getScalarType() != result->getScalarType())
   {
      //ERROR
      return;
   }
   else
   {
      switch(thePatchData->getScalarType())
      {
      case OSSIM_UCHAR:
      {
         fillTileTemplate(static_cast<ossim_uint8>(0),
                          result,
                          ul,
                          ur,
                          deltaUl,
                          deltaUr,
                          length);
         break;
      }
      case OSSIM_USHORT16:
      case OSSIM_USHORT11:
      {
         fillTileTemplate(static_cast<ossim_uint16>(0),
                          result,
                          ul,
                          ur,
                          deltaUl,
                          deltaUr,
                          length);
         break;
      }
      case OSSIM_SSHORT16:
      {
         fillTileTemplate(static_cast<ossim_sint16>(0),
                          result,
                          ul,
                          ur,
                          deltaUl,
                          deltaUr,
                          length);
         break;
      }
      case OSSIM_FLOAT:
      case OSSIM_NORMALIZED_FLOAT:
      {
         fillTileTemplate(static_cast<float>(0),
                          result,
                          ul,
                          ur,
                          deltaUl,
                          deltaUr,
                          length);
         break;
      }
      case OSSIM_DOUBLE:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         fillTileTemplate(static_cast<double>(0),
                          result,
                          ul,
                          ur,
                          deltaUl,
                          deltaUr,
                          length);
         break;
      }
      default:
	{
	  ossimSetError("ossimTilePatch",
			ossimErrorCodes::OSSIM_ERROR,
			"unhandled scalar type %d",
			(int)thePatchData->getScalarType());
	  break;
	}
      }
   }

}


template<class T>
void ossimTilePatch::fillTileTemplate(T /* dummyVariable */,
                                      ossimRefPtr<ossimImageData>& result,
                                      ossimDiscreteConvolutionKernel* kernel)const
{
   // get the origin.  The convolution could be
   // an offset into the patch.
   //
   ossimIpt startOrigin   = result->getOrigin();

   // Make sure that the patch is not empty or NULL
   //
   ossimDataObjectStatus status = thePatchData->getDataObjectStatus();
   if((status==OSSIM_EMPTY)||
      (status == OSSIM_NULL))
   {
      return;
   }
   ossimDpt startDelta(fabs((double)startOrigin.x - thePatchData->getOrigin().x),
                       fabs((double)startOrigin.y - thePatchData->getOrigin().y));
   
   // let's setup some variables that we will need to do the
   // convolution algorithm.
   //
   ossimIrect patchRect   = thePatchData->getImageRectangle();
   long tileHeight        = result->getHeight();
   long tileWidth         = result->getWidth();
   long outputBands       = result->getNumberOfBands();
   long convolutionWidth  = kernel->getWidth();
   long convolutionHeight = kernel->getHeight();
   long convolutionOffsetX= convolutionWidth/2;
   long convolutionOffsetY= convolutionHeight/2;
   long patchWidth        = patchRect.width();
   long patchCenterOffset = (long)(patchWidth*startDelta.y + startDelta.x);
   long patchConvolutionOffsetDelta = patchWidth*convolutionOffsetY + convolutionOffsetX;
   long patchLineStartOffset        = patchCenterOffset  - patchConvolutionOffsetDelta;
   long outputOffset                = 0;
   double min = 1.0;
   double max = 255.0;
   if(status == OSSIM_PARTIAL) // must check for NULLS
   {
      for(long y = 0; y <tileHeight; y++)
      {
         patchCenterOffset = patchLineStartOffset;
         
         for(long x =0; x < tileWidth; x++)
         {  
            if(!thePatchData->isNull(patchCenterOffset))
            {
               double convolveResult = 0;
               for(long b = 0; b < outputBands; ++b)
               {                  
                  min=result->getMinPix(b);
                  max=result->getMaxPix(b);
                  T* buf    = (T*)(thePatchData->getBuf(b)) + patchCenterOffset;
                  T* outBuf = (T*)(result->getBuf(b));

                  kernel->convolveSubImage(buf,
                                           patchWidth,
                                           convolveResult);
                  convolveResult = convolveResult < min?min:convolveResult;
                  convolveResult = convolveResult > max?max:convolveResult;
                  outBuf[outputOffset] = (ossim_uint8)convolveResult;
               }
            }
            else
            {
               result->setNull(outputOffset);
            }
            ++outputOffset;
            ++patchCenterOffset;
         }
         patchLineStartOffset += patchWidth;
      }
    }
    else
    {
       for(long b = 0; b < outputBands; ++b)
       {                  
          double convolveResult = 0;
          const T* buf    = (const T*)thePatchData->getBuf(b);
          T* outBuf = (T*)(result->getBuf(b));
          outputOffset = 0;
          patchCenterOffset = (long)(patchWidth*startDelta.y + startDelta.x);
          patchLineStartOffset  = patchCenterOffset - patchConvolutionOffsetDelta;
          min=result->getMinPix(b);
          max=result->getMaxPix(b);
          
          for(long y = 0; y <tileHeight; y++)
          {
             patchCenterOffset = patchLineStartOffset;
             
             for(long x =0; x < tileWidth; x++)
             {
                kernel->convolveSubImage(&buf[patchCenterOffset],
                                         patchWidth,
                                         convolveResult);
                convolveResult = convolveResult < min? min:convolveResult;
                convolveResult = convolveResult > max?max:convolveResult;
                outBuf[outputOffset] = (T)convolveResult;
                ++outputOffset;
                ++patchCenterOffset;
             }
             patchLineStartOffset += patchWidth;
          }
       }
    }
}


template <class T>
void ossimTilePatch::fillTileTemplate(T /* dummyVariable */,
                              ossimRefPtr<ossimImageData>& result,
                              const ossimDpt   &ul,
                              const ossimDpt   &ur,
                              const ossimDpt   &deltaUl,
                              const ossimDpt   &deltaUr,
                              const ossimDpt   &length)const
{
   double stepSizeWidth  = 1.0/length.x;
   double stepSizeHeight = 1.0/length.y;
   long patchWidth = thePatchData->getWidth();
   long patchHeight = thePatchData->getHeight();

   
   ossimIrect rect = thePatchData->getImageRectangle();
   
   ossimDpt startSave(ul.x - rect.ul().x,
                      ul.y - rect.ul().y );
   ossimDpt endSave(ur.x - rect.ul().x,
                    ur.y - rect.ul().y);
   for(ossim_uint32 band = 0; band < thePatchData->getNumberOfBands(); ++band)
   {
      T *resultBuf = (T*)result->getBuf(band);
      const T *sourceBuf = (T*)thePatchData->getBuf(band);
      ossimDpt start = startSave;
      ossimDpt end   = endSave;
      T nullPix = static_cast<T>(result->getNullPix(band));
      for(long y = 0; y < length.y; y++)
      {
         double deltaX = (end.x - start.x)*stepSizeWidth;
         double deltaY = (end.y - start.y)*stepSizeHeight;
         ossimDpt pointXY = start;
         for(long x = 0; x < length.x; x++)
         {
            int xPixel = pointXY.x<0?(int)floor(pointXY.x):(int)pointXY.x;
            int yPixel = pointXY.y<0?(int)floor(pointXY.y):(int)pointXY.y;
            if( (xPixel >=0) && (xPixel < patchWidth) &&
                (yPixel >=0) && (yPixel < patchHeight))
            {              
               *resultBuf = sourceBuf[yPixel*patchWidth + xPixel];
            }
            else
            {
               *resultBuf = nullPix;
            }
            
            resultBuf++;
            pointXY.y += deltaY;
            pointXY.x += deltaX;
         }
         
         start.x += deltaUl.x;
         start.y += deltaUl.y;
         end.x   += deltaUr.x;
         end.y   += deltaUr.y;
      }
   }
}


void ossimTilePatch::setRect(const ossimDpt &p1,
                             const ossimDpt &p2,
                             const ossimDpt &p3,
                             const ossimDpt &p4,
                             const ossimDpt &tile_size,
                             const ossimDpt &padding)
{
   setRect(findBoundingRect(p1, p2, p3, p4),
           tile_size,
           padding);
}

void ossimTilePatch::setRect(const ossimDpt &center,
                             const ossimDpt &rect_size,
                             const ossimDpt &tile_size,
                             const ossimDpt &padding)
{
   ossimDpt ul (center - rect_size/2.0);
   ossimDpt lr (center + rect_size/2.0);
   ossimDrect rect (ul, lr);
   setRect(rect, tile_size, padding);
}

void ossimTilePatch::setRect(const ossimDrect& aRect,
                             const ossimDpt& /* tile_size*/,
                             const ossimDpt& padding)
{
   ossimDpt   ul(aRect.ul().x - padding.x,
                 aRect.ul().y - padding.y);
   ossimDpt   lr(aRect.lr().x + padding.x,
                 aRect.lr().y + padding.y);

   ossimDrect rect(ul, lr);
   rect.stretchOut();
   
//   rect   = alignRectToBoundary(rect, tile_size);

   if(thePatchData.valid())
   {
      if(  ((long)thePatchData->getWidth()  != (long)rect.width()) ||
           ((long)thePatchData->getHeight() != (long)rect.height()))
      {
         thePatchData->setWidth((long)rect.width());
         thePatchData->setHeight((long)rect.height());
      }
      thePatchData->setOrigin(rect.ul());
      thePatchData->initialize();
   }
}

