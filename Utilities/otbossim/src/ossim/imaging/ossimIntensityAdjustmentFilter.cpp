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
// $Id: ossimIntensityAdjustmentFilter.cpp 13330 2008-07-28 18:04:40Z dburken $
#include <ossim/imaging/ossimIntensityAdjustmentFilter.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimHsvVector.h>
#include <ossim/base/ossimRgbVector.h>

RTTI_DEF1(ossimIntensityAdjustmentFilter, "ossimIntensityAdjustmentFilter", ossimImageSourceFilter);

ossimIntensityAdjustmentFilter::ossimIntensityAdjustmentFilter()
   :ossimImageSourceFilter(),
    theMeanIntensityTarget(ossim::nan()),
    theNormTile(NULL),
    theTile(NULL),
    theBlankTile(NULL)
{
   theGridBounds.makeNan();
}

ossimIntensityAdjustmentFilter::~ossimIntensityAdjustmentFilter()
{
}

ossimRefPtr<ossimImageData> ossimIntensityAdjustmentFilter::getTile(
   const ossimIrect& rect,
   ossim_uint32 resLevel)
{
   if (!theTile.valid())
   {
      allocate(); // first time through.
   }
   
   if(theBlankTile.valid())
   {
      theBlankTile->setImageRectangle(rect);
   }
   if(!theInputConnection)
   {
      return theBlankTile;
   }
   ossimRefPtr<ossimImageData> data =
      theInputConnection->getTile(rect, resLevel);
   
   if(!isSourceEnabled())
   {
      return data;
   }
   theTile->setImageRectangle(rect);
   theNormTile->setImageRectangle(rect);
   loadNormTile(data);

   if(ossim::isnan(theMeanIntensityTarget))
   {
      theMeanIntensityTarget = theMeanIntensityGrid.meanValue();
   }

   loadNormTile(data);
   
   if((theNormTile->getDataObjectStatus()!=OSSIM_NULL)&&
      (theNormTile->getDataObjectStatus()!=OSSIM_EMPTY))
   {
      float* buf[3];
      if(data->getNumberOfBands() == 3)
      {
         buf[0] = (float*)theNormTile->getBuf(0);
         buf[1] = (float*)theNormTile->getBuf(1);
         buf[2] = (float*)theNormTile->getBuf(2);
      }
      else
      {
         buf[0] = (float*)theNormTile->getBuf(0);
         buf[1] = (float*)theNormTile->getBuf(0);
         buf[2] = (float*)theNormTile->getBuf(0);
         
      }
      ossim_int32 y = 0;
      ossim_int32 x = 0;
      ossim_int32 upperY = theNormTile->getHeight();
      ossim_int32 upperX = theNormTile->getWidth();
      ossimIpt origin = rect.ul();
      
      for(y = 0; y < upperY; ++y)
      {
         for(x = 0; x < upperX; ++x)
         {
            ossimIpt pt = ossimIpt(origin.x + x,
                                   origin.y + y) - theGridBounds.ul();

            if((buf[0] != 0) &&
               (buf[1] != 0) &&
               (buf[2] != 0))
            {
               
               ossimRgbVector rgb((ossim_uint8)(*(buf[0])*255.0),
                                  (ossim_uint8)(*(buf[1])*255.0),
                                  (ossim_uint8)(*(buf[2])*255.0));
               ossimHsvVector hsv(rgb);
               
               hsv.setV(matchTargetMean(hsv.getV(),
                                        theMeanIntensityGrid(pt.x, pt.y),
                                        theMeanIntensityTarget,
                                        theNormTile->getMinPix(0),
                                        theNormTile->getMaxPix(0)));
               ossimRgbVector result(hsv);
               
               *buf[0] = result.getR()/255.0;
               *buf[1] = result.getG()/255.0;
               *buf[2] = result.getB()/255.0;
            }
            
            ++buf[0];
            ++buf[1];
            ++buf[2];
         }
      }
   }
   theTile->copyNormalizedBufferToTile((float*)theNormTile->getBuf());
   theTile->validate();
   
   return theTile;
}

void ossimIntensityAdjustmentFilter::initialize()
{
   ossimImageSourceFilter::initialize();

   theNormTile = NULL;
   theBlankTile = NULL;
   
   if(theInputConnection)
   {
      theGridBounds = theInputConnection->getBoundingRect();
   }
   else
   {
      theGridBounds.makeNan();
   }
}

