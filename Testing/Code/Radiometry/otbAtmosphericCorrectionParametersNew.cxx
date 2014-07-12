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


#include "itkMacro.h"
#include "otbAtmosphericCorrectionParameters.h"

#include <cstdlib>

int otbAtmosphericCorrectionParametersNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef otb::AtmosphericCorrectionParameters AtmosphericCorrectionParametersType;
  typedef otb::FilterFunctionValues            FilterFunctionValuesType;
  // Instantiating object
  AtmosphericCorrectionParametersType::Pointer objectAtmo = AtmosphericCorrectionParametersType::New();
  FilterFunctionValuesType::Pointer            objectFilter = FilterFunctionValuesType::New();

  std::cout << objectAtmo << std::endl;
  std::cout << objectFilter << std::endl;

  return EXIT_SUCCESS;
}
