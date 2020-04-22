/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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
  REGISTER_TEST(otbFlexibleDistanceWithMissingValue);
  REGISTER_TEST(otbImagePCAShapeModelEstimatorTest);
  REGISTER_TEST(otbFunctionToImageFilter);
  REGISTER_TEST(otbExtractROI2);
  REGISTER_TEST(otbComplexToVectorImageCastFilterTest);
  REGISTER_TEST(otbMultiChannelExtractROI);
  REGISTER_TEST(otbVectorImageLegacyTest);
  REGISTER_TEST(otbVectorImageTest);
  REGISTER_TEST(otbExtractROITestMetaData);
  REGISTER_TEST(otbTestMultiExtractMultiUpdate);
  REGISTER_TEST(otbExtractROI);
  REGISTER_TEST(otbImageAndVectorImageOperationFilterTest);
  REGISTER_TEST(otbImageOfVectorsToMonoChannelExtractROI);
  REGISTER_TEST(otbExtractROI_RGB);
  REGISTER_TEST(otbComplexToIntensityFilterTest);
  REGISTER_TEST(otbMultiToMonoChannelExtractROI);
  REGISTER_TEST(otbImageTest);
  REGISTER_TEST(otbImageFunctionAdaptor);
  REGISTER_TEST(otbMetaImageFunction);
}
