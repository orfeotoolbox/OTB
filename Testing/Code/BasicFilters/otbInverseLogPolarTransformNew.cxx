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
#include "otbInverseLogPolarTransform.h"
#include <cstdlib>

int otbInverseLogPolarTransformNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef double                                       PrecisionType;
  typedef otb::InverseLogPolarTransform<PrecisionType> InverseLogPolarTransformType;

  // Instantiation
  InverseLogPolarTransformType::Pointer transform = InverseLogPolarTransformType::New();

  std::cout << transform << std::endl;

  return EXIT_SUCCESS;
}
