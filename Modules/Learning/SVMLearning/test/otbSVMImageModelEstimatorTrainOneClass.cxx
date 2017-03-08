/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
#include "otbImage.h"
#include "otbVectorImage.h"
#include <iostream>

#include "otbSVMImageModelEstimator.h"

#include "otbImageFileReader.h"

int otbSVMImageModelEstimatorTrainOneClass(int itkNotUsed(argc), char* argv[])
{
  const char* inputImageFileName = argv[1];
  const char* trainingImageFileName = argv[2];
  const char* outputModelFileName = argv[3];

  typedef double InputPixelType;
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<int,  Dimension>                  TrainingImageType;
  typedef otb::SVMImageModelEstimator<InputImageType,
      TrainingImageType> EstimatorType;
  typedef otb::ImageFileReader<InputImageType>    InputReaderType;
  typedef otb::ImageFileReader<TrainingImageType> TrainingReaderType;

  InputReaderType::Pointer    inputReader    = InputReaderType::New();
  TrainingReaderType::Pointer trainingReader = TrainingReaderType::New();
  EstimatorType::Pointer      svmEstimator   = EstimatorType::New();

  inputReader->SetFileName(inputImageFileName);
  trainingReader->SetFileName(trainingImageFileName);
  inputReader->Update();
  trainingReader->Update();

  svmEstimator->SetInputImage(inputReader->GetOutput());
  svmEstimator->SetTrainingImage(trainingReader->GetOutput());
  svmEstimator->SetSVMType(ONE_CLASS);

  svmEstimator->Update();

  otbGenericMsgDebugMacro(<< "Saving model");
  svmEstimator->GetModel()->SaveModel(outputModelFileName);

  return EXIT_SUCCESS;
}
