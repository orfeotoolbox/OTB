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




#include "itkMacro.h"
#include <iostream>

#include "otbVectorImage.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImageFileWriter.h"

int otbStreamingShortImageFileWriterTest(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];
  int          iStreaming(::atoi(argv[3]));
  bool         streaming = (bool) (iStreaming);
  int          NumberOfStreamDivisions(10);
  if (streaming == true)
    {
    NumberOfStreamDivisions = ::atoi(argv[4]);
    }

  typedef unsigned short InputPixelType;
  typedef unsigned short OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::VectorImage<InputPixelType,  Dimension> InputImageType;
  typedef otb::VectorImage<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>           ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> StreamingWriterType;
  typedef otb::ImageFileWriter<OutputImageType>          WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  if (streaming == true)
    {
    std::cout << "Streaming writing test" << std::endl;
    StreamingWriterType::Pointer writer = StreamingWriterType::New();
    writer->SetFileName(outputFilename);
    writer->SetNumberOfDivisionsStrippedStreaming(NumberOfStreamDivisions);
    writer->SetInput(reader->GetOutput());
    writer->Update();
    }
  else
    {
    std::cout << "Writing test" << std::endl;
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(outputFilename);
    writer->SetInput(reader->GetOutput());
    writer->Update();
    }

  return EXIT_SUCCESS;
}
