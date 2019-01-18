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


#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"
#include "otbMultiChannelExtractROI.h"

#include "otbSystem.h"

int otbTestMultiExtractMultiUpdate(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double                                                        InternPixelType;
  typedef otb::VectorImage<InternPixelType, Dimension>                  InputImageType;
  typedef otb::ImageFileReader<InputImageType>                          ReaderType;
  typedef otb::ImageFileWriter<InputImageType>                          WriterType;
  typedef otb::MultiChannelExtractROI<InternPixelType, InternPixelType> ExtractInternVectorType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  ExtractInternVectorType::Pointer extractInternVector = ExtractInternVectorType::New();
  //readerMS->UpdateOutputInformation();

  extractInternVector->SetInput(reader->GetOutput());
  extractInternVector->SetStartX(atoi(argv[3]));
  extractInternVector->SetStartY(atoi(argv[4]));
  extractInternVector->SetSizeX(atoi(argv[5]));
  extractInternVector->SetSizeY(atoi(argv[6]));

  extractInternVector->SetFirstChannel(atoi(argv[7]));
  extractInternVector->SetLastChannel(atoi(argv[8]));

  extractInternVector->Update();
  extractInternVector->Update();
  extractInternVector->Update();

  writer->SetInput(extractInternVector->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
