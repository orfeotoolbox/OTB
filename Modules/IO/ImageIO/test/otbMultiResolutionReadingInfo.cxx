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

#include <iostream>
#include <fstream>
#include <string>

#include "itkMacro.h"
#include "itksys/SystemTools.hxx"
#include "otbImageFileReader.h"
#include "otbVectorImage.h"


int otbMultiResolutionReadingInfo(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout << argv[0] << "<input filename> <output filename>" << std::endl;
    return EXIT_FAILURE;
  }
  const char* inputFilename       = argv[1];
  const char* outputAsciiFilename = argv[2];

  typedef double     PixelType;
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<PixelType, Dimension> ImageType;

  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer                     reader = ReaderType::New();

  reader->SetFileName(inputFilename);
  reader->UpdateOutputInformation();


  std::vector<std::string> desc = reader->GetOverviewsInfo();
  if (desc.empty())
    return EXIT_FAILURE;

  std::ofstream file;
  file.open(outputAsciiFilename);

  // Parse all the names to delete the path of the filename
  for (unsigned int itRes = 0; itRes < reader->GetOverviewsCount(); itRes++)
  {
    file << "RESOLUTION: " << itRes << "; "
         << "DESC: " << desc[itRes] << std::endl;
  }

  file.close();

  return EXIT_SUCCESS;
}
