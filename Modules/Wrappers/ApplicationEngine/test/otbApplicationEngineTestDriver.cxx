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
  REGISTER_TEST(otbWrapperInputImageParameterTest);
  REGISTER_TEST(otbWrapperFloatParameterTest);
  REGISTER_TEST(otbWrapperDoubleParameterTest);
  REGISTER_TEST(otbWrapperIntParameterTest);
  REGISTER_TEST(otbWrapperRAMParameterTest);
  REGISTER_TEST(otbWrapperStringParameterTest1);
  REGISTER_TEST(otbWrapperChoiceParameterTest1);
  REGISTER_TEST(otbWrapperApplicationRegistry);
  REGISTER_TEST(otbWrapperStringListParameterTest1);
  REGISTER_TEST(otbWrapperDocExampleStructureTest);
  REGISTER_TEST(otbWrapperParameterKey);
  REGISTER_TEST(otbWrapperParameterList);
  REGISTER_TEST(otbWrapperInputImageListParameterTest1);
  REGISTER_TEST(otbWrapperInputVectorDataListParameterTest1);
  REGISTER_TEST(otbWrapperApplicationDocTest);
  REGISTER_TEST(otbWrapperOutputImageParameterTest1);
  //~ REGISTER_TEST(otbWrapperOutputImageParameterConversionTest);
  REGISTER_TEST(otbApplicationMemoryConnectTest);
  REGISTER_TEST(otbWrapperImageInterface);
}
