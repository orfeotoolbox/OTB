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

#include "otbMacro.h"
#include "otbLeastSquareAffineTransformEstimator.h"
#include "itkPoint.h"

int otbLeastSquareAffineTransformEstimatorNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef itk::Point<float,   2>                              PointType;
  typedef otb::LeastSquareAffineTransformEstimator<PointType> EstimatorType;

  // instantiation
  EstimatorType::Pointer estimator = EstimatorType::New();

  std::cout << estimator << std::endl;

  return EXIT_SUCCESS;
}
