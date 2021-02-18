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


#include "itkMacro.h"
#include <iostream>

#include "otbGDALImageIO.h"

//struct sGDALImageIO : public GDALImageIO
//{
//
//};

int otbGDALImageIOImportMetadata(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char* inputFilename  = argv[1];
  const char* outputReaderFilename = argv[2];

  std::ofstream outfileR(outputReaderFilename);
  auto readerGDAL = otb::GDALImageIO::New();
  readerGDAL->SetFileName(inputFilename);

  if (readerGDAL->CanReadFile(inputFilename))
  {
    readerGDAL->ReadImageInformation();
    outfileR << readerGDAL->GetImageMetadata();
  }
  else
  {
    std::cout << "GDALImageIO cannot read the input file: " 
              << inputFilename << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
