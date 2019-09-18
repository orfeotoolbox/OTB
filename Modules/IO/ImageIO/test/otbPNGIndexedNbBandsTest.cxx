/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


#include "otbImageFileReader.h"
#include "otbVectorImage.h"
#include "itkRGBPixel.h"
#include "itkMacro.h"
#include <iostream>

int otbPNGIndexedNbBandsTest(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char*        inputFilename = argv[1];
  const unsigned int nbBands       = atoi(argv[2]);

  typedef itk::RGBPixel<unsigned char> InputPixelType;
  const unsigned int                   Dimension = 2;

  typedef otb::VectorImage<InputPixelType, Dimension> InputImageType;

  typedef otb::ImageFileReader<InputImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(inputFilename);

  reader->UpdateOutputInformation();

  if (reader->GetOutput()->GetNumberOfComponentsPerPixel() == nbBands)
  {
    return EXIT_SUCCESS;
  }
  else
  {
    std::cout << "Read " << reader->GetOutput()->GetNumberOfComponentsPerPixel() << " but the real number of bands is " << nbBands << std::endl;
    return EXIT_FAILURE;
  }
}
