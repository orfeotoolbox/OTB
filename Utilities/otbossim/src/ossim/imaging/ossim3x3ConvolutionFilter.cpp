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
// $Id: ossim3x3ConvolutionFilter.cpp 12956 2008-06-02 01:38:50Z dburken $

#include <cstdlib>
#include <ossim/imaging/ossim3x3ConvolutionFilter.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimMatrixProperty.h>

RTTI_DEF1(ossim3x3ConvolutionFilter, "ossim3x3ConvolutionFilter", ossimImageSourceFilter);

ossim3x3ConvolutionFilter::ossim3x3ConvolutionFilter(ossimObject* owner)
   :ossimImageSourceFilter(owner),
    theTile(NULL),
    theNullPixValue(0),
    theMinPixValue(0),
    theMaxPixValue(0)
{
   theKernel[0][0] = 0.0; theKernel[0][1] = 0.0; theKernel[0][2] = 0.0;
   theKernel[1][0] = 0.0; theKernel[1][1] = 1.0; theKernel[1][2] = 0.0;
   theKernel[2][0] = 0.0; theKernel[2][1] = 0.0; theKernel[2][2] = 0.0;
   
}

ossim3x3ConvolutionFilter::~ossim3x3ConvolutionFilter()
{
}

ossimRefPtr<ossimImageData> ossim3x3ConvolutionFilter::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      return theTile;
   }

   if(!isSourceEnabled())
   {
      return theInputConnection->getTile(tileRect, resLevel);
   }

   //---
   // We have a 3x3 matrix so stretch the rect out to cover
   // the required pixels.  We only need 1 pixel to the left
   // and right of the center pixel.
   //---
   ossimIrect newRect(ossimIpt(tileRect.ul().x - 1,
                               tileRect.ul().y - 1),
                      ossimIpt(tileRect.lr().x + 1,
                               tileRect.lr().y + 1));
   
   ossimRefPtr<ossimImageData> data = theInputConnection->getTile(newRect,
                                                                  resLevel);

   if(!data.valid() || !data->getBuf())
   {
      return data;
   }

    // First time through or after an initialize()...
   if (!theTile.valid())
   {
      allocate();
      if (!theTile.valid()) // Should never happen!
      {
         return data;
      }
   }

   // First time through, after an initialize() or a setKernel()...
   if (!theNullPixValue.size())
   {
      computeNullMinMax();
      if (!theNullPixValue.size()) // Should never happen!
      {
         return data;
      }
   }

   theTile->setImageRectangle(tileRect);
   theTile->makeBlank();
   
   switch(data->getScalarType())
   {
      case OSSIM_UCHAR:
      {
         if(data->getDataObjectStatus() == OSSIM_FULL)
         {
            convolveFull(static_cast<ossim_uint8>(0), data, theTile);
         }
         else
         {
            convolvePartial(static_cast<ossim_uint8>(0), data, theTile);
         }
         break;
      }
      case OSSIM_FLOAT: 
      case OSSIM_NORMALIZED_FLOAT:
      {
         if(data->getDataObjectStatus() == OSSIM_FULL)
         {
            convolveFull(static_cast<float>(0), data, theTile);
         }
         else
         {
            convolvePartial(static_cast<float>(0), data, theTile);
         }
         break;
      }
      case OSSIM_USHORT16:
      case OSSIM_USHORT11:
      {
         if(data->getDataObjectStatus() == OSSIM_FULL)
         {
            convolveFull(static_cast<ossim_uint16>(0), data, theTile);
         }
         else
         {
            convolvePartial(static_cast<ossim_uint16>(0), data, theTile);
         }
         break;
      }
      case OSSIM_SSHORT16:
      {
         if(data->getDataObjectStatus() == OSSIM_FULL)
         {
            convolveFull(static_cast<ossim_sint16>(0), data, theTile);
         }
         else
         {
            convolvePartial(static_cast<ossim_sint16>(0), data, theTile);
         }
         break;
      }
      case OSSIM_DOUBLE:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         if(data->getDataObjectStatus() == OSSIM_FULL)
         {
            convolveFull(static_cast<double>(0), data, theTile);
      }
      else
      {
         convolvePartial(static_cast<double>(0), data, theTile);
      }
         break;
      }
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossim3x3ConvolutionFilter::getTile WARNING:\n"
            << "Scalar type = " << theTile->getScalarType()
            << " Not supported by ossim3x3ConvolutionFilter" << endl;
         break;
      }
   }
   theTile->validate();
   
   return theTile;
}


