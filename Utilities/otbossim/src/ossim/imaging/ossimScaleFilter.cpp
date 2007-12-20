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
// $Id: ossimScaleFilter.cpp 11955 2007-10-31 16:10:22Z gpotts $
#include <ossim/imaging/ossimScaleFilter.h>
#include <ossim/imaging/ossimFilter.h>
#include <ossim/imaging/ossimDiscreteConvolutionKernel.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/base/ossimKeywordNames.h>

RTTI_DEF1(ossimScaleFilter, "ossimScaleFilter", ossimImageSourceFilter);

ossimScaleFilter::ossimScaleFilter()
   :ossimImageSourceFilter(),
    theBlankTile(NULL),
    theTile(NULL),
    theMinifyFilter(NULL),
    theMagnifyFilter(NULL),
    theMinifyFilterType(ossimScaleFilter_NEAREST_NEIGHBOR),
    theMagnifyFilterType(ossimScaleFilter_NEAREST_NEIGHBOR),
    theScaleFactor(1.0, 1.0),
    theInverseScaleFactor(1.0, 1.0),
    theTileSize(64, 64),
    theBlurFactor(1.0)
{
   theInputRect.makeNan();
   theMinifyFilter  = new ossimNearestNeighborFilter();
   theMagnifyFilter = new ossimNearestNeighborFilter();
}

ossimScaleFilter::ossimScaleFilter(ossimImageSource* inputSource,
                                   const ossimDpt& scaleFactor)
   :ossimImageSourceFilter(inputSource),
    theBlankTile(NULL),
    theTile(NULL),
    theMinifyFilter(NULL),
    theMagnifyFilter(NULL),
    theMinifyFilterType(ossimScaleFilter_NEAREST_NEIGHBOR),
    theMagnifyFilterType(ossimScaleFilter_NEAREST_NEIGHBOR),
    theScaleFactor(scaleFactor),
    theTileSize(64, 64),
    theBlurFactor(1.0)
{
   theInputRect.makeNan();
   theMinifyFilter  = new ossimNearestNeighborFilter();
   theMagnifyFilter = new ossimNearestNeighborFilter();
}

ossimScaleFilter::~ossimScaleFilter()
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

ossimRefPtr<ossimImageData> ossimScaleFilter::getTile(
   const ossimIrect& tileRect, ossim_uint32 resLevel)
{
   
   if((!isSourceEnabled())||
      (!theInputConnection)||
      ((theScaleFactor.x == 1.0)&&
       (theScaleFactor.y == 1.0)&&
       (theBlurFactor == 1.0)))
   {
      return ossimImageSourceFilter::getTile(tileRect, resLevel);
   }
   if(!theTile.valid())
   {
      allocate();
   }

   if(!theTile)
   {
      return ossimImageSourceFilter::getTile(tileRect, resLevel);
   }

   theTile->makeBlank();

                       
   ossimIrect imageRect = tileRect*theInverseScaleFactor;

   theTile->setImageRectangle(tileRect);
   theBlankTile->setImageRectangle(tileRect);


   double xSupport;
   double ySupport;

   getSupport(xSupport, ySupport);
   
   ossimIpt deltaPt;
   deltaPt.x = (ossim_int32)ceil(xSupport);
   deltaPt.y = (ossim_int32)ceil(ySupport);

   imageRect = ossimIrect(imageRect.ul().x - (deltaPt.x),
                          imageRect.ul().y - (deltaPt.y),
                          imageRect.lr().x + (deltaPt.x),
                          imageRect.lr().y + (deltaPt.y));

   
   runFilter(imageRect, tileRect);
   
   theTile->validate();
   
   return theTile;
}

