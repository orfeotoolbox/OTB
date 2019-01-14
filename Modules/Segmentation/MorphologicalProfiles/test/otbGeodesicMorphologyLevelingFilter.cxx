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

#include "otbGeodesicMorphologyLevelingFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbGeodesicMorphologyLevelingFilter(int itkNotUsed(argc), char * argv[])
{
  const char * infname     = argv[1];
  const char * inconvfname = argv[2];
  const char * inconcfname = argv[3];
  const char * outfname    = argv[4];

  typedef otb::Image<double, 2>                                                  ImageType;
  typedef otb::GeodesicMorphologyLevelingFilter<ImageType, ImageType, ImageType> FilterType;
  typedef otb::ImageFileReader<ImageType>                                        ReaderType;
  typedef otb::ImageFileWriter<ImageType>                                        WriterType;

  ReaderType::Pointer convreader, concreader, reader;
  reader = ReaderType::New();
  convreader = ReaderType::New();
  concreader = ReaderType::New();

  reader->SetFileName(infname);
  convreader->SetFileName(inconvfname);
  concreader->SetFileName(inconcfname);

  FilterType::Pointer filter = FilterType::New();

  using namespace otb::Functor::LevelingFunctor_tags;
  filter->SetVariadicNamedInput<pixel>(reader->GetOutput());
  filter->SetVariadicNamedInput<convex_pixel>(convreader->GetOutput());
  filter->SetVariadicNamedInput<concave_pixel>(concreader->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outfname);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
