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
  REGISTER_TEST(otbWindowedSincInterpolateImageCosineFunctionNew);
  REGISTER_TEST(otbWindowedSincInterpolateImageLanczosFunction);
  REGISTER_TEST(otbWindowedSincInterpolateImageLanczosFunctionNew);
  REGISTER_TEST(otbWindowedSincInterpolateImageFunctionBaseNew);
  REGISTER_TEST(otbProlateInterpolateImageFunctionNew);
  REGISTER_TEST(otbBSplineDecompositionImageFilterNew);
  REGISTER_TEST(otbBSplineInterpolateImageFunctionNew);
  REGISTER_TEST(otbWindowedSincInterpolateImageBlackmanFunction);
  REGISTER_TEST(otbBSplineDecompositionImageFilter);
  REGISTER_TEST(otbWindowedSincInterpolateImageWelchFunctionNew);
  REGISTER_TEST(otbWindowedSincInterpolateImageGaussianFunction);
  REGISTER_TEST(otbGenericInterpolateImageFunctionNew);
  REGISTER_TEST(otbWindowedSincInterpolateImageBlackmanFunctionNew);
  REGISTER_TEST(otbWindowedSincInterpolateImageCosineFunction);
  REGISTER_TEST(otbWindowedSincInterpolateImageHammingFunction);
  REGISTER_TEST(otbWindowedSincInterpolateImageWelchFunction);
  REGISTER_TEST(otbBSplineInterpolateImageFunction);
  REGISTER_TEST(otbWindowedSincInterpolateImageGaussianFunctionNew);
  REGISTER_TEST(otbWindowedSincInterpolateImageHammingFunctionNew);
  REGISTER_TEST(otbStreamingTraitsImage);
  REGISTER_TEST(otbStreamingTraitsVectorImage);
  REGISTER_TEST(otbBCOInterpolateImageFunction);
  REGISTER_TEST(otbBCOInterpolateImageFunctionNew);
  REGISTER_TEST(otbBCOInterpolateImageFunction2);
  REGISTER_TEST(otbBCOInterpolateImageFunctionOverVectorImageNew);
  REGISTER_TEST(otbBCOInterpolateImageFunctionOverVectorImage);
  REGISTER_TEST(otbBCOInterpolateImageFunctionTest);
  REGISTER_TEST(otbBCOInterpolateImageFunctionVectorImageTest);
  REGISTER_TEST(otbProlateInterpolateImageFunction);
  REGISTER_TEST(otbProlateValidationTest);
}
