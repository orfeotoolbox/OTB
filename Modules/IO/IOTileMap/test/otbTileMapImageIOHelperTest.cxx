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
#include "otbTileMapImageIOHelper.h"

int otbTileMapImageIOHelperNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{

  otb::TileMapImageIOHelper::Pointer myTileMapImageIOHelper;
  myTileMapImageIOHelper = otb::TileMapImageIOHelper::New();
  return EXIT_SUCCESS;
}

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
