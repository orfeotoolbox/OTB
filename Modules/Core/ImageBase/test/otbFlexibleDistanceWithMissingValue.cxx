/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom; Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <iostream>
#include <cstdlib>
#include <iomanip>
#include "itkVariableLengthVector.h"
#include "otbFlexibleDistanceWithMissingValue.h"

int otbFlexibleDistanceWithMissingValue(int itkNotUsed(argc), char * argv[])
{

  typedef itk::VariableLengthVector<double>                             VectorType;
  typedef otb::Statistics::FlexibleDistanceWithMissingValue<VectorType> DistanceType;

  const double epsilon(atof(argv[1]));

  float        a = 2.0;
  float        b = 0.5;
  unsigned int dim = 3;
  DistanceType::SetAlphaBeta(a, b);

  VectorType x(dim);
  x.Fill(1.);

  VectorType y(dim);
  y.Fill(2.);

  DistanceType::Pointer dist = DistanceType::New();
  double                distanceValue = dist->Evaluate(x, y);
  std::cout << std::setprecision(20) << std::endl;
  std::cout << "dim, a, b          : " << dim << "," << a << "," << b << std::endl;
  std::cout << "dim*vcl_pow(3, b) : " << dim*vcl_pow(3, b) << std::endl;
  std::cout << "Distance         : " << distanceValue << std::endl;
  std::cout << "Epsilon          : " << epsilon << std::endl;
  std::cout << "-> Tests diff    : " << vcl_abs(distanceValue - dim * vcl_pow(3, b)) << std::endl;

  if (vcl_abs(distanceValue - dim * vcl_pow(3, b)) < epsilon) return EXIT_SUCCESS;
  else return EXIT_FAILURE;
}
