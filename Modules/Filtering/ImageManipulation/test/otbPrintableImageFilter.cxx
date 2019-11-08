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

#include "itkMacro.h"
#include "otbImageFileWriter.h"
#include "otbVectorImage.h"

#include "otbPrintableImageFilter.h"

int otbPrintableImageFilter(int itkNotUsed(argc), char* argv[])
{
  const char* inputFilename  = argv[1];
  const char* outputFilename = argv[2];

  typedef double     InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::VectorImage<InputPixelType, Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType>      ReaderType;
  typedef otb::PrintableImageFilter<InputImageType> FilterType;
  typedef FilterType::OutputImageType               OutputImageType;
  typedef otb::ImageFileWriter<OutputImageType>     WriterType;

  FilterType::Pointer printableImageFilter = FilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  printableImageFilter->SetInput(reader->GetOutput());
  printableImageFilter->SetChannel(3);
  printableImageFilter->SetChannel(2);
  printableImageFilter->SetChannel(1);
  writer->SetInput(printableImageFilter->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
