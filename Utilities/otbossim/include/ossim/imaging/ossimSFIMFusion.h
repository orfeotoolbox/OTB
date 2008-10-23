//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// LICENSE:  See top level LICENSE.txt
// 
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimSFIMFusion.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimSFIMFusion_HEADER
#define ossimSFIMFusion_HEADER
#include <ossim/imaging/ossimFusionCombiner.h>
#include <ossim/imaging/ossimConvolutionSource.h>
#include <ossim/imaging/ossimImageGaussianFilter.h>
#include <ossim/base/ossimAdjustableParameterInterface.h>

/**
 * This class imlements the fusion algorithm from the paper:
 *
 * "Smoothing Filter-based Intesity Modulation: a spectral preserve
 *  image fusion technique for improving spatial details"
 *
 *  Pulished in INT. J. Remote Sensing, 2000, Vol. 21 NO. 18, 3461-3472
 *
 *  By J. G. LIU
 */
class OSSIM_DLL ossimSFIMFusion : public ossimFusionCombiner,
                                  public ossimAdjustableParameterInterface
{
public:
   ossimSFIMFusion();
   virtual ~ossimSFIMFusion();
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
   void setFilters();
   bool computeRegression(ossim_float64& slopeResult,
                          const ossimIpt& origin,
                          ossimRefPtr<ossimImageData> panData,
                          ossimRefPtr<ossimImageData> colorData,
                          ossim_uint32 colorBandIdx);
   
   ossim_float64 theBlurrKernelWidth;
   ossim_uint32 theHighPassKernelWidth;
   // These are low and high pass filters for the single pan band
   //
   ossimRefPtr<ossimImageGaussianFilter> theLowPassFilter;
   ossimRefPtr<ossimConvolutionSource>   theHighPassFilter;
   ossimRefPtr<ossimImageData>           theNormLowPassTile;
   ossimRefPtr<ossimImageData>           theNormHighPassTile;
   ossimRefPtr<ossimImageData>           theNormColorData;

   NEWMAT::Matrix                        theHighPassMatrix;

TYPE_DATA   
};

#endif
