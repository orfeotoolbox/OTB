//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts

// Contributor:
//         David A. Horner (DAH) http://dave.thehorners.com
//
//*************************************************************************
// $Id: ossimFilterResampler.cpp 20326 2011-12-07 13:48:18Z dburken $

#include <ossim/imaging/ossimFilterResampler.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/imaging/ossimFilterTable.h>
ossimFilterResampler::ossimFilterResampler()
   :theMinifyFilter(new ossimNearestNeighborFilter()),
    theMagnifyFilter(new ossimNearestNeighborFilter()),
    theMinifyFilterType(ossimFilterResampler_NEAREST_NEIGHBOR),
    theMagnifyFilterType(ossimFilterResampler_NEAREST_NEIGHBOR),
    theScaleFactor(1.0, 1.0),
    theInverseScaleFactor(1.0, 1.0),
    theBlurFactor(1.0)
{
   setScaleFactor(ossimDpt(1.0, 1.0));
   loadState(ossimPreferences::instance()->preferencesKWL(),"resampler.");

}

ossimFilterResampler::~ossimFilterResampler()
{
   if(theMinifyFilter)
   {
      delete theMinifyFilter;
      theMinifyFilter = NULL;
   }
   if(theMagnifyFilter)
   {
      delete theMagnifyFilter;
      theMagnifyFilter = NULL;
   }
}