template<class T> void ossim3x3ConvolutionFilter::convolvePartial(
   T,
   ossimRefPtr<ossimImageData> inputData,
   ossimRefPtr<ossimImageData> outputData)
{
   // let's set up some temporary variables so we don't
   // have to call the functions in loops.  Iknow that compilers
   // typically optimize this out but if we are in debug mode 
   // with no optimization it will still run fast
   //
   double sum = 0.0;
   ossim_int32 inputW          = (ossim_int32)inputData->getWidth();
   ossim_int32 outputW         = (ossim_int32)outputData->getWidth();
   ossim_int32 outputH         = (ossim_int32)outputData->getHeight();
   ossim_int32 numberOfBands   = (ossim_int32)inputData->getNumberOfBands();
   ossimIpt outputOrigin  = outputData->getOrigin();
   ossimIpt inputOrigin   = inputData->getOrigin();
   
   ossim_int32 startInputOffset = std::abs(outputOrigin.y - inputOrigin.y)*
      inputW + std::abs(outputOrigin.x - inputOrigin.x);
   ossim_int32 ulKernelStart    = -inputW - 1;
   ossim_int32 leftKernelStart  = -1;
   ossim_int32 llKernelStart    = inputW  - 1;
   
   T* ulKernelStartBuf    = NULL;
   T* leftKernelStartBuf  = NULL;
   T* llKernelStartBuf    = NULL;
   
   for(ossim_int32 band = 0; band < numberOfBands; ++band)
   {
      T* inputBuf  = static_cast<T*>(inputData->getBuf(band))+startInputOffset;
      T* outputBuf = static_cast<T*>(outputData->getBuf(band));
      T maxPix     = static_cast<T>(getMaxPixelValue(band));
      T minPix     = static_cast<T>(getMinPixelValue(band));      
      T nullPix    = static_cast<T>(inputData->getNullPix(band));
      T oNullPix   = static_cast<T>(getNullPixelValue(band));
      
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
                  sum = theKernel[0][0]*(double)ulKernelStartBuf[0] +
                        theKernel[0][1]*(double)ulKernelStartBuf[1] +
                        theKernel[0][2]*(double)ulKernelStartBuf[2] +
                        theKernel[1][0]*(double)leftKernelStartBuf[0] +
                        theKernel[1][1]*(double)leftKernelStartBuf[1] +
                        theKernel[1][2]*(double)leftKernelStartBuf[2] +
                        theKernel[2][0]*(double)llKernelStartBuf[0] +
                        theKernel[2][1]*(double)llKernelStartBuf[1] +
                        theKernel[2][2]*(double)llKernelStartBuf[2];
                  
                  if(sum > maxPix)
                  {
                     *outputBuf = maxPix;
                  }
                  else if(sum < minPix)
                  {
                     *outputBuf = minPix;
                  }
                  else
                  {
                     *outputBuf = static_cast<T>(sum);
                  }
               }
               else {
                  *outputBuf = oNullPix;
               }
               //
               // Need to implement the convolution here
               //
               
               ++ulKernelStartBuf;
               ++leftKernelStartBuf;
               ++llKernelStartBuf;
               ++outputBuf;
            }
         }
      }
   }
}

