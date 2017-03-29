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
  REGISTER_TEST(otbNearestTransformDisplacementFieldGeneratorNew);
  REGISTER_TEST(otbNNearestTransformsLinearInterpolateDisplacementFieldGenerator);
  REGISTER_TEST(otbNNearestPointsLinearInterpolateDisplacementFieldGenerator);
  REGISTER_TEST(otbNearestPointDisplacementFieldGeneratorNew);
  REGISTER_TEST(otbPointSetToDisplacementFieldGeneratorNew);
  REGISTER_TEST(otbPointSetWithTransformToDisplacementFieldGeneratorNew);
  REGISTER_TEST(otbBSplinesInterpolateDisplacementFieldGeneratorNew);
  REGISTER_TEST(otbNNearestTransformsLinearInterpolateDisplacementFieldGeneratorNew);
  REGISTER_TEST(otbNNearestPointsLinearInterpolateDisplacementFieldGeneratorNew);
  REGISTER_TEST(otbNearestTransformDisplacementFieldGenerator);
  REGISTER_TEST(otbNearestPointDisplacementFieldGenerator);
  REGISTER_TEST(otbBSplinesInterpolateTransformDisplacementFieldGenerator);
  REGISTER_TEST(otbBSplinesInterpolateDisplacementFieldGenerator);
  REGISTER_TEST(otbBSplinesInterpolateTransformDisplacementFieldGeneratorNew);
}
