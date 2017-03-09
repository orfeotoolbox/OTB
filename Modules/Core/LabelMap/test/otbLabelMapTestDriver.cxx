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
  REGISTER_TEST(otbAttributesMapLabelObjectWithClassLabelNew);
  REGISTER_TEST(otbLabelObjectMapVectorizer);
  REGISTER_TEST(otbAttributesMapOpeningLabelMapFilterNew);
  REGISTER_TEST(otbLabelImageToLabelMapWithAdjacencyFilterNew);
  REGISTER_TEST(otbLabelImageToLabelMapWithAdjacencyFilter);
  REGISTER_TEST(otbStatisticsAttributesLabelMapFilterNew);
  REGISTER_TEST(otbLabelObjectToPolygonFunctorNew);
  REGISTER_TEST(otbLabelMapWithClassLabelToLabeledSampleListFilterNew);
  REGISTER_TEST(otbAttributesMapLabelObjectNew);
  REGISTER_TEST(otbLabelMapToSampleListFilterNew);
  REGISTER_TEST(otbLabelMapSourceNew);
  REGISTER_TEST(otbLabelMapToAttributeImageFilterNew);
  REGISTER_TEST(otbImageToLabelMapWithAttributesFilter);
  REGISTER_TEST(otbImageToLabelMapWithAttributesFilterNew);
  REGISTER_TEST(otbKMeansAttributesLabelMapFilterNew);
  REGISTER_TEST(otbKMeansAttributesLabelMapFilter);
  REGISTER_TEST(otbLabelMapToSampleListFilter);
  REGISTER_TEST(otbLabelMapWithClassLabelToLabeledSampleListFilter);
  REGISTER_TEST(otbMinMaxAttributesLabelMapFilter);
  REGISTER_TEST(otbMinMaxAttributesLabelMapFilterNew);
  REGISTER_TEST(otbNormalizeAttributesLabelMapFilter);
  REGISTER_TEST(otbNormalizeAttributesLabelMapFilterNew);
  REGISTER_TEST(otbShapeAttributesLabelMapFilterNew);
  REGISTER_TEST(otbBandsStatisticsAttributesLabelMapFilter);
  REGISTER_TEST(otbBandsStatisticsAttributesLabelMapFilterNew);
}
