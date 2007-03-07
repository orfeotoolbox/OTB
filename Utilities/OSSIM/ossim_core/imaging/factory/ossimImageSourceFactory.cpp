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
// $Id: ossimImageSourceFactory.cpp,v 1.96 2005/12/22 15:32:59 gpotts Exp $

#include <imaging/factory/ossimImageSourceFactory.h>
#include <imaging/tile_sources/ossimConvolutionFilter1D.h>
#include <imaging/tile_sources/ossimImageGaussianFilter.h>
#include <imaging/tile_sources/ossimImageRenderer.h>
#include <imaging/tile_sources/ossimCacheTileSource.h>
#include <imaging/tile_sources/ossimFeatherMosaic.h>
#include <imaging/tile_sources/ossimHistogramRemapper.h>
#include <imaging/tile_sources/ossimImageMosaic.h>
#include <imaging/tile_sources/ossimClosestToCenterCombiner.h>
#include <imaging/tile_sources/ossimBlendMosaic.h>
#include <imaging/tile_sources/ossimMaxMosaic.h>
#include <imaging/tile_sources/ossimOrthoImageMosaic.h>
#include <imaging/tile_sources/ossimColorNormalizedFusion.h>
#include <imaging/tile_sources/ossimLocalCorrelationFusion.h>
#include <imaging/tile_sources/ossimSFIMFusion.h>
#include <imaging/tile_sources/ossimTopographicCorrectionFilter.h>
#include <imaging/tile_sources/ossimBandSelector.h>
#include <imaging/tile_sources/ossimNBandToIndexFilter.h>
#include <imaging/tile_sources/ossimRgbToGreyFilter.h>
#include <imaging/tile_sources/ossimRgbToJpegYCbCrSource.h>
#include <imaging/tile_sources/ossimRgbToHsvSource.h>
#include <imaging/tile_sources/ossimRgbToHsiSource.h>
#include <imaging/tile_sources/ossimHsiToRgbSource.h>
#include <imaging/tile_sources/ossimHsvToRgbSource.h>
#include <imaging/tile_sources/ossimHsiRemapper.h>
#include <imaging/tile_sources/ossimJpegYCbCrToRgbSource.h>
#include <imaging/tile_sources/ossimPolyCutter.h>
#include <imaging/tile_sources/ossimGeoPolyCutter.h>
#include <imaging/tile_sources/ossimHistogramEqualization.h>
#include <imaging/tile_sources/ossimHistogramMatchFilter.h>
#include <imaging/tile_sources/ossimHistogramThreshholdFilter.h>
#include <imaging/tile_sources/ossimConvolutionSource.h>
#include <imaging/tile_sources/ossimBrightnessContrastSource.h>
#include <imaging/tile_sources/ossimBrightnessMatch.h>
#include <imaging/tile_sources/ossimScalarRemapper.h>
#include <imaging/tile_sources/ossimBandMergeSource.h>
#include <imaging/tile_sources/ossimIndexToRgbLutFilter.h>
#include <imaging/tile_sources/ossimBumpShadeTileSource.h>
#include <imaging/tile_sources/ossimSubImageTileSource.h>
#include <imaging/tile_sources/ossimRLevelFilter.h>
#include <imaging/tile_sources/ossimBandClipFilter.h>
#include <imaging/tile_sources/ossimValueAssignImageSourceFilter.h>
#include <imaging/tile_sources/ossimImageSharpenFilter.h>
#include <imaging/tile_sources/ossim3x3ConvolutionFilter.h>
#include <imaging/tile_sources/ossimCastTileSourceFilter.h>
#include <imaging/tile_sources/ossimEquationCombiner.h>
#include <imaging/tile_sources/ossimMaskFilter.h>
#include <imaging/misc/ossimPixelFlipper.h>
#include <imaging/elevation/ossimElevImageSource.h>
#include <imaging/tile_sources/ossimLandsatTopoCorrectionFilter.h>
#include <imaging/annotation/ossimGeoAnnotationSource.h>
#include <imaging/annotation/ossimEsriShapeFileFilter.h>
#include <imaging/tile_sources/ossimEsriShapeFileCutter.h>
#include <imaging/annotation/ossimMapCompositionSource.h>
#include <imaging/tile_sources/ossimScaleFilter.h>
#include <imaging/tile_sources/ossimEastingNorthingCutter.h>
#include <imaging/tile_sources/ossimEdgeFilter.h>
#include <imaging/tile_sources/ossimFftFilter.h>
#include <imaging/tile_sources/ossimWatermarkFilter.h>
#include <imaging/tile_sources/ossimTrimFilter.h>


#include <imaging/tonal_balance/ossimGridRemapSource.h>

// Not sure if we want to keep this here
#include <imaging/tonal_balance/ossimAtbController.h>

#include <imaging/ossimImageChain.h>
#include <imaging/histogram/ossimHistogramWriter.h>
#include <imaging/tile_sources/ossimRectangleCutFilter.h>


#include <base/factory/ossimObjectFactoryRegistry.h>
#include <imaging/histogram/ossimImageHistogramSource.h>

#include <imaging/tile_sources/ossimMultiBandHistogramTileSource.h>

#include <imaging/tile_sources/ossimBandAverageFilter.h>
#include <imaging/tile_sources/ossimImageToPlaneNormalFilter.h>

#include <imaging/atmospheric_correction/ossimAtCorrGridRemapper.h>
#include <imaging/atmospheric_correction/ossimAtCorrRemapper.h>

#include <base/common/ossimTrace.h>

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
   else if(name == STATIC_TYPE_NAME(ossimEsriShapeFileFilter))
   {
      return new ossimEsriShapeFileFilter;
   }
   else if(name == STATIC_TYPE_NAME(ossimEsriShapeFileCutter))
   {
      return new ossimEsriShapeFileCutter;
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
   else if(name == STATIC_TYPE_NAME(ossimConvolutionFilter1D))
   {
      return new ossimConvolutionFilter1D(); 
   }
   else if(name == STATIC_TYPE_NAME(ossimImageGaussianFilter))
   {
      return new ossimImageGaussianFilter();
   }
   else if(name == STATIC_TYPE_NAME(ossimTrimFilter))
   {
      return new ossimTrimFilter();
   }
   return NULL;
}

ossimObject* ossimImageSourceFactory::createObject(const ossimKeywordlist& kwl,
                                                   const char* prefix)const
{
   static const char* MODULE = "ossimImageSourceFactory::createSource";
   
   ossimString copyPrefix = prefix;
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
   typeList.push_back(STATIC_TYPE_NAME(ossimEsriShapeFileFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimEsriShapeFileCutter));
   typeList.push_back(STATIC_TYPE_NAME(ossimOrthoImageMosaic));
   typeList.push_back(STATIC_TYPE_NAME(ossimMapCompositionSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimCastTileSourceFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimEquationCombiner));
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
   typeList.push_back(STATIC_TYPE_NAME(ossimConvolutionFilter1D));
   typeList.push_back(STATIC_TYPE_NAME(ossimImageGaussianFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimTrimFilter));
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
