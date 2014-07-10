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

int otbSVMImageModelEstimatorNew(int argc, char* argv[])
{
  typedef double InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::VectorImage<InputPixelType,  Dimension> InputImageType;

  typedef otb::Image<InputPixelType,  Dimension> TrainingImageType;

  typedef otb::SVMImageModelEstimator<InputImageType,
      TrainingImageType>   EstimatorType;

  EstimatorType::Pointer svmModel = EstimatorType::New();

  std::cout << svmModel << std::endl;

  return EXIT_SUCCESS;
}
