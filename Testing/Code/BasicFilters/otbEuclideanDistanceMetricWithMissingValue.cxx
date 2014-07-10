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
#include "itkVariableLengthVector.h"
#include "otbEuclideanDistanceMetricWithMissingValue.h"

int otbEuclideanDistanceMetricWithMissingValue(int itkNotUsed(argc), char ** itkNotUsed(argv))
{

  typedef itk::VariableLengthVector<double>                              VectorType;
  typedef otb::Statistics::EuclideanDistanceMetricWithMissingValue<VectorType> DistanceType;

  DistanceType::Pointer dist = DistanceType::New();

  if (dist->IsMissingValue(0.0)) return EXIT_FAILURE;

  if (dist->IsMissingValue(1.0)) return EXIT_FAILURE;

  if (dist->IsMissingValue(-1.0)) return EXIT_FAILURE;

  /*
    std::cerr << "Is O Nan ?? -> " << dist->IsMissingValue( 0.0 ) << "\n";
    std::cerr << "Is 1 Nan ?? -> " << dist->IsMissingValue( 1.0 ) << "\n";
    std::cerr << "Is -1 Nan ?? -> " << dist->IsMissingValue( -1.0 ) << "\n";
  */

  double x = 0.0;

  dist->SetToMissingValue(x);
  if (!dist->IsMissingValue(x)) return EXIT_FAILURE;
  // std::cerr << "Is x (=" << x << ") Nan ?? -> " << dist->IsMissingValue( x ) << "\n";

  return EXIT_SUCCESS;
}
