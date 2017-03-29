/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbPixelSuppressionByDirectionNew);
  REGISTER_TEST(otbEdgeDetectorImageFilter);
  REGISTER_TEST(otbHorizontalSobelVectorImageFilterNewTest);
  REGISTER_TEST(otbHorizontalSobelVectorImageFilterTest);
  REGISTER_TEST(otbAsymmetricFusionOfLineDetector);
  REGISTER_TEST(otbLocalHoughDraw);
  REGISTER_TEST(otbAssociativeSymmetricalSum);
  REGISTER_TEST(otbPersistentVectorizationFilterNew);
  REGISTER_TEST(otbLineCorrelationDetectorNew);
  REGISTER_TEST(otbPersistentVectorizationFilter);
  REGISTER_TEST(otbEdgeDensityImageFilter);
  REGISTER_TEST(otbLineCorrelationDetector);
  REGISTER_TEST(otbSobelVectorImageFilterNewTest);
  REGISTER_TEST(otbSobelVectorImageFilterTest);
  REGISTER_TEST(otbAssociativeSymmetricalSumNew);
  REGISTER_TEST(otbHoughTransform2DLinesImageTest);
  REGISTER_TEST(otbPixelSuppressionByDirection);
  REGISTER_TEST(otbLocalHoughNew);
  REGISTER_TEST(otbEdgeDetectorImageFilterNew);
  REGISTER_TEST(otbEdgeDensityImageFilterNew);
  REGISTER_TEST(otbLocalHough);
  REGISTER_TEST(otbExtractSegments);
  REGISTER_TEST(otbExtractSegmentsNew);
  REGISTER_TEST(otbAsymmetricFusionOfLineDetectorNew);
  REGISTER_TEST(otbLineRatioDetector);
  REGISTER_TEST(otbTouziEdgeDetectorDirection);
  REGISTER_TEST(otbTouziEdgeDetectorNew);
  REGISTER_TEST(otbVerticalSobelVectorImageFilterNewTest);
  REGISTER_TEST(otbVerticalSobelVectorImageFilterTest);
  REGISTER_TEST(otbStreamingLineSegmentDetectorNew);
  REGISTER_TEST(otbStreamingLineSegmentDetector);
  REGISTER_TEST(otbLineRatioDetectorNew);
  REGISTER_TEST(otbTouziEdgeDetector);
  REGISTER_TEST(otbLineRatioDetectorLinear);
  REGISTER_TEST(otbLineSegmentDetectorNew);
  REGISTER_TEST(otbLineSegmentDetector);
  REGISTER_TEST(otbLineSegmentDetector_8b_16b_compare);
  REGISTER_TEST(otbLineCorrelationDetectorLinear);
  REGISTER_TEST(otbLineDetectorBaseNew);
  REGISTER_TEST(otbFillGapsFilter);
  REGISTER_TEST(otbFillGapsFilterNew);
}
