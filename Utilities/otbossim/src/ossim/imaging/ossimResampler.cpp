//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
//
// Description:
//
// Contains class definition for ossimResampler.
// 
//*******************************************************************
//  $Id: ossimResampler.cpp 20204 2011-11-04 15:12:28Z dburken $


#include <iostream>
using namespace std;
#include <ossim/imaging/ossimResampler.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimFilter.h>
#include <ossim/imaging/ossimTilePatch.h>
#include <ossim/imaging/ossimImageHandler.h>

RTTI_DEF1(ossimResampler, "ossimResampler", ossimConnectableObject)


static const char* RESAMPLER_CONVOLUTION_TYPE_KW = "convolution_type";
static const char* RESAMPLER_SCALE_X_KW          = "scale_x";
static const char* RESAMPLER_SCALE_Y_KW          = "scale_y";
static const char* RESAMPLER_CUBIC_PARAMETER_KW  = "cubic_parameter";

ossimResampler::ossimResampler()
   :ossimConnectableObject(NULL,0,0,true, false),
    theOutputToInputRatio(1,1),
    theResamplerType(ossimResampler_NONE),
    theTableWidthX(0),
    theTableWidthY(0),
    theTableHeight(0),
    theKernelWidth(0),
    theKernelHeight(0),
    theWeightTableX(NULL),
    theWeightTableY(NULL),
    theCubicAdjustableParameter(-.5)
{
}

ossimResampler::~ossimResampler()
{
   deleteWeightTable();
}

void ossimResampler::resample(ossimImageData* input,
                              ossimImageData* output)
{
   if(!input||
      !output ||
      !input->getBuf() ||
      !output->getBuf())
   {
      return;
   }

   if(theWeightTableX&&theWeightTableY)
   {
      ossimScalarType scalarType = input->getScalarType();
      switch(scalarType)
      {
         case OSSIM_UINT8:
         {
            resampleTile(ossim_uint8(0), // dummy template variable
                         input,
                         output);
            break;
         }
         case OSSIM_UINT16:
         case OSSIM_USHORT11:
         {
            resampleTile(ossim_uint16(0), // dummy template variable
                         input,
                         output);
            break;
         }
         case OSSIM_SSHORT16:
         {
            resampleTile(ossim_sint16(0), // dummy template variable
                         input,
                         output);
            break;
         }      
         case OSSIM_UINT32:
         {
            resampleTile(ossim_uint32(0), // dummy template variable
                         input,
                         output);
            break;
         }      
         case OSSIM_FLOAT32:
         case OSSIM_NORMALIZED_FLOAT:
         {
            resampleTile(ossim_float32(0.0), // dummy template variable
                         input,
                         output);
            break;
         }
         case OSSIM_FLOAT64:
         case OSSIM_NORMALIZED_DOUBLE:
         {
            resampleTile(ossim_float64(0.0), // dummy template variable
                         input,
                         output);
            break;
         }
         default:
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimResampler::resample error: unknown scalar type:  "
               << scalarType
               << endl;
            
         }
      }
   }      
   // convolve the input and copy to output.
}

void ossimResampler::resample(ossimImageData* input,
                              ossimImageData* output,
                              const ossimDpt& ul,
                              const ossimDpt& ur,
                              const ossimDpt& deltaUl,
                              const ossimDpt& deltaUr,
                              const ossimDpt& length)
{
   resample(input,
            output,
            output->getImageRectangle(),
            ul,
            ur,
            deltaUl,
            deltaUr,
            length);
}

