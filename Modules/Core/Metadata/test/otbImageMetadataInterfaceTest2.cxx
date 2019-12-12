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

#include <fstream>
#include <iostream>
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageMetadataInterfaceFactory.h"

int otbImageMetadataInterfaceTest2(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char* inputFilename  = argv[1];
  const char* outputFilename = argv[2];

  typedef otb::Image<double, 2> InputImageType;
  typedef otb::ImageFileReader<InputImageType> ImageReaderType;

  ImageReaderType::Pointer reader = ImageReaderType::New();
  reader->SetFileName(inputFilename);
  reader->UpdateOutputInformation();

  otb::ImageMetadataInterfaceBase::Pointer lImageMetadata = otb::ImageMetadataInterfaceFactory::CreateIMI(reader->GetOutput()->GetMetaDataDictionary());

  std::ofstream file;
  file.open(outputFilename);

  // Test the image interface
  lImageMetadata->SetImage(reader->GetOutput());
  file << lImageMetadata->GetProjectionRef() << std::endl;
  file.close();

  return EXIT_SUCCESS;
}
