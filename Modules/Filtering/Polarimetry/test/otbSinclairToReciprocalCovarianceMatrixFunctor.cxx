/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#include "itkMacro.h"

#include "otbSinclairToReciprocalCovarianceMatrixFunctor.h"
#include "itkVariableLengthVector.h"

int otbSinclairToReciprocalCovarianceMatrixFunctor(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef std::complex<double>                   ComplexType;
  typedef itk::VariableLengthVector<ComplexType> OutputType;

  typedef otb::Functor::SinclairToReciprocalCovarianceMatrixFunctor<ComplexType, ComplexType, ComplexType, OutputType >         FunctorType;

  OutputType  result(6);
  FunctorType funct;
  OutputType outputFunct(6);

  result[0] = ComplexType(17.,  0.);
  result[1] = ComplexType(19.7989898732233,7.07106781186548);
  result[2] = ComplexType(11., 10.);
  result[3] = ComplexType(26.,  0.);
  result[4] = ComplexType(16.9705627484771,7.07106781186548);
  result[5] = ComplexType(13.,  0.);

  funct.operator ()( outputFunct, ComplexType(1., 4.), ComplexType(2., 3.), ComplexType(3., 2.) );

  if( std::abs(result[0]-outputFunct[0]) > 1e-10 ||
      std::abs(result[1]-outputFunct[1]) > 1e-10 ||
      std::abs(result[2]-outputFunct[2]) > 1e-10 ||
      std::abs(result[3]-outputFunct[3]) > 1e-10 ||
      std::abs(result[4]-outputFunct[4]) > 1e-10 ||
      std::abs(result[5]-outputFunct[5]) > 1e-10)
  {
    std::cout<<"Test gives :"<<std::endl;
    std::cout<<outputFunct<<std::endl;
    std::cout<<"Wanted results are :"<<std::endl;
    std::cout<<result<<std::endl;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