void ossimResampler::resample(ossimImageData* input,
                              ossimImageData* output,
                              const ossimIrect& outputSubRect,
                              const ossimDpt& ul,
                              const ossimDpt& ur,
                              const ossimDpt& deltaUl,
                              const ossimDpt& deltaUr,
                              const ossimDpt& length)
{
   if(!input||
      !output ||
      !input->getBuf() ||
      !output->getBuf())
   {
      return;
   }
   
   if(theWeightTableX&&theWeightTableY)
   {
      ossimScalarType scalarType = input->getScalarType();
      switch(scalarType)
      {
         case OSSIM_UINT8:
         {
            resampleTile(ossim_uint8(0), // dummy template variable
                         input,
                         output,
                         outputSubRect,
                         ul,
                         ur,
                         deltaUl,
                         deltaUr,
                         length);
            break;
         }
         case OSSIM_UINT16:
         case OSSIM_USHORT11:
         {
            resampleTile(ossim_uint16(0), // dummy template variable
                         input,
                         output,
                         outputSubRect,
                         ul,
                         ur,
                         deltaUl,
                         deltaUr,
                         length);
            break;
         }
         case OSSIM_SINT16:
         {
            resampleTile(ossim_sint16(0), // dummy template variable
                         input,
                         output,
                         outputSubRect,
                         ul,
                         ur,
                         deltaUl,
                         deltaUr,
                         length);
            break;
         }      
         case OSSIM_UINT32:
         {
            resampleTile(ossim_uint32(0), // dummy template variable
                         input,
                         output,
                         outputSubRect,
                         ul,
                         ur,
                         deltaUl,
                         deltaUr,
                         length);
            break;
         }      
         case OSSIM_FLOAT32:
         case OSSIM_NORMALIZED_FLOAT:
         {
            resampleTile(ossim_float32(0.0), // dummy template variable
                         input,
                         output,
                         outputSubRect,
                         ul,
                         ur,
                         deltaUl,
                         deltaUr,
                         length);
            break;
         }
         case OSSIM_FLOAT64:
         case OSSIM_NORMALIZED_DOUBLE:
         {
            resampleTile(ossim_float64(0.0), // dummy template variable
                         input,
                         output,
                         outputSubRect,
                         ul,
                         ur,
                         deltaUl,
                         deltaUr,
                         length);
            break;
         }
         default:
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimResampler::resample error: unknown scalar type:  "
               << scalarType
               << endl;
         }
      }
   }
   
}

void ossimResampler::resampleNearestNeighbor(ossimImageData* input,
                                             ossimImageData* output,
                                             const ossimIrect& outputSubRect,
                                             const ossimDpt& ul,
                                             const ossimDpt& ur,
                                             const ossimDpt& deltaUl,
                                             const ossimDpt& deltaUr,
                                             const ossimDpt& length)
{
   if(!input||
      !output ||
      !input->getBuf() ||
      !output->getBuf())
   {
      return;
   }
   
   ossimScalarType scalarType = input->getScalarType();
   switch(scalarType)
   {
      case OSSIM_UINT8:
      {
         resampleTileNearestNeighbor(ossim_uint8(0),// dummy template variable
                                     input,
                                     output,
                                     outputSubRect,
                                     ul,
                                     ur,
                                     deltaUl,
                                     deltaUr,
                                     length);
         break;
      }
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
      {
         resampleTileNearestNeighbor(ossim_uint16(0),// dummy template variable
                                     input,
                                     output,
                                     outputSubRect,
                                     ul,
                                     ur,
                                     deltaUl,
                                     deltaUr,
                                     length);
         break;
      }
      case OSSIM_SINT16:
      {
         resampleTileNearestNeighbor(ossim_sint16(0),// dummy template variable
                                     input,
                                     output,
                                     outputSubRect,
                                     ul,
                                     ur,
                                     deltaUl,
                                     deltaUr,
                                     length);
         break;
      }      
      case OSSIM_UINT32:
      {
         resampleTileNearestNeighbor(ossim_uint32(0),// dummy template variable
                                     input,
                                     output,
                                     outputSubRect,
                                     ul,
                                     ur,
                                     deltaUl,
                                     deltaUr,
                                     length);
         break;
      }      
      case OSSIM_FLOAT32:
      case OSSIM_NORMALIZED_FLOAT:
      {
         resampleTileNearestNeighbor(ossim_float32(0.0),
                                     input,
                                     output,
                                     outputSubRect,
                                     ul,
                                     ur,
                                     deltaUl,
                                     deltaUr,
                                     length);
         break;
      }
      case OSSIM_FLOAT64:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         resampleTileNearestNeighbor(ossim_float64(0.0),
                                     input,
                                     output,
                                     outputSubRect,
                                     ul,
                                     ur,
                                     deltaUl,
                                     deltaUr,
                                     length);
         break;
      }
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimResampler::resample error: unknown scalar type:  "
            << scalarType
            << endl;
      }
   }   
}

void ossimResampler::resampleNearestNeighbor(ossimImageData* input,
                                             ossimImageData* output,
                                             const ossimDpt& ul,
                                             const ossimDpt& ur,
                                             const ossimDpt& deltaUl,
                                             const ossimDpt& deltaUr,
                                             const ossimDpt& length)
{
   resampleNearestNeighbor(input,
                           output,
                           output->getImageRectangle(),
                           ul,
                           ur,
                           deltaUl,
                           deltaUr,
                           length);
}


template <class T>
void ossimResampler::resampleTile(T,// not used
                                  ossimImageData* input,
                                  ossimImageData* output)
{
   switch(input->getDataObjectStatus())
   {
      case OSSIM_FULL:
      {
         resampleFullTile((T)0, input, output);
         break;
      }
      case OSSIM_PARTIAL:
      {      
         resamplePartialTile((T)0, input, output);
         break;
      }
      default:
      {
         break;
      }
   }
}

