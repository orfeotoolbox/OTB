//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// LICENSE:  See top level LICENSE.txt
// 
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimLocalCorrelationFusion.cpp 15833 2009-10-29 01:41:53Z eshirschorn $
#include <ossim/imaging/ossimLocalCorrelationFusion.h>
#include <ossim/matrix/newmat.h>
#include <ossim/matrix/newmatio.h>
#include <ossim/base/ossim2dLinearRegression.h>
#include <ossim/base/ossimNormRgbVector.h>
#include <ossim/base/ossimHsiVector.h>
#include <ossim/imaging/ossimImageDataFactory.h>

RTTI_DEF2(ossimLocalCorrelationFusion, "ossimLocalCorrelationFusion", ossimFusionCombiner, ossimAdjustableParameterInterface);


static const ossim_uint32 REGRESSION_COEFFICIENT_ATTENUATOR_OFFSET = 0;
static const ossim_uint32 REGRESSION_COEFFICIENT_CLAMP_OFFSET = 1;
static const ossim_uint32 HIGH_PASS_GAIN_OFFSET = 2;
static const ossim_uint32 PAN_BLURR_WIDTH_OFFSET = 3;
static const ossim_uint32  NUMBER_OF_ADJUSTABLE_PARAMETERS = 4;

ossimLocalCorrelationFusion::ossimLocalCorrelationFusion()
   :theBlurrKernelWidth(1.5),
    theHighPassKernelWidth(3),
    theRegressionWidth(5)
{
   theLowPassFilter  = new ossimImageGaussianFilter;
   theHighPassFilter = new ossimConvolutionSource;

   setFilters();

   initAdjustableParameters();
}

ossimLocalCorrelationFusion::~ossimLocalCorrelationFusion()
{
}

