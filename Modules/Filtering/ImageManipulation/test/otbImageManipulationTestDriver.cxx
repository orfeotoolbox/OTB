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
  REGISTER_TEST(otbPrintableImageFilter);
  REGISTER_TEST(otbConcatenateScalarValueImageFilterTest);
  REGISTER_TEST(otbShiftScaleImageAdaptor);
  REGISTER_TEST(otbImageToVectorImageCastFilter);
  REGISTER_TEST(otbFunctionWithNeighborhoodToImageFilter);
  REGISTER_TEST(otbSqrtSpectralAngleImageFilter);
  REGISTER_TEST(otbStreamingShrinkImageFilter);
  REGISTER_TEST(otbUnaryImageFunctorWithVectorImageFilter);
  REGISTER_TEST(otbPrintableImageFilterWithMask);
  REGISTER_TEST(otbStreamingResampleImageFilter);
  REGISTER_TEST(otbVectorImageToAmplitudeImageFilter);
  REGISTER_TEST(otbUnaryFunctorNeighborhoodWithOffsetImageFilter);
  REGISTER_TEST(otbStreamingResampleImageFilterCompareWithITK);
  REGISTER_TEST(otbRegionProjectionResampler);
  REGISTER_TEST(otbUnaryFunctorWithIndexImageFilter);
  REGISTER_TEST(otbMeanFunctorImageTest);
  REGISTER_TEST(otbVectorImageTo3DScalarImageFilter);
  REGISTER_TEST(otbTileImageFilter);
  REGISTER_TEST(otbMatrixImageFilterTest);
  REGISTER_TEST(otbMatrixTransposeMatrixImageFilter);
  REGISTER_TEST(otbUnaryFunctorNeighborhoodImageFilter);
  REGISTER_TEST(otbStreamingInnerProductVectorImageFilter);
  REGISTER_TEST(otbPhaseFunctorTest);
  REGISTER_TEST(otbChangeLabelImageFilter);
  REGISTER_TEST(otbBoxAndWhiskerImageFilter);
  REGISTER_TEST(otbVectorRescaleIntensityImageFilter);
  REGISTER_TEST(otbLog10ThresholdedImageFilterTest);
  REGISTER_TEST(otbExtractROIResample);
  REGISTER_TEST(otbLocalGradientVectorImageFilterTest);
  REGISTER_TEST(otbPerBandVectorImageFilterWithSobelFilter);
  REGISTER_TEST(otbBinaryImageDensityFunction);
  REGISTER_TEST(otbThresholdVectorImageFilterTest);
  REGISTER_TEST(otbPerBandVectorImageFilterWithMeanFilter);
  REGISTER_TEST(otbAmplitudeFunctorTest);
  REGISTER_TEST(otbMultiplyByScalarImageFilterTest);
  REGISTER_TEST(otbClampImageFilterTest);
  REGISTER_TEST(otbClampImageFilterConversionTest);
  REGISTER_TEST(otbConcatenateVectorImageFilter);
  REGISTER_TEST(otbBinaryImageToDensityImageFilter);
  REGISTER_TEST(otbSpectralAngleDistanceImageFilter);
  REGISTER_TEST(otbEuclideanDistanceMetricWithMissingValue);
  REGISTER_TEST(otbNRIBandImagesToOneNComplexBandsImage);
  REGISTER_TEST(otbOneRIBandImageToOneComplexBandImage);
  REGISTER_TEST(otbTwoNRIBandsImageToNComplexBandsImage);
  REGISTER_TEST(otbChangeNoDataValueFilter);
  REGISTER_TEST(otbImageToNoDataMaskFilter);
  REGISTER_TEST(otbChangeInformationImageFilter);
  REGISTER_TEST(otbGridResampleImageFilter);
  REGISTER_TEST(otbMaskedIteratorDecoratorNominal);
  REGISTER_TEST(otbMaskedIteratorDecoratorDegenerate);
  REGISTER_TEST(otbMaskedIteratorDecoratorExtended);
}