template <class T>
void ossimResampler::resamplePartialTile(T,// not used
                                         ossimImageData* input,
                                         ossimImageData* output)
{   
   ossim_int32 maxInputSize  = std::max(input->getWidth(),  input->getHeight());
   ossim_int32 maxOutputSize = std::max(output->getWidth(), output->getHeight());
   ossim_int32 k;
   double *h  = new double[maxInputSize];
   int    *Lx = new int[maxOutputSize];
   int    *Ly = new int[maxOutputSize];
   ossim_int32 out_height = output->getHeight();
   ossim_int32 out_width  = output->getWidth();
   ossim_int32 in_width   = input->getWidth();
   ossim_int32 in_height  = input->getHeight();
   ossim_int32 j;
   ossim_int32 l;

   ossimIpt origin=output->getOrigin();
   ossimIpt scaledOriginShift(input->getOrigin()*-1);

   ossim_int32 evenKernelW = (ossim_int32)(!(theKernelWidth%2));
   ossim_int32 evenKernelH = (ossim_int32)(!(theKernelHeight%2));
   ossim_int32 kernelHorizontalShift = (ossim_int32)(-theKernelWidth/2.0+evenKernelW);
   ossim_int32 kernelVerticalShift   = (ossim_int32)(-theKernelHeight/2.0+evenKernelH);
   
   for (k = 0; k < maxOutputSize; k++)
   {
      Lx[k] = (ossim_int32)(scaledOriginShift.x+((k+origin.x)/
                                           theOutputToInputRatio.x));
   }
   for (k = 0; k < maxOutputSize; k++)
   {
      Ly[k] = (ossim_int32)(scaledOriginShift.y+((k+origin.y)/
                                           theOutputToInputRatio.y));
   }
   for(ossim_int32 band = 0; band < (ossim_int32)input->getNumberOfBands();++band)
   {
      const T* inputBuf  = static_cast<T*>(input->getBuf(band));
      T*       outputBuf = static_cast<T*>(output->getBuf(band));
      T        minPix    = static_cast<T>(input->getMinPix(band));
      T        maxPix    = static_cast<T>(input->getMaxPix(band));
      T        np        = static_cast<T>(input->getNullPix(band));
      
      for (k = 0; k < out_height; k++)
      {
         ossim_int32 indexMod = (ossim_int32)fmod((k+origin.y), theOutputToInputRatio.y);
         if(indexMod >= theTableWidthY) indexMod = theTableWidthY - 1;
         if(indexMod <0) indexMod = 0;
         for (j = 0; j < in_width; j++)
         {
            h[j] = 0.0;
            ossim_int32 count = 0;
            double lastValue = ossim::nan();
            for (l = 0; l < theKernelHeight; l++)
            {
               ossim_int32 index = Ly[k] + l + kernelVerticalShift;
               if ((index >= 0) && (index < in_height))
               {
                  ossim_int32 offset = index*in_width +j;
                  if(!input->isNull(offset))
                  {
                     lastValue = (double)inputBuf[offset];
                     h[j] += lastValue *
                             theWeightTableY[theKernelHeight - l-1][indexMod];
                     ++count;
                  }
                  // I think instead of skipping to next value I'll just assume 0
                  
                  // this was code to skip to next non-null value and use it
                  // 
//                   else
//                   {
//                      for(ossim_int32 templ = l; templ < theKernelHeight; ++templ)
//                      {
//                         index = Ly[k] + templ + kernelVerticalShift;
//                         offset = index*in_width +j;
//                         if(!input->isNull(offset))
//                         {
//                            lastValue = (double)inputBuf[offset];
//                            break;
//                         }
//                      }
//                      if(ossim::isnan(lastValue) == false)
//                      {
//                         h[j] += lastValue *
//                                 theWeightTableY[theKernelHeight - l-1][indexMod];
                        
//                         ++count;
//                      }
//                      else
//                      {
//                         break;
//                      }
//                   }
               }
            }
            if(!count)
            {
               h[j] = ossim::nan();
            }
         }
         for (ossim_int32 m = 0; m < out_width; m++)
         {
            double x = 0.0;
            ossim_int32 indexMod = (ossim_int32)fmod((m+origin.x), theOutputToInputRatio.x);
            if(indexMod >= theTableWidthX) indexMod = theTableWidthX-1;
            if(indexMod <0) indexMod = 0;
            
            if(input->isNull(ossimIpt(Lx[m]-scaledOriginShift.x, Ly[k]-scaledOriginShift.y)))
            {
               outputBuf[k*out_width +m] = np;
            }
            else
            {
               for (l = 0; l < theKernelWidth; l++)
               {
                  ossim_int32 index = Lx[m] + l + kernelHorizontalShift;
                  if ((index >= 0) && (index < in_width))
                  {
                     if(ossim::isnan(h[index]) == false)
                     {
                        x += h[index] * theWeightTableX[theKernelWidth - l-1][indexMod];
                     }

                     // I am commenting out the code that searches for the next
                     // non null value with the kernel range.  This will have the
                     // same effect as multiplying by 0.0 for the NULL value.
                     //
//                      else
//                      {
//                         for(ossim_int32 templ = l; templ < l; ++templ)
//                         {
//                            ossim_int32 index = Lx[m] + templ + kernelHorizontalShift;
//                            if(ossim::isnan(h[index]) == false))
//                            {
//                               lastValue = h[index];
//                               break;
//                            }
//                         }
//                         if(ossim::isnan(lastValue) == false)
//                         {
//                            x += lastValue * theWeightTableX[theKernelWidth - l-1][indexMod];
//                         }
//                      }
                  }
               }
               if (x < minPix)
               {
                  outputBuf[k*out_width +m] = static_cast<T>(minPix);
               }
               else if (x > maxPix)
               {
                  outputBuf[k*out_width +m] = static_cast<T>(maxPix);
               }
               else
               {
                  outputBuf[k*out_width +m] = static_cast<T>(x);
               }
            }
         }
      }
   }
   
   delete [] h;
   delete [] Lx;   
   delete [] Ly;
}

