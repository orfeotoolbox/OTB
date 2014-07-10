/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "itkMacro.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include <iostream>

#include "otbSVMImageModelEstimator.h"

#include "otbImageFileReader.h"

int otbSVMImageModelEstimatorTrainOneClass(int argc, char* argv[])
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
