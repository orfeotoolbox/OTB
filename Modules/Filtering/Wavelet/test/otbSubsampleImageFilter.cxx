/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
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



#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbSubsampleImageFilter.h"

int otbSubsampleImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char * inputFileName = argv[1];
  const char * outputFileName1 = argv[2];
  const char * outputFileName2 = argv[3];

  const unsigned int Dimension = 2;
  typedef double                           PixelType;
  typedef otb::Image<PixelType, Dimension> ImageType;

  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);

  typedef otb::SubsampleImageFilter<ImageType, ImageType, otb::Wavelet::INVERSE>
  InvFilterType;
  InvFilterType::Pointer invFilter = InvFilterType::New();
  invFilter->SetInput(reader->GetOutput());

  itk::Index<Dimension> delta = {{2, 2}};
  invFilter->SetSubsampleFactor(delta);

  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer1 = WriterType::New();
  writer1->SetFileName(outputFileName1);
  writer1->SetInput(invFilter->GetOutput());
  writer1->Update();

  typedef otb::SubsampleImageFilter<ImageType, ImageType, otb::Wavelet::FORWARD>
  ForwardFilterType;
  ForwardFilterType::Pointer filter = ForwardFilterType::New();
  filter->SetInput(invFilter->GetOutput());
  filter->SetSubsampleFactor(invFilter->GetSubsampleFactor());

  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetFileName(outputFileName2);
  writer2->SetInput(filter->GetOutput());
  writer2->Update();

  return EXIT_SUCCESS;
}
