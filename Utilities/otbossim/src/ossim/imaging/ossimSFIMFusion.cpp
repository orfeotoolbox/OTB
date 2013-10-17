//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// LICENSE:  See top level LICENSE.txt
// 
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimSFIMFusion.cpp 22439 2013-10-11 19:16:09Z dburken $
#include <ossim/imaging/ossimSFIMFusion.h>
#include <ossim/matrix/newmat.h>
#include <ossim/matrix/newmatio.h>
#include <ossim/base/ossimNumericProperty.h>
#include <ossim/base/ossim2dLinearRegression.h>
#include <ossim/base/ossimNormRgbVector.h>
#include <ossim/base/ossimHsiVector.h>
#include <ossim/base/ossimVisitor.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimBooleanProperty.h>

RTTI_DEF2(ossimSFIMFusion, "ossimSFIMFusion", ossimFusionCombiner, ossimAdjustableParameterInterface);


static const ossim_uint32 HIGH_PASS_GAIN_OFFSET = 0;
static const ossim_uint32 LOW_PASS_WIDTH_OFFSET = 1;
static const ossim_uint32  NUMBER_OF_ADJUSTABLE_PARAMETERS = 2;

ossimSFIMFusion::ossimSFIMFusion()
   :theLowPassKernelWidth(1.5),
    theHighPassKernelWidth(3)
{
   theAutoAdjustScales = true;
   theLowPassFilter  = new ossimImageGaussianFilter;
   theHighPassFilter = new ossimConvolutionSource;

   setFilters();

   initAdjustableParameters();
}

ossimSFIMFusion::~ossimSFIMFusion()
{
}

ossimRefPtr<ossimImageData> ossimSFIMFusion::getTile(const ossimIrect& rect,
                                                                 ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      return ossimRefPtr<ossimImageData>();
   }

   if (!theIntensityConnection)
   {
      return theInputConnection->getTile(rect, resLevel);
   }
   
   if(!theNormLowPassTile.valid())
   {
      theNormLowPassTile = new ossimImageData(this,
                                              OSSIM_NORMALIZED_FLOAT,
                                              1,
                                              rect.width(),
                                              rect.height());
      theNormHighPassTile = new ossimImageData(this,
                                              OSSIM_NORMALIZED_FLOAT,
                                               1,
                                               rect.width(),
                                               rect.height());
      theNormLowPassTile->initialize();
      theNormHighPassTile->initialize();
      theNormLowPassTile->makeBlank();
      theNormHighPassTile->makeBlank();
   }

   theNormLowPassTile->setImageRectangle(rect);
   theNormHighPassTile->setImageRectangle(rect);
   theNormLowPassTile->makeBlank();
   theNormHighPassTile->makeBlank();

   if(!theLowPassFilter->getInput() && getInput())
   {
      initialize();
   }

   ossimRefPtr<ossimImageData> lowTile  = theLowPassFilter->getTile(rect, resLevel);
   ossimRefPtr<ossimImageData> highTile = theHighPassFilter->getTile(rect, resLevel);
//   ossimRefPtr<ossimImageData> highTile = getNormIntensity(rect, resLevel);

   // if we don't have valid low and high pass then return the input color tile
   // in its original format
   //
   if(!lowTile.valid()||!highTile.valid())
   {
//       return theInputConnection->getTile(rect, resLevel);
      return 0;
   }

   if((lowTile->getDataObjectStatus() == OSSIM_EMPTY)||
      (!lowTile->getBuf()) ||
      (highTile->getDataObjectStatus() == OSSIM_EMPTY)||
      (!highTile->getBuf()))
   {
//      return theInputConnection->getTile(rect, resLevel);
      return 0;
   }

   ossimRefPtr<ossimImageData> normColorData = getNormTile(rect, resLevel);
   
   if(!normColorData.valid())
   {
     return 0;
//      return theTile;
   }

   if((normColorData->getDataObjectStatus() == OSSIM_EMPTY)||
      !normColorData->getBuf())
   {
      return theTile;
   }

   // Must set the rectangle before querrying height and width.
   theTile->makeBlank();
   theTile->setImageRectangle(rect);
   
   ossim_uint32 y = 0;
   ossim_uint32 x = 0;
   ossim_uint32 w = theTile->getWidth();
   ossim_uint32 h = theTile->getHeight();

   ossimRefPtr<ossimImageData> normColorOutputData = (ossimImageData*)normColorData->dup();
   normColorOutputData->setImageRectangle(rect);
   normColorOutputData->loadTile(normColorData.get());
   
   // ossim_float64 slopeResult = 0.0;
   ossim_uint32 idx = 0;
   std::vector<ossim_float32*> bands(normColorData->getNumberOfBands());
   
   lowTile->copyTileToNormalizedBuffer((ossim_float32*)theNormLowPassTile->getBuf());
   
   highTile->copyTileToNormalizedBuffer((ossim_float32*)theNormHighPassTile->getBuf());
   theNormLowPassTile->validate();
   theNormHighPassTile->validate();
   ossimRefPtr<ossimImageData> lowPan = (ossimImageData*)theNormLowPassTile->dup();
   lowPan->setImageRectangle(rect);
   lowPan->loadTile(theNormLowPassTile.get());
   ossimRefPtr<ossimImageData> highPan = (ossimImageData*)theNormHighPassTile->dup();
   highPan->setImageRectangle(rect);
   highPan->loadTile(theNormHighPassTile.get());
   
   ossim_float32* panHigh = (ossim_float32*)highPan->getBuf();
   ossim_float32* panLow  = (ossim_float32*)lowPan->getBuf();
   for(idx = 0; idx < bands.size(); ++idx)
   {
      bands[idx] = (ossim_float32*)normColorOutputData->getBuf(idx);
   }
   // double delta = 0.0;
   ossim_uint32 bandsSize = (ossim_uint32)bands.size();
   double normMinPix = 0.0;
   for(y = 0; y < h; ++y)
   {
      for(x = 0; x < w; ++x)
      {
         for(idx = 0; idx < bandsSize; ++idx)
         {
            if((*bands[idx] != 0.0)&&
               (*panLow > FLT_EPSILON) ) // if band is not null and not divide by 0
            {
               normMinPix = (ossim_float32)normColorOutputData->getMinPix(idx);
               *bands[idx] = ((*bands[idx])*(*panHigh))/
                  (*panLow);
               if(*bands[idx] > 1.0) *bands[idx] = 1.0;
               if(*bands[idx] < normMinPix) *bands[idx] = normMinPix;
            }
            // let's comment out the nulling and we will instead just pass the color on
            //
//            else
//            {
//               *bands[idx] = 0.0;
//            }
            ++bands[idx];
         }
         ++panHigh;
         ++panLow;
      }
   }
   
   theTile->copyNormalizedBufferToTile((ossim_float32*)normColorOutputData->getBuf());
   theTile->validate();
   
   return theTile;
}