void ossimScaleFilter::runFilter(const ossimIrect& imageRect,
                                 const ossimIrect& viewRect)
{
   switch(theTile->getScalarType())
   {
      case OSSIM_UINT8:
      {
         runFilterTemplate((ossim_uint8)0,
                           imageRect,
                           viewRect);
         break;
      }
      case OSSIM_USHORT11:
      case OSSIM_UINT16:
      {
         runFilterTemplate((ossim_uint16)0,
                           imageRect,
                           viewRect);
         break;
      }
      case OSSIM_SINT16:
      {
         runFilterTemplate((ossim_sint16)0,
                           imageRect,
                           viewRect);
         break;
      }
      case OSSIM_UINT32:
      {
         runFilterTemplate((ossim_uint32)0,
                           imageRect,
                           viewRect);
         break;
      }
      case OSSIM_DOUBLE:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         runFilterTemplate((ossim_float64)0,
                           imageRect,
                           viewRect);
         break;
      }
      case OSSIM_FLOAT:
      case OSSIM_NORMALIZED_FLOAT:
      {
         runFilterTemplate((ossim_float32)0,
                           imageRect,
                           viewRect);
         break;
      }
      default:
         break;
   }
}

template <class T>
void ossimScaleFilter::runFilterTemplate(T dummy,
                                         const ossimIrect& imageRect,
                                         const ossimIrect& viewRect)
{
   ossimRefPtr<ossimImageData> inputData =
      theInputConnection->getTile(imageRect);

   if(!inputData.valid()   ||
      !inputData->getBuf() ||
      (inputData->getDataObjectStatus() == OSSIM_EMPTY))
   {
      return;
   }

   ossim_int32 h = imageRect.height();
   ossimRefPtr<ossimImageData> tempData =
      ossimImageDataFactory::instance()->create(NULL,
                                                inputData->getScalarType(),
                                                inputData->getNumberOfBands(),
                                                viewRect.width(),
                                                h);
   tempData->setOrigin(ossimIpt(viewRect.ul().x,
                                imageRect.ul().y));
   
   tempData->initialize();
   
   if((theScaleFactor.x != 1.0)||
      (theBlurFactor != 1.0))
   {
      runHorizontalFilterTemplate(dummy,
                                  inputData,
                                  tempData);
      tempData->validate();
   }
   else
   {
      tempData->loadTile(inputData.get());
   }
   
   if((theScaleFactor.y != 1.0)||
      (theBlurFactor != 1.0))
   {
      runVerticalFilterTemplate(dummy,
                                tempData,
                                theTile);
   }
   else
   {
      theTile->loadTile(tempData.get());
   }
   
   theTile->validate();
}

ossimIrect ossimScaleFilter::getBoundingRect(ossim_uint32 resLevel)const
{
   ossimIrect result = ossimImageSourceFilter::getBoundingRect(resLevel);

   if(!result.hasNans())
   {
      result = ossimIrect(result.ul().x,
                          result.ul().y,
                          result.lr().x+1,
                          result.lr().y+1);
      result *= theScaleFactor;
   }
   
   return result;
}

void ossimScaleFilter::setFilterType(ossimScaleFilterType filterType)
{
   setFilterType(filterType, filterType);
}


void ossimScaleFilter::setFilterType(ossimScaleFilterType minifyFilterType,
                                     ossimScaleFilterType magnifyFilterType)
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
}

ossimFilter* ossimScaleFilter::createNewFilter(ossimScaleFilterType filterType,
                                               ossimScaleFilterType& result)
{
   switch(filterType)
   {
   case ossimScaleFilter_NEAREST_NEIGHBOR:
   {
      return new ossimNearestNeighborFilter();
   }
   case ossimScaleFilter_BOX:
   {
      return new ossimBoxFilter();
   }
   case ossimScaleFilter_GAUSSIAN:
   {
      return new ossimGaussianFilter();
   }
   case ossimScaleFilter_CUBIC:
   {
      return new ossimCubicFilter();
   }
   case ossimScaleFilter_HANNING:
   {
      return new ossimHanningFilter();
   }
   case ossimScaleFilter_HAMMING:
   {
      return new ossimHammingFilter();
   }
   case ossimScaleFilter_LANCZOS:
   {
      return new ossimLanczosFilter();
   }
   case ossimScaleFilter_CATROM:
   {
      return new ossimCatromFilter();
   }
   case ossimScaleFilter_MITCHELL:
   {
      return new ossimMitchellFilter();
   }
   case ossimScaleFilter_BLACKMAN:
   {
      return new ossimBlackmanFilter();
   }
   case ossimScaleFilter_BLACKMAN_SINC:
   {
      return new ossimBlackmanSincFilter();
   }
   case ossimScaleFilter_BLACKMAN_BESSEL:
   {
      return new ossimBlackmanBesselFilter();
   }
   case ossimScaleFilter_QUADRATIC:
   {
      return new ossimQuadraticFilter();
   }
   case ossimScaleFilter_TRIANGLE:
   {
      return new ossimTriangleFilter();
   }
   case ossimScaleFilter_HERMITE:
   {
      return new ossimHermiteFilter();
   }
   
   }

   result = ossimScaleFilter_NEAREST_NEIGHBOR;
   return new ossimNearestNeighborFilter();
}

