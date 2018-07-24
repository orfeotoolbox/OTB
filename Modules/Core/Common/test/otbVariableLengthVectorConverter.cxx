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
#include "otbExtern.h"

#include <cstdlib>
#include <cmath>

#include "otbVariableLengthVectorConverter.h"

int otbVariableLengthVectorConverterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef itk::VariableLengthVector<double>                      InputType0;
  typedef std::vector< std::vector< double > >                   InputType1;
  typedef std::vector< std::vector< std::complex< double > > >   InputType2;
  typedef itk::FixedArray<double>                                InputType3;

  typedef float                                                  PrecisionType;

  typedef otb::VariableLengthVectorConverter<InputType0, PrecisionType>
                                                                 ConverterType0;
  typedef otb::VariableLengthVectorConverter<InputType1, PrecisionType>
                                                                 ConverterType1;
  typedef otb::VariableLengthVectorConverter<InputType2, PrecisionType>
                                                                 ConverterType2;
  typedef otb::VariableLengthVectorConverter<InputType3, PrecisionType>
                                                                 ConverterType3;

  // Instantiating object
  ConverterType0::Pointer converter0 = ConverterType0::New();
  ConverterType1::Pointer converter1 = ConverterType1::New();
  ConverterType2::Pointer converter2 = ConverterType2::New();
  ConverterType3::Pointer converter3 = ConverterType3::New();

  std::cout << converter0 << std::endl;
  std::cout << converter1 << std::endl;
  std::cout << converter2 << std::endl;
  std::cout << converter3 << std::endl;

  return EXIT_SUCCESS;
}
