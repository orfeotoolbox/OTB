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
  REGISTER_TEST(otbWaveletOperator);
  REGISTER_TEST(otbWaveletPacketTransform);
  REGISTER_TEST(otbWaveletTransform);
  REGISTER_TEST(otbSubsampleImageFilter);
  REGISTER_TEST(otbSubsampleImageFilterNew);
  REGISTER_TEST(otbWaveletInverseFilterBankNew);
  REGISTER_TEST(otbWaveletInverseTransformNew);
  REGISTER_TEST(otbWaveletTransformNew);
  REGISTER_TEST(otbWaveletPacketInverseTransformNew);
  REGISTER_TEST(otbWaveletFilterBank);
  REGISTER_TEST(otbWaveletPacketTransformNew);
  REGISTER_TEST(otbWaveletFilterBankNew);
  REGISTER_TEST(otbWaveletOperatorNew);
  REGISTER_TEST(otbWaveletImageToImageFilter);
}
