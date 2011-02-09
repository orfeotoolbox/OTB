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
#include "otbJointMassOfBeliefFilter.h"


int otbJointMassOfBeliefFilter(int argc, char* argv[])
{
  typedef otb::MassOfBelief<std::string> MassOfBeliefFunctionType;
  typedef otb::JointMassOfBeliefFilter<MassOfBeliefFunctionType> JointMassOfBeliefFilterType;
  
  MassOfBeliefFunctionType::Pointer mass1 = MassOfBeliefFunctionType::New();
  MassOfBeliefFunctionType::Pointer mass2 = MassOfBeliefFunctionType::New();
  JointMassOfBeliefFilterType::Pointer jointMassFilter = JointMassOfBeliefFilterType::New();
  
  // Exemple from
  // http://www.sandia.gov/epistemic/Reports/SAND2002-0835.pdf
  // page 27

  MassOfBeliefFunctionType::LabelSetType universe;
  universe.insert("a");
  universe.insert("b");
  universe.insert("c");
  
  mass1->InitializePowerSetMasses(universe);
  mass2->InitializePowerSetMasses(universe);

  MassOfBeliefFunctionType::LabelSetType a,b,c;
  a.insert("a");
  b.insert("b");
  c.insert("c");

  // Mass1 is first expert
  mass1->SetMass(a,0.99);
  mass1->SetMass(b,0.01);

  // Mass2 is second expert
  mass2->SetMass(b,0.01);
  mass2->SetMass(c,0.99);

  // Compute joint mass
  jointMassFilter->PushBackInput(mass1);
  jointMassFilter->PushBackInput(mass2);
  jointMassFilter->Update();

  MassOfBeliefFunctionType::Pointer jointMass = jointMassFilter->GetOutput();

  std::cout<<"Mass 1: "<<std::endl;
  std::cout<<mass1<<std::endl;
  std::cout<<"Mass 2: "<<std::endl;
  std::cout<<mass2<<std::endl;
  std::cout<<"Joint Mass: "<<std::endl;
  std::cout<<jointMass<<std::endl;

  return EXIT_SUCCESS;
}
