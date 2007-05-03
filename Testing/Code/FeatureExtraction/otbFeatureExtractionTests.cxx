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

#include <iostream>
#include "otbTestMain.h" 

void RegisterTests()
{
REGISTER_TEST(otbAlignImageToPath);
REGISTER_TEST(otbDrawPathDessinCarre);
REGISTER_TEST(otbDrawPathAlign);
REGISTER_TEST(otbComplexMomentImage);
REGISTER_TEST(otbHuImage);
REGISTER_TEST(otbFlusserImage);
REGISTER_TEST(otbComplexMomentPathNew);
REGISTER_TEST(otbComplexMomentPath);
REGISTER_TEST(otbComplexMomentPathFloat);
REGISTER_TEST(otbHuPathNew);
REGISTER_TEST(otbHuPath);
REGISTER_TEST(otbFlusserPathNew);
REGISTER_TEST(otbFlusserPath);
REGISTER_TEST(otbOrientationPathNew);
REGISTER_TEST(otbOrientationPath);
REGISTER_TEST(otbCompacityPathNew);
REGISTER_TEST(otbCompacityPathCircle);
REGISTER_TEST(otbCompacityPathSquare);
REGISTER_TEST(otbCompacityPathRectangle);
REGISTER_TEST(otbTouziEdgeDetectorNew);
REGISTER_TEST(otbTouziEdgeDetector);
REGISTER_TEST(otbTouziEdgeDetectorDirection);
REGISTER_TEST(otbLineDetectorBaseNew);
REGISTER_TEST(otbLineRatioDetectorNew);
REGISTER_TEST(otbLineRatioDetectorLinear);
REGISTER_TEST(otbLineRatioDetector);
REGISTER_TEST(otbLineCorrelationDetectorNew);
REGISTER_TEST(otbLineCorrelationDetectorLinear);
REGISTER_TEST(otbLineCorrelationDetector);
REGISTER_TEST(otbAssociativeSymmetricalSumNew);
REGISTER_TEST(otbAssociativeSymmetricalSum);
REGISTER_TEST(otbAssymmetricFusionOfLineDetectorNew);
REGISTER_TEST(otbAssymmetricFusionOfLineDetector);
REGISTER_TEST(otbHarrisImage);
REGISTER_TEST(otbMultiplyByScalarImageFilterTest);
REGISTER_TEST(otbThresholdImageToPointSetTest);
REGISTER_TEST(otbHarrisToPointSet);
REGISTER_TEST(otbPixelSuppressionByDirectionNew);
REGISTER_TEST(otbPixelSuppressionByDirection);
REGISTER_TEST(otbFillGapsFilterNew);
REGISTER_TEST(otbLocalHoughNew);
REGISTER_TEST(otbLocalHough);
REGISTER_TEST(otbLocalHoughDraw);
REGISTER_TEST(otbExtractSegmentsNew);
REGISTER_TEST(otbExtractSegments);
REGISTER_TEST(otbFourierMellinImageFilterNew);
REGISTER_TEST(otbFourierMellinImageFilterTestFFT);
REGISTER_TEST(otbFourierMellinImageFilter);
REGISTER_TEST(otbImageToEdgePathFilterNew);
REGISTER_TEST(otbImageToEdgePathFilter);
REGISTER_TEST(otbModulusAndDirectionImageFiltersNew);
REGISTER_TEST(otbNeighborhoodScalarProductFilterNew);
REGISTER_TEST(otbNeighborhoodScalarProductFilter);
REGISTER_TEST(otbRemoveIsolatedByDirectionFilterNew);
REGISTER_TEST(otbRemoveIsolatedByDirectionFilter);
REGISTER_TEST(otbRemoveWrongDirectionFilterNew);
REGISTER_TEST(otbRemoveWrongDirectionFilter);
REGISTER_TEST(otbNonMaxRemovalByDirectionFilterNew);
REGISTER_TEST(otbNonMaxRemovalByDirectionFilter);
REGISTER_TEST(otbVectorizationPathListFilterNew);
REGISTER_TEST(otbVectorizationPathListFilter);
REGISTER_TEST(otbSimplifyPathFilterNew);
REGISTER_TEST(otbSimplifyPathFilter);
REGISTER_TEST(otbBreakAngularPathListFilterNew);
REGISTER_TEST(otbBreakAngularPathListFilter);
REGISTER_TEST(otbRemoveTortuousPathFilterNew);
REGISTER_TEST(otbRemoveTortuousPathFilter);
REGISTER_TEST(otbLinkPathListFilterNew);
}
