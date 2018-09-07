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

#include <cstdlib>
#include <fstream>
#include "otbTileMapImageIOHelper.h"


int otbTileMapImageIOHelperTest(int itkNotUsed(argc), char * argv[])
{
  otb::TileMapImageIOHelper::Pointer myTileMapImageIOHelper;
  myTileMapImageIOHelper = otb::TileMapImageIOHelper::New();

  char * filename = argv[1];
  std::ofstream file;
  file.open(filename);

  for (unsigned int i=0; i < 22; ++i)
    {
    file << i << ": " << myTileMapImageIOHelper->ConvertDepthToScale(i) << std::endl;
    }

  file.close();
  return EXIT_SUCCESS;
}
