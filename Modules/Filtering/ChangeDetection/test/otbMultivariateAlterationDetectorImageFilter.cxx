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

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultivariateAlterationDetectorImageFilter.h"

typedef otb::VectorImage<unsigned short, 2> ImageType;
typedef otb::VectorImage<double, 2>         OutputImageType;
typedef otb::ImageFileReader<ImageType>       ReaderType;
typedef otb::ImageFileWriter<OutputImageType> WriterType;
typedef otb::MultivariateAlterationDetectorImageFilter<ImageType, OutputImageType> MADFilterType;


int otbMultivariateAlterationDetectorImageFilter(int itkNotUsed(argc), char* argv[])
{
  char* infname1 = argv[1];
  char* infname2 = argv[2];
  char* outfname = argv[3];

  ReaderType::Pointer reader1 = ReaderType::New();
  reader1->SetFileName(infname1);

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName(infname2);

  MADFilterType::Pointer madFilter = MADFilterType::New();
  madFilter->SetInput1(reader1->GetOutput());
  madFilter->SetInput2(reader2->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(madFilter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();

  std::cout << "V1: " << std::endl;
  std::cout << madFilter->GetV1() << std::endl;

  std::cout << "V2: " << std::endl;
  std::cout << madFilter->GetV2() << std::endl;

  std::cout << "Rho: " << std::endl;
  std::cout << madFilter->GetRho() << std::endl;

  return EXIT_SUCCESS;
}
