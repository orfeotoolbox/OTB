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
#include "otbImageFileWriter.h"

#include "otbExtractROI.h"

int otbExtractROI(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];
  unsigned int startX((unsigned int) ::atoi(argv[3]));
  unsigned int startY((unsigned int) ::atoi(argv[4]));
  unsigned int sizeX((unsigned int) ::atoi(argv[5]));
  unsigned int sizeY((unsigned int) ::atoi(argv[6]));

  typedef unsigned char InputPixelType;
  typedef unsigned char OutputPixelType;

  typedef otb::ExtractROI<InputPixelType,
      OutputPixelType>   FilterType;

  typedef FilterType::InputImageType  InputImageType;
  typedef FilterType::OutputImageType OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  FilterType::Pointer filter = FilterType::New();

  filter->SetStartX(startX);
  filter->SetStartY(startY);
  filter->SetSizeX(sizeX);
  filter->SetSizeY(sizeY);

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
