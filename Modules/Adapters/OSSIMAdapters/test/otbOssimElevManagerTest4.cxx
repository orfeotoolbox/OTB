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
#include <sstream>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
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

int otbOssimElevManagerTest4(int argc, char* argv[])
{

  if (argc != 9)
    {
    std::cout << "Usage: " << std::endl;
    std::cout << argv[0] << " srtmDir outfname originX originY spacingX spacingY sizeX sizeY" << std::endl;
    return EXIT_FAILURE;
    }

  const ossimFilename srtmDir(argv[1]);
  const char *        outfname = argv[2];

  double origin[2];
  double spacing[2];
  int    size[2];

  origin[0] = atof(argv[3]);
  origin[1] = atof(argv[4]);
  spacing[0] = atof(argv[5]);
  spacing[1] = atof(argv[6]);
  size[0] =   atoi(argv[7]);
  size[1] =   atoi(argv[8]);

  double* image = new double[size[0] * size[1]];

  ossimElevManager * elevManager = ossimElevManager::instance();

  elevManager->loadElevationPath(srtmDir);

  for (int j = 0; j < size[1]; ++j)
    {
    for (int i = 0; i < size[0]; ++i)
      {
      double point[2];
      point[0] = origin[0] + i * spacing[0];
      point[1] = origin[1] + j * spacing[1];

      ossimGpt ossimWorldPoint;
      ossimWorldPoint.lon = point[0];
      ossimWorldPoint.lat = point[1];
      double height = elevManager->getHeightAboveMSL(ossimWorldPoint);

      if (!ossim::isnan(height))
        {
        // Fill the image
        image[i + j * size[0]] = height;
        }
      else
        {
        // Back to the MNT default value
        image[i + j * size[0]] = 0;
        }
      }
    }

  std::ofstream file;
  std::cout << outfname << std::endl;
  file.open(outfname, ios::binary | ios::out);

  file.write(reinterpret_cast<char*>(image), sizeof(double) * size[0] * size[1]);
  file.close();

  delete[] image;

  //Create the header
  std::stringstream headerName;
  headerName << outfname << ".hdr";
  std::ofstream headerFile;
  headerFile.open(headerName.str());
  headerFile << "ENVI" << std::endl;
  headerFile << "samples = " << size[0] << std::endl;
  headerFile << "lines   = " << size[1] << std::endl;
  headerFile << "bands   = 1" << std::endl;
  headerFile << "header offset = 0" << std::endl;
  headerFile << "file type = ENVI Standard" << std::endl;
  headerFile << "data type = 5" << std::endl;
  headerFile << "interleave = bsq" << std::endl;
  headerFile << "byte order = 0" << std::endl;
  headerFile << std::endl;
  headerFile.close();

  //Check for the index 216, 51 (difference of 4 in the output)
    {
    int    i = 216;
    int    j = 51;
    double point[2];
    point[0] = origin[0] + i * spacing[0];
    point[1] = origin[1] + j * spacing[1];

    ossimGpt ossimWorldPoint;
    ossimWorldPoint.lon = point[0];
    ossimWorldPoint.lat = point[1];
    double height = elevManager->getHeightAboveMSL(ossimWorldPoint);
    std::cout << "Output value at (216, 51): " << height << std::endl;
    std::cout << "At lon: " << ossimWorldPoint.lon <<  " , lat: " << ossimWorldPoint.lat << std::endl;

    }

  return EXIT_SUCCESS;
}
