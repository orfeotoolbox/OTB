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
