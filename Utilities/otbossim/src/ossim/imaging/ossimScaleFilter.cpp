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
// $Id: ossimScaleFilter.cpp 21631 2012-09-06 18:10:55Z dburken $
#include <ossim/imaging/ossimScaleFilter.h>
#include <ossim/imaging/ossimFilter.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/imaging/ossimDiscreteConvolutionKernel.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/base/ossimKeywordNames.h>

RTTI_DEF1(ossimScaleFilter, "ossimScaleFilter", ossimImageSourceFilter);

//**************************************************************************************************
ossimScaleFilter::ossimScaleFilter()
   :ossimImageSourceFilter(),
    m_BlankTile(NULL),
    m_Tile(NULL),
    m_MinifyFilter(NULL),
    m_MagnifyFilter(NULL),
    m_MinifyFilterType(ossimScaleFilter_NEAREST_NEIGHBOR),
    m_MagnifyFilterType(ossimScaleFilter_NEAREST_NEIGHBOR),
    m_ScaleFactor(1.0, 1.0),
    m_InverseScaleFactor(1.0, 1.0),
    m_TileSize(64, 64),
    m_BlurFactor(1.0)
{
   m_InputRect.makeNan();
   m_MinifyFilter  = new ossimNearestNeighborFilter();
   m_MagnifyFilter = new ossimNearestNeighborFilter();
}

//**************************************************************************************************
ossimScaleFilter::ossimScaleFilter(ossimImageSource* inputSource,
                                   const ossimDpt& scaleFactor)
   :ossimImageSourceFilter(inputSource),
    m_BlankTile(NULL),
    m_Tile(NULL),
    m_MinifyFilter(NULL),
    m_MagnifyFilter(NULL),
    m_MinifyFilterType(ossimScaleFilter_NEAREST_NEIGHBOR),
    m_MagnifyFilterType(ossimScaleFilter_NEAREST_NEIGHBOR),
    m_ScaleFactor(scaleFactor),
    m_TileSize(64, 64),
    m_BlurFactor(1.0)
{
   m_InputRect.makeNan();
   m_MinifyFilter  = new ossimNearestNeighborFilter();
   m_MagnifyFilter = new ossimNearestNeighborFilter();
}

//**************************************************************************************************
ossimScaleFilter::~ossimScaleFilter()
{
   if(m_MinifyFilter)
   {
      delete m_MinifyFilter;
      m_MinifyFilter = NULL;
   }
   
   if(m_MagnifyFilter)
   {
      delete m_MagnifyFilter;
      m_MagnifyFilter = NULL;
   }
}

//**************************************************************************************************
ossimRefPtr<ossimImageData> ossimScaleFilter::getTile(
   const ossimIrect& tileRect, ossim_uint32 resLevel)
{
   
   if((!isSourceEnabled())||
      (!theInputConnection)||
      ((m_ScaleFactor.x == 1.0)&&
       (m_ScaleFactor.y == 1.0)&&
       (m_BlurFactor == 1.0)))
   {
      return ossimImageSourceFilter::getTile(tileRect, resLevel);
   }
   if(!m_Tile.valid())
   {
      allocate();
   }

   if(!m_Tile)
   {
      return ossimImageSourceFilter::getTile(tileRect, resLevel);
   }

   m_Tile->makeBlank();

                       
   ossimIrect imageRect = tileRect*m_InverseScaleFactor;

   m_Tile->setImageRectangle(tileRect);
   m_BlankTile->setImageRectangle(tileRect);


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
   
   m_Tile->validate();
   
   return m_Tile;
}

//**************************************************************************************************
void ossimScaleFilter::runFilter(const ossimIrect& imageRect,
                                 const ossimIrect& viewRect)
{
   switch(m_Tile->getScalarType())
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

//**************************************************************************************************
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
   
   if((m_ScaleFactor.x != 1.0)||
      (m_BlurFactor != 1.0))
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
   
   if((m_ScaleFactor.y != 1.0)||
      (m_BlurFactor != 1.0))
   {
      runVerticalFilterTemplate(dummy,
                                tempData,
                                m_Tile);
   }
   else
   {
      m_Tile->loadTile(tempData.get());
   }
   
   m_Tile->validate();
}