ossimRefPtr<ossimImageData> ossimLocalCorrelationFusion::getTile(const ossimIrect& rect,
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

   ossim_int32 regressionW2 = (ossim_int32)ceil(theRegressionWidth/2.0);
   
   ossimIrect expandedRegressionRect(rect.ul().x - regressionW2,
                                     rect.ul().y - regressionW2,
                                     rect.lr().x + regressionW2,
                                     rect.lr().y + regressionW2);
   
   if(!theNormLowPassTile.valid())
   {
      theNormLowPassTile = new ossimImageData(this,
                                              OSSIM_NORMALIZED_FLOAT,
                                              1,
                                              expandedRegressionRect.width(),
                                              expandedRegressionRect.height());
      theNormHighPassTile = new ossimImageData(this,
                                              OSSIM_NORMALIZED_FLOAT,
                                               1,
                                               expandedRegressionRect.width(),
                                               expandedRegressionRect.height());
      theNormLowPassTile->initialize();
      theNormHighPassTile->initialize();
      theNormLowPassTile->makeBlank();
      theNormHighPassTile->makeBlank();
   }

   theNormLowPassTile->setImageRectangle(expandedRegressionRect);
   theNormHighPassTile->setImageRectangle(expandedRegressionRect);
   theNormLowPassTile->makeBlank();
   theNormHighPassTile->makeBlank();

   if(!theLowPassFilter->getInput() && getInput())
   {
      initialize();
   }

   ossimRefPtr<ossimImageData> lowTile  = theLowPassFilter->getTile(expandedRegressionRect, resLevel);
   ossimRefPtr<ossimImageData> highTile = theHighPassFilter->getTile(expandedRegressionRect, resLevel);
//   ossimRefPtr<ossimImageData> highTile = getNormIntensity(expandedRegressionRect, resLevel);
   ossimIpt ul = rect.ul();
   ossimIpt origin;

   // if we don't have valid low and high pass then return the input color tile
   // in its original format
   //
   if(!lowTile.valid()||!highTile.valid())
   {
      return theInputConnection->getTile(rect, resLevel);
   }

   if((lowTile->getDataObjectStatus() == OSSIM_EMPTY)||
      (!lowTile->getBuf()) ||
      (highTile->getDataObjectStatus() == OSSIM_EMPTY)||
      (!highTile->getBuf()))
   {
      return theInputConnection->getTile(rect, resLevel);
   }

   ossimRefPtr<ossimImageData> normColorData = getNormTile(expandedRegressionRect, resLevel);
   
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
   
   ossim_float64 slopeResult = 0.0;
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
   double panAttenuator = computeParameterOffset(REGRESSION_COEFFICIENT_ATTENUATOR_OFFSET);
   double delta = 0.0;
   ossim_uint32 bandsSize = (ossim_uint32)bands.size();
   ossim_float64 slopeClamp = computeParameterOffset(REGRESSION_COEFFICIENT_CLAMP_OFFSET);
   ossim_float64 minSlope = -slopeClamp;
   ossim_float64 maxSlope = slopeClamp;
   for(y = 0; y < h; ++y)
   {
      origin.y = ul.y + y;
      for(x = 0; x < w; ++x)
      {
         origin.x = ul.x + x;
         
//          if(computeRegression(slopeResult,
//                               origin,
//                               theNormLowPassTile,
//                               normColorData,
//                               0))
//          {
//             delta = panAttenuator*slopeResult*(*panHigh - *panLow);
            
         for(idx = 0; idx < bandsSize; ++idx)
         {
            if(*bands[idx] != 0.0) // if band is not null
            {
               if(computeRegression(slopeResult,
                                    origin,
                                    theNormLowPassTile,
                                    normColorData,
                                    idx))
               {
                  if(slopeResult < minSlope) slopeResult = minSlope;
                  if(slopeResult > maxSlope) slopeResult = maxSlope;
                  delta = panAttenuator*slopeResult*(*panHigh - *panLow);
                  ossim_float32 normMinPix = (ossim_float32)normColorOutputData->getMinPix(idx);
                  *bands[idx] += delta;
                  if(*bands[idx] > 1.0) *bands[idx] = 1.0;
                  if(*bands[idx] < normMinPix) *bands[idx] = normMinPix;
               }
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

void ossimLocalCorrelationFusion::initialize()
{
   ossimFusionCombiner::initialize();
   if(!theIntensityConnection)
   {
      theLowPassFilter->disconnect();
      theHighPassFilter->disconnect();
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

void ossimLocalCorrelationFusion::setFilters()
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

bool ossimLocalCorrelationFusion::computeRegression(ossim_float64& slopeResult,
                                                    const ossimIpt& origin,
                                                    const ossimRefPtr<ossimImageData> panData,
                                                    const ossimRefPtr<ossimImageData> colorData,
                                                    ossim_uint32 colorBandIdx)
{
   bool result = false;
   slopeResult = 0.0;
   if(panData.valid()&&colorData.valid())
   {
      ossim_int32 regW2 = (ossim_int32)(theRegressionWidth>>1);
      ossimIrect rect = panData->getImageRectangle();
      const ossim_float32* colorDataPtr  = (const ossim_float32*)colorData->getBuf(colorBandIdx);
      const ossim_float32* colorDataPtr2 = (const ossim_float32*)colorData->getBuf(colorBandIdx);
      
//      const ossim_float32* colorDataPtrBands[3]={0};
      
//       if(colorData->getNumberOfBands() <2)
//       {
//          colorDataPtrBands[0] = (ossim_float32*)colorData->getBuf(0);
//       }
//       else if(colorData->getNumberOfBands() <3)
//       {
//          colorDataPtrBands[0] = (ossim_float32*)colorData->getBuf(0);
//          colorDataPtrBands[1] = (ossim_float32*)colorData->getBuf(1);
//       }
//       else
//       {
//          colorDataPtrBands[0] = (ossim_float32*)colorData->getBuf(0);
//          colorDataPtrBands[1] = (ossim_float32*)colorData->getBuf(1);
//          colorDataPtrBands[2] = (ossim_float32*)colorData->getBuf(2);
//       }
      const ossim_float32* panDataPtr  = (const ossim_float32*)panData->getBuf();
      const ossim_float32* panDataPtr2 = (const ossim_float32*)panData->getBuf();
//      if(!panDataPtr || !colorDataPtrBands[0]) return result;
      if(!panDataPtr || !colorDataPtr) return result;
      
      ossim_float64 panNp   = panData->getNullPix(0);
      ossim_float64 colorNp = colorData->getNullPix(0);
      ossim_int32 w = (ossim_int32)rect.width();
      ossim_int32 offset = (ossim_int32)( ((origin.y-rect.ul().y) - regW2)*w +
                                          ((origin.x-rect.ul().x) - regW2));

      ossim_uint32 y = 0;
      ossim_uint32 x = 0;
      ossim_uint32 idx = 0;
      ossim2dLinearRegression regression;
      ossim_uint32 offsetColorBand;

      for(y = 0; y < theRegressionWidth; ++y)
      {
         offsetColorBand = (offset + y*w);
         panDataPtr   = panDataPtr2   + offsetColorBand;
         colorDataPtr = colorDataPtr2 + offsetColorBand;
         for(x = 0; x < theRegressionWidth; ++x)
         {
//             if((*panDataPtr != panNp)&&
//                (colorDataPtrBands[0][offsetColorBand] != colorNp)&&
//                (colorDataPtrBands[1][offsetColorBand] != colorNp)&&
//                (colorDataPtrBands[2][offsetColorBand] != colorNp))
//             {
            if((*panDataPtr != panNp)&&
               (*colorDataPtr != colorNp))
            {
//                ossimNormRgbVector rgbV(colorDataPtrBands[0][offsetColorBand],
//                                        colorDataPtrBands[1][offsetColorBand],
//                                        colorDataPtrBands[2][offsetColorBand]);
//                ossimHsiVector hsi(rgbV);
               
               regression.addPoint(ossimDpt(*panDataPtr, *colorDataPtr));
//               regression.addPoint(ossimDpt(*panDataPtr, hsi.getI()));
               ++idx;
            }
            ++panDataPtr;
//            ++offsetColorBand;
               ++colorDataPtr;
         }
      }
      if(idx > 2)
      {
         double slope, intercept;
         result = true;
         regression.solve();
         regression.getEquation(slope,
                                intercept);
         
         slopeResult = slope;
      }
   }

   return result;
}

void ossimLocalCorrelationFusion::initAdjustableParameters()
{
   resizeAdjustableParameterArray(NUMBER_OF_ADJUSTABLE_PARAMETERS);

   setAdjustableParameter(REGRESSION_COEFFICIENT_ATTENUATOR_OFFSET,
                          0.0);
   setParameterDescription(REGRESSION_COEFFICIENT_ATTENUATOR_OFFSET,
                           "Attenuator");
   setParameterSigma(REGRESSION_COEFFICIENT_ATTENUATOR_OFFSET,
                     2.0);
   setParameterCenter(REGRESSION_COEFFICIENT_ATTENUATOR_OFFSET,
                      1.0);

   
   setAdjustableParameter(REGRESSION_COEFFICIENT_CLAMP_OFFSET,
                          0.0);
   setParameterDescription(REGRESSION_COEFFICIENT_CLAMP_OFFSET,
                           "Regression clamp");
   setParameterSigma(REGRESSION_COEFFICIENT_CLAMP_OFFSET,
                     10.0);
   setParameterCenter(REGRESSION_COEFFICIENT_CLAMP_OFFSET,
                      10.0);
   
   
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
   setParameterOffset(REGRESSION_COEFFICIENT_CLAMP_OFFSET,
                      4.0);
}

void ossimLocalCorrelationFusion::adjustableParametersChanged()
{
//   std::cout << "Parameter offset = " << computeParameterOffset(2) << std::endl;
   theBlurrKernelWidth = (ossim_uint32)(ossim::round<int>(computeParameterOffset(PAN_BLURR_WIDTH_OFFSET)));
}


bool ossimLocalCorrelationFusion::saveState(ossimKeywordlist& kwl,
                                            const char* prefix) const
{
   ossimFusionCombiner::saveState(kwl, prefix);
   saveAdjustments(kwl, prefix);

   return true;
}

bool ossimLocalCorrelationFusion::loadState(const ossimKeywordlist& kwl,
                                            const char* prefix)
{
   ossimFusionCombiner::loadState(kwl, prefix);
   loadAdjustments(kwl, prefix);
   adjustableParametersChanged();
   
   return true;
}
