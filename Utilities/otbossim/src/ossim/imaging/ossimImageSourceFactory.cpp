//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: This is the image source factory.
//              It will try to create image filters
//              first then it will try the image writers
//              and image handlers (image readers)
//
//*************************************************************************
// $Id: ossimImageSourceFactory.cpp 19005 2011-03-03 15:56:58Z gpotts $

#include <ossim/imaging/ossimImageSourceFactory.h>
#include <ossim/imaging/ossimConvolutionFilter1D.h>
#include <ossim/imaging/ossimImageGaussianFilter.h>
#include <ossim/imaging/ossimImageRenderer.h>
#include <ossim/imaging/ossimCacheTileSource.h>
#include <ossim/imaging/ossimFeatherMosaic.h>
#include <ossim/imaging/ossimHistogramRemapper.h>
#include <ossim/imaging/ossimNullPixelFlip.h>
#include <ossim/imaging/ossimImageMosaic.h>
#include <ossim/imaging/ossimClosestToCenterCombiner.h>
#include <ossim/imaging/ossimBlendMosaic.h>
#include <ossim/imaging/ossimMaxMosaic.h>
#include <ossim/imaging/ossimOrthoImageMosaic.h>
#include <ossim/imaging/ossimColorNormalizedFusion.h>
#include <ossim/imaging/ossimLocalCorrelationFusion.h>
#include <ossim/imaging/ossimSFIMFusion.h>
#include <ossim/imaging/ossimTopographicCorrectionFilter.h>
#include <ossim/imaging/ossimBandSelector.h>
#include <ossim/imaging/ossimNBandToIndexFilter.h>
#include <ossim/imaging/ossimRgbToGreyFilter.h>
#include <ossim/imaging/ossimRgbToJpegYCbCrSource.h>
#include <ossim/imaging/ossimRgbToHsvSource.h>
#include <ossim/imaging/ossimRgbToHsiSource.h>
#include <ossim/imaging/ossimHsiToRgbSource.h>
#include <ossim/imaging/ossimHsvToRgbSource.h>
#include <ossim/imaging/ossimHsiRemapper.h>
#include <ossim/imaging/ossimJpegYCbCrToRgbSource.h>
#include <ossim/imaging/ossimPolyCutter.h>
#include <ossim/imaging/ossimGeoPolyCutter.h>
#include <ossim/imaging/ossimHistogramEqualization.h>
#include <ossim/imaging/ossimHistogramMatchFilter.h>
#include <ossim/imaging/ossimHistogramThreshholdFilter.h>
#include <ossim/imaging/ossimConvolutionSource.h>
#include <ossim/imaging/ossimSICDToDetectedImage.h>
#include <ossim/imaging/ossimBrightnessContrastSource.h>
#include <ossim/imaging/ossimBrightnessMatch.h>
#include <ossim/imaging/ossimScalarRemapper.h>
#include <ossim/imaging/ossimBandMergeSource.h>
#include <ossim/imaging/ossimIndexToRgbLutFilter.h>
#include <ossim/imaging/ossimBumpShadeTileSource.h>
#include <ossim/imaging/ossimSubImageTileSource.h>
#include <ossim/imaging/ossimRLevelFilter.h>
#include <ossim/imaging/ossimBandClipFilter.h>
#include <ossim/imaging/ossimValueAssignImageSourceFilter.h>
#include <ossim/imaging/ossimImageSharpenFilter.h>
#include <ossim/imaging/ossim3x3ConvolutionFilter.h>
#include <ossim/imaging/ossimCastTileSourceFilter.h>
#include <ossim/imaging/ossimEquationCombiner.h>
#include <ossim/imaging/ossimImageGaussianFilter.h>
#include <ossim/imaging/ossimMaskFilter.h>
#include <ossim/imaging/ossimPixelFlipper.h>
#include <ossim/imaging/ossimElevImageSource.h>
#include <ossim/imaging/ossimLandsatTopoCorrectionFilter.h>
#include <ossim/imaging/ossimGeoAnnotationSource.h>
#include <ossim/imaging/ossimMapCompositionSource.h>
#include <ossim/imaging/ossimScaleFilter.h>
#include <ossim/imaging/ossimEastingNorthingCutter.h>
#include <ossim/imaging/ossimEdgeFilter.h>
#include <ossim/imaging/ossimFftFilter.h>
#include <ossim/imaging/ossimWatermarkFilter.h>
#include <ossim/imaging/ossimTrimFilter.h>
#include <ossim/imaging/ossimTwoColorView.h>
#include <ossim/imaging/ossimMemoryImageSource.h>

