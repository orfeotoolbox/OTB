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

#include "otbImage.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkCastImageFilter.h"

/**
 * This test reproduces a problem encountered with CastImageFilter and a streaming
 * writer. When the image to write is already loaded in memory and InPlaceOn() is
 * called, only the first stripe is repeated across the output image.
 * Issue number 0000428
 */
int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout << argv[0] << " <input image> <output image>" << std::endl;
    return EXIT_FAILURE;
  }

  typedef otb::Image<float, 2> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;
  typedef itk::CastImageFilter<ImageType, ImageType> CastFilterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->Update();

  CastFilterType::Pointer caster = CastFilterType::New();
  caster->SetInput(reader->GetOutput());
  caster->InPlaceOn();

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(caster->GetOutput());
  writer->SetFileName(argv[2]);
  // writer->SetAutomaticTiledStreaming(1024);
  writer->SetNumberOfDivisionsStrippedStreaming(10);
  writer->Update();

  return EXIT_SUCCESS;
}
