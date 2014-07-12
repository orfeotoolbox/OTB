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


#include "itkFixedArray.h"
#include "otbTimeSeries.h"

int otbPolynomialTimeSeriesTest(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{

  typedef double CoefficientPrecisionType;
  const unsigned int Degree = 2;
  typedef otb::PolynomialTimeSeries< Degree, CoefficientPrecisionType > FunctionType;
  typedef FunctionType::CoefficientsType CoefficientsType;

  CoefficientsType coefs;

  coefs[0] = 1;
  coefs[1] = 2;
  coefs[2] = 4;

  FunctionType f;
  f.SetCoefficients( coefs );

  for( unsigned int i=0; i<=Degree; ++i)
    if(coefs[i] != f.GetCoefficient(i))
      return EXIT_FAILURE;

  int VALMAX = 10;
  for(int val = -VALMAX; val <= VALMAX; ++val)
    if( f.GetValue( val ) != (coefs[0]+coefs[1]*val+coefs[2]*val*val) )
      return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
