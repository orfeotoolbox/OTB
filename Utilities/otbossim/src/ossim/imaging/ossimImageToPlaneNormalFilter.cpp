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
// $Id: ossimImageToPlaneNormalFilter.cpp 21631 2012-09-06 18:10:55Z dburken $
#include <ossim/imaging/ossimImageToPlaneNormalFilter.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNumericProperty.h>
#include <ossim/base/ossimBooleanProperty.h>

static const char* SMOOTHNESS_FACTOR_KW="smoothness_factor";

RTTI_DEF1(ossimImageToPlaneNormalFilter, "ossimImageToPlaneNormalFilter", ossimImageSourceFilter);

ossimImageToPlaneNormalFilter::ossimImageToPlaneNormalFilter()
   :ossimImageSourceFilter(),
    theTile(NULL),
    theBlankTile(NULL),
    theTrackScaleFlag(true),
    theXScale(1.0),
    theYScale(1.0),
    theSmoothnessFactor(1.0)
{
}

ossimImageToPlaneNormalFilter::ossimImageToPlaneNormalFilter(ossimImageSource* inputSource)
   :ossimImageSourceFilter(inputSource),
    theTile(NULL),
    theBlankTile(NULL),
    theTrackScaleFlag(true),
    theXScale(1.0),
    theYScale(1.0),
    theSmoothnessFactor(1.0)
{
}

ossimRefPtr<ossimImageData> ossimImageToPlaneNormalFilter::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if(!isSourceEnabled()||!theInputConnection)
   {
      return ossimImageSourceFilter::getTile(tileRect, resLevel);
   }

   if(!theTile.valid())
   {
      initialize();
   }

   if(!theTile.valid())
   {
      return ossimImageSourceFilter::getTile(tileRect, resLevel);
   }

   theTile->setImageRectangle(tileRect);

   ossimIrect requestRect(tileRect.ul().x - 1,
                          tileRect.ul().y - 1,
                          tileRect.lr().x + 1,
                          tileRect.lr().y + 1);

   ossimRefPtr<ossimImageData> input =
      theInputConnection->getTile(requestRect, resLevel);

   if(!input||(input->getDataObjectStatus()==OSSIM_EMPTY)||!input->getBuf())
   {
      if(tileRect.completely_within(theInputBounds))
      {
         initializeTile();
         theTile->validate();
         return theTile.get();
      }
      theBlankTile->setImageRectangle(tileRect);
      return theBlankTile;
   }

   double oldScaleX = theXScale;
   double oldScaleY = theYScale;

   if(resLevel > 0)
   {
      ossimDpt scaleFactor;
      theInputConnection->getDecimationFactor(resLevel, scaleFactor);

      if(!scaleFactor.hasNans())
      {
         theXScale *= scaleFactor.x;
         theYScale *= scaleFactor.y;
      }
   }

   computeNormals(input, theTile);

   theXScale = oldScaleX;
   theYScale = oldScaleY;

   theTile->validate();

   return theTile;
}

void ossimImageToPlaneNormalFilter::initializeTile()
{
   double* x = static_cast<double*>(theTile->getBuf(0));
   double* y = static_cast<double*>(theTile->getBuf(1));
   double* z = static_cast<double*>(theTile->getBuf(2));
   
   if(x) std::fill(x, x+theTile->getSizePerBand(), 0.0);
   if(y) std::fill(y, y+theTile->getSizePerBand(), 0.0);
   if(z) std::fill(z, z+theTile->getSizePerBand(), 1.0);
}

void ossimImageToPlaneNormalFilter::initialize()
{
   if(theInputConnection)
   {
      theInputConnection->initialize();
      
      theInputBounds = theInputConnection->getBoundingRect();
      theTile      = ossimImageDataFactory::instance()->create(this, this);
      theBlankTile = (ossimImageData*)(theTile->dup());
      theTile->initialize();

      if(theTrackScaleFlag)
      {
         ossimRefPtr<ossimImageGeometry> geom = theInputConnection->getImageGeometry();
         if( geom.valid() )
         {
            ossimDpt pt = geom->getMetersPerPixel();
            if(!pt.hasNans())
            {
               theXScale = 1.0/pt.x;
               theYScale = 1.0/pt.y;
            }
         }
      }
   }
}

