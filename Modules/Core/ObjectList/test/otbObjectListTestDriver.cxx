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
  REGISTER_TEST(otbObjectList);
  REGISTER_TEST(otbObjectListNew);
  REGISTER_TEST(otbImageList);
  REGISTER_TEST(otbImageListNew);
  REGISTER_TEST(otbImageListSourceNew);
  REGISTER_TEST(otbImageListToImageFilterNew);
  REGISTER_TEST(otbImageListToImageListApplyFilter);
  REGISTER_TEST(otbImageListToImageListApplyFilterNew);
  REGISTER_TEST(otbImageListToImageListFilterNew);
  REGISTER_TEST(otbImageListToVectorImageFilter2);
  REGISTER_TEST(otbImageListToVectorImageFilter);
  REGISTER_TEST(otbImageListToVectorImageFilterNew);
  REGISTER_TEST(otbObjectList2);
  REGISTER_TEST(otbObjectListToObjectListFilterNew);
  REGISTER_TEST(otbImageToImageListFilterNew);
  REGISTER_TEST(otbVectorImageToImageListFilter);
  REGISTER_TEST(otbVectorImageToImageListFilterNew);
  REGISTER_TEST(otbVectorImageToImageListFilterIterator);
}
