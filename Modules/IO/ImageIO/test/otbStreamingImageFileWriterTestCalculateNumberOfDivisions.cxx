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

#include "otbImage.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbImageFileWriterTestCalculateNumberOfDivisions(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char* inputFilename  = argv[1];
  const char* outputFilename = argv[2];
  std::string MethodCalculateNumberOfStreamDivision(argv[3]);

  typedef unsigned int PixelType;
  const unsigned int   Dimension = 2;

  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> StreamingWriterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  std::cout << "Streaming writing test" << std::endl;
  StreamingWriterType::Pointer writer = StreamingWriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(reader->GetOutput());

  if (MethodCalculateNumberOfStreamDivision == "SetNumberOfDivisionsStrippedStreaming")
  {
    writer->SetNumberOfDivisionsStrippedStreaming(::atoi(argv[4]));
  }
  else if (MethodCalculateNumberOfStreamDivision == "SetNumberOfDivisionsTiledStreaming")
  {
    writer->SetNumberOfDivisionsTiledStreaming(::atoi(argv[4]));
  }
  else if (MethodCalculateNumberOfStreamDivision == "SetNumberOfLinesStrippedStreaming")
  {
    writer->SetNumberOfLinesStrippedStreaming(::atoi(argv[4]));
  }
  else if (MethodCalculateNumberOfStreamDivision == "SetAutomaticStrippedStreaming")
  {
    writer->SetAutomaticStrippedStreaming(::atoi(argv[4]));
  }
  else if (MethodCalculateNumberOfStreamDivision == "SetTileDimensionTiledStreaming")
  {
    writer->SetTileDimensionTiledStreaming(::atoi(argv[4]));
  }
  else if (MethodCalculateNumberOfStreamDivision == "SetAutomaticTiledStreaming")
  {
    writer->SetAutomaticTiledStreaming(::atoi(argv[4]));
  }
  else if (MethodCalculateNumberOfStreamDivision == "DEFAULT")
  {
  }
  else
  {
    itkGenericExceptionMacro(<< "Parameter value not authorized !!!");
  }
  writer->Update();

  return EXIT_SUCCESS;
}
