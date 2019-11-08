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

#include "otbSFSTexturesImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbSFSTexturesImageFilterTest(int itkNotUsed(argc), char* argv[])
{
  typedef double     PixelType;
  const unsigned int Dimension = 2;

  std::string  inName           = argv[1];
  std::string  outNameLength    = argv[2];
  std::string  outNameWidth     = argv[3];
  std::string  outNameWMean     = argv[4];
  std::string  outNameRatio     = argv[5];
  std::string  outNameSD        = argv[6];
  PixelType    spectThresh      = atof(argv[7]);
  unsigned int spatialThresh    = atoi(argv[8]);
  unsigned int dirNb            = atoi(argv[9]);
  unsigned int maxConsideration = atoi(argv[10]);
  double       alpha            = atof(argv[11]);

  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;
  typedef otb::SFSTexturesImageFilter<ImageType, ImageType> FilterType;

  FilterType::Pointer filter       = FilterType::New();
  ReaderType::Pointer reader       = ReaderType::New();
  WriterType::Pointer writerLength = WriterType::New();
  WriterType::Pointer writerWidth  = WriterType::New();
  WriterType::Pointer writerWMean  = WriterType::New();
  WriterType::Pointer writerRatio  = WriterType::New();
  WriterType::Pointer writerSD     = WriterType::New();

  reader->SetFileName(inName);
  reader->GenerateOutputInformation();

  filter->SetSpectralThreshold(spectThresh);
  filter->SetSpatialThreshold(spatialThresh);
  filter->SetNumberOfDirections(dirNb);
  filter->SetRatioMaxConsiderationNumber(maxConsideration);
  filter->SetAlpha(alpha);
  // disable PSI texture
  filter->SetFeatureStatus(FilterType::PSI, false);
  filter->SetInput(reader->GetOutput());

  writerLength->SetFileName(outNameLength);
  writerLength->SetInput(filter->GetLengthOutput());
  writerLength->Update();

  writerWidth->SetFileName(outNameWidth);
  writerWidth->SetInput(filter->GetWidthOutput());
  writerWidth->Update();

  writerWMean->SetFileName(outNameWMean);
  writerWMean->SetInput(filter->GetWMeanOutput());
  writerWMean->Update();

  writerRatio->SetFileName(outNameRatio);
  writerRatio->SetInput(filter->GetRatioOutput());
  writerRatio->Update();

  writerSD->SetFileName(outNameSD);
  writerSD->SetInput(filter->GetSDOutput());
  writerSD->Update();

  return EXIT_SUCCESS;
}
