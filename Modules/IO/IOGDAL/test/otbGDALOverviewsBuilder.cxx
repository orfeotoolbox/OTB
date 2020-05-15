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


#include "otbGDALDriverManagerWrapper.h"
#include "otbGDALImageIO.h"
#include "otbStandardOneLineFilterWatcher.h"

using namespace otb;


int otbGDALOverviewsBuilder(int itkNotUsed(argc), char* argv[])
{
  const char* inputFilename = argv[1];
  int         nbResolution  = atoi(argv[2]);
  std::string filename(inputFilename);

  typedef otb::GDALOverviewsBuilder FilterType;
  FilterType::Pointer               filter = FilterType::New();

  otb::GDALResamplingType resamp = GDAL_RESAMPLING_AVERAGE;

  filter->SetInputFileName(filename);
  filter->SetNbResolutions(nbResolution);
  filter->SetResamplingMethod(resamp);

  {
    StandardOneLineFilterWatcher<> watcher(filter, "Overviews creation");
    filter->Update();
  }

  otb::GDALImageIO::Pointer io = otb::GDALImageIO::New();
  io->SetFileName(inputFilename);
  bool canRead = io->CanReadFile(inputFilename);

  if (!canRead)
  {
    std::cerr << "Failed to read file " << inputFilename << " with GdalImageIO." << std::endl;
    return EXIT_FAILURE;
  }

  io->ReadImageInformation();
  // std::cout << io->GetOverviewsCount() << std::endl;

  if (io->GetOverviewsCount() != static_cast<unsigned int>(nbResolution))
  {
    std::cout << "Got " << io->GetOverviewsCount() << " overviews, expected " << nbResolution << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
