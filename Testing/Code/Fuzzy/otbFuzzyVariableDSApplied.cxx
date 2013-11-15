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


#include "otbFuzzyVariable.h"

int otbFuzzyVariableDSApplied(int argc, char* argv[])
{
  typedef float PrecisionType;

  typedef otb::FuzzyVariable<std::string, PrecisionType> FuzzyVarType;

  FuzzyVarType::Pointer fv = FuzzyVarType::New();

  fv->SetMembership("H1", 0, 0, 0.1, 0.5, 0, 0.8);
  fv->SetMembership("!H1", 0.5, 0.8, 1.0, 1.0, 0, 0.8);
  fv->SetMembership("Theta", 0.1, 0.5, 0.5, 0.8, 0.2, 1);

  std::string maxVar;
  PrecisionType pos;
  PrecisionType memH1;
  PrecisionType memH1_;
  PrecisionType memTheta;


  for(unsigned int i=0; i<=100; ++i)
    {
    pos       = (PrecisionType)i/100;
    maxVar    = fv->GetMaxVar(pos);
    memH1     = fv->GetMembership("H1", pos);
    memH1_    = fv->GetMembership("!H1", pos);
    memTheta  = fv->GetMembership("Theta", pos);


    std::cout << "Memberships("
              << pos
              << "): ["
              << memH1
              << ", "
              << memH1_
              << ", "
              << memTheta
              <<"] - MaxVar("
              << pos
              << ") : "
              << maxVar
              << std::endl;
    }

  fv->RemoveMembership("H1");
  fv->RemoveMembership("!H1");
  fv->RemoveMembership("Theta");
  fv->Clear();

  return EXIT_SUCCESS;
}