//**************************************************************************************************
ossimIrect ossimScaleFilter::getBoundingRect(ossim_uint32 resLevel)const
{
   ossimIrect result = ossimImageSourceFilter::getBoundingRect(resLevel);

   if(!result.hasNans())
   {
      result = ossimIrect(result.ul().x,
                          result.ul().y,
                          result.lr().x+1,
                          result.lr().y+1);
      result *= m_ScaleFactor;
   }
   
   return result;
}

//**************************************************************************************************
void ossimScaleFilter::setFilterType(ossimScaleFilterType filterType)
{
   setFilterType(filterType, filterType);
}


//**************************************************************************************************
void ossimScaleFilter::setFilterType(ossimScaleFilterType minifyFilterType,
                                     ossimScaleFilterType magnifyFilterType)
{
   if(m_MinifyFilter)
   {
      delete m_MinifyFilter;
      m_MinifyFilter = NULL;
   }
   if(m_MagnifyFilter)
   {
      delete m_MagnifyFilter;
      m_MagnifyFilter = NULL;
   }
   
   m_MinifyFilterType  = minifyFilterType;
   m_MagnifyFilterType = magnifyFilterType;
   
   m_MinifyFilter  = createNewFilter(minifyFilterType, m_MinifyFilterType);
   m_MagnifyFilter = createNewFilter(magnifyFilterType, m_MagnifyFilterType);
}

//**************************************************************************************************
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

//**************************************************************************************************
void ossimScaleFilter::setScaleFactor(const ossimDpt& scale)
{
   m_ScaleFactor = scale;
   if(fabs(m_ScaleFactor.x) <= FLT_EPSILON)
   {
      m_ScaleFactor.x = 1.0;
   }
   if(fabs(m_ScaleFactor.y) <= FLT_EPSILON)
   {
      m_ScaleFactor.y = 1.0;
   }

   m_InverseScaleFactor.x = 1.0/m_ScaleFactor.x;
   m_InverseScaleFactor.y = 1.0/m_ScaleFactor.y;

   // A change in the scale factor implies a change to the image geometry. If one has been created
   // it needs to be modified:
   updateGeometry();
}


