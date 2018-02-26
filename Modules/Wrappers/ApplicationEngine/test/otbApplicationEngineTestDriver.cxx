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
  REGISTER_TEST(otbWrapperApplicationNew);
  REGISTER_TEST(otbWrapperInputImageParameterNew);
  REGISTER_TEST(otbWrapperInputImageParameterTest1);
  REGISTER_TEST(otbWrapperNumericalParameterNew);
  REGISTER_TEST(otbWrapperNumericalParameterTest1);
  REGISTER_TEST(otbWrapperStringParameterNew);
  REGISTER_TEST(otbWrapperStringParameterTest1);
  REGISTER_TEST(otbWrapperChoiceParameterNew);
  REGISTER_TEST(otbWrapperChoiceParameterTest1);
  REGISTER_TEST(otbWrapperApplicationRegistry);
  REGISTER_TEST(otbWrapperStringListParameterNew);
  REGISTER_TEST(otbWrapperStringListParameterTest1);
  REGISTER_TEST(otbWrapperRAMParameterNew);
  REGISTER_TEST(otbWrapperDocExampleStructureNew);
  REGISTER_TEST(otbWrapperDocExampleStructureTest);
  REGISTER_TEST(otbWrapperParameterKey);
  REGISTER_TEST(otbWrapperParameterListNew);
  REGISTER_TEST(otbWrapperParameterList);
  REGISTER_TEST(otbWrapperEmptyParameterNew);
  REGISTER_TEST(otbWrapperInputImageListParameterNew);
  REGISTER_TEST(otbWrapperInputImageListParameterTest1);
  REGISTER_TEST(otbWrapperInputVectorDataListParameterNew);
  REGISTER_TEST(otbWrapperInputVectorDataListParameterTest1);
  REGISTER_TEST(otbWrapperApplicationDocTest);
  REGISTER_TEST(otbWrapperApplicationHtmlDocGeneratorNew);
  REGISTER_TEST(otbWrapperApplicationHtmlDocGeneratorTest1);
  REGISTER_TEST(otbWrapperInputVectorDataParameterNew);
  REGISTER_TEST(otbWrapperOutputImageParameterNew);
  REGISTER_TEST(otbWrapperOutputImageParameterTest1);
  //~ REGISTER_TEST(otbWrapperOutputImageParameterConversionTest);
  REGISTER_TEST(otbApplicationMemoryConnectTest);
}
