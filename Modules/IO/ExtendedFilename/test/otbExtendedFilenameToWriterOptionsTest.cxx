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

#include "otbExtendedFilenameToWriterOptions.h"
#include <iostream>
#include <fstream>

typedef otb::ExtendedFilenameToWriterOptions FilenameHelperType;

int otbExtendedFilenameToWriterOptions(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char* inputExtendedFilename = argv[1];
  const char* outputFilename        = argv[2];

  FilenameHelperType::Pointer helper = FilenameHelperType::New();

  helper->SetExtendedFileName(inputExtendedFilename);

  std::ofstream file;
  file.open(outputFilename);

  file << helper->SimpleFileNameIsSet() << std::endl;
  file << helper->GetSimpleFileName() << std::endl;

  file << helper->WriteGEOMFileIsSet() << std::endl;
  file << helper->GetWriteGEOMFile() << std::endl;

  file << helper->gdalCreationOptionsIsSet() << std::endl;
  if (helper->gdalCreationOptionsIsSet())
    for (unsigned int i = 0; i < helper->GetgdalCreationOptions().size(); i++)
    {
      file << helper->GetgdalCreationOptions()[i] << std::endl;
    }

  return EXIT_SUCCESS;
}
