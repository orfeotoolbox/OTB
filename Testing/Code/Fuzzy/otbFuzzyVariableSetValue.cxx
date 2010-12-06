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

#include "otbFuzzyVariable.h"

int otbFuzzyVariableSetValue(int argc, char* argv[])
{
  typedef float PrecisionType;
  const unsigned int NumberOfValues = 3;
  typedef otb::FuzzyVariable<NumberOfValues, PrecisionType> FuzzyVarType;

  FuzzyVarType::Pointer fv = FuzzyVarType::New();

  enum TheValues { Low, Medium, High };

  fv->SetMembership(Low, 0, 0, 0.25, 0.75);
  fv->SetMembership(Medium, 0, 0.25, 0.75, 1);
  fv->SetMembership(High, 0.25, 0.75, 1.0, 1.0);

  fv->SetValue( 0.5 );

  FuzzyVarType::ValueVectorType memValues = fv->GetMembershipValues();

  std::cout << memValues << std::endl;

  if( memValues[Low] != static_cast<PrecisionType>((0.75-0.5)/(0.75-0.25)) )
    return EXIT_FAILURE;

  if( memValues[Medium] != static_cast<PrecisionType>(1) )
    return EXIT_FAILURE;

  if( memValues[High] != static_cast<PrecisionType>((0.75-0.5)/(0.75-0.25)) )
    return EXIT_FAILURE;
  
  return EXIT_SUCCESS;
}