//**************************************************************************************************
template <class T> void ossimScaleFilter::runHorizontalFilterTemplate(
   T /* dummy */,
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
   ossimIpt inputUl = m_InputRect.ul();
   ossimIpt inputLr = m_InputRect.lr();
   
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
   ossim_int32 numberOfBands = m_Tile->getNumberOfBands();
   
   scale = m_BlurFactor*ossim::max(1.0/m_ScaleFactor.x, 1.0);
   
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
         center=(origin.x + x+ .5)/m_ScaleFactor.x;
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

//**************************************************************************************************
template <class T> void ossimScaleFilter::runVerticalFilterTemplate(
   T /* dummy */,
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
   ossimIpt inputUl = m_InputRect.ul();
   ossimIpt inputLr = m_InputRect.lr();
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
   ossim_int32 numberOfBands = m_Tile->getNumberOfBands();
   
   scale = m_BlurFactor*ossim::max(1.0/m_ScaleFactor.y, 1.0);
   
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
         center=(double) ((y + origin.y+0.5)/m_ScaleFactor.y);
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

//**************************************************************************************************
void ossimScaleFilter::initialize()
{
   ossimImageSourceFilter::initialize();

   // Force an allocate next getTile.
   m_Tile = NULL;
   m_BlankTile = NULL;
   m_InputRect.makeNan();
}

//**************************************************************************************************
void ossimScaleFilter::allocate()
{
   m_Tile      = NULL;
   m_BlankTile = NULL;
   m_InputRect.makeNan();

   if(theInputConnection&&isSourceEnabled())
   {
      m_Tile      = ossimImageDataFactory::instance()->create(this, this);
      m_BlankTile = ossimImageDataFactory::instance()->create(this, this);
      
      m_Tile->initialize();

      m_InputRect = theInputConnection->getBoundingRect();
   }
}

//**************************************************************************************************
// Returns a pointer reference to the active image geometry at this filter. The input source
// geometry is modified, so we need to maintain our own geometry object as a data member.
//**************************************************************************************************
ossimRefPtr<ossimImageGeometry> ossimScaleFilter::getImageGeometry()
{
   // Have we already defined our own geometry? Return it if so:
   if (m_ScaledGeometry.valid()) return m_ScaledGeometry;

   // Otherwise we'll need to establish a geometry based on the input connection:
   if(theInputConnection)
   {
      // Fetch the map projection of the input image if it exists:
      ossimRefPtr<ossimImageGeometry> inputGeom = theInputConnection->getImageGeometry();

      // If trivial case of identity scale, just pass along the input connection's geometry:
      if ((m_ScaleFactor.x == 1.0) && (m_ScaleFactor.y == 1.0))
         return inputGeom;

      // Need to create a copy of the input geom and modify it as our own, then pass that:
      if ( inputGeom.valid() )
      {
         m_ScaledGeometry = new ossimImageGeometry(*inputGeom);
         updateGeometry();

         // Return the modified geometry:
         return m_ScaledGeometry;
      }
   }

   // No geometry defined, return NULL pointer:
   return ossimRefPtr<ossimImageGeometry>();
}

//**************************************************************************************************
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

//**************************************************************************************************
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

//**************************************************************************************************
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

//**************************************************************************************************
void ossimScaleFilter::getSupport(double& x, double& y)
{
   const ossimFilter* horizontalFilter = getHorizontalFilter();
   const ossimFilter* verticalFilter   = getVerticalFilter();
   
   x = m_BlurFactor*ossim::max(1.0/m_ScaleFactor.x, 1.0)*
       horizontalFilter->getSupport();
   y = m_BlurFactor*ossim::max(1.0/m_ScaleFactor.y, 1.0)*
       verticalFilter->getSupport();
}

//**************************************************************************************************
const ossimFilter* ossimScaleFilter::getHorizontalFilter()const
{
   if(m_ScaleFactor.x < 1)
   {
      return m_MinifyFilter;
   }

   return m_MagnifyFilter;
}

//**************************************************************************************************
const ossimFilter* ossimScaleFilter::getVerticalFilter()const
{
   if(m_ScaleFactor.y < 1)
   {
      return m_MinifyFilter;
   }

   return m_MagnifyFilter;
}



//**************************************************************************************************
bool ossimScaleFilter::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
   kwl.add(prefix,
           ossimKeywordNames::SCALE_X_KW,
           m_ScaleFactor.x,
           true);
   kwl.add(prefix,
           ossimKeywordNames::SCALE_Y_KW,
           m_ScaleFactor.y,
           true);
   kwl.add(prefix,
           "minify_type",
           getFilterTypeAsString(m_MinifyFilterType),
           true);
   kwl.add(prefix,
           "magnify_type",
           getFilterTypeAsString(m_MagnifyFilterType),
           true);
   
   return ossimImageSourceFilter::saveState(kwl, prefix);
}

//**************************************************************************************************
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
   
   m_ScaleFactor.x = scalex.toDouble();
   m_ScaleFactor.y = scaley.toDouble();

   if(fabs(m_ScaleFactor.x) <= FLT_EPSILON)
   {
      m_ScaleFactor.x = 1.0;
   }
   if(fabs(m_ScaleFactor.y) <= FLT_EPSILON)
   {
      m_ScaleFactor.y = 1.0;
   }

   m_InverseScaleFactor.x = 1.0/m_ScaleFactor.x;
   m_InverseScaleFactor.y = 1.0/m_ScaleFactor.y;
   
   setFilterType(getFilterType(minify),
                 getFilterType(magnify));
   
   // A change in the scale factor implies a change to the image geometry. If one has been created
   // it needs to be modified:
   updateGeometry();

   return ossimImageSourceFilter::loadState(kwl, prefix);
}

//**************************************************************************************************
//! If this object is maintaining an ossimImageGeometry, this method needs to be called after 
//! a scale change so that the geometry's projection is modified accordingly.
//**************************************************************************************************
void ossimScaleFilter::updateGeometry()
{
   if (m_ScaledGeometry.valid())
   {
      // Modify the image geometry's projection with the scale factor before returning geom:
      ossimProjection* proj = m_ScaledGeometry->getProjection();
      ossimMapProjection* mapProj = PTR_CAST(ossimMapProjection, proj);
      if(mapProj)
         mapProj->applyScale(m_InverseScaleFactor, true);
   }
}
