/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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


// Includes
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbMPIConfig.h"
#include "otbMPIVrtWriter.h"

#include <cstdlib>
#include <iostream>

// Test
int otbMPIReadWriteTest(int argc, char* argv[])
{

  // Mono-thread execution
  itk::MultiThreader::SetGlobalMaximumNumberOfThreads(1);
  itk::MultiThreader::SetGlobalDefaultNumberOfThreads(1);

  // MPI Initialization
  typedef otb::MPIConfig MPIConfigType;
  MPIConfigType::Pointer config = MPIConfigType::Instance();
  config->Init(argc, argv, true);

  // Verify the number of parameters in the command line
  if (argc != 3)
  {
    std::stringstream message;
    message << "Usage: " << std::endl;
    message << argv[0] << " inputImageFile outputImageFile " << std::endl;
    config->logError(message.str());
    config->abort(EXIT_FAILURE);
  }

  // Image types
  typedef float      PixelType;
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<PixelType, Dimension> ImageType;

  // Reader
  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer                     reader = ReaderType::New();

  // Reader configuration
  std::string inputFilename = std::string(argv[1]);
  reader->SetFileName(inputFilename);

  // Update MPI Pipeline
  std::string outputFilename = std::string(argv[2]);

  otb::WriteMPI(reader->GetOutput(), outputFilename);

  config->barrier();

  return EXIT_SUCCESS;
}
