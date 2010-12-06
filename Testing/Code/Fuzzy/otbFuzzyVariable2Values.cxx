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

int otbFuzzyVariable2Values(int argc, char* argv[])
{
  typedef float PrecisionType;
  const unsigned int NumberOfValues = 2;
  typedef otb::FuzzyVariable<NumberOfValues, PrecisionType> FuzzyVarType;

  FuzzyVarType::Pointer fv = FuzzyVarType::New();

  /*
   We define 2 trapezoidal functions for Low and High values
       |
     1 |--------------\     -----------------
       |               \   /
       |                \ /
       |   Low           X      High
       |                / \
       |               /   \
       |--------------/-------------------------------
       0             0.3   0.7              1
   */

  enum TheValues { Low, High };

  fv->SetMembership(Low, 0, 0, 0.3, 0.7);
  fv->SetMembership(High, 0.3, 0.7, 1.0, 1.0);

  PrecisionType mem;
  mem = fv->GetMembership(Low, 0.2);

  if( mem != 1 )
    return EXIT_FAILURE;

  mem = fv->GetMembership(Low, 0.9);

  if( mem != 0 )
    return EXIT_FAILURE;

  mem = fv->GetMembership(Low, 0.5);

  if( mem != 0.5 )
    return EXIT_FAILURE;

  mem = fv->GetMembership(High, 0.2);

  if( mem != 0 )
    return EXIT_FAILURE;

  mem = fv->GetMembership(High, 0.9);

  if( mem != 1 )
    return EXIT_FAILURE;

  mem = fv->GetMembership(High, 0.5);

  if( mem != 0.5 )
    return EXIT_FAILURE;


  
  return EXIT_SUCCESS;
}