template <class T>
void ossimResampler::resampleFullTile(T,// not used
                                      ossimImageData* input,
                                      ossimImageData* output)
{
   ossim_int32 maxInputSize  = std::max(input->getWidth(),  input->getHeight());
   ossim_int32 maxOutputSize = std::max(output->getWidth(), output->getHeight());
   ossim_int32 k;
   double *h     = new double[maxInputSize];
   ossim_int32 *Lx        = new ossim_int32[maxOutputSize];
   ossim_int32 *Ly        = new ossim_int32[maxOutputSize];
   ossim_int32 out_height = output->getHeight();
   ossim_int32 out_width  = output->getWidth();
   ossim_int32 in_width   = input->getWidth();
   ossim_int32 in_height  = input->getHeight();
   ossim_int32 j;
   ossim_int32 l;
   
   ossimIpt origin=output->getOrigin();
   ossimIpt scaledOriginShift(input->getOrigin()*-1);

   ossim_int32 evenKernelW = (ossim_int32)(!(theKernelWidth%2));
   ossim_int32 evenKernelH = (ossim_int32)(!(theKernelHeight%2));
   ossim_int32 kernelHorizontalShift = (ossim_int32)(-theKernelWidth/2.0+evenKernelW);
   ossim_int32 kernelVerticalShift   = (ossim_int32)(-theKernelHeight/2.0+evenKernelH);
   
   for (k = 0; k < maxOutputSize; k++)
   {
      Lx[k] = (ossim_int32)(scaledOriginShift.x+((k+origin.x)/
                                           theOutputToInputRatio.x));
   }
   for (k = 0; k < maxOutputSize; k++)
   {
      Ly[k] = (ossim_int32)(scaledOriginShift.y+((k+origin.y)/
                                           theOutputToInputRatio.y));
   }
   for(ossim_int32 band = 0; band < (ossim_int32)input->getNumberOfBands();++band)
   {
      const T* inputBuf  = (const T*)(input->getBuf(band));
      T*       outputBuf = (T*)(output->getBuf(band));
      double  minPix    = static_cast<T>(input->getMinPix()[band]);
      double  maxPix    = static_cast<T>(input->getMaxPix()[band]);
      
      for (k = 0; k < out_height; k++)
      {
         int indexMod = (int)fmod((k+origin.y), theOutputToInputRatio.y);
         if(indexMod >= theTableWidthY) indexMod = theTableWidthY - 1;
         if(indexMod <0) indexMod = 0;
         for (j = 0; j < in_width; j++)
         {
            h[j] = 0.0;
            for (l = 0; l < theKernelHeight; l++)
            {
               ossim_int32 index = Ly[k] + l + kernelVerticalShift;
               if ((index >= 0) && (index < in_height))
               {
                  h[j] += ((double)inputBuf[index*in_width +j]) *
                          theWeightTableY[theKernelHeight - l-1][indexMod];
               }
            }
         }
         for (ossim_int32 m = 0; m < out_width; m++)
         {
            double x = 0.0;
            int indexMod = (int)fmod((m+origin.x), theOutputToInputRatio.x);
            if(indexMod >= theTableWidthX) indexMod = theTableWidthX-1;
            if(indexMod <0) indexMod = 0;
            
            for (l = 0; l < theKernelWidth; l++)
            {
               ossim_int32 index = Lx[m] + l + kernelHorizontalShift;
               if ((index >= 0) && (index < in_width))
               {
         
                  x += h[index] * theWeightTableX[theKernelWidth - l-1][indexMod];
               }
            }
            if (x < minPix)
            {
               outputBuf[k*out_width +m] = static_cast<T>(minPix);
            }
            else if (x > maxPix)
            {
               outputBuf[k*out_width +m] = static_cast<T>(maxPix);
            }
            else
            {
               outputBuf[k*out_width +m] = static_cast<T>(x);
            }
         }
      }
   }
   
   delete [] h;
   delete [] Lx;   
   delete [] Ly;
}



