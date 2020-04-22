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
#include "otbVectorImage.h"
#include "otbStreamingShrinkImageFilter.h"

int otbStreamingShrinkImageFilter(int itkNotUsed(argc), char* argv[])
{
  char*              inputFilename  = argv[1];
  char*              outputFilename = argv[2];
  unsigned int       shrinkFactor   = atoi(argv[3]);
  const unsigned int Dimension      = 2;

  typedef unsigned int PixelType;
  typedef otb::VectorImage<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;
  typedef otb::StreamingShrinkImageFilter<ImageType, ImageType> ShrinkType;

  ReaderType::Pointer reader = ReaderType::New();
  ShrinkType::Pointer shrink = ShrinkType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);
  shrink->SetShrinkFactor(shrinkFactor);

  shrink->SetInput(reader->GetOutput());
  shrink->Update();

  writer->SetInput(shrink->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
