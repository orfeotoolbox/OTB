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
#include "otbPixelSuppressionByDirectionImageFilter.h"

int otbPixelSuppressionByDirection(int itkNotUsed(argc), char* argv[])
{
  const char* inputFilename1 = argv[1];
  const char* inputFilename2 = argv[2];
  const char* outputFilename = argv[3];

  unsigned int RadiusX((unsigned int)::atoi(argv[4]));
  float        AngularBeam((float)::atof(argv[5]));

  typedef double     InputPixelType;
  typedef double     OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType1;
  typedef otb::ImageFileReader<InputImageType>  ReaderType2;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::PixelSuppressionByDirectionImageFilter<InputImageType, OutputImageType> FilterType;

  FilterType::Pointer filter = FilterType::New();

  FilterType::SizeType Radius;
  Radius[0] = RadiusX;
  Radius[1] = RadiusX;

  filter->SetRadius(Radius);
  filter->SetAngularBeam(AngularBeam);

  ReaderType1::Pointer reader1 = ReaderType1::New();
  ReaderType2::Pointer reader2 = ReaderType2::New();
  WriterType::Pointer  writer  = WriterType::New();

  reader1->SetFileName(inputFilename1);
  reader2->SetFileName(inputFilename2);
  writer->SetFileName(outputFilename);

  filter->SetInputImage(reader1->GetOutput());
  filter->SetInputImageDirection(reader2->GetOutput());
  writer->SetInput(filter->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