template <class T>
void ossimResampler::resampleTile(T,// not used
                                  ossimImageData* input,
                                  ossimImageData* output,
                                  const ossimIrect& outputSubRect,
                                  const ossimDpt& inputUl,
                                  const ossimDpt& inputUr,
                                  const ossimDpt& deltaUl,
                                  const ossimDpt& deltaUr,
                                  const ossimDpt& outLength)
{
   ossimRefPtr<ossimImageData> dupIn = input;
   ossimDpt origin = input->getOrigin();
   ossimDpt newInputUl = inputUl;
   ossimDpt newInputUr = inputUr;
   ossimDpt newDeltaUl = deltaUl;
   ossimDpt newDeltaUr = deltaUr;

   if(theResamplerType != ossimResampler_NEAREST_NEIGHBOR)
   {
      newInputUl = ossimDpt(inputUl.x * theOutputToInputRatio.x,
                            inputUl.y * theOutputToInputRatio.y);
      newInputUr = ossimDpt(inputUr.x * theOutputToInputRatio.x,
                            inputUr.y * theOutputToInputRatio.y);
      newDeltaUl = ossimDpt(deltaUl.x * theOutputToInputRatio.x,
                            deltaUl.y * theOutputToInputRatio.y);
      newDeltaUr = ossimDpt(deltaUr.x * theOutputToInputRatio.x,
                            deltaUr.y * theOutputToInputRatio.y);

      ossimDpt newLl = newInputUl + newDeltaUl*outLength.y;
      ossimDpt newLr = newInputUr + newDeltaUr*outLength.y;
      
      ossimDrect newBoundingRect(newInputUl,
                                 newInputUr,
                                 newLl,
                                 newLr);
      

      newBoundingRect = ossimDrect(newBoundingRect.ul() - ossimDpt(theKernelWidth, theKernelHeight),
                                   newBoundingRect.lr() + ossimDpt(theKernelWidth, theKernelHeight));
      ossimIrect roundedRect = newBoundingRect;
      origin = roundedRect.ul();

      dupIn = new ossimImageData(NULL,
                                 input->getScalarType(),
                                 input->getNumberOfBands(),
                                 roundedRect.width(),
                                 roundedRect.height());
      dupIn->setOrigin(roundedRect.ul());
      dupIn->initialize();
      resampleTile(static_cast<T>(0), input, dupIn.get());
   }
   long    inWidth        = dupIn->getWidth();
   long    inHeight       = dupIn->getHeight();
   double  stepSizeWidth  = 1.0/outLength.x;
   double  stepSizeHeight = 1.0/outLength.y;   
   ossimIrect rect = dupIn->getImageRectangle();
   
   ossimDpt startSave(newInputUl.x - rect.ul().x,
                      newInputUl.y - rect.ul().y );
   ossimDpt endSave(newInputUr.x - rect.ul().x,
                    newInputUr.y - rect.ul().y);

   ossimIrect outputRect = output->getImageRectangle();
   ossimIpt subRectUl = outputSubRect.ul();
   long     subRectH  = outputSubRect.height();
   long     subRectW  = outputSubRect.width();
   
   ossimIpt outputRectUl    = outputRect.ul();
   long outputRectW         = outputRect.width();
   long resultOffset = (subRectUl.y - outputRectUl.y)*outputRectW + (subRectUl.x - outputRectUl.x);

   for(ossim_uint32 band = 0; band < input->getNumberOfBands(); band++)
   {
      T* resultBuf       = static_cast<T*>(output->getBuf(band))+resultOffset;
      const T *sourceBuf = static_cast<T*>(dupIn->getBuf(band));
      ossimDpt start = startSave;
      ossimDpt end   = endSave;
      T np = (T)output->getNullPix(band);
                        
      for(long y = 0; y < subRectH; y++)
      {
         double deltaX = (end.x - start.x)*stepSizeWidth;
         double deltaY = (end.y - start.y)*stepSizeHeight;
         ossimDpt pointXY = start;
         
         for(long x = 0; x < subRectW; x++)
         {
            int xPixel = pointXY.x<0?(int)floor(pointXY.x):(int)pointXY.x;
            int yPixel = pointXY.y<0?(int)floor(pointXY.y):(int)pointXY.y;

            
            if( (xPixel >=0) && (xPixel < inWidth) &&
                (yPixel >=0) && (yPixel < inHeight))
            {
               resultBuf[x] = sourceBuf[yPixel*inWidth + xPixel];
            }
            else
            {
               resultBuf[x] = np;
            }
            
            pointXY.y += deltaY;
            pointXY.x += deltaX;
         }
         resultBuf += outputRectW;
         
         start.x += newDeltaUl.x;
         start.y += newDeltaUl.y;
         end.x   += newDeltaUr.x;
         end.y   += newDeltaUr.y;
      }
   }
   dupIn = 0;
}

