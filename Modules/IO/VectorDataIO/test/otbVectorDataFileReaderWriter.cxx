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


#include "otbMacro.h"

#include "otbVectorData.h"
#include "otbVectorDataFileWriter.h"
#include "otbVectorDataFileReader.h"

int otbVectorDataFileReaderWriter(int itkNotUsed(argc), char* argv[])
{

  typedef otb::VectorData<double, 2> VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType> ReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType> WriterType;

  // Instantiation
  ReaderType::Pointer     reader = ReaderType::New();
  WriterType::Pointer     writer = WriterType::New();
  VectorDataType::Pointer data   = VectorDataType::New();

  reader->SetFileName(argv[1]);

  writer->SetFileName(argv[2]);
  writer->SetInput(reader->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
