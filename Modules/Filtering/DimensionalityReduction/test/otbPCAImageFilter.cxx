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


#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbCommandProgressUpdate.h"

#include "otbPCAImageFilter.h"

int otbPCAImageFilterTest(int, char* argv[])
{
  const unsigned int nbComponents = atoi(argv[5]);

  bool normalization = false;
  if (std::string(argv[4]).compare("true") == 0)
    normalization = true;
  
  bool whitening = false;
  if (std::string(argv[6]).compare("true") == 0)
    whitening = true;

  // Main type definition
  const unsigned int Dimension = 2;
  typedef double     PixelType;
  typedef otb::VectorImage<PixelType, Dimension> ImageType;

  // Reading input images
  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer                     reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  // Image filtering
  typedef otb::PCAImageFilter<ImageType, ImageType, otb::Transform::FORWARD> FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());
  filter->SetNumberOfPrincipalComponentsRequired(nbComponents);
  filter->SetUseNormalization(normalization);
  filter->SetWhitening(whitening);

  typedef otb::CommandProgressUpdate<FilterType> CommandType;
  CommandType::Pointer                           observer = CommandType::New();
  filter->AddObserver(itk::ProgressEvent(), observer);

  // Writing
  typedef otb::ImageFileWriter<ImageType> ImageWriterType;
  ImageWriterType::Pointer                writer = ImageWriterType::New();
  writer->SetFileName(argv[2]);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  typedef otb::PCAImageFilter<ImageType, ImageType, otb::Transform::INVERSE> InvFilterType;
  InvFilterType::Pointer invFilter = InvFilterType::New();
  invFilter->SetInput(filter->GetOutput());
  if (normalization)
  {
    invFilter->SetMeanValues(filter->GetMeanValues());
    invFilter->SetStdDevValues(filter->GetStdDevValues());
  }
  invFilter->SetTransformationMatrix(filter->GetTransformationMatrix());

  typedef otb::CommandProgressUpdate<InvFilterType> CommandType2;
  CommandType2::Pointer                             invObserver = CommandType2::New();
  invFilter->AddObserver(itk::ProgressEvent(), invObserver);

  ImageWriterType::Pointer invWriter = ImageWriterType::New();
  invWriter->SetFileName(argv[3]);
  invWriter->SetInput(invFilter->GetOutput());
  invWriter->Update();


  return EXIT_SUCCESS;
}
