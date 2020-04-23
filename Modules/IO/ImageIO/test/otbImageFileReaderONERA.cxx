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


#include <iostream>
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiChannelExtractROI.h"

int otbImageFileReaderONERATest(int argc, char* argv[])
{
  // Verify the number of parameters in the command line
  const char* inputFilename  = argv[1];
  const char* outputFilename = argv[2];

  typedef float InputPixelType;
  typedef float OutputPixelType;

  typedef otb::VectorImage<InputPixelType, 2>  InputImageType;
  typedef otb::VectorImage<OutputPixelType, 2> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  typedef otb::MultiChannelExtractROI<OutputPixelType, OutputPixelType> ExtractROIFilterType;

  ReaderType::Pointer           reader           = ReaderType::New();
  ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
  WriterType::Pointer           writer           = WriterType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  if (argc == 7)
  {
    extractROIFilter->SetStartX(atoi(argv[3]));
    extractROIFilter->SetStartY(atoi(argv[4]));
    extractROIFilter->SetSizeX(atoi(argv[5]));
    extractROIFilter->SetSizeY(atoi(argv[6]));
    extractROIFilter->SetInput(reader->GetOutput());
    writer->SetInput(extractROIFilter->GetOutput());
  }
  else if (argc == 3)
  {
    writer->SetInput(reader->GetOutput());
  }

  writer->Update();

  return EXIT_SUCCESS;
}
