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

#include "otbImage.h"
#include "otbVectorImage.h"
#include "itkMacro.h"
#include <iostream>

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbImageFileReaderWithExtendedFilename(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char* inputFilename   = argv[1];
  const char* outputFilename1 = argv[2];
  const char* outputFilename2 = argv[3];

  typedef float      InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType, Dimension> InputImageType;

  typedef otb::ImageFileReader<InputImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();

  std::ofstream file1;
  file1.open(outputFilename1);

  std::ofstream file2;
  file2.open(outputFilename2);

  reader->SetFileName(inputFilename);
  reader->Update();

  file1 << reader->GetOutput()->GetImageKeywordlist();

  file2 << "ProjRef: " << reader->GetOutput()->GetProjectionRef() << std::endl;
  file2 << "Origin: " << reader->GetOutput()->GetOrigin() << std::endl;
  file2 << "Spacing: " << reader->GetOutput()->GetSignedSpacing() << std::endl;

  return EXIT_SUCCESS;
}

int otbImageFileWriterWithExtendedFilename(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char* inputFilename  = argv[1];
  const char* outputFilename = argv[2];

  typedef unsigned char InputPixelType;
  const unsigned int    Dimension = 2;

  typedef otb::VectorImage<InputPixelType, Dimension> InputImageType;

  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::ImageFileWriter<InputImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);
  writer->SetInput(reader->GetOutput());
  writer->SetFileName(outputFilename);
  writer->Update();

  return EXIT_SUCCESS;
}