#include <ossim/imaging/ossimGridRemapSource.h>

// Not sure if we want to keep this here
#include <ossim/imaging/ossimAtbController.h>

#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimHistogramWriter.h>
#include <ossim/imaging/ossimRectangleCutFilter.h>


#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/imaging/ossimImageHistogramSource.h>

#include <ossim/imaging/ossimMultiBandHistogramTileSource.h>

#include <ossim/imaging/ossimBandAverageFilter.h>
#include <ossim/imaging/ossimImageToPlaneNormalFilter.h>

#include <ossim/imaging/ossimAtCorrGridRemapper.h>
#include <ossim/imaging/ossimAtCorrRemapper.h>


#include <ossim/base/ossimTrace.h>

RTTI_DEF1(ossimImageSourceFactory, "ossimImageSourceFactory", ossimImageSourceFactoryBase);

static ossimTrace traceDebug("ossimImageSourceFactory:debug");

ossimImageSourceFactory* ossimImageSourceFactory::theInstance=NULL;

ossimImageSourceFactory::~ossimImageSourceFactory()
{
   theInstance = NULL;
   ossimObjectFactoryRegistry::instance()->unregisterFactory(this);
}
ossimImageSourceFactory* ossimImageSourceFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimImageSourceFactory;
   }

   return theInstance;
}

