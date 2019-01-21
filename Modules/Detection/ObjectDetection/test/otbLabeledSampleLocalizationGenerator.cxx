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
#include "otbLabeledSampleLocalizationGenerator.h"


int otbLabeledSampleLocalizationGenerator(int itkNotUsed(argc), char* argv[])
{
  const char * inputVD1  = argv[1];
  const char * inputVD2  = argv[2];
  const char * outputVD  = argv[3];

  typedef otb::VectorData<>                                        VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType>                VectorDataReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType>                VectorDataWriterType;
  typedef otb::LabeledSampleLocalizationGenerator<VectorDataType>  GeneratorType;

  // instantiation
  VectorDataReaderType::Pointer  reader1    = VectorDataReaderType::New();
  VectorDataReaderType::Pointer  reader2    = VectorDataReaderType::New();
  VectorDataWriterType::Pointer  writer     = VectorDataWriterType::New();
  GeneratorType::Pointer         generator  = GeneratorType::New();

  reader1->SetFileName(inputVD1);
  //reader1->Update();

  reader2->SetFileName(inputVD2);
  //reader2->Update();

  generator->PushBackInput(reader1->GetOutput());
  generator->PushBackInput(reader2->GetOutput());

  generator->SetSeed(0); // enable reproducible random number sequence
  generator->SetClassKey("Class");
  generator->SetNoClassIdentifier(0);
  generator->SetInhibitionRadius(5);
  generator->SetRandomLocalizationDensity(0.004);
  generator->SetNbMaxIteration(1000);
  generator->SetNumberOfPositiveSamplesPerPoint(2);

  //generator->Update();

  writer->SetFileName(outputVD);
  writer->SetInput(generator->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