template<class T> void ossim3x3ConvolutionFilter::convolveFull(
   T,
   ossimRefPtr<ossimImageData> inputData,
   ossimRefPtr<ossimImageData> outputData)
{
   // let's set up some temporary variables so we don't
   // have to call the functions in loops.  Iknow that compilers
   // typically optimize this out but if we are in debug mode 
   // with no optimization it will still run fast
   //
   double sum = 0.0;
   ossim_int32 inputW = static_cast<ossim_int32>(inputData->getWidth());
   ossim_uint32 outputW       = outputData->getWidth();
   ossim_uint32 outputH       = outputData->getHeight();
   ossim_uint32 numberOfBands = inputData->getNumberOfBands();
   ossimIpt outputOrigin = outputData->getOrigin();
   ossimIpt inputOrigin  = inputData->getOrigin();
   
   ossim_int32 startInputOffset = std::abs(outputOrigin.y - inputOrigin.y)*
      inputW + std::abs(outputOrigin.x - inputOrigin.x);
   ossim_int32 ulKernelStart    = -inputW - 1;
   ossim_int32 leftKernelStart  = -1;
   ossim_int32 llKernelStart    = inputW  - 1;
   
   T* ulKernelStartBuf   = NULL;
   T* leftKernelStartBuf = NULL;
   T* llKernelStartBuf   = NULL;
   
   for(ossim_uint32 band = 0; band < numberOfBands; ++band)
   {
      T* inputBuf  = static_cast<T*>(inputData->getBuf(band))+startInputOffset;
      T* outputBuf = static_cast<T*>(outputData->getBuf(band));
      T maxPix     = static_cast<T>(getMaxPixelValue(band));
      T minPix     = static_cast<T>(getMinPixelValue(band));
      
      if(inputBuf&&outputBuf)
      {
         for(ossim_uint32 row = 0; row < outputW; ++row)
         {
            ossim_int32 rowOffset    = inputW*row;
            ulKernelStartBuf   = inputBuf + (rowOffset + ulKernelStart);
            leftKernelStartBuf = inputBuf + (rowOffset + leftKernelStart);
            llKernelStartBuf   = inputBuf + (rowOffset + llKernelStart);
            for(ossim_uint32 col = 0; col < outputH; ++col)
            {
               sum = theKernel[0][0]*(double)ulKernelStartBuf[0] +
                     theKernel[0][1]*(double)ulKernelStartBuf[1] +
                     theKernel[0][2]*(double)ulKernelStartBuf[2] +
                     theKernel[1][0]*(double)leftKernelStartBuf[0] +
                     theKernel[1][1]*(double)leftKernelStartBuf[1] +
                     theKernel[1][2]*(double)leftKernelStartBuf[2] +
                     theKernel[2][0]*(double)llKernelStartBuf[0] +
                     theKernel[2][1]*(double)llKernelStartBuf[1] +
                     theKernel[2][2]*(double)llKernelStartBuf[2];
               
               if(sum > maxPix)
               {
                  *outputBuf = maxPix;
               }
               else if(sum < minPix)
               {
                  *outputBuf = minPix;
               }
               else
               {
                  *outputBuf = static_cast<T>(sum);
               }
               //
               // Need to implement the convolution here.
               //

               
               ++ulKernelStartBuf;
               ++leftKernelStartBuf;
               ++llKernelStartBuf;
               ++outputBuf;
            }
         }
      }
   }
}

void ossim3x3ConvolutionFilter::initialize()
{
   //---
   // NOTE:
   // Since initialize get called often sequentially we will wipe things slick
   // but not reallocate to avoid multiple delete/allocates.
   //
   // On the first getTile call things will be reallocated/computed.
   //---
   theTile = NULL;
   clearNullMinMax();
}

void ossim3x3ConvolutionFilter::allocate()
{   
   if(theInputConnection)
   {
      ossimImageDataFactory* idf = ossimImageDataFactory::instance();
      
      theTile = idf->create(this,
                            this);
      
      theTile->initialize();
   }
}