void ossimScaleFilter::setScaleFactor(const ossimDpt& scale)
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


template <class T> void ossimScaleFilter::runHorizontalFilterTemplate(
   T dummy,
   const ossimRefPtr<ossimImageData>& input,
   ossimRefPtr<ossimImageData>& output)
{
   ossimIrect viewRect  = output->getImageRectangle();
   ossimIrect imageRect = input->getImageRectangle();
   ossim_int32 vw = viewRect.width();
   ossim_int32 vh = viewRect.height();
   ossim_int32 iw = imageRect.width();
   ossimIpt origin(viewRect.ul());
   ossimIpt imageOrigin(imageRect.ul());
   ossimIpt inputUl = theInputRect.ul();
   ossimIpt inputLr = theInputRect.lr();
   
   double scale = 0.0;
   double support = 0.0;
   ossim_int32 x = 0;
   ossim_int32 y = 0;
   ossim_int32 start = 0;
   ossim_int32 stop  = 0;
   ossim_int32 kernelIdx = 0;
   const ossimFilter* filter = getHorizontalFilter();
   ossim_float64 center = 0.0;
   ossim_int32 bandIdx = 0;
   ossim_int32 numberOfBands = theTile->getNumberOfBands();
   
   scale = theBlurFactor*ossim::max(1.0/theScaleFactor.x, 1.0);
   
   support=scale*filter->getSupport();
   if (support <= 0.5)
   {
      support = 0.5 + FLT_EPSILON;
      scale = 1.0;
   }
   scale=1.0/scale;
   for(bandIdx = 0; bandIdx < numberOfBands; ++bandIdx)
   {
      T* imageBuf = (T*)input->getBuf(bandIdx);
      T* viewBuf  = (T*)output->getBuf(bandIdx);
      T np        = (T)input->getNullPix(bandIdx);
      T outNp     = (T)output->getNullPix(bandIdx);
      T outMinPix = (T)output->getMinPix(bandIdx);
      T outMaxPix = (T)output->getMaxPix(bandIdx);
      
      for(x = 0; x < vw; ++x)
      {
         center=(origin.x + x+ .5)/theScaleFactor.x;
         start=ossim::max((ossim_int32)ossim::round<int>(center-support), (ossim_int32)inputUl.x);
         stop=ossim::min((ossim_int32)ossim::round<int>(center+support), (ossim_int32)inputLr.x);
         ossim_int32 delta = stop-start;
         if (delta <= 0)
         {
            break;
         }
         vector<double> kernel(delta);
         double density=0.0;
         
         for(kernelIdx = 0; kernelIdx < delta; ++kernelIdx)
         {
            double t = scale*(start + kernelIdx -
                              center + .5);
            kernel[kernelIdx] = filter->filter(t,
                                               filter->getSupport());
            density += kernel[kernelIdx];
         }
         if ((density != 0.0) && (density != 1.0))
         {
            /*
              Normalize.
            */
            density=1.0/density;
            for (kernelIdx=0; kernelIdx < delta; kernelIdx++)
               kernel[kernelIdx]*=density;
         }
         ossim_int32 offset       = start  - imageOrigin.x;
         
         T* xptr         = imageBuf + offset;
         T* xCenterptr   = imageBuf + offset;
         T* outptr       = viewBuf  + x;
         
         for(y = 0; y < vh; ++y)
         {
            double result = 0.0;
            density = 0.0;
            if((*xCenterptr) == np)
            {
               *outptr = outNp;
            }
            else
            {
               for(kernelIdx = 0; kernelIdx < (int)kernel.size(); ++kernelIdx)
               {
                  if((*xptr != np)&&
                     (kernel[kernelIdx] != 0.0))
                  {
                     result  += ((double)(*(xptr+kernelIdx))*kernel[kernelIdx]);
                     density += kernel[kernelIdx];
                  }
               }
               if(density != 0.0)
               {
                  result /= density;
                  
                  if(result < outMinPix) result = outMinPix;
                  if(result > outMaxPix) result = outMaxPix;
                  
                  *outptr = (T)result;
               }
               else
               {
                  *outptr = outNp;
               }
            }
            xCenterptr += iw;
            xptr   += iw;
            outptr += vw;
         }
      }
   }
}

