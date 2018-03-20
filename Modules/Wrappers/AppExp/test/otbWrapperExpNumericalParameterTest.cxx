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

#include "otbWrapperExpNumericalParameter.h"
#include <complex>

using namespace otb::WrapperExp;

int NumericalInstantiationTest( int  , char **  )
{
  NumericalParameter< int >::Pointer intParam = NumericalParameter< int >::New();
  NumericalParameter< double >::Pointer doubleParam = NumericalParameter< double >::New();
  // NumericalParameter< std::complex<int> >::Pointer complexParam = NumericalParameter< std::complex<int> >::New();
  return EXIT_SUCCESS;
}

int NumericalManipulationTest( int , char ** )
{
  NumericalParameter< int >::Pointer param = NumericalParameter<int>::New();
  int min = param->GetMinimumValue();
  int max = param->GetMaximumValue();
  int def = param->GetDefaultValue();
  int val = param->GetIntegerValue();
  float fval = param->GetFloattingValue();
  std::string sval = param->GetLitteralValue();
  bool has_val = param->HasValue();

  return EXIT_SUCCESS;
}

#define NumericalManipulation( type ) \