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

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbLocalRxDetectorFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbFunctorImageFilter.h"

int LocalRXDetectorTest(int itkNotUsed(argc), char* argv[])
{
  typedef double PixelType;
  typedef otb::VectorImage<PixelType, 2> VectorImageType;
  typedef otb::Image<PixelType, 2>       ImageType;
  typedef otb::Functor::LocalRxDetectionFunctor<PixelType> LocalRxDetectorFunctorType;

  typedef otb::ImageFileReader<VectorImageType> ReaderType;

  typedef unsigned char                        WritePixelType;
  typedef otb::Image<WritePixelType>           WriteImageType;
  typedef otb::ImageFileWriter<WriteImageType> WriterType;
  typedef itk::RescaleIntensityImageFilter<ImageType, WriteImageType> RescalerType;

  /////////// PARAMETERS ///////////

  const char*        filename       = argv[1];
  const char*        outputFilename = argv[2];
  const unsigned int externalRadius = atoi(argv[3]);
  const unsigned int internalRadius = atoi(argv[4]);

  //////// RX Filtering /////////

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(filename);

  LocalRxDetectorFunctorType detectorFunctor;

  detectorFunctor.SetInternalRadius(internalRadius, internalRadius);

  auto rxDetector = otb::NewFunctorFilter(detectorFunctor, {{externalRadius, externalRadius}});

  rxDetector->SetInputs(reader->GetOutput());

  //////// Rescaling /////////

  RescalerType::Pointer rescaler = RescalerType::New();
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  rescaler->SetInput(rxDetector->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(rescaler->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
