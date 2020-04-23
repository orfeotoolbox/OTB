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


#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbOverlapSaveConvolutionImageFilter.h"

int otbOverlapSaveConvolutionImageFilter(int itkNotUsed(argc), char* argv[])
{
  const char* inputFileName  = argv[1];
  const char* outputFileName = argv[2];

  typedef double     InputPixelType;
  typedef double     OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType, Dimension>  PanchroImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  typedef otb::ImageFileReader<PanchroImageType> ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>  WriterType;
  typedef otb::OverlapSaveConvolutionImageFilter<PanchroImageType, OutputImageType> ConvFilterType;

  ReaderType::Pointer     reader     = ReaderType::New();
  WriterType::Pointer     writer     = WriterType::New();
  ConvFilterType::Pointer convFilter = ConvFilterType::New();

  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);

  ConvFilterType::InputSizeType radius;
  radius[0] = 3;
  radius[1] = 3;
  ConvFilterType::ArrayType filterCoeffs;
  filterCoeffs.SetSize((2 * radius[0] + 1) * (2 * radius[1] + 1));
  filterCoeffs.Fill(1);

  convFilter->SetRadius(radius);
  convFilter->SetFilter(filterCoeffs);
  convFilter->NormalizeFilterOn();

  convFilter->SetInput(reader->GetOutput());
  writer->SetInput(convFilter->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
