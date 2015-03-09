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
#include <iostream>
#include "otbSVMKernels.h"
#include "otbSVMModel.h"

int otbSVMModelCopyGenericKernelTest(int argc, char* argv[])
{
  if (argc != 3)
    {
    std::cerr << "Wrong number of arguments" << std::endl;
    return EXIT_FAILURE;
    }

  typedef unsigned char InputPixelType;
  typedef unsigned char LabelPixelType;

  typedef otb::SVMModel<InputPixelType, LabelPixelType> ModelType;

  // Create the model to be copied
  ModelType::Pointer    svmModel = ModelType::New();
  otb::RBFKernelFunctor lFunctor;
  svmModel->SetKernelFunctor(&lFunctor);
  svmModel->LoadModel(argv[1]);

  // Copy the model and print it
  ModelType::Pointer svmModelCopy;
  svmModelCopy = svmModel->GetCopy();
  svmModelCopy->SaveModel(argv[2]);

  return EXIT_SUCCESS;
}
