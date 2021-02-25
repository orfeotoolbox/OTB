/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#include <fstream>
#include <cstdlib>

#include "otb_ossim.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wshadow"
#include "ossim/elevation/ossimElevManager.h"
#include "ossim/base/ossimFilename.h"
#pragma GCC diagnostic pop
#else
#include "ossim/elevation/ossimElevManager.h"
#include "ossim/base/ossimFilename.h"
#endif

#include "itkPoint.h"

int otbOssimElevManagerTest2(int argc, char* argv[])
{

  if (argc != 7)
  {
    std::cout << "Usage: " << std::endl;
    std::cout << argv[0] << " srtmDir outfname originX originY spacingX sizeX " << std::endl;
    return EXIT_FAILURE;
  }

  const ossimFilename srtmDir(argv[1]);
  const char*         outfname = argv[2];

  typedef itk::Point<double, 2> PointType;
  PointType origin;
  double    spacing;
  int       size;

  origin[0] = atof(argv[3]);
  origin[1] = atof(argv[4]);
  spacing   = atof(argv[5]);
  size      = atoi(argv[6]);

  ossimElevManager* elevManager = ossimElevManager::instance();

  elevManager->loadElevationPath(srtmDir);

  std::ofstream file;
  file.open(outfname);

  for (int i = 0; i < size; ++i)
  {
    PointType point;
    point[0] = origin[0] + i * spacing;
    point[1] = origin[1];
    ossimGpt ossimWorldPoint;
    ossimWorldPoint.lon = point[0];
    ossimWorldPoint.lat = point[1];
    double height       = elevManager->getHeightAboveMSL(ossimWorldPoint);
    file << point << " -> " << height << std::endl;
  }

  file.close();

  return EXIT_SUCCESS;
}
