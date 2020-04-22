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

#include "otbVcaImageFilter.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

const unsigned int Dimension = 2;
typedef double     PixelType;
typedef double     PrecisionType;

typedef otb::Image<PixelType, Dimension>       ImageType;
typedef otb::VectorImage<PixelType, Dimension> VectorImageType;
typedef otb::VCAImageFilter<VectorImageType> VCAFilterType;

typedef otb::ImageFileReader<VectorImageType> ReaderType;
typedef otb::ImageFileWriter<VectorImageType> WriterType;


int otbVCAImageFilterTestHighSNR(int itkNotUsed(argc), char* argv[])
{
  const char*        inputImage   = argv[1];
  const char*        outputImage  = argv[2];
  const unsigned int nbEndmembers = atoi(argv[3]);

  ReaderType::Pointer readerImage = ReaderType::New();
  readerImage->SetFileName(inputImage);

  VCAFilterType::Pointer vca = VCAFilterType::New();
  vca->SetNumberOfEndmembers(nbEndmembers);
  vca->SetInput(readerImage->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputImage);
  writer->SetInput(vca->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