template <class T> void ossimScaleFilter::runVerticalFilterTemplate(
   T dummy,
   const ossimRefPtr<ossimImageData>& input,
   ossimRefPtr<ossimImageData>& output)
{
   ossimIrect viewRect  = output->getImageRectangle();
   ossimIrect imageRect = input->getImageRectangle();
   ossim_int32 vw = viewRect.width();
   ossim_int32 vh = viewRect.height();
   ossim_int32 iw = imageRect.width();
   ossimIpt origin(viewRect.ul());
   ossimIpt imageOrigin(imageRect.ul());
   ossimIpt inputUl = theInputRect.ul();
   ossimIpt inputLr = theInputRect.lr();
   double scale = 0.0;
   double support = 0.0;
   ossim_int32 x = 0;
   ossim_int32 y = 0;
   ossim_int32 start = 0;
   ossim_int32 stop  = 0;
   ossim_int32 kernelIdx = 0;
   const ossimFilter* filter = getVerticalFilter();
   ossim_float64 center = 0.0;
   ossim_int32 bandIdx = 0;
   ossim_int32 numberOfBands = theTile->getNumberOfBands();
   
   scale = theBlurFactor*ossim::max(1.0/theScaleFactor.y, 1.0);
   
   support=scale*filter->getSupport();
   if (support <= 0.5)
   {
      support = .5 + FLT_EPSILON;
      scale = 1.0;
   }
   scale=1.0/scale;

   for(bandIdx = 0; bandIdx < numberOfBands; ++bandIdx)
   {
      T* imageBuf = (T*)input->getBuf(bandIdx);
      T* viewBuf  = (T*)output->getBuf(bandIdx);
      T np        = (T)input->getNullPix(bandIdx);
      T outNp     = (T)output->getNullPix(bandIdx);
      T outMinPix = (T)output->getMinPix(bandIdx);
      T outMaxPix = (T)output->getMaxPix(bandIdx);
     
      for(y = 0; y < vh; ++y)
      {
         center=(double) ((y + origin.y+0.5)/theScaleFactor.y);
         start=ossim::max((ossim_int32)ossim::round<int>(center-support), (ossim_int32)inputUl.y);
         stop=ossim::min((ossim_int32)ossim::round<int>(center+support), (ossim_int32)inputLr.y);
         ossim_int32 delta = stop-start;
         if (delta <= 0)
         {
            break;
         }
         vector<double> kernel(delta);
         double density = 0.0;
         for(kernelIdx = 0; kernelIdx < delta; ++kernelIdx)
         {
            kernel[kernelIdx] = filter->filter(scale*(start + kernelIdx - center + .5),
                                               filter->getSupport());
            density += kernel[kernelIdx];
         }
         if ((density != 0.0) && (density != 1.0))
         {
            /*
              Normalize.
            */
            density=1.0/density;
            for (kernelIdx=0; kernelIdx < delta; kernelIdx++)
               kernel[kernelIdx]*=density;
         }

         ossim_int32 offset       = ((start  - imageOrigin.y)*iw);
         ossim_int32 offsetCenter = ((((ossim_int32)center) - imageOrigin.y)*iw);
        
         for(x = 0; x < vw; ++x)
         {
            T* yptr         = imageBuf + offset       + x;
            T* yCenterptr   = imageBuf + offsetCenter + x;
            double result = 0.0;
            density = 0.0;

            if((*yCenterptr) == np)
            {
               *viewBuf = outNp;
            }
            else
            {
               for(kernelIdx = 0; kernelIdx < delta; ++kernelIdx)
               {
                  if((*yptr != np)&&
                     (kernel[kernelIdx] != 0.0))
                  {
                     result  += ((*yptr)*kernel[kernelIdx]);
                     density += kernel[kernelIdx];
                  }
                  yptr += iw;
               }
               if(density != 0.0)
               {
                  result /= density;
                 
                  if(result < outMinPix) result = outMinPix;
                  if(result > outMaxPix) result = outMaxPix;
                 
                  *viewBuf = (T)result;
               }
               else
               {
                  *viewBuf = outNp;
               }
            }
            ++viewBuf;
         }
      }
   }
}