void ossimSFIMFusion::initialize()
{
   ossimFusionCombiner::initialize();
   if(!theIntensityConnection)
   {
      theLowPassFilter->disconnectAllInputs();
      theHighPassFilter->disconnectAllInputs();
   }
   else
   {
      theLowPassFilter->connectMyInputTo(0, PTR_CAST(ossimConnectableObject,
                                                     theIntensityConnection->getObject()));
      theHighPassFilter->connectMyInputTo(0, PTR_CAST(ossimConnectableObject,
                                                      theIntensityConnection->getObject()));
      adjustableParametersChanged();
      setFilters();
      theLowPassFilter->initialize();
      theHighPassFilter->initialize();
      if(theAutoAdjustScales)
      {
         if(theInputConnection && theIntensityConnection)
         {
            ossimTypeNameVisitor visitor("ossimImageRenderer", true);
            
            theInputConnection->accept(visitor);
            ossimRefPtr<ossimConnectableObject> inputColor = visitor.getObjectAs<ossimConnectableObject>();
            visitor.reset();
            theIntensityConnection->accept(visitor);
            ossimRefPtr<ossimConnectableObject> inputPan = visitor.getObjectAs<ossimConnectableObject>();
            
            
            if(inputColor.valid()&&inputPan.valid())
            {
               ossimImageSource* inputColorSource = dynamic_cast<ossimImageSource*> (inputColor->getInput());
               ossimImageSource* inputPanSource = dynamic_cast<ossimImageSource*> (inputPan->getInput());
               
               if(inputColorSource&&inputPanSource)
               {
                  ossimRefPtr<ossimImageGeometry> colorGeom     = inputColorSource->getImageGeometry();
                  ossimRefPtr<ossimImageGeometry> intensityGeom = inputPanSource->getImageGeometry();
                  if(colorGeom.valid()&&intensityGeom.valid())
                  {
                     ossimDpt gsdIntensity = intensityGeom->getMetersPerPixel();
                     ossimDpt gsdColor     = colorGeom->getMetersPerPixel();
                     if(!gsdColor.hasNans()&&!gsdIntensity.hasNans())
                     {
                        double scaleChange = gsdColor.length()/gsdIntensity.length();
                        if(scaleChange < 1.0) scaleChange = 1.0;
                        setParameterOffset(LOW_PASS_WIDTH_OFFSET,
                                           scaleChange);
                     }
                  }
               }
            }
         }
      }
   }
}

void ossimSFIMFusion::setFilters()
{
   theLowPassFilter->setGaussStd(theLowPassKernelWidth);
   
   theHighPassMatrix = NEWMAT::Matrix(theHighPassKernelWidth, theHighPassKernelWidth);
   theHighPassMatrix = 0;
   theHighPassMatrix[theHighPassKernelWidth>>1][theHighPassKernelWidth>>1] = 1;
   
   // adjust the gain for the high pass filter
   //
   NEWMAT::Matrix high =  theHighPassMatrix;

   ossim_float64 kernelW2 = theHighPassKernelWidth*theHighPassKernelWidth;
   double gain = computeParameterOffset(HIGH_PASS_GAIN_OFFSET)*(kernelW2);
   double multiplier = gain/(kernelW2);
   high = -multiplier;
   ossim_int32 cx = theHighPassKernelWidth>>1;
   ossim_int32 cy = theHighPassKernelWidth>>1;

   if(gain > FLT_EPSILON)
   {
      high[cy][cx] = multiplier* ( (kernelW2-1)+kernelW2/gain);
   }
   else
   {
      high = 0.0;
      high[cy][cx] = 1.0;
   }

   theHighPassFilter->setConvolution(high);
}

