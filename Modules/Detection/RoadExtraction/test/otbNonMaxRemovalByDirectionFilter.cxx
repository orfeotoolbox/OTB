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
#include "otbNonMaxRemovalByDirectionFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbNonMaxRemovalByDirectionFilter(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double                                                               PixelType;
  typedef otb::Image<PixelType, Dimension>                                     ImageType;
  typedef otb::ImageFileReader<ImageType>                                      ReaderType;
  typedef otb::ImageFileWriter<ImageType>                                      WriterType;
  typedef otb::NonMaxRemovalByDirectionFilter<ImageType, ImageType, ImageType> NonMaxRemovalByDirectionFilterType;

  // Instantiating object
  ReaderType::Pointer modReader = ReaderType::New();
  ReaderType::Pointer dirReader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  modReader->SetFileName(argv[1]);
  dirReader->SetFileName(argv[2]);
  writer->SetFileName(argv[3]);
  NonMaxRemovalByDirectionFilterType::Pointer filter = NonMaxRemovalByDirectionFilterType::New();
  filter->SetInput(modReader->GetOutput());
  filter->SetInputDirection(dirReader->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
