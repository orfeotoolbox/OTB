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
#include "otbFlexibleDistanceWithMissingValue.h"

int otbFlexibleDistanceWithMissingValue( int argc, char * argv[] )
{

  typedef itk::VariableLengthVector< double > VectorType;
  typedef otb::Statistics::FlexibleDistanceWithMissingValue< VectorType > DistanceType;

  float a = 2.0;
  float b = 0.5;
  DistanceType::SetAlphaBeta(a,b);

  VectorType x(3);
  x.Fill( 1. );

  VectorType y(3);
  y.Fill( 2. );

  DistanceType::Pointer dist = DistanceType::New();
  double distanceValue = dist->Evaluate( x, y );
  std::cout << "Distance: " << distanceValue << std::endl;

  if ( distanceValue == 3. )
    return EXIT_SUCCESS;
  else
    return EXIT_FAILURE;
}



