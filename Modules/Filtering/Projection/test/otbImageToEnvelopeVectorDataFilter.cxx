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


#include "otbImage.h"
#include "otbImageToEnvelopeVectorDataFilter.h"
#include "otbVectorData.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"

typedef unsigned short PixelType;
typedef otb::Image<PixelType, 2> ImageType;
typedef otb::ImageFileReader<ImageType>           ReaderType;
typedef otb::VectorData<>                         VectorDataType;
typedef otb::VectorDataFileWriter<VectorDataType> WriterType;
typedef otb::ImageToEnvelopeVectorDataFilter<ImageType, VectorDataType> FilterType;


int otbImageToEnvelopeVectorDataFilter(int itkNotUsed(argc), char* argv[])
{
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(argv[2]);
  writer->Update();

  return EXIT_SUCCESS;
}
