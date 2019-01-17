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

  std::cout<<massFunction<<std::endl;

  std::cout<<"Removing mass from set ";
  MassOfBeliefFunctionType::PrintLabelSet(std::cout, set4);
  std::cout<<std::endl;

  massFunction->RemoveMass(set4);

  std::cout<<massFunction<<std::endl;

  std::cout<<"Estimating uncertainty "<<std::endl;
  massFunction->EstimateUncertainty();

  std::cout<<massFunction<<std::endl;

  std::cout<<"Removing mass ";
  MassOfBeliefFunctionType::PrintLabelSet(std::cout, set2);
  std::cout<<" and adding mass ";
  MassOfBeliefFunctionType::PrintLabelSet(std::cout, set3);
  std::cout<<std::endl;
  massFunction->RemoveMass(set2);
  massFunction->SetMass(set3, 0.6);

  std::cout<<massFunction<<std::endl;

  std::cout<<"Normalizing masses "<<std::endl;
  massFunction->Normalize();

  std::cout<<massFunction<<std::endl;

  std::cout<<"Belief of ";
  MassOfBeliefFunctionType::PrintLabelSet(std::cout, set3);
  std::cout<<" is "<<massFunction->GetBelief(set3)<<std::endl;
  
  std::cout<<"Plausibility of ";
  MassOfBeliefFunctionType::PrintLabelSet(std::cout, set3);
  std::cout<<" is "<<massFunction->GetPlausibility(set3)<<std::endl;

  MassOfBeliefFunctionType::LabelSetType otherSet;
  otherSet.insert("cat");
  otherSet.insert("dog");
  otherSet.insert("bird");

  std::cout<<"Initializing with power set from universal set ";
  MassOfBeliefFunctionType::PrintLabelSet(std::cout, otherSet);
  std::cout<<std::endl;
  
  massFunction->InitializePowerSetMasses(otherSet);

  std::cout<<massFunction<<std::endl;

  return EXIT_SUCCESS;
}