template <class T>
void ossimResampler::resampleTileNearestNeighbor(T, // dummy template variable
                                                 ossimImageData* input,
                                                 ossimImageData* output,
                                                 const ossimDpt& ul,
                                                 const ossimDpt& ur,
                                                 const ossimDpt& deltaUl,
                                                 const ossimDpt& deltaUr,
                                                 const ossimDpt& length)
{
   resampleTileNearestNeighbor((T)0,
                               input,
                               output,
                               output->getImageRectangle(),
                               ul,
                               ur,
                               deltaUl,
                               deltaUr,
                               length);
}

template <class T>
void ossimResampler::resampleTileNearestNeighbor(T, // dummy template variable
                                                 ossimImageData* input,
                                                 ossimImageData* output,
                                                 const ossimIrect& outputSubRect,
                                                 const ossimDpt& ul,
                                                 const ossimDpt& ur,
                                                 const ossimDpt& deltaUl,
                                                 const ossimDpt& deltaUr,
                                                 const ossimDpt& length)
{   
   long    inWidth        = input->getWidth();
   long    inHeight       = input->getHeight();
   double  stepSizeWidth  = 1.0/length.x;
   double  stepSizeHeight = 1.0/length.y;
   ossimIrect rect        = input->getImageRectangle();
   
   ossimDpt startSave(ul.x - rect.ul().x,
                      ul.y - rect.ul().y );
   
   ossimDpt endSave(ur.x - rect.ul().x,
                    ur.y - rect.ul().y);

   ossimIrect outputRect = output->getImageRectangle();
   ossimIpt subRectUl = outputSubRect.ul();
//   ossimIpt subRectUl((int)outputSubRect.ul().x,
//                      (int)outputSubRect.ul().y);
   long     subRectH  = outputSubRect.height();
   long     subRectW  = outputSubRect.width();
   
   ossimIpt outputRectUl    = outputRect.ul();
   long outputRectW         = outputRect.width();

   long resultOffset = (subRectUl.y - outputRectUl.y)*outputRectW + (subRectUl.x - outputRectUl.x);
   for(ossim_uint32 band = 0; band < input->getNumberOfBands(); band++)
   {
      T* resultBuf       = static_cast<T*>(output->getBuf(band))+resultOffset;
      const T *sourceBuf = static_cast<T*>(input->getBuf(band));
      ossimDpt start = startSave;
      ossimDpt end   = endSave;
      T inNp  = (T)input->getNullPix(band);
      T outNp = (T)output->getNullPix(band);
                        
      for(long y = 0; y < subRectH; y++)
      {
         double deltaX = (end.x - start.x)*stepSizeWidth;
         double deltaY = (end.y - start.y)*stepSizeHeight;
         ossimDpt pointXY = start;
         
         for(long x = 0; x < subRectW; x++)
         {
            int xPixel = pointXY.x<0?(int)floor(pointXY.x):(int)pointXY.x;
            int yPixel = pointXY.y<0?(int)floor(pointXY.y):(int)pointXY.y;

            
             if( (xPixel >=0) && (xPixel < inWidth) &&
                 (yPixel >=0) && (yPixel < inHeight))
             {
                T value = sourceBuf[yPixel*inWidth + xPixel];

                if(value != inNp)
                {
                   resultBuf[x] = value;
                }
                else
                {
                   resultBuf[x] = outNp;
                }
            }
            else
            {
               resultBuf[x] = outNp;
            }
            
            pointXY.y += deltaY;
            pointXY.x += deltaX;
         }
         resultBuf += outputRectW;
         
         start.x += deltaUl.x;
         start.y += deltaUl.y;
         end.x   += deltaUr.x;
         end.y   += deltaUr.y;
      }
   }
}

ossim_int32 ossimResampler::getKernelWidth()const
{
   return theKernelWidth;
}

ossim_int32 ossimResampler::getKernelHeight()const
{
   return theKernelHeight;
}

