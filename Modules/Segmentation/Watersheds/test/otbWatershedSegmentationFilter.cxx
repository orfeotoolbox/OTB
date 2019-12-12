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

#include "otbWatershedSegmentationFilter.h"
#include "itkMacro.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkGradientMagnitudeImageFilter.h"

int otbWatershedSegmentationFilter(int argc, char* argv[])
{
  if (argc != 5)
  {
    std::cerr << "Usage: " << argv[0] << " inputFileName outputLabelFileName threshold level" << std::endl;
    return EXIT_FAILURE;
  }

  const char*  inputFileName       = argv[1];
  const char*  outputLabelFileName = argv[2];
  const double threshold           = atof(argv[3]);
  const double level               = atof(argv[4]);

  /* maxit - threshold */
  const unsigned int Dimension = 2;
  typedef float      PixelType;
  typedef otb::Image<PixelType, Dimension>    InputImageType;
  typedef otb::Image<unsigned int, Dimension> LabelImageType;
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::ImageFileWriter<LabelImageType> LabelWriterType;

  typedef itk::GradientMagnitudeImageFilter<InputImageType, InputImageType> GradientMagnitudeFilterType;
  typedef otb::WatershedSegmentationFilter<InputImageType, LabelImageType>  FilterType;

  // Instantiating object
  GradientMagnitudeFilterType::Pointer gradientMagnitudeFilter = GradientMagnitudeFilterType::New();
  FilterType::Pointer                  filter                  = FilterType::New();
  ReaderType::Pointer                  reader                  = ReaderType::New();

  reader->SetFileName(inputFileName);

  gradientMagnitudeFilter->SetInput(reader->GetOutput());

  // Set filter parameters
  filter->SetThreshold(threshold);
  filter->SetLevel(level);
  filter->SetInput(gradientMagnitudeFilter->GetOutput());

  LabelWriterType::Pointer writerLabel = LabelWriterType::New();
  writerLabel->SetFileName(outputLabelFileName);
  writerLabel->SetInput(filter->GetOutput());
  writerLabel->Update();

  return EXIT_SUCCESS;
}
