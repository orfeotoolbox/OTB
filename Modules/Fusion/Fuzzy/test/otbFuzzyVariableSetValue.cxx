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

int otbFuzzyVariableSetValue(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef float PrecisionType;
  typedef otb::FuzzyVariable<std::string, PrecisionType> FuzzyVarType;

  FuzzyVarType::Pointer fv = FuzzyVarType::New();

  fv->SetMembership("Low", 0, 0, 0.25, 0.75);
  fv->SetMembership("Medium", 0, 0.25, 0.75, 1);
  fv->SetMembership("High", 0.25, 0.75, 1.0, 1.0);


  FuzzyVarType::MembershipValueType memValues = fv->GetMembership(0.5);

  FuzzyVarType::PrintMembershipValueType(std::cout, memValues);

  if( memValues["Low"] != static_cast<PrecisionType>((0.75-0.5)/(0.75-0.25)) )
    return EXIT_FAILURE;

  if( memValues["Medium"] != static_cast<PrecisionType>(1) )
    return EXIT_FAILURE;

  if( memValues["High"] != static_cast<PrecisionType>((0.75-0.5)/(0.75-0.25)) )
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
