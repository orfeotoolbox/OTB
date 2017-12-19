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
  REGISTER_TEST(otbVariableLengthVectorConverterNew);
  REGISTER_TEST(otbImageRegionTileMapSplitter);
  REGISTER_TEST(otbImageRegionAdaptativeSplitterNew);
  REGISTER_TEST(otbImageRegionAdaptativeSplitter);
  REGISTER_TEST(otbRGBAPixelConverterNew);
  REGISTER_TEST(otbRGBAPixelConverter);
  REGISTER_TEST(otbRectangle);
  REGISTER_TEST(otbImageRegionNonUniformMultidimensionalSplitterNew);
  REGISTER_TEST(otbSystemTest);
  REGISTER_TEST(otbStopwatchTest);
  REGISTER_TEST(otbParseHdfSubsetName);
  REGISTER_TEST(otbParseHdfFileName);
  REGISTER_TEST(otbImageRegionSquareTileSplitterNew);
  REGISTER_TEST(otbImageRegionSquareTileSplitter);
  REGISTER_TEST(otbImageRegionTileMapSplitterNew);
  REGISTER_TEST(otbImageRegionNonUniformMultidimensionalSplitter);
  REGISTER_TEST(otbRectangleNew);
  REGISTER_TEST(otbConfigurationManagerTest);
  REGISTER_TEST(otbStandardFilterWatcherNew);
  REGISTER_TEST(otbStandardOneLineFilterWatcherTest);
  REGISTER_TEST(otbStandardWriterWatcher);
}
