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

#include <stdio.h>

#include "otbPointSetDensityGaussianFunction.h"
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"

int otbPointSetDensityGaussianFunctionNew(int, char*[])
{

  const unsigned int Dimension = 2;
  typedef float PixelType;

  typedef itk::VariableLengthVector<PixelType>                           RealVectorType;
  typedef itk::PointSet<RealVectorType, Dimension>                       PointSetType;
  typedef otb::PointSetDensityGaussianFunction <PointSetType, PixelType> FunctionType;

  /**Instantiation of a Smart Pointer*/
  FunctionType::Pointer filter = FunctionType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