void ossimScaleFilter::initialize()
{
   ossimImageSourceFilter::initialize();

   // Force an allocate next getTile.
   theTile = NULL;
   theBlankTile = NULL;
   theInputRect.makeNan();
}

void ossimScaleFilter::allocate()
{
   theTile      = NULL;
   theBlankTile = NULL;
   theInputRect.makeNan();

   if(theInputConnection&&isSourceEnabled())
   {
      theTile      = ossimImageDataFactory::instance()->create(this, this);
      theBlankTile = ossimImageDataFactory::instance()->create(this, this);
      
      theTile->initialize();

      theInputRect = theInputConnection->getBoundingRect();
   }
}

bool ossimScaleFilter::getImageGeometry(ossimKeywordlist& kwl,
                                        const char* prefix)
{
   if(theInputConnection)
   {
      theInputConnection->getImageGeometry(kwl, prefix);

      ossimProjection* proj = ossimProjectionFactoryRegistry::instance()->createProjection(kwl,
                                                                                           prefix);

      if(proj)
      {
         ossimMapProjection* mapProj = PTR_CAST(ossimMapProjection,
                                                proj);

         if(mapProj)
         {
            mapProj->applyScale(theInverseScaleFactor, true);

            mapProj->saveState(kwl, prefix);
         }
         
         delete proj;

         return true;
      }
   }
   return false;
}

ossimIrect ossimScaleFilter::scaleRect(const ossimIrect input,
                                       const ossimDpt& scaleFactor)const
{
   ossimIpt origin(ossim::round<int>(input.ul().x*scaleFactor.x),
                   ossim::round<int>(input.ul().y*scaleFactor.y));
   ossim_int32 w = ossim::round<int>(input.width()*scaleFactor.x);
   ossim_int32 h = ossim::round<int>(input.height()*scaleFactor.y);

   if(w < 1) w = 1;
   if(h < 1) h = 1;
   
   return ossimIrect(origin.x,
                     origin.y,
                     origin.x + (w-1),
                     origin.y + (h-1));
}

ossimString ossimScaleFilter::getFilterTypeAsString(ossimScaleFilterType type)const
{
   switch(type)
   {
   case ossimScaleFilter_NEAREST_NEIGHBOR:
   {
      return "nearest_neighbor";
   }
   case ossimScaleFilter_BOX:
   {
      return "box";
   }
   case ossimScaleFilter_GAUSSIAN:
   {
      return "gaussian";
   }
   case ossimScaleFilter_CUBIC:
   {
      return "cubic";
   }
   case ossimScaleFilter_HANNING:
   {
      return "hanning";
   }
   case ossimScaleFilter_HAMMING:
   {
      return "hamming";
   }
   case ossimScaleFilter_LANCZOS:
   {
      return "lanczos";
   }
   case ossimScaleFilter_MITCHELL:
   {
      return "mitchell";
   }
   case ossimScaleFilter_CATROM:
   {
      return "catrom";
   }
   case ossimScaleFilter_BLACKMAN:
   {
      return "blackman";
   }
   case ossimScaleFilter_BLACKMAN_SINC:
   {
      return "blackman_sinc";
   }
   case ossimScaleFilter_BLACKMAN_BESSEL:
   {
      return "blackman_bessel";
   }
   case ossimScaleFilter_QUADRATIC:
   {
      return "quadratic";
   }
   case ossimScaleFilter_TRIANGLE:
   {
      return "triangle";
   }
   case ossimScaleFilter_HERMITE:
   {
      return "hermite";
   }
   }

   return "nearest_neighbor";
}

