/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#include "otbOGRDataSourceWrapper.h"

#include <cstdlib> // EXIT_FAILURE
#include <iostream>

using namespace otb;

/*===========================================================================*/
/*==============================[ other stuff ]==============================*/
/*===========================================================================*/
// int otbOGRDataSourceNew (int argc, char **argv)
int main(int argc, char **argv)
{
  if (argc < 2)
    {
    std::cerr << "otbOGRDataSourceNew <shape-file>\n";
    return EXIT_FAILURE;
    }
  ogr::DataSource::Pointer ds = ogr::DataSource::New(argv[1]);

  std::cout << "file:\t" << argv[1] << std::endl;
  std::cout << "nb layers:\t" << ds->GetLayersCount() << std::endl;
  std::cout << "nb features(0):\t" << ds->Size(false) << std::endl;
  std::cout << "nb features(1):\t" << ds->Size(true) << std::endl;
  ds->Print(std::cout);
  return EXIT_SUCCESS;
}