ossimObject* ossimImageSourceFactory::createObject(const ossimString& name)const
{

   // lets do the filters first
   if( name == STATIC_TYPE_NAME(ossimBandSelector) ||
       name == "ossimNativeBandSelectorTileSource" ) 
   {
      //---
      // For backwards compatibility check old name:
      // "ossimNativeBandSelectorTileSource"
      //---
      return new ossimBandSelector;
   }
   else if(name ==  STATIC_TYPE_NAME(ossimNullPixelFlip))
   {
      return new ossimNullPixelFlip;
   }
   else if(name == STATIC_TYPE_NAME(ossimImageRenderer))
   {
      return new ossimImageRenderer;
   }
   else if(name == STATIC_TYPE_NAME(ossimCacheTileSource))
   {
      return new ossimCacheTileSource;
   }
   else if(name == STATIC_TYPE_NAME(ossimColorNormalizedFusion))
   {
      return new ossimColorNormalizedFusion;
   }
   else if(name == STATIC_TYPE_NAME(ossimLocalCorrelationFusion))
   {
      return new ossimLocalCorrelationFusion;
   }
   else if(name == STATIC_TYPE_NAME(ossimSFIMFusion))
   {
      return new ossimSFIMFusion;
   }
   else if(name == STATIC_TYPE_NAME(ossimBlendMosaic))
   {
      return new ossimBlendMosaic;
   }
   else if(name == STATIC_TYPE_NAME(ossimMaxMosaic))
   {
      return new ossimMaxMosaic;
   }
   else if(name == STATIC_TYPE_NAME(ossimHistogramRemapper))
   {
      return new ossimHistogramRemapper;
   }
   else if(name == STATIC_TYPE_NAME(ossimImageMosaic))
   {
      return new ossimImageMosaic;
   }
   else if(name == STATIC_TYPE_NAME(ossimClosestToCenterCombiner))
   {
      return new ossimClosestToCenterCombiner;
   }
   else if(name == STATIC_TYPE_NAME(ossimRgbToGreyFilter))
   {
      return new ossimRgbToGreyFilter;
   }
   else if(name == STATIC_TYPE_NAME(ossimNBandToIndexFilter))
   {
      return new ossimNBandToIndexFilter;
   }
   else if(name == STATIC_TYPE_NAME(ossimImageChain))
   {
      return new ossimImageChain;
   }
   else if(name == STATIC_TYPE_NAME(ossimPolyCutter))
   {
      return new ossimPolyCutter;
   }
   else if(name == STATIC_TYPE_NAME(ossimGeoPolyCutter))
   {
      return new ossimGeoPolyCutter;
   }
   else if(name == STATIC_TYPE_NAME(ossimJpegYCbCrToRgbSource))
   {
      return new ossimJpegYCbCrToRgbSource;
   }
   else if(name == STATIC_TYPE_NAME(ossimRgbToJpegYCbCrSource))
   {
      return new ossimRgbToJpegYCbCrSource;
   }
   else if(name == STATIC_TYPE_NAME(ossimHsiToRgbSource))
   {
      return new ossimHsiToRgbSource;
   }
   else if(name == STATIC_TYPE_NAME(ossimRgbToHsiSource))
   {
      return new ossimRgbToHsiSource;
   }
   else if(name == STATIC_TYPE_NAME(ossimHsvToRgbSource))
   {
      return new ossimHsvToRgbSource;
   }
   else if(name == STATIC_TYPE_NAME(ossimRgbToHsvSource))
   {
      return new ossimRgbToHsvSource;
   }
   else if(name == STATIC_TYPE_NAME(ossimHistogramEqualization))
   {
      return new ossimHistogramEqualization;
   }
   else if(name == STATIC_TYPE_NAME(ossimHistogramMatchFilter))
   {
      return new ossimHistogramMatchFilter;
   }
   else if(name == STATIC_TYPE_NAME(ossimHistogramThreshholdFilter))
   {
      return new ossimHistogramThreshholdFilter;
   }
   else if(name == STATIC_TYPE_NAME(ossimGeoAnnotationSource))
   {
      return new ossimGeoAnnotationSource;
   }
   else if(name == STATIC_TYPE_NAME(ossimConvolutionSource))
   {
      return new ossimConvolutionSource;
   }
   else if(name == STATIC_TYPE_NAME(ossimSICDToDetectedImage))
   {
	   return new ossimSICDToDetectedImage;
   }
   else if(name == STATIC_TYPE_NAME(ossimBrightnessContrastSource))
   {
      return new ossimBrightnessContrastSource;
   }
   else if(name == STATIC_TYPE_NAME(ossimBrightnessMatch))
   {
      return new ossimBrightnessMatch;
   }
   else if(name == STATIC_TYPE_NAME(ossimScalarRemapper))
   {
      return new ossimScalarRemapper;
   }
   else if(name == STATIC_TYPE_NAME(ossimBandMergeSource))
   {
      return new ossimBandMergeSource;
   }  
   else if(name == STATIC_TYPE_NAME(ossimFeatherMosaic))
   {
      return new ossimFeatherMosaic;
   }  
   else if(name == STATIC_TYPE_NAME(ossimGridRemapSource))
   {
      return new ossimGridRemapSource;
   }
   else if(name == STATIC_TYPE_NAME(ossimEquationCombiner))
   {
      return new ossimEquationCombiner;
   }
   else if(name == STATIC_TYPE_NAME(ossimConvolutionFilter1D))
   {
      return new ossimConvolutionFilter1D;
   }
   else if(name == STATIC_TYPE_NAME(ossimImageGaussianFilter))
   {
      return new ossimImageGaussianFilter;
   }
   // Not sure if we want to keep this here
   else if(name == STATIC_TYPE_NAME(ossimAtbController))
   {
      return new ossimAtbController;
   }  
   else if(name == STATIC_TYPE_NAME(ossimIndexToRgbLutFilter))
   {
      return new ossimIndexToRgbLutFilter;
   }
   else if(name ==  STATIC_TYPE_NAME(ossimBumpShadeTileSource))
   {
      return new ossimBumpShadeTileSource;
   }
   else if(name ==  STATIC_TYPE_NAME(ossimSubImageTileSource))
   {
      return new ossimSubImageTileSource;
   }
   else if(name ==  STATIC_TYPE_NAME(ossimRLevelFilter))
   {
      return new ossimRLevelFilter;
   }
   else if(name ==  STATIC_TYPE_NAME(ossimBandClipFilter))
   {
      return new ossimBandClipFilter;
   }
   else if(name ==  STATIC_TYPE_NAME(ossimHsiRemapper))
   {
      return new ossimHsiRemapper;
   }
   else if(name == STATIC_TYPE_NAME(ossimHistogramWriter))
   {
      return new ossimHistogramWriter;
   }
   else if(name == STATIC_TYPE_NAME(ossimImageHistogramSource))
   {
      return new ossimImageHistogramSource;
   }
   else if(name == STATIC_TYPE_NAME(ossimRectangleCutFilter))
   {
      return new ossimRectangleCutFilter;
   }
   else if(name == STATIC_TYPE_NAME(ossimCastTileSourceFilter))
   {
      return new ossimCastTileSourceFilter;
   }
   // all the image handlers
   else if(name == STATIC_TYPE_NAME(ossimValueAssignImageSourceFilter))
   {
      return new ossimValueAssignImageSourceFilter;
   }
   else if(name == STATIC_TYPE_NAME(ossimImageSharpenFilter))
   {
      return new ossimImageSharpenFilter;
   }
   else if(name == STATIC_TYPE_NAME(ossim3x3ConvolutionFilter))
   {
      return new ossim3x3ConvolutionFilter;
   }
   else if(name == STATIC_TYPE_NAME(ossimOrthoImageMosaic))
   {
      return new ossimOrthoImageMosaic;
   }
   else if(name == STATIC_TYPE_NAME(ossimMapCompositionSource))
   {
      return new ossimMapCompositionSource;
   }
   else if(name ==  STATIC_TYPE_NAME(ossimElevImageSource))
   {
      return new ossimElevImageSource;
   }
   else if(name == STATIC_TYPE_NAME(ossimMaskFilter))
   {
      return new ossimMaskFilter;
   }
   else if(name == STATIC_TYPE_NAME(ossimMultiBandHistogramTileSource))
   {
      return new ossimMultiBandHistogramTileSource;
   }
   else if(name == STATIC_TYPE_NAME(ossimBandAverageFilter))
   {
      return new ossimBandAverageFilter;
   }
   else if(name == STATIC_TYPE_NAME(ossimPixelFlipper))
   {
      return new ossimPixelFlipper();
   }
   else if(name == STATIC_TYPE_NAME(ossimScaleFilter))
   {
      return new ossimScaleFilter();
   }
   else if(name == STATIC_TYPE_NAME(ossimImageToPlaneNormalFilter))
   {
      return new ossimImageToPlaneNormalFilter();
   }
   else if(name == STATIC_TYPE_NAME(ossimTopographicCorrectionFilter))
   {
      return new ossimTopographicCorrectionFilter();
   }
   else if(name == STATIC_TYPE_NAME(ossimLandsatTopoCorrectionFilter))
   {
      return new ossimLandsatTopoCorrectionFilter();
   }
   else if(name == STATIC_TYPE_NAME(ossimAtCorrRemapper))
   {
      return new ossimAtCorrRemapper();
   }
   else if(name == STATIC_TYPE_NAME(ossimAtCorrGridRemapper))
   {
      return new ossimAtCorrGridRemapper();
   }
   else if(name == STATIC_TYPE_NAME(ossimEastingNorthingCutter))
   {
      return new ossimEastingNorthingCutter();
   }
   else if(name == STATIC_TYPE_NAME(ossimEdgeFilter))
   {
      return new ossimEdgeFilter();
   }
   else if(name == STATIC_TYPE_NAME(ossimFftFilter))
   {
      return new ossimFftFilter();
   }
   else if(name == STATIC_TYPE_NAME(ossimWatermarkFilter))
   {
      return new ossimWatermarkFilter();
   }
   else if(name == STATIC_TYPE_NAME(ossimImageGaussianFilter))
   {
      return new ossimImageGaussianFilter();
   }
   else if(name == STATIC_TYPE_NAME(ossimTrimFilter))
   {
      return new ossimTrimFilter();
   }
   else if(name == STATIC_TYPE_NAME(ossimTwoColorView))
   {
      return new ossimTwoColorView;
   }
   else if(name == STATIC_TYPE_NAME(ossimImageHistogramSource))
   {
      return new ossimImageHistogramSource();
   }
   else if(name == STATIC_TYPE_NAME(ossimImageSourceFilter))
   {
      // this is just a pass through source
      return new ossimImageSourceFilter;
   }
   else if(name == STATIC_TYPE_NAME(ossimMemoryImageSource))
   {
      // this is just a pass through source
      return new ossimMemoryImageSource;
   }
   return NULL;
}

