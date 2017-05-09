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
  REGISTER_TEST(otbCompacityPathRectangle);
  REGISTER_TEST(otbVectorizationPathListFilterNew);
  REGISTER_TEST(otbRegionImageToRectangularPathListFilterNew);
  REGISTER_TEST(otbImageToEdgePathFilter);
  REGISTER_TEST(otbCompacityPathCircle);
  REGISTER_TEST(otbDrawPathListFilterWithValue);
  REGISTER_TEST(otbImageFittingPolygonListFilterNew);
  REGISTER_TEST(otbDrawPathDessinCarre);
  REGISTER_TEST(otbCompacityPathSquare);
  REGISTER_TEST(otbPathLengthFunctor);
  REGISTER_TEST(otbPathListToHistogramGenerator);
  REGISTER_TEST(otbImageFittingPolygonListFilter);
  REGISTER_TEST(otbImageToPathFilterNew);
  REGISTER_TEST(otbImageToEdgePathFilterNew);
  REGISTER_TEST(otbDrawPathListFilter);
  REGISTER_TEST(otbDrawPathListFilterNew);
  REGISTER_TEST(otbDrawPathFilter);
  REGISTER_TEST(otbOrientationPath);
  REGISTER_TEST(otbOrientationPathNew);
  REGISTER_TEST(otbDrawPathFilterNew);
  REGISTER_TEST(otbPolyLineImageConstIterator);
  REGISTER_TEST(otbRegionImageToRectangularPathListFilter);
  REGISTER_TEST(otbVectorizationPathListFilter);
  REGISTER_TEST(otbPathListToPathListFilterNew);
  REGISTER_TEST(otbPathListToHistogramGeneratorNew);
  REGISTER_TEST(otbCompacityPathNew);
  REGISTER_TEST(otbClosePathFunctor);
  REGISTER_TEST(otbPolyLineImageIterator);
}
