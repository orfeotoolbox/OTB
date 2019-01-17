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



#include "otbFuzzyVariable.h"

int otbFuzzyVariable2Values(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef float PrecisionType;

  typedef otb::FuzzyVariable<std::string, PrecisionType> FuzzyVarType;

  FuzzyVarType::Pointer fv = FuzzyVarType::New();

  /*
   We define 2 trapezoidal functions for Low and High values
       |
     1 |--------------\     -----------------
       |               \   /
       |                \ /
       |   Low           X      High
       |                / \
       |               /   \
       |--------------/-------------------------------
       0             0.3   0.7              1
   */


  fv->SetMembership("Low", 0, 0, 0.3, 0.7);
  fv->SetMembership("High", 0.3, 0.7, 1.0, 1.0);

  PrecisionType mem;
  mem = fv->GetMembership("Low", 0.2);

  if( mem != 1 )
    return EXIT_FAILURE;

  mem = fv->GetMembership("Low", 0.9);

  if( mem != 0 )
    return EXIT_FAILURE;

  mem = fv->GetMembership("Low", 0.5);

  if( mem != 0.5 )
    return EXIT_FAILURE;

  mem = fv->GetMembership("High", 0.2);

  if( mem != 0 )
    return EXIT_FAILURE;

  mem = fv->GetMembership("High", 0.9);

  if( mem != 1 )
    return EXIT_FAILURE;

  mem = fv->GetMembership("High", 0.5);

  if( mem != 0.5 )
    return EXIT_FAILURE;

  // For the sake of coverage
  fv->RemoveMembership("Low");
  fv->Clear();

  return EXIT_SUCCESS;
}
