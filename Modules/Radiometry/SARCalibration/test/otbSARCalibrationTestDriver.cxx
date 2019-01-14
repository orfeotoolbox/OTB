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
  REGISTER_TEST(otbSarBrightnessFunction);
  REGISTER_TEST(otbSarBrightnessToImageFilterTestWithoutNoise);
  REGISTER_TEST(otbTerraSarBrightnessImageFilterTest);
  REGISTER_TEST(otbSarRadiometricCalibrationToImageFilterWithRealPixelTest);
  REGISTER_TEST(otbSarParametricMapFunctionToImageFilter);
  REGISTER_TEST(otbSarRadiometricCalibrationToImageFilterWithExtractROIBeforeTest);
  REGISTER_TEST(otbSarRadiometricCalibrationToImageFilterWithComplexPixelTestWithoutNoise);
  REGISTER_TEST(otbSarBrightnessFunctorWithoutNoise);
  REGISTER_TEST(otbTerraSarBrightnessFunctor);
  REGISTER_TEST(otbSarBrightnessToImageWithComplexPixelFilterTest);
  REGISTER_TEST(otbSarParametricMapFunctionTest);
  REGISTER_TEST(otbSarRadiometricCalibrationToImageFilterCompareTest);
  REGISTER_TEST(otbSarBrightnessFunctor);
  REGISTER_TEST(otbSarBrightnessFunctionWithoutNoise);
  REGISTER_TEST(otbSarRadiometricCalibrationFunction);
  REGISTER_TEST(otbSarRadiometricCalibrationFunctionWithoutNoise);
  REGISTER_TEST(otbTerraSarBrightnessImageComplexFilterTest);
  REGISTER_TEST(otbSarRadiometricCalibrationToImageFilterWithComplexPixelTest);
  REGISTER_TEST(otbSarBrightnessToImageFilterTest);
  REGISTER_TEST(otbSarDeburstFilterTest);
  REGISTER_TEST(otbSarBurstExtractionFilterTest);
}