void ossim3x3ConvolutionFilter::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property) return;
   if(property->getName() == "Kernel")
   {
      ossimMatrixProperty* matrixProperty = PTR_CAST(ossimMatrixProperty,
                                                     property.get());
      if(matrixProperty)
      {
         theKernel[0][0] = (*matrixProperty)(0,0);
         theKernel[1][0] = (*matrixProperty)(1,0);
         theKernel[2][0] = (*matrixProperty)(2,0);
         theKernel[0][1] = (*matrixProperty)(0,1);
         theKernel[1][1] = (*matrixProperty)(1,1);
         theKernel[2][1] = (*matrixProperty)(2,1);
         theKernel[0][2] = (*matrixProperty)(0,2);
         theKernel[1][2] = (*matrixProperty)(1,2);
         theKernel[2][2] = (*matrixProperty)(2,2);

      }
      else
      {
         ossimImageSourceFilter::setProperty(property);
      }
   }
   else
   {
      ossimImageSourceFilter::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossim3x3ConvolutionFilter::getProperty(const ossimString& name)const
{
   if(name == "Kernel")
   {
      ossimMatrixProperty* property = new ossimMatrixProperty(name);
      property->resize(3,3);
      (*property)(0,0) = theKernel[0][0];
      (*property)(1,0) = theKernel[1][0];
      (*property)(2,0) = theKernel[2][0];
      (*property)(0,1) = theKernel[0][1];
      (*property)(1,1) = theKernel[1][1];
      (*property)(2,1) = theKernel[2][1];
      (*property)(0,2) = theKernel[0][2];
      (*property)(1,2) = theKernel[1][2];
      (*property)(2,2) = theKernel[2][2];
      property->setCacheRefreshBit();

      return property;
   }
   return ossimImageSourceFilter::getProperty(name);
}

void ossim3x3ConvolutionFilter::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSourceFilter::getPropertyNames(propertyNames);
   propertyNames.push_back("Kernel");
}

bool ossim3x3ConvolutionFilter::saveState(ossimKeywordlist& kwl,
                                          const char* prefix)const
{   
   kwl.add(prefix,
           "rows",
           3,
           true);
   
   kwl.add(prefix,
           "cols",
           3,
           true);
   
   for(ossim_int32 row = 0; row < 3; ++row)
   {
      for(ossim_int32 col =0; col < 3; ++col)
      {
         ossimString newPrefix = "m" +
                                 ossimString::toString(row+1) + "_" +
                                 ossimString::toString(col+1);
         kwl.add(prefix,
                 newPrefix,
                 theKernel[row][col],
                 true);          
      }
   }

   
   return ossimImageSourceFilter::saveState(kwl, prefix);
}


bool ossim3x3ConvolutionFilter::loadState(const ossimKeywordlist& kwl,
                                          const char* prefix)
{
   ossimString newPrefix = prefix;
   newPrefix += ossimString("m");
   
   for(ossim_int32 r = 0; r < 3; r++)
   {
      for(ossim_int32 c = 0; c < 3; c++)
      {
         theKernel[r][c] = 0.0;
         
         ossimString value = ossimString::toString(r+1);
         value += "_";
         value += ossimString::toString(c+1);
         
         const char* v = kwl.find(newPrefix.c_str(),
                                  value.c_str());
         if(v)
         {
            theKernel[r][c] = ossimString(v).toDouble();
         }
      }
   }
   
   return ossimImageSourceFilter::loadState(kwl, prefix);
}

void ossim3x3ConvolutionFilter::getKernel(double kernel[3][3])
{
   kernel[0][0] = theKernel[0][0];
   kernel[0][1] = theKernel[0][1];
   kernel[0][2] = theKernel[0][2];
   kernel[1][0] = theKernel[1][0];
   kernel[1][1] = theKernel[1][1];
   kernel[1][2] = theKernel[1][2];
   kernel[2][0] = theKernel[2][0];
   kernel[2][1] = theKernel[2][1];
   kernel[2][2] = theKernel[2][2];
}

