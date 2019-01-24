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

#include <complex>
#include <cmath>
#include "otbPolarimetricSynthesisFunctor.h"

int otbPolarimetricSynthesisFunctor(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef std::complex<double>                   ComplexType;

  typedef otb::Functor::PolarimetricSynthesisFunctor<ComplexType, ComplexType, ComplexType, ComplexType, double >         FunctorType;

  double result(449.);
  double outputFunct(0.);
  FunctorType funct;
  FunctorType::ComplexArrayType l_Ei;
  FunctorType::ComplexArrayType l_Er;

  l_Ei[0] = ComplexType(0.5, 0);
  l_Ei[1] = ComplexType(1., 1.2);
  l_Er[0] = ComplexType(-2.5, 0.);
  l_Er[1] = ComplexType(0., 1.5);

  funct.SetEi(l_Ei);
  funct.SetEi(l_Er);

  outputFunct = funct.operator ()( ComplexType(1.5, 4.5), ComplexType(2.5, 3.5), ComplexType(3.5, 2.5), ComplexType(4.5, 1.5) );


  if( std::abs(result-outputFunct) > 1e-10 )
  {
    std::cout<<"Test gives :"<<std::endl;
    std::cout<<outputFunct<<std::endl;
    std::cout<<"Wanted results are :"<<std::endl;
    std::cout<<result<<std::endl;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
