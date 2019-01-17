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

#include "otbImageOfVectorsToMonoChannelExtractROI.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbImageOfVectorsToMonoChannelExtractROI(int itkNotUsed(argc), char * argv[])
{
  const int Dimension = 2;
  typedef unsigned char                         ScalarPixelType;
  typedef itk::Vector<unsigned char, Dimension> VectorPixelType;

  const char * inputImageFileName = argv[1];
  const char * outputImageFileName = argv[7];
  typedef otb::Image<ScalarPixelType, Dimension>                                       ScalarImageType;
  typedef otb::Image<VectorPixelType, Dimension>                                       VectorImageType;
  typedef otb::ImageOfVectorsToMonoChannelExtractROI<VectorImageType, ScalarImageType> FilterType;

  typedef otb::ImageFileReader<VectorImageType> ReaderType;
  typedef otb::ImageFileWriter<ScalarImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputImageFileName);
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputImageFileName);
  // Instantiating object
  FilterType::Pointer object = FilterType::New();

  object->SetStartX(atoi(argv[2]));
  object->SetStartY(atoi(argv[3]));
  object->SetSizeX(atoi(argv[4]));
  object->SetSizeY(atoi(argv[5]));
  object->SetChannel(atoi(argv[6]));

  object->SetInput(reader->GetOutput());
  writer->SetInput(object->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