void ossimIntensityAdjustmentFilter::allocate()
{
   theBlankTile = ossimImageDataFactory::instance()->create(this, this);
   theTile = (ossimImageData*)theBlankTile->dup();
   theTile->initialize();
}

void ossimIntensityAdjustmentFilter::createAndPopulateGrid(const ossimIpt& spacing,
                                                           double targetMean)
{
   if(theInputConnection)
   {
      ossim_uint32 x = 0;
      ossim_uint32 y = 0;

      theGridBounds        = theInputConnection->getBoundingRect();

      ossimDrect rect(0,
                      0,
                      theGridBounds.width()-1,
                      theGridBounds.height()-1);
                      
      theMeanIntensityGrid = ossimDblGrid(rect,
                                          ossimDpt(spacing),
                                          0);

      for(y = 0; y <= theGridBounds.height(); y+=spacing.y)
      {
         for(x = 0; x <= theGridBounds.width(); x+=spacing.x)
         {
            ossimIpt ul(x - 16,
                        y - 16);
            
            ossimIrect sampleRect(ul.x,
                                  ul.y,
                                  ul.x + 31,
                                  ul.y + 31);
            ossimRefPtr<ossimImageData> data = theInputConnection->getTile(sampleRect);
            double mean = computeMeanIntensity(data);
            theMeanIntensityGrid.setNearestNode(ossimDpt(x, y), mean);
         }
      }
//      theMeanIntensityGrid.interpolateNullValuedNodes();
   }
   theMeanIntensityTarget = targetMean;
}

double ossimIntensityAdjustmentFilter::computeMeanIntensity(
   ossimRefPtr<ossimImageData>& data)
{
   double result  = 0;
   double divisor = 0;

   if(data.valid() &&
      (data->getDataObjectStatus()!=OSSIM_NULL)&&
      (data->getDataObjectStatus()!=OSSIM_EMPTY))
   {
      loadNormTile(data);

      int i = 0;
      int upper = data->getWidth()*data->getHeight();
      float* buf[3];
      if(data->getNumberOfBands() == 3)
      {
         buf[0] = (float*)theNormTile->getBuf(0);
         buf[1] = (float*)theNormTile->getBuf(1);
         buf[2] = (float*)theNormTile->getBuf(2);
      }
      else
      {
         buf[0] = (float*)theNormTile->getBuf();
         buf[1] = (float*)theNormTile->getBuf();
         buf[2] = (float*)theNormTile->getBuf();
      }
      for(i = 0; i < upper; ++i)
      {
         ossimRgbVector rgb((ossim_uint8)(*(buf[0])*255.0),
                            (ossim_uint8)(*(buf[1])*255.0),
                            (ossim_uint8)(*(buf[2])*255.0));
         ossimHsvVector hsv(rgb);
         if(hsv.getV() > 0.0)
         {
            result += hsv.getV();
            divisor += 1.0;
         }
         ++buf[0];
         ++buf[1];
         ++buf[2];
      }
   }

   if(divisor > 0.0)
   {
      result /= divisor;
   }
   return result;
}

void ossimIntensityAdjustmentFilter::loadNormTile(ossimRefPtr<ossimImageData>& data)
{
   if(!theNormTile)
   {
      theNormTile = ossimImageDataFactory::instance()->create(this,
                                                              OSSIM_NORMALIZED_FLOAT,
                                                              data->getNumberOfBands(),
                                                              data->getWidth(),
                                                              data->getHeight());
      theNormTile->initialize();
   }
   else
   {
      theNormTile->setImageRectangle(data->getImageRectangle());
   }
   data->copyTileToNormalizedBuffer((float*)theNormTile->getBuf());
   theNormTile->validate();
}

double ossimIntensityAdjustmentFilter::matchTargetMean(double inputValue,
                                                       double meanValue,
                                                       double targetMean,
                                                       double minValue,
                                                       double maxValue)
{
    // max change
   const double delta = targetMean - meanValue;
   double weight=0.0;
   
   // weight max change dependend on proximity to end points
   if (inputValue <= meanValue)
   {
      weight = fabs((inputValue - minValue) / (meanValue - minValue));
   }
   else
   {
      weight = fabs((maxValue - inputValue) / (maxValue - meanValue));
   }

   return  (inputValue + (delta * weight));
}
