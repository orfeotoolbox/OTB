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


#include <iostream>
#include <iomanip>
#include <fstream>
#include "itkMacro.h"
#include "otbImage.h"

#include "otbImageFileReader.h"
#include "otbRadiometricMomentsImageFunction.h"


int otbRadiometricMomentsImageFunction(int itkNotUsed(argc), char* argv[])
{
  const char* inputFilename  = argv[1];
  const char* outputFilename = argv[2];

  typedef unsigned char InputPixelType;
  const unsigned int    Dimension = 2;

  typedef otb::Image<InputPixelType, Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType>                 ReaderType;
  typedef otb::RadiometricMomentsImageFunction<InputImageType> FunctionType;
  typedef FunctionType::OutputType                             OutputType;

  ReaderType::Pointer   reader   = ReaderType::New();
  FunctionType::Pointer function = FunctionType::New();

  reader->SetFileName(inputFilename);
  reader->Update();
  function->SetInputImage(reader->GetOutput());

  InputImageType::IndexType index;
  index[0] = 100;
  index[1] = 100;

  function->SetNeighborhoodRadius(3);
  OutputType Result;
  Result = function->EvaluateAtIndex(index);

  std::ofstream outputStream(outputFilename);
  outputStream << std::setprecision(10) << "Radiometric moments: [10]" << std::endl;

  for (unsigned int j = 1; j < 5; ++j)
  {
    outputStream << "Radiometric Moment(" << j << ") = " << Result[j - 1] << std::endl;
  }

  outputStream.close();

  return EXIT_SUCCESS;
}
