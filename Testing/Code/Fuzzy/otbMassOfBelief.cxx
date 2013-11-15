/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "otbMassOfBelief.h"

typedef otb::MassOfBelief<std::string> MassOfBeliefFunctionType;

std::ostream& operator <<(std::ostream& os, const MassOfBeliefFunctionType::LabelSetType& l )
{
  MassOfBeliefFunctionType::PrintLabelSet(os, l);
  return os;
}

int otbMassOfBelief(int argc, char* argv[])
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

  std::cout<<"Removing mass from set " <<set4<<std::endl;

  massFunction->RemoveMass(set4);

  std::cout<<massFunction<<std::endl;

  std::cout<<"Estimating uncertainty "<<std::endl;
  massFunction->EstimateUncertainty();

  std::cout<<massFunction<<std::endl;

  std::cout<<"Removing mass "<<set2<<" and adding mass "<<set3<<std::endl;
  massFunction->RemoveMass(set2);
  massFunction->SetMass(set3, 0.6);

  std::cout<<massFunction<<std::endl;

  std::cout<<"Normalizing masses "<<std::endl;
  massFunction->Normalize();

  std::cout<<massFunction<<std::endl;

  std::cout<<"Belief of "<<set3<<" is "<<massFunction->GetBelief(set3)<<std::endl;
  std::cout<<"Plausibility of "<<set3<<" is "<<massFunction->GetPlausibility(set3)<<std::endl;

  MassOfBeliefFunctionType::LabelSetType otherSet;
  otherSet.insert("cat");
  otherSet.insert("dog");
  otherSet.insert("bird");

  std::cout<<"Initializing with power set from universal set " << otherSet<<std::endl;
  massFunction->InitializePowerSetMasses(otherSet);

  std::cout<<massFunction<<std::endl;

  return EXIT_SUCCESS;
}