void ossimImageToPlaneNormalFilter::computeNormals(
   ossimRefPtr<ossimImageData>& inputTile,
   ossimRefPtr<ossimImageData>& outputTile)
{
   switch(inputTile->getScalarType())
   {
      case OSSIM_SSHORT16:
      {
         computeNormalsTemplate((ossim_sint16)0,
                                inputTile,
                                outputTile);
         break;
      }
      case OSSIM_UCHAR:
      {
         computeNormalsTemplate((ossim_uint8)0,
                                inputTile,
                                outputTile);
         break;
      }
      case OSSIM_USHORT11:
      case OSSIM_USHORT16:
      {
         computeNormalsTemplate((ossim_uint16)0,
                                inputTile,
                                outputTile);
         break;
      }
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_DOUBLE:
      {
         computeNormalsTemplate((ossim_float64)0,
                                inputTile,
                                outputTile);
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT:
      {
         computeNormalsTemplate((ossim_float32)0,
                                inputTile,
                                outputTile);
         break;
      }
      default:
         break;
   }
}

template <class T> void ossimImageToPlaneNormalFilter::computeNormalsTemplate(
   T /* inputScalarTypeDummy */,
   ossimRefPtr<ossimImageData>& inputTile,
   ossimRefPtr<ossimImageData>& outputTile)
{
   T inputNull = (T)inputTile->getNullPix(0);
   T* inbuf = (T*)inputTile->getBuf();

   double* normX = (double*)outputTile->getBuf(0);
   double* normY = (double*)outputTile->getBuf(1);
   double* normZ = (double*)outputTile->getBuf(2);
   ossim_int32 inbuf_width = inputTile->getWidth();
   ossim_int32 normbuf_width = outputTile->getWidth();
   ossim_int32 normbuf_height = outputTile->getHeight();
   ossimColumnVector3d normal;

   for (ossim_int32 y=0; y<normbuf_height; y++)
   {
      // Establish offsets into the image and output normals buffers given row:
      ossim_uint32 n = y*normbuf_width;
      ossim_uint32 i = (y+1)*inbuf_width + 1;
      
      // Loop to compute the gradient (normal) vector [dh/dx, dh/dy, 1]:
      for (ossim_int32 x=0; x<normbuf_width; x++)
      {
         // Default in case of null inputs is a flat earth:
         normal[0] = 0;
         normal[1] = 0;
         normal[2] = 1.0;

         // Compute the x-direction differential:
         if (inbuf[i+1] != inputNull)
         {
            if (inbuf[i-1] != inputNull)
               normal[0] = theXScale*theSmoothnessFactor*(inbuf[i+1] - inbuf[i-1]) / 2.0;
            else if (inbuf[i] != inputNull)
               normal[0] = theXScale*theSmoothnessFactor*(inbuf[i+1] - inbuf[i]);
         }
         else if ((inbuf[i] != inputNull) && (inbuf[i-1] != inputNull))
         {
            normal[0] = theXScale*theSmoothnessFactor*(inbuf[i] - inbuf[i-1]);
         }

         // Compute the y-direction differential:
         if (inbuf[i+inbuf_width] != inputNull)
         {
            if (inbuf[i-inbuf_width] != inputNull)
               normal[1] = theYScale*theSmoothnessFactor*(inbuf[i+inbuf_width] - inbuf[i-inbuf_width]) / 2.0;
            else if (inbuf[i] != inputNull)
               normal[1] = theYScale*theSmoothnessFactor*(inbuf[i+inbuf_width] - inbuf[i]);
         }
         else if ((inbuf[i] != inputNull) && (inbuf[i-inbuf_width] != inputNull))
         {
            normal[1] = theYScale*theSmoothnessFactor*(inbuf[i] - inbuf[i-inbuf_width]);
         }

         // Stuff the normalized gradient vector into the output buffers:
         normal = normal.unit();
         normX[n] = normal[0];
         normY[n] = normal[1];
         normZ[n] = normal[2];
         
         ++n;
         ++i;
      }
   }
}

bool ossimImageToPlaneNormalFilter::loadState(const ossimKeywordlist& kwl,
                                              const char* prefix)
{
   ossimString scaleX     = kwl.find(prefix, ossimKeywordNames::SCALE_PER_PIXEL_X_KW);
   ossimString scaleY     = kwl.find(prefix, ossimKeywordNames::SCALE_PER_PIXEL_Y_KW);
   ossimString trackFlag  = kwl.find(prefix, "track_scale_flag");
   ossimString smoothness = kwl.find(prefix, SMOOTHNESS_FACTOR_KW);

   if(scaleX != "")
   {
      theXScale = scaleX.toDouble();
   }
   if(scaleY != "")
   {
      theYScale = scaleY.toDouble();
   }
   if(trackFlag != "")
   {
      theTrackScaleFlag = trackFlag.toBool();
   }
   if(smoothness!="")
   {
      theSmoothnessFactor = smoothness.toDouble();
   }

   return ossimImageSourceFilter::loadState(kwl, prefix);
}

bool ossimImageToPlaneNormalFilter::saveState(ossimKeywordlist& kwl,
                                              const char* prefix)const
{
   kwl.add(prefix,
           ossimKeywordNames::SCALE_PER_PIXEL_X_KW,
           theXScale,
           true);

   kwl.add(prefix,
           ossimKeywordNames::SCALE_PER_PIXEL_Y_KW,
           theXScale,
           true);

   kwl.add(prefix,
           "track_scale_flag",
           (ossim_uint32)theTrackScaleFlag,
           true);

   kwl.add(prefix,
           SMOOTHNESS_FACTOR_KW,
           theSmoothnessFactor,
           true);

   return ossimImageSourceFilter::saveState(kwl, prefix);
}

double ossimImageToPlaneNormalFilter::getMinPixelValue(ossim_uint32 band)const
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getMinPixelValue(band);
   }
   return -1;
}

