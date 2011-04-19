/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// this file defines the otbCommonTest for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbUniformAlphaBlendingFunctionNew);
  REGISTER_TEST(otbBlendingImageFilterNew);
  REGISTER_TEST(otbBlendingImageFilter);
  REGISTER_TEST(otbMultiplyBlendingFunctionNew);
  REGISTER_TEST(otbImageViewNew);
  REGISTER_TEST(otbImageViewerEndToEndSingleLayer);
  REGISTER_TEST(otbImageViewerEndToEndSingleLayerWithSelectAreaHandler);
  REGISTER_TEST(otbImageViewerEndToEndTwoLayers);
  REGISTER_TEST(otbImageViewerEndToEndGlComponent);
  REGISTER_TEST(otbPixelDescriptionModelNew);
  REGISTER_TEST(otbPixelDescriptionModelSingleLayer);
  REGISTER_TEST(otbHistogramCurveNew);
  REGISTER_TEST(otbCurves2DWidgetNew);
  REGISTER_TEST(otbCurves2DWidget);
  REGISTER_TEST(otbCurves2DWidgetWithHistogram);
  REGISTER_TEST(otbVectorDataGlComponentNew);
  REGISTER_TEST(otbImageWidgetWithVectorDataGlComponent);
  REGISTER_TEST(otbCircleGlComponentNew);
  REGISTER_TEST(otbCrossGlComponentNew);
  REGISTER_TEST(otbImageViewerEndToEndWithVectorData);
  REGISTER_TEST(otbStandardImageViewerNew);
  REGISTER_TEST(otbStandardImageViewer);
  REGISTER_TEST(otbStandardImageViewerRGBNew);
  REGISTER_TEST(otbStandardImageViewerSplitted);
  REGISTER_TEST(otbPackedWidgetManagerNew);
  REGISTER_TEST(otbSplittedWidgetManagerNew);
  REGISTER_TEST(otbVerticalAsymptoteCurveNew);
}
