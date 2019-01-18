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
#include "otbImageToVectorImageCastFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImage.h"
#include "otbImage.h"

int otbImageToVectorImageCastFilter(int itkNotUsed(argc), char * argv[])
{
  const char *       infilename = argv[1];
  const char *       outfilename = argv[2];
  const unsigned int Dimension = 2;
  typedef double PixelType;

  typedef otb::Image<PixelType, Dimension>       ImageType;
  typedef otb::VectorImage<PixelType, Dimension> VectorImageType;

  typedef otb::ImageToVectorImageCastFilter<ImageType, VectorImageType> VectorImageCastFilterType;
  typedef otb::ImageFileReader<ImageType>                               ReaderType;
  typedef otb::ImageFileWriter<VectorImageType>                         WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infilename);
  VectorImageCastFilterType::Pointer cast = VectorImageCastFilterType::New();
  cast->SetInput(reader->GetOutput());
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outfilename);
  writer->SetInput(cast->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
