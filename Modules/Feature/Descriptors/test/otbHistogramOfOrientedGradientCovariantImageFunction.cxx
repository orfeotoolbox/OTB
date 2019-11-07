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
#include <iomanip>
#include <fstream>
#include "itkMacro.h"
#include "otbImage.h"

#include "otbImageFileReader.h"
#include "itkGradientImageFilter.h"
#include "otbHistogramOfOrientedGradientCovariantImageFunction.h"

typedef unsigned short ShortPixelType;
const unsigned int     Dimension = 2;

typedef otb::Image<ShortPixelType, Dimension> InputImageType;
typedef otb::ImageFileReader<InputImageType>                                       ReaderType;
typedef itk::GradientImageFilter<InputImageType>                                   GradientFilterType;
typedef GradientFilterType::OutputImageType                                        CovariantImageType;
typedef otb::HistogramOfOrientedGradientCovariantImageFunction<CovariantImageType> FunctionType;


int otbHistogramOfOrientedGradientCovariantImageFunction(int itkNotUsed(argc), char* argv[])
{
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  GradientFilterType::Pointer gradient = GradientFilterType::New();
  gradient->SetInput(reader->GetOutput());
  gradient->SetUseImageSpacing(false);
  gradient->SetUseImageDirection(false);
  gradient->Update();

  // Instantiating object
  FunctionType::Pointer function = FunctionType::New();
  function->SetInputImage(gradient->GetOutput());

  InputImageType::IndexType index;

  unsigned int radius = atoi(argv[3]);
  index[0]            = atoi(argv[4]);
  index[1]            = atoi(argv[5]);

  function->SetNeighborhoodRadius(radius);

  FunctionType::OutputType value = function->EvaluateAtIndex(index);

  std::ofstream ofs(argv[2]);

  for (unsigned i = 0; i < 5; ++i)
  {
    for (unsigned j = 0; j < function->GetNumberOfOrientationBins(); ++j)
    {
      ofs << value[i][j] << "\t";
    }
    ofs << std::endl;
  }

  ofs.close();

  return EXIT_SUCCESS;
}
