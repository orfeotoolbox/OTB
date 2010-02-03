//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// LICENSE:  See top level LICENSE.txt
// 
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimLocalCorrelationFusion.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimLocalCorrelationFusion_HEADER
#define ossimLocalCorrelationFusion_HEADER
#include <ossim/imaging/ossimFusionCombiner.h>
#include <ossim/imaging/ossimConvolutionSource.h>
#include <ossim/imaging/ossimImageGaussianFilter.h>
#include <ossim/base/ossimAdjustableParameterInterface.h>

/**
  * This implements a Local correlation fusion based on the paper:
  *
  *  "A Local Correlation Approach For the Fusion of Remote Sensing Data With Different
  *   Spatial Resolution In Forestry Applications"
  *   by J. Hill, C. Diemer, O. Stover, and Th. Udelhoven
  *   Published in:
  *       International Archives of Photogrammetry and Remote Sensing, Vol 32, Part 7-4-3 W6,
  *       Valladolid, Spain, 3-4 June, 1999
  *
  *   A new band is computed by the formula:
  *
  *   ColorHigh = ColorLow + RegressionSlope*(PanHigh-PanLow);
  *
  *   Adjustable paraemters have been added to clamp the regression slope and attenuate
  *   colorLowOffset value and blurring the Pan.
  *
  *   ColorHigh = ColorLow + Clamp(RegressionSlope)*Attenuator(PanHigh-PanLow);
  *
  *   The regression coefficient is coputed for each color band.
  */
class OSSIM_DLL ossimLocalCorrelationFusion : public ossimFusionCombiner,
                                              public ossimAdjustableParameterInterface
{
public:
   ossimLocalCorrelationFusion();
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32 resLevel=0);
   virtual void initialize();

   virtual ossimObject* getBaseObject()
   {
      return this;
   }
   virtual const ossimObject* getBaseObject()const
   {
      return this;
   }
   virtual void initAdjustableParameters();
   virtual void adjustableParametersChanged();
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
protected:
   virtual ~ossimLocalCorrelationFusion();
   void setFilters();
   bool computeRegression(ossim_float64& slopeResult,
                          const ossimIpt& origin,
                          ossimRefPtr<ossimImageData> panData,
                          ossimRefPtr<ossimImageData> colorData,
                          ossim_uint32 colorBandIdx);
   
   ossim_float64 theBlurrKernelWidth;
   ossim_uint32 theHighPassKernelWidth;
   ossim_uint32 theRegressionWidth;
   // These are low and high pass filters for the single pan band
   //
   ossimRefPtr<ossimImageGaussianFilter> theLowPassFilter;
   ossimRefPtr<ossimConvolutionSource> theHighPassFilter;
   ossimRefPtr<ossimImageData>         theNormLowPassTile;
   ossimRefPtr<ossimImageData>         theNormHighPassTile;
   ossimRefPtr<ossimImageData>         theNormColorData;

   NEWMAT::Matrix                      theHighPassMatrix;

TYPE_DATA   
};

#endif