void ossimResampler::deleteWeightTable()
{
   if(theWeightTableX)
   {
      for(ossim_int32 index = 0; index < theTableHeight; ++index)
      {
         delete [] theWeightTableX[index];
      }
      delete [] theWeightTableX;

      theWeightTableX = NULL;
   }
   if(theWeightTableY)
   {
      for(ossim_int32 index = 0; index < theTableHeight; ++index)
      {
         delete [] theWeightTableY[index];
      }
      delete [] theWeightTableY;

      theWeightTableY = NULL;
   }
}

void ossimResampler::allocateWeightTable()//uint32 outWidth)
{
   if(theWeightTableX||
      theWeightTableY)
   {
      deleteWeightTable();
   }
   
   switch(theResamplerType)
   {
   case ossimResampler_BICUBIC:
   {
      theTableHeight = 4;
      break;
   }
   case ossimResampler_BILINEAR:
   {
      theTableHeight = 2;
      break;
   }
   case ossimResampler_NEAREST_NEIGHBOR:
   {
      theTableHeight = 1;
      break;
   }
   case ossimResampler_NONE:
     {
       theResamplerType = ossimResampler_NEAREST_NEIGHBOR;
      theTableHeight = 1;
      break;
     }
   }
   theTableWidthX = (ossim_int32)ossim::round<int>(theOutputToInputRatio.x);
   theTableWidthY = (ossim_int32)ossim::round<int>(theOutputToInputRatio.y);
   if(theTableWidthX&&theTableHeight)
   {
      theWeightTableX = new double*[theTableHeight];
      
      for(ossim_int32 index = 0; index < theTableHeight; ++index)
      {
         theWeightTableX[index] = new double[theTableWidthX];
      }
   }
   if(theTableWidthY&&theTableHeight)
   {
      theWeightTableY = new double*[theTableHeight];
      
      for(ossim_int32 index = 0; index < theTableHeight; ++index)
      {
         theWeightTableY[index] = new double[theTableWidthY];
      }
   }
}

void ossimResampler::generateWeightTable()
{
   if(theWeightTableX&&
      theWeightTableY)
   {
//      ossim_int32 d = theOutputToInputRatio.theDen;
//      ossim_int32 n = theOutputToInputRatio.theNum;
      ossim_int32 i = 0;
      
      double x = 0.0;
      
      switch(theResamplerType)
      {
   case ossimResampler_NONE:
     {
       theResamplerType = ossimResampler_NEAREST_NEIGHBOR;
       for (i = 0; i < theTableWidthY; i++)
         {
	   theWeightTableY[0][i] = 1;
         }
       for (i = 0; i < theTableWidthX; i++)
         {
	   theWeightTableX[0][i] = 1;
         }
      break;
     }
      case ossimResampler_NEAREST_NEIGHBOR:
      {         
         for (i = 0; i < theTableWidthY; i++)
         {
            theWeightTableY[0][i] = 1;
         }
         for (i = 0; i < theTableWidthX; i++)
         {
            theWeightTableX[0][i] = 1;
         }
         break;
      }
      case ossimResampler_BILINEAR:
      {
         for (i = 0; i < theTableWidthX; i++)
         {
            x = (double)i/(double)(theTableWidthX);
            theWeightTableX[0][i] = x;
            theWeightTableX[1][i] = 1-x;
         }
         for (i = 0; i < theTableWidthY; i++)
         {
            x = (double)i/(double)(theTableWidthY);
            theWeightTableY[0][i] = x;
            theWeightTableY[1][i] = 1-x;
         }
         break;
      }
      case ossimResampler_BICUBIC:
      {         
         for (i = 0; i < theTableWidthX; i++)
         {
            x = (double)i/(double)(theTableWidthX);
            theWeightTableX[0][i] = getCubicC0(x);
            theWeightTableX[1][i] = getCubicC1(x);
            theWeightTableX[2][i] = getCubicC2(x);
            theWeightTableX[3][i] = getCubicC3(x);               
         }
         for (i = 0; i < theTableWidthY; i++)
         {
            x = (double)i/(double)(theTableWidthY);
            theWeightTableY[0][i] = getCubicC0(x);
            theWeightTableY[1][i] = getCubicC1(x);
            theWeightTableY[2][i] = getCubicC2(x);
            theWeightTableY[3][i] = getCubicC3(x);               
         }
         break;
      }
      }
   }
}

void ossimResampler::setResamplerType(ossimResLevelResamplerType type)
{
   if(theResamplerType != type)
   {
      theResamplerType = type;
      
      switch(theResamplerType)
      {
      case ossimResampler_NONE:
      {
	theResamplerType = ossimResampler_NEAREST_NEIGHBOR;
         theKernelWidth  = 1;
         theKernelHeight = 1;

         break;
      }
      case ossimResampler_NEAREST_NEIGHBOR:
      {
         theKernelWidth  = 1;
         theKernelHeight = 1;

         break;
      }
      case ossimResampler_BILINEAR:
      {
         theKernelWidth  = 2;
         theKernelHeight = 2;

         break;
      }
      case ossimResampler_BICUBIC:
      {
         theKernelWidth  = 4;
         theKernelHeight = 4;
         
         break;
      }
      }
      
      allocateWeightTable();
      generateWeightTable();
   }
}

