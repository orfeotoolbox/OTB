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

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbConcatenateVectorImageFilter.h"

int otbConcatenateVectorImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char *       filename1 = argv[1];
  const char *       filename2 = argv[2];
  const char *       outputfilename = argv[3];
  const unsigned int Dimension = 2;
  typedef unsigned char                                                                        InputPixelType;
  typedef otb::VectorImage<InputPixelType, Dimension>                                          VectorImageType;
  typedef otb::ImageFileReader<VectorImageType>                                                ReaderType;
  typedef otb::ImageFileWriter<VectorImageType>                                       WriterType;
  typedef otb::ConcatenateVectorImageFilter<VectorImageType, VectorImageType, VectorImageType> FilterType;

  ReaderType::Pointer reader1 = ReaderType::New();
  ReaderType::Pointer reader2 = ReaderType::New();
  reader1->SetFileName(filename1);
  reader2->SetFileName(filename2);

  // Instantiating filter object
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput1(reader1->GetOutput());
  filter->SetInput2(reader2->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputfilename);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
