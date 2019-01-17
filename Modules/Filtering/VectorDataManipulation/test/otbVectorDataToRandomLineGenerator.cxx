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



#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbVectorDataToRandomLineGenerator.h"


int otbVectorDataToRandomLineGenerator(int itkNotUsed(argc), char* argv[])
{
  const char * inputVD  = argv[1];
  const char * outputVD  = argv[2];

  typedef otb::VectorData<>                                        VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType>                VectorDataReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType>                VectorDataWriterType;
  typedef otb::VectorDataToRandomLineGenerator<VectorDataType>     GeneratorType;

  // instantiation
  VectorDataReaderType::Pointer  reader     = VectorDataReaderType::New();
  VectorDataWriterType::Pointer  writer     = VectorDataWriterType::New();
  GeneratorType::Pointer         generator  = GeneratorType::New();

  reader->SetFileName(inputVD);
  reader->Update();

  generator->SetInput(reader->GetOutput());
  generator->SetSeed(0); // enable reproducible random number sequence
  generator->SetNumberOfOutputLine(20);
  generator->SetMinLineSize(5);
  generator->SetMaxLineSize(20);

  generator->Update();

  writer->SetFileName(outputVD);
  writer->SetInput(generator->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
