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


#include "itkFixedArray.h"

#include "otbMath.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbAngularProjectionImageFilter.h"

int otbAngularProjectionImageFilterTest(int, char* argv[])
{
  std::string inputImageName1 = argv[1];
  std::string inputImageName2 = argv[2];
  std::string outputImageName = argv[3];

  // Main type definition
  const unsigned int Dimension = 2;

  typedef float  PixelType;
  typedef double PrecisionType;
  typedef itk::FixedArray<PrecisionType, 1> AngleType;
  typedef otb::Image<PixelType, Dimension>  ImageType;

  // Reading images
  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer                     reader1 = ReaderType::New();
  reader1->SetFileName(inputImageName1);

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName(inputImageName2);

  // Parameter (to be changed if necessary)
  AngleType angle;
  angle[0] = otb::CONST_PI_2;

  // Filtering
  typedef otb::AngularProjectionImageFilter<ImageType, ImageType, AngleType, PrecisionType> FilterType;

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(0, reader1->GetOutput());
  filter->SetInput(1, reader2->GetOutput());
  filter->SetAngleArray(angle);

  // Saving
  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer                     writer = WriterType::New();
  writer->SetFileName(outputImageName);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
