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

int otbFuzzyVariableSetValue(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  typedef float PrecisionType;
  typedef otb::FuzzyVariable<std::string, PrecisionType> FuzzyVarType;

  FuzzyVarType::Pointer fv = FuzzyVarType::New();

  fv->SetMembership("Low", 0, 0, 0.25, 0.75);
  fv->SetMembership("Medium", 0, 0.25, 0.75, 1);
  fv->SetMembership("High", 0.25, 0.75, 1.0, 1.0);


  FuzzyVarType::MembershipValueType memValues = fv->GetMembership(0.5);

  FuzzyVarType::PrintMembershipValueType(std::cout, memValues);

  if (memValues["Low"] != static_cast<PrecisionType>((0.75 - 0.5) / (0.75 - 0.25)))
    return EXIT_FAILURE;

  if (memValues["Medium"] != static_cast<PrecisionType>(1))
    return EXIT_FAILURE;

  if (memValues["High"] != static_cast<PrecisionType>((0.75 - 0.5) / (0.75 - 0.25)))
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