void ossim3x3ConvolutionFilter::setKernel(double kernel[3][3])
{
   theKernel[0][0] = kernel[0][0];
   theKernel[0][1] = kernel[0][1];
   theKernel[0][2] = kernel[0][2];
   theKernel[1][0] = kernel[1][0];
   theKernel[1][1] = kernel[1][1];
   theKernel[1][2] = kernel[1][2];
   theKernel[2][0] = kernel[2][0];
   theKernel[2][1] = kernel[2][1];
   theKernel[2][2] = kernel[2][2];

   // Will be recomputed first getTile call.
   clearNullMinMax();
}

double ossim3x3ConvolutionFilter::getNullPixelValue(ossim_uint32 band)const
{
   if( isSourceEnabled() && (band < theNullPixValue.size()) )
   {
      return theNullPixValue[band];
   }

   return ossim::defaultNull(getOutputScalarType());
}

double ossim3x3ConvolutionFilter::getMinPixelValue(ossim_uint32 band)const
{
   if( isSourceEnabled() && (band < theMinPixValue.size()) )
   {
      return theMinPixValue[band];
   }
   
   return ossimImageSource::getMinPixelValue(band);
}

double ossim3x3ConvolutionFilter::getMaxPixelValue(ossim_uint32 band)const
{
   if( isSourceEnabled() && (band < theMaxPixValue.size()) )
   {
      return theMaxPixValue[band];
   }

   return ossimImageSource::getMaxPixelValue(band);
}

void ossim3x3ConvolutionFilter::clearNullMinMax()
{
   theNullPixValue.clear();
   theMinPixValue.clear();
   theMaxPixValue.clear();
}

void ossim3x3ConvolutionFilter::computeNullMinMax()
{
   const ossim_uint32 BANDS = getNumberOfOutputBands();

   theNullPixValue.resize(BANDS);
   theMinPixValue.resize(BANDS);
   theMaxPixValue.resize(BANDS);

   ossim_float64 defaultNull = ossim::defaultNull(getOutputScalarType());
   ossim_float64 defaultMin = ossim::defaultMin(getOutputScalarType());
   ossim_float64 defaultMax = ossim::defaultMax(getOutputScalarType());
  
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      if(theInputConnection)
      {
         ossim_float64 inputNull = theInputConnection->getNullPixelValue(band);
         ossim_float64 inputMin  = theInputConnection->getMinPixelValue(band);
         ossim_float64 inputMax  = theInputConnection->getMaxPixelValue(band);
         ossim_float64 tempMin   = 0.0;
         ossim_float64 tempMax   = 0.0;
         ossim_float64 k         = 0.0;
         for(int i=0;i<3;++i)
         {
            for(int j=0;j<3;++j)
            {
               k=theKernel[i][j];
               tempMin += (k<0.0) ? k*inputMax : k*inputMin;
               tempMax += (k>0.0) ? k*inputMax : k*inputMin;
            }
         }

         if((inputNull < getMinPixelValue(band)) ||
            (inputNull > getMaxPixelValue(band)))
         {
            theNullPixValue[band] = inputNull;
         }
         else
         {
            theNullPixValue[band] = defaultNull;
         }

         if((tempMin >= defaultMin) && (tempMin <= defaultMax))
         {
            theMinPixValue[band] = tempMin;
         }
         else
         {
            theMinPixValue[band] = defaultMin;
         }

         if((tempMax >= defaultMin) && (tempMax <= defaultMax))
         {
            theMaxPixValue[band] = tempMax;
         }
         else
         {
            theMaxPixValue[band] = defaultMax;
         }
         
      }
      else // No input connection...
      {
         theNullPixValue[band] = defaultNull;
         theMinPixValue[band]  = defaultMin;
         theMaxPixValue[band]  = defaultMax;
      }
      
   } // End of band loop.
}
