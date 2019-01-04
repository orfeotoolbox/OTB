/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
// Images
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"

// Concatener
#include "otbConcatenateVectorDataFilter.h"


typedef otb::VectorData<>                                 VectorDataType;
typedef otb::VectorDataFileReader<VectorDataType>         ReaderType;
typedef otb::VectorDataFileWriter<VectorDataType>         WriterType;
typedef otb::ConcatenateVectorDataFilter<VectorDataType>  ConcatenateFilterType;

int otbConcatenateVectorDataFilter (int argc, char * argv[])
{
  // Instantiate a concatenate filter
  // Concatenate the vector datas
  ConcatenateFilterType::Pointer concatenate = ConcatenateFilterType::New();
  for (int idx = 1; idx < argc-1 ; ++idx)
    {
    // Reader object
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(argv[idx]);
    reader->Update();

    concatenate->AddInput(reader->GetOutput());
    }

  // Write the output
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[argc-1]);
  writer->SetInput(concatenate->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