ossimObject* ossimImageSourceFactory::createObject(const ossimKeywordlist& kwl,
                                                   const char* prefix)const
{
   static const char* MODULE = "ossimImageSourceFactory::createSource";
   
   ossimString copyPrefix;
   if (prefix)
   {
      copyPrefix = prefix;
   }
   
   ossimObject* result = NULL;
   
   if(traceDebug())
   {
      CLOG << "looking up type keyword for prefix = " << copyPrefix << endl;
   }

   const char* lookup = kwl.find(copyPrefix, "type");
   if(lookup)
   {
      ossimString name = lookup;
      result           = createObject(name);
      
      if(result)
      {
         if(traceDebug())
         {
            CLOG << "found source " << result->getClassName() << " now loading state" << endl;
         }
         result->loadState(kwl, copyPrefix.c_str());
      }
      else
      {
         if(traceDebug())
         {
            CLOG << "type not found " << lookup << endl;
         }
      }
   }
   else
   {
      if(traceDebug())
      {
         CLOG << "type keyword not found" << endl;
      }
   }
   return result;
}

void ossimImageSourceFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back(STATIC_TYPE_NAME(ossimBandSelector));
   typeList.push_back(STATIC_TYPE_NAME(ossimImageRenderer));
   typeList.push_back(STATIC_TYPE_NAME(ossimCacheTileSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimBlendMosaic));
   typeList.push_back(STATIC_TYPE_NAME(ossimMaxMosaic));   
   typeList.push_back(STATIC_TYPE_NAME(ossimNullPixelFlip));
   typeList.push_back(STATIC_TYPE_NAME(ossimColorNormalizedFusion));
   typeList.push_back(STATIC_TYPE_NAME(ossimLocalCorrelationFusion));
   typeList.push_back(STATIC_TYPE_NAME(ossimSFIMFusion));
   typeList.push_back(STATIC_TYPE_NAME(ossimHistogramRemapper));
   typeList.push_back(STATIC_TYPE_NAME(ossimImageMosaic));
   typeList.push_back(STATIC_TYPE_NAME(ossimClosestToCenterCombiner));
   typeList.push_back(STATIC_TYPE_NAME(ossimRgbToGreyFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimNBandToIndexFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimImageChain));
   typeList.push_back(STATIC_TYPE_NAME(ossimPolyCutter));
   typeList.push_back(STATIC_TYPE_NAME(ossimGeoPolyCutter));
   typeList.push_back(STATIC_TYPE_NAME(ossimJpegYCbCrToRgbSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimRgbToJpegYCbCrSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimHsiToRgbSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimRgbToHsiSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimHsvToRgbSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimRgbToHsvSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimHistogramEqualization));
   typeList.push_back(STATIC_TYPE_NAME(ossimHistogramMatchFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimHistogramThreshholdFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimGeoAnnotationSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimConvolutionSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimSICDToDetectedImage));
   typeList.push_back(STATIC_TYPE_NAME(ossimBrightnessContrastSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimBrightnessMatch));
   typeList.push_back(STATIC_TYPE_NAME(ossimScalarRemapper));
   typeList.push_back(STATIC_TYPE_NAME(ossimBandMergeSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimFeatherMosaic));
   typeList.push_back(STATIC_TYPE_NAME(ossimGridRemapSource));
   // Not sure if we want to keep this here
   typeList.push_back(STATIC_TYPE_NAME(ossimAtbController));
   typeList.push_back(STATIC_TYPE_NAME(ossimIndexToRgbLutFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimBumpShadeTileSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimSubImageTileSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimRLevelFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimBandClipFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimHsiRemapper));
   typeList.push_back(STATIC_TYPE_NAME(ossimHistogramWriter));
   typeList.push_back(STATIC_TYPE_NAME(ossimImageHistogramSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimRectangleCutFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimElevImageSource));
   
   typeList.push_back(STATIC_TYPE_NAME(ossimValueAssignImageSourceFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimImageSharpenFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossim3x3ConvolutionFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimOrthoImageMosaic));
   typeList.push_back(STATIC_TYPE_NAME(ossimMapCompositionSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimCastTileSourceFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimEquationCombiner));
   typeList.push_back(STATIC_TYPE_NAME(ossimConvolutionFilter1D));
   typeList.push_back(STATIC_TYPE_NAME(ossimImageGaussianFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimMaskFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimMultiBandHistogramTileSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimBandAverageFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimPixelFlipper));
   typeList.push_back(STATIC_TYPE_NAME(ossimScaleFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimImageToPlaneNormalFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimTopographicCorrectionFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimLandsatTopoCorrectionFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimAtCorrRemapper));
   typeList.push_back(STATIC_TYPE_NAME(ossimAtCorrGridRemapper));
   typeList.push_back(STATIC_TYPE_NAME(ossimEastingNorthingCutter));
   typeList.push_back(STATIC_TYPE_NAME(ossimEdgeFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimFftFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimWatermarkFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimImageGaussianFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimTrimFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimTwoColorView));
   typeList.push_back(STATIC_TYPE_NAME(ossimImageHistogramSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimImageSourceFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimMemoryImageSource));
}

// Hide from use...
ossimImageSourceFactory::ossimImageSourceFactory()
   :ossimImageSourceFactoryBase()
{}

ossimImageSourceFactory::ossimImageSourceFactory(const ossimImageSourceFactory&)
   :ossimImageSourceFactoryBase()
{}

const ossimImageSourceFactory& ossimImageSourceFactory::operator=(ossimImageSourceFactory&)
{
   return *this;
}
