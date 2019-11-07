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


#include "otbSpectralSensitivityReader.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"

using namespace otb;


int otbSpectralSensitivityReaderTest(int itkNotUsed(argc), char* argv[])
{
  const char* dataPath  = argv[1];
  const char* imageName = argv[2];
  const char* output    = argv[3];

  typedef VectorImage<double>        ImageType;
  typedef ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(imageName);
  reader->UpdateOutputInformation();

  SpectralSensitivityReader::Pointer spectSen = SpectralSensitivityReader::New();

  spectSen->SetDataPath(dataPath);
  spectSen->SetImage(reader->GetOutput());

  spectSen->Update();

  std::ofstream fout(output);
  fout << spectSen << std::endl;
  fout.close();

  return EXIT_SUCCESS;
}

int otbSpectralSensitivityReaderGenericTest(int itkNotUsed(argc), char* argv[])
{
  const char* sensitivityFileName = argv[1];
  const char* imageName           = argv[2];
  const char* output              = argv[3];

  typedef VectorImage<double>        ImageType;
  typedef ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(imageName);
  reader->UpdateOutputInformation();

  SpectralSensitivityReader::Pointer spectSen = SpectralSensitivityReader::New();

  spectSen->SetFileName(sensitivityFileName);
  //   std::cout << "test : " << "filemane " << spectSen->GetFileName() <<std::endl;
  spectSen->SetImage(reader->GetOutput());

  spectSen->Update();

  std::ofstream fout(output);
  fout << spectSen << std::endl;
  fout.close();

  return EXIT_SUCCESS;
}
