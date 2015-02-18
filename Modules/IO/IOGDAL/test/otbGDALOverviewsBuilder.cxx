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
#include "otbStandardOneLineFilterWatcher.h"

using namespace otb;

int otbGDALOverviewsBuilderNew(int itkNotUsed(argc), char* itkNotUsed(argv) [])
{
  typedef otb::GDALOverviewsBuilder GDALOverviewsBuilderType;
  GDALOverviewsBuilderType::Pointer object = GDALOverviewsBuilderType::New();

  std::cout << object << std::endl;

  return EXIT_SUCCESS;
}

int otbGDALOverviewsBuilder(int itkNotUsed(argc), char* argv[])
{
  const char * inputFilename  = argv[1];
  int nbResolution = atoi(argv[2]);
  std::string filename(inputFilename);

  typedef otb::GDALOverviewsBuilder FilterType;
  FilterType::Pointer filter = FilterType::New();

  otb::GDALResamplingType resamp = AVERAGE;

  filter->SetInputFileName(filename);
  filter->SetNbOfResolutions(nbResolution);
  filter->SetResamplingMethod(resamp);

  {
    StandardOneLineFilterWatcher watcher(filter,"Overviews creation");
    filter->Update();
  }

  otb::GDALImageIO::Pointer io = otb::GDALImageIO::New();
  io->SetFileName(inputFilename);
  io->CanReadFile(inputFilename);
  io->ReadImageInformation();
  //std::cout << io->GetNumberOfOverviews() << std::endl;

  if (io->GetNumberOfOverviews() != static_cast<unsigned int>(nbResolution)-1 )
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
