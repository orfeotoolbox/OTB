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
  REGISTER_TEST(otbComplexMomentPathFloat);
  REGISTER_TEST(otbRadiometricMomentsImageFunction);
  REGISTER_TEST(otbHuMomentsImageFunction);
  REGISTER_TEST(otbHuMomentsImageFunctionScaleInvariant);
  REGISTER_TEST(otbHuMomentsImageFunctionRotationInvariant);
  REGISTER_TEST(otbRealMomentsImageFunction);
  REGISTER_TEST(otbComplexMomentsImageFunction);
  REGISTER_TEST(otbComplexMomentsImageFunctionScaleInvariant);
  REGISTER_TEST(otbFlusserMomentsImageFunction);
  REGISTER_TEST(otbFlusserMomentsImageFunctionScaleInvariant);
  REGISTER_TEST(otbFlusserMomentsImageFunctionRotationInvariant);
  REGISTER_TEST(otbHuPath);
  REGISTER_TEST(otbFlusserPath);
  REGISTER_TEST(otbComplexMomentPath);
  REGISTER_TEST(otbRadiometricMomentsImageFilter);
}