void ossimResampler::setRatio(double outputToInputRatio)
{
   // make it square
   setRatio(ossimDpt(outputToInputRatio, outputToInputRatio));
}

void ossimResampler::setRatio(const ossimDpt& outputToInputRatio)
{
   // make it square
   theOutputToInputRatio.x = (outputToInputRatio.x);
   theOutputToInputRatio.y = (outputToInputRatio.y);

   if((theTableWidthX != ossim::round<int>(outputToInputRatio.x))||
      (theTableWidthY != ossim::round<int>(outputToInputRatio.y)))
   {
      allocateWeightTable();
      generateWeightTable();
   }
}

double ossimResampler::getCubicC0(double t)const
{
   return ((-theCubicAdjustableParameter * t * t * t) +
           (theCubicAdjustableParameter * t * t));
}

double ossimResampler::getCubicC1(double t)const
{
   return (-(theCubicAdjustableParameter + 2.0) * t * t * t +
           (2.0 * theCubicAdjustableParameter + 3.0) * t * t -
           theCubicAdjustableParameter * t);
}

double ossimResampler::getCubicC2(double t)const
{
   return ((theCubicAdjustableParameter + 2.0) * t * t * t -
           (theCubicAdjustableParameter + 3.0) * t * t + 1.0);
}

double ossimResampler::getCubicC3(double t)const
{
   return ((theCubicAdjustableParameter * t * t * t) -
           (2.0f * theCubicAdjustableParameter * t * t) +
           (theCubicAdjustableParameter * t));
}

bool ossimResampler::loadState(const ossimKeywordlist& kwl,
                               const char* prefix)
{   
   const char* resamplerType = kwl.find(prefix, RESAMPLER_CONVOLUTION_TYPE_KW);
   const char* scaleX        = kwl.find(prefix, RESAMPLER_SCALE_X_KW);
   const char* scaleY        = kwl.find(prefix, RESAMPLER_SCALE_Y_KW);
   const char* cubicParameter= kwl.find(prefix, RESAMPLER_CUBIC_PARAMETER_KW);
   
   if(cubicParameter)
   {
      theCubicAdjustableParameter = ossimString(cubicParameter).toDouble();
      if(theCubicAdjustableParameter < -1) theCubicAdjustableParameter = -1;
      if(theCubicAdjustableParameter > 0)  theCubicAdjustableParameter = 0;
   }
   else
   {
      theCubicAdjustableParameter = -.5;
   }
   if(resamplerType)
   {
      ossimString test =ossimString(resamplerType).upcase().trim();
      
      if( test == "BICUBIC")
      {
         setResamplerType(ossimResampler::ossimResampler_BICUBIC);
      }
      else if( test == "BILINEAR")
      {
         setResamplerType(ossimResampler::ossimResampler_BILINEAR);
      }
      else
      {
         setResamplerType(ossimResampler::ossimResampler_NEAREST_NEIGHBOR);
      }
   }
   if(scaleX&&scaleY)
   {
      setRatio(ossimDpt(ossimString(scaleX).toDouble(),
                        ossimString(scaleY).toDouble()));
   }
   allocateWeightTable();
   generateWeightTable();
   
   return ossimConnectableObject::loadState(kwl, prefix);
}

bool ossimResampler::saveState(ossimKeywordlist& kwl,
                               const char* prefix)const
{
   ossimString resamplerType;
   if(getResamplerType() == ossimResampler_BICUBIC)
   {
      resamplerType = "BICUBIC";
   }
   else if(getResamplerType() == ossimResampler_BILINEAR)
   {
      resamplerType = "BILINEAR";
   }
   else 
   {
      resamplerType = "NEAREST_NEIGHBOR";
   }
   kwl.add(prefix,
           RESAMPLER_CONVOLUTION_TYPE_KW,
           resamplerType.c_str(),
           true);

   kwl.add(prefix,
           RESAMPLER_SCALE_X_KW,
           theOutputToInputRatio.x,
           true);

   kwl.add(prefix,
           RESAMPLER_SCALE_Y_KW,
           theOutputToInputRatio.y,
           true);

   kwl.add(prefix,
           RESAMPLER_CUBIC_PARAMETER_KW,
           theCubicAdjustableParameter,
           true);
   
   return ossimConnectableObject::saveState(kwl, prefix);
}
