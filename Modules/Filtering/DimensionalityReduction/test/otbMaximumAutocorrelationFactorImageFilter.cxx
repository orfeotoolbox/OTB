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

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMaximumAutocorrelationFactorImageFilter.h"

typedef otb::VectorImage<unsigned short, 2> ImageType;
typedef otb::VectorImage<double, 2>         OutputImageType;
typedef otb::ImageFileReader<ImageType>       ReaderType;
typedef otb::ImageFileWriter<OutputImageType> WriterType;
typedef otb::MaximumAutocorrelationFactorImageFilter<ImageType, OutputImageType> MADFilterType;


int otbMaximumAutocorrelationFactorImageFilter(int itkNotUsed(argc), char* argv[])
{
  char* infname  = argv[1];
  char* outfname = argv[2];

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  MADFilterType::Pointer madFilter = MADFilterType::New();
  madFilter->SetInput(reader->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(madFilter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();

  std::cout << "V: " << std::endl;
  std::cout << madFilter->GetV() << std::endl;

  std::cout << "Auto-correlation: " << std::endl;
  std::cout << madFilter->GetAutoCorrelation() << std::endl;

  return EXIT_SUCCESS;
}