void ossimSFIMFusion::initAdjustableParameters()
{
   resizeAdjustableParameterArray(NUMBER_OF_ADJUSTABLE_PARAMETERS);

   setAdjustableParameter(HIGH_PASS_GAIN_OFFSET,
                          -1.0);
   setParameterDescription(HIGH_PASS_GAIN_OFFSET,
                           "High pass gain");
   setParameterSigma(HIGH_PASS_GAIN_OFFSET,
                     1);
   setParameterCenter(HIGH_PASS_GAIN_OFFSET,
                      1.0);

   setAdjustableParameter(LOW_PASS_WIDTH_OFFSET,
                          -1);
   setParameterDescription(LOW_PASS_WIDTH_OFFSET,
                           "Low pass kernel width");
   setParameterSigma(LOW_PASS_WIDTH_OFFSET,
                     40);
   setParameterCenter(LOW_PASS_WIDTH_OFFSET,
                      40.5);
   

   setParameterOffset(LOW_PASS_WIDTH_OFFSET,
                      1.5);
}

void ossimSFIMFusion::adjustableParametersChanged()
{
//   std::cout << "Parameter offset = " << computeParameterOffset(2) << std::endl;
   theLowPassKernelWidth = (ossim_uint32)(ossim::round<int>(computeParameterOffset(LOW_PASS_WIDTH_OFFSET)));
}

void ossimSFIMFusion::setProperty(ossimRefPtr<ossimProperty> property)
{
   ossimString name = property->getName();
   if(name=="low_pass_kernel_width")
   {
      setParameterOffset(LOW_PASS_WIDTH_OFFSET,
                         property->valueToString().toDouble(),
                         true);
   }
   else if(name=="high_pass_gain")
   {
      setParameterOffset(HIGH_PASS_GAIN_OFFSET,
                         property->valueToString().toDouble(),
                         true);
   }
   else if(name=="auto_adjust_scales")
   {
      theAutoAdjustScales =  property->valueToString().toBool();
   }
   else
   {
      ossimFusionCombiner::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimSFIMFusion::getProperty(const ossimString& name)const
{
   if(name == "low_pass_kernel_width")
   {
      ossimNumericProperty* prop =  new ossimNumericProperty(name, 
                                      ossimString::toString(computeParameterOffset(LOW_PASS_WIDTH_OFFSET)),
                                      getParameterCenter(LOW_PASS_WIDTH_OFFSET)-getParameterSigma(LOW_PASS_WIDTH_OFFSET),
                                      getParameterCenter(LOW_PASS_WIDTH_OFFSET)+getParameterSigma(LOW_PASS_WIDTH_OFFSET));
      prop->setCacheRefreshBit();
      return prop;
   }
   else if(name == "high_pass_gain")
   {
      ossimNumericProperty* prop =  new ossimNumericProperty(name, 
                                      ossimString::toString(computeParameterOffset(HIGH_PASS_GAIN_OFFSET)),
                                      getParameterCenter(HIGH_PASS_GAIN_OFFSET)-getParameterSigma(HIGH_PASS_GAIN_OFFSET),
                                      getParameterCenter(HIGH_PASS_GAIN_OFFSET)+getParameterSigma(HIGH_PASS_GAIN_OFFSET));
      prop->setCacheRefreshBit();
      return prop;
   }
   else if(name=="auto_adjust_scales")
   {
      ossimBooleanProperty* prop = new ossimBooleanProperty(name,theAutoAdjustScales);
      prop->setCacheRefreshBit();
      return prop;
   }
   
   return ossimFusionCombiner::getProperty(name);
}

void ossimSFIMFusion::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimFusionCombiner::getPropertyNames(propertyNames);
   propertyNames.push_back("low_pass_kernel_width");
   propertyNames.push_back("high_pass_gain");
   propertyNames.push_back("auto_adjust_scales");
}

bool ossimSFIMFusion::saveState(ossimKeywordlist& kwl,
                                            const char* prefix) const
{
   ossimFusionCombiner::saveState(kwl, prefix);
   saveAdjustments(kwl, prefix);
   kwl.add(prefix,
           "auto_adjust_scales",
           theAutoAdjustScales,
           true);
   
   return true;
}

bool ossimSFIMFusion::loadState(const ossimKeywordlist& kwl,
                                            const char* prefix)
{
   ossimFusionCombiner::loadState(kwl, prefix);
   loadAdjustments(kwl, prefix);
   adjustableParametersChanged();
   ossimString autoAdjustScales = kwl.find(prefix, "auto_adjust_scales");
                                    
   if(!autoAdjustScales.empty())
   {
      theAutoAdjustScales = autoAdjustScales.toBool();
   }
   return true;
}
