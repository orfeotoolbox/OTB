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
  REGISTER_TEST(otbImageToSURFKeyPointSetFilterOutputDescriptorAscii);
  REGISTER_TEST(otbKeyPointSetsMatchingFilterNew);
  REGISTER_TEST(otbImageToSIFTKeyPointSetFilterOutputInterestPointAscii);
  REGISTER_TEST(otbHistogramOfOrientedGradientCovariantImageFunctionNew);
  REGISTER_TEST(otbHistogramOfOrientedGradientCovariantImageFunction);
  REGISTER_TEST(otbImageToSURFKeyPointSetFilterOutputInterestPointAscii);
  REGISTER_TEST(otbImageToSURFKeyPointSetFilterNew);
  REGISTER_TEST(otbLandmarkNew);
  REGISTER_TEST(otbImageToSIFTKeyPointSetFilterNew);
  REGISTER_TEST(otbKeyPointSetsMatchingFilter);
  REGISTER_TEST(otbImageToSIFTKeyPointSetFilterOutputDescriptorAscii);
  REGISTER_TEST(otbImageToSIFTKeyPointSetFilterOutputAscii);
  REGISTER_TEST(otbFourierMellinImageFilter);
  REGISTER_TEST(otbImageToHessianDeterminantImageFilter);
  REGISTER_TEST(otbImageToSIFTKeyPointSetFilterOutputImage);
  REGISTER_TEST(otbFourierMellinDescriptorsNew);
  REGISTER_TEST(otbFourierMellinDescriptors);
  REGISTER_TEST(otbFourierMellinDescriptorsScaleInvariant);
  REGISTER_TEST(otbFourierMellinDescriptorsRotationInvariant);
  REGISTER_TEST(otbImageToSIFTKeyPointSetFilterDistanceMap);
  REGISTER_TEST(otbImageToHessianDeterminantImageFilterNew);
  REGISTER_TEST(otbFourierMellinImageFilterNew);
#ifdef OTB_USE_SIFTFAST
  REGISTER_TEST(otbImageToFastSIFTKeyPointSetFilterNew);
  REGISTER_TEST(otbImageToFastSIFTKeyPointSetFilterOutputInterestPointAscii);
  REGISTER_TEST(otbImageToFastSIFTKeyPointSetFilterOutputDescriptorAscii);
#endif
}