ossimScaleFilter::ossimScaleFilterType ossimScaleFilter::getFilterType(const ossimString& type)const
{
   ossimString typeUpper = type;
   typeUpper = typeUpper.upcase();

   if(typeUpper.contains("BOX"))
   {
      return ossimScaleFilter_BOX;
   }
   else if(typeUpper.contains("NEAREST_NEIGHBOR"))
   {
      return ossimScaleFilter_NEAREST_NEIGHBOR;
   }
   else if(typeUpper.contains("GAUSSIAN"))
   {
      return ossimScaleFilter_GAUSSIAN;
   }
   else if(typeUpper.contains("HANNING"))
   {
      return ossimScaleFilter_HANNING;
   }
   else if(typeUpper.contains("HAMMING"))
   {
      return ossimScaleFilter_HAMMING;
   }
   else if(typeUpper.contains("LANCZOS"))
   {
      return ossimScaleFilter_LANCZOS;
   }
   else if(typeUpper.contains("MITCHELL"))
   {
      return ossimScaleFilter_MITCHELL;
   }
   else if(typeUpper.contains("CATROM"))
   {
      return ossimScaleFilter_CATROM;
   }
   else if(typeUpper.contains("CUBIC"))
   {
      return ossimScaleFilter_CUBIC;
   }
   else if(typeUpper.contains("BLACKMAN_BESSEL"))
   {
      return ossimScaleFilter_BLACKMAN_BESSEL;
   }
   else if(typeUpper.contains("BLACKMAN_SINC"))
   {
      return ossimScaleFilter_BLACKMAN_SINC;
   }
   else if(typeUpper.contains("BLACKMAN"))
   {
      return ossimScaleFilter_BLACKMAN;
   }
   else if(typeUpper.contains("QUADRATIC"))
   {
      return ossimScaleFilter_QUADRATIC;
   }
   else if(typeUpper.contains("TRIANGLE"))
   {
      return ossimScaleFilter_TRIANGLE;
   }
   else if(typeUpper.contains("HERMITE"))
   {
      return ossimScaleFilter_HERMITE;
   }
   
   return ossimScaleFilter_NEAREST_NEIGHBOR;
}

void ossimScaleFilter::getSupport(double& x, double& y)
{
   const ossimFilter* horizontalFilter = getHorizontalFilter();
   const ossimFilter* verticalFilter   = getVerticalFilter();
   
   x = theBlurFactor*ossim::max(1.0/theScaleFactor.x, 1.0)*
       horizontalFilter->getSupport();
   y = theBlurFactor*ossim::max(1.0/theScaleFactor.y, 1.0)*
       verticalFilter->getSupport();
}

const ossimFilter* ossimScaleFilter::getHorizontalFilter()const
{
   if(theScaleFactor.x < 1)
   {
      return theMinifyFilter;
   }

   return theMagnifyFilter;
}

const ossimFilter* ossimScaleFilter::getVerticalFilter()const
{
   if(theScaleFactor.y < 1)
   {
      return theMinifyFilter;
   }

   return theMagnifyFilter;
}



bool ossimScaleFilter::saveState(ossimKeywordlist& kwl, const char* prefix)const
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
   
   return ossimImageSourceFilter::saveState(kwl, prefix);
}

bool ossimScaleFilter::loadState(const ossimKeywordlist& kwl,
                                 const char* prefix)
{
   ossimString scalex  = kwl.find(prefix,
                                 ossimKeywordNames::SCALE_X_KW);
   ossimString scaley  = kwl.find(prefix,
                                  ossimKeywordNames::SCALE_Y_KW);
   ossimString minify  = kwl.find(prefix,
                                  "minify_type");
   ossimString magnify = kwl.find(prefix,
                                  "magnify_type");
   
   theScaleFactor.x = scalex.toDouble();
   theScaleFactor.y = scaley.toDouble();

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
   
   return ossimImageSourceFilter::loadState(kwl, prefix);
}

