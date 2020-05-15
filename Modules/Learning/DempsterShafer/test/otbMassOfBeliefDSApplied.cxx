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

#include "otbFuzzyVariable.h"
#include "otbJointMassOfBeliefFilter.h"
#include "otbMacro.h"


typedef float PrecisionType;
typedef otb::FuzzyVariable<std::string, PrecisionType> FuzzyVarType;
typedef otb::MassOfBelief<std::string>                         MassOfBeliefFunctionType;
typedef otb::JointMassOfBeliefFilter<MassOfBeliefFunctionType> JointMassOfBeliefFilterType;

int otbMassOfBeliefDSApplied(int itkNotUsed(argc), char* argv[])
{
  const char* hyp_1    = argv[1];
  const char* hyp_2    = argv[2];
  double      desc1Val = atof(argv[3]);
  double      desc2Val = atof(argv[4]);


  // Descriptors and associated fuzzy variables
  FuzzyVarType::Pointer desc1 = FuzzyVarType::New();
  FuzzyVarType::Pointer desc2 = FuzzyVarType::New();

  desc1->SetMembership("H1", 0, 0, 0.1, 0.5, 0, 0.8);
  desc1->SetMembership("H1_", 0.5, 0.8, 1.0, 1.0, 0, 0.8);

  desc2->SetMembership("H2", 0, 0, 0.58, 0.68, 0, 0.99);
  desc2->SetMembership("H2_", 0.68, 0.98, 1.0, 1.0, 0, 0.99);

  // Corresponding masses
  MassOfBeliefFunctionType::Pointer mass1     = MassOfBeliefFunctionType::New();
  MassOfBeliefFunctionType::Pointer mass2     = MassOfBeliefFunctionType::New();
  MassOfBeliefFunctionType::Pointer jointMass = MassOfBeliefFunctionType::New();

  MassOfBeliefFunctionType::LabelSetType H1, H1_, H2, H2_, universe, Hyp;
  // Defining universe
  universe.insert("H1");
  universe.insert("H1_");
  universe.insert("H2");
  universe.insert("H2_");

  // Studied hypothesis
  Hyp.insert(hyp_1);
  Hyp.insert(hyp_2);

  // Initialize masses
  mass1->InitializePowerSetMasses(universe);
  mass2->InitializePowerSetMasses(universe);

  H1.insert("H1");
  H1_.insert("H1_");
  mass1->SetMass(H1, desc1->GetMembership("H1", desc1Val));
  mass1->SetMass(H1_, desc1->GetMembership("H1_", desc1Val));
  mass1->EstimateUncertainty();

  H2.insert("H2");
  H2_.insert("H2_");
  mass2->SetMass(H2, desc2->GetMembership("H2", desc2Val));
  mass2->SetMass(H2_, desc2->GetMembership("H2_", desc2Val));
  mass2->EstimateUncertainty();

  JointMassOfBeliefFilterType::Pointer jointMassFilter = JointMassOfBeliefFilterType::New();
  // Compute joint mass
  jointMassFilter->PushBackInput(mass1);
  jointMassFilter->PushBackInput(mass2);
  jointMassFilter->Update();
  jointMass = jointMassFilter->GetOutput();

  otbLogMacro(Debug, << "Mass 1:" << mass1);

  otbLogMacro(Debug, << "Mass 2:" << mass2);

  otbLogMacro(Debug, << "Joint mass :" << jointMass);

  std::ostringstream oss;
  MassOfBeliefFunctionType::PrintLabelSet(oss, Hyp);
  otbLogMacro(Info, << "Considered Hypothesis : " << oss.str());

  otbLogMacro(Info, << "Belief(Hyp) : " << jointMass->GetBelief(Hyp));
  otbLogMacro(Info, << "Plausibility(Hyp) : " << jointMass->GetPlausibility(Hyp));
  otbLogMacro(Info, << "Score(Hyp) : " << (jointMass->GetBelief(Hyp) + jointMass->GetPlausibility(Hyp)) / 2.0);

  if (jointMass->GetBelief(Hyp) > jointMass->GetPlausibility(Hyp))
  {
    otbLogMacro(Warning, << "Belief > Plausibility");
    return EXIT_FAILURE;
  }
  else
  {
    return EXIT_SUCCESS;
  }
}
