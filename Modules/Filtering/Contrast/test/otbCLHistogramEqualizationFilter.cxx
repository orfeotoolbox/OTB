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
#include "otbImage.h"
#include "otbCLHistogramEqualizationFilter.h"

int otbCLHistogramEqualizationFilter(int itkNotUsed(argc), char* argv[])
{
  typedef int        InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType, Dimension>                              InputImageType;
  typedef otb::CLHistogramEqualizationFilter<InputImageType, InputImageType> FilterType;
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::ImageFileWriter<InputImageType> WriterType;

  ReaderType::Pointer reader(ReaderType::New());
  WriterType::Pointer writer(WriterType::New());
  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);
  reader->UpdateOutputInformation();

  FilterType::Pointer histoEqualize(FilterType::New());

  histoEqualize->SetInput(reader->GetOutput());
  histoEqualize->SetMin(0);
  histoEqualize->SetMax(255);
  histoEqualize->SetNbBin(256);
  auto size = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
  size[0] /= 4;
  size[1] /= 4;
  histoEqualize->SetThumbSize(size);

  writer->SetInput(histoEqualize->GetOutput());
  writer->Update();
  return EXIT_SUCCESS;
}