void ossimFilterResampler::resample(const ossimRefPtr<ossimImageData>& input,
				    ossimRefPtr<ossimImageData>& output,
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

void ossimFilterResampler::resample(const ossimRefPtr<ossimImageData>& input,
				    ossimRefPtr<ossimImageData>& output,
				    const ossimIrect& outputSubRect,
				    const ossimDpt& ul,
				    const ossimDpt& ur,
				    const ossimDpt& deltaUl,
				    const ossimDpt& deltaUr,
				    const ossimDpt& length)
{
   if(!input.valid()   ||
      !output.valid()  ||
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
         resampleBilinearTile(ossim_uint8(0), // dummy template variable
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
      case OSSIM_SINT8:
      {
         resampleBilinearTile(ossim_sint8(0), // dummy template variable
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
	 resampleBilinearTile(ossim_uint16(0), // dummy template variable
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
	 resampleBilinearTile(ossim_sint16(0), // dummy template variable
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
	 resampleBilinearTile(ossim_uint32(0), // dummy template variable
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
      case OSSIM_SINT32:
      {
	 resampleBilinearTile(ossim_sint32(0), // dummy template variable
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
	 resampleBilinearTile(ossim_float32(0.0), // dummy template variable
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
	 resampleBilinearTile(ossim_float64(0.0), // dummy template variable
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
            << "ossimFilterResampler::resample error: unknown scalar type:  "
            << scalarType << endl;
      }
   }
}


ossimFilter* ossimFilterResampler::createNewFilter(
   ossimFilterResamplerType filterType,
   ossimFilterResamplerType& result)
{
   switch(filterType)
   {
      case ossimFilterResampler_NEAREST_NEIGHBOR:
      {
         return new ossimNearestNeighborFilter();
      }
      case ossimFilterResampler_BOX:
      {
         return new ossimBoxFilter();
      }
      case ossimFilterResampler_GAUSSIAN:
      {
         return new ossimGaussianFilter();
      }
      case ossimFilterResampler_CUBIC:
      {
         return new ossimCubicFilter();
      }
      case ossimFilterResampler_HANNING:
      {
         return new ossimHanningFilter();
      }
      case ossimFilterResampler_HAMMING:
      {
         return new ossimHammingFilter();
      }
      case ossimFilterResampler_LANCZOS:
      {
         return new ossimLanczosFilter();
      }
      case ossimFilterResampler_CATROM:
      {
         return new ossimCatromFilter();
      }
      case ossimFilterResampler_MITCHELL:
      {
         return new ossimMitchellFilter();
      }
      case ossimFilterResampler_BLACKMAN:
      {
         return new ossimBlackmanFilter();
      }
      case ossimFilterResampler_BLACKMAN_SINC:
      {
         return new ossimBlackmanSincFilter();
      }
      case ossimFilterResampler_BLACKMAN_BESSEL:
      {
         return new ossimBlackmanBesselFilter();
      }
      case ossimFilterResampler_QUADRATIC:
      {
         return new ossimQuadraticFilter();
      }
      case ossimFilterResampler_TRIANGLE:
      {
         return new ossimTriangleFilter();
      }
      case ossimFilterResampler_HERMITE:
      {
         return new ossimHermiteFilter();
      }
      case ossimFilterResampler_BELL:
      {
         return new ossimGaussianFilter();
      }
      case ossimFilterResampler_BSPLINE:
      {
         return new ossimBSplineFilter();
      }
   }

   result = ossimFilterResampler_NEAREST_NEIGHBOR;
   return new ossimNearestNeighborFilter();
}

void ossimFilterResampler::setScaleFactor(const ossimDpt& scale)
{
   theScaleFactor = scale;
   if(fabs(theScaleFactor.x) <= FLT_EPSILON)
   {
      theScaleFactor.x = 1.0;
   }
   if(fabs(theScaleFactor.y) <= FLT_EPSILON)
   {
      theScaleFactor.y = 1.0;
   }

   theInverseScaleFactor.x = 1.0/theScaleFactor.x;
   theInverseScaleFactor.y = 1.0/theScaleFactor.y;
}

template <class T> void ossimFilterResampler::resampleBilinearTile(
   T /* dummy */,
   const ossimRefPtr<ossimImageData>& input,
   ossimRefPtr<ossimImageData>& output,
   const ossimIrect& outputSubRect,
   const ossimDpt& inputUl,
   const ossimDpt& inputUr,
   const ossimDpt& deltaUl,
   const ossimDpt& deltaUr,
   const ossimDpt& outLength)
{
#if 0 /* Please leave for debug. */
   std::cout << "INPUT  = \n" << *input << std::endl
             << "OUTPUT = \n" << *output << std::endl
             << "inputUL= " << inputUl << std::endl
             << "inputUR= " << inputUr << std::endl
             << "deltaUL= " << deltaUl << std::endl
             << "deltaUr= " << deltaUr << std::endl
             << "outlength= " << outLength << std::endl;
#endif
   
   ossim_uint32  band, centerOffset;
   ossim_float64 tmpFlt64, stepSizeWidth;

   if(outLength.x>1) {
      stepSizeWidth  = 1.0/(outLength.x-1.0);
   } else {
      stepSizeWidth   = 1.0;
   }

   // INPUT INFORMATION
   ossim_uint32       inWidth      = input->getWidth();
   ossim_uint32       inBandSize   = input->getSizePerBand();  // fix for out-of-bounds check OLK 06/2005
   ossim_uint32       BANDS        = input->getNumberOfBands();
   ossimIrect         inputRect    = input->getImageRectangle();

   // OUTPUT INFORMATION
   const ossim_float64* NULL_PIX      = output->getNullPix(); 
   const ossim_float64* MIN_PIX       = output->getMinPix(); 
   const ossim_float64* MAX_PIX       = output->getMaxPix(); 
   ossimIrect           outputRect    = output->getImageRectangle();
   ossim_uint32         resultRectH   = outputSubRect.height();
   ossim_uint32         resultRectW   = outputSubRect.width();
   ossim_uint32         outputRectW   = outputRect.width();

   // calculate the offset into the data so we can refer to it at 0 index.
   ossim_uint32 resultOffset=(outputSubRect.ul().y - outputRect.ul().y)*outputRectW +
                             (outputSubRect.ul().x - outputRect.ul().x);

   // make a local copy of the band pointers (at resultOffset)
   ossim_float64 *densityvals=new ossim_float64[BANDS];
   ossim_float64 *pixelvals=new ossim_float64[BANDS];
   const T* *inputBuf  = new const T*[BANDS];
   T* *resultBuf = new T*[BANDS];
   if(!pixelvals||!inputBuf||!resultBuf)
   {
      return;
   }
   
   for(band = 0; band < BANDS; ++band)
   {
      inputBuf[band] = static_cast<const T*>(input->getBuf(band));
      resultBuf[band] = static_cast<T*>(output->getBuf(band))+resultOffset;
   }

   // FILTER INFORMAION
   ossim_uint32 xkernel_width  = theFilterTable.getWidth();;
   ossim_uint32 ykernel_height = theFilterTable.getHeight();
   double xkernel_half_width   = theFilterTable.getXSupport();
   double ykernel_half_height  = theFilterTable.getYSupport();

   double initialx  = inputUl.x-inputRect.ul().x;
   double initialy  = inputUl.y-inputRect.ul().y;
   double terminalx = inputUr.x-inputRect.ul().x;
   double terminaly = inputUr.y-inputRect.ul().y;
   double pointx,pointy,deltaX,deltaY;
   ossim_int32 starty,startx; 

   if(xkernel_width==0 || ykernel_height==0)
   {
      // USING NEAREST NEIGHBOR
      for(ossim_uint32 resultY = 0; resultY < resultRectH; ++resultY)
      {
//          deltaX = (terminalx-initialx) * stepSizeWidth;
//          deltaY = (terminaly-initialy) * stepSizeHeight;
         // this should be stepsize width for both since we are traversing horizontal
         deltaX = (terminalx-initialx) * stepSizeWidth;
         deltaY = (terminaly-initialy) * stepSizeWidth;
         pointx = initialx;
         pointy = initialy;
         for(ossim_uint32 resultX = 0; resultX < resultRectW; ++resultX)
         {
            // just sample center in input space.
            centerOffset = ossim::round<int>(pointy)*inWidth + ossim::round<int>(pointx);
            for(band=0;band<BANDS;++band)
            {
               resultBuf[band][resultX] = inputBuf[band][centerOffset];
            }
            pointy += deltaY;
            pointx += deltaX;
         } // End of loop in x direction.
         
         // increment pointers to where we are now.
         for(band=0;band<BANDS;++band)
         {
            resultBuf[band] += outputRectW;
         }
         initialx += deltaUl.x;
         initialy += deltaUl.y;
         terminalx  += deltaUr.x;
         terminaly  += deltaUr.y;
      } // End of loop in y direction.
      
   }
   else
   {
      // USING A KERNEL
      const double* kernel;
      ossim_uint32 iy,ix,sourceIndex,nullCount;
      for(ossim_uint32 resultY = 0; resultY < resultRectH; ++resultY)
      {
         deltaX = (terminalx-initialx) * stepSizeWidth;
         deltaY = (terminaly-initialy) * stepSizeWidth;
         pointx = initialx;
         pointy = initialy;
         for(ossim_uint32 resultX = 0; resultX < resultRectW; ++resultX)
         {
            starty  = ossim::round<int>(pointy - ykernel_half_height + .5);
            startx  = ossim::round<int>(pointx - xkernel_half_width + .5);
            centerOffset = ossim::round<int>(pointy)*inWidth + ossim::round<int>(pointx);
            sourceIndex = starty*inWidth+startx;

            // look at center pixel, make sure they aren't all null.
            nullCount=0;
            if(centerOffset<inBandSize)
            {
               for (band=0;band<BANDS;++band)
               {
                  if(inputBuf[band][centerOffset]==static_cast<T>(NULL_PIX[band]))
                  {
                     ++nullCount;
                  }
               }
               // the center of the kernel is outside the input space, just set null.
            }
            else
            {
               nullCount=BANDS;
            }

            // make sure we have non-null data and we fit within the inputBuf.
            if ( nullCount==BANDS || (sourceIndex>=inBandSize))
            {
               // we don't need to continue, just assign null!
               for (band=0;band<BANDS;++band)
               {
                  resultBuf[band][resultX] = static_cast<T>(NULL_PIX[band]);
               }
            }
            else
            {  
               kernel = theFilterTable.getClosestWeights(pointx,pointy);
               if(kernel)
               {
                  // reset the pixel/density sums for each band to zero.
                  memset(densityvals,'\0',sizeof(ossim_float64)*BANDS);
                  memset(pixelvals,'\0',sizeof(ossim_float64)*BANDS);

                  // apply kernel to input space.
                  for (iy=0;((iy<ykernel_height)&&(sourceIndex<inBandSize));++iy)
                  {
                     for (ix = 0;((ix<xkernel_width)&&(sourceIndex<inBandSize));++ix)
                     {
                        tmpFlt64=*kernel; // pixel weight;
                        for(band=0;band<BANDS;++band)
                        {
                           if(inputBuf[band][sourceIndex]!=NULL_PIX[band])
                           {
                              densityvals[band] += tmpFlt64;
                              pixelvals[band] += (inputBuf[band][sourceIndex]*tmpFlt64);
                           }
                        }
                        ++sourceIndex;
                        ++kernel;
                        if(sourceIndex>=inBandSize)
                        {
                           break;
                        }
                     }
                     sourceIndex+=(inWidth-xkernel_width);
                  }

                  // actually assign the value to the output
                  for (band = 0; band < BANDS; ++band)
                  {
                     if(densityvals[band]<=FLT_EPSILON)
                     {
                        //---
                        // Setting tempFlt64 to pixelvals[band] causing 0's where -32768
                        // should be when null check was skipped above.
                        // tmpFlt64 = pixelvals[band];
                        //---
                        tmpFlt64 = NULL_PIX[band];
                     }
                     else
                     {
                        // normalize
                        tmpFlt64 = pixelvals[band]/densityvals[band];
                     }
                     
                     // clamp
                     tmpFlt64 = (tmpFlt64>=MIN_PIX[band]?(tmpFlt64<MAX_PIX[band]?tmpFlt64:MAX_PIX[band]):MIN_PIX[band]); 
                     // set resultant pixel value.
                     resultBuf[band][resultX] = static_cast<T>(tmpFlt64);
                  }

                  // we didn't get a filter kernel, just set NULL in this disaster.
               }
               else
               {
                  for (band=0;band<BANDS;++band)
                  {
                     resultBuf[band][resultX] = static_cast<T>(NULL_PIX[band]);
                  }
               }                  
            }
            pointy += deltaY;
            pointx += deltaX;
         } // End of loop in x direction.

         // increment pointers to where we are now.
         for(band=0;band<BANDS;++band)
         {
            resultBuf[band] += outputRectW;
         }
         initialx += deltaUl.x;
         initialy += deltaUl.y;
         terminalx  += deltaUr.x;
         terminaly  += deltaUr.y;
      } // End of loop in y direction.
   } // USING A KERNEL END

   delete [] densityvals;
   delete [] pixelvals;
   delete [] resultBuf;
   delete [] inputBuf;
}

ossimString ossimFilterResampler::getFilterTypeAsString(ossimFilterResamplerType type)const
{
   switch(type)
   {
      case ossimFilterResampler_NEAREST_NEIGHBOR:
      {
         return "nearest neighbor";
      }
      case ossimFilterResampler_BOX:
      {
         return "box";
      }
      case ossimFilterResampler_GAUSSIAN:
      {
         return "gaussian";
      }
      case ossimFilterResampler_CUBIC:
      {
         return "cubic";
      }
      case ossimFilterResampler_HANNING:
      {
         return "hanning";
      }
      case ossimFilterResampler_HAMMING:
      {
         return "hamming";
      }
      case ossimFilterResampler_LANCZOS:
      {
         return "lanczos";
      }
      case ossimFilterResampler_MITCHELL:
      {
         return "mitchell";
      }
      case ossimFilterResampler_CATROM:
      {
         return "catrom";
      }
      case ossimFilterResampler_BLACKMAN:
      {
         return "blackman";
      }
      case ossimFilterResampler_BLACKMAN_SINC:
      {
         return "sinc";
      }
      case ossimFilterResampler_BLACKMAN_BESSEL:
      {
         return "bessel";
      }
      case ossimFilterResampler_QUADRATIC:
      {
         return "quadratic";
      }
      case ossimFilterResampler_TRIANGLE:
      {
         return "bilinear";
      }
      case ossimFilterResampler_HERMITE:
      {
         return "hermite";
      }
      case ossimFilterResampler_BELL:
      {
         return "gaussian";
      }
      case ossimFilterResampler_BSPLINE:
      {
         return "bspline";
      }
   }

   return "nearest neighbor";
}

void ossimFilterResampler::getFilterTypes(std::vector<ossimString>& filterTypes)const
{
  filterTypes.push_back("nearest neighbor");
  filterTypes.push_back("bilinear");
  filterTypes.push_back("cubic");
//  filterTypes.push_back("bell");
  filterTypes.push_back("bessel");
  filterTypes.push_back("blackman");
  filterTypes.push_back("box");
  filterTypes.push_back("bspline");
  filterTypes.push_back("catrom");
  filterTypes.push_back("gaussian");
  filterTypes.push_back("hanning");
  filterTypes.push_back("hamming");
  filterTypes.push_back("hermite");
  filterTypes.push_back("lanczos");
  filterTypes.push_back("mitchell");
  filterTypes.push_back("quadratic");
  filterTypes.push_back("sinc");
}


ossimFilterResampler::ossimFilterResamplerType ossimFilterResampler::getFilterType(const ossimString& type)const
{
   ossimString typeUpper = type;
   typeUpper = typeUpper.upcase();

   if(typeUpper.contains("BOX"))
   {
      return ossimFilterResampler_BOX;
   }
   else if(typeUpper.contains("NEAREST"))
   {
      return ossimFilterResampler_NEAREST_NEIGHBOR;
   }
   else if(typeUpper.contains("GAUSSIAN"))
   {
      return ossimFilterResampler_GAUSSIAN;
   }
   else if(typeUpper.contains("HANNING"))
   {
      return ossimFilterResampler_HANNING;
   }
   else if(typeUpper.contains("HAMMING"))
   {
      return ossimFilterResampler_HAMMING;
   }
   else if(typeUpper.contains("LANCZOS"))
   {
      return ossimFilterResampler_LANCZOS;
   }
   else if(typeUpper.contains("MITCHELL"))
   {
      return ossimFilterResampler_MITCHELL;
   }
   else if(typeUpper.contains("CATROM"))
   {
      return ossimFilterResampler_CATROM;
   }
   else if(typeUpper.contains("CUBIC"))
   {
      return ossimFilterResampler_CUBIC;
   }
   else if(typeUpper.contains("BESSEL"))
   {
      return ossimFilterResampler_BLACKMAN_BESSEL;
   }
   else if(typeUpper.contains("SINC"))
   {
      return ossimFilterResampler_BLACKMAN_SINC;
   }
   else if(typeUpper.contains("BLACKMAN"))
   {
      return ossimFilterResampler_BLACKMAN;
   }
   else if(typeUpper.contains("QUADRATIC"))
   {
      return ossimFilterResampler_QUADRATIC;
   }
   else if(typeUpper.contains("TRIANGLE"))
   {
      return ossimFilterResampler_TRIANGLE;
   }
   else if(typeUpper.contains("BILINEAR"))
   {
      return ossimFilterResampler_TRIANGLE;
   }
   else if(typeUpper.contains("HERMITE"))
   {
      return ossimFilterResampler_HERMITE;
   }
//    else if(typeUpper.contains("BELL"))
//    {
//       return ossimFilterResampler_BELL;
//    }
   else if(typeUpper.contains("BSPLINE"))
   {
      return ossimFilterResampler_BSPLINE;
   }
   
   return ossimFilterResampler_NEAREST_NEIGHBOR;
}

void ossimFilterResampler::getKernelSupport(double& x, double& y)const
{
   const ossimFilter* horizontalFilter = getHorizontalFilter();
   const ossimFilter* verticalFilter   = getVerticalFilter();

   if(!horizontalFilter)
   {
      x = 0.0;
   }
   else
   {
//       x = theBlurFactor*ossim::max(1.0/theScaleFactor.x, 1.0)*
//           horizontalFilter->getSupport();
      x = horizontalFilter->getSupport();
   }

   if(!verticalFilter)
   {
      y = 0.0;
   }
   else
   {
//       y = theBlurFactor*ossim::max(1.0/theScaleFactor.y, 1.0)*
//           verticalFilter->getSupport();
      y = verticalFilter->getSupport();
   }
}

const ossimFilter* ossimFilterResampler::getHorizontalFilter()const
{
   if(theScaleFactor.x < 1)
   {
      return theMinifyFilter;
   }

   return theMagnifyFilter;
}

const ossimFilter* ossimFilterResampler::getVerticalFilter()const
{
   if(theScaleFactor.y < 1)
   {
      return theMinifyFilter;
   }

   return theMagnifyFilter;
}

void ossimFilterResampler::setFilterType(const ossimString& type)
{
   setFilterType(type, type);
}

void ossimFilterResampler::setFilterType(ossimFilterResamplerType filterType)
{
   setFilterType(filterType, filterType);
}
void ossimFilterResampler::setFilterType(const ossimString& minifyType,
                                         const ossimString& magnifyType)
{
   setFilterType(getFilterType(minifyType),
                 getFilterType(magnifyType));
}

void ossimFilterResampler::setMinifyFilterType(const ossimString& minifyType)
{
   setMinifyFilterType(getFilterType(minifyType));
}

void ossimFilterResampler::setMagnifyFilterType(const ossimString& magnifyType)
{
   setMagnifyFilterType(getFilterType(magnifyType));
}

void ossimFilterResampler::setMinifyFilterType(ossimFilterResamplerType filterType)
{
   setFilterType(filterType,
                 theMagnifyFilterType);
}

void ossimFilterResampler::setMagnifyFilterType(ossimFilterResamplerType filterType)
{
   setFilterType(theMinifyFilterType,filterType);
}

ossimString ossimFilterResampler::getMinifyFilterTypeAsString()const
{
   return getFilterTypeAsString(theMinifyFilterType);
}

ossimString ossimFilterResampler::getMagnifyFilterTypeAsString()const
{
   return getFilterTypeAsString(theMagnifyFilterType);
}

void ossimFilterResampler::setFilterType(
   ossimFilterResamplerType minifyFilterType,
   ossimFilterResamplerType magnifyFilterType)
{
   if(theMinifyFilter)
   {
      delete theMinifyFilter;
      theMinifyFilter = NULL;
   }
   if(theMagnifyFilter)
   {
      delete theMagnifyFilter;
      theMagnifyFilter = NULL;
   }
   
   theMinifyFilterType  = minifyFilterType;
   theMagnifyFilterType = magnifyFilterType;
   
   theMinifyFilter  = createNewFilter(minifyFilterType, theMinifyFilterType);
   theMagnifyFilter = createNewFilter(magnifyFilterType, theMagnifyFilterType);
   computeTable();
}

ossim_float64 ossimFilterResampler::getBlurFactor()const
{
   return theBlurFactor;
}

void ossimFilterResampler::setBlurFactor(ossim_float64 blur)
{
   theBlurFactor = blur;
}

bool ossimFilterResampler::saveState(ossimKeywordlist& kwl,
                                     const char* prefix)const
{
   kwl.add(prefix,
           ossimKeywordNames::SCALE_X_KW,
           theScaleFactor.x,
           true);
   kwl.add(prefix,
           ossimKeywordNames::SCALE_Y_KW,
           theScaleFactor.y,
           true);
   kwl.add(prefix,
           "minify_type",
           getFilterTypeAsString(theMinifyFilterType),
           true);
   kwl.add(prefix,
           "magnify_type",
           getFilterTypeAsString(theMagnifyFilterType),
           true);

   return true;
}

bool ossimFilterResampler::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   const char* lookup = 0;

   lookup = kwl.find(prefix, ossimKeywordNames::SCALE_X_KW);
   if (lookup)
   {
      theScaleFactor.x = ossimString(lookup).toDouble();
   }

   lookup = kwl.find(prefix, ossimKeywordNames::SCALE_Y_KW);
   if (lookup)
   {
      theScaleFactor.y = ossimString(lookup).toDouble();
   }

   ossimString minify;
   lookup = kwl.find(prefix, "minify_type");
   if (lookup)
   {
      minify = lookup;
   }

   ossimString magnify;
   lookup = kwl.find(prefix, "magnify_type");
   if (lookup)
   {
      magnify = lookup;
   }

   if(fabs(theScaleFactor.x) <= FLT_EPSILON)
   {
      theScaleFactor.x = 1.0;
   }
   if(fabs(theScaleFactor.y) <= FLT_EPSILON)
   {
      theScaleFactor.y = 1.0;
   }

   theInverseScaleFactor.x = 1.0/theScaleFactor.x;
   theInverseScaleFactor.y = 1.0/theScaleFactor.y;
   
   setFilterType(getFilterType(minify),
                 getFilterType(magnify));

   return true;
}

void ossimFilterResampler::computeTable()
{
  theFilterTable.buildTable(32, *theMagnifyFilter);
}

