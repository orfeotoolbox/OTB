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

#include "otbImageFileReader.h"
#include "otbComplexMomentsImageFunction.h"
#include "otbBCOInterpolateImageFunction.h"
#include "otbStreamingResampleImageFilter.h"


int otbComplexMomentsImageFunction(int itkNotUsed(argc), char* argv[])
{
  const char*  inputFilename = argv[1];
  unsigned int p((unsigned int)::atoi(argv[2]));
  unsigned int q((unsigned int)::atoi(argv[3]));
  const char*  outputFilename = argv[4];

  typedef unsigned char InputPixelType;
  const unsigned int    Dimension = 2;

  typedef otb::Image<InputPixelType, Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType> ReaderType;

  typedef otb::ComplexMomentsImageFunction<InputImageType> CMType;
  typedef CMType::OutputType                               OutputType;

  ReaderType::Pointer reader   = ReaderType::New();
  CMType::Pointer     function = CMType::New();

  reader->SetFileName(inputFilename);

  reader->Update();
  function->SetInputImage(reader->GetOutput());

  function->SetQmax(q);
  function->SetPmax(p);

  InputImageType::IndexType index;
  index[0] = 100;
  index[1] = 100;

  OutputType Result;

  std::ofstream outputStream(outputFilename);
  outputStream << std::setprecision(10);

  function->SetNeighborhoodRadius(3);
  Result = function->EvaluateAtIndex(index);
  for (unsigned int k = 0; k <= p; ++k)
  {
    for (unsigned int l = 0; l <= q; ++l)
    {
      outputStream << "ComplexMoment c(" << k << l << ") : " << Result.at(k).at(l) << std::endl;
    }
  }
  outputStream.close();

  return EXIT_SUCCESS;
}


int otbComplexMomentsImageFunctionScaleInvariant(int itkNotUsed(argc), char* argv[])
{
  const char*  inputFilename = argv[1];
  unsigned int p((unsigned int)::atoi(argv[2]));
  unsigned int q((unsigned int)::atoi(argv[3]));

  typedef double     InputPixelType;
  const unsigned int Dimension = 2;
  typedef otb::Image<InputPixelType, Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::StreamingResampleImageFilter<InputImageType, InputImageType, double> StreamingResampleImageFilterType;
  typedef otb::BCOInterpolateImageFunction<InputImageType, double> InterpolatorType;
  typedef otb::ComplexMomentsImageFunction<InputImageType> FunctionType;
  typedef FunctionType::OutputType                         OutputType;

  ReaderType::Pointer                       reader       = ReaderType::New();
  StreamingResampleImageFilterType::Pointer resampler    = StreamingResampleImageFilterType::New();
  InterpolatorType::Pointer                 interpolator = InterpolatorType::New();
  FunctionType::Pointer                     function1    = FunctionType::New();
  FunctionType::Pointer                     function2    = FunctionType::New();

  reader->SetFileName(inputFilename);
  reader->Update();

  interpolator->SetInputImage(reader->GetOutput());
  interpolator->SetRadius(2);
  interpolator->SetAlpha(-0.5);

  resampler->SetInput(reader->GetOutput());
  resampler->SetInterpolator(interpolator);
  StreamingResampleImageFilterType::SizeType size;
  size[0] = 1024;
  size[1] = 1024;
  resampler->SetOutputSize(size);
  resampler->SetOutputSpacing(0.5);
  resampler->Update();

  function1->SetInputImage(reader->GetOutput());
  function1->SetQmax(q);
  function1->SetPmax(p);
  InputImageType::IndexType index1;
  index1[0] = 256;
  index1[1] = 256;
  function1->SetNeighborhoodRadius(4);
  OutputType Result1 = function1->EvaluateAtIndex(index1);

  function2->SetInputImage(resampler->GetOutput());
  function2->SetQmax(q);
  function2->SetPmax(p);
  InputImageType::IndexType index2;
  index2[0] = 512;
  index2[1] = 512;
  function2->SetNeighborhoodRadius(8);
  OutputType Result2 = function2->EvaluateAtIndex(index2);

  double error;

  error = 0.0;

  for (unsigned int k = 0; k <= p; ++k)
  {
    for (unsigned int l = 0; l <= q; ++l)
    {
      error += std::pow(std::abs(Result1.at(k).at(l) - Result2.at(k).at(l)), 2);

      std::cout << "Original - C" << k << l << " : " << Result1.at(k).at(l) << "  /  Scaled - C" << k << l << " : " << Result2.at(k).at(l) << std::endl;
    }
  }

  error = std::sqrt(error) / (q + p);
  std::cout << "Error : " << error << std::endl << std::endl;

  if (error > 1E-3)
  {
    itkGenericExceptionMacro(<< "Error = " << error << "  > 1E-3     -> TEST FAILLED" << std::endl);
  }

  return EXIT_SUCCESS;
}
