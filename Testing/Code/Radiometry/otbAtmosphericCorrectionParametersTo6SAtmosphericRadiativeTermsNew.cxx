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

#include "otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms.h"
#include <cstdlib>

int otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTermsNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef otb::AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
  AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTermsType;
  // Instantiating object
  AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTermsType::Pointer object =
    AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTermsType::New();

  std::cout << object << std::endl;

  return EXIT_SUCCESS;
}
