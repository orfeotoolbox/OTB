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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbMassOfBelief.h"

#include "otbFuzzyVariable.h"
#include "otbJointMassOfBeliefFilter.h"

int otbMassOfBeliefDSApplied(int argc, char* argv[])
{
  typedef float                           PrecisionType;
  typedef otb::FuzzyVariable<std::string,
    PrecisionType>                        FuzzyVarType;
  
  typedef otb::MassOfBelief<std::string>  MassOfBeliefFunctionType;

  typedef otb::JointMassOfBeliefFilter<MassOfBeliefFunctionType> JointMassOfBeliefFilterType;

  FuzzyVarType::Pointer desc1 = FuzzyVarType::New();
  FuzzyVarType::Pointer desc2 = FuzzyVarType::New();

  desc1->SetMembership("H1", 0, 0, 0.1, 0.5, 0, 0.8);
  desc1->SetMembership("H1_", 0.5, 0.8, 1.0, 1.0, 0, 0.8);
  
  desc2->SetMembership("H2", 0, 0, 0.58, 0.68, 0, 0.99);
  desc2->SetMembership("H2_", 0.68, 0.98, 1.0, 1.0, 0, 0.99);

  PrecisionType desc1Val = 0.9;
  PrecisionType desc2Val = 0.9;

  MassOfBeliefFunctionType::Pointer mass1 = MassOfBeliefFunctionType::New();
  MassOfBeliefFunctionType::Pointer mass2 = MassOfBeliefFunctionType::New();
  MassOfBeliefFunctionType::Pointer jointMass = MassOfBeliefFunctionType::New();

  MassOfBeliefFunctionType::LabelSetType H1, H1_, H2, H2_, universe, Hyp;
  universe.insert("H1");
  universe.insert("H1_");
  universe.insert("H2");
  universe.insert("H2_");

  Hyp.insert("H1");
  Hyp.insert("H2_");
  
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

  std::cout<<mass1<<std::endl;
  std::cout << "["
            << desc1->GetMembership("H1", desc1Val)
            << ","
            << desc1->GetMembership("H1_", desc1Val)
            << "]"
            << std::endl;

  std::cout<<mass2<<std::endl;
  std::cout << "["
            << desc2->GetMembership("H2", desc2Val)
            << ","
            << desc2->GetMembership("H2_", desc2Val)
            << "]"
            << std::endl;

  std::cout << jointMass << std::endl;
  std::cout << "Belief(Hyp) : "
            << jointMass->GetBelief(Hyp)
            << "  -  Plausibility(Hyp) : "
            << jointMass->GetPlausibility(Hyp)
            << "  -  Score(Hyp) : "
            << (jointMass->GetBelief(Hyp) + jointMass->GetPlausibility(Hyp))/2.0
            << std::endl;
  
  return EXIT_SUCCESS;
}
  
  
