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

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbUrbanAreaDetectionImageFilter.h"

int otbUrbanAreaDetectionImageFilter(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double                                 PixelType;
  typedef otb::VectorImage<PixelType, Dimension> InputVectorImageType;
  typedef otb::Image<unsigned char, Dimension>   OutputImageType;

  typedef otb::ImageFileReader<InputVectorImageType> ReaderType;

  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::UrbanAreaDetectionImageFilter<InputVectorImageType,
      OutputImageType> UrbanAreaDetectionFilterType;

  // Instantiating objects
  UrbanAreaDetectionFilterType::Pointer filter = UrbanAreaDetectionFilterType::New();
  ReaderType::Pointer                   reader = ReaderType::New();
  WriterType::Pointer                   writer = WriterType::New();

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  filter->SetInput(reader->GetOutput());
  filter->SetRedIndex(::atoi(argv[3]));
  filter->SetGreenIndex(::atoi(argv[4]));
  filter->SetNIRIndex(::atoi(argv[5]));
  filter->SetThresholdValue(::atof(argv[6]));
  filter->SetThresholdDensity(::atof(argv[7]));

  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
