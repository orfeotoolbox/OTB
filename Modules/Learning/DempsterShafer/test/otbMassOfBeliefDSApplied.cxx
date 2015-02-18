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

#include "otbFuzzyVariable.h"
#include "otbJointMassOfBeliefFilter.h"


typedef float                           PrecisionType;
typedef otb::FuzzyVariable<std::string, PrecisionType>
                                        FuzzyVarType;
typedef otb::MassOfBelief<std::string>  MassOfBeliefFunctionType;
typedef otb::JointMassOfBeliefFilter<MassOfBeliefFunctionType>
                                        JointMassOfBeliefFilterType;

int otbMassOfBeliefDSApplied(int itkNotUsed(argc), char* argv[])
{
  const char * hyp_1  = argv[1];
  const char * hyp_2  = argv[2];
  double desc1Val = atof(argv[3]);
  double desc2Val = atof(argv[4]);


  // Descriptors and associated fuzzy variables
  FuzzyVarType::Pointer desc1 = FuzzyVarType::New();
  FuzzyVarType::Pointer desc2 = FuzzyVarType::New();

  desc1->SetMembership("H1", 0, 0, 0.1, 0.5, 0, 0.8);
  desc1->SetMembership("H1_", 0.5, 0.8, 1.0, 1.0, 0, 0.8);

  desc2->SetMembership("H2", 0, 0, 0.58, 0.68, 0, 0.99);
  desc2->SetMembership("H2_", 0.68, 0.98, 1.0, 1.0, 0, 0.99);

  // Corresponding masses
  MassOfBeliefFunctionType::Pointer mass1 = MassOfBeliefFunctionType::New();
  MassOfBeliefFunctionType::Pointer mass2 = MassOfBeliefFunctionType::New();
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

  std::cout<<mass1<<std::endl;

  std::cout<<mass2<<std::endl;

  std::cout << jointMass << std::endl;
  
  std::cout << "Considered Hypothesis : ";
  MassOfBeliefFunctionType::PrintLabelSet(std::cout, Hyp);
  std::cout << std::endl;
  
  std::cout << "Belief(Hyp) : "
            << jointMass->GetBelief(Hyp)
            << "  -  Plausibility(Hyp) : "
            << jointMass->GetPlausibility(Hyp)
            << "  -  Score(Hyp) : "
            << (jointMass->GetBelief(Hyp) + jointMass->GetPlausibility(Hyp))/2.0
            << std::endl;

  if (jointMass->GetBelief(Hyp) > jointMass->GetPlausibility(Hyp))
    {
    std::cout << "Belief > Plausibility" << std::endl;
    return EXIT_FAILURE;
    }
  else
    {
    return EXIT_SUCCESS;
    }
}
