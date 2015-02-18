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

int otbFuzzyVariable2Values(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef float PrecisionType;

  typedef otb::FuzzyVariable<std::string, PrecisionType> FuzzyVarType;

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


  fv->SetMembership("Low", 0, 0, 0.3, 0.7);
  fv->SetMembership("High", 0.3, 0.7, 1.0, 1.0);

  PrecisionType mem;
  mem = fv->GetMembership("Low", 0.2);

  if( mem != 1 )
    return EXIT_FAILURE;

  mem = fv->GetMembership("Low", 0.9);

  if( mem != 0 )
    return EXIT_FAILURE;

  mem = fv->GetMembership("Low", 0.5);

  if( mem != 0.5 )
    return EXIT_FAILURE;

  mem = fv->GetMembership("High", 0.2);

  if( mem != 0 )
    return EXIT_FAILURE;

  mem = fv->GetMembership("High", 0.9);

  if( mem != 1 )
    return EXIT_FAILURE;

  mem = fv->GetMembership("High", 0.5);

  if( mem != 0.5 )
    return EXIT_FAILURE;

  // For the sake of coverage
  fv->RemoveMembership("Low");
  fv->Clear();

  return EXIT_SUCCESS;
}
