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

#include "itkMacro.h"

#include "otbMuellerToReciprocalCovarianceImageFilter.h"
#include "itkVariableLengthVector.h"
#include <complex>

int otbMuellerToReciprocalCovarianceFunctor(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  typedef std::complex<double>                   ComplexType;
  typedef itk::VariableLengthVector<double>      VectorDoubleType;
  typedef itk::VariableLengthVector<ComplexType> VectorComplexType;

  typedef otb::Functor::MuellerToReciprocalCovarianceFunctor<VectorDoubleType, VectorComplexType> FunctorType;

  VectorDoubleType input;
  input.SetSize(16);
  VectorComplexType outputFunct(6);
  outputFunct.SetSize(6);
  VectorComplexType result;
  result.SetSize(6);

  double val = 0;
  for (unsigned int i = 0; i < input.Size(); ++i)
  {
    input[i] = val;
    val += 0.5;
  }

  result[0] = ComplexType(1.75, 0);
  result[1] = ComplexType(2.82842712474619, 3.53553390593274);
  result[2] = ComplexType(-6.25, -5.5);
  result[3] = ComplexType(-2.5, 0);
  result[4] = ComplexType(-1.4142135623731, -1.4142135623731);
  result[5] = ComplexType(0.75, 0);

  FunctorType funct;
  funct.operator()(outputFunct, input);
  std::cout << outputFunct << std::endl;

  if (std::abs(result[0].real() - outputFunct[0].real()) > 1e-10 || std::abs(result[0].imag() - outputFunct[0].imag()) > 1e-10 ||
      std::abs(result[1].real() - outputFunct[1].real()) > 1e-10 || std::abs(result[1].imag() - outputFunct[1].imag()) > 1e-10 ||
      std::abs(result[2].real() - outputFunct[2].real()) > 1e-10 || std::abs(result[2].imag() - outputFunct[2].imag()) > 1e-10 ||
      std::abs(result[3].real() - outputFunct[3].real()) > 1e-10 || std::abs(result[3].imag() - outputFunct[3].imag()) > 1e-10 ||
      std::abs(result[4].real() - outputFunct[4].real()) > 1e-10 || std::abs(result[4].imag() - outputFunct[4].imag()) > 1e-10 ||
      std::abs(result[5].real() - outputFunct[5].real()) > 1e-10 || std::abs(result[5].imag() - outputFunct[5].imag()) > 1e-10)

  {
    std::cout << "Test gives :" << std::endl;
    std::cout << outputFunct << std::endl;
    std::cout << "Wanted results are :" << std::endl;
    std::cout << result << std::endl;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
