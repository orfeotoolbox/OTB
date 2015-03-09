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

#include <fstream>
#include <cstdlib>

#include "ossim/elevation/ossimElevManager.h"
#include "ossim/base/ossimFilename.h"

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
  const char *        outfname = argv[2];

  typedef itk::Point<double, 2> PointType;
  PointType origin;
  double    spacing;
  int       size;

  origin[0] = atof(argv[3]);
  origin[1] = atof(argv[4]);
  spacing = atof(argv[5]);
  size = atoi(argv[6]);

  ossimElevManager * elevManager = ossimElevManager::instance();

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
    double height = elevManager->getHeightAboveMSL(ossimWorldPoint);
    file << point << " -> " << height << std::endl;
    }

  file.close();

  return EXIT_SUCCESS;
}
