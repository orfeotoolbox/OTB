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


#include "otbMassOfBelief.h"
#include "otbJointMassOfBeliefFilter.h"
#include "otbMath.h"

int otbJointMassOfBeliefFilter(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  typedef otb::MassOfBelief<std::string>                         MassOfBeliefFunctionType;
  typedef otb::JointMassOfBeliefFilter<MassOfBeliefFunctionType> JointMassOfBeliefFilterType;

  std::vector<double> baseline, results;

  baseline.push_back(0.);
  baseline.push_back(0.130725);
  baseline.push_back(0.346374);
  baseline.push_back(0.477099);
  baseline.push_back(0.522901);
  baseline.push_back(0.653626);
  baseline.push_back(0.869275);
  baseline.push_back(1.);

  MassOfBeliefFunctionType::Pointer    mass1           = MassOfBeliefFunctionType::New();
  MassOfBeliefFunctionType::Pointer    mass2           = MassOfBeliefFunctionType::New();
  JointMassOfBeliefFilterType::Pointer jointMassFilter = JointMassOfBeliefFilterType::New();

  MassOfBeliefFunctionType::LabelSetType universe;
  universe.insert("a");
  universe.insert("b");
  universe.insert("c");
  universe.insert("d");

  mass1->InitializePowerSetMasses(universe);
  mass2->InitializePowerSetMasses(universe);

  MassOfBeliefFunctionType::LabelSetType a, b, c, d, hyp;
  a.insert("a");
  b.insert("b");
  c.insert("c");
  d.insert("d");

  mass1->SetMass(a, 0.);
  mass1->SetMass(b, 0.8);
  mass1->EstimateUncertainty();

  mass2->SetMass(c, 0.);
  mass2->SetMass(d, 0.726);
  mass2->EstimateUncertainty();

  // Compute joint mass
  jointMassFilter->PushBackInput(mass1);
  jointMassFilter->PushBackInput(mass2);
  jointMassFilter->Update();

  MassOfBeliefFunctionType::Pointer jointMass = jointMassFilter->GetOutput();

  hyp.clear();
  hyp.insert("a");
  hyp.insert("c");
  results.push_back(jointMass->GetBelief(hyp));
  results.push_back(jointMass->GetPlausibility(hyp));

  hyp.clear();
  hyp.insert("a");
  hyp.insert("d");
  results.push_back(jointMass->GetBelief(hyp));
  results.push_back(jointMass->GetPlausibility(hyp));

  hyp.clear();
  hyp.insert("b");
  hyp.insert("c");
  results.push_back(jointMass->GetBelief(hyp));
  results.push_back(jointMass->GetPlausibility(hyp));

  hyp.clear();
  hyp.insert("b");
  hyp.insert("d");
  results.push_back(jointMass->GetBelief(hyp));
  results.push_back(jointMass->GetPlausibility(hyp));

  for (unsigned int i = 0; i < baseline.size(); ++i)
  {
    if (std::abs(baseline[i] - results[i]) >= .000001)
    {
      std::cout << "Non-regression test [" << i << "] failed: "
                << "baseline[i](" << baseline[i] << ") != results[i](" << results[i] << std::endl;

      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

int otbJointMassOfBeliefFilterLimit(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  typedef otb::MassOfBelief<std::string>                         MassOfBeliefFunctionType;
  typedef otb::JointMassOfBeliefFilter<MassOfBeliefFunctionType> JointMassOfBeliefFilterType;

  MassOfBeliefFunctionType::Pointer    mass1           = MassOfBeliefFunctionType::New();
  MassOfBeliefFunctionType::Pointer    mass2           = MassOfBeliefFunctionType::New();
  JointMassOfBeliefFilterType::Pointer jointMassFilter = JointMassOfBeliefFilterType::New();

  // Example from
  // http://www.sandia.gov/epistemic/Reports/SAND2002-0835.pdf
  // page 27

  MassOfBeliefFunctionType::LabelSetType universe;
  universe.insert("a");
  universe.insert("b");
  universe.insert("c");

  mass1->InitializePowerSetMasses(universe);
  mass2->InitializePowerSetMasses(universe);

  MassOfBeliefFunctionType::LabelSetType a, b, c;
  a.insert("a");
  b.insert("b");
  c.insert("c");

  // Mass1 is first expert
  mass1->SetMass(a, 0.99);
  mass1->SetMass(b, 0.01);

  // Mass2 is second expert
  mass2->SetMass(b, 0.01);
  mass2->SetMass(c, 0.99);

  // Compute joint mass
  jointMassFilter->PushBackInput(mass1);
  jointMassFilter->PushBackInput(mass2);
  jointMassFilter->Update();

  MassOfBeliefFunctionType::Pointer jointMass = jointMassFilter->GetOutput();

  std::cout << "Mass 1: " << std::endl;
  std::cout << mass1 << std::endl;
  std::cout << "Mass 2: " << std::endl;
  std::cout << mass2 << std::endl;
  std::cout << "Joint Mass: " << std::endl;
  std::cout << jointMass << std::endl;

  return EXIT_SUCCESS;
}
