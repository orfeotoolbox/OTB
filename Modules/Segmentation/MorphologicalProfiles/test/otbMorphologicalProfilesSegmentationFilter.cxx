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

#include "otbMorphologicalProfilesSegmentationFilter.h"
#include "itkMacro.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbMorphologicalProfilesSegmentationFilter(int argc, char* argv[])
{
  if (argc != 6)
  {
    std::cerr << "Usage: " << argv[0] << " inputFileName outputFileName start step size" << std::endl;
    return EXIT_FAILURE;
  }

  const char*        inputFileName  = argv[1];
  const char*        outputFileName = argv[2];
  const unsigned int start          = atoi(argv[3]);
  const unsigned int step           = atoi(argv[4]);
  const unsigned int size           = atoi(argv[5]);

  const unsigned int Dimension = 2;
  typedef float      PixelType;
  typedef otb::Image<PixelType, Dimension>    ImageType;
  typedef otb::Image<unsigned int, Dimension> LabelImageType;
  typedef otb::ImageFileReader<ImageType>      ReaderType;
  typedef otb::ImageFileWriter<LabelImageType> LabelWriterType;

  typedef otb::MorphologicalProfilesSegmentationFilter<ImageType, LabelImageType> FilterType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(inputFileName);

  // Set filter parameters
  filter->SetProfileStart(start);
  filter->SetProfileSize(size);
  filter->SetProfileStep(step);
  filter->SetInput(reader->GetOutput());

  // filter->SetNumberOfThreads(1);
  LabelWriterType::Pointer writerLabel = LabelWriterType::New();

  writerLabel->SetFileName(outputFileName);
  writerLabel->SetInput(filter->GetOutput());

  writerLabel->Update();

  return EXIT_SUCCESS;
}
