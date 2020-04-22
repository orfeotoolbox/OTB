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


#include "itkMacro.h"
#include <iostream>
#include <fstream>
#include <string>

#include "otbImage.h"
#include "otbImageFileReader.h"

int otbImageTest(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char* inputFilename       = argv[1];
  const char* outputAsciiFilename = argv[2];
  //  unsigned int  GCPnum((unsigned int)::atoi(argv[2]));

  typedef unsigned char InputPixelType;
  const unsigned int    Dimension = 2;

  std::ofstream file;

  file.open(outputAsciiFilename);

  typedef otb::Image<InputPixelType, Dimension> InputImageType;

  InputImageType::Pointer image = InputImageType::New();

  typedef otb::ImageFileReader<InputImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(inputFilename);
  reader->GenerateOutputInformation();

  image = reader->GetOutput();

  std::cout << "------ IMAGE --------" << std::endl;
  std::cout << image << std::endl;
  std::cout << "---------------------" << std::endl;

  file << "------ IMAGE --------" << std::endl;
  file << "Spacing " << image->GetSignedSpacing() << std::endl;
  file << "Origin " << image->GetOrigin() << std::endl;
  file << "Projection REF " << image->GetProjectionRef() << std::endl;
  file << "GCP Projection " << image->GetGCPProjection() << std::endl;
  unsigned int GCPCount = image->GetGCPCount();
  file << "GCP Count " << image->GetGCPCount() << std::endl;

  for (unsigned int GCPnum = 0; GCPnum < GCPCount; GCPnum++)
  {
    file << "GCP[" << GCPnum << "] Id " << image->GetGCPId(GCPnum) << std::endl;
    file << "GCP[" << GCPnum << "] Info " << image->GetGCPInfo(GCPnum) << std::endl;
    file << "GCP[" << GCPnum << "] Row " << image->GetGCPRow(GCPnum) << std::endl;
    file << "GCP[" << GCPnum << "] Col " << image->GetGCPCol(GCPnum) << std::endl;
    file << "GCP[" << GCPnum << "] X " << image->GetGCPX(GCPnum) << std::endl;
    file << "GCP[" << GCPnum << "] Y " << image->GetGCPY(GCPnum) << std::endl;
    file << "GCP[" << GCPnum << "] Z " << image->GetGCPZ(GCPnum) << std::endl;
    file << "----------------" << std::endl;
  }

  InputImageType::VectorType tab = image->GetGeoTransform();

  file << "Geo Transform " << std::endl;
  for (unsigned int i = 0; i < tab.size(); ++i)
  {
    file << " " << i << " -> " << tab[i] << std::endl;
  }
  tab.clear();

  tab = image->GetUpperLeftCorner();
  file << "Corners " << std::endl;
  for (unsigned int i = 0; i < tab.size(); ++i)
  {
    file << " UL[" << i << "] -> " << tab[i] << std::endl;
  }
  tab.clear();

  tab = image->GetUpperRightCorner();
  for (unsigned int i = 0; i < tab.size(); ++i)
  {
    file << " UR[" << i << "] -> " << tab[i] << std::endl;
  }
  tab.clear();

  tab = image->GetLowerLeftCorner();
  for (unsigned int i = 0; i < tab.size(); ++i)
  {
    file << " LL[" << i << "] -> " << tab[i] << std::endl;
  }
  tab.clear();

  tab = image->GetLowerRightCorner();
  for (unsigned int i = 0; i < tab.size(); ++i)
  {
    file << " LR[" << i << "] -> " << tab[i] << std::endl;
  }
  tab.clear();

  file.close();

  return EXIT_SUCCESS;
}
