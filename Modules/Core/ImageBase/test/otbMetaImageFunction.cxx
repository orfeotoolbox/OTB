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


#include "otbMetaImageFunction.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbFlusserMomentsImageFunction.h"
#include "otbImageFunctionAdaptor.h"

typedef unsigned short InputPixelType;
typedef double         PrecisionType;
const unsigned int     Dimension = 2;

typedef otb::Image<InputPixelType, Dimension> InputImageType;
typedef otb::ImageFileReader<InputImageType>             ReaderType;
typedef otb::FlusserMomentsImageFunction<InputImageType> FlusserFunctionType;
typedef otb::ImageFunctionAdaptor<FlusserFunctionType, PrecisionType> FunctionType;

typedef otb::MetaImageFunction<PrecisionType, double> MetaImageFunctionType;
typedef MetaImageFunctionType::PointType  PointType;
typedef MetaImageFunctionType::OutputType OutputType;


int otbMetaImageFunction(int itkNotUsed(argc), char* argv[])
{
  // Read the input image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->Update();

  // Build  flusser functions with different radius
  FunctionType::Pointer function1 = FunctionType::New();
  FunctionType::Pointer function2 = FunctionType::New();
  FunctionType::Pointer function3 = FunctionType::New();

  function1->SetInputImage(reader->GetOutput());
  function2->SetInputImage(reader->GetOutput());
  function3->SetInputImage(reader->GetOutput());

  function1->GetInternalImageFunction()->SetNeighborhoodRadius(3);
  function2->GetInternalImageFunction()->SetNeighborhoodRadius(5);
  function3->GetInternalImageFunction()->SetNeighborhoodRadius(7);

  std::ofstream outputStream(argv[2]);

  MetaImageFunctionType::Pointer metaFunction = MetaImageFunctionType::New();
  metaFunction->AddFunction(function1);
  metaFunction->AddFunction(function2);
  metaFunction->AddFunction(function3);

  outputStream << "Initial number of functions: " << metaFunction->GetNumberOfFunctions() << std::endl;

  metaFunction->RemoveNthFunction(2);

  outputStream << "Number of functions after removing the 2nd: " << metaFunction->GetNumberOfFunctions() << std::endl;

  metaFunction->ClearFunctions();

  outputStream << "Number of functions after clear: " << metaFunction->GetNumberOfFunctions() << std::endl;

  metaFunction->AddFunction(function1);
  metaFunction->AddFunction(function2);
  metaFunction->AddFunction(function3);

  outputStream << "Adding functions again: " << metaFunction->GetNumberOfFunctions() << std::endl;

  // For coverage
  metaFunction->GetNthFunction(0);

  PointType p;
  p[0] = atof(argv[3]);
  p[1] = atof(argv[4]);

  OutputType output = metaFunction->Evaluate(p);

  outputStream << "Evaluate(" << p << ") = " << output << std::endl;

  outputStream.close();

  return EXIT_SUCCESS;
}
