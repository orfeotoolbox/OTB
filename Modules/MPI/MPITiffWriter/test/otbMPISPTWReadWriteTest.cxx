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


// Includes
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbMPIConfig.h"
#include "otbSimpleParallelTiffWriter.h"

#include <iostream>
#include <cstdlib>

int otbMPISPTWReadWriteTest(int argc, char* argv[])
{

  // Initialize MPI environment
  otb::MPIConfig::Pointer config = otb::MPIConfig::Instance();
  config->Init(argc,argv);

  // Get command line arguments
  if (argc != 3)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile outputImageFile " << std::endl;
    return EXIT_SUCCESS;
    }

  // Image typedefs
  typedef float PixelType;
  typedef otb::VectorImage<PixelType>	ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::SimpleParallelTiffWriter<ImageType> WriterType;

  // Reader configuration
  ReaderType::Pointer reader = ReaderType::New();
  std::string inputFilename = std::string(argv[1]);
  reader->SetFileName(inputFilename);
  reader->GenerateOutputInformation();

  // Writer configuration
  WriterType::Pointer writer = WriterType::New();
  std::string outputFilename = std::string(argv[2]);
  writer->SetFileName(outputFilename);
  writer->SetInput(reader->GetOutput());
  
  // Execute the MPI pipeline
  try{
    writer->Update();
  }
  catch (std::exception & err) {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err.what() << std::endl;

    config->abort(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;

}
