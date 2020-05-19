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

#include "otbEstimateInnerProductPCAImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbVectorImage.h"

int otbEstimateInnerProductPCAImageFilter(int itkNotUsed(argc), char* argv[])
{
  typedef double     PixelType;
  const unsigned int Dimension      = 2;
  const char*        inputFileName  = argv[1];
  const char*        outputFilename = argv[2];
  const unsigned int numberOfPrincipalComponentsRequired(atoi(argv[3]));
  const bool         centerdata = atoi(argv[4]);

  typedef otb::VectorImage<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;
  typedef otb::EstimateInnerProductPCAImageFilter<ImageType, ImageType> PCAFilterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);

  PCAFilterType::Pointer pcafilter = PCAFilterType::New();

  // Compute Inner Product raw
  pcafilter->SetNumberOfPrincipalComponentsRequired(numberOfPrincipalComponentsRequired);
  pcafilter->SetCenterData(centerdata);
  pcafilter->SetInput(reader->GetOutput());
  pcafilter->Update();

  writer->SetInput(pcafilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
