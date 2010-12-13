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

int otbFuzzyVariableGetMaxVar(int argc, char* argv[])
{
  typedef float PrecisionType;
  const unsigned int NumberOfValues = 4;
  typedef otb::FuzzyVariable<NumberOfValues, PrecisionType> FuzzyVarType;

  FuzzyVarType::Pointer fv = FuzzyVarType::New();

  enum TheValues { Low, MediumLow, MediumHigh, High };

  fv->SetMembership(Low, 0, 0, 0.25, 0.5);
  fv->SetMembership(MediumLow, 0, 0.2, 0.5, 0.6);
  fv->SetMembership(MediumHigh, 0.5, 0.6, 0.8, 1);
  fv->SetMembership(High, 0.5, 0.75, 1.0, 1.0);

  fv->SetValue( 0.4 );

  unsigned int maxVar = fv->GetMaxVar();

  if( maxVar != MediumLow )
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
