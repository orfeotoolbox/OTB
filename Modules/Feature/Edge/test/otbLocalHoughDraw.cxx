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
#include <list>

#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbLocalHoughFilter.h"
#include "otbDrawLineSpatialObjectListFilter.h"


#include "itkRescaleIntensityImageFilter.h"

int otbLocalHoughDraw(int itkNotUsed(argc), char* argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  unsigned int RadiusX((unsigned int) ::atoi(argv[3]));
  unsigned int RadiusY((unsigned int) ::atoi(argv[4]));
  unsigned int NumberOfLines((unsigned int) ::atoi(argv[5]));

  typedef unsigned char InputPixelType;
  typedef unsigned char OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::LocalHoughFilter<InputImageType> FilterType;
  //typedef otb::ImageToLineSpatialObjectListFilter< InputImageType >   FilterType;

  FilterType::Pointer filter = FilterType::New();

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef itk::RescaleIntensityImageFilter<InputImageType> RescalerType;

  ReaderType::Pointer   reader = ReaderType::New();
  WriterType::Pointer   writer = WriterType::New();
  RescalerType::Pointer rescaler = RescalerType::New();

  reader->SetFileName(inputFilename);
  reader->Update();

  writer->SetFileName(outputFilename);

  FilterType::SizeType Radius;
  Radius[0] = RadiusX;
  Radius[1] = RadiusY;

  filter->SetRadius(Radius);
  filter->SetNumberOfLines(NumberOfLines);

  rescaler->SetInput(reader->GetOutput());
  filter->SetInput(rescaler->GetOutput());
  filter->Update();

  typedef otb::DrawLineSpatialObjectListFilter<InputImageType, OutputImageType> DrawFilterType;

  DrawFilterType::Pointer drawfilter = DrawFilterType::New();

  drawfilter->SetInputLineSpatialObjectList(filter->GetOutput());

  drawfilter->SetInput(reader->GetOutput());
  writer->SetInput(drawfilter->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