double ossimImageToPlaneNormalFilter::getMaxPixelValue(ossim_uint32 band)const
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getMaxPixelValue(band);
   }
   return 1.0;
}

ossimScalarType ossimImageToPlaneNormalFilter::getOutputScalarType() const
{
   if(isSourceEnabled())
   {
      return OSSIM_DOUBLE;
   }
   
   return ossimImageSourceFilter::getOutputScalarType();
}

ossim_uint32 ossimImageToPlaneNormalFilter::getNumberOfOutputBands() const
{
   if(isSourceEnabled())
   {
      return 3;
   }
   return ossimImageSourceFilter::getNumberOfOutputBands();
}

void ossimImageToPlaneNormalFilter::setXScale(const double& scale)
{
   theXScale = scale;
}

double ossimImageToPlaneNormalFilter::getXScale()const
{
   return theXScale;
}

double ossimImageToPlaneNormalFilter::getYScale()const
{
   return theYScale;
}

void ossimImageToPlaneNormalFilter::setYScale(const double& scale)
{
   theYScale = scale;
}

void ossimImageToPlaneNormalFilter::setTrackScaleFlag(bool flag)
{
   theTrackScaleFlag = flag;
}

bool ossimImageToPlaneNormalFilter::getTrackScaleFlag()const
{
   return theTrackScaleFlag;
}

void ossimImageToPlaneNormalFilter::setSmoothnessFactor(double value)
{
   theSmoothnessFactor = value;
}

double ossimImageToPlaneNormalFilter::getSmoothnessFactor()const
{
   return theSmoothnessFactor;
}

void ossimImageToPlaneNormalFilter::setProperty(ossimRefPtr<ossimProperty> property)
{
   ossimString name = property->getName();
   if((name == "smoothnessFactor")||
      (name == "gain"))
   {
      theSmoothnessFactor = property->valueToString().toDouble();
      initialize();
   }
   else if(name == "xscale")
   {
      theXScale = property->valueToString().toDouble();
      initialize();
   }
   else if(name == "yscale")
   {
      theYScale = property->valueToString().toDouble();
      initialize();
   }
   else if(name == "autoTrackScaleFlag")
   {
      theTrackScaleFlag = property->valueToString().toDouble();
      initialize();
   }
   else
   {
      ossimImageSourceFilter::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimImageToPlaneNormalFilter::getProperty(const ossimString& name)const
{
   if((name == "smoothnessFactor")||
      (name == "gain"))
   {
      ossimNumericProperty* prop = new ossimNumericProperty("gain", ossimString::toString(theSmoothnessFactor), .0001, 40);
      prop->setCacheRefreshBit();
      return prop;
   }
   else if(name == "xscale")
   {
      ossimNumericProperty* prop = new ossimNumericProperty(name, ossimString::toString(theXScale), .0001, 50000);
      prop->setCacheRefreshBit();
      return prop;
   }
   else if(name == "yscale")
   {
      ossimNumericProperty* prop = new ossimNumericProperty(name, ossimString::toString(theYScale), .0001, 50000);
      prop->setCacheRefreshBit();
      return prop;
   }
   else if(name == "autoTrackScaleFlag")
   {
      ossimBooleanProperty* prop = new ossimBooleanProperty(name, theTrackScaleFlag);
      prop->setCacheRefreshBit();
      return prop;
   }
   
   return ossimImageSourceFilter::getProperty(name);
}

void ossimImageToPlaneNormalFilter::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSourceFilter::getPropertyNames(propertyNames);
   propertyNames.push_back("gain");
   propertyNames.push_back("xscale");
   propertyNames.push_back("yscale");
   propertyNames.push_back("autoTrackScaleFlag");
}
