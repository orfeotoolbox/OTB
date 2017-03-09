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
  REGISTER_TEST(otbMorphologicalProfilesSegmentationFilter);
  REGISTER_TEST(otbProfileToProfileDerivativeFilterNew);
  REGISTER_TEST(otbGeodesicMorphologyDecompositionImageFilter);
  REGISTER_TEST(otbMorphologicalClosingProfileFilterNew);
  REGISTER_TEST(otbConvexOrConcaveClassificationFilter);
  REGISTER_TEST(otbGeodesicMorphologyLevelingFilter);
  REGISTER_TEST(otbConvexOrConcaveClassificationFilterNew);
  REGISTER_TEST(otbMultiScaleConvexOrConcaveClassificationFilter);
  REGISTER_TEST(otbMorphologicalOpeningProfileFilter);
  REGISTER_TEST(otbGeodesicMorphologyIterativeDecompositionImageFilter);
  REGISTER_TEST(otbGeodesicMorphologyIterativeDecompositionImageFilterNew);
  REGISTER_TEST(otbGeodesicMorphologyLevelingFilterNew);
  REGISTER_TEST(otbClosingOpeningMorphologicalFilter);
  REGISTER_TEST(otbProfileDerivativeToMultiScaleCharacteristicsFilterNew);
  REGISTER_TEST(otbProfileToProfileDerivativeFilter);
  REGISTER_TEST(otbOpeningClosingMorphologicalFilterNew);
  REGISTER_TEST(otbGeodesicMorphologyDecompositionImageFilterNew);
  REGISTER_TEST(otbMorphologicalOpeningProfileFilterNew);
  REGISTER_TEST(otbProfileDerivativeToMultiScaleCharacteristicsFilter);
  REGISTER_TEST(otbMultiScaleConvexOrConcaveClassificationFilterNew);
  REGISTER_TEST(otbClosingOpeningMorphologicalFilterNew);
  REGISTER_TEST(otbOpeningClosingMorphologicalFilter);
  REGISTER_TEST(otbMorphologicalClosingProfileFilter);
}
