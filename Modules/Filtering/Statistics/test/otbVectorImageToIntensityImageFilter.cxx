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
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImageToIntensityImageFilter.h"

int otbVectorImageToIntensityImageFilter(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double                                                                  PixelType;
  typedef otb::VectorImage<PixelType, Dimension>                                  InputImageType;
  typedef otb::Image<PixelType, Dimension>                                        OutputImageType;
  typedef otb::ImageFileReader<InputImageType>                                    ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>                                   WriterType;
  typedef otb::VectorImageToIntensityImageFilter<InputImageType, OutputImageType> FilterType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  InputImageType::PixelType refPixel;

  filter->SetInput(reader->GetOutput());

  writer->SetInput(filter->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
