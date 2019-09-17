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


#include "otbThresholdVectorImageFilter.h"
#include "otbVectorImage.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

/** Pixel typedefs */
typedef double       InputPixelType;
typedef unsigned int OutputPixelType;

/** Image typedefs */
const unsigned int Dimension = 2;
typedef otb::VectorImage<InputPixelType, Dimension>  InputImageType;
typedef otb::VectorImage<OutputPixelType, Dimension> OutputImageType;

typedef otb::ThresholdVectorImageFilter<InputImageType, OutputImageType> FilterType;


int otbThresholdVectorImageFilterTest(int itkNotUsed(argc), char* argv[])
{
  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  /** instantiating the filter */
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(argv[1]);
  filter->SetInput(reader->GetOutput());
  filter->SetLower(100);
  filter->SetUpper(150);
  filter->SetOutsideValue(0);

  writer->SetInput(filter->GetOutput());
  writer->SetFileName(argv[2]);
  writer->Update();

  return EXIT_SUCCESS;
}
