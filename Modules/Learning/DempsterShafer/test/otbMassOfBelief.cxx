/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#include "otbMassOfBelief.h"
#include "otbMacro.h"

typedef otb::MassOfBelief<std::string> MassOfBeliefFunctionType;

int otbMassOfBelief(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  // Instantiate function
  MassOfBeliefFunctionType::Pointer massFunction = MassOfBeliefFunctionType::New();

  MassOfBeliefFunctionType::LabelSetType set1, set2, set3, set4;
  set1.insert("white");
  set1.insert("blue");
  set2.insert("white");
  set2.insert("green");
  set3.insert("red");
  set3.insert("green");
  set4.insert("white");
  set4.insert("green");
  set4.insert("red");

  massFunction->SetMass(set1, 0.25);
  massFunction->SetMass(set2, 0.3);
  massFunction->SetMass(set4, 0.1);

  otbLogMacro(Debug, << massFunction);

  std::ostringstream oss;
  MassOfBeliefFunctionType::PrintLabelSet(oss, set4);
  otbLogMacro(Info, << "Removing mass from set " << oss.str());

  massFunction->RemoveMass(set4);

  otbLogMacro(Debug, << massFunction);

  otbLogMacro(Info, << "Estimating uncertainty");
  massFunction->EstimateUncertainty();

  otbLogMacro(Debug, << massFunction);

  oss.str(std::string());
  oss << "Removing mass ";
  MassOfBeliefFunctionType::PrintLabelSet(oss, set2);
  oss << " and adding mass ";
  MassOfBeliefFunctionType::PrintLabelSet(oss, set3);
  otbLogMacro(Info, << oss.str());

  massFunction->RemoveMass(set2);
  massFunction->SetMass(set3, 0.6);

  otbLogMacro(Debug, << massFunction);

  otbLogMacro(Info, << "Normalizing masses ");
  massFunction->Normalize();

  otbLogMacro(Debug, << massFunction);

  oss.str(std::string());
  MassOfBeliefFunctionType::PrintLabelSet(oss, set3);
  otbLogMacro(Info, << "Belief of " << oss.str() << " is " << massFunction->GetBelief(set3));

  oss.str(std::string());
  MassOfBeliefFunctionType::PrintLabelSet(oss, set3);
  otbLogMacro(Info, << "Plausibility of " << oss.str() << " is " << massFunction->GetPlausibility(set3));

  MassOfBeliefFunctionType::LabelSetType otherSet;
  otherSet.insert("cat");
  otherSet.insert("dog");
  otherSet.insert("bird");

  oss.str(std::string());
  MassOfBeliefFunctionType::PrintLabelSet(oss, otherSet);
  otbLogMacro(Info, << "Initializing with power set from universal set " << oss.str());

  massFunction->InitializePowerSetMasses(otherSet);

  otbLogMacro(Debug, << massFunction);

  return EXIT_SUCCESS;
}
