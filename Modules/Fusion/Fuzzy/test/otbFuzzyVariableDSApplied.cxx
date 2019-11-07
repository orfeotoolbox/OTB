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

int otbFuzzyVariableDSApplied(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  typedef float PrecisionType;

  typedef otb::FuzzyVariable<std::string, PrecisionType> FuzzyVarType;

  FuzzyVarType::Pointer fv = FuzzyVarType::New();

  fv->SetMembership("H1", 0, 0, 0.1, 0.5, 0, 0.8);
  fv->SetMembership("!H1", 0.5, 0.8, 1.0, 1.0, 0, 0.8);
  fv->SetMembership("Theta", 0.1, 0.5, 0.5, 0.8, 0.2, 1);

  std::string   maxVar;
  PrecisionType pos;
  PrecisionType memH1;
  PrecisionType memH1_;
  PrecisionType memTheta;


  for (unsigned int i = 0; i <= 100; ++i)
  {
    pos      = (PrecisionType)i / 100;
    maxVar   = fv->GetMaxVar(pos);
    memH1    = fv->GetMembership("H1", pos);
    memH1_   = fv->GetMembership("!H1", pos);
    memTheta = fv->GetMembership("Theta", pos);


    std::cout << "Memberships(" << pos << "): [" << memH1 << ", " << memH1_ << ", " << memTheta << "] - MaxVar(" << pos << ") : " << maxVar << std::endl;
  }

  fv->RemoveMembership("H1");
  fv->RemoveMembership("!H1");
  fv->RemoveMembership("Theta");
  fv->Clear();

  return EXIT_SUCCESS;
}
