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
  REGISTER_TEST(otbLikelihoodPathListFilter);
  REGISTER_TEST(otbParallelLinePathListFilterNew);
  REGISTER_TEST(otbRemoveWrongDirectionFilter);
  REGISTER_TEST(otbSimplifyPathListFilter);
  REGISTER_TEST(otbRoadExtractionFilterNew);
  REGISTER_TEST(otbRemoveIsolatedByDirectionFilterNew);
  REGISTER_TEST(otbAlignImageToPath);
  REGISTER_TEST(otbNonMaxRemovalByDirectionFilterNew);
  REGISTER_TEST(otbNeighborhoodScalarProductFilterNew);
  REGISTER_TEST(otbRemoveTortuousPathListFilterNew);
  REGISTER_TEST(otbNonMaxRemovalByDirectionFilter);
  REGISTER_TEST(otbRemoveWrongDirectionFilterNew);
  REGISTER_TEST(otbRoadExtractionFilter);
  REGISTER_TEST(otbLinkPathListFilter);
  REGISTER_TEST(otbModulusAndDirectionImageFiltersNew);
  REGISTER_TEST(otbSimplifyPathListFilterNew);
  REGISTER_TEST(otbRemoveTortuousPathListFilter);
  REGISTER_TEST(otbLinkPathListFilterNew);
  REGISTER_TEST(otbLikelihoodPathListFilterNew);
  REGISTER_TEST(otbParallelLinePathListFilter);
  REGISTER_TEST(otbRemoveIsolatedByDirectionFilter);
  REGISTER_TEST(otbVectorDataToRoadDescriptionFilterNew);
  REGISTER_TEST(otbVectorDataToRoadDescriptionFilter);
  REGISTER_TEST(otbSimplifyManyPathListFilter);
  REGISTER_TEST(otbNeighborhoodScalarProductFilter);
  REGISTER_TEST(otbDrawPathAlign);
  REGISTER_TEST(otbBreakAngularPathListFilter);
  REGISTER_TEST(otbBreakAngularPathListFilterNew);
}
