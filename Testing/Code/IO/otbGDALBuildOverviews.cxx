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

#include "otbGDALDriverManagerWrapper.h"
#include "otbGDALImageIO.h"
using namespace otb::gdal;

int otbGDALBuildOverviews(int argc, char* argv[])
{
  const char * inputFilename  = argv[1];
  int nbResolution = atoi(argv[2]);
  std::string filename(inputFilename);

  if (! GDALBuildOverviewsFromResolution(filename,static_cast<unsigned int>(nbResolution)) )
    return EXIT_FAILURE;

  otb::GDALImageIO::Pointer io = otb::GDALImageIO::New();
  io->SetFileName(inputFilename);
  io->CanReadFile(inputFilename);
  io->ReadImageInformation();
  //std::cout << io->GetNumberOfOverviews() << std::endl;

  if (io->GetNumberOfOverviews() != static_cast<unsigned int>(nbResolution)-1 )
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
