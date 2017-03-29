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
  REGISTER_TEST(otbRCC8GraphNew);
  REGISTER_TEST(otbRCC8GraphFileReader);
  REGISTER_TEST(otbRCC8GraphIOEndToEnd);
  REGISTER_TEST(otbPolygonListToRCC8GraphFilter);
  REGISTER_TEST(otbRCC8VertexBase);
  REGISTER_TEST(otbRCC8GraphFileReaderNew);
  REGISTER_TEST(otbRCC8VertexWithCompacityNew);
  REGISTER_TEST(otbImageMultiSegmentationToRCC8GraphFilterNew);
  REGISTER_TEST(otbRCC8GraphFileWriterNew);
  REGISTER_TEST(otbPolygonToPolygonRCC8Calculator);
  REGISTER_TEST(otbImageListToRCC8GraphFilterNew);
  REGISTER_TEST(otbImageToImageRCC8Calculator);
  REGISTER_TEST(otbRCC8Edge);
  REGISTER_TEST(otbPolygonListToRCC8GraphFilterNew);
  REGISTER_TEST(otbRCC8VertexWithCompacity);
  REGISTER_TEST(otbRCC8EdgeNew);
  REGISTER_TEST(otbPolygonToPolygonRCC8CalculatorNew);
  REGISTER_TEST(otbRCC8VertexBaseNew);
  REGISTER_TEST(otbRCC8Graph);
  REGISTER_TEST(otbRCC8GraphSourceNew);
  REGISTER_TEST(otbRCC8GraphFileWriter);
  REGISTER_TEST(otbImageMultiSegmentationToRCC8GraphFilter);
  REGISTER_TEST(otbImageToImageRCC8CalculatorNew);
}
