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
#include "itkPointSet.h"
#include <iostream>

#include "otbSVMPointSetModelEstimator.h"

int otbSVMPointSetModelEstimatorNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  typedef std::vector<double> InputPixelType;
  typedef double              LabelPixelType;
  const unsigned int Dimension = 2;

  typedef itk::PointSet<InputPixelType,  Dimension> MeasurePointSetType;
  typedef itk::PointSet<LabelPixelType,  Dimension> LabelPointSetType;
  typedef otb::SVMPointSetModelEstimator<MeasurePointSetType,
      LabelPointSetType>   EstimatorType;

  EstimatorType::Pointer estimator = EstimatorType::New();

  std::cout << estimator << std::endl;

  return EXIT_SUCCESS;
}
