/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "otbSinclairToCircularCovarianceMatrixImageFilter.h"
#include "itkVariableLengthVector.h"

int otbSinclairToCircularCovarianceMatrixFunctor(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef std::complex<double>                   ComplexType;
  typedef itk::VariableLengthVector<ComplexType> OutputType;

  typedef otb::Functor::SinclairToCircularCovarianceMatrixFunctor<ComplexType, ComplexType, ComplexType, ComplexType, OutputType >         FunctorType;

  OutputType  result(10);
  FunctorType funct;
  OutputType  outputFunct(10);

  result[0] = ComplexType( 32.,  0. );
  result[1] = ComplexType(  24., 0. );
  result[2] = ComplexType(  16., 0.);
  result[3] = ComplexType( 8.,   0  );
  result[4] = ComplexType( 18.,   0. );
  result[5] = ComplexType( 12. ,  0. );
  result[6] = ComplexType(  6. ,  0. );
  result[7] = ComplexType(  8,    0. );
  result[8] = ComplexType(  4.,   0. );
  result[9] = ComplexType(  2,    0. );

  funct.operator()(outputFunct, ComplexType(1., 4.), ComplexType(2., 3.), ComplexType(3., 2.), ComplexType(4., 1.));


  if( std::abs(result[0]-outputFunct[0]) > 1e-10 ||
      std::abs(result[1]-outputFunct[1]) > 1e-10 ||
      std::abs(result[2]-outputFunct[2]) > 1e-10 ||
      std::abs(result[3]-outputFunct[3]) > 1e-10 ||
      std::abs(result[4]-outputFunct[4]) > 1e-10 ||
      std::abs(result[5]-outputFunct[5]) > 1e-10 ||
      std::abs(result[6]-outputFunct[6]) > 1e-10 ||
      std::abs(result[7]-outputFunct[7]) > 1e-10 ||
      std::abs(result[8]-outputFunct[8]) > 1e-10 ||
      std::abs(result[9]-outputFunct[9]) > 1e-10)
  {
    std::cout<<"Test gives :"<<std::endl;
    std::cout<<outputFunct<<std::endl;
    std::cout<<"Wanted results are :"<<std::endl;
    std::cout<<result<<std::endl;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
