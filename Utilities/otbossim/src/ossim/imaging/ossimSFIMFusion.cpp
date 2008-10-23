//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// LICENSE:  See top level LICENSE.txt
// 
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimSFIMFusion.cpp 11347 2007-07-23 13:01:59Z gpotts $
#include <ossim/imaging/ossimSFIMFusion.h>
#include <ossim/matrix/newmat.h>
#include <ossim/matrix/newmatio.h>
#include <ossim/base/ossim2dLinearRegression.h>
#include <ossim/base/ossimNormRgbVector.h>
#include <ossim/base/ossimHsiVector.h>
#include <ossim/imaging/ossimImageDataFactory.h>

RTTI_DEF2(ossimSFIMFusion, "ossimSFIMFusion", ossimFusionCombiner, ossimAdjustableParameterInterface);


static const ossim_uint32 HIGH_PASS_GAIN_OFFSET = 0;
static const ossim_uint32 PAN_BLURR_WIDTH_OFFSET = 1;
static const ossim_uint32  NUMBER_OF_ADJUSTABLE_PARAMETERS = 2;

ossimSFIMFusion::ossimSFIMFusion()
   :theBlurrKernelWidth(1.5),
    theHighPassKernelWidth(3)
{
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
   
   ossim_uint32 y = 0;
   ossim_uint32 x = 0;
   ossim_uint32 w = theTile->getWidth();
   ossim_uint32 h = theTile->getHeight();

   theTile->makeBlank();
   theTile->setImageRectangle(rect);
   
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
   ossim_uint32 bandsSize = bands.size();
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
            else
            {
               *bands[idx] = 0.0;
            }
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
      setFilters();
      theLowPassFilter->initialize();
      theHighPassFilter->initialize();
   }
}

void ossimSFIMFusion::setFilters()
{
   theLowPassFilter->setGaussStd(theBlurrKernelWidth);
   
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

   setAdjustableParameter(PAN_BLURR_WIDTH_OFFSET,
                          -1);
   setParameterDescription(PAN_BLURR_WIDTH_OFFSET,
                           "Blurring kernel width");
   setParameterSigma(PAN_BLURR_WIDTH_OFFSET,
                     7);
   setParameterCenter(PAN_BLURR_WIDTH_OFFSET,
                      7.5);
   

   setParameterOffset(PAN_BLURR_WIDTH_OFFSET,
                      1.5);
}

void ossimSFIMFusion::adjustableParametersChanged()
{
//   std::cout << "Parameter offset = " << computeParameterOffset(2) << std::endl;
   theBlurrKernelWidth = (ossim_uint32)(ossim::round<int>(computeParameterOffset(PAN_BLURR_WIDTH_OFFSET)));
}


bool ossimSFIMFusion::saveState(ossimKeywordlist& kwl,
                                            const char* prefix) const
{
   ossimFusionCombiner::saveState(kwl, prefix);
   saveAdjustments(kwl, prefix);

   return true;
}

bool ossimSFIMFusion::loadState(const ossimKeywordlist& kwl,
                                            const char* prefix)
{
   ossimFusionCombiner::loadState(kwl, prefix);
   loadAdjustments(kwl, prefix);
   adjustableParametersChanged();
   
   return true;
}
