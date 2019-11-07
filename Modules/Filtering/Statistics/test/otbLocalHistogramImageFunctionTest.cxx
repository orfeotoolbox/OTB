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


#include <iomanip>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbLocalHistogramImageFunction.h"

int otbLocalHistogramImageFunctionTest(int itkNotUsed(argc), char* argv[])
{
  const char* inputFilename  = argv[1];
  const char* outputFilename = argv[2];

  typedef float      InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType, Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType>             ReaderType;
  typedef otb::LocalHistogramImageFunction<InputImageType> FunctionType;


  ReaderType::Pointer   reader   = ReaderType::New();
  FunctionType::Pointer function = FunctionType::New();

  reader->SetFileName(inputFilename);
  reader->Update();

  function->SetInputImage(reader->GetOutput());

  InputImageType::IndexType index;
  index[0] = atoi(argv[3]);
  index[1] = atoi(argv[4]);

  function->SetNeighborhoodRadius(atoi(argv[5]));
  function->SetHistogramMin(atoi(argv[6]));
  function->SetHistogramMax(atoi(argv[7]));
  function->GaussianSmoothingOff();

  FunctionType::OutputType Result;
  Result = function->EvaluateAtIndex(index);


  std::ofstream outputStream(outputFilename);
  outputStream << std::setprecision(10) << std::endl;
  outputStream << "Index: " << index << std::endl;

  for (unsigned int i = 0; i < function->GetNumberOfHistogramBins(); ++i)
  {
    outputStream << "Bin[" << i << "] = "
                 << "[" << Result->GetBinMin(0, i) << ", " << Result->GetBinMax(0, i) << "[ -> " << Result->GetFrequency(i, 0) << std::endl;
  }

  outputStream.close();

  return EXIT_SUCCESS;
}
