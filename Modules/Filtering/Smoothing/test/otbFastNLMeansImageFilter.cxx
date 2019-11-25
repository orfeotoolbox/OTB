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

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbFastNLMeansImageFilter.h"

int otbFastNLMeansImageFilter(int itkNotUsed(argc), char* argv[])
{
  const char* inputFilename  = argv[1];
  const char* outputFilename = argv[2];

  int HalfPatchSize = atoi(argv[3]);
  int HalfSearchSize = atoi(argv[4]);
  float Thresh = atof(argv[5]);
  
  typedef float     InputPixelType;
  typedef float     OutputPixelType;

  typedef otb::Image<InputPixelType>  InputImageType;
  typedef otb::Image<OutputPixelType> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::NLMeansFilter<InputImageType, OutputImageType> FilterType;

  FilterType::Pointer filterNLMeans = FilterType::New();

  filterNLMeans->SetHalfWindowSize(HalfPatchSize);
  filterNLMeans->SetHalfSearchSize(HalfSearchSize);
  filterNLMeans->SetCutOffDistance(Thresh);

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  filterNLMeans->SetInput(reader->GetOutput());
  writer->SetInput(filterNLMeans->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
