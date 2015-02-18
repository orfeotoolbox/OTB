/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <cstdlib>
#include <fstream>
#include <sstream>

#include "ossim/elevation/ossimElevManager.h"
#include "ossim/base/ossimFilename.h"

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
  headerFile.open(headerName.str().c_str());
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
